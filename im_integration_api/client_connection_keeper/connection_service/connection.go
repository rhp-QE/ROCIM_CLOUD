package connection_service

import (
	"context"
	"fmt"
	"log"
	"time"

	"github.com/cloudwego/netpoll"
	"google.golang.org/protobuf/proto"
	imcloud_pb "roc.com/imcloud/integration_api/client_connection_keeper/pb"
	"roc.com/imcloud/integration_api/client_connection_keeper/util"
)

type Config struct {
	NetWork string
	Port    string
}

// ----------- interface begin ----------------
type LongConnectionService interface {
	// 设置配置信息
	SetConfig(config Config)

	// 运行
	Run()
}

func CreateLCService() LongConnectionService {
	return &longConnectionService{}
}

// ------- impl ------------

type longConnectionService struct {
	config   Config
	liveCons []netpoll.Connection
}

// ----------------- public method begin ---------------------

func (conService *longConnectionService) SetConfig(config Config) {
	conService.config = config
}

func (conService *longConnectionService) Run() {
	// 1、创建 listener
	listener, err := netpoll.CreateListener(conService.config.NetWork, conService.config.Port)
	if err != nil {
		log.Fatal("Failed to create listener:", err)
	}

	handleData := func(ctx context.Context, conn netpoll.Connection) error {
		return conService.handleData(ctx, conn)
	}

	onConnect := func(ctx context.Context, conn netpoll.Connection) context.Context {
		return conService.onConnect(ctx, conn)
	}

	onDisconnect := func(ctx context.Context, conn netpoll.Connection) {
		conService.conDisconnect(ctx, conn)
	}

	// 2. 初始化 EventLoop
	eventLoop, _ := netpoll.NewEventLoop(
		handleData,
		netpoll.WithOnPrepare(onPrepare),
		netpoll.WithOnConnect(onConnect),
		netpoll.WithOnDisconnect(onDisconnect),
	)

	// 3. 启动服务
	log.Println("Server started on :", conService.config.Port)
	err = eventLoop.Serve(listener)
	if err != nil {
		log.Fatal("Failed to start server:", err)
	}
}

// ----------------- private method begin ---------------------

// 处理连接建立前的准备工作（如设置超时）
func onPrepare(conn netpoll.Connection) context.Context {
	conn.SetReadTimeout(30 * time.Second) // 设置读超时
	return context.Background()
}

// 处理新连接
func (conService *longConnectionService) onConnect(ctx context.Context, conn netpoll.Connection) context.Context {
	log.Println("New connection:", conn.RemoteAddr())

	// 记录连接
	conService.liveCons = append(conService.liveCons, conn)

	return ctx
}

// 处理接收到的数据
func (conService *longConnectionService) handleData(ctx context.Context, conn netpoll.Connection) error {
	pkgLenReader, _ := conn.Reader().Slice(4)
	defer pkgLenReader.Release()

	// 获取 数据包
	pkgLen := int(peekUint32(pkgLenReader))
	pkgReader, _ := conn.Reader().Slice(pkgLen)
	defer pkgReader.Release()

	println("pkglen", pkgLen)

	// 处理 数据包
	reqData, _ := pkgReader.ReadBinary(pkgLen)
	req := &imcloud_pb.RequestBody{}
	if err := proto.Unmarshal(reqData, req); err != nil {
		fmt.Println("[Unmarshal error]")
	}

	resp := &imcloud_pb.ResponseBody{
		ResponseId: req.RequestId,
	}

	writer := conn.Writer()

	respLen := proto.Size(resp)
	respLenData := make([]byte, 4)
	util.EncodeUint32LE(uint32(respLen), respLenData)

	respData, _ := proto.Marshal(resp)

	writer.WriteBinary(respLenData)
	writer.WriteBinary(respData)
	writer.Flush()

	return nil
}

// 处理连接断开
func (conService *longConnectionService) conDisconnect(ctx context.Context, conn netpoll.Connection) {
	log.Println("Connection closed:", conn.RemoteAddr())
}

func peekUint32(reader netpoll.Reader) uint32 {
	buf, err := reader.Peek(4)
	if err != nil {
		fmt.Printf("[peekUint32 error] : ", err)
	}

	number, _ := util.DecodeUint32LE(buf)
	return number
}

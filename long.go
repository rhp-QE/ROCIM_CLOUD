package main

import (
	"context"
	"log"
	"syscall"
	"time"

	"github.com/cloudwego/netpoll"
)

func LongTest() {
	// 1. 创建 Listener
	listener, err := netpoll.CreateListener("tcp", ":8282")
	syscall.SetsockoptInt(listener.Fd(), syscall.SOL_SOCKET, syscall.SO_REUSEADDR, 1)
	if err != nil {
		log.Fatal("Failed to create listener:", err)
	}

	// 2. 初始化 EventLoop
	eventLoop, _ := netpoll.NewEventLoop(
		// 定义处理逻辑
		handleData,
		netpoll.WithOnPrepare(onPrepare),
		netpoll.WithOnConnect(onConnect),
		netpoll.WithOnDisconnect(onDisconnect),
	)

	// 3. 启动服务
	log.Println("Server started on :8282")
	err = eventLoop.Serve(listener)
	if err != nil {
		log.Fatal("Failed to start server:", err)
	}
}

// 处理连接建立前的准备工作（如设置超时）
func onPrepare(conn netpoll.Connection) context.Context {
	conn.SetReadTimeout(30 * time.Second) // 设置读超时
	return context.Background()
}

// 处理新连接
func onConnect(ctx context.Context, conn netpoll.Connection) context.Context {
	log.Println("New connection:", conn.RemoteAddr())
	return ctx
}

// 处理数据读取和业务逻辑
func handleData(ctx context.Context, conn netpoll.Connection) error {
	// 读取数据
	reader := conn.Reader()
	len := reader.Len()
	data, _ := reader.ReadString(reader.Len())
	log.Printf("Received: %d", len)

	// 业务处理（示例：回显数据）
	resp := []byte(data)

	// 写入响应
	writer := conn.Writer()
	writer.WriteBinary(resp)
	return writer.Flush()
}

// 处理连接断开
func onDisconnect(ctx context.Context, conn netpoll.Connection) {
	log.Println("Connection closed:", conn.RemoteAddr())
}

package main

import (
	"context"

	"github.com/cloudwego/hertz/pkg/app"
	"github.com/cloudwego/hertz/pkg/app/server"
	"github.com/cloudwego/hertz/pkg/common/utils"
	"github.com/cloudwego/hertz/pkg/protocol/consts"
)

func HttpTest() {
	// 初始化 Hertz 服务端，监听 8080 端口
	h := server.Default(server.WithHostPorts(":8080"))

	// 注册 GET 路由
	h.GET("/hello", func(c context.Context, ctx *app.RequestContext) {
		ctx.JSON(consts.StatusOK, utils.H{
			"message": "Hello, Hertz!",
		})
	})

	// 注册 POST 路由
	h.POST("/echo", func(c context.Context, ctx *app.RequestContext) {
		// 读取请求体
		body := ctx.Request.Body()
		ctx.Data(consts.StatusOK, "text/plain", body)
	})

	// 启动服务
	h.Spin()
}

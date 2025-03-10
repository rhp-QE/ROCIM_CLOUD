package main

import (
	"roc.com/imcloud/integration_api/client_connection_keeper/connection_service"
)

func main() {
	LCService := connection_service.CreateLCService()
	LCService.SetConfig(connection_service.Config{
		NetWork: "tcp",
		Port:    ":6080",
	})
	LCService.Run()
}

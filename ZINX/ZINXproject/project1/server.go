package main

import "ZINX_Framework/ZNet"

//基于zinx框架开发的服务器

func main() {
	//创建一个server句柄
	s := ZNet.CreateServer("ZinxV0.1")

	s.Server()
}

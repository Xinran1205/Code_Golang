package main

func main() {
	//127.0.0.1是一个保留地址，专门用于在本机上进行网络通信。在同一台计算机上运行的不同操作系统（例如Windows和Ubuntu）被视为不同的机器，
	//因此在Windows系统上绑定127.0.0.1地址和端口号后，只能接受来自该Windows系统的连接，而不能接受来自Ubuntu系统的连接。
	//所以我如果用goLand运行，他实际上就是在windows上面启动的，只能用windows对他进行测试
	//所以我改用ubuntu对他进行编译go build -o server server.go main.go，
	//这样我们运行可执行文件server，我们就可以用ubuntu对他进行测试（nc指令）
	server := NewServer("127.0.0.1", 8888)
	server.Start()
}

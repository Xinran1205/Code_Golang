package main

import (
	"fmt"
	"net"
)

type Server struct {
	Ip   string
	Port int
}

// 创建一个server，方法名大写表示接口对外开放
func NewServer(ip string, port int) *Server {
	server := &Server{Ip: ip, Port: port}
	return server
}

func (s *Server) Handler(conn net.Conn) {
	// 当前链接的业务
	fmt.Println("链接建立成功")
}

// 启动服务器
func (s *Server) Start() {
	//socket listen,创建监听器
	listner, err := net.Listen("tcp", fmt.Sprintf("%s:%d", s.Ip, s.Port))
	if err != nil {
		fmt.Println("net.Listen err:", err)
		return
	}
	//close listen socket,这个是函数结束后才关闭（defer）
	defer listner.Close()

	for {
		//服务器在调用accept函数之后，会阻塞等待客户端的连接请求，一旦有客户端连接请求到达，
		//accept函数会返回已连接套接字的描述符，服务器就可以使用该已连接套接字与客户端进行通信。

		//服务器在调用accept函数时，会创建一个新的已连接套接字来处理客户端的请求。并保留原始的套接字用于继续接受其它客户端的连接请求。
		//这样就可以同时处理多个客户端的连接请求，并与它们进行通信，而不会影响其它已经建立的连接。
		conn, err := listner.Accept()

		fmt.Println("listner.Accept")
		if err != nil {
			fmt.Println("listner.Accept err:", err)
			continue
		}
		//do handler
		go s.Handler(conn)
	}

}

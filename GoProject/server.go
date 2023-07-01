package main

import (
	"fmt"
	"io"
	"net"
	"sync"
	"time"
)

type Server struct {
	Ip   string
	Port int
	// 在线用户列表,key是用户名，value是用户的对象
	OnlineMap map[string]*User
	//需要一个锁
	mapLock sync.RWMutex

	msg chan string
}

// 创建一个server（一个server对象），方法名大写表示接口对外开放
func NewServer(ip string, port int) *Server {
	server := &Server{Ip: ip, Port: port, OnlineMap: make(map[string]*User), msg: make(chan string)}
	//返回对象的地址，所以用指针接收
	return server
}

func (s *Server) SevListenMessage() {
	for {
		msg := <-s.msg
		//将msg发送给全部的在线用户
		s.mapLock.Lock()
		for _, cli := range s.OnlineMap {
			cli.C <- msg
		}
		s.mapLock.Unlock()
	}
}

// 把当前用户上线消息广播给所有用户，其实真的广播是ListenMessage()这个函数做的
// 这个只是把信息发给了服务器
func (s *Server) BroadCast(user *User, msg string) {
	sendMsg := "[" + user.Addr + "]" + user.Name + ":" + msg
	s.msg <- sendMsg
}

func (s *Server) Handler(conn net.Conn) {
	fmt.Println("connection successful")
	user := NewUser(conn, s)
	// 当前链接的业务
	//用户上线，将用户加入到onlineMap中
	user.UserOnline()
	isLive := make(chan bool)
	go func() {
		buf := make([]byte, 4096)
		for {
			n, err := conn.Read(buf)
			if n == 0 {
				user.UserOffline()
				return
			}
			if err != nil && err != io.EOF {
				fmt.Println("conn read err", err)
				return
			}
			msg := string(buf[:n-1])
			user.SendMsg(msg)
			isLive <- true
		}
	}()

	for {
		//select中case先后顺序没有影响
		select {
		case <-isLive:
			//当前用户是活跃的，应该重置定时器
			//不做任何事情，为了激活select，使得select退出做for循环以至于重置下面的定时器

		case <-time.After(time.Second * 15):
			//超时处理
			user.ToCurCli("你被踢了")
			//销毁资源
			close(user.C)
			//关闭连接
			conn.Close()

			return
		}
	}
}

// 启动服务器
func (s *Server) Start() {
	//在Go语言中，当你调用net.Listen()函数时，它会自动创建一个套接字并将其绑定到指定的本地地址，这个过程被称为"隐式绑定"。
	//这个过程会自动完成，因此你不需要显式地调用bind()函数。
	//在Go语言中，net.Listen()函数返回一个net.Listener接口类型的对象，用于接受连接请求并返回一个net.Conn接口类型的对象来处理连接。
	//当你需要停止侦听器时，只需要调用listener.Close()函数即可关闭侦听器。
	listner, err := net.Listen("tcp", fmt.Sprintf("%s:%d", s.Ip, s.Port))
	if err != nil {
		fmt.Println("net.Listen err:", err)
		return
	}
	//close listen socket,这个是函数结束后才关闭（defer）
	defer listner.Close()

	//启动监听Message的goroutine
	go s.SevListenMessage()

	for {
		//服务器在调用accept函数时，会阻塞等待客户端的连接请求，一旦有客户端连接请求到达，
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

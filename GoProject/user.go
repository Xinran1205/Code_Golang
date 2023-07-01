package main

import (
	"net"
)

type User struct {
	Name string
	Addr string
	C    chan string
	conn net.Conn
	s    *Server
}

// 创建一个用户的API
func NewUser(conn net.Conn, server *Server) *User {
	userAddr := conn.RemoteAddr().String()
	user := &User{
		Name: userAddr,
		Addr: userAddr,
		C:    make(chan string),
		conn: conn,
		s:    server,
	}

	//在 Go 语言中，使用 go 关键字启动的 goroutine 是一个独立的执行线程，
	//它 会在后台运行，不会阻塞当前的程序流程。当一个函数返回时，该函数中启动的 goroutine 会继续运行，直到该 goroutine 执行完毕或者程序退出。
	//其实下面这句和创建用户没关系，他的作用是后面当我们user收到了消息，他就可以直接发给这个user对应的客户端
	//启动监听当前user channel消息的goroutine
	go user.ListenMessage()

	return user
}

func (u *User) UserOnline() {
	u.s.mapLock.Lock()
	u.s.OnlineMap[u.Name] = u
	u.s.mapLock.Unlock()
	u.s.BroadCast(u, "已上线")
}

func (u *User) UserOffline() {
	u.s.mapLock.Lock()
	delete(u.s.OnlineMap, u.Name)
	u.s.mapLock.Unlock()
	u.s.BroadCast(u, "已下线")
}

func (u *User) SendMsg(msg string) {
	u.s.BroadCast(u, msg)
}

// 监听当前user channel的方法，一旦有消息，就直接发送给对端客户端，注意是客户端，因为这个类还是属于服务器的部分
func (u *User) ListenMessage() {
	for {
		msg := <-u.C

		u.conn.Write([]byte(msg + "\n"))
	}
}

package ZNet

//这个zinx框架必须放在gopath下面
//在$GOPATH/src下创建ZINX_Framework文件夹

import "ZINX/ZINX_Framework/ZInterface"

type Server struct {
	Name      string
	IPVersion string
	IP        string
	Port      int
}

func (s *Server) StartServer() {
	panic("implement me")
}

func (s *Server) StopServer() {

}

func (s *Server) RunServer() {
	panic("implement me")
}

//初始化Server模块的方法

func CreateServer(name string) ZInterface.IServer {
	s := &Server{
		Name:      name,
		IPVersion: "tcp4",
		IP:        "0.0.0.0",
		Port:      8999,
	}
	return s
}

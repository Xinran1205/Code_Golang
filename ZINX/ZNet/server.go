package ZNet

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

func (s *Server) RunServe() {
	panic("implement me")
}

package ZInterface

type IServer interface {
	// Start server
	StartServer()
	// Stop server
	StopServer()
	// Run server
	RunServer()
}

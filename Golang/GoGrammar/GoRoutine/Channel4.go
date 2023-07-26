package main

import "fmt"

// select具备多路channel的监控状态功能
func fibonacci(c, quit chan int) {
	x, y := 1, 1
	for {
		select {
		case c <- x: //当c可写时，执行下面内容，c是无缓冲的channel，只有在main中执行<-c时，才可写，
			// 想想无缓冲channel是怎么通信的，左右两个人都准备好才行，否则阻塞
			x, y = y, x+y
		case <-quit: //当quit可读时，执行下面内容，quit本来是空的，不可读，只有在main中执行quit <- 0时，才可读
			fmt.Println("quit")
			return
		}
	}
}
func main() {
	c := make(chan int)
	quit := make(chan int)

	go func() {
		for i := 0; i < 10; i++ {
			//子进程执行到这里，阻塞，等待main进程调用函数fibonacci在里面c <- x
			fmt.Println(<-c)
		}
		quit <- 0
	}()

	fibonacci(c, quit)
}

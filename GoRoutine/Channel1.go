package main

//无缓冲通道练习
import "fmt"

func main() {
	//定义一个无缓冲channel
	c := make(chan int)

	go func() {
		defer fmt.Println("子goroutine结束")
		fmt.Println("子goroutine正在运行")

		//往channel中写数据，如果没有人读，就会阻塞
		c <- 666
	}()
	//阻塞概念非常重要，这就无形中提供了一种同步
	//所以，理解一下这句话：defer fmt.Println("子协程结束")一定是在num := <-c结束之后才执行
	num := <-c //从channel中读取数据，如果没有人写，就会阻塞

	//也可以这么写
	// <-c //这么写c的值就被丢弃了，但是同样如果没有人写，就会阻塞
	fmt.Println("num = ", num)
	fmt.Println("main结束")
}

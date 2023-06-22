package main

//有缓冲管道练习
import (
	"fmt"
	"time"
)

func main() {
	//定义一个有缓冲区的channel，缓冲区大小为3
	//有缓冲的channel阻塞条件是什么：
	//1.缓冲区满了（再写就阻塞） 2.缓冲区空了（再读就阻塞）
	//注意，这两个前提都是channel没有关闭
	c := make(chan int, 3)

	//获取当前channel中元素数量和容量
	fmt.Println("len(c) = ", len(c), "cap(c) = ", cap(c))

	go func() {
		defer fmt.Println("子goroutine结束")
		for i := 0; i < 3; i++ {
			c <- i
			fmt.Println("子goroutine正在运行，写入数据：", i, "len(c) = ", len(c), "cap(c) = ", cap(c))
		}
	}()

	//如果把上面的for循环改成4，写入3个后缓冲区满了，再写就阻塞了
	//这时只能等main读取了一个，缓冲区有空间了，子goroutine才继续写入
	//go func() {
	//	defer fmt.Println("子goroutine结束")
	//	for i := 0; i < 4; i++ {
	//		c <- i
	//		fmt.Println("子goroutine正在运行，写入数据：", i, "len(c) = ", len(c), "cap(c) = ", cap(c))
	//	}
	//}()

	//我想让子goroutine先发，发完3个，再让主来读，所以让主goroutine休眠一会
	time.Sleep(2 * time.Second)

	for i := 0; i < 3; i++ {
		num := <-c
		fmt.Println("读到：", num)
	}

	fmt.Println("main结束")
}

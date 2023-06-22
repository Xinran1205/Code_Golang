package main

import (
	"fmt"
	"runtime"
	"time"
)

func main() {
	//用go创建承载一个形参为空，返回值为空的函数的goroutine
	go func() {
		defer fmt.Println("A.defer")
		//这是一个匿名函数，不是再开一个goroutine
		func() {
			defer fmt.Println("B.defer")
			//退出当前的goroutine
			runtime.Goexit()
			fmt.Println("B")
		}()
		fmt.Println("A")
	}()

	//思考如何获取这个返回值，
	//肯定不能写t:=下面这个，因为这个是个goroutine，它不支持阻塞操作的，它是异步操作（并行的）。 像a:= 5+10 其实是阻塞操作
	//答案：使用管道机制：channel！！来通信
	go func(a int, b int) bool {
		fmt.Println("a = ", a, "b = ", b)
		return true
	}(10, 20) //这个是函数的入参传值

	//写个死循环防止main退出导致前面这个go func(){}()也退出了
	for {
		time.Sleep(1 * time.Second)
	}
}

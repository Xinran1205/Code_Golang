package main

import (
	"fmt"
	"time"
)

func newTask() {
	i := 0
	for {
		i++
		fmt.Println("new goroutine: i = ", i)
		time.Sleep(1 * time.Second)
	}
}

func main() {
	//创建一个goRoutine，去执行newTask()函数
	//这个就是理解为我以前学的c语言的fork，创建的是子进程
	go newTask()
	//所以我们如果在这就退出main函数，那么newTask()函数就不会执行了
	//因为main理解为父进程，父进程退出了，子进程也就退出了

	//但是我们可以让main函数休眠一段时间，让newTask()函数执行一段时间
	i := 0
	for {
		i++
		fmt.Println("main goroutine: i = ", i)
		time.Sleep(1 * time.Second)
	}
}

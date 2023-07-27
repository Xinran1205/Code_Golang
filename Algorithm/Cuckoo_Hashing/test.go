package main

import (
	"fmt"
)

func main() {
	Test1()
}

func Test1() {
	a := InitHash()
	//这两行可以使一个goroutine的大小为1GB
	//runtime.GOMAXPROCS(runtime.NumCPU())
	//runtime.GOMAXPROCS(1024)
	//插入20个数
	for i := 0; i < 10000; i++ {
		//当你使用 go 关键字调用一个函数时，会创建一个新的 goroutine 来并发执行这个函数。
		//这个新创建的 goroutine 只负责执行该函数，一旦函数执行完毕，这个 goroutine 就会结束。
		//因此，一个 goroutine 只负责执行一个函数，它的生命周期与该函数的执行时间相关。
		go a.Put(i, i)
		//go func(i int) {
		//	a.Put(i, i)
		//}(i)
	}
	//下面都是父进程
	//for a.Get(1) == nil {
	//
	//}
	//a.Delete(1)

	for a.numElements != 10000 {
	}
	fmt.Println(size(a.HashArrA))
	fmt.Println(size(a.HashArrB))
	fmt.Println(cap(a.HashArrA))
	fmt.Println(a.numElements)
}

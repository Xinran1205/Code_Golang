package main

import (
	"fmt"
)

func main() {
	Test1()
	//fmt.Println(cap(a.HashArrA))
}

func Test1() {
	a := InitHash()
	//插入20个数
	for i := 0; i < 50; i++ {
		go a.Put(i, i)
		//go func(i int) {
		//	a.Put(i, i)
		//}(i)
	}
	//if a.Get(1) != nil {
	//	a.Delete(1)
	//}
	//time.Sleep(time.Second * 3)
	for a.numElements != 50 {
	}

	fmt.Println(a.HashArrA)
	fmt.Println(a.HashArrB)
	fmt.Println(a.numElements)
}

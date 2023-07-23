package main

import "fmt"

func main() {
	a := InitHash()
	a.Put(1, "t")
	fmt.Println(a.Get(1))
	//a.Delete(1)
	//a.Put(1, "k")
	//fmt.Println(a.Get(1))
	//fmt.Println(a.size)
	//a.Put(3, "a")
	//a.Put(4, "b")
	//a.Put(5, "c")
	//a.Put(5231, "d")
	//a.Put(532, "e")
	//a.Put(5789, "f")
	//a.Put(11, true)
	//
	//fmt.Println(a.Get(5231))
	//fmt.Println(a.Get(2))
	//fmt.Println(a.HashArrA)
	//fmt.Println(a.HashArrB)
	////a.Delete(3)
	//fmt.Println(a.Get(3))
	//a.Put(2, "wxr")
	//a.Put(3, "nihao")
	//a.Put(4, "nihaoaaa")
	//fmt.Println(a.Get(2))
	//fmt.Println(a.Get(3))
	//fmt.Println(a.Get(4))
}

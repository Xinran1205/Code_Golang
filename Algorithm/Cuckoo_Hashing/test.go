package main

import "fmt"

func main() {
	a := InitHash()
	a.Put(1, "1")
	a.Put(2, "2")
	a.Put(3, "3")
	a.Put(4, "4")
	a.Put(5, "5")
	a.Put(6, "6")
	a.Put(7, "7")
	a.Put(8, "8")
	a.Put(9, "9")
	a.Put(10, "10")
	a.Put(11, "11")
	a.Put(12, "12")
	a.Put(13, "13")
	a.Put(14, "14")
	a.Put(15, "15")
	a.Put(16, "16")
	a.Put(17, "17")
	a.Put(18, "18")
	a.Put(19, "19")
	a.Put(20, "20")
	fmt.Println(a.HashArrA)
	fmt.Println(a.HashArrB)
	fmt.Println(a.numElements)
	//fmt.Println(cap(a.HashArrA))
}

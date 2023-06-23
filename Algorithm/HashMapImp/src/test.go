package main

import "fmt"

func main() {
	a := initHash()
	a.Put(1, "t")
	a.Put(2, "k")
	a.Put(3, "a")
	a.Put(4, "b")
	a.Put(5, "c")
	a.Put(5231, "d")
	a.Put(532, "e")
	a.Put(5789, "f")
	a.Put(11, true)

	fmt.Println(a.Get(5231))
	fmt.Println(a.Get(1))
	a.Delete(3)
	fmt.Println(a.Get(3))
}

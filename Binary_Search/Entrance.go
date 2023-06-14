package main

import "fmt"

func main() {
	TestNormalBS()
}

func TestNormalBS() {
	a := make([]int, 10)
	//rand.Seed(1)
	for i := range a {
		a[i] = i
	}
	b := NormalBS(a, 9)
	fmt.Println(a)
	fmt.Println(b)
}

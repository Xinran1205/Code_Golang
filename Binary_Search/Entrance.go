package main

import (
	"fmt"
	"math/rand"
	"sort"
)

func main() {
	TestRecurBS()
	TestNormalBS()
}

func TestNormalBS() {
	a := make([]int, 100)
	rand.Seed(1)
	for i := range a {
		a[i] = rand.Intn(100)
	}
	sort.Ints(a)
	b := NormalBS2(a, 9)
	fmt.Println(b)
}

func TestRecurBS() {
	a := make([]int, 100)
	rand.Seed(1)
	for i := range a {
		a[i] = rand.Intn(100)
	}
	sort.Ints(a)
	fmt.Println(a)
	b := RecurBS(a, 0, len(a)-1, 9)
	fmt.Println(b)
}

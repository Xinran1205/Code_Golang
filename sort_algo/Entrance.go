package main

import (
	"fmt"
	"math/rand"
	"runtime"
	"time"
)

func main() {
	//OptimizedInsertAndInsert()
	//shellSortTest()
	//mergeSortTest()
	//prioQueTest()
	//HeapTest()
	BubbleTest()
}

func BubbleTest() {
	a := make([]int, 10000)
	rand.Seed(1)
	for i := range a {
		a[i] = rand.Intn(100000)
	}
	fmt.Println(isSorted(a))
	bubbleSort(a)
	fmt.Println(isSorted(a))
}

func HeapTest() {
	//这个a := make([]int, 10)语法其实创建的是切片，10是切片容量和长度，
	//可以再给一个参数make([]int, 10，100)，这样10是长度，100是容量，这个就是里面有10个元素都是0，
	//容量就是提前开辟好的空间，当容量满了，我们再append时，go语言底层会再开辟一个与前一个容量相同的空间
	a := make([]int, 10)
	rand.Seed(1)
	for i := range a {
		a[i] = rand.Intn(100)
	}
	heapSort(a)
	fmt.Println(isSortedExceptOne(a))
}

func prioQueTest() {
	rand.Seed(1)
	var a []int
	a = append(a, 8888)
	for i := 0; i < 10; i++ {
		a = insert(a, rand.Intn(1000))
	}
	for i := 10; i > 0; i-- {
		if len(a) > 1 {
			i = pop(&a)
			fmt.Println(i)
		}
	}
}

func mergeSortTest() {
	a := make([]int, 10000)
	rand.Seed(1)
	for i := range a {
		a[i] = rand.Intn(100000)
	}
	fmt.Println(isSorted(a))
	mergeSort(a)
	fmt.Println(isSorted(a))
}

func shellSortTest() {
	a := make([]int, 10000)
	rand.Seed(1)
	for i := range a {
		a[i] = rand.Intn(100000)
	}
	fmt.Println(isSorted(a))
	shellSort(a)
	fmt.Println(isSorted(a))
}

// OptimizedInsertAndInsert 比较二分优化过的插入排序和普通插入排序的耗时
func OptimizedInsertAndInsert() {
	//m是用来看内存消耗的
	var m1, m2 runtime.MemStats

	a := make([]int, 100000)
	//种子值，同一种子值下每次随机出来的种子是一样的,
	//比如我后面再用rand.Intn(100)仍然是一样的，除非改变种子值，
	//所以如果想让每次生成的不一样我们一般让种子值等于电脑时间
	rand.Seed(1)
	for i := range a {
		a[i] = rand.Intn(10000000)
	}

	fmt.Println(isSorted(a))
	runtime.ReadMemStats(&m1)
	start := time.Now()
	quickSort(a)
	elapsed := time.Since(start)
	fmt.Println(isSorted(a))
	runtime.ReadMemStats(&m2)
	fmt.Printf("Algorithm 1 memory usage: %d bytes\n", m2.TotalAlloc-m1.TotalAlloc)
	fmt.Printf("func1 took %d\n", elapsed.Nanoseconds())

	rand.Seed(1)
	c := make([]int, 100000)
	for i := range c {
		c[i] = rand.Intn(10000000)
	}
	fmt.Println(isSorted(c))
	runtime.ReadMemStats(&m1)
	start3 := time.Now()
	quickSortThree(c)
	elapsed3 := time.Since(start3)
	fmt.Println(isSorted(c))
	runtime.ReadMemStats(&m2)
	fmt.Printf("Algorithm 3 memory usage: %d bytes\n", m2.TotalAlloc-m1.TotalAlloc)
	fmt.Printf("func3 took %d\n", elapsed3.Nanoseconds())
}

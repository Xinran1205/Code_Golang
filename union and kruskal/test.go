package main

import "fmt"

func main() {
	t := createNodeArray(10)
	fmt.Println(t.isConnected(t[0], t[1]))
	t.union(t[0], t[1])
	fmt.Println(t.isConnected(t[0], t[1]))
	t.union(t[2], t[3])
	t.union(t[0], t[3])
	fmt.Println(t.isConnected(t[1], t[2]))
	fmt.Println(t[3].next.val)
	//fmt.Println(t[2].next.val)
	//fmt.Println(t[1].next.val)
	//fmt.Println(t[0].next.val)
	t.union(t[3], t[4])
	//fmt.Println(t[4].next.val)
	fmt.Println(t[3].next.val)
	//for i := range t {
	//	fmt.Println(t[i].rank)
	//}
}

package main

type node struct {
	val  int
	rank int
	next *node
}

type nodeArray []*node

func createNodeArray(n int) nodeArray {
	arr := make(nodeArray, n)
	for i := 0; i < n; i++ {
		arr[i] = &node{val: i, next: nil, rank: 1}
		arr[i].next = arr[i]
	}
	return arr
}

// 需要维护一个栈
func (NodeArray *nodeArray) find(a *node) *node {
	stack := Stack{}
	for a.next != a {
		stack.Push(*a)
		a = a.next
	}
	//此时a是所在集合的根节点
	//下面让这个一条链上面的所有节点直接挂在这个根节点上面
	for !stack.IsEmpty() {
		t := stack.Pop()
		t.next = a
	}
	return a
}

func (NodeArray *nodeArray) union(a *node, b *node) {
	i := NodeArray.find(a)
	j := NodeArray.find(b)
	if i == j {
		return
	}
	if i.rank < j.rank {
		i.next = j
		j.rank = i.rank + j.rank
	} else {
		j.next = i
		i.rank = j.rank + i.rank
	}
}

func (NodeArray *nodeArray) isConnected(a *node, b *node) bool {
	if NodeArray.find(a) == NodeArray.find(b) {
		return true
	} else {
		return false
	}
}

type Stack []node

func (s *Stack) Push(item node) {
	*s = append(*s, item)
}

func (s *Stack) Pop() *node {
	if len(*s) == 0 {
		return nil
	}
	item := (*s)[len(*s)-1]
	//在这个表达式中，*s 表示取出指针 s 所指向的切片对象，
	//然后使用切片表达式 [:len(*s)-1] 获取该切片的一个子序列，
	//包含从0开始到 len(*s)-1 结束（不包含 len(*s)-1）的元素。
	*s = (*s)[:len(*s)-1]
	return &item
}

func (s *Stack) IsEmpty() bool {
	return len(*s) == 0
}

//这两个用不到
//func (s *Stack) Peek() *node {
//	if len(*s) == 0 {
//		return nil
//	}
//	return &(*s)[len(*s)-1]
//}

//func (s *Stack) Len() int {
//	return len(*s)
//}

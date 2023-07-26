package main

import (
	"container/list"
)

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
	stack := &Stack{list: list.New()}
	for a.next != a {
		stack.Push(a)
		a = a.next
	}
	//此时a是所在集合的根节点
	//下面让这个一条链上面的所有节点直接挂在这个根节点上面
	for n := stack.Pop(); n != nil; n = stack.Pop() {
		n.next = a
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

// 这个栈要非常注意，可把我写累死了，
// 因为要注意栈里面存的是指针，因为我们需要修改node的next指针
// 发现有next和prev，继而研究这个栈发现这个栈的底层实现是双向链表，
// 这个双向链表是尾插法， 并且他有一个有哨兵节点，通过哨兵节点的pre可以找到最后一个节点然后开始插入操作
type Stack struct {
	list *list.List
}

func (s *Stack) Push(n *node) {
	s.list.PushBack(n)
}

func (s *Stack) Pop() *node {
	if s.list.Len() == 0 {
		return nil
	}
	return s.list.Remove(s.list.Back()).(*node)
}

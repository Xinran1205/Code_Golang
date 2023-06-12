package main

import (
	"strconv"
)

type node struct {
	key   interface{}
	value interface{}
	next  *node
}

type HashArray [10]*node

func initHash() HashArray {
	m := HashArray{}
	return m
}

func hashValue(key interface{}) int {
	// 将键转换为字节数组类型
	var b []byte
	switch k := key.(type) {
	case int:
		b = strconv.AppendInt(b, int64(k), 10)
	case bool:
		if k {
			b = []byte("true")
		} else {
			b = []byte("false")
		}
	case string:
		b = []byte(k)
	case []byte:
		b = k
	default:
		// 其他类型的键暂不支持
		return -1
	}

	// 计算字节数组的哈希值
	var hash int
	for i := 0; i < len(b); i++ {
		hash = 31*hash + int(b[i])
	}

	// 取哈希值的绝对值，并对数组长度取模
	return (hash & 0x7fffffff) % 10
}

func (hashArray *HashArray) Put(key interface{}, value interface{}) {
	index := hashValue(key)

	if hashArray[index] == nil {
		hashArray[index] = &node{key, value, nil}
	} else {
		FindtheLastNode := hashArray[index]
		for FindtheLastNode.next != nil {
			if key == FindtheLastNode.key {
				FindtheLastNode.value = value
				return
			}
			FindtheLastNode = FindtheLastNode.next
		}
		if key == FindtheLastNode.key {
			FindtheLastNode.value = value
			return
		} else {
			FindtheLastNode.next = &node{key, value, nil}
		}
	}
}

func (hashArray *HashArray) Get(key interface{}) interface{} {
	index := hashValue(key)
	node := hashArray[index]
	for node != nil {
		if key == node.key {
			return node.value
		}
		node = node.next
	}
	return nil
}

func (hashArray *HashArray) Delete(key interface{}) bool {
	index := hashValue(key)
	preNode := hashArray[index]
	if preNode == nil {
		return false
	} else if preNode.key == key {
		hashArray[index] = nil
		return true
	}
	node := preNode.next
	for node != nil {
		if key == node.key {
			preNode.next = node.next
			return true
		}
		preNode = preNode.next
		node = node.next
	}
	return false
}

package main

import (
	"fmt"
	"math/rand"
)

// 这里我们给一个我们最大key得值
const Maxkey = 100000

// 定义数组大小
const ArraySize = 100

// 一个大的质数
const P = 115249

// 这个是为了每次交替踢出去
var IsCheckArrA bool

type node struct {
	key        interface{}
	value      interface{}
	HashAIndex int
	HashBIndex int
}

// 这个就是我们初始化的一个结构体，里面两个指针，分别指向两个数组
type HashArray struct {
	HashArrA *HashArrayA
	HashArrB *HashArrayB
}

type HashArrayA [ArraySize]*node

type HashArrayB [ArraySize]*node

func InitHash() HashArray {
	IsCheckArrA = true
	m := HashArray{}
	m.HashArrA = &HashArrayA{}
	m.HashArrB = &HashArrayB{}
	return m
}

func UniversalHashA(key interface{}) int {
	m := ArraySize
	p := P
	switch k := key.(type) {
	case int:
		rand.Seed(int64(k))
		a := rand.Intn(p)
		b := rand.Intn(p-1) + 1
		h := ((a*k + b) % p) % m
		return h
	default:
		// 其他类型的键暂不支持
		return -1
	}
	return -1
}

// 这个B哈希的种子和A哈希需要不一样
func UniversalHashB(key interface{}) int {
	m := ArraySize
	p := P
	switch k := key.(type) {
	case int:
		rand.Seed(int64(k) + Maxkey)
		a := rand.Intn(p)
		b := rand.Intn(p-1) + 1
		h := ((a*k + b) % p) % m
		return h
	default:
		// 其他类型的键暂不支持
		return -1
	}
	return -1
}

// 我这个写的应该是有漏洞的
// 首先我没有考虑到如果A数组和B数组都满了的情况
func (hashArray *HashArray) Put(key interface{}, value interface{}) {
	IndexA := UniversalHashA(key)
	fmt.Println("可以放在A数组", IndexA)
	IndexB := UniversalHashB(key)
	fmt.Println("可以放在B数组", IndexB)
	//这里判断条件要包含数组里面的值的key和我们要放的key一样（已经放进去的情况）
	if hashArray.HashArrA[IndexA] == nil || hashArray.HashArrA[IndexA].key == key {
		hashArray.HashArrA[IndexA] = &node{key, value, IndexA, IndexB}
	} else if hashArray.HashArrB[IndexB] == nil || hashArray.HashArrB[IndexB].key == key {
		hashArray.HashArrB[IndexB] = &node{key, value, IndexA, IndexB}
	} else {
		//如果A和B都不为空，我们就把他踢到对面
		//这里要注意比如我们key等于10对应的A数组下标有人，B数组下标也有人。
		//我们就把A数组下标的人踢走，A数组下标的人比如说是30，他对应的A数组是现在这里，
		//但是他对应的B数组下标和我们的10是不一样的，因为哈希A和哈希B函数是不一样的，
		//两个不同的key两个不同哈希全部一样的概率是非常小的
		NextKey := hashArray.HashArrA[IndexA].key
		NextValue := hashArray.HashArrA[IndexA].value
		if IsCheckArrA {
			hashArray.HashArrA[IndexA] = &node{key, value, IndexA, IndexB}
			IsCheckArrA = false
		} else {
			NextKey = hashArray.HashArrB[IndexB].key
			NextValue = hashArray.HashArrB[IndexB].value
			hashArray.HashArrB[IndexB] = &node{key, value, IndexA, IndexB}
			IsCheckArrA = true
		}
		//递归
		hashArray.Put(NextKey, NextValue)
	}
}

func (hashArray *HashArray) Get(key interface{}) interface{} {
	IndexA := UniversalHashA(key)
	IndexB := UniversalHashB(key)
	if hashArray.HashArrA[IndexA] != nil && hashArray.HashArrA[IndexA].key == key {
		return hashArray.HashArrA[IndexA].value
	}
	if hashArray.HashArrB[IndexB] != nil && hashArray.HashArrB[IndexB].key == key {
		return hashArray.HashArrB[IndexB].value
	}
	fmt.Println("cannot get Not found key:", key)
	return nil
}

func (hashArray *HashArray) Delete(key interface{}) bool {
	IndexA := UniversalHashA(key)
	IndexB := UniversalHashB(key)
	if hashArray.HashArrA[IndexA] != nil && hashArray.HashArrA[IndexA].key == key {
		hashArray.HashArrA[IndexA] = nil
		return true
	}
	if hashArray.HashArrB[IndexB] != nil && hashArray.HashArrB[IndexB].key == key {
		hashArray.HashArrB[IndexB] = nil
		return true
	}
	fmt.Println("cannot delete Not found key:", key)
	return false
}

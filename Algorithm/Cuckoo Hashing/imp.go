package main

//https://zhuanlan.zhihu.com/p/462813998
//这里提到了改良的方法，增加巢穴，3，4个巢穴

import (
	"fmt"
	"math/rand"
	"sync"
)

// 这里我们给一个我们最大key得值
const RandomValue = 100000

// 定义数组大小，这里注意一下，这个是一个数组的大小，我们有两个数组，所以实际上我们的容量是两倍
const ArraySize = 10

// 一个大的质数，我在思考，这个P是不是要搞一个数组，这样rehash的时候就使用不同的P
const P = 115249

// 最大递归次数
const MaxDepth = 20

// 负载因子 百分之75就扩容
const MaxLoadFactor float64 = 0.75

// 这个是为了每次交替踢出去，这个是我随便想的，应该可以算出一个每次踢谁出去
var IsCheckArrA bool

type node struct {
	key        interface{}
	value      interface{}
	HashAIndex int
	HashBIndex int
}

// 这个就是我们初始化的一个结构体，里面两个指针，分别指向两个数组
type HashArray struct {
	HashArrA []*node
	HashArrB []*node
	//来个锁
	mutexLock sync.Mutex
	//用来记录数组里面有多少个元素，防止死递归
	numElements int
}

func InitHash() HashArray {
	IsCheckArrA = true
	m := HashArray{}
	m.HashArrA = make([]*node, ArraySize)
	m.HashArrB = make([]*node, ArraySize)
	m.numElements = 0
	return m
}

// 这个随机种子设置很有讲究，我希望扩容（rehash）后，随机的a和b不一样，所以我可以把随机种子设置为加上容量
// 这样随机种子会随着容量的变化而变化，同时在同意容量下，相同的key也可以生成相同的a和b即相同的hash值
func UniversalHashA(key interface{}, arraySize int) int {
	m := arraySize
	p := P
	switch k := key.(type) {
	case int:
		rand.Seed(int64(k) + int64(m))
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

// 这个B哈希的种子和A哈希需要不一样，所以我就把随机种子改掉了
func UniversalHashB(key interface{}, arraySize int) int {
	m := arraySize
	p := P
	switch k := key.(type) {
	case int:
		rand.Seed(int64(k) + int64(m) + RandomValue)
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

// 这个是对外的接口，因为我不想让用户输入深度，这会让用户感觉很奇怪
func (hashArray *HashArray) Put(key interface{}, value interface{}) {
	hashArray.mutexLock.Lock()
	hashArray.PutFunc(key, value, 0)
	hashArray.mutexLock.Unlock()
}

func (hashArray *HashArray) PutFunc(key interface{}, value interface{}, depth int) error {
	//cap代表容量，len代表长度,这里是相等的
	IndexA := UniversalHashA(key, cap(hashArray.HashArrA))
	IndexB := UniversalHashB(key, cap(hashArray.HashArrB))
	//这里判断条件要包含数组里面的值的key和我们要放的key一样（已经放进去的情况）
	if hashArray.HashArrA[IndexA] == nil || hashArray.HashArrA[IndexA].key == key {
		if hashArray.HashArrA[IndexA] == nil {
			//这里要注意一下，如果是替换key，我们数组中的size是不变的
			hashArray.numElements++
		}
		hashArray.HashArrA[IndexA] = &node{key, value, IndexA, IndexB}
		return nil
	} else if hashArray.HashArrB[IndexB] == nil || hashArray.HashArrB[IndexB].key == key {
		if hashArray.HashArrB[IndexB] == nil {
			hashArray.numElements++
		}
		hashArray.HashArrB[IndexB] = &node{key, value, IndexA, IndexB}
		return nil
	} else if float64(hashArray.numElements) > float64(2*cap(hashArray.HashArrA))*MaxLoadFactor {
		//有一个非常要注意的，当我们插入某个数，他要rehash时，这个数是没有被放进去的
		//这里我们不能直接用2*ArraySize，因为我每次扩容完，他都是新的大小了
		//我们在这里写扩容，可能数组已经快满了，但是因为他一直有地方可以放，我们就让他放
		//实际上我们只有当没地方放并且数组大小大于75%的时候，我们才扩容
		//当数组大小大于75%的时候，我们就扩容
		hashArray.Rehash(&node{key, value, IndexA, IndexB})
		fmt.Println("array is full,Rehash")
		return nil
	} else if depth > MaxDepth {
		//这里我随便写的最大递归次数，20,这种情况就是数组没有满，但是出现了死递归的情况
		//这种情况rehash，可以不扩容，我觉得也可以扩容
		hashArray.Rehash(&node{key, value, IndexA, IndexB})
		fmt.Println("recursion limit exceeded,Rehash")
		return nil
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
		hashArray.PutFunc(NextKey, NextValue, depth+1)
	}
	return nil
}

func (hashArray *HashArray) Rehash(curNode *node) {
	// 创建一个新的 HashArrayA 数组和 HashArrayB 数组
	newHashArrA := make([]*node, ArraySize*2)
	newHashArrB := make([]*node, ArraySize*2)

	//当我们要rehash时，当前要插入的元素也要放进去
	hashArray.rehashNode(newHashArrA, newHashArrB, curNode, 0)

	// 遍历 HashArrayA 和 HashArrayB 数组中的所有节点，并重新散列到新数组中
	for _, n := range hashArray.HashArrA {
		if n != nil {
			hashArray.rehashNode(newHashArrA, newHashArrB, n, 0)
		}
	}
	for _, n := range hashArray.HashArrB {
		if n != nil {
			hashArray.rehashNode(newHashArrA, newHashArrB, n, 0)
		}
	}
	// 将新数组设置为哈希表的数组
	//根据go语言垃圾回收特性，无需回收旧数组，旧数组会被自动回收
	hashArray.HashArrA = newHashArrA
	hashArray.HashArrB = newHashArrB
	return
}

func (hashArray *HashArray) rehashNode(newHashArrA []*node, newHashArrB []*node, n *node, depth int) {
	// 重新计算节点 n 的哈希值
	IndexA := UniversalHashA(n.key, cap(newHashArrA))
	IndexB := UniversalHashB(n.key, cap(newHashArrB))
	//我们要把n里面的原来的hash下标更新
	n.HashAIndex = IndexA
	n.HashBIndex = IndexB
	// 如果新数组中的节点为空，则直接将节点 n 放入新数组中
	if newHashArrA[IndexA] == nil {
		newHashArrA[IndexA] = n
		return
	} else if newHashArrB[IndexB] == nil {
		newHashArrB[IndexB] = n
		return
	} else if depth > MaxDepth {
		//扩容,这个情况其实可以注释掉，因为需要两次的扩容的情况几乎不可能
		hashArray.Rehash(n)
		return
	} else {
		// 这里这4个值是现在A里面的node的数据，要被踢出来的node的数据
		NextKey := hashArray.HashArrA[IndexA].key
		NextValue := hashArray.HashArrA[IndexA].value
		NextArrAIndex := hashArray.HashArrA[IndexA].HashAIndex
		NextArrBIndex := hashArray.HashArrA[IndexA].HashBIndex
		if IsCheckArrA {
			hashArray.HashArrA[IndexA] = n
			IsCheckArrA = false
		} else {
			NextKey = hashArray.HashArrB[IndexB].key
			NextValue = hashArray.HashArrB[IndexB].value
			NextArrAIndex = hashArray.HashArrB[IndexB].HashAIndex
			NextArrBIndex = hashArray.HashArrB[IndexB].HashBIndex
			hashArray.HashArrB[IndexB] = n
			IsCheckArrA = true
		}
		//递归
		hashArray.rehashNode(newHashArrA, newHashArrB, &node{NextKey, NextValue, NextArrAIndex, NextArrBIndex}, depth+1)
	}
	return
}

func (hashArray *HashArray) Get(key interface{}) interface{} {
	IndexA := UniversalHashA(key, cap(hashArray.HashArrA))
	IndexB := UniversalHashB(key, cap(hashArray.HashArrB))
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
	hashArray.mutexLock.Lock()
	IndexA := UniversalHashA(key, cap(hashArray.HashArrA))
	IndexB := UniversalHashB(key, cap(hashArray.HashArrB))
	if hashArray.HashArrA[IndexA] != nil && hashArray.HashArrA[IndexA].key == key {
		hashArray.HashArrA[IndexA] = nil
		hashArray.numElements--
		hashArray.mutexLock.Unlock()
		return true
	}
	if hashArray.HashArrB[IndexB] != nil && hashArray.HashArrB[IndexB].key == key {
		hashArray.HashArrB[IndexB] = nil
		hashArray.numElements--
		hashArray.mutexLock.Unlock()
		return true
	}
	fmt.Println("cannot delete Not found key:", key)
	hashArray.mutexLock.Unlock()
	return false
}

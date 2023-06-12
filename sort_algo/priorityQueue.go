package main

//对于我们这个优先队列，我们是数组实现，
//数组的0位我们不存元素空出来，因为0位不好做统一处理，比如2k,2k+1

// 向优先队列中插入一个元素
// 我先用切片，这样好像就不用扩容了

func insert(a []int, num int) []int {
	a = append(a, num)
	swim(a)
	return a
}

// 从优先队列中取出一个元素
func pop(a *[]int) int {
	i := (*a)[1]
	exch(*a, 1, len(*a)-1)
	*a = (*a)[:len(*a)-1]
	sink(a)
	return i
}

// 上浮
func swim(a []int) {
	//首先我们上浮是在插入一个新元素，对这个新元素上浮
	//那么新元素一定先是插在数组的最后，所以对数组最后一个元素进行上浮
	for p := len(a) - 1; p > 1; {
		if a[p] <= a[p/2] {
			break
		} else {
			exch(a, p, p/2)
			p = p / 2
		}
	}
}

// 下沉
// 下沉就是删除根节点，我们每次删除其实就是删除根节点
// 因为根节点是值最大的节点，那么删除他，就得替换一个上去
// 我们替换最后一个元素上去再调整树的结构
func sink(a *[]int) {
	for i := 1; 2*i <= len(*a)-1; {
		p := 2 * i
		if p < len(*a)-1 && (*a)[p] < (*a)[p+1] {
			p++
		}
		if (*a)[i] > (*a)[p] {
			break
		}
		exch(*a, i, p)
		i = p
	}
}

// 堆排序算法
func heapSort(a []int) {
	n := len(a) - 1
	for k := n / 2; k >= 1; k-- {
		heapSink(a, k, n)
	}
	for n > 1 {
		exch(a, 1, n)
		n--
		heapSink(a, 1, n)
	}
}

func heapSink(a []int, i int, n int) {
	for 2*i <= n {
		p := 2 * i
		if p < n && a[p] < a[p+1] {
			p++
		}
		if a[i] > a[p] {
			break
		}
		exch(a, i, p)
		i = p
	}
}

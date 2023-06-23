package main

func partition(a []int, lo int, hi int) int {
	i := lo
	j := hi
	v := a[lo]
	for i < j {
		for i < j && a[j] >= v {
			j--
		}
		for i < j && a[i] <= v {
			i++
		}
		exch(a, i, j)
	}
	exch(a, lo, j)
	return j
}

func quickSort(a []int) {
	//要先打乱
	//比如对一个业务要使用快速排序，我们先对这串数打乱，防止出现最坏情况
	qc(a, 0, len(a)-1)
}

func quickSortThree(a []int) {
	DivideInThree(a, 0, len(a)-1)
}

func qc(a []int, i int, j int) {
	if j <= i {
		//效果不是很好
		//InsertSortForQS(a, i, j)
		return
	}
	p := partition(a, i, j)
	qc(a, i, p-1)
	qc(a, p+1, j)
}

func DivideInThree(a []int, lo int, hi int) {
	if hi <= lo {
		return
	}
	i := lo + 1
	j := lo
	gt := hi
	v := a[lo]
	for i <= gt {
		if a[i] < v {
			exch(a, i, j)
			i++
			j++
		} else if a[i] > v {
			exch(a, i, gt)
			gt--
		} else {
			i++
		}
	}
	DivideInThree(a, lo, j-1)
	DivideInThree(a, gt+1, hi)
}

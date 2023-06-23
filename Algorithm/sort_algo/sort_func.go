package main

// 排列,升序
// 使用comparable的话好像i和j只能比较==或!=
func less1[T comparable](i, j T) bool {
	return i != j
}

// 不确定是不是这么写
func less2[T int | string | float32](i, j T) bool {
	return i < j
}

func less(i, j int) bool {
	return i < j
}

// 冒泡排序，思路就是每次把一个最大的数换到最后一位
// 可以理解为从右往左排序
func bubbleSort(arr []int) {
	for i := range arr {
		for j := 0; j+1 < len(arr)-i; j++ {
			if arr[j] > arr[j+1] {
				exch(arr, j, j+1)
			}
		}
	}
}

// SelectSort 选择排序，每次找出的最小的值放到最前面
func SelectSort(arr []int) {
	for i := range arr {
		min := arr[i]
		index := i
		for j := i + 1; j < len(arr); j++ {
			if less(arr[j], min) {
				min = arr[j]
				index = j
			}
		}
		exch(arr, index, i)
	}
}

// InsertSort 插入排序，保证前面一直是有序的，
// 遇到下一个数，将这个数移到前面合适的位置
func InsertSort(arr []int) {
	for i := range arr {
		for j := i; j > 0; j-- {
			if arr[j] < arr[j-1] {
				exch(arr, j-1, j)
			}
		}
	}
}

//这是为了优化快速排序而单独写的插入排序，因为当快速排序最后很少元素时，用插入排序更快

func InsertSortForQS(arr []int, lo int, hi int) {
	for i := lo; i <= hi; i++ {
		for j := i; j > 0; j-- {
			if arr[j] < arr[j-1] {
				exch(arr, j-1, j)
			}
		}
	}
}

// 优化过后的插入排序，这样可以减少一些比较
func OptimizedInsertSort(arr []int) {
	for i := range arr {
		//用二分法直接找到要插入的地点
		insertPos := binarySearch(arr, 0, i, arr[i])
		//这里要保存一下arr[i]的值
		q := arr[i]
		for j := i; j > insertPos; j-- {
			arr[j] = arr[j-1]
		}
		arr[insertPos] = q
	}
}

// 希尔排序，现在用的这个margin叫做Knuth 序列
func shellSort(arr []int) {
	le := len(arr)
	//margin := int(math.Floor(math.Log(float64(le)) / math.Log(3)))
	margin := 1
	////这里就是初始化一下间隔
	for margin <= le/3 {
		margin = margin*3 + 1
	}
	//间隔到1然后排一下整个就肯定排好了
	for margin >= 1 {
		for j := margin; j < len(arr); j++ {
			for p := j; p >= margin; p = p - margin {
				if arr[p] < arr[p-margin] {
					exch(arr, p, p-margin)
				}
			}
		}
		margin = (margin - 1) / 3
	}
}

func binarySearch(arr []int, lo, hi, x int) int {
	for lo < hi {
		mid := lo + (hi-lo)/2
		if arr[mid] < x {
			lo = mid + 1
		} else {
			hi = mid
		}
	}
	return lo
}

func exch(arr []int, i int, j int) {
	t := arr[j]
	arr[j] = arr[i]
	arr[i] = t
}

func isSorted(arr []int) bool {
	for i := 1; i < len(arr); i++ {
		if arr[i] < arr[i-1] {
			return false
		}
	}
	return true
}

// 不看第一位
func isSortedExceptOne(arr []int) bool {
	for i := 2; i < len(arr); i++ {
		if arr[i] < arr[i-1] {
			return false
		}
	}
	return true
}

package main

// 二分查找是否含有值v,没有就返回插入位置
func NormalBS(arr []int, v int) int {
	lo := 0
	hi := len(arr) - 1
	for lo < hi {
		mid := lo + (hi-lo)/2
		if arr[mid] >= v {
			hi = mid
		} else {
			lo = mid + 1
		}
	}
	return lo
}

// 另一种写法
func NormalBS2(arr []int, v int) int {
	lo := 0
	hi := len(arr) - 1
	for lo <= hi {
		mid := lo + (hi-lo)/2
		if arr[mid] > v {
			hi = mid - 1
		} else if arr[mid] < v {
			lo = mid + 1
		} else {
			return mid
		}
	}
	return lo
}

// 递归二分
func RecurBS(arr []int, lo int, hi int, v int) int {
	if lo > hi {
		return lo
	}
	mid := lo + (hi-lo)/2
	if arr[mid] > v {
		return RecurBS(arr, lo, mid-1, v)
	} else if arr[mid] < v {
		return RecurBS(arr, mid+1, hi, v)
	} else {
		return mid
	}
}

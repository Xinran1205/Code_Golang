package main

import "sort"

// 二分查找是否含有值v
func NormalBS(arr []int, v int) int {
	sort.Ints(arr)
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

package main

// 把aux从数组替换成切片感觉更好一点，因为使用切片他是在merge函数内作用
// 如果使用数组，需要把他设为全局变量，并且容量固定
func mergeSort(arr []int) {
	//aux = make([]int, len(arr))
	//sort(arr, 0, len(arr)-1)
	fromBotSort(arr)
}

func merge(arr []int, lo int, mid int, hi int) {
	i := lo
	j := mid + 1
	var aux []int
	for t := range arr {
		aux = append(aux, arr[t])
		//aux[t] = arr[t]
	}
	for p := lo; p <= hi; p++ {
		if i > mid {
			arr[p] = aux[j]
			j++
		} else if j > hi {
			arr[p] = aux[i]
			i++
		} else if less(aux[j], aux[i]) {
			arr[p] = aux[j]
			j++
		} else {
			arr[p] = aux[i]
			i++
		}
	}
}

// 自顶向下归并
func sort(arr []int, i int, j int) {
	if j <= i {
		return
	}
	mid := i + (j-i)/2
	sort(arr, i, mid)
	sort(arr, mid+1, j)
	if arr[mid] < arr[mid+1] {
		return
	}
	merge(arr, i, mid, j)
}

// 自底向上归并
func fromBotSort(arr []int) {
	for o := 1; o < len(arr); o = o + o {
		for j := 0; j < len(arr)-o; j = j + o + o {
			k := j + 2*o - 1
			if k > len(arr)-1 {
				k = len(arr) - 1
			}
			merge(arr, j, j+o-1, k)
		}
	}
}

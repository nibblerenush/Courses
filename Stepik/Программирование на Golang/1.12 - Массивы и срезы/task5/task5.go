package main

import "fmt"

func main() {
	var N int
	fmt.Scan(&N)

	var slice []int
	for i := 0; i < N; i++ {
		var num int
		fmt.Scan(&num)
		slice = append(slice, num)
	}

	var count int = 0
	for i := 0; i < len(slice); i++ {
		if slice[i] > 0 {
			count++
		}
	}

	fmt.Println(count)
}

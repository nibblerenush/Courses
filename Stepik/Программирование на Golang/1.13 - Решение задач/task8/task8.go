package main

import "fmt"

func main() {
	var N int
	fmt.Scan(&N)

	var count int = 0
	var min int

	for i := 0; i < N; i++ {
		var num int
		fmt.Scan(&num)

		if i == 0 || num < min {
			min = num
			count = 1
		} else if num == min {
			count++
		}

	}

	fmt.Println(count)
}

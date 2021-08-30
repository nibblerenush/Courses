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

	fmt.Println(slice[3])
}

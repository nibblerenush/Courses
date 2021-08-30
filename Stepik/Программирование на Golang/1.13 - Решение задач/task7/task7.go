package main

import "fmt"

func main() {
	var N int
	fmt.Scan(&N)

	var count int = 0
	for i := 0; i < N; i++ {
		var num int
		fmt.Scan(&num)
		if num == 0 {
			count++
		}
	}

	fmt.Println(count)
}

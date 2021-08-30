package main

import "fmt"

func main() {
	var n int
	var max int = 0
	var max_count int = 0

	for fmt.Scan(&n); n != 0; fmt.Scan(&n) {
		if n > max {
			max = n
			max_count = 1
		} else if n == max {
			max_count++
		}
	}

	fmt.Println(max_count)
}

package main

import "fmt"

func work(x int) int {
	return x
}

func main() {
	var cache = make(map[int]int)

	for i := 0; i < 10; i++ {
		var num int
		fmt.Scan(&num)

		if res, ok := cache[num]; ok {
			fmt.Print(res, " ")
		} else {
			cache[num] = work(num)
			fmt.Print(cache[num], " ")
		}
	}
}

package main

import "fmt"

func count(x int) int {
	if x == 1 {
		return 1
	}
	return 0
}

func vote(x int, y int, z int) int {
	var one_count int = 0

	one_count += count(x)
	one_count += count(y)
	one_count += count(z)

	if one_count >= 2 {
		return 1
	}
	return 0
}

func main() {
	fmt.Println(vote(1, 1, 1))
}

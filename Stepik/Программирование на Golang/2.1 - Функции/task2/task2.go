package main

import "fmt"

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

func minimumFromFour() int {
	var a, b, c, d int
	fmt.Scan(&a, &b, &c, &d)
	return min(min(a, b), min(c, d))
}

func main() {
	fmt.Println(minimumFromFour())
}

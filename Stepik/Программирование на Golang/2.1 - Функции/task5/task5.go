package main

import "fmt"

func sumInt(a ...int) (int, int) {
	var count int = 0
	var sum int = 0

	for _, elem := range a {
		count++
		sum += elem
	}

	return count, sum
}

func main() {
	var count, sum int = sumInt(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
	fmt.Println(count, sum)
}

package main

import "fmt"

func fibonacci(n int) int {
	var fib []int = []int{0, 1, 1}

	for i := 3; i <= n; i++ {
		fib = append(fib, fib[len(fib)-1]+fib[len(fib)-2])
	}

	return fib[n]
}

func main() {
	fmt.Println(fibonacci(10))
}

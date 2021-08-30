package main

import "fmt"

func main() {
	var A int
	fmt.Scan(&A)

	var fib []int = []int{0, 1, 1}
	var isFound bool = false

	for fib[len(fib)-1] <= A {
		if fib[len(fib)-1] == A {
			fmt.Println(len(fib) - 1)
			isFound = true
		}

		fib = append(fib, fib[len(fib)-1]+fib[len(fib)-2])
	}

	if !isFound {
		fmt.Println(-1)
	}
}

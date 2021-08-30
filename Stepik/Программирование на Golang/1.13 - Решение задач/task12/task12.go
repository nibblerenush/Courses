package main

import "fmt"

func main() {
	var N int
	fmt.Scan(&N)

	var m int = 1

	for m <= N {
		fmt.Print(m, " ")
		m *= 2
	}
}

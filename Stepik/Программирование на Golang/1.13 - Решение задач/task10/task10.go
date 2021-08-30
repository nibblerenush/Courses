package main

import "fmt"

func main() {
	var a, b int
	fmt.Scan(&a, &b)

	var isFound bool = false

	for i := b; i >= a; i-- {
		if i%7 == 0 {
			fmt.Println(i)
			isFound = true
			break
		}
	}

	if !isFound {
		fmt.Println("NO")
	}
}

package main

import "fmt"

func main() {
	var a int
	fmt.Scan(&a)
	var b int
	fmt.Scan(&b)

	var sum int = 0
	for i := a; i <= b; i++ {
		sum += i
	}

	fmt.Println(sum)
}

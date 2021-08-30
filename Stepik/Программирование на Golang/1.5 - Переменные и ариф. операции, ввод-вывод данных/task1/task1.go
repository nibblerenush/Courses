package main

import "fmt"

func main() {
	var a int = 0
	fmt.Scan(&a)
	a = a * 2
	a = a + 100
	fmt.Println(a)
}

package main

import "fmt"

func main() {
	var num int
	fmt.Scan(&num)

	var a int = num % 10
	var b int = num / 10 % 10
	var c int = num / 100

	fmt.Println(a + b + c)
}

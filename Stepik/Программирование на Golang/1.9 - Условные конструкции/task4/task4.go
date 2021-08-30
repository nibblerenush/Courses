package main

import "fmt"

func main() {
	var num int
	fmt.Scan(&num)

	var a int = num % 10
	var b int = num / 10 % 10
	var c int = num / 100 % 10

	var d int = num / 1000 % 10
	var e int = num / 10000 % 10
	var f int = num / 100000

	if (a + b + c) == (d + e + f) {
		fmt.Println("YES")
	} else {
		fmt.Println("NO")
	}
}

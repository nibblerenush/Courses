package main

import "fmt"

func main() {
	var num int
	fmt.Scan(&num)

	var a int = num % 10
	var b int = num / 10 % 10
	var c int = num / 100

	if a != b && a != c && b != c {
		fmt.Println("YES")
	} else {
		fmt.Println("NO")
	}
}

package main

import "fmt"

func main() {
	var a int
	fmt.Scan(&a)
	c := a / 10 % 10
	fmt.Println(c)
}

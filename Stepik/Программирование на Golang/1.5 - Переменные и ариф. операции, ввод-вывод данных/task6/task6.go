package main

import "fmt"

func main() {
	var a int
	fmt.Scan(&a)
	a = a * 2

	hours := a / 60
	minutes := a % 60

	fmt.Println("It is", hours, "hours", minutes, "minutes.")
}

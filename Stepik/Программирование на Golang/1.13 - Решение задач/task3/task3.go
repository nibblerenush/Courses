package main

import "fmt"

func main() {
	var k int
	fmt.Scan(&k)

	var hours int = k / 3600
	k %= 3600

	var minutes int = k / 60

	fmt.Println("It is", hours, "hours", minutes, "minutes.")
}

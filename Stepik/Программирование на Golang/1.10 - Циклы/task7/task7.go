package main

import "fmt"

func main() {
	var x, p, y int
	fmt.Scan(&x, &p, &y)

	var years int = 0
	for x < y {
		var add int = x * p / 100.0
		x += add
		years++
	}

	fmt.Println(years)
}

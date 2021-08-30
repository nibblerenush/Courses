package main

import "fmt"

func main() {
	var a, b int
	fmt.Scan(&a, &b)

	var result float64
	result = float64(a)/2.0 + float64(b)/2.0
	fmt.Println(result)
}

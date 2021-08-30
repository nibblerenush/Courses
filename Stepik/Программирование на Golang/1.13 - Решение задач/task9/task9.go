package main

import "fmt"

func main() {
	var n int
	fmt.Scan(&n)

	var m int = n
	for m/10 != 0 {
		n = m
		m = 0

		for n != 0 {
			m += n % 10
			n = n / 10
		}
	}

	fmt.Println(m)
}

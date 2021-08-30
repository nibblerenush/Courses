package main

import "fmt"

func main() {
	var a, b string
	fmt.Scan(&a, &b)

	for i := 0; i < len(a); i++ {
		for j := 0; j < len(b); j++ {
			if a[i] == b[j] {
				fmt.Print(string(a[i]), string(' '))
			}
		}
	}
}

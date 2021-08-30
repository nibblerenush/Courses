package main

import "fmt"

func main() {
	var num, digit int
	fmt.Scan(&num, &digit)

	var buffer []int
	for num != 0 {
		var mod int = num % 10
		if mod != digit {
			buffer = append(buffer, mod)
		}
		num /= 10
	}

	for i := len(buffer) - 1; i >= 0; i-- {
		fmt.Print(buffer[i])
	}
}

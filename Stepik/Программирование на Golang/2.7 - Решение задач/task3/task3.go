package main

import (
	"fmt"
)

func main() {
	var str string
	fmt.Scan(&str)

	var max byte = '0'
	for i := 0; i < len(str); i++ {
		if str[i] > max {
			max = str[i]
		}
	}

	fmt.Printf("%c", max)
}

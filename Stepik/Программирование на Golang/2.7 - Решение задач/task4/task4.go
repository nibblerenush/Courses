package main

import (
	"fmt"
)

func main() {
	var str string
	fmt.Scan(&str)

	for i := 0; i < len(str); i++ {
		var num byte = str[i] - '0'
		fmt.Print(num * num)
	}
}

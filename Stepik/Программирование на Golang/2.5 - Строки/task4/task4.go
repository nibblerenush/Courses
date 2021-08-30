package main

import (
	"fmt"
)

func main() {
	var str string
	fmt.Scan(&str)

	var result []byte
	for i := 0; i < len(str); i++ {
		if i%2 != 0 {
			result = append(result, str[i])
		}
	}

	fmt.Println(string(result))
}

package main

import (
	"fmt"
)

func main() {
	var str string
	fmt.Scan(&str)

	if len(str) == 1 {
		fmt.Println(str)
	} else {
		for i := 0; i < len(str)-1; i++ {
			fmt.Printf("%c%c", str[i], '*')
		}
		fmt.Printf("%c", str[len(str)-1])
	}
}

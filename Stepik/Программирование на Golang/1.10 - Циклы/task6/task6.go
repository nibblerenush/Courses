package main

import "fmt"

func main() {
	for {
		var num int
		fmt.Scan(&num)

		if num < 10 {
			continue
		}

		if num > 100 {
			break
		}

		fmt.Println(num)
	}
}

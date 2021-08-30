package main

import "fmt"

func main() {
	var n int
	fmt.Scan(&n)

	var sum int = 0
	for i := 0; i < n; i++ {
		var num int
		fmt.Scan(&num)

		if num >= 10 && num <= 99 && num%8 == 0 {
			sum += num
		}
	}

	fmt.Println(sum)
}

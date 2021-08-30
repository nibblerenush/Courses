package main

import "fmt"

func main() {
	var num int
	fmt.Scan(&num)

	switch {
	case num >= 0 && num <= 9:
		fmt.Println(num)
	case num >= 10 && num <= 99:
		fmt.Println(num / 10)
	case num >= 100 && num <= 999:
		fmt.Println(num / 100)
	case num >= 1000 && num <= 9999:
		fmt.Println(num / 1000)
	case num == 10000:
		fmt.Println(1)
	}
}

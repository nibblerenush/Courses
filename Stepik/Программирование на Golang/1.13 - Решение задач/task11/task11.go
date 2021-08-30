package main

import "fmt"

func main() {
	var n int
	fmt.Scan(&n)

	if n >= 11 && n <= 19 {
		fmt.Println(n, "korov")
	} else {
		switch n % 10 {
		case 1:
			fmt.Println(n, "korova")
		case 2:
			fallthrough
		case 3:
			fallthrough
		case 4:
			fmt.Println(n, "korovy")
		default:
			fmt.Println(n, "korov")
		}
	}
}

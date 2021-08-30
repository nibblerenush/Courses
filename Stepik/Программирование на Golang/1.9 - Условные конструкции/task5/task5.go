package main

import "fmt"

func main() {
	var num int
	fmt.Scan(&num)

	if (num%400 == 0) || (num%4 == 0 && num%100 != 0) {
		fmt.Println("YES")
	} else {
		fmt.Println("NO")
	}
}

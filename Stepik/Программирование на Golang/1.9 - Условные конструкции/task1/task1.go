package main

import "fmt"

func main() {
	var num int
	fmt.Scan(&num)

	if num > 0 {
		fmt.Println("Число положительное")
	} else if num < 0 {
		fmt.Println("Число отрицательное")
	} else {
		fmt.Println("Ноль")
	}
}

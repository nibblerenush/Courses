package main

import "fmt"

func main() {
	var workArray [10]uint8
	for i := 0; i < len(workArray); i++ {
		fmt.Scan(&workArray[i])
	}

	for i := 0; i < 3; i++ {
		var i, j int
		fmt.Scan(&i, &j)
		var temp uint8 = workArray[i]
		workArray[i] = workArray[j]
		workArray[j] = temp
	}

	for i := 0; i < len(workArray); i++ {
		fmt.Print(workArray[i], " ")
	}
}

package main

import "fmt"

func task2(c chan string, str string) {
	for i := 0; i < 5; i++ {
		c <- (str + " ")
	}
}

func main() {
	c := make(chan string, 5)
	task2(c, "41")

	for i := 0; i < 5; i++ {
		fmt.Println(<-c)
	}
}

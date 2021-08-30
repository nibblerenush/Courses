package main

import "fmt"

func task(c chan int, N int) {
	c <- (N + 1)
}

func main() {
	c := make(chan int, 5)
	task(c, 41)
	fmt.Println(<-c)
}

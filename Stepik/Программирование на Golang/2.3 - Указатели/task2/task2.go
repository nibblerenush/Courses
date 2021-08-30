package main

import "fmt"

func test(x1 *int, x2 *int) {
	var temp int = *x1
	*x1 = *x2
	*x2 = temp
	fmt.Println(*x1, *x2)
}

func main() {
	var x *int = new(int)
	*x = 2
	test(x, x)
}

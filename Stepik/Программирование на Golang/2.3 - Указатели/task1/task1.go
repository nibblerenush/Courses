package main

import "fmt"

// считайте что fmt уже импортирован и main объявлен
func test(x1 *int, x2 *int) {
	// здесь пишите ваш код
	var product int = (*x1) * (*x2)
	fmt.Println(product)
}

func main() {
	var x *int = new(int)
	*x = 2
	test(x, x)
}

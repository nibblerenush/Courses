package main

import (
	"fmt"
	"strconv"
	"unicode"
)

func adding(x string, y string) int64 {
	xr := []rune(x)
	yr := []rune(y)

	var xn, yn string

	for _, s := range xr {
		if unicode.IsDigit(s) {
			xn += string(s)
		}
	}

	for _, s := range yr {
		if unicode.IsDigit(s) {
			yn += string(s)
		}
	}

	a, _ := strconv.ParseInt(xn, 10, 64)
	b, _ := strconv.ParseInt(yn, 10, 64)
	return a + b
}

func main() {
	fmt.Println(adding("%^80", "hhhhh20&&&&nd"))
}

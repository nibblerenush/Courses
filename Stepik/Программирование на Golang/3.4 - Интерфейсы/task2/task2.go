package main

import (
	"fmt"
	"strings"
)

type charge_t int

func (c charge_t) String() string {
	var str []byte
	str = append(str, '[')

	for i := 0; i < 10-int(c); i++ {
		str = append(str, ' ')
	}

	for i := 0; i < int(c); i++ {
		str = append(str, 'X')
	}

	str = append(str, ']')
	return string(str)
}

func main() {
	var str string
	fmt.Scan(&str)

	var batteryForTest charge_t = charge_t(strings.Count(str, "1"))
	fmt.Println(batteryForTest)
}

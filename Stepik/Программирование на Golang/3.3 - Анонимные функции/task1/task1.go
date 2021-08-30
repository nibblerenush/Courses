package main

import "fmt"

func main() {
	fn := func(x uint) uint {
		var num []byte

		for x != 0 {
			var mod uint = x % 10
			x /= 10

			if mod%2 == 0 && mod != 0 {
				num = append(num, byte(mod))
			}
		}

		var res uint = 0
		for i := len(num) - 1; i >= 0; i-- {
			res *= 10
			res += uint(num[i])
		}

		if res == 0 {
			return 100
		}

		return res
	}

	fmt.Println(fn(727178))
}

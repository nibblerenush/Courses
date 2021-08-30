package main

import (
	"fmt"
	"strings"
)

func main() {
	var str string
	fmt.Scan(&str)

	var several_counts []byte
	for i := 0; i < len(str); i++ {
		if strings.Count(str, string(str[i])) != 1 {
			several_counts = append(several_counts, str[i])
		}
	}

	var result string = str
	for i := 0; i < len(several_counts); i++ {
		result = strings.ReplaceAll(result, string(several_counts[i]), "")
	}

	fmt.Println(result)
}

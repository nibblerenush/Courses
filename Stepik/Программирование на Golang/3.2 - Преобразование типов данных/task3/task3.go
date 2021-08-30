package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	scanner.Scan()
	s := scanner.Text()

	split_strs := strings.Split(s, ";")
	split_strs[0] = strings.ReplaceAll(split_strs[0], ",", ".")
	split_strs[0] = strings.ReplaceAll(split_strs[0], " ", "")

	split_strs[1] = strings.ReplaceAll(split_strs[1], ",", ".")
	split_strs[1] = strings.ReplaceAll(split_strs[1], " ", "")

	num1, _ := strconv.ParseFloat(split_strs[0], 64)
	num2, _ := strconv.ParseFloat(split_strs[1], 64)
	fmt.Printf("%.4f", num1/num2)
}

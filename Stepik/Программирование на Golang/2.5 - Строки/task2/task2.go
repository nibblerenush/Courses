package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"unicode"
)

func main() {
	text, _ := bufio.NewReader(os.Stdin).ReadString('\n')

	if len(text) == 0 {
		fmt.Println("Нет")
		return
	} else if text[len(text)-1] == '\n' {
		text = strings.Trim(text, "\n")
	}

	if len(text) == 0 {
		fmt.Println("Нет")
		return
	} else if text[len(text)-1] == '\r' {
		text = strings.Trim(text, "\r")
	}

	r := []rune(text)

	var i int = 0
	var j int = len(r) - 1

	for i < j {
		if unicode.ToUpper(r[i]) != unicode.ToUpper(r[j]) {
			fmt.Println("Нет")
			return
		}
		i++
		j--
	}
	fmt.Println("Палиндром")
}

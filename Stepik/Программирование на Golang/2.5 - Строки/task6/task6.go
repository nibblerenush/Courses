package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func main() {
	text, _ := bufio.NewReader(os.Stdin).ReadString('\n')

	if len(text) == 0 {
		fmt.Println("Wrong password")
		return
	} else if text[len(text)-1] == '\n' {
		text = strings.Trim(text, "\n")
	}

	if len(text) == 0 {
		fmt.Println("Wrong password")
		return
	} else if text[len(text)-1] == '\r' {
		text = strings.Trim(text, "\r")
	}

	r := []rune(text)

	for i := 0; i < len(r); i++ {
		if !((r[i] >= '0' && r[i] <= '9') || (r[i] >= 'A' && r[i] <= 'Z') || (r[i] >= 'a' && r[i] <= 'z')) {
			fmt.Println("Wrong password")
			return
		}
	}

	if len(r) < 5 {
		fmt.Println("Wrong password")
	} else {
		fmt.Println("Ok")
	}
}

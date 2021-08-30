package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"time"
)

func main() {
	text, _ := bufio.NewReader(os.Stdin).ReadString('\n')

	if len(text) == 0 {
		fmt.Println("Wrong")
		return
	} else if text[len(text)-1] == '\n' {
		text = strings.Trim(text, "\n")
	}

	if len(text) == 0 {
		fmt.Println("Wrong")
		return
	} else if text[len(text)-1] == '\r' {
		text = strings.Trim(text, "\r")
	}

	t_time, _ := time.Parse("2006-01-02 15:04:05", text)

	if t_time.Hour() >= 13 {
		t_time = t_time.AddDate(0, 0, 1)
	}

	fmt.Println(t_time.Format("2006-01-02 15:04:05"))
}

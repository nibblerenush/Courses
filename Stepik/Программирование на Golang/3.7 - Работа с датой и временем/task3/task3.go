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

	times_str := strings.Split(text, ",")

	//fmt.Println(times_str[0], times_str[1])

	t_time1, _ := time.Parse("02.01.2006 15:04:05", times_str[0])
	t_time2, _ := time.Parse("02.01.2006 15:04:05", times_str[1])

	// t1 > t2
	if t_time1.After(t_time2) {
		fmt.Println(t_time1.Sub(t_time2).Round(time.Second))
	} else {
		fmt.Println(t_time2.Sub(t_time1).Round(time.Second))
	}
}

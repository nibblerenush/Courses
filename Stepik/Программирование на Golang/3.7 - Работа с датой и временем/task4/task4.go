package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"time"
)

const now = 1589570165

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

	r := []rune(text)

	var parse_time []byte
	parse_time = append(parse_time, '0')
	parse_time = append(parse_time, 'h')

	for i := 0; i < len(r); i++ {
		if r[i] >= '0' && r[i] <= '9' {
			parse_time = append(parse_time, byte(r[i]))
		} else if r[i] == 'м' {
			parse_time = append(parse_time, 'm')
		}
	}

	parse_time = append(parse_time, 's')
	//fmt.Println(string(parse_time))
	dur, _ := time.ParseDuration(string(parse_time))
	time_t := time.Unix(1589570165, 0).UTC().Add(dur)
	fmt.Println(time_t.Format(time.UnixDate))
}

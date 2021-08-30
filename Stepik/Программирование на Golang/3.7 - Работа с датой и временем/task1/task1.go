package main

import (
	"fmt"
	"time"
)

func main() {
	var t_str string
	fmt.Scan(&t_str)

	t_time, _ := time.Parse(time.RFC3339, t_str)
	fmt.Println(t_time.Format(time.UnixDate))
}

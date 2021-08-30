package main

import (
	"bufio"
	"io"
	"os"
	"strconv"
)

func main() {
	reader := bufio.NewReader(os.Stdin)

	var result int64 = 0
	for true {
		str, err := reader.ReadString('\n')

		var num_str []byte
		for i := 0; i < len(str); i++ {
			if str[i] >= '0' && str[i] <= '9' {
				num_str = append(num_str, str[i])
			}
		}

		num, _ := strconv.ParseInt(string(num_str), 10, 64)
		result += num

		if err == io.EOF {
			break
		}
	}

	writer := bufio.NewWriter(os.Stdout)
	writer.WriteString(strconv.FormatInt(result, 10))
	writer.Flush()
}

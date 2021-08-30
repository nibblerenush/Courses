package main

func removeDuplicates(inputStream chan string, outputStream chan string) {
	var prev_str string = ""
	for str := range inputStream {
		if str != prev_str {
			outputStream <- str
			prev_str = str
		}
	}

	close(outputStream)
}

func main() {
}

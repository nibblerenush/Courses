package main

func calculator(arguments <-chan int, done <-chan struct{}) <-chan int {
	output := make(chan int)

	go func() {
		defer close(output)

		var sum int = 0

		for {
			select {
			case x := <-arguments:
				sum += x
			case <-done:
				output <- (sum)
				return
			}
		}

	}()

	return output
}

func main() {
}

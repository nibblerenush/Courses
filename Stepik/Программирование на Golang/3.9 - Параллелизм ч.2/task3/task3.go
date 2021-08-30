package main

func calculator(firstChan <-chan int, secondChan <-chan int, stopChan <-chan struct{}) <-chan int {
	output := make(chan int)

	go func() {
		defer close(output)

		select {
		case x := <-firstChan:
			output <- (x * x)
		case x := <-secondChan:
			output <- (3 * x)
		case <-stopChan:
			return
		}
	}()

	return output
}

func main() {
}

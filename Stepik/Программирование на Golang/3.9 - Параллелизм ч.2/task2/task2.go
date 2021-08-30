package main

import (
	"fmt"
	"sync"
)

func work() {
	fmt.Println("Work")
}

func main() {
	wg := new(sync.WaitGroup)

	for i := 0; i < 10; i++ {
		wg.Add(1)
		go func(wg *sync.WaitGroup) {
			defer wg.Done()
			work()
		}(wg)
	}

	wg.Wait()
}

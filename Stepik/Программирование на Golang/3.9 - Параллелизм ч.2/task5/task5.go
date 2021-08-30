package main

func merge2Channels(fn func(int) int, in1 <-chan int, in2 <-chan int, out chan<- int, n int) {
	go func() {
		y := make([]int, n, n)
		dones := make([]chan struct{}, n, n)
		for i := 0; i < n; i++ {
			dones[i] = make(chan struct{})
		}

		for i := 0; i < n; i++ {
			x1_in1 := <-in1
			x2_in2 := <-in2

			go func(index int, x1 int, x2 int) {
				defer close(dones[index])

				done1 := make(chan struct{})
				done2 := make(chan struct{})

				var y1 int
				var y2 int

				go func() {
					defer close(done1)
					y1 = fn(x1)
				}()

				go func() {
					defer close(done2)
					y2 = fn(x2)
				}()

				<-done1
				<-done2

				y[index] = y1 + y2
			}(i, x1_in1, x2_in2)
		}

		for i := 0; i < n; i++ {
			<-dones[i]
			out <- y[i]
		}

	}()
}

func main() {
}

package main

func main() {
	groupCity := map[int][]string{
		10:   []string{},
		100:  []string{},
		1000: []string{},
	}

	cityPopulation := map[string]int{}

	for _, city := range groupCity[10] {
		delete(cityPopulation, city)
	}

	for _, city := range groupCity[1000] {
		delete(cityPopulation, city)
	}
}

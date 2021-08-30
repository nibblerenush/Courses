package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"os"
)

type Student struct {
	LastName   string
	Firstname  string
	MiddleName string
	Birthday   string
	Address    string
	Phone      string
	Rating     []int
}

type Group struct {
	ID       int
	Number   string
	Year     int
	Students []Student
}

type Result struct {
	Average float64
}

func main() {
	//file, _ := os.Open("data.json")
	bytes, _ := ioutil.ReadAll(os.Stdin)

	var group Group
	json.Unmarshal(bytes, &group)

	var mark_count int = 0
	for i := 0; i < len(group.Students); i++ {
		mark_count += len(group.Students[i].Rating)
	}

	result := Result{float64(mark_count) / float64(len(group.Students))}
	result_bytes, _ := json.MarshalIndent(result, "", "    ")
	fmt.Println(string(result_bytes))
}

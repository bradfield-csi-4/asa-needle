package main

import (
	"fmt"
	"os"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func main() {
	fmt.Println("I am the PVM")
	f, err := os.Open("../survivors/1.pyc")
	check(err)
	defer f.Close()
	b1 := make([]byte, 100)
	n1, err := f.Read(b1)
	check(err)
	fmt.Printf("%d bytes: %d\n", n1, b1)
}

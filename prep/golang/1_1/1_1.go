package main

import (
	"fmt"
	"os"
)

func main() {
	var s, row_sep = "", ""
	for index, value := range os.Args[1:] {
		fmt.Printf("%s%d %s", row_sep, index, value)
		row_sep = "\n"
	}
	fmt.Println(s)
}

package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	counts := make(map[string]map[string]int)
	files := os.Args[1:]
	if len(files) == 0 {
		countLines(os.Stdin, counts, "")
	} else {
		for _, arg := range files {
			f, err := os.Open(arg)
			if err != nil {
				fmt.Fprintf(os.Stderr, "dup2: %v\n", err)
				continue
			}
			countLines(f, counts, arg)
			f.Close()
		}
	}

	for line, info := range counts {
		var file_names = ""
		var total_count = 0
		for file, count := range info {
			total_count += count
			file_names += file + " "
		}
		fmt.Printf("%d\t%s\t%s\n", total_count, line, file_names)
	}
}

func countLines(f *os.File, counts map[string]map[string]int, file_name string) {
	var new_file_name string
	if file_name == "" {
		new_file_name = "os args"
	} else {
		new_file_name = file_name
	}

	input := bufio.NewScanner(f)
	for input.Scan() {
		line := input.Text()
		if counts[line] == nil {
			counts[line] = make(map[string]int)
		}
		counts[line][new_file_name] += 1
		// bs, _ := json.Marshal(counts)
		// fmt.Println(string(bs))
	}
}

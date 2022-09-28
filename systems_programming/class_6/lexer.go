package main

import "strings"

func lexer(s string) ([]string, error) {
	return strings.Split(s, " "), nil
}

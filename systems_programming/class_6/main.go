package main

import (
	"fmt"
)

var grammar1String = "OR ( AND ( TERM ( hello ) TERM ( world ) ) AND ( TERM ( alice ) NOT ( TERM ( BOB ) ) )"

func main() {
	s, err := lexer(grammar1String)
	if err != nil {
		fmt.Println(err)
	}
	fmt.Printf("lexed list %x\n", s)
	p, err := parse(s)
	if err != nil {
		fmt.Println(err)
	}
	fmt.Printf("parsed tree %x\n", p)
}

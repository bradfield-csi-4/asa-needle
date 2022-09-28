package main

import (
	"fmt"
)

type Stmt interface {
	isStmt()
}

type AndStmt struct {
	stmts map[int]Stmt
}

type OrStmt struct {
	stmts map[int]Stmt
}

type NotStmt struct {
	stmt Stmt
}

type TermStmt struct {
	str string
}

func (*AndStmt) isStmt()  {}
func (*OrStmt) isStmt()   {}
func (*NotStmt) isStmt()  {}
func (*TermStmt) isStmt() {}

/*
	Receives a lexed list of strings, and returns the Head statement of an abstract syntax tree
*/
func parse(str []string) (Stmt, error) {
	parsed, err := parseStmt(preParseStmt{
		body: str[2 : len(str)-1],
		name: str[0],
	})
	if err != nil {
		return nil, err
	}
	return parsed, nil
}

type preParseStmt struct {
	body []string
	name string
}

func splitLogicList(str []string) []preParseStmt {
	fmt.Printf("%x", str)

	parenCount := 0
	name := ""

	stmts := make([]preParseStmt, 0)

	startIndex := 0

	for i, chr := range str {
		if chr == "(" {
			if parenCount == 0 {
				startIndex = i + 1
				name = str[i-1]
			}
			parenCount += 1
		} else if chr == ")" {
			parenCount -= 1
			if parenCount == 0 {
				stmts = append(stmts, preParseStmt{
					body: str[startIndex:i],
					name: name,
				})
			}
		}
	}
	return stmts
}

func parseStmt(ppStmt preParseStmt) (Stmt, error) {
	switch ppStmt.name {
	case "AND":
		return parseAnd(ppStmt.body)
	case "OR":
		return parseOr(ppStmt.body)
	case "NOT":
		return parseNot(ppStmt.body)
	case "TERM":
		return parseTerm(ppStmt.body)
	default:
		return nil, fmt.Errorf("invalid keyword")
	}
}

func parseAnd(slice []string) (*AndStmt, error) {
	andStmt := AndStmt{
		stmts: make(map[int]Stmt, 0),
	}
	stmts := splitLogicList(slice)
	for _, stmt := range stmts {
		parsed, err := parseStmt(stmt)
		if err != nil {
			return nil, fmt.Errorf("parse not: %e", err)
		}
		andStmt.stmts[len(andStmt.stmts)] = parsed
	}
	return &andStmt, nil
}

func parseOr(slice []string) (*OrStmt, error) {
	orStmt := OrStmt{
		stmts: make(map[int]Stmt, 0),
	}
	stmts := splitLogicList(slice)
	for _, stmt := range stmts {
		parsed, err := parseStmt(stmt)
		if err != nil {
			return nil, fmt.Errorf("parse not: %e", err)
		}
		orStmt.stmts[len(orStmt.stmts)] = parsed
	}
	return &orStmt, nil
}

func parseNot(slice []string) (*NotStmt, error) {
	stmt, err := parse(slice)
	if err != nil {
		return &NotStmt{stmt: stmt}, nil
	}
	return nil, fmt.Errorf("parse not: %e", err)
}

func parseTerm(slice []string) (*TermStmt, error) {
	if len(slice) != 1 {
		return nil, fmt.Errorf("usage: Term ( STRING )")
	}
	return &TermStmt{str: slice[0]}, nil
}

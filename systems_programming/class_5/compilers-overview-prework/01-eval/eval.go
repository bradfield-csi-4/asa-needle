package main

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/token"
	"log"
	"strconv"
)

// Given an expression containing only int types, evaluate
// the expression and return the result.
func Evaluate(expr ast.Expr) (int, error) {

	var iexpr interface{} = expr
	if _, ok := iexpr.(*ast.BasicLit); ok {
		myString := expr.(*ast.BasicLit).Value
		basicVal, err := strconv.Atoi(myString)
		if err != nil {
			return 0, fmt.Errorf("failed to convert value to int")
		}
		return basicVal, nil
	} else if _, ok := iexpr.(*ast.ParenExpr); ok {
		valX, errX := Evaluate(expr.(*ast.ParenExpr).X)
		if errX != nil {
			return 0, fmt.Errorf("x: %e", errX)
		}
		return valX, nil
	} else if _, ok := iexpr.(*ast.BinaryExpr); ok {

		valX, errX := Evaluate(expr.(*ast.BinaryExpr).X)
		valY, errY := Evaluate(expr.(*ast.BinaryExpr).Y)

		if errX != nil || errY != nil {
			return 0, fmt.Errorf("x: %e, y: %e", errX, errY)
		}
		switch expr.(*ast.BinaryExpr).Op.String() {
		case "+":
			return valX + valY, nil
		case "-":
			return valX - valY, nil
		case "*":
			return valX * valY, nil
		case "/":
			return valX / valY, nil
		default:
			return 0, fmt.Errorf("unrecognized operator")
		}
	}
	return 0, fmt.Errorf("encountered node that was not a recognized type")
}

func main() {

	fset := token.NewFileSet()
	expr, err := parser.ParseExprFrom(fset, "", "2 * (3 - 4 * (5 + 6) + 7)", 0)
	if err != nil {
		log.Fatal(err)
	}
	ast.Print(fset, expr)
}

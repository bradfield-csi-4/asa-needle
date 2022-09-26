package main

import (
	"fmt"
	"go/ast"
	"strconv"
)

var labelCounter int
var localVariables = make(map[string]int)

// Given an AST node corresponding to a function (guaranteed to be
// of the form `func f(x, y byte) byte`), compile it into assembly
// code.
//
// Recall from the README that the input parameters `x` and `y` should
// be read from memory addresses `1` and `2`, and the return value
// should be written to memory address `0`.
func compileExpr(expr ast.Expr) (string, error) {
	assembly := ""
	if _, ok := expr.(*ast.Ident); ok {
		if val, ok := localVariables[expr.(*ast.Ident).Name]; ok {
			assembly += "push " + strconv.Itoa(val) + "\n"
		} else {
			return "", fmt.Errorf("assignment to unknown variable")
		}
	} else if _, ok := expr.(*ast.BasicLit); ok {
		value := expr.(*ast.BasicLit).Value
		assembly += "pushi " + value + "\n"

	} else if _, ok := expr.(*ast.BinaryExpr); ok {
		asmX, err := compileExpr(expr.(*ast.BinaryExpr).X)
		if err != nil {
			return "", fmt.Errorf("compiling x error: %e", err)
		}
		assembly += asmX
		asmY, err := compileExpr(expr.(*ast.BinaryExpr).Y)
		if err != nil {
			return "", fmt.Errorf("compiling y error: %e", err)
		}
		assembly += asmY
		switch expr.(*ast.BinaryExpr).Op.String() {
		case "+":
			assembly += "add\n"
		case "-":
			assembly += "sub\n"
		case "*":
			assembly += "mul\n"
		case "/":
			assembly += "div\n"
		case "==":
			assembly += "eq\n"
		case ">":
			assembly += "gt\n"
		case "<":
			assembly += "lt\n"
		default:
			return "", fmt.Errorf("unrecognized operator")
		}
	} else if _, ok := expr.(*ast.ParenExpr); ok {
		asmX, err := compileExpr(expr.(*ast.ParenExpr).X)
		if err != nil {
			return "", fmt.Errorf("compiling paren expr error: %e", err)
		} else {
			assembly += asmX
		}
	}
	return assembly, nil
}

func compileStmt(stmt ast.Stmt) (string, error) {
	var istmt interface{} = stmt
	assembly := ""
	if _, ok := istmt.(*ast.AssignStmt); ok {
		// assume that assignment expression has one node on left hand side
		assigned := istmt.(*ast.AssignStmt).Rhs[0]
		assignedAsm, err := compileExpr(assigned)
		if err != nil {
			return "", fmt.Errorf("compiling x error: %e", err)
		}
		assembly += assignedAsm
		varName := istmt.(*ast.AssignStmt).Lhs[0].(*ast.Ident).Name
		// assume: that only assignments are to x and y
		if val, ok := localVariables[varName]; ok {
			assembly += "pop " + strconv.Itoa(val) + "\n"
		} else {
			return "", fmt.Errorf("assignment to unknown variable")
		}
	} else if _, ok := istmt.(*ast.ReturnStmt); ok {
		results := istmt.(*ast.ReturnStmt).Results
		returnAsm, err := compileExpr(results[0])
		if err != nil {
			return "", fmt.Errorf("return stmt error: %e", err)
		}
		assembly += returnAsm + "pop 0\n"
	} else if _, ok := istmt.(*ast.IfStmt); ok {

		blockEndLabel := ".L" + strconv.Itoa(labelCounter)
		labelCounter += 1
		elseEndLabel := ".L" + strconv.Itoa(labelCounter)
		labelCounter += 1
		cond := istmt.(*ast.IfStmt).Cond
		condAsm, err := compileExpr(cond)
		if err != nil {
			return "", fmt.Errorf("return cond error: %e", err)
		}
		assembly += condAsm
		assembly += "jeqz " + blockEndLabel + "\n"
		condBlockAsm, err := compileBlock(istmt.(*ast.IfStmt).Body)
		if err != nil {
			return "", fmt.Errorf("return if body error: %e", err)
		}
		assembly += condBlockAsm
		assembly += "jump " + elseEndLabel + "\n"
		assembly += "label " + blockEndLabel + "\n"

		// assume: if block has else statement
		elseBlockAsm, err := compileBlock(istmt.(*ast.IfStmt).Else.(*ast.BlockStmt))
		if err != nil {
			return "", fmt.Errorf("return if else error: %e", err)
		}
		assembly += elseBlockAsm
		assembly += "label " + elseEndLabel + "\n"
	} else if _, ok := istmt.(*ast.DeclStmt); ok {
		varName := istmt.(*ast.DeclStmt).Decl.(*ast.GenDecl).Specs[0].(*ast.ValueSpec).Names[0].Name
		nextFreeRegiser := len(localVariables) + 1
		if nextFreeRegiser > 7 {
			return "", fmt.Errorf("not enough space for local variable")
		}
		localVariables[varName] = nextFreeRegiser
	} else if _, ok := istmt.(*ast.ForStmt); ok {
		condLabel := ".L" + strconv.Itoa(labelCounter)
		labelCounter += 1
		endLabel := ".L" + strconv.Itoa(labelCounter)
		labelCounter += 1

		assembly += "label " + condLabel + "\n"
		condAsm, err := compileExpr(istmt.(*ast.ForStmt).Cond)
		if err != nil {
			return "", fmt.Errorf("for loop condition error: %e", err)
		}
		assembly += condAsm
		assembly += "jeqz " + endLabel + "\n"
		forBlockAsm, err := compileBlock(istmt.(*ast.ForStmt).Body)
		if err != nil {
			return "", fmt.Errorf("for loop body error: %e", err)
		}
		assembly += forBlockAsm
		assembly += "jump " + condLabel + "\n"
		assembly += "label " + endLabel + "\n"
	}
	return assembly, nil
}

func compileBlock(block *ast.BlockStmt) (string, error) {
	assembly := ""
	for _, stmt := range block.List {
		lineAsm, err := compileStmt(stmt)
		if err != nil {
			return "", fmt.Errorf("failed to compile line: %e", err)
		}
		assembly += lineAsm
	}
	return assembly, nil
}

func compile(node *ast.FuncDecl) (string, error) {
	assembly := ""
	labelCounter = 1
	localVariables["x"] = 1
	localVariables["y"] = 2
	blockAsm, err := compileBlock(node.Body)
	if err != nil {
		return "", fmt.Errorf("failed to compile block: %e", err)
	}
	assembly += blockAsm
	return assembly + "halt\n", nil
}

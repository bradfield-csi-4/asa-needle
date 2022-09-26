package main

import (
	"bytes"
	"log"
	"os"
	"sort"
	"strings"

	"github.com/dave/dst"
	"github.com/dave/dst/decorator"
)

const src string = `package foo

import (
	"fmt"
	"time"
)

func baz() {
	fmt.Println("Hello, world!")
}

type A int

const b = "testing"

func bar() {
	fmt.Println(time.Now())
}`

// Moves all top-level functions to the end, sorted in alphabetical order.
// The "source file" is given as a string (rather than e.g. a filename).
func SortFunctions(src string) (string, error) {
	f, err := decorator.Parse(src)
	if err != nil {
		log.Fatal(err)
	}
	declsSlice := f.Decls[:]
	sort.Slice(declsSlice, func(i, j int) bool {
		var idecl interface{} = f.Decls[i]
		var jdecl interface{} = f.Decls[j]
		_, isFuncI := idecl.(*dst.FuncDecl)
		_, isFuncJ := jdecl.(*dst.FuncDecl)
		if isFuncI && isFuncJ {
			val := strings.Compare(f.Decls[i].(*dst.FuncDecl).Name.Name, f.Decls[j].(*dst.FuncDecl).Name.Name)
			if val == 1 {
				return false
			} else {
				return true
			}
		}
		return !isFuncI && isFuncJ
	})
	f.Decls = declsSlice
	var output bytes.Buffer
	decorator.Fprint(&output, f)
	return output.String(), nil
}

func main() {
	f, err := decorator.Parse(src)
	if err != nil {
		log.Fatal(err)
	}

	// Print AST
	err = dst.Fprint(os.Stdout, f, nil)
	if err != nil {
		log.Fatal(err)
	}

	// Convert AST back to source
	err = decorator.Print(f)
	if err != nil {
		log.Fatal(err)
	}
}

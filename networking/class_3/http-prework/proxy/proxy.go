package main

import (
	"bufio"
	"fmt"
	"net"
	"strings"
)

const (
	response = `HTTP/1.1 200 OK
Date: Mon, 27 Jul 2009 12:28:53 GMT
Server: Something
Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
Content-Length: %s
Content-Type: text/html
Connection: Closed

<html>
<body>
<h1>%s</h1>
</body>
</html>
`
)

func getResponse(status string) string {
	return fmt.Sprintf(response, fmt.Sprint(39+len(status)), status)
}

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func main() {
	ln, err := net.Listen("tcp", ":8080")
	check(err)

	cache := make(map[string]string)
	cache_key := make(chan string)
	cache_val := make(chan string)

	for {
		dest_conn, err := net.Dial("tcp", ":9988")
		check(err)
		src_conn, err := ln.Accept()
		check(err)
		go handleConnection(src_conn, dest_conn, cache, cache_key, cache_val)
		key := <-cache_key
		val := <-cache_val
		if key != "" {
			cache[key] = val
		}
	}
}

func handleConnection(src_conn net.Conn, dest_conn net.Conn, cache map[string]string, cache_key chan string, cache_val chan string) {
	defer src_conn.Close()
	defer dest_conn.Close()

	fmt.Println(src_conn.RemoteAddr())

	request := make([]byte, 4096)
	_, err := src_conn.Read(request)
	check(err)

	string_request := string(request)
	path := strings.Split(string_request, " ")[1]

	respText, ok := cache[path]
	if !ok {
		fmt.Println("cache miss")
		fmt.Fprint(dest_conn, string(request))
		respText, err = bufio.NewReader(dest_conn).ReadString('\n')
		check(err)
		cache_key <- path
		cache_val <- respText
	} else {
		cache_key <- ""
		cache_val <- ""
		fmt.Println("cache hit")
	}
	response := getResponse(respText)
	fmt.Fprint(src_conn, response)
	fmt.Println(respText)
}

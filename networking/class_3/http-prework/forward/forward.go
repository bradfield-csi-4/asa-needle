package main

import (
	"bufio"
	"fmt"
	"net"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func main() {
	ln, err := net.Listen("tcp", ":8080")
	check(err)
	server_conn, err := net.Dial("tcp", ":1234")
	check(err)
	for {
		conn, err := ln.Accept()
		check(err)
		go handleConnection(conn, server_conn)
	}
}

func handleConnection(conn net.Conn, server_conn net.Conn) {
	defer conn.Close()
	data := make([]byte, 4096)
	_, err := conn.Read(data)
	check(err)
	data_to_send := string(data)
	check(err)

	fmt.Fprint(server_conn, string(data_to_send))
	status, err := bufio.NewReader(server_conn).ReadString('\n')
	check(err)
	fmt.Println(status)
}

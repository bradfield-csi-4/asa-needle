package main

import (
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
	for {
		conn, err := ln.Accept()
		check(err)
		go handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	defer conn.Close()
	data := make([]byte, 4096)
	_, err := conn.Read(data)
	check(err)
	data_to_send := string(data)
	check(err)

	conn.Write([]byte(data_to_send))
}

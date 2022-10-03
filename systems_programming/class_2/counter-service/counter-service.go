package main

import (
	"fmt"
	"sync"
	"sync/atomic"
)

type CounterService interface {
	// Returns values in ascending order; it should be safe to call
	// getNext() concurrently from multiple goroutines without any
	// additional synchronization.
	getNext() uint64
}

type NoSyncCounter struct {
	val uint64
}

func (c *NoSyncCounter) getNext() uint64 {
	c.val = c.val + 1
	return c.val
}

type SyncAtomicCounter struct {
	val uint64
}

func (c *SyncAtomicCounter) getNext() uint64 {
	return atomic.AddUint64(&c.val, 1)
}

type MutexCounter struct {
	val uint64
	mu  sync.Mutex
}

func (c *MutexCounter) getNext() uint64 {
	c.mu.Lock()
	defer c.mu.Unlock()
	c.val = c.val + 1
	return c.val
}

type ChanCounter struct {
	val      uint64
	receiver chan uint64
	sender   chan int
}

func (c *ChanCounter) getNext() uint64 {
	c.sender <- 0
	return <-c.receiver
}

func nextChan(c *ChanCounter, receiver chan uint64, sender chan int) {
	for {
		<-sender
		c.val = c.val + 1
		receiver <- c.val
	}
}

func handler(counter CounterService) {
	defer wg.Done()
	counter.getNext()
	val := counter.getNext()
	fmt.Printf("%d\n", val)
}

var wg sync.WaitGroup

func main() {
	wg.Add(100)
	var counter ChanCounter
	counter.val = 0
	counter.receiver = make(chan uint64)
	counter.sender = make(chan int)
	go nextChan(&counter, counter.receiver, counter.sender)
	for i := 0; i < 50; i++ {
		go handler(&counter)
		go handler(&counter)
	}
	wg.Wait()
}

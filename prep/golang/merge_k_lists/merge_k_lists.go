/**
 * Definition for singly-linked list.
 * type ListNode struct {
 *     Val int
 *     Next *ListNode
 * }
 */

/** brute force
* Write function to merge two lists
* apply successively to each list in array
* Time Complexity of merge function: O(n)
* n = number of items
* k = number of lists
* Each item will have to be touched on average k/2 times
* So, overall Time Complexity is O(n*k)
 */

package main

import (
	"fmt"
)

type ListNode struct {
	Val  int
	Next *ListNode
}

func peekNextVal(list1 *ListNode) int {
	return (*(*list1).Next).Val
}

func mergeTwoLists(p1 *ListNode, p2 *ListNode) *ListNode {

	var prefixPtr *ListNode

	if (*p1).Val >= (*p2).Val {
		prefixPtr = p2
		for (*p1).Val >= peekNextVal(prefixPtr) {
			prefixPtr = (*prefixPtr).Next
		}
		(*prefixPtr).Next = p1
	}

	for (*p1).Next != nil && (*p2).Next != nil {
		nextVal := peekNextVal(p1)
		// If next value in list 1 is higher than value in list 2
		if nextVal >= (*p2).Val {
			// add list 2 node to list 1
			newNode := p2
			(*newNode).Next = (*p1).Next
			(*p1).Next = newNode
			// set pointer 2 to next node in list 2
			p2 = (*p2).Next
		} else {
			if (*p1).Next == nil {
				(*p1).Next = p2
			}
			p1 = (*p1).Next
		}
	}
	return p1
}

func main() {
	p1 := &ListNode{1,
		&ListNode{4,
			&ListNode{
				5, nil,
			},
		},
	}
	p2 := &ListNode{1,
		&ListNode{3,
			&ListNode{
				4, nil,
			},
		},
	}

	p3 := mergeTwoLists(p1, p2)

	for (*p3).Next != nil {
		fmt.Printf("%d\n", (*p3).Val)
		p3 = (*p3).Next
	}
	fmt.Printf("%d\n", (*p3).Val)

}

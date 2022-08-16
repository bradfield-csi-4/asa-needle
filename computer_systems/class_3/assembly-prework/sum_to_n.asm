section .text
global sum_to_n

sum_to_n: 
    mov qword r10, 0 ; add total to r10
    mov qword r11, rdi ; add counter to r11
LOOP_START:
	cmp qword r11, 0
	je LOOP_END;
    add qword r10, r11 ; add counter to total
    sub qword r11, 1 ; increment counter
    jmp LOOP_START;
LOOP_END:
    mov qword rax, r10; move total to output
    ret


; O(1) solution
; sum_to_n: 
;   	mov qword rax, rdi ; add n to r10
; 	add qword rax, 1 ; add 1
; 	imul qword rax, rdi ; times n
; 	shr qword rax, 1 ; divide by two
;   ret

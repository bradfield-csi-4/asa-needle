section .text
global fib
fib:
	push rsi        ; rsi and rbx will get overwritten by callee, so need to push to stack
	push rbx        ; they preserve n and fib(n-1) respectively
	mov rsi, rdi    ; free up rdi to use as argument for next call of fib
	mov rax, rdi    ; store base case
	cmp rsi, 1      ; return base case if less than 2
	jle .return;
	lea rdi, [rsi-1]; load n-1 into first arg register 
	call fib        ; fib(n-1)
	mov rbx, rax    ; store result of fib(n-1)
	lea rdi, [rsi-2]; load n-2 into first arg register 
	call fib        ; fib(n-2)
	add rax, rbx;   ;add fib(n-1), fib(n-2)
.return:
	pop rbx;
	pop rsi;
	ret




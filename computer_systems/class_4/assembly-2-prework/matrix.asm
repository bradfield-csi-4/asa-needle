section .text
global index
index:
	; rdi: matrix
	; rsi: rows
	; rdx: cols
	; rcx: rindex
	; r8: cindex
	; matrix + L[cols*rindex+cindex]
	imul rdx, rcx;
	add rdx, r8;
	imul rdx, 4;
	add rdx, rdi;
	mov rax, [rdx];
	ret
section .text
global pangram

; A-Z, 65 - 90, 01000001 - 01011010
; a-z, 97 - 122 01100001 - 01111010

pangram:		
		mov rax, 0 					; final boolean result
		mov r10, 0  				; check letters seen
		mov rsi, 0x3ffffff 			; mask
		mov rdx, 0					; store current letter
START_LOOP:
		mov r11, 1 					; represents the letter a/A  
		movzx edx, byte [rdi] 			; move letter to rdx
		add qword rdi, 1 			; increment string input pointer
		cmp rdx, byte 0x00			; if end of string, return result
		je STORE_RESULT 			;
		and byte dl, 31 			; reduce ascii characters to ordinal system where a/A = 1 and z/Z = 27
SHIFT_LOOP:	
		sub rdx, byte 1				; shift encoding so that a/A = 0 and z/Z = 26
		cmp byte dl, 0x00 			; if it is a/A, then it will leave r11 as is at 1, else
		je INSERT_LETTER 			; 
		shl r11,  1					; ...it will shift the 1 bit left equal to the letters distance from a/A
		jmp SHIFT_LOOP;
INSERT_LETTER:
		or r10, r11 				; add letter if it doesn't exist to r10 
		jmp START_LOOP;
STORE_RESULT:
		and r10,  rsi				; remove irrelevant data
		cmp r10, rsi 				; return false if r10 does not contain every letter, else
		jne END;
		mov rax, 1 					; ...return true
END: 	
		ret
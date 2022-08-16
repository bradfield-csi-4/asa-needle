section .text
global binary_convert

binary_convert:
			mov rax, 0;
START_LOOP:
			add qword rdi, 1;
			cmp byte [rdi-1], 0x00;
			je END;
			shl rax, 1;
			cmp byte [rdi-1], 48;
			je START_LOOP;
			add rax, 1;
			jmp START_LOOP;
END: 		ret

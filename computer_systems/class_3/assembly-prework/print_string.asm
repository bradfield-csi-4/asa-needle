
          global    _main

          section   .text
_main:    mov       rax, 0x02000004         ; system call for write
          mov       rsi, rdi                ; address of string to output
          mov       rdi, 1                  ; file handle 1 is stdout

          mov       rdx, 13                 ; number of bytes
          syscall                           ; invoke operating system to do the write
          mov       rax, 0x02000001         ; system call for exit
          xor       rdi, rdi                ; exit code 0
          syscall                           ; invoke operating system to exit

          section   .data
message:  db        "Hello, World", 10      ; note the newline at the end
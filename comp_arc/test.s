.intel syntax
.text
.globl main
.type main, @function
main:
/* function prologue */
push %rbp
mov %rbp, %rsp
/* call puts("Hello, World!") */
mov %rdi, OFFSET FLAT:main.S 0
call puts
/* return zero */
mov %rax, 0
mov %rsp, %rbp
pop %rbp
ret
main.end:
.size main, .-main
.section .rodata
main.S 0:
.string "Hello, World!"
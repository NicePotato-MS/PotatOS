.globl stack
.comm stack, 0x8000, 16

.section .text
.globl _start
.type _start, @function
_start:
    lea stack + 0x8000 - 8(%rip), %rsp
    jmp _kmain

.section .note-GNU-stack
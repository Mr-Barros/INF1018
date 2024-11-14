

.globl main
main:
pushq %rbp
movq %rsp, %rbp

call *%rdi

leave
ret

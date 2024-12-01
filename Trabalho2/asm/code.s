

.text
.globl code
code:
    pushq   %rbp
    movq    %rsp, %rbp
    
    movl    0x1, %edi

    movq    0x00000000000000da, %rax
    call    *%rax

    leave
    ret

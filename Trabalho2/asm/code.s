

.text
.globl code
code:
    pushq   %rbp
    movq    %rsp, %rbp

    movl    %edi, %edi
    movq    %rdi, %rdi
    movl    %esi, %esi
    movq    %rsi, %rsi

    movl    $12, %ebx
    call    *%rdi           /* chama funcao original */

    leave
    ret

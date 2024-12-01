

.text
.globl code
code:
    pushq   %rbp
    movq    %rsp, %rbp
    
    movl    $0x1, %edi

    movq    $3000000000, %rdi
    movq    $3000000000, %rax

    movq    $0x3000000000, %r11
    movl    (%r11), %edi
    movq    (%r11), %rdi

    movl    (%r11), %r8d
    movl    (%r11), %r9d
    movl    (%r11), %r10d

    movq    (%r11), %r8
    movq    (%r11), %r9
    movq    (%r11), %r10

    movq  %r8, %rdi

    movq  %r9, %rsi

    movq  %r10, %rdx

    movq  %rdi, %r8

    movq  %rsi, %r9

    movq  %rdx, %r10

    call    *%rax

    leave
    ret

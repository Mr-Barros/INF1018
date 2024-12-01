

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

    call    *%rax

    leave
    ret

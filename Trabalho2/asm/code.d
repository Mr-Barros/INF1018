
code.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <code>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	8b 3c 25 01 00 00 00 	mov    0x1,%edi
   b:	48 8b 04 25 da 00 00 	mov    0xda,%rax
  12:	00 
  13:	ff d0                	call   *%rax
  15:	c9                   	leave
  16:	c3                   	ret


code.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <code>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	48 8b 04 25 da 00 00 	mov    0xda,%rax
   b:	00 
   c:	ff d0                	call   *%rax
   e:	c9                   	leave
   f:	c3                   	ret

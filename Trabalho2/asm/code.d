
asm/code.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <code>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	bf 01 00 00 00       	mov    $0x1,%edi
   9:	48 bf 00 5e d0 b2 00 	movabs $0xb2d05e00,%rdi
  10:	00 00 00 
  13:	48 b8 00 5e d0 b2 00 	movabs $0xb2d05e00,%rax
  1a:	00 00 00 
  1d:	49 bb 00 00 00 00 30 	movabs $0x3000000000,%r11
  24:	00 00 00 
  27:	41 8b 3b             	mov    (%r11),%edi
  2a:	49 8b 3b             	mov    (%r11),%rdi
  2d:	ff d0                	call   *%rax
  2f:	c9                   	leave
  30:	c3                   	ret

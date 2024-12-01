
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
  2d:	45 8b 03             	mov    (%r11),%r8d
  30:	45 8b 0b             	mov    (%r11),%r9d
  33:	45 8b 13             	mov    (%r11),%r10d
  36:	4d 8b 03             	mov    (%r11),%r8
  39:	4d 8b 0b             	mov    (%r11),%r9
  3c:	4d 8b 13             	mov    (%r11),%r10
  3f:	4c 89 c7             	mov    %r8,%rdi
  42:	4c 89 ce             	mov    %r9,%rsi
  45:	4c 89 d2             	mov    %r10,%rdx
  48:	49 89 f8             	mov    %rdi,%r8
  4b:	49 89 f1             	mov    %rsi,%r9
  4e:	49 89 d2             	mov    %rdx,%r10
  51:	ff d0                	call   *%rax
  53:	c9                   	leave
  54:	c3                   	ret

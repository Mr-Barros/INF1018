
code.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <code>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	89 ff                	mov    %edi,%edi
   6:	48 89 ff             	mov    %rdi,%rdi
   9:	89 f6                	mov    %esi,%esi
   b:	48 89 f6             	mov    %rsi,%rsi
   e:	bb 0c 00 00 00       	mov    $0xc,%ebx
  13:	ff d7                	call   *%rdi
  15:	c9                   	leave
  16:	c3                   	ret

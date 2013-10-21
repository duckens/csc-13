
bin/hello_syscall:     file format elf32-i386


Disassembly of section .text:

08048074 <_start>:
 8048074:	ba 0e 00 00 00       	mov    $0xe,%edx
 8048079:	b9 98 90 04 08       	mov    $0x8049098,%ecx
 804807e:	bb 01 00 00 00       	mov    $0x1,%ebx
 8048083:	b8 04 00 00 00       	mov    $0x4,%eax
 8048088:	cd 80                	int    $0x80
 804808a:	bb 00 00 00 00       	mov    $0x0,%ebx
 804808f:	b8 01 00 00 00       	mov    $0x1,%eax
 8048094:	cd 80                	int    $0x80

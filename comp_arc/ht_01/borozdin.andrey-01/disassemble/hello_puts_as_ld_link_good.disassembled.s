
bin/hello_puts_as_ld_link_good:     file format elf32-i386


Disassembly of section .plt:

08048170 <puts@plt-0x10>:
 8048170:	ff 35 50 92 04 08    	pushl  0x8049250
 8048176:	ff 25 54 92 04 08    	jmp    *0x8049254
 804817c:	00 00                	add    %al,(%eax)
	...

08048180 <puts@plt>:
 8048180:	ff 25 58 92 04 08    	jmp    *0x8049258
 8048186:	68 00 00 00 00       	push   $0x0
 804818b:	e9 e0 ff ff ff       	jmp    8048170 <len+0x8048163>

Disassembly of section .text:

08048190 <_start>:
 8048190:	55                   	push   %ebp
 8048191:	89 e5                	mov    %esp,%ebp
 8048193:	68 5c 92 04 08       	push   $0x804925c
 8048198:	e8 e3 ff ff ff       	call   8048180 <puts@plt>
 804819d:	89 ec                	mov    %ebp,%esp
 804819f:	5d                   	pop    %ebp
 80481a0:	bb 03 00 00 00       	mov    $0x3,%ebx
 80481a5:	b8 01 00 00 00       	mov    $0x1,%eax
 80481aa:	cd 80                	int    $0x80

.global asm_func


.text
						

asm_func:

	push	%ebp
	movl	%esp, %ebp
	push	$msg
	call	puts
	movl	%ebp, %esp
	pop		%ebp
	
	
	mov		$6, %eax
	lgdt	%eax
	ret



#	movl	$3, %ebx
#	movl	$1, %eax
#	int		$0x80

.data

msg:
	.ascii    "Hello, world!"
	len = . - msg

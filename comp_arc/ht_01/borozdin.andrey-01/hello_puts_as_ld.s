.global _start			# we must export the entry point to the ELF linker or
						# loader. They conventionally recognize _start as their
						# entry point. Use ld -e foo to override the default.
.extern  puts

.text					# section declaration
						

_start:

	push	%ebp
	movl	%esp, %ebp
	push	$msg
	call	puts
	movl	%ebp, %esp
	pop		%ebp


	movl	$3, %ebx
	movl	$1, %eax
	int		$0x80

.data

msg:
	.ascii    "Hello, world!"
	len = . - msg

	.file	"main.cpp"
	.section	.text._ZN1A3fooEv,"axG",@progbits,_ZN1A3fooEv,comdat
	.align 2
	.weak	_ZN1A3fooEv
	.type	_ZN1A3fooEv, @function
_ZN1A3fooEv:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	leal	73(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, (%eax)
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	_ZN1A3fooEv, .-_ZN1A3fooEv
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$32, %esp
	leal	20(%esp), %eax
	movl	%eax, (%esp)
	call	_ZN1A3fooEv
	movl	%eax, 28(%esp)
	movl	$125, 20(%esp)
	movl	$976, 24(%esp)
	leal	20(%esp), %eax
	movl	%eax, (%esp)
	call	_ZN1A3fooEv
	movl	%eax, 28(%esp)
	leal	24(%esp), %eax
	movl	%eax, (%esp)
	call	_ZN1A3fooEv
	movl	%eax, 28(%esp)
	movl	$5, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.7.2-2ubuntu1) 4.7.2"
	.section	.note.GNU-stack,"",@progbits

	.text

	.global putsprint
	.type putsprint, %function

	.extern  puts

putsprint:
	stmdb 		sp!, {lr}
	bl 			puts
	ldmia 		sp!, {pc}
	
	.size	putsprint, .-putsprint
	
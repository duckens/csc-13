	.text
	.global sysprint
	.type sysprint, %function
	
sysprint:
	stmdb 		sp!, {lr}
	mov		r2, r1 					/* size passed in r1 */
	mov		r1, r0 					/* ptr passed in r0 */
	mov		r0, #1					/* stdout == 1 */
	mov		r7, #4					/* write syscall number = 4 */
	swi 	#0						/* int 0x80 */
	ldmia 		sp!, {pc}
	.size	sysprint, .-sysprint
	
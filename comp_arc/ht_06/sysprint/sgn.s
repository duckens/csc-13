	.text
	.global sgn
	.type sgn, %function

sgn:
	cmp		r0, #0
	movlt	r0, #-1
	movgt	r0, #1
	bx		lr
	.size	sgn, . - sgn
	
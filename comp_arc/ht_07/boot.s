	.code16
	.global main
	.text

main:
	movb $'X' , %al       #character to print
	movb $0x0e, %ah       #bios service code to print
	int  $0x10            #interrupt the cpu now

hang:
	jmp hang



	.data		#append boot signature
sign:
	.byte 0x00
	.byte 0x00
	.byte 0x55
	.byte 0xaa

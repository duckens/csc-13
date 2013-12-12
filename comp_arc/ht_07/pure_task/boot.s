	.code16
	.global main
	.text

_start:
	movb $'H' , %al       #character to print
	movb $0x0e, %ah       #bios service code to print one char
	int  $0x10            #interrupt

	movb $'e' , %al       #character to print
	movb $0x0e, %ah       #bios service code to print one char
	int  $0x10            #interrupt

	movb $'l' , %al       #character to print
	movb $0x0e, %ah       #bios service code to print one char
	int  $0x10            #interrupt

	movb $'l' , %al       #character to print
	movb $0x0e, %ah       #bios service code to print one char
	int  $0x10            #interrupt

	movb $'o' , %al       #character to print
	movb $0x0e, %ah       #bios service code to print one char
	int  $0x10            #interrupt

	movb $',' , %al       #character to print
	movb $0x0e, %ah       #bios service code to print one char
	int  $0x10            #interrupt

	movb $' ' , %al       #character to print
	movb $0x0e, %ah       #bios service code to print one char
	int  $0x10            #interrupt

	movb $'W' , %al       #character to print
	movb $0x0e, %ah       #bios service code to print one char
	int  $0x10            #interrupt

	movb $'o' , %al       #character to print
	movb $0x0e, %ah       #bios service code to print one char
	int  $0x10            #interrupt

	movb $'r' , %al       #character to print
	movb $0x0e, %ah       #bios service code to print one char
	int  $0x10            #interrupt

	movb $'l' , %al       #character to print
	movb $0x0e, %ah       #bios service code to print one char
	int  $0x10            #interrupt

	movb $'d' , %al       #character to print
	movb $0x0e, %ah       #bios service code to print one char
	int  $0x10            #interrupt

	movb $'\n' , %al      #character to print
	movb $0x0e, %ah       #bios service code to print one char
	int  $0x10            #interrupt

	movb $0x00, %ah       #wait for key
	int  $0x16

	int  $0x19            #reboot?



	. = _start + 510      #mov to 510th byte from 0 pos
	.byte 0x55            #append boot signature
	.byte 0xaa            #append boot signature 

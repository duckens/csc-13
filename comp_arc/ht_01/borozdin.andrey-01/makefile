all: make_directories hello_c hello_syscall hello_puts_gcc hello_puts_as_ld_compile hello_puts_as_ld_link_good hello_puts_as_ld_link_bad


hello_c:

	gcc -S hello_c.c -o hello_c.s
	gcc -c hello_c.s -o obj/hello_c.o
	gcc obj/hello_c.o -o bin/hello_c -Wl,-Map,bin/hello_c.map
	objdump -S bin/hello_c > disassemble/hello_c.disassembled.s

hello_syscall:

	as hello_syscall.s -o obj/hello_syscall.o
	ld obj/hello_syscall.o -o bin/hello_syscall -Map bin/hello_syscall.map
	objdump -S bin/hello_syscall > disassemble/hello_syscall.disassembled.s

hello_puts_as_ld_compile:
	as hello_puts_as_ld.s -o obj/hello_puts_as_ld.o

hello_puts_as_ld_link_good:
	ld obj/hello_puts_as_ld.o -o bin/hello_puts_as_ld_link_good -lc -dynamic-linker /lib/ld-linux.so.2 -Map bin/hello_puts_as_ld_link_good.map
	# nothing works without "-dynamic-linker /lib/ld-linux.so.2" parameter
	# maybe you'll need different file
	# I have linux Mint 14, 32-bit, latest gcc
	objdump -S bin/hello_puts_as_ld_link_good > disassemble/hello_puts_as_ld_link_good.disassembled.s

hello_puts_as_ld_link_bad:
	ld obj/hello_puts_as_ld.o -o bin/hello_puts_as_ld_link_bad -lc -Map bin/hello_puts_as_ld_link_bad.map
	objdump -S bin/hello_puts_as_ld_link_bad > disassemble/hello_puts_as_ld_link_bad.disassembled.s

hello_puts_gcc:

	gcc hello_puts_gcc.s -o bin/hello_puts_gcc -Wl,-Map,bin/hello_puts_gcc.map
	objdump -S bin/hello_puts_gcc > disassemble/hello_puts_gcc.disassembled.s

make_directories:
	mkdir -p ./bin
	mkdir -p ./obj
	mkdir -p ./disassemble

clean:
	rm -fv ./bin/*
	rm -fv ./obj/*
	rm -fv ./disassemble/*
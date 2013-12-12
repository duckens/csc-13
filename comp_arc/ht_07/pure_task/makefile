all:
	as boot.s -c -o boot.o
	ld boot.o -o boot.bin -T boot.ld
	objcopy --output-target binary boot.bin

clean:
	rm -rfv boot.o

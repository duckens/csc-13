BID_DIR := ./bin
OBJ_DIR := ./obj

all:
	mkdir -p $(BID_DIR)
	mkdir -p $(OBJ_DIR)
	as boot.s -c -o $(OBJ_DIR)/boot.o
	ld $(OBJ_DIR)/boot.o -o $(BID_DIR)/boot.elf -T boot.ld
	cp $(BID_DIR)/boot.elf $(BID_DIR)/boot.bin
	objcopy --output-target binary $(BID_DIR)/boot.bin

	as test2.s -o $(BID_DIR)/test2.elf
	cp $(BID_DIR)/test2.elf $(BID_DIR)/test2.bin
	objcopy --output-target binary $(BID_DIR)/test2.bin

clean:
	rm -rfv $(BID_DIR)
	rm -rfv $(OBJ_DIR)

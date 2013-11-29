all: make_directories library copy_lib executable copy_ex

BIN_DIR := ./bin

make_directories:
	mkdir -p $(BIN_DIR)

library:
	gcc sysprint.s -o $(BIN_DIR)/libtestsysprint.so -shared

copy_lib:
	cp $(BIN_DIR)/libtestsysprint.so /data/botbrew/lib/

executable:
	gcc main.c sgn.s -o $(BIN_DIR)/test_main -ltestsysprint

copy_ex:
	cp $(BIN_DIR)/test_main /data/botbrew/bin/

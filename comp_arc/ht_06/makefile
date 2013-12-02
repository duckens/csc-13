all: simple

BIN_DIR := ./bin
LIB_DIR := ./lib
TARGET_BIN_DIR := /data/botbrew/bin
TARGET_LIB_DIR := /data/botbrew/lib

simple:
	gcc putsprint.s -o libhello.so -shared
	gcc main.c -o test_main -lhello -L.

phone:
	mkdir -p $(BIN_DIR)
	mkdir -p $(LIB_DIR)
	gcc putsprint.s -o $(LIB_DIR)/libhello.so -shared
	cp $(LIB_DIR)/libhello.so $(TARGET_LIB_DIR)
	gcc main.c -o $(BIN_DIR)/test_main -lhello
	cp $(BIN_DIR)/test_main $(TARGET_BIN_DIR)

kill:
	rm $(TARGET_BIN_DIR)/test_main
	rm $(TARGET_LIB_DIR)/libtestsysprint.so
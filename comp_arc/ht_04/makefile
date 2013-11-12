CC				:= g++
DEBUGFLAGS		:= -DDEBUG -DLOCAL
OBJ_DIR			:= ./obj
BIN_DIR			:= ./bin
SRC_DIR			:= ./src

all: make_directories release

debug:
	$(CC) $(DEBUGFLAGS) $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o -c
	$(CC) $(DEBUGFLAGS) $(SRC_DIR)/selectorStructParser.cpp -o $(OBJ_DIR)/selectorStructParser.o -c
	$(CC) $(DEBUGFLAGS) $(SRC_DIR)/gdtRecordParser.cpp -o $(OBJ_DIR)/gdtRecordParser.o -c
	$(CC) $(DEBUGFLAGS) $(SRC_DIR)/pdRecordParser.cpp -o $(OBJ_DIR)/pdRecordParser.o -c
	$(CC) $(DEBUGFLAGS) $(SRC_DIR)/linearAddressParser.cpp -o $(OBJ_DIR)/linearAddressParser.o -c
	$(CC) $(DEBUGFLAGS) $(SRC_DIR)/gdtAddressResolver.cpp -o $(OBJ_DIR)/gdtAddressResolver.o -c
	$(CC) $(DEBUGFLAGS) $(SRC_DIR)/pdAddressResolver.cpp -o $(OBJ_DIR)/pdAddressResolver.o -c
	$(CC) $(DEBUGFLAGS) $(SRC_DIR)/debugFunctions.cpp -o $(OBJ_DIR)/debugFunctions.o -c
	$(CC) $(OBJ_DIR)/pdAddressResolver.o $(OBJ_DIR)/linearAddressParser.o $(OBJ_DIR)/main.o $(OBJ_DIR)/selectorStructParser.o $(OBJ_DIR)/gdtRecordParser.o $(OBJ_DIR)/pdRecordParser.o $(OBJ_DIR)/gdtAddressResolver.o $(OBJ_DIR)/debugFunctions.o -o $(BIN_DIR)/exe

release:
	$(CC) $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o -c
	$(CC) $(SRC_DIR)/selectorStructParser.cpp -o $(OBJ_DIR)/selectorStructParser.o -c
	$(CC) $(SRC_DIR)/gdtRecordParser.cpp -o $(OBJ_DIR)/gdtRecordParser.o -c
	$(CC) $(SRC_DIR)/pdRecordParser.cpp -o $(OBJ_DIR)/pdRecordParser.o -c
	$(CC) $(SRC_DIR)/linearAddressParser.cpp -o $(OBJ_DIR)/linearAddressParser.o -c
	$(CC) $(SRC_DIR)/gdtAddressResolver.cpp -o $(OBJ_DIR)/gdtAddressResolver.o -c
	$(CC) $(SRC_DIR)/pdAddressResolver.cpp -o $(OBJ_DIR)/pdAddressResolver.o -c
	$(CC) $(SRC_DIR)/debugFunctions.cpp -o $(OBJ_DIR)/debugFunctions.o -c
	$(CC) $(OBJ_DIR)/pdAddressResolver.o $(OBJ_DIR)/linearAddressParser.o $(OBJ_DIR)/main.o $(OBJ_DIR)/selectorStructParser.o $(OBJ_DIR)/gdtRecordParser.o $(OBJ_DIR)/pdRecordParser.o $(OBJ_DIR)/gdtAddressResolver.o $(OBJ_DIR)/debugFunctions.o -o $(BIN_DIR)/exe	

make_directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

clean:
	rm -rfv $(BIN_DIR)/exe
	rm -rfv $(OBJ_DIR)/*.o


CC				:= g++
CFLAGS			:= -I/usr/local/include/opencv -L/usr/local/lib
OBJECTS			:= 
LIBRARIES		:= -lopencv_core -lopencv_imgproc -lopencv_highgui

#.PHONY: all clean

all: clean make_directories task_01 task_02 test

task_01:
	$(CC) $(CFLAGS) -o ./bin/task_01 task_01.cpp $(LIBRARIES)

task_02:
	$(CC) $(CFLAGS) -o ./bin/task_02 task_02.cpp $(LIBRARIES)

make_directories:
	mkdir -p ./bin

clean:
	rm -fv *.o
	rm -fv ./bin/*
	rm -fv ./Lena_*.jpg

test:
	./bin/task_02 Lena.jpg

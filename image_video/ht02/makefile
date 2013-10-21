CC				:= g++
CFLAGS			:= -I/usr/local/include/opencv -L/usr/local/lib
OBJECTS			:= 
LIBRARIES		:= -lopencv_core -lopencv_imgproc -lopencv_highgui

#.PHONY: all clean

all: clean make_directories andrew.borozdin.task-02

andrew.borozdin.task-02:
	$(CC) $(CFLAGS) -o ./bin/andrew.borozdin.task-02 andrew.borozdin.task-02.cpp $(LIBRARIES)


make_directories:
	mkdir -p ./bin

clean:
	rm -fv *.o
	rm -fv ./bin/*

test:
	./bin/andrew.borozdin.task-02

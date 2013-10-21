CC				:= g++
CFLAGS			:= -I/usr/local/include/opencv -L/usr/local/lib
OBJECTS			:= 
LIBRARIES		:= -lopencv_core -lopencv_imgproc -lopencv_highgui

.PHONY: all clean

all: test task_01

test: 
	$(CC) $(CFLAGS) -o test test.cpp $(LIBRARIES)

task_01: 
	$(CC) $(CFLAGS) -o task_01 task_01.cpp $(LIBRARIES)

clean:
	rm -f *.o
	rm task_01
	rm test

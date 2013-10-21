CC				:= g++
CFLAGS			:= -I/usr/local/include/opencv -L/usr/local/lib
OBJECTS			:= 
LIBRARIES		:= -lopencv_core -lopencv_imgproc -lopencv_highgui

.PHONY: all clean

all: first seminar_2_linear_point_op

seminar_2_linear_point_op: 
	$(CC) $(CFLAGS) -o seminar_2_linear_point_op seminar_2_linear_point_op.cpp $(LIBRARIES)

first: 
	$(CC) $(CFLAGS) -o first first.cpp $(LIBRARIES)


clean:
	rm -f *.o
	rm first
	rm seminar_2_linear_point_op

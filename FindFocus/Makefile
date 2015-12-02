CC = g++

SRCS = main.cpp ShapeFromFocus.cpp GrayLevelCooccurrenceMatrix.cpp FindFocus.cpp
OBJS = $(SRCS:%.cpp=%.o)

INC_PATH_OPENCV = /home/keti/Downloads/opencv-2.4.11/include
LIB_PATH_OPENCV = /usr/local/lib
LIB_OPTION = -lopencv_core -lopencv_highgui -lopencv_imgproc

TARGET = FindFocus

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) -L$(LIB_PATH_OPENCV) -I$(INC_PATH_OPENCV) $(LIB_OPTION)

depend:
	gccmakedep $(SRCS)

clean:
	@rm -rf *.o main

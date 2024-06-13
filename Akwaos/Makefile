CC := g++

CFLAGS := -std=c++11 -Wall

LIBS := -L./vclib/lib/x64 -lSDL2main -lSDL2

INCLUDES := -I./vclib/include

SRCS := main.cpp

OBJS := $(SRCS:.cpp=.o)

TARGET := AKWAOS

TEXTURES_DIR := textures

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

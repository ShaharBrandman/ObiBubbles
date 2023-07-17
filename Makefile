CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lGL -lglfw -lm

TARGET = bubbles.o

all: $(TARGET)

$(TARGET): main.c glad.c shaders.c resize_window.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c glad.c shaders.c resize_window.c $(LIBS)

clean:
	rm -f $(TARGET)

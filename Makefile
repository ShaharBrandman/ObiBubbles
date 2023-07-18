CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lGL -lglfw -lm

TARGET = bubbles.o

all: $(TARGET)

$(TARGET): main.c glad.c shaders.c resize_window.c projectile.c cursor.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c glad.c shaders.c resize_window.c projectile.c cursor.c $(LIBS)

clean:
	rm -f $(TARGET)

CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lGL -lglfw -lm

TARGET = bubbles.o
TARGET_DEV = dev.o

all: $(TARGET)

$(TARGET): main.c 
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(LIBS)

all: $(TARGET_DEV)
$(TARGET_DEV): main.c 
	$(CC) $(CFLAGS) -o $(TARGET_DEV) main.c $(LIBS)

clean:
	rm -f $(TARGET)

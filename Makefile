CC=gcc
CFLAGS=-I.
LIBS=-ljansson
SOURCE=$(wildcard *.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
TARGET=test

.PHONY: all

$(TARGET): $(OBJECTS) $(LIBS)
	$(CC) -o $@ $(OBJECTS) $(SOURCE) $(LIBS)

clean:
	rm -f *.o *~ core $(TARGET)
TARGET = plane
LIBS = -lm
CC = gcc
CFLAGS = -g -pedantic -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

# remove the object files
clean:
	-rm -f *.o

# remove the object files and the binary executable
clean-all:
	-rm -f *.o
	-rm -f $(TARGET)

# remove the data output by the program
clean-data:
	-rm -f *.pdf
	-rm -f *.dat

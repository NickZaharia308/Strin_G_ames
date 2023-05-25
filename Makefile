CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# List of source files
SOURCES = wordle.c hangman.c scrabble.c main.c

# List of object files
OBJECTS = $(SOURCES:.c=.o)

# Name of the executable
EXECUTABLE = game

# Default target
all: $(EXECUTABLE)

# Compile each source file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link the object files into the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean the compiled files
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

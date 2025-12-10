# Compiler (supports optional cross-compilation)
CC := $(CROSS_COMPILE)gcc

# Compiler flags
CFLAGS := -Wall -Werror

# Target executable
TARGET := writer

# Default target: build the writer application
all: $(TARGET)

# Build rule
$(TARGET): writer.o
	$(CC) $(CFLAGS) -o $(TARGET) writer.o

# Compile object file from source
writer.o: writer.c
	$(CC) $(CFLAGS) -c writer.c

# Clean rule
clean:
	rm -f $(TARGET) *.o


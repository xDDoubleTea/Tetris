# Compiler
CC = gcc

# Target executable name
TARGET = hello

# Source files
SRCS = hello.c

# Object files (derived from source files)
OBJS = $(SRCS:.c=.o)

# Allegro 5 libraries to link against
# You're using the core, font, and ttf addons.
ALLEGRO_PKGS = allegro-5 allegro_font-5 allegro_ttf-5

# Compiler flags
# -Wall: Enable all common warnings
# `pkg-config --cflags ...`: Gets include paths for Allegro
CFLAGS = -Wall $(shell pkg-config --cflags $(ALLEGRO_PKGS)) -std=c99 -g

# Linker flags
# `pkg-config --libs ...`: Gets library paths and names for Allegro
LDFLAGS = $(shell pkg-config --libs $(ALLEGRO_PKGS))

# Default target: build the executable
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files into object files
# $<: the first prerequisite (the .c file)
# $@: the target (the .o file)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Target to clean up build files
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets are not actual files
.PHONY: all clean

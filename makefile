NAME = sdl_template

# compiler settings
CC := clang
CFLAGS := -Wall -g
LDFLAGS = $(shell sdl2-config --cflags --libs) -lm

# file locations
DIR_BIN := bin
DIR_OBJ := obj
SRC = $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c) $(wildcard src/**/**/**/**/*.c)
OBJ = $(patsubst src/%,$(DIR_OBJ)/$(ARCH)/%,$(SRC:.c=.o))
TARGET = $(DIR_BIN)/$(ARCH)/$(NAME)$(EXT)

clean:
	rm -rf $(DIR_BIN) $(DIR_OBJ)

# sets the variables for the different targets
linux-x86_64:
	$(MAKE) build ARCH=linux-x86_64 CFLAGS="$(CFLAGS) -target x86_64-pc-linux-gnu"
win-x86_64:
	$(MAKE) build ARCH=win-x86-64 CFLAGS="$(CFLAGS) -target x86_64-pc-windows-gnu" EXT=".exe"
web:
	$(MAKE) build ARCH=web CC=emcc EXT=".html"

build: dirs binary

# creates the binary
binary: $(OBJ)
	$(CC) -o $(TARGET) $^ $(CFLAGS) $(LDFLAGS)

# creates the object files, include a flag for no unused command line arguments, because in this context it's unneeded
$(DIR_OBJ)/$(ARCH)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS) -Wno-unused-command-line-argument

dirs:
	mkdir -p $(DIR_BIN)/$(ARCH)
	mkdir -p $(DIR_OBJ)/$(ARCH)

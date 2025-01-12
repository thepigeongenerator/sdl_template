NAME = sdl_template
CFLAGS = -Wall -g -march=x86-64 -lm

DIR_BIN = bin
DIR_OBJ = obj

SRC = $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c) $(wildcard src/**/**/**/**/*.c)
OBJ = $(patsubst src/%,$(DIR_OBJ)/$(ARCH)/%,$(SRC:.c=.o))

TARGET = $(DIR_BIN)/$(ARCH)/$(NAME)$(EXT)

clean:
	rm -rf $(DIR_BIN) $(DIR_OBJ)

# sets the variables for the different targets
linux-x86_64:
	$(MAKE) build ARCH=linux-x86_64 CC=clang PCFLAGS="-target x86_64-pc-linux-gnu -lSDL2"
win-x86_64:
	$(MAKE) build ARCH=win-x86-64 CC=clang PCFLAGS="-target x86_64-pc-windows-gnu -lmingw32 -lSDL2main -lSDL2 -mwindows" EXT=".exe"
web:
	$(MAKE) build ARCH=web CC=emcc PCFLAGS="-s USE_SDL=2" EXT=".html"

build: dirs binary

# creates the binary
binary: $(OBJ)
	$(CC) -o $(TARGET) $^ $(CFLAGS) $(PCFLAGS)

# creates the object files, include a flag for no unused command line arguments, because in this context it's unneeded
$(DIR_OBJ)/$(ARCH)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS) $(PCFLAGS) -Wno-unused-command-line-argument

dirs:
	mkdir -p $(DIR_BIN)/$(ARCH)
	mkdir -p $(DIR_OBJ)/$(ARCH)

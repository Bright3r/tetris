CFLAGS = -Wall -Wextra -pedantic -g -I./src
LDFLAGS = -L./src
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_mixer -lm -pthread
SRC = ./src/main.c ./src/tetromino.c ./src/tiles.c
DEST = main

all:
	gcc $(CFLAGS) $(SRC) $(LDFLAGS) $(LIBS) -o $(DEST)

clean:
	rm -f $(DEST)

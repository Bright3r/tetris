#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "types.h"
#include "tetromino.c"

#define TITLE "Title"
#define FRAME_INTERVAL 16.667f
#define TILE_SIZE 25

typedef struct tile {
  SDL_Rect *rect; 
  SDL_Color *color;
} tile;

void init();
void cleanup_SDL();
void refreshScreen();
void setRenderColor(SDL_Color *color);
tile *createTile(int row, int col, SDL_Color *color);
void destroyTile(tile *tile);
void drawTile(tile *tile);
void drawAnonymousTile(int row, int col, SDL_Color *color);
void drawTetromino(tetromino *piece);

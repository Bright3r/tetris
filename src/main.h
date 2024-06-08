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

typedef struct tile_t {
  SDL_Rect *rect; 
  SDL_Color *color;
} tile_t;

typedef struct tilemap_t {
  tile_t **map;
  int num_tiles;
} tilemap_t;

void init();
void cleanup_SDL();
void refreshScreen();
void setRenderColor(SDL_Color *color);
tile_t *createTile(int col, int row, SDL_Color *color);
void destroyTile(tile_t *tile);
void drawTile(tile_t *tile);
void drawAnonymousTile(int col, int row, SDL_Color *color);
tilemap_t *createTileMap();
void destroyTileMap(tilemap_t *tilemap);
void addTile(tilemap_t *tilemap, tile_t *tile);
void drawTetromino(tetromino *piece);
void drawTileMap(tilemap_t *tilemap);
void tileify(tilemap_t *tilemap, tetromino *piece);




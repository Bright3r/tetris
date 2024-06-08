#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "types.h"
#include "tiles.c"
#include "tetromino.c"

#define TITLE "Title"
#define FRAME_INTERVAL 16.667f
#define TILE_SIZE 25

void init();
void cleanup_SDL();
void refreshScreen();
void setRenderColor(SDL_Color *color);
void drawTile(tile_t *tile);
void drawAnonymousTile(int col, int row, SDL_Color *color);
tilemap_t *createTileMap();
void drawTetromino(tetromino *piece);
void drawTileMap(tilemap_t *tilemap);
void tileify(tilemap_t *tilemap, tetromino *piece);
void updatePiece(tilemap_t *tilemap, tetromino **piece, uint32_t *last_update_time, float game_speed);




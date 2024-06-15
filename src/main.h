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
#define TILE_SIZE 25
#define FRAME_INTERVAL 16.667f
#define TICK_RATE 200.0f
#define INPUT_REFRESH_RATE 50.0f


void init();
void cleanup_SDL();
void refreshScreen();
void setRenderColor(SDL_Color *color);
float getElapsedTime(uint32_t last_time);
void drawTile(tile_t *tile);
void drawAnonymousTile(int col, int row, SDL_Color *color);
tilemap_t *createTileMap();
void drawTetromino(tetromino *piece);
void drawTileMap(tilemap_t *tilemap);
void updatePiece(tilemap_t *tilemap, tetromino **piece, uint32_t *last_update_time, float game_speed);
void movePieceRight(tetromino *piece, tilemap_t *tilemap, uint32_t *last_input_time);
void movePieceLeft(tetromino *piece, tilemap_t *tilemap, uint32_t *last_input_time);
void movePieceDown(tetromino **piece_ptr, tilemap_t *tilemap);
void dropPiece(tetromino **piece_ptr, tilemap_t *tilemap);
void tileify(tilemap_t *tilemap, tetromino *piece);
void handleFilledRows(tilemap_t *tilemap);
void shiftRowsDown(tilemap_t *tilemap, int starting_row);



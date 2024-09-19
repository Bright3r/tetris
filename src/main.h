#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "types.h"
#include "tiles.h"
#include "tetromino.h"

#define TITLE "TETRIS"
#define FRAME_INTERVAL 16.667f
#define TICK_RATE 300.0f
#define INPUT_REFRESH_RATE 70.0f


void init();
void cleanup_SDL();
void refreshScreen();
void setRenderColor(SDL_Color *color);
float getElapsedTime(uint32_t last_time);
void drawGridLines();
void drawText(SDL_Color *color, int x, int y, char *msg);
void drawScore(int score);
void gameloop();
void startMenu();
void drawTile(tile_t *tile);
void drawAnonymousTile(int row, int col, SDL_Color *color);
void drawTetromino(tetromino *piece, SDL_Color *color);
void drawGhostPiece(tilemap_t *tilemap, tetromino *piece);
void drawNextTetromino(tetromino *piece);
void drawHeldTetromino(tetromino *piece);
void drawTileMap(tilemap_t *tilemap);
bool updatePiece(tilemap_t *tilemap, tetromino **piece_ptr, tetromino **next_piece_ptr, bool *is_holding_piece, uint32_t *last_update_time, float game_speed);
void movePieceRight(tetromino *piece, tilemap_t *tilemap, uint32_t *last_input_time);
void movePieceLeft(tetromino *piece, tilemap_t *tilemap, uint32_t *last_input_time);
bool movePieceDown(tetromino **piece_ptr, tetromino **next_piece_ptr, tilemap_t *tilemap, bool *is_holding_piece);
bool dropPiece(tetromino **piece_ptr, tetromino **next_piece_ptr, tilemap_t *tilemap, bool *is_holding_piece);
bool placePiece(tetromino **piece_ptr, tetromino **next_piece_ptr, tilemap_t *tilemap, bool *is_holding_piece);
void swapPieces(tetromino **curr_piece, tetromino **hold_piece);
bool tileify(tilemap_t *tilemap, tetromino *piece);
void handleFilledRows(tilemap_t *tilemap);
void shiftRowsDown(tilemap_t *tilemap, int starting_row);
bool gameover();
void handleWindowResize(tilemap_t *tilemap);





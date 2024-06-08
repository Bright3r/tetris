#pragma once

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.h"

#define NUM_TETROMINO_TYPES 7
#define TETROMINO_WIDTH 4
#define MAX_ROWS 20
#define MAX_COLUMNS 10

typedef bool tetromino_state[TETROMINO_WIDTH][TETROMINO_WIDTH];

typedef enum tetromino_type {
  i_piece, j_piece, l_piece, o_piece, s_piece, t_piece, z_piece
} tetromino_type;

typedef struct tetromino {
  tetromino_type type;
  tetromino_state state;
  int row, col;
} tetromino;

static tetromino_state I_PIECE_INITIAL_STATE = {
  { 0, 0, 0, 1 },
  { 0, 0, 0, 1 },
  { 0, 0, 0, 1 },
  { 0, 0, 0, 1 }
};

static tetromino_state J_PIECE_INITIAL_STATE = {
  { 0, 0, 0, 0 },
  { 0, 0, 0, 0 },
  { 1, 0, 0, 0 },
  { 1, 1, 1, 0 }
};

static tetromino_state L_PIECE_INITIAL_STATE = {
  { 0, 0, 0, 0 },
  { 0, 0, 0, 1 },
  { 0, 0, 0, 1 },
  { 0, 1, 1, 1 }
};

static tetromino_state O_PIECE_INITIAL_STATE = {
  { 0, 0, 0, 0 },
  { 0, 0, 0, 0 },
  { 0, 1, 1, 0 },
  { 0, 1, 1, 0 }
};

static tetromino_state S_PIECE_INITIAL_STATE = {
  { 0, 0, 0, 0 },
  { 0, 0, 0, 0 },
  { 0, 1, 1, 0 },
  { 1, 1, 0, 0 }
};

static tetromino_state T_PIECE_INITIAL_STATE = {
  { 0, 0, 0, 0 },
  { 0, 0, 0, 0 },
  { 0, 0, 1, 0 },
  { 0, 1, 1, 1 }
};

static tetromino_state Z_PIECE_INITIAL_STATE = {
  { 0, 0, 0, 0 },
  { 0, 0, 1, 0 },
  { 0, 1, 1, 0 },
  { 0, 1, 0, 0 }
};

tetromino *createTetromino(tetromino_type piece_type, int col, int row);
void destroyTetromino(tetromino *piece);
tetromino_type getTetrominoType(int idx);
tetromino *createRandomTetromino();
SDL_Color *getTetrominoColor(tetromino_type piece_type);
int getBottomRow(tetromino *piece);
int getRightmostCol(tetromino *piece);
int getLeftmostCol(tetromino *piece);
bool checkBorderCollisions(tetromino *piece);

bool checkTileCollisions(tetromino *piece, tilemap_t *tilemap);
bool checkCollisionWithTile(tetromino *piece, tile_t *tile);
bool isOnFloor(tetromino *piece);
void movePieceRight(tetromino *piece);
void movePieceLeft(tetromino *piece);
void rotateTetrominoRight(tetromino *piece);
void rotateTetrominoLeft(tetromino *piece);
void tryToChangeState(tetromino *piece, tetromino_state *new_state);




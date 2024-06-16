#pragma once

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.h"

#define NUM_TETROMINO_TYPES 7
#define TETROMINO_WIDTH 4
#define NUM_TETROMINO_PERMUTATIONS 4
#define MAX_ROWS 20
#define MAX_COLUMNS 10

typedef const bool tetromino_state[TETROMINO_WIDTH][TETROMINO_WIDTH];

typedef enum tetromino_type {
  i_piece, j_piece, l_piece, o_piece, s_piece, t_piece, z_piece
} tetromino_type;

typedef struct tetromino {
  tetromino_type type;
  int state_idx;
  int row, col;
} tetromino;

static const tetromino_state I_PIECE_STATES[NUM_TETROMINO_PERMUTATIONS] = {
  {
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 1, 1, 1, 1 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 1, 1, 1, 1 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
  },
};

static const tetromino_state J_PIECE_STATES[NUM_TETROMINO_PERMUTATIONS] = {
  {
    { 0, 0, 0, 0 },
    { 0, 0, 1, 1 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 1, 1, 1 },
    { 0, 0, 0, 1 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 1 },
    { 0, 0, 0, 1 },
    { 0, 0, 1, 1 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 1, 1 },
    { 0, 0, 0, 0 }
  },
};

static const tetromino_state L_PIECE_STATES[NUM_TETROMINO_PERMUTATIONS] = {
  {
    { 0, 0, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 1 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 1, 1, 1 },
    { 0, 1, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 1 },
    { 0, 1, 1, 1 },
    { 0, 0, 0, 0 }
  },
};

static const tetromino_state O_PIECE_STATES[NUM_TETROMINO_PERMUTATIONS] = {
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 1, 1 },
    { 0, 0, 1, 1 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 1, 1 },
    { 0, 0, 1, 1 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 1, 1 },
    { 0, 0, 1, 1 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 1, 1 },
    { 0, 0, 1, 1 }
  },
};

static const tetromino_state S_PIECE_STATES[NUM_TETROMINO_PERMUTATIONS] = {
  {
    { 0, 0, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 1 },
    { 0, 0, 0, 1 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 1, 1 },
    { 0, 1, 1, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 1, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 1, 1 },
    { 0, 1, 1, 0 },
    { 0, 0, 0, 0 }
  },
};

static const tetromino_state T_PIECE_STATES[NUM_TETROMINO_PERMUTATIONS] = {
  {
    { 0, 0, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 1, 1, 1 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 1 },
    { 0, 0, 1, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 1, 1, 1 },
    { 0, 0, 1, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 1, 0 }
  },
};

static const tetromino_state Z_PIECE_STATES[NUM_TETROMINO_PERMUTATIONS] = {
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 1 },
    { 0, 0, 1, 1 },
    { 0, 0, 1, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 1, 1 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 1 },
    { 0, 0, 1, 1 },
    { 0, 0, 1, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 1, 1 },
    { 0, 0, 0, 0 }
  },
};



tetromino *createTetromino(tetromino_type piece_type, int col, int row);
void destroyTetromino(tetromino *piece);
tetromino_type getTetrominoType(int idx);
tetromino_state *getTetrominoState(tetromino *piece);
tetromino *createRandomTetromino();
SDL_Color *getTetrominoColor(tetromino_type piece_type);
int getBottomRow(tetromino *piece);
int getRightmostCol(tetromino *piece);
int getLeftmostCol(tetromino *piece);
bool isOnFloor(tetromino *piece);
bool checkBorderCollisions(tetromino *piece);
bool checkTileCollisions(tilemap_t *tilemap, tetromino *piece);
bool checkCollisionWithTile(tetromino *piece, tile_t *tile);
void rotateTetrominoRight(tilemap_t *tilemap, tetromino *piece);
void rotateTetrominoLeft(tilemap_t *tilemap, tetromino *piece);




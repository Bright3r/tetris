
#include "tetromino.h"

tetromino *createTetromino(tetromino_type piece_type, int col, int row) {
  tetromino *piece = (tetromino *) malloc(sizeof(tetromino));
  piece->type = piece_type;
  piece->row = row;
  piece->col = col;

  switch (piece_type) {
    case i_piece:
      memcpy(piece->state, I_PIECE_INITIAL_STATE, sizeof(tetromino_state));
      break;
    case j_piece:
      memcpy(piece->state, J_PIECE_INITIAL_STATE, sizeof(tetromino_state));
      break;
    case l_piece:
      memcpy(piece->state, L_PIECE_INITIAL_STATE, sizeof(tetromino_state));
      break;
    case o_piece:
      memcpy(piece->state, O_PIECE_INITIAL_STATE, sizeof(tetromino_state));
      break;
    case s_piece:
      memcpy(piece->state, S_PIECE_INITIAL_STATE, sizeof(tetromino_state));
      break;
    case t_piece: 
      memcpy(piece->state, T_PIECE_INITIAL_STATE, sizeof(tetromino_state));
      break;
    case z_piece:
      memcpy(piece->state, Z_PIECE_INITIAL_STATE, sizeof(tetromino_state));
      break;
  }

  return piece;
}

void destroyTetromino(tetromino *piece) {
  free(piece);
}

tetromino_type getTetrominoType(int idx) {
  static const tetromino_type tetromino_types[NUM_TETROMINO_TYPES] = {
    i_piece, j_piece, l_piece, o_piece, s_piece, t_piece, z_piece
  };

  return tetromino_types[idx];
}

tetromino *createRandomTetromino() {
  static const int MIDDLE_COL = (MAX_COLUMNS - TETROMINO_WIDTH) / 2;

  tetromino_type random_piece_type = getTetrominoType(rand() % NUM_TETROMINO_TYPES);
  tetromino *piece = createTetromino(random_piece_type, MIDDLE_COL, -TETROMINO_WIDTH);
  return piece;
}

SDL_Color *getTetrominoColor(tetromino_type piece_type) {
  switch (piece_type) {
    case i_piece:
      return &COLOR_LIGHT_BLUE;
    case j_piece:
      return &COLOR_DARK_BLUE;
    case l_piece:
      return &COLOR_ORANGE;
    case o_piece:
      return &COLOR_YELLOW;
    case s_piece:
      return &COLOR_GREEN;
    case t_piece:
      return &COLOR_PURPLE;
    case z_piece:
      return &COLOR_RED;
  }

  return NULL;
}

int getBottomRow(tetromino *piece) {
  for (int row = TETROMINO_WIDTH - 1; row >= 0; row--) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      if (piece->state[row][col]) {
        return piece->row + row;
      }
    }
  }

  return -1;
}

int getRightmostCol(tetromino *piece) {
  for (int col = TETROMINO_WIDTH - 1; col >= 0; col--) {
    for (int row = 0; row < TETROMINO_WIDTH; row++) {
      if (piece->state[row][col]) {
        return piece->col + col;
      }
    }
  }

  return -1;
}

int getLeftmostCol(tetromino *piece) {
  for (int col = 0; col < TETROMINO_WIDTH; col++) {
    for (int row = 0; row < TETROMINO_WIDTH; row++) {
      if (piece->state[row][col]) {
        return piece->col + col;
      }
    }
  }

  return -1;
}

bool checkBorderCollisions(tetromino *piece) {
  int bottom_row = getBottomRow(piece);
  int leftmost_col = getLeftmostCol(piece);
  int rightmost_col = getRightmostCol(piece);

  printf("BOTTOM ROW: %d, LEFT COL: %d, RIGHT COL: %d\n", bottom_row, leftmost_col, rightmost_col);

  if (bottom_row >= MAX_ROWS || leftmost_col < 0 || rightmost_col >= MAX_COLUMNS) {
    return true;
  }
  return false;
}

bool checkTileCollisions(tetromino *piece, tilemap_t *tilemap) {
  // check if piece collides with any tiles
  for (int i = 0; i < tilemap->num_tiles; i++) {
    tile_t *curr_tile = tilemap->map[i];
    if (checkCollisionWithTile(piece, curr_tile)) {
      return true;
    }
  }

  return false;
}

bool checkCollisionWithTile(tetromino *piece, tile_t *tile) {
  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      if (piece->state[row][col]) {
        // check if piece segment and tile are on same (col, row)
        int board_row = piece->row + row;
        int board_col = piece->col + col;
        if ((board_row == tile->row) && (board_col == tile->col)) {
          return true;
        }
      }
    }
  }

  return false;
}

bool isOnFloor(tetromino *piece) {
  int bottom_row = getBottomRow(piece);
  return bottom_row >= MAX_ROWS;
}



void tryToChangeState(tetromino *piece, tilemap_t *tilemap, tetromino_state *new_state) {
  // save current state of piece
  tetromino_state temp_state;
  memcpy(temp_state, piece->state, sizeof(tetromino_state));

  // apply move to piece
  memcpy(piece->state, *new_state, sizeof(tetromino_state));

  // check if move results in a collision
  if (checkBorderCollisions(piece) || checkTileCollisions(piece, tilemap)) {
    // undo rotation
    memcpy(piece->state, temp_state, sizeof(tetromino_state));
  }
}

void rotateTetrominoRight(tetromino *piece, tilemap_t *tilemap) {
  // get state after rotating current state right
  tetromino_state new_state;
  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      new_state[TETROMINO_WIDTH - 1 - col][row] = piece->state[row][col];
    }
  } 

  tryToChangeState(piece, tilemap, &new_state);
}

void rotateTetrominoLeft(tetromino *piece, tilemap_t *tilemap) {
  // get state after rotating current state left
  tetromino_state new_state;
  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      new_state[col][TETROMINO_WIDTH - 1 - row] = piece->state[row][col];
    }
  } 

  tryToChangeState(piece, tilemap, &new_state);
}


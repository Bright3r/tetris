
#include "tetromino.h"

tetromino *createTetromino(tetromino_type piece_type, int col, int row) {
  tetromino *piece = (tetromino *) malloc(sizeof(tetromino));
  piece->type = piece_type;
  piece->state_idx = 0;
  piece->row = row;
  piece->col = col;

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

tetromino_state *getTetrominoState(tetromino *piece) {
  switch (piece->type) {
    case i_piece:
      return &I_PIECE_STATES[piece->state_idx];
    case j_piece:
      return &J_PIECE_STATES[piece->state_idx];
    case l_piece:
      return &L_PIECE_STATES[piece->state_idx];
    case o_piece:
      return &O_PIECE_STATES[piece->state_idx];
    case s_piece:
      return &S_PIECE_STATES[piece->state_idx];
    case t_piece: 
      return &T_PIECE_STATES[piece->state_idx];
    case z_piece:
      return &Z_PIECE_STATES[piece->state_idx];
  };

  return NULL;
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
      tetromino_state *piece_state = getTetrominoState(piece);
      if ((*piece_state)[row][col]) {
        return piece->row + row;
      }
    }
  }

  return -1;
}

int getRightmostCol(tetromino *piece) {
  for (int col = TETROMINO_WIDTH - 1; col >= 0; col--) {
    for (int row = 0; row < TETROMINO_WIDTH; row++) {
      tetromino_state *piece_state = getTetrominoState(piece);
      if ((*piece_state)[row][col]) {
        return piece->col + col;
      }
    }
  }

  return -1;
}

int getLeftmostCol(tetromino *piece) {
  for (int col = 0; col < TETROMINO_WIDTH; col++) {
    for (int row = 0; row < TETROMINO_WIDTH; row++) {
      tetromino_state *piece_state = getTetrominoState(piece);
      if ((*piece_state)[row][col]) {
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

bool checkTileCollisions(tilemap_t *tilemap, tetromino *piece) {
  // check if piece collides with any tiles
  for (int row = 0; row < tilemap->num_rows; row++) {
    for (int col = 0; col < tilemap->num_cols; col++) {
      tile_t *curr_tile = getTile(tilemap, row, col);
      if (curr_tile != NULL && checkCollisionWithTile(piece, curr_tile)) {
        return true;
      }
    }
  }

  return false;
}

bool checkCollisionWithTile(tetromino *piece, tile_t *tile) {
  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      tetromino_state *piece_state = getTetrominoState(piece);
      if ((*piece_state)[row][col]) {
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

void rotateTetrominoRight(tilemap_t *tilemap, tetromino *piece) {
  int temp_state_idx = piece->state_idx;

  // Change piece to next state
  int new_state_idx = (piece->state_idx + 1) % 4;
  piece->state_idx = new_state_idx;

  // Check if new state results in a collision
  if (checkBorderCollisions(piece) || checkTileCollisions(tilemap, piece)) {
    // Undo state change
    piece->state_idx = temp_state_idx;
  }
}

void rotateTetrominoLeft(tilemap_t *tilemap, tetromino *piece) {
  int temp_state_idx = piece->state_idx;

  // Change piece to next state
  int new_state_idx = (piece->state_idx - 1) % 4;
  piece->state_idx = new_state_idx;

  // Check if new state results in a collision
  if (checkBorderCollisions(piece) || checkTileCollisions(tilemap, piece)) {
    // Undo state change
    piece->state_idx = temp_state_idx;
  }
}


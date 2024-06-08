
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
  int random_piece_number = rand() % NUM_TETROMINO_TYPES;
  tetromino *piece = createTetromino(getTetrominoType(random_piece_number), 0, 0);
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

void try_to_change_state(tetromino *piece, tetromino_state *new_state) {
  // save current state of piece
  tetromino_state temp_state;
  memcpy(temp_state, piece->state, sizeof(tetromino_state));

  // apply move to piece
  memcpy(piece->state, *new_state, sizeof(tetromino_state));

  // check if move results in a collision
  if (checkBorderCollisions(piece)) {
    // undo rotation
    memcpy(piece->state, temp_state, sizeof(tetromino_state));
  }
}

void rotateTetrominoRight(tetromino *piece) {
  // get state after rotating current state right
  tetromino_state new_state;
  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      new_state[TETROMINO_WIDTH - 1 - col][row] = piece->state[row][col];
    }
  } 

  try_to_change_state(piece, &new_state);
}

void rotateTetrominoLeft(tetromino *piece) {
  // get state after rotating current state left
  tetromino_state new_state;
  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      new_state[col][TETROMINO_WIDTH - 1 - row] = piece->state[row][col];
    }
  } 

  try_to_change_state(piece, &new_state);
}

void updatePiece(tetromino *piece, uint32_t *last_update_time, float game_speed) {
  // check whether enough time has passed since last update
  uint32_t curr_time = SDL_GetTicks();
  uint32_t elapsed_time = curr_time - *last_update_time;
  if (elapsed_time < floor(game_speed)) {
    return;
  }

  // move piece and check whether the move results in a collision
  piece->row++;
  if (checkBorderCollisions(piece)) {
    // undo move and exit without updating piece
    piece->row--;
    return;
  }

  *last_update_time = curr_time;
}

void movePieceRight(tetromino *piece) {
  int rightmost_col = getRightmostCol(piece);
  if ((rightmost_col + 1) >= MAX_COLUMNS) {
    return;
  }

  piece->col++;
}

void movePieceLeft(tetromino *piece) {
  int leftmost_col = 0;
  for (int col = TETROMINO_WIDTH - 1; col >= 0; col--) {
    for (int row = 0; row < TETROMINO_WIDTH; row++) {
      if (piece->state[row][col]) {
        leftmost_col = piece->col + col;
        break;
      }
    }
  }

  if ((leftmost_col - 1) < 0) {
    return;
  }

  piece->col--;
}

bool isOnFloor(tetromino *piece) {
  int bottom_row = getBottomRow(piece);
  return bottom_row >= (MAX_ROWS - 1);
}

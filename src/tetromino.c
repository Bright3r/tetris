
#include "tetromino.h"

tetromino *createTetromino(tetromino_type piece_type, int row, int col) {
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

void rotateTetrominoRight(tetromino *piece) {
  tetromino_state new_state;
  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      new_state[TETROMINO_WIDTH - 1 - col][row] = piece->state[row][col];
    }
  } 

  memcpy(piece->state, new_state, sizeof(tetromino_state));
}

void rotateTetrominoLeft(tetromino *piece) {
  tetromino_state new_state;
  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      new_state[col][TETROMINO_WIDTH - 1 - row] = piece->state[row][col];
    }
  } 

  memcpy(piece->state, new_state, sizeof(tetromino_state));
}


void updatePiece(tetromino *piece, uint32_t *last_update_time, float game_speed) {
  // check whether enough time has passed since last update
  uint32_t curr_time = SDL_GetTicks();
  uint32_t elapsed_time = curr_time - *last_update_time;
  if (elapsed_time < floor(game_speed)) {
    return;
  }

  if (piece->row < MAX_ROWS) {
    piece->row++;
    *last_update_time = curr_time;
    printf("Row: %d, Col: %d\n", piece->row, piece->col);
  }
}

void movePieceRight(tetromino *piece) {
  int rightmost_col = 0;
  for (int col = 0; col < TETROMINO_WIDTH; col++) {
    for (int row = 0; row < TETROMINO_WIDTH; row++) {
      if (piece->state[row][col]) {
        rightmost_col = piece->col + col;
        break;
      }
    }
  }

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

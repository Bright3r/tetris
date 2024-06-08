
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

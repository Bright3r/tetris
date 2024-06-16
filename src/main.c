#include "main.h"

static const int window_width = MAX_COLUMNS * TILE_SIZE * 2;
static const int window_height = MAX_ROWS * TILE_SIZE;

static const int gameboard_width = MAX_COLUMNS * TILE_SIZE;
static const int gameboard_height = MAX_ROWS * TILE_SIZE;


static SDL_Window *window;
static SDL_Renderer *renderer;
static TTF_Font *font;
static Mix_Music *music;

static int score = 0;

int main(void) {
  init(); // Initialize SDL2
  
  // Run game as long as player keeps restarting
  bool should_restart = true;
  while (should_restart) {
    printf("Restarting!\n");
    gameloop();

    should_restart = gameover();
  }

  cleanup_SDL();
  printf("Sucess!\n");
  return 0;
}



void init() {
  // Video
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL_Init has failed: %s\n", SDL_GetError());
  }

  // Fonts
  if (TTF_Init() < 0) {
    fprintf(stderr, "TTF_Init has failed: %s\n", TTF_GetError());
  }

  font = TTF_OpenFont("./assets/fonts/PixelifySans-VariableFont_wght.ttf", 24);
  if (font == NULL) {
    fprintf(stderr, "Failed to open font: %s\n", TTF_GetError());
  }

  // Audio
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024) < 0) {
    fprintf(stderr, "SDL_Mixer has failed: %s\n", Mix_GetError());
  }

  // // Play background music
  // music = Mix_LoadMUS("./assets/sounds/music.mp3");
  // if (music == NULL) {
  //   fprintf(stderr, "Failed to load music: %s\n", Mix_GetError());
  // }
  // else if (Mix_PlayMusic(music, -1) < 0) {
  //   fprintf(stderr, "Failed to play music: %s\n", Mix_GetError());
  // }

  // Load Sound Effects
  // chompSound = Mix_LoadWAV("./assets/sounds/chomp.mp3");
  // if (chompSound == NULL) {
  //   fprintf(stderr, "Failed to load sound: %s\n", Mix_GetError());
  // }


  // Create window and renderer
  window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    fprintf(stderr, "Failed to render window: %s\n", SDL_GetError());
  }
}


void cleanup_SDL() {
  Mix_FreeMusic(music);

  TTF_CloseFont(font);
  TTF_Quit();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
}

void refreshScreen() {
  SDL_SetRenderDrawColor(renderer, COLOR_BG.r, COLOR_BG.g, COLOR_BG.b, COLOR_BG.a);
  SDL_RenderClear(renderer);
}

void setRenderColor(SDL_Color *color) {
  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
}

float getElapsedTime(uint32_t last_time) {
  return SDL_GetTicks() - last_time;
}

void drawGridLines() {
  setRenderColor(&COLOR_WHITE);
  for (int row = 0; row <= MAX_ROWS; row++) {
    int row_height = row * TILE_SIZE;
    if (row_height == gameboard_height) {
      row_height--;
    }
    SDL_RenderDrawLine(renderer, 0, row_height, gameboard_width, row_height);
  }

  for (int col = 0; col <= MAX_COLUMNS; col++) {
    int col_width = col * TILE_SIZE;
    if (col_width == gameboard_width) {
      col_width--;
    }
    SDL_RenderDrawLine(renderer, col_width, 0, col_width, gameboard_height);
  }
}

void drawText(SDL_Color *color, int x, int y, char *msg) {
  SDL_Surface *text_surface = TTF_RenderText_Solid(font, msg, *color);
  SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, text_surface);

  // Draw text such that (x, y) is the center
  SDL_Rect dest = {x - (text_surface->w / 2), y - (text_surface->h / 2), text_surface->w, text_surface->h};
  SDL_RenderCopy(renderer, text, NULL, &dest);

  SDL_DestroyTexture(text);
  SDL_FreeSurface(text_surface);
}

void drawScore(int score) {
  // Get score as a string
  char score_text[10] = {0};
  sprintf(score_text, "%d", score);

  drawText(&COLOR_WHITE, window_width * 3 / 4, TILE_SIZE, "SCORE");
  drawText(&COLOR_WHITE, window_width * 3 / 4, TILE_SIZE * 2, score_text);
}





void gameloop() {
  tilemap_t *tilemap = createTileMap(MAX_ROWS, MAX_COLUMNS);
  tetromino *piece = createRandomTetromino();

  bool is_game_running = true;
  uint32_t last_update_time = SDL_GetTicks();
  uint32_t last_input_time = SDL_GetTicks();
  SDL_Event event;

  // Game loop
  while (is_game_running) {
    uint32_t start_time = SDL_GetTicks();
    bool piece_moved_successfully = false;

    // event handling
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        is_game_running = false;
      }
      else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_RETURN:
            is_game_running = false;
            break;
          case SDLK_w:
            rotateTetrominoRight(tilemap, piece);
            break;
          case SDLK_s:
            rotateTetrominoLeft(tilemap, piece);
            break;
          case SDLK_d:
            movePieceRight(piece, tilemap, &last_input_time);
            break;
          case SDLK_a:
            movePieceLeft(piece, tilemap, &last_input_time);
            break;
          case SDLK_SPACE:
            // end game if piece drops 
            piece_moved_successfully = dropPiece(&piece, tilemap);
            break;
        }
      }
    }

    // update screen
    refreshScreen();

    drawTetromino(piece);
    drawTileMap(tilemap);
    drawGridLines();
    drawScore(score);

    SDL_RenderPresent(renderer);

    // game logic
    piece_moved_successfully = updatePiece(tilemap, &piece, &last_update_time, TICK_RATE);

    // end game if moving piece resulted in a game over
    if (is_game_running) {
      is_game_running = piece_moved_successfully;
    }

    // Cap Framerate
    SDL_Delay(floor(FRAME_INTERVAL - getElapsedTime(start_time)));
  }

  // cleanup game state
  destroyTetromino(piece);
  destroyTileMap(tilemap);
}



void drawTile(tile_t *tile) {
  setRenderColor(tile->color);
  SDL_RenderFillRect(renderer, tile->rect);
}

void drawAnonymousTile(int col, int row, SDL_Color *color) {
  tile_t *tile = createTile(col, row, color, TILE_SIZE);
  drawTile(tile);
  destroyTile(tile);
}

void drawTetromino(tetromino *piece) {
  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      tetromino_state *piece_state = getTetrominoState(piece);
      if ((*piece_state)[row][col]) {
        int board_row = piece->row + row;
        int board_col = piece->col + col;
        drawAnonymousTile(board_col, board_row, getTetrominoColor(piece->type));
      }
    }
  }
}

void drawTileMap(tilemap_t *tilemap) {
  for (int row = 0; row < tilemap->num_rows; row++) {
    for (int col = 0; col < tilemap->num_cols; col++) {
      tile_t *tile = getTile(tilemap, row, col);
      if (tile != NULL) {
        drawTile(getTile(tilemap, row, col));
      }
    }
  }
}

bool updatePiece(tilemap_t *tilemap, tetromino **piece, uint32_t *last_update_time, float game_speed) {
  // check whether enough time has passed since last update
  if (getElapsedTime(*last_update_time) < floor(game_speed)) {
    return true;
  }

  *last_update_time = SDL_GetTicks();

  // apply gravity to piece
  return movePieceDown(piece, tilemap);
}

void movePieceRight(tetromino *piece, tilemap_t *tilemap, uint32_t *last_input_time) {
  // make sure we do not handle inputs too frequently
  if (getElapsedTime(*last_input_time) < INPUT_REFRESH_RATE) {
    return;
  }

  // make move
  piece->col++;

  // check if move results in collision
  if (checkBorderCollisions(piece) || checkTileCollisions(tilemap, piece)) {
    // undo move
    piece->col--;
  }

  *last_input_time = SDL_GetTicks();
}

void movePieceLeft(tetromino *piece, tilemap_t *tilemap, uint32_t *last_input_time) {
  // make sure we do not handle inputs too frequently
  if (getElapsedTime(*last_input_time) < INPUT_REFRESH_RATE) {
    return;
  }

  // make move
  piece->col--;

  // check if move results in collision
  if (checkBorderCollisions(piece) || checkTileCollisions(tilemap, piece)) {
    // undo move
    piece->col++;
  }

  *last_input_time = SDL_GetTicks();
}

// Moves the tetromino down one tile
// Returns true if piece is moved successfully, or false on game over
bool movePieceDown(tetromino **piece_ptr, tilemap_t *tilemap) {
  static bool has_been_on_ground = false;
  tetromino *piece = *piece_ptr;

  // move piece and check whether the move results in a collision
  piece->row++;
  if (isOnFloor(piece) || checkTileCollisions(tilemap, piece)) {
    // undo move
    piece->row--;

    // give the player an extra i-frame when first hitting the ground
    if (!has_been_on_ground) {
      has_been_on_ground = true;
      return true;
    }

    // reset i-frame status
    has_been_on_ground = false;

    // place piece down and check if it results in a game over
    return placePiece(piece_ptr, tilemap);
  }

  return true;
}

// Instantly drops the tetromino onto the ground
// Returns true if piece is placed successfully, or false on game over
bool dropPiece(tetromino **piece_ptr, tilemap_t *tilemap) {
  tetromino *piece = *piece_ptr;

  // move piece down until a collision occurs
  while (!(isOnFloor(piece) || checkTileCollisions(tilemap, piece))) {
    piece->row++;
  }

  // undo collision
  piece->row--;

  // place piece down and check if it results in a game over
  return placePiece(piece_ptr, tilemap);
}

// Places piece down in tilemap and checks if it results in a game over
// Returns true if piece is placed successfully, or false on game over
bool placePiece(tetromino **piece_ptr, tilemap_t *tilemap) {
  tetromino *piece = *piece_ptr;
  bool is_game_over = !tileify(tilemap, piece);
  if (is_game_over) {
    return false;
  }

  // remove any rows that were filled
  handleFilledRows(tilemap);

  // replace current piece with a new tetromino
  destroyTetromino(piece);
  *piece_ptr = createRandomTetromino();

  return true;
}

// Places a tetromino into the tilemap
// Returns true if successful, or false if the piece would be placed out of bounds (game over)
bool tileify(tilemap_t *tilemap, tetromino *piece) {
  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      tetromino_state *piece_state = getTetrominoState(piece);
      if ((*piece_state)[row][col]) {
        int board_row = piece->row + row;
        int board_col = piece->col + col;

        // game is over if
        if (board_row < 0) {
          return false;
        }

        SDL_Color *color = getTetrominoColor(piece->type);
        tile_t *new_tile = createTile(board_col, board_row, color, TILE_SIZE);

        addTile(tilemap, new_tile);
      }
    }
  }

  return true;
}


void handleFilledRows(tilemap_t *tilemap) {
  int rows_filled = 0;
  for (int row = tilemap->num_rows - 1; row >= 0; row--) {
    // check rows from bottom to top to see if filled
    bool is_full_row = true;
    for (int col = 0; col < tilemap->num_cols; col++) {
      // if tile at column is empty, row not filled
      tile_t *tile = getTile(tilemap, row, col);
      if (tile == NULL) {
        is_full_row = false;
        break;
      }
    }

    if (is_full_row) {
      rows_filled++;

      // destroy current row
      for (int col = 0; col < tilemap->num_cols; col++) {
        tile_t *tile = getTile(tilemap, row, col);
        removeTile(tilemap, tile);
      }

      // shift all rows above it down by one
      shiftRowsDown(tilemap, row - 1);

      // recheck current row for filledness since we shifted
      row++;
    }
  }

  // add points to score based on how many lines were cleared
  switch (rows_filled) {
    case 1:
      score += 40;
      break;
    case 2:
      score += 100;
      break;
    case 3:
      score += 300;
      break;
    case 4:
      score += 1200;
      break;
  }
}


void shiftRowsDown(tilemap_t *tilemap, int starting_row) {
  for (int row = starting_row; row >= 0; row--) {
    for (int col = 0; col < tilemap->num_cols; col++) {
      tile_t *shifting_tile = getTile(tilemap, row, col);
      if (shifting_tile != NULL) {
        // copy shifting tile to one row down
        tile_t *new_tile = createTile(col, row + 1, shifting_tile->color, TILE_SIZE);
        addTile(tilemap, new_tile);

        // remove shifting tile from tilemap
        removeTile(tilemap, shifting_tile);
      }
    }
  }
}

bool gameover() {
  printf("Game Over!\n");
  return false;
}




#include "main.h"

static int TILE_SIZE = 25;
static int window_width = 500;
static int window_height = 500;
static int gameboard_width = 250;
static int gameboard_height = 500;

static SDL_Window *window;
static SDL_Renderer *renderer;
static TTF_Font *font;
static Mix_Music *music;

static int high_score = 0;
static int score = 0;
static float difficulty_multiplier = 1.0f;

int main(void) {
  init(); // Initialize SDL2
  
  startMenu();

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

  // Play background music
  music = Mix_LoadMUS("./assets/sounds/music.mp3");
  if (music == NULL) {
    fprintf(stderr, "Failed to load music: %s\n", Mix_GetError());
  }
  else if (Mix_PlayMusic(music, -1) < 0) {
    fprintf(stderr, "Failed to play music: %s\n", Mix_GetError());
  }

  // Load Sound Effects
  // chompSound = Mix_LoadWAV("./assets/sounds/chomp.mp3");
  // if (chompSound == NULL) {
  //   fprintf(stderr, "Failed to load sound: %s\n", Mix_GetError());
  // }


  // Create window and renderer
  window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    fprintf(stderr, "Failed to render window: %s\n", SDL_GetError());
  }

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
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
  // Create game objects
  tilemap_t *tilemap = createTileMap(MAX_ROWS, MAX_COLUMNS);
  tetromino *piece = createRandomTetromino();
  tetromino *next_piece = createRandomTetromino();
  tetromino *hold_piece = createRandomTetromino();

  // User input flags
  bool is_holding_piece = false;
  bool is_pressing_left = false;
  bool is_pressing_right = false;

  // Timing variables
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
      else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
        handleWindowResize(tilemap);
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
          case SDLK_a:
            is_pressing_left = true;
            break;
          case SDLK_d:
            is_pressing_right = true;
            break;
          case SDLK_UP:
            rotateTetrominoRight(tilemap, piece);
            break;
          case SDLK_DOWN:
            rotateTetrominoLeft(tilemap, piece);
            break;
          case SDLK_LEFT:
            is_pressing_left = true;
            break;
          case SDLK_RIGHT:
            is_pressing_right = true;
            break;
          case SDLK_SPACE:
            // end game if piece drops 
            piece_moved_successfully = dropPiece(&piece, &next_piece, tilemap, &is_holding_piece);
            break;
          case SDLK_c:
            if (!is_holding_piece) {
              swapPieces(&piece, &hold_piece);
              is_holding_piece = true;
            }
            break;
        }
      }
      else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
          case SDLK_a:
            is_pressing_left = false;
            break;
          case SDLK_d:
            is_pressing_right = false;
            break;
          case SDLK_LEFT:
            is_pressing_left = false;
            break;
          case SDLK_RIGHT:
            is_pressing_right = false;
            break;
        }
      }
    }

    if (is_pressing_left) {
      movePieceLeft(piece, tilemap, &last_input_time);
    }
    if (is_pressing_right) {
      movePieceRight(piece, tilemap, &last_input_time);
    }

    // update screen
    refreshScreen();

    drawTetromino(piece, getTetrominoColor(piece->type));
    drawGhostPiece(tilemap, piece);
    drawNextTetromino(next_piece);
    drawHeldTetromino(hold_piece);
    drawTileMap(tilemap);
    drawGridLines();
    drawScore(score);

    SDL_RenderPresent(renderer);

    // game logic
    piece_moved_successfully = updatePiece(tilemap, &piece, &next_piece, &is_holding_piece, &last_update_time, TICK_RATE * difficulty_multiplier);

    // end game if moving piece resulted in a game over
    if (is_game_running) {
      is_game_running = piece_moved_successfully;
    }

    // Cap Framerate
    Uint32 delay = (Uint32) floor(FRAME_INTERVAL - getElapsedTime(start_time));
    if (delay > FRAME_INTERVAL) {
      delay = 0;
    }
    SDL_Delay(delay);
  }

  // cleanup game state
  destroyTetromino(piece);
  destroyTetromino(next_piece);
  destroyTileMap(tilemap);
}


void startMenu() {
  bool is_ready = false;
  SDL_Event event;
  while (!is_ready) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        cleanup_SDL();
      }
      else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
        handleWindowResize(NULL);
      }
      else if (event.type == SDL_KEYDOWN) {
        is_ready = true;
      }
    }

    refreshScreen();
    drawText(&COLOR_WHITE, window_width / 2, window_height / 4, "Press any key to start");
    SDL_RenderPresent(renderer);

    SDL_Delay(FRAME_INTERVAL);
  }
}


void drawTile(tile_t *tile) {
  setRenderColor(tile->color);
  SDL_RenderFillRect(renderer, tile->rect);
}

void drawAnonymousTile(int row, int col, SDL_Color *color) {
  tile_t *tile = createTile(row, col, color, TILE_SIZE);
  drawTile(tile);
  destroyTile(tile);
}

void drawTetromino(tetromino *piece, SDL_Color *color) {
  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      tetromino_state *piece_state = getTetrominoState(piece);
      if ((*piece_state)[row][col]) {
        int board_row = piece->row + row;
        int board_col = piece->col + col;
        drawAnonymousTile(board_row, board_col, color);
      }
    }
  }
}

void drawNextTetromino(tetromino *piece) {
  static SDL_Rect outline;
  outline.x = 12 * TILE_SIZE;
  outline.y = 4 * TILE_SIZE;
  outline.w = 6 * TILE_SIZE;
  outline.h = 7 * TILE_SIZE;
  setRenderColor(&COLOR_WHITE);
  SDL_RenderDrawRect(renderer, &outline);

  drawText(&COLOR_WHITE, 15 * TILE_SIZE, 5 * TILE_SIZE, "Next");

  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      tetromino_state *piece_state = getTetrominoState(piece);
      if ((*piece_state)[row][col]) {
        int next_piece_row = 6 + row;
        int next_piece_col = 12 + col;
        drawAnonymousTile(next_piece_row, next_piece_col, getTetrominoColor(piece->type));
      }
    }
  }
}

void drawHeldTetromino(tetromino *piece) {
  static SDL_Rect outline;
  outline.x = 12 * TILE_SIZE;
  outline.y = 12 * TILE_SIZE;
  outline.w = 6 * TILE_SIZE;
  outline.h = 7 * TILE_SIZE;
  setRenderColor(&COLOR_WHITE);
  SDL_RenderDrawRect(renderer, &outline);

  drawText(&COLOR_WHITE, 15 * TILE_SIZE, 13 * TILE_SIZE, "Hold");

  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      tetromino_state *piece_state = getTetrominoState(piece);
      if ((*piece_state)[row][col]) {
        int next_piece_row = 14 + row;
        int next_piece_col = 12 + col;
        drawAnonymousTile(next_piece_row, next_piece_col, getTetrominoColor(piece->type));
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

void drawGhostPiece(tilemap_t *tilemap, tetromino *piece) {
  // clone piece
  tetromino *ghost = createTetromino(piece->type, piece->row, piece->col);
  ghost->state_idx = piece->state_idx;

  // drop piece until there is a collision
  while (!isOnFloor(ghost) && !checkTileCollisions(tilemap, ghost)) {
    ghost->row++;
  }
  ghost->row--;

  SDL_Color *piece_color = getTetrominoColor(piece->type);
  SDL_Color ghost_color = { piece_color->r, piece_color->g, piece_color->b, piece_color->a * 0.5 };
  drawTetromino(ghost, &ghost_color);

  destroyTetromino(ghost);
}

// Moves the tetromino down one tile every game tick
// Returns true if piece is moved successfully, or false on game over
bool updatePiece(tilemap_t *tilemap, tetromino **piece_ptr, tetromino **next_piece_ptr, bool *is_holding_piece, uint32_t *last_update_time, float game_speed) {
  // check whether enough time has passed since last update
  if (getElapsedTime(*last_update_time) < floor(game_speed)) {
    return true;
  }

  *last_update_time = SDL_GetTicks();

  // apply gravity to piece
  return movePieceDown(piece_ptr, next_piece_ptr, tilemap, is_holding_piece);
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
bool movePieceDown(tetromino **piece_ptr, tetromino **next_piece_ptr, tilemap_t *tilemap, bool *is_holding_piece) {
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
    return placePiece(piece_ptr, next_piece_ptr, tilemap, is_holding_piece);
  }

  return true;
}


// Instantly drops the tetromino onto the ground
// Returns true if piece is placed successfully, or false on game over
bool dropPiece(tetromino **piece_ptr, tetromino **next_piece_ptr, tilemap_t *tilemap, bool *is_holding_piece) {
  tetromino *piece = *piece_ptr;

  // move piece down until a collision occurs
  while (!(isOnFloor(piece) || checkTileCollisions(tilemap, piece))) {
    piece->row++;
  }

  // undo collision
  piece->row--;

  // place piece down and check if it results in a game over
  return placePiece(piece_ptr, next_piece_ptr, tilemap, is_holding_piece);
}

// Places piece down in tilemap and checks if it results in a game over
// Returns true if piece is placed successfully, or false on game over
bool placePiece(tetromino **piece_ptr, tetromino **next_piece_ptr, tilemap_t *tilemap, bool *is_holding_piece) {
  tetromino *piece = *piece_ptr;
  bool is_game_over = !tileify(tilemap, piece);
  if (is_game_over) {
    return false;
  }

  // remove any rows that were filled
  handleFilledRows(tilemap);

  // replace current piece with a new tetromino
  destroyTetromino(piece);
  *piece_ptr = *next_piece_ptr;
  *next_piece_ptr = createRandomTetromino();
  *is_holding_piece = false;

  return true;
}


void swapPieces(tetromino **curr_piece, tetromino **hold_piece) {
  // reset position of current piece
  (*curr_piece)->row = -TETROMINO_WIDTH;
  (*curr_piece)->col = (MAX_COLUMNS - TETROMINO_WIDTH) / 2;
  
  // hold current piece and switch to next piece
  tetromino *temp = *hold_piece;
  *hold_piece = *curr_piece;
  *curr_piece = temp;
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
        tile_t *new_tile = createTile(board_row, board_col, color, TILE_SIZE);

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

  // increase difficulty
  difficulty_multiplier *= 0.99f;
}


void shiftRowsDown(tilemap_t *tilemap, int starting_row) {
  for (int row = starting_row; row >= 0; row--) {
    for (int col = 0; col < tilemap->num_cols; col++) {
      tile_t *shifting_tile = getTile(tilemap, row, col);
      if (shifting_tile != NULL) {
        // copy shifting tile to one row down
        tile_t *new_tile = createTile(row + 1, col, shifting_tile->color, TILE_SIZE);
        addTile(tilemap, new_tile);

        // remove shifting tile from tilemap
        removeTile(tilemap, shifting_tile);
      }
    }
  }
}

bool gameover() {
  if (score > high_score) {
    high_score = score;
  }

  // reset game vars
  score = 0;
  difficulty_multiplier = 1.0f;

  // listen for user choice
  SDL_Event event;
  while (true) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        return false;
      }
      else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
        handleWindowResize(NULL);
      }
      else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            return false;
          case SDLK_RETURN:
            return true;
        }
      }
    }

    // draw text
    refreshScreen();
    drawText(&COLOR_WHITE, window_width / 2, window_height / 4, "Game Over!");

    char score_text[20] = {0};
    sprintf(score_text, "Your Score: %d", score);
    drawText(&COLOR_WHITE, window_width / 2, (window_height / 2) - TILE_SIZE, score_text);

    char high_score_text[20] = {0};
    sprintf(high_score_text, "High Score: %d", high_score);
    drawText(&COLOR_WHITE, window_width / 2, (window_height / 2), high_score_text);

    drawText(&COLOR_WHITE, window_width / 2, (window_height * 3 / 4), "Press Esc to exit or Enter to play again!");
    SDL_RenderPresent(renderer);

    // Cap Framerate
    SDL_Delay(FRAME_INTERVAL);
  }

  return false;
}

void handleWindowResize(tilemap_t *tilemap) {
  // adjust tile size if window size was changed
  SDL_GetWindowSize(window, &window_width, &window_height);
  TILE_SIZE = window_height / MAX_ROWS;

  window_width = MAX_COLUMNS * TILE_SIZE * 2;
  gameboard_width = MAX_COLUMNS * TILE_SIZE;
  gameboard_height = MAX_ROWS * TILE_SIZE;
  SDL_SetWindowSize(window, window_width, window_height);
  
  if (tilemap != NULL) {
    resizeTileMap(tilemap, TILE_SIZE);
  }
}




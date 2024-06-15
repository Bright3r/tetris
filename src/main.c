#include "main.h"

static const int window_width = MAX_COLUMNS * TILE_SIZE;
static const int window_height = MAX_ROWS * TILE_SIZE;

static SDL_Window *window;
static SDL_Renderer *renderer;
static TTF_Font *font;
static Mix_Music *music;

int main(void) {
  init(); // Initialize SDL2
  
  tilemap_t *tilemap = createTileMap(MAX_ROWS, MAX_COLUMNS);
  tetromino *piece = createRandomTetromino();

  static bool is_moving_left = false;
  static bool is_moving_right = false;

  // Gameloop
  bool is_game_running = true;
  uint32_t last_update_time = SDL_GetTicks();
  uint32_t last_input_time = SDL_GetTicks();
  SDL_Event event;
  while (is_game_running) {
    uint32_t start_time = SDL_GetTicks();

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
            is_moving_right = true;
            break;
          case SDLK_a:
            is_moving_left = true;
            break;
        }
      }
      else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
          case SDLK_d:
            is_moving_right = false;
            break;
          case SDLK_a:
            is_moving_left = false;
            break;
        }
      }
    }

    // execute player inputs
    if (is_moving_right) {
      movePieceRight(piece, tilemap, &last_input_time);
    }
    if (is_moving_left) {
      movePieceLeft(piece, tilemap, &last_input_time);
    }


    // update screen
    refreshScreen();

    drawTetromino(piece);
    drawTileMap(tilemap);

    SDL_RenderPresent(renderer);

    // game logic
    updatePiece(tilemap, &piece, &last_update_time, TICK_RATE);


    // Cap Framerate
    SDL_Delay(floor(FRAME_INTERVAL - getElapsedTime(start_time)));
  }

  
  // cleanup
  destroyTetromino(piece);
  destroyTileMap(tilemap);

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

void updatePiece(tilemap_t *tilemap, tetromino **piece, uint32_t *last_update_time, float game_speed) {
  // check whether enough time has passed since last update
  if (getElapsedTime(*last_update_time) < floor(game_speed)) {
    return;
  }

  // apply gravity to piece
  movePieceDown(piece, tilemap);

  *last_update_time = SDL_GetTicks();
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

void movePieceDown(tetromino **piece_ptr, tilemap_t *tilemap) {
  static bool has_been_on_ground = false;

  // move piece and check whether the move results in a collision
  (*piece_ptr)->row++;
  if (isOnFloor(*piece_ptr) || checkTileCollisions(tilemap, *piece_ptr)) {
    // undo move
    (*piece_ptr)->row--;

    // give the player an extra i-frame when first hitting the ground
    if (!has_been_on_ground) {
      has_been_on_ground = true;
      return;
    }

    // place piece down
    tileify(tilemap, *piece_ptr);

    // replace current piece with a new tetromino
    destroyTetromino(*piece_ptr);
    *piece_ptr = createRandomTetromino();

    // reset i-frame status
    has_been_on_ground = false;
  }
}

void tileify(tilemap_t *tilemap, tetromino *piece) {
  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      tetromino_state *piece_state = getTetrominoState(piece);
      if ((*piece_state)[row][col]) {
        int board_row = piece->row + row;
        int board_col = piece->col + col;
        SDL_Color *color = getTetrominoColor(piece->type);
        tile_t *new_tile = createTile(board_col, board_row, color, TILE_SIZE);

        addTile(tilemap, new_tile);
      }
    }
  }
}

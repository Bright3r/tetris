#include "main.h"

static const int window_width = MAX_COLUMNS * TILE_SIZE;
static const int window_height = MAX_ROWS * TILE_SIZE;

static SDL_Window *window;
static SDL_Renderer *renderer;
static TTF_Font *font;
static Mix_Music *music;


int main(void) {
  init(); // Initialize SDL2

  // int random_piece_number = rand() % NUM_TETROMINO_TYPES;
  int random_piece_number = 0;
  tetromino *piece = createTetromino(getTetrominoType(random_piece_number), 10, 0);

  // Gameloop
  bool isGameRunning = true;
  SDL_Event event;
  uint32_t last_update_time = SDL_GetTicks();
  while (isGameRunning) {
    uint32_t startTime = SDL_GetTicks();

    // event handling
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        isGameRunning = false;
      }
      else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_RETURN:
            isGameRunning = false;
            break;
          case SDLK_w:
            rotateTetrominoRight(piece);
            break;
          case SDLK_s:
            rotateTetrominoLeft(piece);
            break;
          case SDLK_d:
            movePieceRight(piece);
            break;
          case SDLK_a:
            movePieceLeft(piece);
            break;
        }
      }
    }

    refreshScreen();

    // drawAnonymousTile(1, 1, &COLOR_WHITE);
    // drawAnonymousTile(1, 2, &COLOR_WHITE);
    // drawAnonymousTile(1, 3, &COLOR_WHITE);
    // drawAnonymousTile(1, 4, &COLOR_WHITE);
    //
    // drawAnonymousTile(5, 4, &COLOR_WHITE);
    // drawAnonymousTile(6, 4, &COLOR_WHITE);
    // drawAnonymousTile(7, 4, &COLOR_WHITE);
    // drawAnonymousTile(8, 4, &COLOR_WHITE);
    
    // game logic
    drawTetromino(piece);
    updatePiece(piece, &last_update_time, 100.0f);

    SDL_RenderPresent(renderer);

    // Cap Framerate
    uint32_t currTime = SDL_GetTicks();
    float elapsedTime = currTime - startTime;
    SDL_Delay(floor(FRAME_INTERVAL - elapsedTime));
  }

  
  destroyTetromino(piece);


  printf("Sucess!\n");
  cleanup_SDL();
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

tile *createTile(int col, int row, SDL_Color *color) {  
  SDL_Rect *rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
  rect->y = row * TILE_SIZE;
  rect->x = col * TILE_SIZE;
  rect->w = TILE_SIZE;
  rect->h = TILE_SIZE;

  tile *new_tile = (tile *) malloc(sizeof(tile));
  new_tile->rect = rect;
  new_tile->color = color;
  return new_tile;
}

void destroyTile(tile *tile) {
  free(tile->rect);
  free(tile);
}

void drawTile(tile *my_tile) {
  setRenderColor(my_tile->color);
  SDL_RenderFillRect(renderer, my_tile->rect);
}

void drawAnonymousTile(int col, int row, SDL_Color *color) {
  tile *tile = createTile(col, row, color);
  drawTile(tile);
  destroyTile(tile);
}

void drawTetromino(tetromino *piece) {
  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      if (piece->state[row][col]) {
        int board_row = piece->row + row;
        int board_col = piece->col + col;
        drawAnonymousTile(board_col, board_row, getTetrominoColor(piece->type));
        // printf("Board Row: %d, Board Col: %d\n", board_row, board_col);
      }
    }
  }
}



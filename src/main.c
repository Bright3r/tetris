#include "main.h"

static const int window_width = 600;
static const int window_height = 600;

static SDL_Window *window;
static SDL_Renderer *renderer;
static TTF_Font *font;
static Mix_Music *music;


int main(void) {
  init(); // Initialize SDL2

  tetromino *pieces[NUM_TETROMINO_TYPES] = {0};
  for (int i = 0; i < NUM_TETROMINO_TYPES; i++) {
    pieces[i] = createTetromino(getTetrominoType(i), i * 2, i * 2);
    rotateTetrominoLeft(pieces[i]);
  }


  // Gameloop
  bool isGameRunning = true;
  SDL_Event event;
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
        }
      }
    }

    refreshScreen();
    for (int i = 0; i < NUM_TETROMINO_TYPES; i++) {
      drawTetromino(pieces[i]);
    }
    // drawAnonymousTile(4, 4, &COLOR_WHITE);
    // drawAnonymousTile(4, 5, &COLOR_DARK_BLUE);
    // drawAnonymousTile(4, 6, &COLOR_LIGHT_BLUE);
    // drawAnonymousTile(4, 7, &COLOR_ORANGE);

    SDL_RenderPresent(renderer);

    // Cap Framerate
    uint32_t currTime = SDL_GetTicks();
    float elapsedTime = currTime - startTime;
    SDL_Delay(floor(FRAME_INTERVAL - elapsedTime));
  }

  
  for (int i = 0; i < NUM_TETROMINO_TYPES; i++) {
    destroyTetromino(pieces[i]);
  }


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

tile *createTile(int row, int col, SDL_Color *color) {  
  SDL_Rect *rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
  rect->x = row * TILE_SIZE;
  rect->y = col * TILE_SIZE;
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

void drawAnonymousTile(int row, int col, SDL_Color *color) {
  tile *tile = createTile(row, col, color);
  drawTile(tile);
  destroyTile(tile);
}

void drawTetromino(tetromino *piece) {
  for (int row = 0; row < TETROMINO_WIDTH; row++) {
    for (int col = 0; col < TETROMINO_WIDTH; col++) {
      if (piece->state[row][col]) {
        int board_row = piece->row + col;
        int board_col = piece->col + row;
        drawAnonymousTile(board_row, board_col, getTetrominoColor(piece->type));
      }
    }
  }
}


#include <stdlib.h>

typedef struct tile_t {
  SDL_Rect *rect; 
  SDL_Color *color;
  int row, col;
} tile_t;

typedef struct tilemap_t {
  tile_t **map;
  int num_tiles;
} tilemap_t;

tile_t *createTile(int col, int row, SDL_Color *color, int tile_size);
void destroyTile(tile_t *tile);
tilemap_t *createTileMap(int num_rows, int num_cols);
void destroyTileMap(tilemap_t *tilemap);
void addTile(tilemap_t *tilemap, tile_t *tile);




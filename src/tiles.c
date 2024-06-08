
#include "tiles.h"

tile_t *createTile(int col, int row, SDL_Color *color, int tile_size) {  
  SDL_Rect *rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
  rect->y = row * tile_size;
  rect->x = col * tile_size;
  rect->w = tile_size;
  rect->h = tile_size;

  tile_t *tile = (tile_t *) malloc(sizeof(tile_t));
  tile->rect = rect;
  tile->color = color;
  tile->row = row;
  tile->col = col;
  return tile;
}

void destroyTile(tile_t *tile) {
  free(tile->rect);
  free(tile);
}

tilemap_t *createTileMap(int num_rows, int num_cols) {
  tilemap_t *tilemap = (tilemap_t *) malloc(sizeof(tilemap));
  tilemap->map = (tile_t **) calloc(num_rows * num_cols, sizeof(tile_t *));
  tilemap->num_tiles = 0;

  return tilemap;
}

void destroyTileMap(tilemap_t *tilemap) {
  for (int i = 0; i < tilemap->num_tiles; i++) {
    destroyTile(tilemap->map[i]);
  }

  free(tilemap->map);
  free(tilemap);
}

void addTile(tilemap_t *tilemap, tile_t *tile) {
  tilemap->map[tilemap->num_tiles++] = tile;
}



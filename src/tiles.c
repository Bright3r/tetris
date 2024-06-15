
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
  tilemap->num_rows = num_rows;
  tilemap->num_cols = num_cols;

  bool *is_filled = (bool *) calloc(num_rows * num_cols, sizeof(bool));
  tilemap->is_filled = is_filled;

  return tilemap;
}

void destroyTileMap(tilemap_t *tilemap) {
  for (int row = 0; row < tilemap->num_rows; row++) {
    for (int col = 0; col < tilemap->num_cols; col++) {
      int tile_idx = getTileMapIdx(tilemap, row, col);
      if (tilemap->is_filled[tile_idx]) {
        tile_t *tile = getTile(tilemap, row, col);
        destroyTile(tile);
      }
    }
  }

  free(tilemap->map);
  free(tilemap);
}

int getTileMapIdx(tilemap_t *tilemap, int row, int col) {
  return (row * tilemap->num_cols) + col;
}

int getTileIdx(tilemap_t *tilemap, tile_t *tile) {
  return (tile->row * tilemap->num_cols) + tile->col;
}

tile_t *getTile(tilemap_t *tilemap, int row, int col) {
  int tile_idx = getTileMapIdx(tilemap, row, col);
  if (tilemap->is_filled[tile_idx]) {
    return tilemap->map[tile_idx];
  }
  return NULL;
}

void addTile(tilemap_t *tilemap, tile_t *tile) {
  int tile_idx = getTileIdx(tilemap, tile);
  tilemap->map[tile_idx] = tile;
  tilemap->is_filled[tile_idx] = true;
}


void removeTile(tilemap_t *tilemap, tile_t *tile) {
  int tile_idx = getTileIdx(tilemap, tile);
  destroyTile(tile);

  tilemap->map[tile_idx] = NULL;
  tilemap->is_filled[tile_idx] = false;
}




#pragma once

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct tile_t {
  SDL_Rect *rect; 
  SDL_Color *color;
  int row, col;
} tile_t;

typedef struct tilemap_t {
  tile_t **map;
  bool *is_filled;
  int num_rows, num_cols;
} tilemap_t;

tile_t *createTile(int row, int col, SDL_Color *color, int tile_size);
void destroyTile(tile_t *tile);
tilemap_t *createTileMap(int num_rows, int num_cols);
int getTileMapIdx(tilemap_t *tilemap, int row, int col);
int getTileIdx(tilemap_t *tilemap, tile_t *tile);
void destroyTileMap(tilemap_t *tilemap);
void addTile(tilemap_t *tilemap, tile_t *tile);
void removeTile(tilemap_t *tilemap, tile_t *tile);
tile_t *getTile(tilemap_t *tilemap, int row, int col);
void resizeTileMap(tilemap_t *tilemap, int new_tile_size);

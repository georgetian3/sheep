#ifndef TILE_H
#define TILE_H


#define N_TILE_TYPES 3
#define N_TILES 9

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#include <windows.h>
#include "button.h"

HBITMAP tile_bitmaps[N_TILE_TYPES];
void load_tiles() {
    const char* filenames[] = {
        "../res/carrot.bmp",
        "../res/corn.bmp",
        "../res/grass.bmp",
    };
    for (int i = 0; i < N_TILE_TYPES; i++) {
        tile_bitmaps[i] = (HBITMAP)LoadImage(NULL, filenames[i], IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
        if (tile_bitmaps[i] == NULL) {
            printf("LoadImage failed: %d", i);
            exit(1);
        }
    }
}

struct Tile {
    HWND hWnd;
    int enabled;
    int type;
    int id;
    int i;
    int j;
    int k;
};

int __tile_count = 0;
struct Tile* tiles[N_TILES];

void init_tiles() {
    for (int i = 0; i < N_TILES; i++) {
        tiles[i] = 0;
    }
}

struct Tile* get_tile(HWND hWnd) {
    for (int i = 0; i < N_TILES; i++) {
        if (tiles[i]->hWnd == hWnd) {
            return tiles[i];
        }
    }
    return 0;
}

int i_to_x(int i) {
    return i;
}

int j_to_y(int j) {
    return j;
}

struct Tile* create_tile(HWND parent, int enabled, int type, int id, int i, int j, int k) {
    if (__tile_count >= N_TILES) {
        return 0;
    }
    for (int c = 0; c < N_TILES; c++) {
        if (tiles[c] == 0) {
            struct Tile* new_tile = (struct Tile*)malloc(sizeof(struct Tile));
            new_tile->hWnd = create_button(parent, id, TILE_WIDTH, TILE_HEIGHT);
            new_tile->enabled = enabled;
            new_tile->i = i;
            new_tile->j = j;
            new_tile->k = k;
            tiles[c] = new_tile;
            return new_tile;
        }
    }
    return 0;
}

void delete_tile(HWND hWnd) {
    for (int i = 0; i < N_TILES; i++) {
        if (tiles[i]->hWnd == hWnd) {
            free(tiles[i]);
            tiles[i] = 0;
            return;
        }
    }
}

#endif
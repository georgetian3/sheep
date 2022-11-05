#ifndef TILE_H
#define TILE_H

#define N_TILE_TYPES 3
#define N_TILES 9

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#include <windows.h>
#include "button.h"

const char* filenames[2][N_TILE_TYPES] = {
    {
        "../res/carrot.bmp",
        "../res/corn.bmp",
        "../res/grass.bmp",
    },
    {
        "../res/carrot_dark.bmp",
        "../res/corn_dark.bmp",
        "../res/grass_dark.bmp",
    },
};


HBITMAP tile_bitmaps[2][N_TILE_TYPES];

void load_tiles() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < N_TILE_TYPES; j++) {

            tile_bitmaps[i][j] = (HBITMAP)LoadImage(NULL, filenames[i][j], IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
            if (tile_bitmaps[i] == NULL) {
                printf("LoadImage failed: %d", i);
                exit(1);
            }
        }
    }
}


int i_to_x(int i) {
    return i;
}

int j_to_y(int j) {
    return j;
}


#endif
#ifndef TILE_H
#define TILE_H

#define N_TILE_TYPES 5
#define N_TILES 9
#define N_BUTTON_TYPES 4

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#include <windows.h>


#define TYPE_START      -2
#define TYPE_SETTINGS   -1
#define TYPE_CARROT     0
#define TYPE_CORN       1
#define TYPE_GRASS      2
#define TYPE_TREE       3
#define TYPE_FORK       4
#define TYPE_FIRE       5
#define TYPE_GLOVE      6
#define TYPE_COTTON     7
#define TYPE_RICE       8
#define TYPE_HAY        9


const char* tile_names[N_TILE_TYPES] = {
    "carrot", "corn", "grass", "tree", "fork",
};

const char* dir = "../res/";
const char* ext = ".bmp";
const char* dark = "_dark";

#define MAX_FILENAME_LEN 255

HBITMAP bitmaps[2][N_TILE_TYPES];


HBITMAP load_bitmap(const char* filename, int wd, int ht) {
    return LoadImage(NULL, filename, IMAGE_BITMAP, wd, ht, LR_LOADFROMFILE);
}


void load_bitmaps() {
    char filename[MAX_FILENAME_LEN];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < N_TILE_TYPES; j++) {
            memset(filename, 0, MAX_FILENAME_LEN);
            strcat(filename, dir);
            strcat(filename, tile_names[j]);
            if (i == 0) {
                strcat(filename, dark);
            }
            strcat(filename, ext);
            bitmaps[i][j] = load_bitmap(filename, TILE_WIDTH, TILE_HEIGHT);
            if (bitmaps[i] == NULL) {
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
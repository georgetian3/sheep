#ifndef TILE_H
#define TILE_H

#define N_TILE_TYPES 16
#define N_TILES 18

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#include <windows.h>

#define TYPE_CARROT     0
#define TYPE_CORN       1
#define TYPE_GRASS      2
#define TYPE_TREE       3
#define TYPE_FORK       4
#define TYPE_BELL       5
#define TYPE_BRUSH      6
#define TYPE_BUCKET     7
#define TYPE_CABBAGE    8
#define TYPE_FIRE       9
#define TYPE_HAY        10
#define TYPE_GLOVE      11
#define TYPE_MILK       12
#define TYPE_PLIERS     13
#define TYPE_RICE       14
#define TYPE_ROPE       15
#define N_TILE_TYPES    16
#define TYPE_START      16
#define TYPE_UNDO       17
#define N_BUTTON_TYPES  18

const char* bitmap_names[N_BUTTON_TYPES] = {
    "carrot", "corn", "grass", "tree", "fork", "bell",
    "brush", "bucket", "cabbage", "fire", "hay", "gloves",
    "milk", "pliers", "rice", "rope", "start", "undo",
};

const char* dir = "../res/";
const char* ext = ".bmp";
const char* dark = "_dark";

#define MAX_FILENAME_LEN 255

HBITMAP bitmaps[2][N_BUTTON_TYPES]; // bitmaps[0] contains the dark versions, and bitmaps[1] the light

HBITMAP load_bitmap(const char* filename) {
    return LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}


void load_bitmaps() {
    char filename[MAX_FILENAME_LEN];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < N_BUTTON_TYPES; j++) {
            memset(filename, 0, MAX_FILENAME_LEN);
            strcat(filename, dir);
            strcat(filename, bitmap_names[j]);
            if (i == 0) {
                strcat(filename, dark);
            }
            strcat(filename, ext);
            bitmaps[i][j] = load_bitmap(filename);
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
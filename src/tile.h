#ifndef TILE_H
#define TILE_H


#define N_TILE_TYPES 3
#define N_TILES 9

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#include <windows.h>

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



HWND tile_buttons[N_TILES];

int tile_type(HWND btn) {
    // https://learn.microsoft.com/en-us/windows/win32/controls/bm-getimage
    // given a button, returns the index of the hbitmap applied to it in tile_bitmaps
    HBITMAP bm = (HBITMAP)SendMessage(
        btn,
        BM_GETIMAGE,
        IMAGE_BITMAP,
        0
    );
    for (int i = 0; i < N_TILE_TYPES; i++) {
        if (bm == tile_bitmaps[i]) {
            return i;
        }
    }
    return -1;
}


#endif
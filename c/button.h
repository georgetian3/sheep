#ifndef BUTTON_H
#define BUTTON_H

#include <stdio.h>
#include <windows.h>

//#include "sound.h"
#include "tile.h"

#define N_BUTTONS 500

#define STATE_DISABLED     0
#define STATE_ENABLED      1
#define STATE_HIDDEN       2
#define N_STATES           3


struct Button {
    HWND hWnd;

    BOOL active;
    BOOL moving;
    BOOL gray;
    BOOL in_slot;
    void (*callback)();
    int type;
    int id;
    int k; // vertical axis
    // properties used for button animation
    POINT start_pos;
    int start_time;
    double dxpf; // delta x per frame - amount of horizontal pixels to move per frame
    double dypf;
    int frame; // current frame
    int frames; // number of frames of movement
};
struct Button* start_game;
struct Button* end_game;
struct Button* undo_btn;
int __button_count = 0;
struct Button* __buttons[N_BUTTONS] = {0};
int last_x =0;
int last_y =0;
int last_index = -1;
struct Button* last_button = 0;
int __search_struct(struct Button* btn) {
    for (int i = 0; i < N_BUTTONS; i++) {
        if (__buttons[i] == btn) {
            return i;
        }
    }
    return -1;
}

int __search_hwnd(HWND hWnd) {
    for (int i = 0; i < N_BUTTONS; i++) {
        if (__buttons[i] && __buttons[i]->hWnd == hWnd) {
            return i;
        }
    }
    return -1;
}


void win_pos(HWND hWnd, POINT* point) {
    RECT rect;
    GetWindowRect(hWnd, &rect);
    MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)&rect, 2);
    point->x = rect.left;
    point->y = rect.top;
}

HWND id_to_hwnd(HWND parent, int id) {
    //https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getdlgitem
    return GetDlgItem(parent, id);
}

    

struct Button* get_button_index(int index) {
    if (index < 0 || index >= N_BUTTONS) {
        printf("get_button bad index\n");
        exit(1);
    }
    return __buttons[index];
}

struct Button* get_button(HWND hWnd) {
    /*
    * given the hWnd of the button, returns
    * the pointer to the Button struct, or
    * 0 if the struct was not found
    */
    int index = __search_hwnd(hWnd);
    if (index == -1) {
        return 0;
    }
    return __buttons[index];
}

int __id_count = 0;

struct Button* create_button(
        HWND parent, int type,
        int x, int y, int k, int wd, int ht) {
    /*
    * returns the pointer to the newly created Button struct,
    * or if the struct cannot be created
    */


    if (__button_count >= N_BUTTONS) {
        return 0;
    }
    int index = __search_struct(0);
    if (index == -1) {
        printf("No empty button struct???\n");
        exit(1);
    }

    // find unused id
    while (id_to_hwnd(parent, __id_count)) {
        __id_count++;
    }

    struct Button* new_btn = (struct Button*)malloc(sizeof(struct Button));
    new_btn->hWnd = (HWND)CreateWindowEx(0, "button", NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_OWNERDRAW , x, y, wd, ht, parent, (HMENU)__id_count, NULL, NULL);

    if (new_btn->hWnd == NULL) {
        printf("CreateWindow failed\n");
        exit(1);
    }

    new_btn->k = k;
    new_btn->active = TRUE;
    new_btn->gray = FALSE;
    new_btn->moving = FALSE;
    new_btn->in_slot = FALSE;
    new_btn->type = type;
    new_btn->callback = 0;
    __buttons[index] = new_btn;
    __button_count++;

    return new_btn;

}

void __delete_button(int i) {
    printf("Delete button %d\n", i);
    DestroyWindow(__buttons[i]->hWnd);
    free(__buttons[i]);
    __buttons[i] = 0;
    __button_count--;
}



void delete_button_struct(struct Button* btn) {
    int index = __search_struct(btn);
    if (index == -1) {
        printf("Delete struct not found\n");
        exit(1);
    }
    __delete_button(index);
}

void delete_button_hwnd(HWND hWnd) {
    int index = __search_hwnd(hWnd);
    if (index == -1) {
        printf("Delete hwnd not found\n");
        exit(1);
    }
    __delete_button(index);
}

void draw_button(HWND parent, int id, DRAWITEMSTRUCT* dis) {
    //printf("Draw button\n");
    struct Button* btn = get_button(id_to_hwnd(parent, id));

    LPARAM lBitmap;

    if (btn == 0) {
        printf("Draw button not found\n");
        exit(1);
    } else if(btn == start_game){
        lBitmap = start_bmp;
    } else if (btn==end_game){
        lBitmap = end_bmp;
    } 
    else{
        lBitmap = bitmaps[!btn->gray][btn->type];
    }
    DrawStateW(
        dis->hDC, 0, 0,
        (LPARAM)(end_bmp),
         0, 0, 0, 0, 0, DST_BITMAP
    );
    }else{
        DrawStateW(
        dis->hDC, 0, 0,
        (LPARAM)(bitmaps[!btn->gray][btn->type]),
         0, 0, 0, 0, 0, DST_BITMAP
    );
}


void set_active(struct Button* btn, BOOL active) {
    btn->active = active;
    EnableWindow(btn->hWnd, active);
}

BOOL is_tile(struct Button* btn) {
    return btn->type >= 0 && btn->type < N_TILE_TYPES;
}



#endif
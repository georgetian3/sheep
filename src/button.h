#ifndef BUTTON_H
#define BUTTON_H

#include <stdio.h>
#include <windows.h>

#include "sound.h"
#include "tile.h"

#define N_BUTTONS 100

#define BUTTON_STATUS_DISABLED 0
#define BUTTON_STATUS_ENABLED 1
#define BUTTON_STATUS_MOVING 2

struct Button {
    HWND hWnd;

    int status;

    HBITMAP img_enabled;
    HBITMAP img_disabled;
    int type;
    long long id;
    int k; // vertical axis

    // properties used for button animation
    POINT start_pos;
    int start_time;
    double dxpf; // delta x per frame - amount of horizontal pixels to move per frame
    double dypf;
    int frame; // current frame
    int frames; // number of frames of movement
};

int __button_count = 0;
struct Button* __buttons[N_BUTTONS] = {0};

void show_button(HWND btn, BOOL visible) {
    ShowWindow(btn, visible ? SW_SHOW : SW_HIDE);
}



void enable_button(HWND btn, BOOL enable) {

    EnableWindow(btn, enable);
}

HWND id_to_hwnd(HWND parent, int id) {
    //https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getdlgitem
    return GetDlgItem(parent, id);
}





/*
 * given the hWnd of the button, returns
 * the pointer to the Button struct, or
 * 0 if the struct was not found
 */
struct Button* get_button(HWND hWnd) {
    for (int i = 0; i < N_BUTTONS; i++) {
        if (__buttons[i] && __buttons[i]->hWnd == hWnd) {
            return __buttons[i];
        }
    }
    printf("Get button not found\n");
    exit(1);
}

/*
 * returns the pointer to the newly created Button struct,
 * or if the struct cannot be created
 */
struct Button* create_button(
        HWND parent, int status,
        HBITMAP img_enabled,
        HBITMAP img_disabled,
        long long id, int x, int y, int k, int wd, int ht) {

    if (__button_count >= N_BUTTONS) {
        return 0;
    }
    for (int i = 0; i < N_TILES; i++) {
        if (__buttons[i] == 0) {
            struct Button* new_btn = (struct Button*)malloc(sizeof(struct Button));

            printf("new button %d\n", i);

            new_btn->hWnd = (HWND)CreateWindowEx(0, "button", NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP | WS_CLIPSIBLINGS | BS_OWNERDRAW, x, y, wd, ht, parent, (HMENU)id, NULL, NULL);

            if (new_btn->hWnd == NULL) {
                printf("CreateWindow failed\n");
            } else {
                printf("New hwnd %u\n", new_btn->hWnd);
            }
            if (status == BUTTON_STATUS_MOVING) {
                printf("Don't create moving button\n");
                exit(1);
            }
            new_btn->k = k;
            new_btn->status = status;
            new_btn->img_enabled = img_enabled;
            new_btn->img_disabled = img_disabled;
            __buttons[i] = new_btn;
            __button_count++;
            return new_btn;
        }
    }
    return 0;
}

void delete_button(HWND hWnd) {
    for (int i = 0; i < N_BUTTONS; i++) {
        if (__buttons[i]->hWnd == hWnd) {
            DestroyWindow(__buttons[i]->hWnd);
            free(__buttons[i]);
            __buttons[i] = 0;
            return;
        }
    }
}

void draw_button(HWND parent, int id, DRAWITEMSTRUCT* dis) {
    struct Button* btn = get_button(id_to_hwnd(parent, id));
    DrawStateW(
        dis->hDC, 0, 0,
        (LPARAM)(btn->status == 0 ? btn->img_disabled : btn->img_enabled),
         0, 0, 0, 0, 0, DST_BITMAP
    );
}

void handle_button_event(HWND hWnd, int id, int event) {

    printf("%d %d %d\n", hWnd, id, event);
    struct Button* btn = get_button(hWnd);
    btn->status = BUTTON_STATUS_DISABLED;
    InvalidateRect(hWnd, 0, 0);
    //move_button(btn, rand_int(0, 400), rand_int(0, 400), 0.5);

}

#endif
#ifndef BUTTON_H
#define BUTTON_H

#include <stdio.h>
#include <windows.h>

#include "sound.h"

#define N_BUTTONS 100




void set_image(HWND btn, HBITMAP bitmap) {
    LRESULT res = SendMessage(
        btn,
        BM_SETIMAGE,
        IMAGE_BITMAP,
        (LPARAM)bitmap
    );
    if (res == (LRESULT)NULL) {
        printf("SendMessage failed\n");
        //exit(1);
    }
}

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

#include "random.h"

void handle_button_event(HWND btn, int id, int event) {
    printf("%d %d %d\n", btn, id, event);
    //move_button(btn, rand_int(0, 400), rand_int(0, 400), 0.5);

}

#define BUTTON_STATUS_DISABLED 0
#define BUTTON_STATUS_ENABLED 1
#define BUTTON_STATUS_MOVING 2

struct Button {
    HWND hWnd;

    int status;

    HBITMAP img_enabled;
    HBITMAP img_disabled;
    int type;
    int id;

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


/*
 * given the hWnd of the button, returns
 * the pointer to the Button struct, or
 * 0 if the struct was not found
 */
struct Button* get_button(HWND hWnd) {
    for (int i = 0; i < N_BUTTONS; i++) {
        if (__buttons[i]->hWnd == hWnd) {
            return __buttons[i];
        }
    }
    return 0;
}

/*
 * returns the pointer to the newly created Button struct,
 * or if the struct cannot be created
 */
struct Button* create_button(
        HWND parent, int status,
        const char* img_enabled,
        const char* img_disabled,
        int id, int x, int y, int wd, int ht) {

    if (__button_count >= N_BUTTONS) {
        return 0;
    }
    for (int i = 0; i < N_TILES; i++) {
        if (__buttons[i] == 0) {
            struct Button* new_button = (struct Button*)malloc(sizeof(struct Button));

            new_button->hWnd = (HWND)CreateWindowEx(0, "button", NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP | WS_CLIPSIBLINGS | BS_OWNERDRAW, x, y, wd, ht, parent, (HMENU)id, NULL, NULL);

            if (new_button->hWnd == NULL) {
                printf("CreateWindow failed\n");
            }
            if (status == BUTTON_STATUS_MOVING) {
                printf("Don't create moving button\n");
                exit(1);
            }
            new_button->status = status;
            __buttons[i] = new_button;
            return new_button;
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

void draw_button(HWND hWnd) {
    struct Button* btn = get_button(hWnd);

    DrawStateW(
        ((DRAWITEMSTRUCT*)hWnd)->hDC, 0, 0,
        (LPARAM)(btn->status == 0 ? btn->img_disabled : btn->img_enabled), 0, 0, 0, 0, 0, DST_BITMAP
    );
}

#endif
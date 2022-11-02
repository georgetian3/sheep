#ifndef BUTTON_H
#define BUTTON_H

#include <stdio.h>
#include <windows.h>

#include "sound.h"

HWND create_button(HWND parent, long long id, int width, int height) {
    // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadimagea
    HWND btn = (HWND)CreateWindowEx(0, "button", NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP | WS_CLIPSIBLINGS | BS_OWNERDRAW, 0, 0, width, height, parent, (HMENU)id, NULL, NULL);
    if (btn == NULL) {
        printf("CreateWindow failed\n");
    }
    return btn;
}


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


void delete_button(HWND btn) {
    DestroyWindow(btn);
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

#endif
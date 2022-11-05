#include <math.h>
#include <stdio.h>
#include <windows.h>

#include "tile.h"

#define FPS 60
#define MSPF 1000.0 / FPS




POINT win_pos(HWND hwnd, POINT* point) {
    RECT rect;
    GetWindowRect(hwnd, &rect);
    MapWindowPoints(HWND_DESKTOP, GetParent(hwnd), (LPPOINT)&point, 1);
    //point->x = rect.left;
    //point->y = rect.top;
}


void __move_button(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    struct Button* btn = get_button(hWnd);
    if (btn == 0) {
        printf("Move button not found\n");
        exit(1);
    }
    if (btn->status != BUTTON_STATUS_ENABLED) {
        printf("Button not supposed to be moving\n");
        exit(1);
    }

    BOOL res = SetWindowPos(hWnd, NULL,
        btn->start_pos.x + btn->dxpf * btn->frame,
        btn->start_pos.y + btn->dypf * btn->frame,
        0, 0, SWP_NOSIZE
    );
    if (res == FALSE) {
        printf("__move_button failed\n");
        exit(1);
    }
    btn->frame++;
    if (btn->frame > btn->frames) {
        btn->status = BUTTON_STATUS_ENABLED;
        KillTimer(btn, idEvent);
    }
    
}

void move_button(HWND hWnd, int x, int y, double time) {
    
    if (time == 0) {
        BOOL res = SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE);
        if (res == FALSE) {
            printf("move_button failed\n");
            exit(1);
        }
        return;
    }

    struct Button* btn = get_button(hWnd);
    if (btn->status == BUTTON_STATUS_MOVING) {
        printf("Already moving\n");
        exit(1);
    }

    btn->status = BUTTON_STATUS_MOVING;

    win_pos(hWnd, &btn->start_pos);
    btn->frame = 1;
    btn->start_time = GetTickCount();
    btn->frames = time * FPS;
    btn->dxpf = 1.0 * (x - btn->start_pos.x) / btn->frames;
    btn->dypf = 1.0 * (y - btn->start_pos.y) / btn->frames;

    int res = SetTimer(hWnd, 0, MSPF, (TIMERPROC)__move_button);
    if (res == 0) {
        printf("SetTimer failed\n");
        exit(1);
    }
    return;
}
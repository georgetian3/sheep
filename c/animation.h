#include <math.h>
#include <stdio.h>
#include <windows.h>

#include "button.h"
#include "utils.h"
#define FPS 120
#define MSPF 1000.0 / FPS
void update();


void __move_button(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    //printf("__Move button\n");
    struct Button* btn = get_button(hWnd);
    if (btn == 0) {
        printf("Move button not found\n");
        exit(1);
    }
    if (!btn->moving) {
        printf("Button not supposed to be moving\n");
        exit(1);
    }

    BOOL res = SetWindowPos(hWnd, HWND_TOP,
        btn->start_pos.x + btn->dxpf * btn->frame,
        btn->start_pos.y + btn->dypf * btn->frame,
        0, 0, SWP_NOSIZE|SWP_NOZORDER
    );
    if (res == FALSE) {
        printf("__move_button failed\n");
        exit(1);
    }
    btn->frame++;
    if (btn->frame > btn->frames) {
        btn->moving = FALSE;
        KillTimer(hWnd, idEvent);
        if (btn->callback!=0) {
            if(btn -> callback == 1){
                 update();
            }
            else if (btn->callback == 2){
                match_slot();
            }
        }
    }
    InvalidateRect(btn->hWnd, 0, 0);
}

void move_button(struct Button* btn, int x, int y, double time) {
    printf("Move button\n");
    if (time == 0) {
        BOOL res = SetWindowPos(btn->hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
        if (res == FALSE) {
            printf("move_button failed\n");
            exit(1);
        }
        if (btn->callback!=0) {
            printf("timer 0 callback");
            if(btn -> callback == 1){
                 update();
            }
            else if (btn->callback == 2){
                match_slot();
            }
        }
        return;
    }

    win_pos(btn->hWnd, &btn->start_pos);
    btn->moving = TRUE;
    btn->frame = 1;
    btn->start_time = GetTickCount();
    btn->frames = time * FPS;
    btn->dxpf = 1.0 * (x - btn->start_pos.x) / btn->frames;
    btn->dypf = 1.0 * (y - btn->start_pos.y) / btn->frames;

    int res = SetTimer(btn->hWnd, 0, MSPF, (TIMERPROC)__move_button);    
    if (res == 0) {
        printf("SetTimer failed\n");
        exit(1);
    }
    return;
}
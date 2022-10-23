#include <math.h>
#include <stdio.h>
#include <windows.h>

#include "tile.h"

#define FPS 60
#define MSPF 1000.0 / FPS

struct MovingTile {
    HWND button;
    int start_x;
    int start_y;
    int start_time;
    double dxpf;
    double dypf;
    int frame;
    int frames;
    BOOL moving;
};



POINT win_pos(HWND hwnd) {
    RECT rect;
    GetWindowRect(hwnd, &rect);
    MapWindowPoints(HWND_DESKTOP, GetParent(hwnd), (LPPOINT)&rect, 2);
    POINT point;
    point.x = rect.left;
    point.y = rect.top;
    return point;
}

struct MovingTile movingTiles[N_TILES];

void __move_button(HWND btn, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    for (int i = 0; i < N_TILES; i++) {
        if (movingTiles[i].button == btn) {
            BOOL res = SetWindowPos(btn, NULL,
                movingTiles[i].start_x + movingTiles[i].dxpf * movingTiles[i].frame,
                movingTiles[i].start_y + movingTiles[i].dypf * movingTiles[i].frame,
                0, 0, SWP_NOSIZE
            );
            if (res == FALSE) {
                printf("__move_button failed\n");
                exit(1);
            }
            movingTiles[i].frame++;
            if (movingTiles[i].frame > movingTiles[i].frames) {
                movingTiles[i].moving = FALSE;
                KillTimer(btn, idEvent);
            }
            return;
        }
    }
    printf("MovingTile not found\n");
    exit(1);
}

void move_button(HWND btn, int x, int y, double time) {
    
    if (time == 0) {
        BOOL res = SetWindowPos(btn, NULL, x, y, 0, 0, SWP_NOSIZE);
        if (res == FALSE) {
            printf("move_button failed\n");
            exit(1);
        }
        return;
    }

    for (int i = 0; i < N_TILES; i++) {
        if (!movingTiles[i].moving) {

            movingTiles[i].moving = TRUE;
            movingTiles[i].button = btn;

            POINT start = win_pos(btn);
            movingTiles[i].start_x = start.x;
            movingTiles[i].start_y = start.y;
            movingTiles[i].frame = 1;
            movingTiles[i].start_time = GetTickCount();
            movingTiles[i].frames = time * FPS;
            movingTiles[i].dxpf = 1.0 * (x - start.x) / movingTiles[i].frames;
            movingTiles[i].dypf = 1.0 * (y - start.y) / movingTiles[i].frames;

            int res = SetTimer(btn, 0, MSPF, (TIMERPROC)__move_button);
            if (res == 0) {
                printf("SetTimer failed\n");
                exit(1);
            }
            return;
        }
    }

    printf("All tiles moving\n");
    exit(1);


}
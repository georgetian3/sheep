#include <windows.h>
#include <stdio.h>



#include <math.h>


#define WINDOW_WIDTH 768
#define WINDOW_HEIGHT 1024


#include "animation.h"
#include "button.h"
#include "tile.h"
#include "sound.h"







void set_tiles() {
    int rows = 3;
    int cols = 3;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            ;
        }
    }
}

HBITMAP bg_bmp, old_bg_bmp;
BITMAP bmp;

HDC hdc, hdcMem;
PAINTSTRUCT ps;
RECT rect;
RECT rc;

void LoadScreen(HWND hWnd) {
    bg_bmp = (HBITMAP)LoadImage(NULL, "../res/bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


    RECT rect;
    HDC hdc = GetDC(hWnd);
    HBRUSH brush = CreatePatternBrush(bg_bmp);
    GetWindowRect(hWnd, &rect);
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);
    ReleaseDC(hWnd, hdc);


    /* GetObject(bg_bmp, sizeof(BITMAP), &bmp);
    hdc = GetDC(hWnd);
    hdcMem = CreateCompatibleDC(hdc);
    old_bg_bmp = SelectObject(hdcMem, bg_bmp);
    ReleaseDC(hWnd, hdc); */

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            LoadScreen(hwnd);
            //printf("%d %d %d\n", bg_bmp, bg, res);
            //PlaySound("../res/welcome.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
            load_tiles();
            // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadimagea
            tile_buttons[0] = create_button(hwnd, 100, TILE_WIDTH, TILE_HEIGHT);
            tile_buttons[1] = create_button(hwnd, 200, TILE_WIDTH, TILE_HEIGHT);
            move_button(tile_buttons[1], 50, 50, 0);
            set_image(tile_buttons[0], tile_bitmaps[0]);
            set_image(tile_buttons[1], tile_bitmaps[1]);
            SoundType s = Welcome;
            play_sound(s, TRUE, TRUE);

            break;
        }
        case WM_COMMAND: {
            // https://learn.microsoft.com/en-us/windows/win32/controls/bn-clicked
            handle_button_event((HWND)lParam, LOWORD(wParam), HIWORD(wParam));
            break;
        }
        case WM_PAINT: {
            //printf("Paint event\n");
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        case WM_ERASEBKGND: {
            printf("erase\n");
            //LoadScreen(hwnd);

        }
        default: {
            //printf("???\n");
        }
    }
  
    return DefWindowProc(hwnd, msg, wParam, lParam);
}






































int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow) {
    srand(time(NULL));
    
    MSG msg;    
    WNDCLASS wc = {0};
    wc.lpszClassName = "Buttons";
    wc.hInstance     = hInstance ;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc   = WndProc ;
    wc.hCursor       = LoadCursor(0, IDC_ARROW);
    RegisterClass(&wc);
    CreateWindow(
        wc.lpszClassName,
        "Sheep",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0,
        0,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0,
        0,
        hInstance,
        0
    );  

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

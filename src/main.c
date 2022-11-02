#include <windows.h>
#include <stdio.h>



#include <math.h>


#define WINDOW_WIDTH 768
#define WINDOW_HEIGHT 1024


#include "animation.h"
#include "button.h"
#include "tile.h"
#include "sound.h"

struct Tile *t1, *t2;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            //printf("%d %d %d\n", bg_bmp, bg, res);
            //PlaySound("../res/welcome.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
            load_tiles();
            // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadimagea
            t1 = create_tile(hWnd, 1, 0, 0, 10, 10, 10);

            //SoundType s = Welcome;
            //play_sound(s, TRUE, TRUE);

            break;
        }
        case WM_COMMAND: {
            // https://learn.microsoft.com/en-us/windows/win32/controls/bn-clicked
            handle_button_event((HWND)lParam, LOWORD(wParam), HIWORD(wParam));
            break;
        }
        case WM_DRAWITEM: {
            printf("drawitem\n");
            DrawStateW(((DRAWITEMSTRUCT*)lParam)->hDC, 0, 0, (LPARAM)tile_bitmaps[0], 0, 0, 0, 0, 0, DST_BITMAP | DSS_DISABLED);
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
            
            //printf("erase\n");
            //LoadScreen(hwnd);

        }
        default: {
            //printf("???\n");
        }
    }
  
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

































const char titleStr[] = "Sheep";




int WINAPI WinMain(HINSTANCE hInst,
                   HINSTANCE hPrevInst,
                   LPSTR pCmdLine,
                   int nCmdShow) {
    srand(time(NULL));
    
    MSG msg;    
    WNDCLASS wc = {0};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = titleStr;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.hCursor       = LoadCursor(0, IDC_ARROW);
    RegisterClass(&wc);
    CreateWindow(
        titleStr,
        titleStr,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0,
        0,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0,
        0,
        hInst,
        0
    );  

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

#include <windows.h>
#include <stdio.h>



#include <math.h>


#define WINDOW_WIDTH 768
#define WINDOW_HEIGHT 1024


#include "animation.h"
#include "button.h"
#include "tile.h"
#include "sound.h"

void handle_button_click(HWND parent, struct Button* btn) {
    printf("handle_button_click, ptr %u hwnd %u type %u\n", btn, btn->hWnd, btn->type);
    //move_button(hWnd, 400, 400, 2);


    InvalidateRect(btn->hWnd, 0, 0);
}

struct Button *b1, *b2;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_DRAWITEM: {
            //printf("WM_DRAWITEM\n");
            draw_button(hWnd, wParam, (DRAWITEMSTRUCT*)lParam);
        }
        case WM_COMMAND: {
            //printf("WM_COMMAND\n");
            // https://learn.microsoft.com/en-us/windows/win32/controls/bn-clicked
            struct Button* btn = get_button((HWND)lParam);
            if (btn && btn->state == STATE_ENABLED) {
                handle_button_click(hWnd, btn);
            }
            break;
        }
        case WM_CREATE: {
            load_bitmaps();

            for (int i = 0; i < 10; i++) {
                create_button(
                    hWnd,
                    STATE_ENABLED,
                    i % N_TILE_TYPES,
                    i, 32 * i, 32 * i, 1, TILE_WIDTH, TILE_HEIGHT
                );
            }
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default: {
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
    }
  
    return 0;
}

































const char titleStr[] = "Sheep";
int WINAPI WinMain(HINSTANCE hInst,
                   HINSTANCE hPrevInst,
                   LPSTR pCmdLine,
                   int nCmdShow) {
    //srand(time(NULL));
    
    MSG msg;    
    WNDCLASS wc = {0};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = titleStr;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.hCursor       = LoadCursor(0, IDC_ARROW);
    RegisterClass(&wc);
    CreateWindow(titleStr, titleStr, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInst, 0
    );  

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
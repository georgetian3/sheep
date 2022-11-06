#pragma warning(disable:4996)

#include "animation.h"
#include "button.h"
#include "utils.h"
#include <stdio.h>

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <mmsystem.h>

#include <math.h>



void update() {
    struct Button *a, *b;
    for (int i = 0; i < N_BUTTONS; i++) {
        b = get_button_index(i);
        if (!b || !is_tile(b) || b->moving) {
            continue;
        }
        for (int j = 0; j < N_BUTTONS; j++) {
            a = get_button_index(j);
            if (a == b || !a || !is_tile(b) || a->moving) {
                continue;
            }
            if (b->in_slot) {
                set_active(b, FALSE);
                b->gray = FALSE;
            } else if (overlap(a, b)) {
                set_active(b, FALSE);
                b->gray = TRUE;
            }
        }
    }
}






HBITMAP bmp_src;



void update_slot(struct Button* btn) {
    printf("Update slot\n");
    int insert_index = 0; // index of to insert btn
    int count = 1; // number of tiles of the same type

    // finds the correct values for the above 2 variables
    while (insert_index < slot_count) {
        printf("insert index %d\n", insert_index);
        if (slot[insert_index]->type == btn->type) {
            count++;
        } else if (count > 1) {
            break;
        }
        insert_index++;
    }
    printf("begin insert_slot\n");
    insert_slot(btn, insert_index);
    printf("finished insert_slot\n");
    printf("finished update slot\n");
}


void handle_button_click(HWND parent, struct Button* btn) {
    printf("handle_button_click, ptr %u hwnd %u\n", btn, btn->hWnd);
    if (is_tile(btn)) {
        if (btn->in_slot) {
            return;
        }
        
        update_slot(btn);
        update();
        if (slot_count >= SLOT_SIZE) {
            lose();
        }
    }


    InvalidateRect(btn->hWnd, 0, 0);
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_DRAWITEM: {
            draw_button(hWnd, (int)wParam, (DRAWITEMSTRUCT*)lParam);
        }
        case WM_COMMAND: {
            struct Button* btn = get_button((HWND)lParam);
            if (btn && btn->active) {
                handle_button_click(hWnd, btn);
            }
            break;
        }
        case WM_CREATE: {

            printf("WM_CREATE\n");
            bmp_src = load_bitmap("../res/bg.bmp", 0, 0);
            load_bitmaps();

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    create_button(hWnd, STATE_ENABLED, (i * 3 + j) % N_TILE_TYPES, 100 * (i + 1), 100 * (j + 1), 1, TILE_WIDTH, TILE_HEIGHT);
                }
            }
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    create_button(hWnd, STATE_ENABLED, (i * 3 + j) % N_TILE_TYPES, 100 * (i + 1) + 10, 100 * (j + 1) + 10, 2, TILE_WIDTH, TILE_HEIGHT);
                }
            }
            update();
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc_window = BeginPaint(hWnd, &ps);
            HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
            HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);
            HBITMAP    blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
            SelectObject(hdc_memBuffer, blankBmp);
            SelectObject(hdc_loadBmp, bmp_src);
            BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);
            BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);
            DeleteObject(blankBmp);
            DeleteDC(hdc_memBuffer);
            DeleteDC(hdc_loadBmp);
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        case WM_TIMER: {
            printf("WM_TIMER\n");
            break;
        }

    }
  
    return DefWindowProc(hWnd, msg, wParam, lParam);
}



const char titleStr[] = "Sheep";
int WINAPI WinMain(
        HINSTANCE hInst,
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
    CreateWindow(titleStr, titleStr, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInst, 0
    );  
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
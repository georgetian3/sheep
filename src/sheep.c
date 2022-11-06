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




void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam);
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);
//void UpdateUnits(HWND hWnd);
void Paint(HWND hWnd);




struct Stage* currentStage = NULL; //当前场景状态


void update() {
    struct Button *a, *b;
    for (int i = 0; i < N_TILES; i++) {
        for (int j = 0; j < N_TILES; j++) {
            a = get_button_index(j);
            b = get_button_index(i);
            if (a && b && (b->in_slot || overlap(a, b))) {
                set_state(b, STATE_DISABLED);
            }
        }
    }
}






HBITMAP bmp_Background, bmp_src;
// 初始化背景函数
HBITMAP InitBackGround(HWND hWnd) {

    bmp_src = load_bitmap("../res/bg.bmp", 0, 0);

    PAINTSTRUCT ps;
    HDC hdc_window = BeginPaint(hWnd, &ps);

    HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
    HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

    //初始化缓存
    HBITMAP    bmp_output = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
    SelectObject(hdc_memBuffer, bmp_output);

    //加载资源
    SelectObject(hdc_loadBmp, bmp_src);
    TransparentBlt(
        hdc_memBuffer,0, 0,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        hdc_loadBmp, 0,0,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        RGB(255, 255, 255));
    // 最后将所有的信息绘制到屏幕上
    BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

    // 回收资源所占的内存（非常重要）
    DeleteDC(hdc_memBuffer);
    DeleteDC(hdc_loadBmp);

    // 结束绘制
    EndPaint(hWnd, &ps);

    return bmp_output;
}





void update_slot(struct Button* btn) {
    int insert_index; // index of to insert btn
    int count = 1; // number of tiles of the same type

    // finds the correct values for the above 2 variables
    for (insert_index = 0; insert_index < slot_count; insert_index++) {
        if (slot[insert_index]->type == btn->type) {
            count++;
        } else if (count > 1) {
            break;
        }
    }
    
    insert_slot(btn, insert_index);

    if (count == MATCH_COUNT) {
        match_slot(insert_index);
    }

}



// 初始化游戏场景函数
void InitStage(HWND hWnd, int stageID,LPARAM lParam) {
    int item[NUM] = {0};
    for (int i = 0; i < NUM;) {
        for (int j = 0; j < NUM_CORN*3; j++, i++) {
            item[i] = 0;
        }
        for (int j = 0; j < NUM_GRASS*3; j++, i++) {
            item[i] = 1;
        }
        for (int j = 0; j < NUM_CARROT*3; j++, i++) {
            item[i] = 2;
        }
    }
    shuffle(item, NUM);
    //memset(map, -1, sizeof(map));

}
        


void Paint(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc_window = BeginPaint(hWnd, &ps);

    HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
    HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

    //初始化缓存
    HBITMAP    blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
    SelectObject(hdc_memBuffer, blankBmp);

    // 绘制背景到缓存
    SelectObject(hdc_loadBmp, bmp_src);

    BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);



    // 最后将所有的信息绘制到屏幕上
    BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

    // 回收资源所占的内存（非常重要）
    DeleteObject(blankBmp);
    DeleteDC(hdc_memBuffer);
    DeleteDC(hdc_loadBmp);

    // 结束绘制
    EndPaint(hWnd, &ps);
}


void handle_button_click(HWND parent, struct Button* btn) {
    printf("handle_button_click, ptr %u hwnd %u\n", btn, btn->hWnd);
    if (is_tile(btn)) {
        if (btn->in_slot) {
            return;
        }
        update_slot(btn);
        update();
    }


    InvalidateRect(btn->hWnd, 0, 0);
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_DRAWITEM: {
            printf("WM_DRAWITEM\n");
            draw_button(hWnd, (int)wParam, (DRAWITEMSTRUCT*)lParam);
        }
        case WM_COMMAND: {
            printf("WM_COMMAND\n");
            struct Button* btn = get_button((HWND)lParam);
            if (btn && btn->state == STATE_ENABLED) {
                handle_button_click(hWnd, btn);
            }
            break;
        }
        case WM_CREATE: {

            printf("WM_CREATE\n");
            bmp_Background = InitBackGround(hWnd);
            printf("Done InitBackGround\n");
            //初始化开始场景
            //InitStage(hWnd, STAGE_STARTMENU,lParam);
            printf("Done InitStage\n");
            //初始化主计时器
            //SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
            load_bitmaps();
            create_button(hWnd, STATE_ENABLED, TYPE_GRASS, 100, 100, 1, TILE_WIDTH, TILE_HEIGHT);
            break;
        }
        case WM_PAINT: {
            printf("WM_PAINT\n");
            Paint(hWnd);
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
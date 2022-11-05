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






HBITMAP bmp_Background;
// 初始化背景函数
HBITMAP InitBackGround(HWND hWnd) {

    HBITMAP bmp_src = load_bitmap("../res/bg.bmp", 0, 0);

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




void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    //UpdateUnits(hWnd);
    InvalidateRect(hWnd, NULL, FALSE);
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
    int flag = 0;
    for (int i = 0; i < MAP_WIDTH; i+=3) {
        for (int j = 0; j < MAP_LENGTH; j+=3) {
            for (int k = 0; k < MAP_HEIGHT; k++) {
                //map[i][j][k] = item[flag];
                flag++;
            }
        }
    }
    if (currentStage != NULL) free(currentStage);
    currentStage = (struct Stage*)malloc(sizeof(struct Stage));
    currentStage->stageID = stageID;
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_LENGTH; j++) {
            for (int k = 0; k < MAP_HEIGHT; k++) { 
                //HWND btn = createButton(hWnd, lParam, 100 * k + i  * 10 + j, 10 * k + UNIT_SIZE_X/2 + i * UNIT_SIZE_X/2, 10 * k + UNIT_SIZE_Y/2 + j * UNIT_SIZE_Y/2, map[i][j][k]);
                //button_type[i][j][k] = map[i][j][k];
                //all_button[i][j][k] = btn;
            }
            
        }
    }
    update(hWnd);
    //createButton(hWnd, lParam, 0, 64, 64);

    if (stageID == STAGE_STARTMENU) {
        currentStage->bg = bmp_Background;
        currentStage->timeCountDown = 0;
        currentStage->timerOn = FALSE;

        //显示开始界面的按钮
        /* for (int i = 0; i < buttons.size(); i++) {
            struct Button* button = buttons[i];
            if (button->buttonID == BUTTON_STARTGAME) {
                button->visible = TRUE;
            }
            else {
                button->visible = FALSE;
            }
        } */


    }
    else if (stageID >= STAGE_1 && stageID <= STAGE_1)
    {
        currentStage->bg = bmp_Background;
        currentStage->timeCountDown = 10000;
        currentStage->timerOn = TRUE;

        //显示游戏界面的按钮
        /* for (int i = 0; i < buttons.size(); i++) {
            struct Button* button = buttons[i];
            if (false) 
            {
                button->visible = true;
            }
            else {
                button->visible = false;
            }
        } */

        // 按场景初始化单位
        switch (stageID) {
            case STAGE_1:
                break;
            default:
                break;
        }


    }

    //刷新显示
    InvalidateRect(hWnd, NULL, FALSE);
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
    SelectObject(hdc_loadBmp, currentStage->bg);

    BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);

    // 按场景分类绘制内容到缓存
    if (currentStage->stageID == STAGE_STARTMENU) {

    }
    else if (currentStage->stageID >= STAGE_1 && currentStage->stageID <= STAGE_1) 
    {


    }

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
            draw_button(hWnd, (int)wParam, (DRAWITEMSTRUCT*)lParam);
        }
        case WM_COMMAND: {
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
            InitStage(hWnd, STAGE_STARTMENU,lParam);
            printf("Done InitStage\n");
            //初始化主计时器
            SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
            load_bitmaps();
            break;
        }
        case WM_PAINT: {
            Paint(hWnd);
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        case WM_TIMER: {
            if (currentStage != NULL && currentStage->timerOn) TimerUpdate(hWnd, wParam, lParam);
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
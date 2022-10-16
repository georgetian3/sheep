#include <windows.h>
#include <stdio.h>

#include <time.h>
#include <stdlib.h>


#define N_TILE_TYPES 3
#define N_TILES 9

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#define FPS 60


HWND tile_buttons[N_TILES];
HWND create_button(HWND hwnd, long long id) {
    // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadimagea
    HWND btn = (HWND)CreateWindow("button", NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP | WS_CLIPSIBLINGS, 0, 0, TILE_WIDTH, TILE_HEIGHT, hwnd, (HMENU)id, NULL, NULL);
    if (btn == NULL) {
        printf("CreateWindow failed\n");
    }

}


HBITMAP tile_bitmaps[N_TILE_TYPES];
void load_tiles() {
    const char* filenames[] = {
        "../res/1.bmp",
        "../res/2.bmp",
        "../res/3.bmp",
    };
    for (int i = 0; i < N_TILE_TYPES; i++) {
        tile_bitmaps[i] = (HBITMAP)LoadImage(NULL, filenames[i], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (tile_bitmaps[i] == NULL) {
            printf("LoadImage failed: %d", i);
            exit(1);
        }
    }
}

POINT win_pos(HWND hwnd) {
    RECT rect;
    GetWindowRect(hwnd, &rect);
    MapWindowPoints(HWND_DESKTOP, GetParent(hwnd), (LPPOINT)&rect, 2);
    POINT point;
    point.x = rect.left;
    point.y = rect.top;
    return point;
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

void move_button(HWND btn, int x, int y) {
    BOOL res = SetWindowPos(btn, NULL, x, y, 0, 0, SWP_NOSIZE);
    if (res == FALSE) {
        printf("move_button failed\n");
        exit(1);
    }
}

void animate_button(HWND btn, int x, int y, double speed) {
    ;
}

int rand_int(int min, int max) {
    return rand() % (max - min) + min;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            load_tiles();
            // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadimagea
            tile_buttons[0] = create_button(hwnd, 100);
            tile_buttons[1] = create_button(hwnd, 200);
            set_image(tile_buttons[0], tile_bitmaps[0]);
            move_button(tile_buttons[1], 10, 10);
            break;
        }
        case WM_COMMAND: {
            // https://learn.microsoft.com/en-us/windows/win32/controls/bn-clicked
            printf("%d\n", LOWORD(wParam));
            move_button(tile_buttons[1], rand_int(50, 200), rand_int(50, 200));
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
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
    wc.lpszClassName = TEXT( "Buttons" );
    wc.hInstance     = hInstance ;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc   = WndProc ;
    wc.hCursor       = LoadCursor(0, IDC_ARROW);
    RegisterClass(&wc);
    CreateWindow(wc.lpszClassName, TEXT("Sheep"),
                 WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                 150, 150, 230, 150, 0, 0, hInstance, 0);  

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

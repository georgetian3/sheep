#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#include <time.h>
#include <stdlib.h>

#include <math.h>

#define N_TILE_TYPES 3
#define N_TILES 9

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#define WINDOW_WIDTH 768
#define WINDOW_HEIGHT 1024

#define FPS 30

// 初始化背景函数
HBITMAP InitBackground(HWND hWnd, HBITMAP bmp_src) {

	srand(time(NULL));
	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//初始化缓存
	HBITMAP	bmp_output = CreateCompatibleBitmap(hdc_window, 768, 1024);
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
        "../res/carrot.bmp",
        "../res/corn.bmp",
        "../res/grass.bmp",
    };
    for (int i = 0; i < N_TILE_TYPES; i++) {
        tile_bitmaps[i] = (HBITMAP)LoadImage(NULL, filenames[i], IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
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
    HBITMAP bm = (HBITMAP)SendMessage(
        btn,
        BM_GETIMAGE,
        IMAGE_BITMAP,
        0
    );
    printf("setimage hbitmap %d %d\n", bitmap, bm);
    if (res == (LRESULT)NULL) {
        printf("SendMessage failed\n");
        //exit(1);
    }
}

struct MoveArgs {
    HWND btn;
    int x;
    int y;
    double speed;
    BOOL threaded;
};

void move_button(HWND btn, int x, int y, double speed, BOOL threaded) {
    if (speed == 0) {
        //printf("SetWindowPos %d %d\n", x, y);
        BOOL res = SetWindowPos(btn, NULL, x, y, 0, 0, SWP_NOSIZE);
        if (res == FALSE) {
            printf("move_button failed\n");
            exit(1);
        }
        return;
    }
    if (threaded) {
        POINT start = win_pos(btn);
        double dx = x - start.x;
        double dy = y - start.y;
        double dist = sqrt(pow(dx, 2) + pow(dy, 2));
        double frames = dist / speed * FPS;
        double dxpf = dx / frames;
        double dypf = dy / frames;
        //printf("x y dist dx dy dxpf dypf start.x start.y frames %f %f %f %f %f %f %f %f %f %f\n", x, y, dist, dx, dy, dxpf, dypf, start.x, start.y, frames);
        for (int i = 0; i < frames; i++) {
            //printf("x y dist dx dy dxpf dypf start.x start.y frames %f %f %f %f %f %f %f %f %f %f\n", x, y, dist, dx, dy, dxpf, dypf, start.x, start.y, frames);
            move_button(btn, start.x + i * dxpf, start.y + i * dypf, 0, FALSE);
            RedrawWindow(btn ,NULL,NULL,RDW_INVALIDATE | RDW_INTERNALPAINT);
        }
        move_button(btn, x, y, 0, FALSE);
        return;
    }

    // https://learn.microsoft.com/en-us/windows/win32/procthread/creating-threads
    // https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
    struct MoveArgs* moveArgs = (struct MoveArgs*)malloc(sizeof(struct MoveArgs));

    moveArgs->btn = btn;
    moveArgs->x = x;
    moveArgs->y = y;
    moveArgs->speed = speed;
    moveArgs->threaded = TRUE;

    /* HANDLE thread = CreateThread(
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)move_button,
        moveArgs,
        0,
        NULL
    ); */



    //free(moveArgs);

    move_button(btn, x, y, speed, TRUE);


}

void show_button(HWND btn, BOOL visible) {
    ShowWindow(btn, visible ? SW_SHOW : SW_HIDE);
}


void delete_button(HWND btn) {
    DestroyWindow(btn);
}

void enable_button(HWND btn, BOOL enable) {
    EnableWindow(btn, enable);
}

int rand_int(int min, int max) {
    return rand() % (max - min) + min;
}

int tile_type(HWND btn) {
    // https://learn.microsoft.com/en-us/windows/win32/controls/bm-getimage
    HBITMAP bm = (HBITMAP)SendMessage(
        btn,
        BM_GETIMAGE,
        IMAGE_BITMAP,
        0
    );
    printf("hbitmap %d\n", bm);
    for (int i = 0; i < N_TILE_TYPES; i++) {
        if (bm == tile_bitmaps[i]) {
            return i;
        }
    }
    return -1;
}


HWND id_to_hwnd(HWND parent, int id) {
    //https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getdlgitem
    return GetDlgItem(parent, id);
}

void set_tiles() {
    int rows = 3;
    int cols = 3;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            ;
        }
    }
}

void welcome() {
    
}

void game() {

    ;
}

void won() {

}

void lost() {
    won();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            PlaySound("../res/welcome.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
            load_tiles();
            HBITMAP bg = (HBITMAP)LoadImage(NULL, "../res/bg.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
            //InitBackground(hwnd, bg);
            // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadimagea
            tile_buttons[0] = create_button(hwnd, 100);
            set_image(tile_buttons[0], tile_bitmaps[1]);

            printf("i: %d\n", tile_type(tile_buttons[0]));
            break;
        }
        case WM_COMMAND: {
            // https://learn.microsoft.com/en-us/windows/win32/controls/bn-clicked
            PlaySound("../res/welcome.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
            move_button(tile_buttons[0], 200, 200, 0.5, FALSE);
            break;
        }
        case WM_PAINT: {
            printf("Paint event\n");
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
                 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInstance, 0);  

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

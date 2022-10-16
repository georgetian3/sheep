#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow) {
    MSG msg;    
    WNDCLASS wc = {0};
    wc.lpszClassName = TEXT( "Buttons" );
    wc.hInstance     = hInstance ;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc   = WndProc ;
    wc.hCursor       = LoadCursor(0, IDC_ARROW);
    RegisterClass(&wc);
    CreateWindow(wc.lpszClassName, TEXT("Buttons"),
                 WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                 150, 150, 230, 150, 0, 0, hInstance, 0);  

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int) msg.wParam;
}


HWND createButton(HWND hwnd, const char* bitmap, int id, int x, int y) {
    HBITMAP bm = (HBITMAP)LoadImage(0, bitmap, IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    if (bm  == NULL) {
        printf("LoadImage failed\n");
    }

    HWND btn = (HWND)CreateWindow(TEXT("button"), 0, WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT, x, y, 64, 64, hwnd, (HMENU)id, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
    
    if (btn == NULL) {
        printf("CreateWindow failed\n");
        return NULL;
    }

    LRESULT res = SendMessage(
        btn,
        (UINT)BM_SETIMAGE,
        (WPARAM)IMAGE_BITMAP,
        (LPARAM)bm
    );
    if (res == (LRESULT)NULL) {
        printf("SendMessage failed\n");
        return NULL;
    }

    return btn;
}

LRESULT enableButton(HWND btn, BOOL enable) {
    return EnableWindow(btn, enable);
}

HWND btn1, btn2;
int enabled = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    switch(msg) {
        case WM_CREATE: {
            // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadimagea
            btn1 = createButton(hwnd, "../res/1.bmp", 1, 10, 10);
            btn2 = createButton(hwnd, "../res/2.bmp", 2, 20, 20);
            
            break;
        }
        case WM_COMMAND: {
            // https://learn.microsoft.com/en-us/windows/win32/controls/bn-clicked
            //printf("%d\n", LOWORD(wParam));
            LRESULT res = enableButton(btn1, FALSE);
            EnableWindow(btn2, TRUE);
            UpdateWindow(btn2);
            printf("%d\n", res);
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        case WM_ENABLE: {
            printf("???\n");
        }
        default: {
            //printf("???\n");
        }
    }
  
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
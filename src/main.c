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


HWND createButton(HWND hwnd, const char* bitmap, HMENU id, int x, int y) {
    // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadimagea
    HBITMAP bm = (HBITMAP)LoadImage(NULL, bitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (bm == NULL) {
        printf("LoadImage failed\n");
    } else {
        printf("LoadImage succeeded: %u\n", bm);
    }


    HWND btn = (HWND)CreateWindow(TEXT("button"), 0, WS_VISIBLE | WS_CHILD | BS_BITMAP, x, y, 64, 64, hwnd, id, NULL, NULL);
    
    if (btn == NULL) {
        printf("CreateWindow failed\n");
        return NULL;
    } else {
        printf("CreateWindow succeeded: %u\n", btn);
    }

    LRESULT res = SendMessage(
        btn,
        BM_SETIMAGE,
        IMAGE_BITMAP,
        bm
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
            EnableWindow(btn2, FALSE);
            UpdateWindow(btn2);
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

update PROC USES eax ebx ecx edx esi edi
    LOCAL a:DWORD, b:DWORD

    ret
update ENDP





COMMENT `
    for (int i = 0; i < N_BUTTONS; i++) {
        b = get_button_index(i);
        if (!b || !is_tile(b) || b->moving) {
            continue;
        }
        int flag = TRUE;
        for (int j = 0; j < N_BUTTONS; j++) { 
            a = get_button_index(j);
            if (a == b || !a || !is_tile(b) || a->moving) {
                continue;
            }
            if (b->in_slot) {
                set_active(b, FALSE);
                b->gray = FALSE;
                flag=FALSE;
            } else if (overlap(a, b)) {
                set_active(b, FALSE);
                b->gray = TRUE;
                flag=FALSE;
            }
        }
        if(flag==TRUE){
            set_active(b, TRUE);
            b->gray = FALSE;
            InvalidateRect(b->hWnd,0,0);
        }
    }
}






HBITMAP bmp_src;



void update_slot(struct Button* btn) {
    //printf("Update slot\n");
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
    //printf("finished insert_slot\n");
    //printf("finished update slot\n");
}


void handle_button_click(HWND parent, struct Button* btn) {
    //printf("handle_button_click, ptr %u hwnd %u\n", btn, btn->hWnd);
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
            //printf("WM_DRAWITEM\n");
            draw_button(hWnd, (int)wParam, (DRAWITEMSTRUCT*)lParam);
        }
        case WM_COMMAND: {
            //printf("WM_COMMAND\n");
            struct Button* btn = get_button((HWND)lParam);
            if (btn && btn->active) {
                handle_button_click(hWnd, btn);
            }
            break;
        }
        case WM_CREATE: {

            //printf("WM_CREATE\n");
            bmp_src = load_bitmap("../res/bg.bmp");
            load_bitmaps();
            build_map(hWnd,"map2.txt");
            update();
            break;
        }
        case WM_PAINT: {
            //printf("WM_PAINT\n");
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
            //printf("WM_TIMER\n");
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

`
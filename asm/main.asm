.686
.model flat, stdcall  ; 32 bit memory model
option casemap :none  ; case sensitive

include includes.inc
include macros.inc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.DATA

include data.inc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.CODE

;p:1D Array i:index return :p[i]
AA1 PROC USES ebx ecx edx esi edi p:DWORD, i:DWORD
    COMMENT !
        p: pointer to 1D array
        i: array index
        returns in eax the pointer
        to the corresponding element
    !

        mov     eax, p
        mov     ebx, i
        lea     eax, [eax + ebx * SIZEOF DWORD]
        String  tt, "p %d i %d res %d", 10, 13
        ;Print   OFFSET tt, p, i, eax
        ret
AA1 ENDP

;p:2D Array i:index j:index return:p[i][j]
AA2 PROC USES ebx ecx edx esi edi p:DWORD, i:DWORD, j:DWORD
    COMMENT !
        p: pointer to 2D array
        i, j: array indexes
        returns in eax the pointer
        to the corresponding element
    !
    
        mov     eax, p
        mov     ebx, i
        mov     ecx, j
        lea     eax, [eax + ebx * SIZEOF DWORD]
        mov     eax, [eax]
        lea     eax, [eax + ecx * SIZEOF DWORD]
        String  ttt, "p %d i %d j %d res %d", 10, 13
        ;Print   OFFSET ttt, p, i, j, eax

        ret
AA2 ENDP



String  here, "HERE", 10, 13
String  done, "DONE", 10, 13
String  hexStr, "%x"
String  decStr, "%d"
String  charStr, "%s", 0ah, 0dh
String  newline, 10, 13
String  parStr, "Parent: %x", 10, 13

include code.inc


String wm_create, "WM_CREATE", 10, 13
String wm_command, "WM_COMMAND", 10, 13
String wm_drawitem, "WM_DRAWITEM", 10, 13
String clicked, "Button clicked", 10, 13

test_time   REAL8 0.2

;MessageLoop
WndProc PROC hWnd:DWORD, uMsg:DWORD, wParam:DWORD, lParam:DWORD

        ;Print   OFFSET parStr, hWnd


        mov ebx, uMsg

        .IF ebx == WM_DRAWITEM
                PINVOKE draw_button, hWnd, wParam, lParam
        .ELSEIF ebx == WM_COMMAND
                ; lParam: hWnd of button
                ; LOWORD(wParam): id
                ; HIWORD(wParam): event
                
                Print   OFFSET clicked
                INVOKE  get_button, lParam
                String  gbr, "get_button, %x", 10, 13
                Print   OFFSET gbr, eax
                mov     esi, eax
                AINVOKE is_button, esi
                .IF eax != 0
                        mov     eax, (Button PTR [esi]).active
                        .IF eax != 0
                                String  hbcb, "handle_button_click hWnd %x esi %x", 10, 13
                                Print   OFFSET hbcb, hWnd, esi
                                Print   OFFSET newline
                                PINVOKE handle_button_click, hWnd, esi
                        .ENDIF
                .ENDIF
                PINVOKE print_buttons
        .ELSEIF ebx == WM_CREATE

                AINVOKE LoadImageA, 0, OFFSET icon, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE or LR_LOADFROMFILE
                mov     ecx, eax
                PINVOKE SendMessage, hWnd, WM_SETICON, ICON_SMALL, ecx;
                PINVOKE SendMessage, hWnd, WM_SETICON, ICON_BIG, ecx;
                AINVOKE GetWindow, hWnd, GW_OWNER
                PINVOKE SendMessage, eax, WM_SETICON, ICON_SMALL, ecx;
                PINVOKE SendMessage, eax, WM_SETICON, ICON_BIG, ecx;

                AINVOKE LoadImageA, 0, OFFSET icon, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE or LR_LOADFROMFILE
                mov     ecx, eax
                PINVOKE SendMessage, hWnd, WM_SETICON, ICON_SMALL, ecx;
                PINVOKE SendMessage, hWnd, WM_SETICON, ICON_BIG, ecx;
                AINVOKE GetWindow, hWnd, GW_OWNER
                PINVOKE SendMessage, eax, WM_SETICON, ICON_SMALL, ecx;
                PINVOKE SendMessage, eax, WM_SETICON, ICON_BIG, ecx;



                mov     ecx, OFFSET main_bg
                AINVOKE load_bitmap, ecx
                mov     bmp_bg, eax
                PINVOKE  load_bitmaps
                ;INVOKE  build_map, hWnd, OFFSET MAP1
                ;mov     total, eax
                PINVOKE create_button, hWnd, TYPE_START, 300, 320, -1, 200, 80
                PINVOKE create_button, hWnd, TYPE_END, 300, 400, -1, 200, 56

                INVOKE  play_sound, 0, 0, 0
                ;PINVOKE update
                PINVOKE print_buttons
        .ELSEIF ebx == WM_PAINT
                INVOKE paint, hWnd
        .ELSEIF ebx == WM_CLOSE
                INVOKE  PostQuitMessage, 0
        .ELSE
                INVOKE  DefWindowProc, hWnd, uMsg, wParam, lParam
        .ENDIF
    ret
WndProc ENDP


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;MainWindow
WinMain PROC hInst:HINSTANCE, hPrevInst:HINSTANCE, pCmdLine:LPSTR, nCmdShow:DWORD
        LOCAL wc:WNDCLASS, msg:MSGStruct, winRect:RECT, hMainWnd:DWORD

        finit

        INVOKE  crt_time, 0
        INVOKE  crt_srand, eax

        String  WindowName, "Sheep"
        mov     eax, hInst
        mov     wc.hInstance, eax
        INVOKE  LoadCursor, NULL, IDC_ARROW
        mov     wc.hCursor, eax
        mov     wc.lpszClassName, OFFSET WindowName
        mov     wc.lpfnWndProc, OFFSET WndProc

        INVOKE  RegisterClass, ADDR wc
        .IF eax == 0
                jmp Exit
        .ENDIF
        
        INVOKE  CreateWindowEx, 0, ADDR WindowName, ADDR WindowName,
                WS_OVERLAPPED or WS_CAPTION or WS_SYSMENU or WS_MINIMIZEBOX,
                0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInst, 0
        mov     hMainWnd,eax
        .IF eax == 0
                jmp Exit
        .ENDIF

        INVOKE  ShowWindow, hMainWnd, SW_SHOW
        INVOKE  UpdateWindow, hMainWnd

    MsgLoop:
        INVOKE  GetMessage, ADDR msg, 0, 0, 0
        .IF eax == 0
                jmp Exit
        .ENDIF
        INVOKE  DispatchMessage, ADDR msg
        jmp     MsgLoop

    Exit:
        INVOKE  ExitProcess, eax

WinMain ENDP
END WinMain
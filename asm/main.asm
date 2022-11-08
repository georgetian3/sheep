.686
.model flat, stdcall  ; 32 bit memory model
option casemap :none  ; case sensitive

include includes.inc
include macros.inc

;#############################################################
.DATA

include data.inc

msg         MSGStruct <>
winRect     RECT <>
hMainWnd    DWORD ?

emptyStr    byte    0



; Define the Application's Window class structure.
;MainWin WNDCLASS <NULL,WinProc,NULL,NULL,NULL,NULL,NULL,COLOR_WINDOW,NULL,WindowName>




;#############################################################
.CODE





String  here, "HERE", 10, 13
String  done, "DONE", 10, 13
String  hexStr, "%x"
String  decStr, "%d"
String  newline, 10, 13


include code.inc



WndProc PROC hWnd:DWORD, uMsg:DWORD, wParam:DWORD, lParam:DWORD

    ;LOCAL point:POINT
        String wm_create, "WM_CREATE", 10, 13
        String wm_command, "WM_COMMAND", 10, 13
        String wm_drawitem, "WM_DRAWITEM", 10, 13
        mov ebx, uMsg

        .IF ebx == WM_DRAWITEM
            Print   OFFSET wm_drawitem
            INVOKE  draw_button, hWnd, wParam, lParam
        .ELSEIF ebx == WM_COMMAND
            Print   OFFSET wm_command

            ; lParam: hWnd of button
            ; LOWORD(wParam): id
            ; HIWORD(wParam): event
            
            ;String  clicked, "Button clicked", 10, 13
            ;Print   OFFSET clicked
            ;INVOKE  WinPos, lParam, ADDR point

            ;String  intStr, "%d", 10, 13
            ;Print   OFFSET intStr, point.x
            ;Print   OFFSET intStr, point.y
        .ELSEIF ebx == WM_CREATE
            Print   OFFSET wm_create
            Print   OFFSET wm_command
            INVOKE  load_bitmaps
            Print   OFFSET here
            INVOKE  create_button, hWnd, 9, 100, 100, 1, TILE_WIDTH, TILE_HEIGHT
            Print   OFFSET wm_command
            INVOKE  play_sound, 0, 0, 0
        .ELSEIF ebx == WM_CLOSE
            INVOKE  PostQuitMessage, 0
        .ELSE
            INVOKE  DefWindowProc, hWnd, uMsg, wParam, lParam
        .ENDIF
    ret
WndProc ENDP

WinMain PROC hInst:HINSTANCE, hPrevInst:HINSTANCE, pCmdLine:LPSTR, nCmdShow:DWORD
        LOCAL wc:WNDCLASS

        finit

        String  WindowName, "Sheep"
        INVOKE  GetModuleHandle, NULL
        mov     eax, hInst
        mov     wc.hInstance, eax
        INVOKE  LoadCursor, NULL, IDC_ARROW
        mov     wc.hCursor, eax
        mov     wc.lpszClassName, OFFSET WindowName
        mov     wc.lpfnWndProc, OFFSET WndProc


        INVOKE RegisterClass, ADDR wc
        .IF eax == 0
            jmp Exit_Program
        .ENDIF
        
        INVOKE CreateWindowEx, 0, ADDR WindowName, ADDR WindowName,
            WS_OVERLAPPED or WS_CAPTION or WS_SYSMENU or WS_MINIMIZEBOX, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInst, 0
        mov hMainWnd,eax

        .IF eax == 0
        jmp  Exit_Program
        .ENDIF

        INVOKE ShowWindow, hMainWnd, SW_SHOW
        INVOKE UpdateWindow, hMainWnd

    Message_Loop:
        INVOKE GetMessage, ADDR msg, NULL,NULL,NULL
        .IF eax == 0
        jmp Exit_Program
        .ENDIF

        INVOKE DispatchMessage, ADDR msg
        jmp Message_Loop

    Exit_Program:
        INVOKE ExitProcess,0
WinMain ENDP
END WinMain
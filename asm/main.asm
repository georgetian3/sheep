.386
.model flat, stdcall  ; 32 bit memory model
option casemap :none  ; case sensitive

include includes.inc

;#############################################################
.DATA

include macros.inc
WindowName  byte "Sheep", 0
msg         MSGStruct <>
winRect     RECT <>
hMainWnd    DWORD ?
hInstance   DWORD ?

emptyStr    byte    0

; button.inc required data
N_BUTTONS equ 100
__button_count DWORD 0
__id_count DWORD 0
__buttons DWORD N_BUTTONS DUP(?)

; Define the Application's Window class structure.
MainWin WNDCLASS <NULL,WinProc,NULL,NULL,NULL,NULL,NULL,COLOR_WINDOW,NULL,WindowName>


SLOT_MOVE_TIME REAL8 0.2

WINDOW_WIDTH equ 768
WINDOW_HEIGHT equ 1024
STAGE_STARTMENU equ 0
STAGE_1 equ 1
UNIT_SIZE_X equ 80
UNIT_SIZE_Y equ 73
BUTTON_STARTGAME equ 1001
BUTTON_STARTGAME_WIDTH equ 212
BUTTON_STARTGAME_HEIGHT equ 76

MAP_WIDTH equ 8
MAP_LENGTH equ 8
MAP_HEIGHT equ 2

SLOT_X equ 100
SLOT_X_OFFSET equ 20
SLOT_Y equ 809
SLOT_SIZE equ 7
MATCH_COUNT equ 3

;SLOT_MOVE_TIME equ 0.5

slot_count DWORD 0
slot DWORD SLOT_SIZE DUP(0)




;#############################################################
.CODE

String  here, "HERE", 10, 13
String  hexStr, "%x"
String  decStr, "%d"
String  newline, 10, 13

include tile.inc
include button.inc
COMMENT `
include animation.inc
include utils.inc
include random.inc
include sound.inc
include sheep.inc
`
Print WindowName



WinProc PROC hWnd:DWORD, uMsg:DWORD, wParam:DWORD, lParam:DWORD

    ;LOCAL point:POINT
        String wm_create, "WM_CREATE", 10, 13
        String wm_command, "WM_COMMAND", 10, 13
        String wm_drawitem, "WM_DRAWITEM", 10, 13
        String done, "DONE", 10, 13
        mov ebx, uMsg

        .IF ebx == WM_DRAWITEM
            Print   OFFSET wm_drawitem
            INVOKE  draw_button, hWnd, wParam, lParam
            Print   OFFSET done
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
            INVOKE  ExitProcess, 0
            INVOKE  create_button, hWnd, 9, 100, 100, 1, TILE_WIDTH, TILE_HEIGHT
            Print   OFFSET wm_command
            ;INVOKE  play_sound, 0, 0, 0
        .ELSEIF ebx == WM_CLOSE
            INVOKE  PostQuitMessage, 0
        .ELSE
            INVOKE  DefWindowProc, hWnd, uMsg, wParam, lParam
        .ENDIF
    ret
WinProc ENDP







WinMain PROC
        finit
        INVOKE GetModuleHandle, NULL
        mov hInstance, eax
        mov MainWin.hInstance, eax
        INVOKE LoadIcon, NULL, IDI_APPLICATION
        mov MainWin.hIcon, eax
        INVOKE LoadCursor, NULL, IDC_ARROW
        mov MainWin.hCursor, eax

        INVOKE RegisterClass, ADDR MainWin
        .IF eax == 0
            jmp Exit_Program
        .ENDIF
        
        INVOKE CreateWindowEx, 0, ADDR WindowName,
            ADDR WindowName,MAIN_WINDOW_STYLE,
            CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
            CW_USEDEFAULT,NULL,NULL,hInstance,NULL
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
.386
.model flat, stdcall  ; 32 bit memory model
option casemap :none  ; case sensitive

include includes.inc
include macros.inc

;#############################################################
.DATA

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






;#############################################################
.CODE


include button.inc
include animation.inc
include tile.inc
include random.inc
include utils.inc
include sound.inc
include sheep.inc

WinProc PROC hWnd:DWORD, uMsg:DWORD, wParam:DWORD, lParam:DWORD

    LOCAL point:POINT

        mov ebx, uMsg

        .IF ebx == WM_COMMAND
            ; lParam: hWnd of button
            ; LOWORD(wParam): id
            ; HIWORD(wParam): event
            
            String  clicked, "Button clicked", 10, 13
            Print   OFFSET clicked
            INVOKE  WinPos, lParam, ADDR point

            String  intStr, "%d", 10, 13
            Print   OFFSET intStr, point.x
            Print   OFFSET intStr, point.y
        .ELSEIF ebx == WM_CREATE
            ;INVOKE  CreateButton, hWnd, 100, 100, 100, 100, 0
            INVOKE load_tiles

            ;play welcome.wav as the game starts
            INVOKE  play_sound, 0, 0, 0

            
        .ELSEIF ebx == WM_CLOSE
            INVOKE  PostQuitMessage,0
        .ELSE
            INVOKE  DefWindowProc, hWnd, uMsg, wParam, lParam
        .ENDIF

    WinProcExit:
        ret
WinProc ENDP































WinMain PROC
; Get a handle to the current process.
    INVOKE GetModuleHandle, NULL
    mov hInstance, eax
    mov MainWin.hInstance, eax

; Load the program's icon and cursor.
    INVOKE LoadIcon, NULL, IDI_APPLICATION
    mov MainWin.hIcon, eax
    INVOKE LoadCursor, NULL, IDC_ARROW
    mov MainWin.hCursor, eax

; Register the window class.
    INVOKE RegisterClass, ADDR MainWin
    .IF eax == 0
      jmp Exit_Program
    .ENDIF

; Create the application's main window.
; Returns a handle to the main window in EAX.
    INVOKE CreateWindowEx, 0, ADDR WindowName,
      ADDR WindowName,MAIN_WINDOW_STYLE,
      CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
      CW_USEDEFAULT,NULL,NULL,hInstance,NULL
    mov hMainWnd,eax

; If CreateWindowEx failed, display a message & exit.
    .IF eax == 0
      jmp  Exit_Program
    .ENDIF

; Show and draw the window.
    INVOKE ShowWindow, hMainWnd, SW_SHOW
    INVOKE UpdateWindow, hMainWnd

; Begin the program's message-handling loop.
Message_Loop:
    ; Get next message from the queue.
    INVOKE GetMessage, ADDR msg, NULL,NULL,NULL

    ; Quit if no more messages.
    .IF eax == 0
      jmp Exit_Program
    .ENDIF

    ; Relay the message to the program's WinProc.
    INVOKE DispatchMessage, ADDR msg
    jmp Message_Loop

Exit_Program:
      INVOKE ExitProcess,0
WinMain ENDP





END WinMain
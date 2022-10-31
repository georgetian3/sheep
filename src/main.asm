.386
.model flat, stdcall  ; 32 bit memory model
option casemap :none  ; case sensitive

include         includes.inc

include         button.inc

;==================== DATA =======================
.data


WindowName  BYTE "Sheep",0
className   BYTE "Sheep",0

; Define the Application's Window class structure.
MainWin WNDCLASS <NULL,WinProc,NULL,NULL,NULL,NULL,NULL,COLOR_WINDOW,NULL,className>

msg          MSGStruct <>
winRect   RECT <>
hMainWnd  DWORD ?
hInstance DWORD ?

buttonStr       byte    "button", 0
cwF             byte    "CreateWindow failed", 10, 13, 0
success     byte "SUCCESS", 10, 13, 0
error       byte "ERROR", 10, 13, 0

szText MACRO Name, Text:VARARG
    LOCAL lbl
        jmp lbl
        Name db Text,0
        lbl:
ENDM

;=================== CODE =========================
.code

CreateButton PROC hParent:DWORD,wd:DWORD,ht:DWORD,ID:DWORD

; BmpButton PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
; invoke BmpButton,hWnd,20,20,100,25,500

    szText bmpBtnCl,"BUTTON"
    szText blnk2,0

        invoke CreateWindowEx,0,
                ADDR bmpBtnCl,ADDR blnk2,
                WS_CHILD or WS_VISIBLE or BS_BITMAP,
                0,0,wd,ht,hParent,ID,
                hInstance,NULL

        .IF eax == 0
            INVOKE  crt_printf, OFFSET error
        .ELSE
            INVOKE  crt_printf, OFFSET success
        .ENDIF

    ret

CreateButton ENDP

ShowButton PROC USES ebx ecx edx hWnd:DWORD, visible:BYTE
        INVOKE  ShowWindow, hWnd, visible
    ret
ShowButton ENDP


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
    INVOKE CreateWindowEx, 0, ADDR className,
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

;-----------------------------------------------------
WinProc PROC, hWnd:DWORD, uMsg:DWORD, wParam:DWORD, lParam:DWORD
    mov ebx, uMsg

    .IF ebx == WM_CREATE
        INVOKE  CreateButton, hWnd, 100, 100, 0
        INVOKE  ShowButton, eax, 1

      jmp WinProcExit
    .ELSEIF ebx == WM_CLOSE
      INVOKE PostQuitMessage,0
      jmp WinProcExit
    .ELSE
      INVOKE DefWindowProc, hWnd, uMsg, wParam, lParam
      jmp WinProcExit
    .ENDIF

WinProcExit:
    ret
WinProc ENDP



END WinMain
.386
.model flat, stdcall  ; 32 bit memory model
option casemap :none  ; case sensitive

include         includes.inc

include         button.inc

;==================== DATA =======================
.data
WindowName	BYTE "Sheep",0
className	BYTE "Sheep",0

;tile.h data
N_TILE_TYPE BYTE 3
N_TILES BYTE 9
TILE_WIDTH BYTE 64
TILE_HEIGHT BYTE 64
tile_bitmaps DWORD 3 DUP(?)
whatever BYTE ?
img_error byte "LoadImage failed"

;animation.h data
FPS BYTE 60
MSPF BYTE 17


MovingTile STRUCT
	button DWORD ?
	start_x WORD ?
	start_y WORD ?
	start_time WORD ?
	dxpf REAL8 ?
	dypf REAL8 ?
	frame WORD ?
	frames WORD ?
	moving BYTE ?
MovingTile ENDS



; Define the Application's Window class structure.
MainWin WNDCLASS <NULL,WinProc,NULL,NULL,NULL,NULL,NULL, \
	COLOR_WINDOW,NULL,className>

msg	      MSGStruct <>
winRect   RECT <>
hMainWnd  DWORD ?
hInstance DWORD ?

buttonStr       byte    "button", 0
cwF             byte    "CreateWindow failed", 10, 13, 0

szText MACRO Name, Text:VARARG
    LOCAL lbl
        jmp lbl
        Name db Text,0
        lbl:
ENDM

;=================== CODE =========================
.code
load_tiles PROC
	filenames_carrot BYTE "../res/carrot.bmp"
	filenames_corn BYTE "../res/corn.bmp"
	filenames_grass BYTE "../res/grass.bmp"
	invoke LoadImageW, whatever, filenames_carrot, 0, 64, 64, 0x00000010
	.IF eax ==0
		invoke crt_printf, offset img_error
	.ENDif
	ret
load_tiles ENDP

CreateButton PROC USES ebx ecx edx hWnd:DWORD, id:DWORD, width0:DWORD, height:DWORD
    
        INVOKE  CreateWindowEx, 0, OFFSET buttonStr, 0, 0, 0, 0, width0, height, hWnd, id, 0, 0

        .IF eax == 0
            INVOKE  crt_printf, OFFSET cwF
        .ENDIF

    ret

CreateButton ENDP

ShowButton PROC USES ebx ecx edx hWnd:DWORD, visible:BYTE
        INVOKE  ShowWindow, hWnd, visible
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
	mov eax, uMsg

	.IF eax == WM_CREATE
        INVOKE  CreateButton, hWnd, 1, 100, 100
        INVOKE  crt_printf, OFFSET cwf
        INVOKE  ShowButton, eax, 1
        INVOKE  crt_printf, OFFSET cwf

	  jmp WinProcExit
	.ELSEIF eax == WM_CLOSE
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

; This program displays a resizable application window and
; several popup message boxes.
; Thanks to Tom Joyce for creating a prototype
; from which this program was derived.
; Last update: 9/24/01

INCLUDE C:\masm32\Irvine\Irvine32.inc
INCLUDE C:\masm32\Irvine\GraphWin.inc

INCLUDELIB C:\masm32\Irvine\Irvine32.lib


include         C:\masm32\include\msvcrt.inc
includelib      C:\masm32\lib\msvcrt.lib

includelib      C:\masm32\Irvine\Kernel32.lib
includelib      C:\masm32\Irvine\User32.Lib

include         button.inc

;==================== DATA =======================
.data


WindowName  BYTE "Sheep",0
className   BYTE "Sheep",0

; Define the Application's Window class structure.
MainWin WNDCLASS <NULL,WinProc,NULL,NULL,NULL,NULL,NULL, \
	COLOR_WINDOW,NULL,className>

msg	      MSGStruct <>
winRect   RECT <>
hMainWnd  DWORD ?
hInstance DWORD ?

buttonStr       byte    "button", 0
cwF             byte    "CreateWindow failed", 10, 13, 0

;=================== CODE =========================
.code

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
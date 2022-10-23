.386
.MODEL flat, stdcall
option casemap:none


include includes.inc

WinMain PROTO :DWORD,:DWORD,:DWORD,:DWORD
WndProc PROTO :DWORD,:DWORD,:DWORD,:DWORD

.DATA

testStr     byte    "test", 10, 13, 0

winTitle       byte    "Sheep", 0

.CODE

WndProc PROC hWnd: HWND, uMsg: UINT, wParam: WPARAM, lParam: LPARAM
        .IF uMsg == WM_CREATE
            INVOKE  crt_printf, OFFSET testStr
        .ELSEIF uMsg == WM_DESTROY
            INVOKE PostQuitMessage, NULL
        .ENDIF
	ret
WndProc ENDP

main PROC
        INVOKE  crt_printf, OFFSET testStr


main ENDP
END main
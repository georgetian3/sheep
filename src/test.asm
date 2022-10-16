; *** minimalistic code to create a window; see also \masm32\examples\exampl01\generic\generic.asm and Iczelion tutorial #3 ***
include masm32rt.inc        ; set defaults and include frequently used libraries (Kernel32, User32, CRT, ...)

.data        ; initialised data section
txClass        db "MyWinClass", 0                ; class name, will be registered below
wcx        WNDCLASSEX <WNDCLASSEX, CS_HREDRAW or CS_VREDRAW, WndProc, 0, 0, 1, 2, 3, COLOR_BTNFACE+1, 0, txClass, 4>

.data?        ; uninitialised data - use for handles etc
hEdit        dd ?

.code
WinMain proc uses ebx
LOCAL msg:MSG
  mov ebx, offset wcx
  wc equ [ebx.WNDCLASSEX]
  mov wc.hInstance, rv(GetModuleHandle, 0)
  mov wc.hIcon, rv(LoadIcon, NULL, IDI_APPLICATION)
  mov wc.hIconSm, eax
  ; mov wc.hCursor, rv(LoadCursor, NULL, IDC_ARROW)        ; not needed
  invoke RegisterClassEx, addr wc                ; the window class needs to be registered
  invoke CreateWindowEx, NULL, wc.lpszClassName, chr$("Hello World"),        ; window title
    WS_OVERLAPPEDWINDOW or WS_VISIBLE,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,        ; x, y, w, h
    NULL, NULL, wc.hInstance, NULL
  .While 1
        invoke GetMessage, ADDR msg, NULL, 0, 0
        .Break .if !eax
        invoke TranslateMessage, ADDR msg
        invoke DispatchMessage, ADDR msg
  .Endw
  exit msg.wParam
WinMain endp

WndProc proc uses esi edi ebx hWnd, uMsg, wParam:WPARAM, lParam:LPARAM
  SWITCH uMsg
  CASE WM_CREATE
        mov esi, rv(CreateMenu)                ; create the main menu
        mov edi, rv(CreateMenu)                ; create a sub-menu
        invoke AppendMenu, esi, MF_POPUP, edi, chr$("&File")        ; add it to the main menu
        invoke AppendMenu, edi, MF_STRING, 101, chr$("&New")        ; and add
        invoke AppendMenu, edi, MF_STRING, 102, chr$("&Save")        ; two items
        invoke SetMenu, hWnd, esi                ; attach menu to main window
        invoke CreateWindowEx, WS_EX_CLIENTEDGE, chr$("edit"), NULL,
          WS_CHILD or WS_VISIBLE or WS_BORDER or ES_AUTOVSCROLL or ES_MULTILINE,
          9, 9, 300, 200,
          hWnd, 103, wcx.hInstance, NULL        ; we have added an edit control
        mov hEdit, eax        ; you may need this global variable for further processing

  CASE WM_COMMAND
        movzx eax, word ptr wParam        ; the Ids are in the LoWord of wParam
        Switch eax
        case 101
                MsgBox 0, "You clicked New", "Hi", MB_OK
        case 102
                MsgBox 0, "You clicked Save", "Hi", MB_OK
        Endsw
  CASE WM_DESTROY
        invoke PostQuitMessage, NULL                ; quit after WM_CLOSE
  ENDSW
  invoke DefWindowProc, hWnd, uMsg, wParam, lParam        ; default processing
  ret
WndProc endp

end WinMain
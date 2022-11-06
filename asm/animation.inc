

WinPos PROC USES eax ebx ecx edx esi edi hWnd:DWORD, pPoint:DWORD
    COMMENT !
        Parameters
            [in] hWnd: handle to the window
            [out] pPoint: pointer to a POINT struct
        Saves in pPoint the position of the top-right corner of the window with respect to its parent
    !
    LOCAL rect:RECT, parent:DWORD

        INVOKE  GetWindowRect, hWnd, ADDR rect
        INVOKE  GetParent, hWnd
        mov     parent, eax
        INVOKE  MapWindowPoints, HWND_DESKTOP, parent, ADDR rect, 2


        mov     edi, pPoint
        mov     eax, rect.left
        mov     [edi], eax                  ; x
        mov     eax, rect.top
        mov     [edi + SIZEOF DWORD], eax   ; y

    ret
WinPos ENDP


move_button PROC USES eax ebx ecx edx esi edi btn_:DWORD, x:DWORD, y:DWORD, time:REAL8
    COMMENT !
        Parameters
            [in] hWnd: handle to the button
            [in] x, y: final position of the button
            [in] time: time taken for button to complete movement
    !

    ret
move_button ENDP
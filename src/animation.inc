FPS equ 120
MSPF equ 8

__move_button PROC uses eax ebx ecx edx esi edi hWnd:DWORD, uMsg:DWORD, idEvent:DWORD, dwTime:DWORD
    local btn_mov:DWORD
    String notfound, "Move Button not found", 0ah, 0dh
    String wrongstat, "Button not supposed to be moving", 0ah, 0dh
    String movefailed, "__move_button failed", 0ah, 0dh
    INVOKE get_button, hWnd
    mov btn_mov, eax
    .IF (btn_mov == 0)
        CheckError notfound
    .ENDIF
    mov ebx, (Button PTR [btn_mov]).moving
    not ebx
    .IF (ebx)
        CheckError wrongstat
    .ENDIF

    mov ecx, (Button PTR [btn_mov]).start_pos
    mov ebx, (POINT PTR [ecx]).x
    mov eax, (Button PTR [btn_mov]).dxpf
    mul (Button PTR [btn_mov]).frame
    add eax, ebx
    mov edx, eax

    mov ecx, (Button PTR [btn_mov]).start_pos
    mov ebx, (POINT PTR [ecx]).y
    mov eax, (Button PTR [btn_mov]).dypf
    mul (Button PTR [btn_mov]).frame
    add eax, ebx
    mov edi, eax
    INVOKE SetWindowPos, hWnd, 0, edx, edi, 0, 0, SWP_NOSIZE
    .IF (eax == 0)
        CheckError movefailed
    .ENDIF
    inc (Button PTR [btn_mov]).frame
    mov eax, (Button PTR [btn_mov]).frame
    mov ebx, (Button PTR [btn_mov]).frames
    .IF (eax > ebx)
        mov (Button PTR [btn_mov]).moving, 0
        INVOKE KillTimer, hWnd, idEvent
        mov ecx, (Button PTR [btn_mov]).callback
        .IF (ecx)
            INVOKE (Button PTR [btn_mov]).callback
        .ENDIF
    .ENDIF
    ret
__move_button ENDP


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


MoveButton PROC USES eax ebx ecx edx esi edi hWnd:DWORD, x:DWORD, y:DWORD, time:REAL8
    COMMENT !
        Parameters
            [in] hWnd: handle to the button
            [in] x, y: final position of the button
            [in] time: time taken for button to complete movement
    !

    ret
MoveButton ENDP
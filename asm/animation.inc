FPS equ 120
MSPF equ 8

__move_button PROC uses eax ebx ecx edx esi edi hWnd:DWORD, uMsg:DWORD, idEvent:QWORD, dwTime:DWORD
    local btn_mov:DWORD
    String notfound, "Move Button not found", 0ah, 0dh
    String wrongstat, "Button not supposed to be moving", 0ah, 0dh
    String movefailed, "__move_button failed", 0ah, 0dh
    INVOKE get_button, hWnd
    mov btn_mov, eax
    .IF (btn_mov == 0)
        CheckError notfound
    .ENDIF
    mov edi, btn_mov
    mov ebx, (Button PTR [edi]).moving
    not ebx
    .IF (ebx)
        CheckError wrongstat
    .ENDIF

    mov ecx, (Button PTR [edi]).start_pos
    mov ebx, (POINT PTR [ecx]).x
    mov eax, (Button PTR [edi]).dxpf
    mul (Button PTR [edi]).frame
    add eax, ebx
    mov edx, eax

    mov ecx, (Button PTR [edi]).start_pos
    mov ebx, (POINT PTR [ecx]).y
    mov eax, (Button PTR [edi]).dypf
    mul (Button PTR [edi]).frame
    add eax, ebx
    mov esi, eax
    INVOKE SetWindowPos, hWnd, 0, edx, esi, 0, 0, SWP_NOSIZE
    .IF (eax == 0)
        CheckError movefailed
    .ENDIF
    inc (Button PTR [edi]).frame
    mov eax, (Button PTR [edi]).frame
    mov ebx, (Button PTR [edi]).frames
    .IF (eax > ebx)
        mov (Button PTR [edi]).moving, 0
        INVOKE KillTimer, hWnd, idEvent
        mov ecx, (Button PTR [edi]).callback
        .IF (ecx)
            ;INVOKE (Button PTR [edi]).callback
        .ENDIF
    .ENDIF
    ret
__move_button ENDP

move_button PROC USES eax ebx ecx edx esi edi hWnd:DWORD, x:DWORD, y:DWORD, time:REAL8
    COMMENT !
        Parameters
            [in] hWnd: handle to the button
            [in] x, y: final position of the button
            [in] time: time taken for button to complete movement
    !

    ret
move_button ENDP
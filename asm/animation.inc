FPS equ 120
MSPF equ 8

__move_button PROC uses eax ebx ecx edx esi edi hWnd:DWORD, uMsg:DWORD, idEvent:QWORD, dwTime:DWORD
    local btn_mov:DWORD
    String notfound, "Move Button not found", 0ah, 0dh
    String wrongstat, "Button not supposed to be moving", 0ah, 0dh
    String movefailed, "__move_button failed", 0ah, 0dh
    INVOKE get_button, hWnd
    mov btn_mov, eax
    .IF btn_mov == 0
            Print OFFSET notfound
    .ENDIF
    mov     edi, btn_mov
    mov     ebx, (Button PTR [edi]).moving
    not     ebx
    .IF ebx
            Print   OFFSET wrongstat
    .ENDIF

    mov     esi, (Button PTR [edi]).start_pos
    mov     ebx, (POINT PTR [esi]).x
    mov eax, (Button PTR [edi]).dxpf
    mul (Button PTR [edi]).frame
    add eax, ebx
    mov ecx, eax

    mov ebx, (POINT PTR [esi]).y
    mov eax, (Button PTR [edi]).dypf
    mul (Button PTR [edi]).frame
    add eax, ebx
    mov edx, eax
    INVOKE SetWindowPos, hWnd, 0, ecx, edx, 0, 0, SWP_NOSIZE|SWP_NOZORDER
    .IF eax == 0
        CheckError movefailed
    .ENDIF
    inc (Button PTR [edi]).frame
    mov eax, (Button PTR [edi]).frame
    mov ebx, (Button PTR [edi]).frames
    .IF eax > ebx
        mov (Button PTR [edi]).moving, 0
        INVOKE KillTimer, hWnd, idEvent
        mov ecx, (Button PTR [edi]).callback
        .IF ecx
            INVOKE update
        .ENDIF
    .ENDIF
    mov ebx, (Button PTR [edi]).hWnd
    INVOKE InvalidateRect, ebx, 0, 0
    ret
__move_button ENDP

move_button PROC USES eax ebx ecx edx esi edi hWnd:DWORD, x:DWORD, y:DWORD, time:REAL8
    String startmove, "Move button", 0ah, 0dh
    String movebtnfailed, "move button failed", 0ah, 0dh
    String timerfailed, "SetTimer failed", 0ah, 0dh
    Print OFFSET startmove
    .IF time == 0
        mov ebx, hWnd
        INVOKE SetWindowPos, (Button PTR [ebx]).hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER
        .IF eax == 0
            Print OFFSET movebtnfailed
        .ENDIF
        .IF (Button PTR [ebx]).callback
            INVOKE update
        .ENDIF
        ret
    .ENDIF

    mov edi, hWnd
    mov ebx, (Button PTR [edi]).hWnd
    mov ecx, addr (Button PTR [edi]).start_pos
    INVOKE win_pos, ebx, ecx
    mov (Button PTR [edi]).moving, 1
    mov (Button PTR [edi]).frame, 1

    INVOKE GetTickCount
    mov (Button PTR [edi]).start_time, eax
    mov eax, FPS
    mul time
    mov (Button PTR [edi]).frames, eax
    
    mov esi, (Button PTR [edi]).start_pos
    mov ebx, (POINT PTR [esi]).x
    mov eax, x
    sub eax, ebx
    div (Button PTR [edi]).frames
    mov (Button PTR [edi]).dxpf, eax

    mov ebx, (POINT PTR [esi]).y
    mov eax, y
    sub eax, ebx
    div (Button PTR [edi]).frames
    mov (Button PTR [edi]).dypf, eax

    INVOKE SetTimer, (Button PTR[edi]).hWnd, 0, MSPF, __move_button
    .IF eax == 0
        Print OFFSET timerfailed
    .ENDIF
    ret
move_button ENDP
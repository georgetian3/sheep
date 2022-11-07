FPS equ 120
MSPF equ 8

__move_button PROC uses eax ebx ecx edx esi edi hWnd:DWORD, uMsg:DWORD, idEvent:DWORD, dwTime:DWORD
    local btn_mov:DWORD
    String move_button_not_found, "Move Button not found", 0ah, 0dh
    String wrong_stat, "Button not supposed to be moving", 0ah, 0dh
    String __move_failed, "__move_button failed", 0ah, 0dh
    INVOKE get_button, hWnd
    mov btn_mov, eax
    .IF btn_mov == 0
            Print OFFSET move_button_not_found
    .ENDIF
    mov     edx, btn_mov
    mov     ebx, (Button PTR [edx]).moving
    not     ebx
    .IF ebx
            Print   OFFSET wrong_stat
    .ENDIF
    
    mov edi, btn_mov
    mov esi, (Button PTR [edi]).start_pos.x
    mov eax, (Button PTR [edi]).dxpf    ;real8
    
    mul (Button PTR [edi]).frame    ;real8 mul
    add eax, esi
    mov ecx, eax

    mov ebx, (Button PTR [edi]).start_pos.y
    mov eax, (Button PTR [edi]).dypf    ;real8
    mul (Button PTR [edi]).frame    ;real8 mul
    add eax, ebx
    mov edx, eax
    mov ebx, SWP_NOSIZE
    or ebx, SWP_NOZORDER
    INVOKE SetWindowPos, hWnd, 0, ecx, edx, 0, 0, ebx
    .IF eax == 0
        Print OFFSET __move_failed
    .ENDIF
    inc (Button PTR [edi]).frame
    mov eax, (Button PTR [edi]).frame
    mov ebx, (Button PTR [edi]).frames
    .IF eax > ebx
        mov (Button PTR [edi]).moving, 0
        INVOKE KillTimer, hWnd, idEvent
        mov ecx, (Button PTR [edi]).callback
        .IF ecx
            ;INVOKE update
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
    .IF time == 0 ;real8 problem
        mov ebx, hWnd
        mov ecx, SWP_NOSIZE
        or ecx, SWP_NOZORDER
        pushad
        INVOKE SetWindowPos, (Button PTR [ebx]).hWnd, HWND_TOP, x, y, 0, 0, ecx
        .IF eax == 0
            Print OFFSET movebtnfailed
        .ENDIF
        popad
        mov edx, (Button PTR [ebx]).callback
        .IF edx
            ;INVOKE update
        .ENDIF
        ret
    .ENDIF

    mov edi, hWnd
    mov ebx, (Button PTR [edi]).hWnd
    mov ecx, hWnd[64]
    INVOKE win_pos, ebx, ecx
    mov (Button PTR [edi]).moving, 1
    mov (Button PTR [edi]).frame, 1

    INVOKE GetTickCount
    mov (Button PTR [edi]).start_time, eax
    mov eax, FPS
    mul time    ;real8 problem
    mov (Button PTR [edi]).frames, eax
    
    mov esi, (Button PTR [edi]).start_pos.x
    mov eax, x
    sub eax, esi
    div (Button PTR [edi]).frames
    mov (Button PTR [edi]).dxpf, eax    ;real8 problem

    mov ebx, (Button PTR [edi]).start_pos.y
    mov eax, y
    sub eax, ebx
    div (Button PTR [edi]).frames
    mov (Button PTR [edi]).dypf, eax    ;real8 problem

    INVOKE SetTimer, (Button PTR[edi]).hWnd, 0, MSPF, __move_button
    .IF eax == 0
        Print OFFSET timerfailed
    .ENDIF
    ret
move_button ENDP
__move_button PROC uses eax ebx ecx edx esi edi hWnd:DWORD, uMsg:DWORD, idEvent:DWORD, dwTime:DWORD
    local btn_mov:DWORD, temp_x_result:DWORD, temp_y_result:DWORD
    String move_button_not_found, "Move Button not found", 0ah, 0dh
    String wrong_stat, "Button not supposed to be moving", 0ah, 0dh
    String __move_failed, "__move_button failed", 0ah, 0dh
    FINIT
    INVOKE get_button, hWnd
    mov btn_mov, eax
    .IF btn_mov == 0
            Print OFFSET move_button_not_found
    .ENDIF
    mov edx, btn_mov
    mov ebx, (Button PTR [edx]).moving
    not ebx
    ; .IF ebx
    ;     Print   OFFSET wrong_stat
    ; .ENDIF
    
    mov edi, btn_mov
    mov esi, (Button PTR [edi]).start_pos.x
    
    fild (Button PTR [edi]).frame
    fmul (Button PTR [edi]).dxpf
    fist temp_x_result
    add esi, temp_x_result
   
    mov ecx, (Button PTR [edi]).start_pos.y
    fild (Button PTR [edi]).frame
    fmul (Button PTR [edi]).dypf
    fist temp_y_result
    add ecx, temp_y_result

    mov ebx, SWP_NOSIZE
    or ebx, SWP_NOZORDER
    INVOKE SetWindowPos, hWnd, 0, esi, ecx, 0, 0, ebx
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
        .IF ecx == 1
            INVOKE update
        .ELSEIF ecx==2
            INVOKE match_slot
        .ENDIF
    .ENDIF
    mov ebx, (Button PTR [edi]).hWnd
    INVOKE InvalidateRect, ebx, 0, 0
    ret
__move_button ENDP

move_button PROC USES eax ebx ecx edx esi edi hWnd:DWORD, x:DWORD, y:DWORD, __time:REAL8
    local temp_FPS:DWORD, temp_frames_result:DWORD, temp_differnece:DWORD
    String startmove, "Move button", 0ah, 0dh
    String movebtnfailed, "move button failed", 0ah, 0dh
    String timerfailed, "SetTimer failed", 0ah, 0dh
    String printfloat, "%f", 0ah, 0dh ;for testing float
    String posi ,"x = %d , y = %d ",0ah,0dh
    Print OFFSET startmove
    finit
    fld epsilon
    fldz
    fsub __time
    fabs
    fcom ST(1)
    ; fstp floattest
    ; Print OFFSET printfloat, floattest
    ja skip
    Print OFFSET done   ;没到过这里
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
    ; Print OFFSET newline
    ; Print OFFSET decStr, edx
    ; Print OFFSET newline
    ; Print OFFSET newline
    .IF edx == 1
        INVOKE update
    .ELSEIF edx == 2
        INVOKE match_slot
    .ENDIF
    ret

    skip:
    mov edi, hWnd
    ;INVOKE  get_button, hWnd
    ;mov     edi, eax

    mov ebx, (Button PTR [edi]).hWnd

    Print offset here
    INVOKE win_pos ,ebx,edi
    mov (Button PTR [edi]).moving, 1
    mov (Button PTR [edi]).frame, 1
    INVOKE GetTickCount
    mov (Button PTR [edi]).start_time, eax
    mov temp_FPS, FPS
    fild temp_FPS
    fmul __time
    fist temp_frames_result
    mov eax, temp_frames_result
    mov (Button PTR [edi]).frames, eax
    
    mov esi, (Button PTR [edi]).start_pos.x

    mov eax, x
    sub eax, esi

    mov temp_differnece, eax
    fild (Button PTR [edi]).frames
    fild temp_differnece
    fdiv ST(0), ST(1)
    fstp (Button PTR [edi]).dxpf
    ; Print offset newline
    ; Print OFFSET printfloat, (Button PTR [edi]).dxpf
    ; Print offset newline
    
    mov ebx, (Button PTR [edi]).start_pos.y
    Print OFFSET posi,esi,ebx
    mov eax, y
    sub eax, ebx
    mov temp_differnece, eax
    fild (Button PTR [edi]).frames
    fild temp_differnece

    fdiv ST(0), ST(1)
    fstp (Button PTR [edi]).dypf
    ; Print offset newline
    ; Print OFFSET printfloat, (Button PTR [edi]).dypf
    ; Print offset newline

    INVOKE SetTimer, (Button PTR[edi]).hWnd, 0, MSPF, __move_button
    .IF eax == 0
        Print OFFSET timerfailed
    .ENDIF
    ret
move_button ENDP
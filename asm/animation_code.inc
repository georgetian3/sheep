update PROC USES eax ebx ecx edx esi edi
    LOCAL a:DWORD, b:DWORD, btn_update:DWORD, flag:DWORD
    String update_begin, "begin update", 0ah, 0dh
    String no_button, "no button found", 0ah, 0dh
    String update_end, "update end", 0ah, 0dh
    Print OFFSET update_begin
    mov ecx, 0
    .WHILE ecx < N_BUTTONS
        push ecx
        INVOKE get_button_at_index, ecx
        .IF eax == -1
            jmp S1
        .ENDIF
        mov b, eax
        INVOKE is_tile, b
        .IF eax == 0
            mov ebx, 1
        .ELSEIF eax == 1
            mov ebx, 0
        .ENDIF
        .IF ebx
            Print offset newline
            Print offset hexStr, eax
            Print offset newline
            Print offset newline
            jmp S1
        .ENDIF
        ret
        mov ebx, b
        mov eax, (Button PTR [ebx]).moving
        .IF eax
            jmp S1
        .ENDIF
        mov flag, 1
        mov esi, 0
        .WHILE esi < N_BUTTONS
            push esi
            INVOKE get_button_at_index, esi
            .IF eax == -1
                jmp S2
            .ENDIF
            mov a, eax
            mov ebx, b

            Print offset newline
            Print offset hexStr, eax
            Print offset newline
            Print offset hexStr, ebx
            Print offset newline
            Print offset newline
            ret
            cmp eax, ebx
            je S2
            INVOKE is_tile, a   ;note!!
            not eax
            .IF eax
                jmp S2
            .ENDIF
            mov ebx, b
            mov eax, (Button PTR [ebx]).moving
            .IF eax
                jmp S2
            .ENDIF
            mov eax, a
            INVOKE overlap, a, b
            mov edx, eax
            mov eax, (Button PTR [ebx]).in_slot  
            .IF eax
                INVOKE set_active, b, 0
                mov (Button PTR [ebx]).gray, 0
                mov flag, 0
            .ELSEIF  edx
                INVOKE set_active, b, 0
                mov (Button PTR [ebx]).gray, 1
                mov flag, 0
                mov edi, (Button PTR [ebx]).hWnd
                mov esi, b
                INVOKE win_pos, edi, esi
            .ENDIF
            S2:
                pop esi
                inc esi 
        .ENDW
        mov eax, flag
        .IF eax == 1
            INVOKE set_active, b, 1
            mov ebx, b
            mov (Button PTR [ebx]).gray, 0
        .ENDIF
        S1:
            pop ecx
            inc ecx
    .ENDW
    mov ecx, 0
    .WHILE ecx < N_BUTTONS
        push ecx
        INVOKE get_button_at_index, ecx
        .IF eax == 0
            Print OFFSET no_button
        .ENDIF
        mov btn_update, eax
        .IF btn_update
            mov ebx, btn_update
            mov edx, (Button PTR [ebx]).hWnd
            INVOKE InvalidateRect, edx, 0, 0
        .ENDIF
        pop ecx
        inc ecx
    .ENDW

    Print OFFSET update_end
    ret
update ENDP

match_slot PROC uses eax ebx ecx edx esi edi
    local count:DWORD, prev_type:DWORD
    String test_print , "test here" ,0ah,0dh
    String matching, "match slot", 0ah, 0dh
    Print offset matching
    mov count, 0
    mov prev_type, -1
    mov ecx, 0
    .WHILE (ecx < slot_count)
        AINVOKE AA1, OFFSET slot, ecx
        mov esi, [eax]
        mov edi, (Button PTR [esi]).type_
        .IF edi == prev_type
            inc     count
            .IF count >= MATCH_COUNT
                mov edx, 0
                .WHILE(edx < count)
                    push edx
                    mov     ebx, ecx
                    sub     ebx, edx
                    AINVOKE AA1, OFFSET slot, ebx
                    mov     eax, [eax]
                    INVOKE  delete_button, (Button PTR [eax]).hWnd
                    pop edx
                    inc edx
                .ENDW
                mov     ebx, ecx
                inc     ebx
                .WHILE ebx < slot_count
        Print OFFSET here
                        mov     edx, ebx
                        sub     edx, count
                        AINVOKE slot_x, edx
                        mov     esi, eax
                        AINVOKE AA1, OFFSET slot, ebx
                        mov     eax, [eax]
                        PINVOKE move_button, eax, esi, SLOT_Y, SLOT_MOVE_TIME
                        mov     edi, [eax]
                        AINVOKE AA1, OFFSET slot, edx
                        mov     [eax], edi

                        inc     ebx
                .ENDW
                mov esi, count
                sub slot_count, esi
                mov     last_button, 0
                ret
            .ENDIF
        .ELSE
            mov count, 1
            AINVOKE AA1, OFFSET slot, ecx
            mov     eax, [eax]
            mov     eax, (Button PTR [eax]).type_
            mov     prev_type, eax
        .ENDIF
        inc ecx
    .ENDW
    INVOKE print_buttons
    ret
match_slot ENDP

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
    Print OFFSET newline
    Print OFFSET decStr, edx
    Print OFFSET newline
    Print OFFSET newline
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
    INVOKE win_pos, ebx, edi


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
    Print offset newline
    Print OFFSET printfloat, (Button PTR [edi]).dxpf
    Print offset newline
    
    mov ebx, (Button PTR [edi]).start_pos.y
    mov eax, y
    sub eax, ebx
    mov temp_differnece, eax
    fild (Button PTR [edi]).frames
    fild temp_differnece

    fdiv ST(0), ST(1)
    fstp (Button PTR [edi]).dypf
    Print offset newline
    Print OFFSET printfloat, (Button PTR [edi]).dypf
    Print offset newline

    INVOKE SetTimer, (Button PTR[edi]).hWnd, 0, MSPF, __move_button
    .IF eax == 0
        Print OFFSET timerfailed
    .ENDIF
    ret
move_button ENDP
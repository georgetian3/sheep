
Stage Struct
    id DWORD ?
    timeCountDown DWORD ?
    bg DWORD ?
Stage ENDS

Array2D PROC p:DWORD, i:DWORD, j:DWORD

        ; i, j *= 4
        sal     i, 2
        sal     j, 2

        mov     eax, p
        add     eax, i
        mov     eax, [eax]

        add     eax, j

    ret
Array2D ENDP

overlap PROC uses ebx ecx edx esi edi a:DWORD, b:DWORD
    local pt_a:DWORD, pt_b:DWORD
    mov ebx, (Button PTR [a]).k
    mov ecx, (Button PTR [b]).k
    .IF (ebx <= ecx)
        mov eax, 0
        jmp F1
    .ENDIF
    lea ebx, (Button PTR [a]).in_slot
    .IF (ebx)
        mov eax, 0
        jmp F1
    .ENDIF

    mov ebx, (Button PTR [a]).hWnd
    mov ecx, pt_a
    INVOKE win_pos, ebx, ecx
    mov ebx, (Button PTR [b]).hWnd
    mov ecx, pt_b
    INVOKE win_pos, ebx, ecx

    mov edx, (POINT PTR [pt_b]).x
    mov ebx, TILE_WIDTH
    add ebx, (POINT PTR [pt_b]).x
    .IF (edx >= ebx)
        mov eax, 0
        jmp F1
    .ENDIF
    mov ebx, (POINT PTR [pt_b]).x
    sub ebx, TILE_WIDTH
    .IF (edx <= ebx)
        mov eax, 0
        jmp F1
    .ENDIF
    mov edx, (POINT PTR [pt_b]).y
    mov ebx, (POINT PTR [pt_b]).y
    add ebx, TILE_HEIGHT
    .IF (edx >= ebx)
        mov eax, 0
        jmp F1
    .ENDIF
    mov ebx, (POINT PTR [pt_b]).y
    sub ebx, TILE_HEIGHT
    .IF (edx <= ebx)
        mov eax, 0
        jmp F1
    .ENDIF

    mov eax, 1
    F1:
        ret
overlap ENDP

shuffle PROC uses eax ebx ecx edx esi edi arr:DWORD, n:DWORD
    mov ecx, n
    dec ecx
    .WHILE (ecx > 0)
        INVOKE crt_rand
        div n
        mov ebx, arr[edx]
        mov esi, n
        mov eax, arr[esi]
        mov arr[edx], eax
        mov arr[esi], ebx
        dec ecx
    .ENDW
    ret
shuffle ENDP

slot_x PROC uses ebx ecx edx esi edi slot_index:DWORD
    mov ebx, SLOT_X_OFFSET
    add ebx, TILE_WIDTH
    mov eax, slot_index
    mul ebx
    add eax, SLOT_X
    ret
slot_x ENDP

match_slot PROC uses eax ebx ecx edx esi edi
    local count:DWORD, prev_type:DWORD
    String matching, "match slot", 0ah, 0dh
    Print matching
    mov count, 0
    mov prev_type, -1
    mov ecx, 0
    .WHILE (ecx < slot_count)
        mov esi, slot[ecx]
        mov edi, (Button PTR [esi]).type_
        .IF( edi == prev_type)
            inc count
            .IF (count >= MATCH_COUNT)
                mov edx, 0
                .WHILE(edx < count)
                    mov ebx, ecx
                    sub ebx, edx
                    INVOKE delete_button_struct, slot[ebx]
                .ENDW
                mov ebx, ecx
                inc ebx
                mov edx, ebx
                .WHILE(edx < slot_count)
                    mov ebx, edx
                    sub ebx, count
                    INVOKE slot_x, ebx

                    ;INVOKE move_button, slot[edx], eax, SLOT_Y, SLOT_LOSE_TIME
                    mov esi, slot[edx]
                    mov slot[ebx], esi
                .ENDW
                mov esi, count
                sub slot_count, esi
                jmp F1
            .ENDIF
        .ELSE
            mov count, 1
            mov ebx, slot[ecx]
            mov eax, (Button PTR [ebx]).type_
            mov prev_type, eax
        .ENDIF
        inc ecx
    .ENDW
    ;update()?
    F1:
    ret
match_slot ENDP


insert_slot PROC uses eax ebx ecx edx esi edi btn_insert:DWORD, index:DWORD
        String  insertprint, "Insert Slot", 0ah, 0dh
        Print   OFFSET insertprint
        mov ecx, slot_count
        dec ecx
        .IF ecx == 0FFFFFFFFh
            jmp L1
        .ENDIF
        Print Offset hexStr,index

        .WHILE (ecx >= index)
            mov ebx, ecx
            inc ebx
            INVOKE slot_x, ebx
            Print Offset hexStr,ecx
            INVOKE move_button, slot[ecx], eax, SLOT_Y, SLOT_MOVE_TIME
            mov esi, slot[ecx]
            mov slot[ebx], esi
            dec ecx
        .ENDW
        L1:
        mov eax, btn_insert
        mov slot[index], eax
        mov (Button PTR [eax]).in_slot, 1
        mov (Button PTR [eax]).callback, 2
        inc slot_count
        INVOKE slot_x, index
        ret
        INVOKE move_button, btn_insert, eax, SLOT_Y, SLOT_MOVE_TIME
    ret
insert_slot ENDP

destroy_all_button PROC uses eax ebx ecx edx esi edi
    String destroy_print, "%d", 0ah, 0dh
    mov ecx, 0
    .WHILE ecx < N_BUTTONS
        mov eax, 4
        mul ecx
        mov ebx, __buttons[eax]
        .IF ebx
            pushad
            INVOKE delete_button_struct, ebx
            popad
            Print OFFSET destroy_print, ecx
        .ENDIF
        inc ecx
    .ENDW
    mov start_game, 0
    mov end_game, 0
    mov undo_btn, 0
    ret
destroy_all_button ENDP

paint PROC uses eax ebx ecx edx esi edi hWnd:DWORD
    local ps:PAINTSTRUCT, hdc_window:HDC, hdc_memBuffer:HDC, hdc_loadBmp:HDC, blankBmp:DWORD
    String paint_failed, "Paint() error", 0ah, 0dh
    mov ebx, hWnd
    INVOKE BeginPaint, ebx, addr ps
    .IF eax == 0
        Print OFFSET paint_failed
    .ENDIF
    mov hdc_window, eax

    INVOKE CreateCompatibleDC, hdc_window
    .IF eax == 0
        Print OFFSET paint_failed
    .ENDIF
    mov hdc_memBuffer, eax

    INVOKE CreateCompatibleDC, hdc_window
    .IF eax == 0
        Print OFFSET paint_failed
    .ENDIF
    mov hdc_loadBmp, eax

    INVOKE CreateCompatibleBitmap, hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT
    .IF eax == 0
        Print OFFSET paint_failed
    .ENDIF
    mov blankBmp, eax

    INVOKE SelectObject, hdc_memBuffer, blankBmp
    INVOKE SelectObject, hdc_loadBmp, bmp_bg
    INVOKE BitBlt, hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY
    INVOKE BitBlt, hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY
    INVOKE DeleteObject, blankBmp
    INVOKE DeleteDC, hdc_memBuffer
    INVOKE DeleteDC, hdc_loadBmp
    mov ebx, hWnd
    INVOKE EndPaint, ebx, addr ps
    ret
paint ENDP
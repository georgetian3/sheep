overlap PROC uses ebx ecx edx esi edi a:DWORD, b:DWORD
    local pa:DWORD, pb:DWORD, a_x:DWORD, a_y:DWORD, b_x:DWORD, b_y:DWORD


    COMMENT `
    mov     eax, a
    mov     ebx, b
    mov     ecx, (Button PTR [eax]).k
    mov     edx, (Button PTR [ebx]).k

    .IF ecx <= edx
            mov     eax, 0
            ret
    .ENDIF

    mov     ecx (Button PTR [eax]).in_slot
    .IF eax != 0
            mov     eac, 0
            ret
    ret

    `

    mov edx , a
    mov esi , b 
    mov ebx, (Button PTR [edx]).k
    mov ecx, (Button PTR [esi]).k
    .IF (ebx <= ecx)
        mov eax, 0
        jmp F1
    .ENDIF
    lea ebx, (Button PTR [edx]).in_slot
    .IF (ebx)
        mov eax, 0
        jmp F1
    .ENDIF

    mov ebx, (Button PTR [edx]).hWnd
    mov ecx, pa
    INVOKE win_pos, ebx, ecx
    mov ebx, (Button PTR [esi]).hWnd
    mov ecx, pb
    INVOKE win_pos, ebx, ecx
    mov ecx, pb
    mov edx, (POINT PTR [ecx]).x
    mov ebx, TILE_WIDTH
    mov ecx, pb
    add ebx, (POINT PTR [ecx]).x
    .IF (edx >= ebx)
        mov eax, 0
        jmp F1
    .ENDIF
    mov ecx, pb
    mov ebx, (POINT PTR [ecx]).x
    sub ebx, TILE_WIDTH
    .IF (edx <= ebx)
        mov eax, 0
        jmp F1
    .ENDIF
    mov ecx, pb
    mov edx, (POINT PTR [ecx]).y
    mov ebx, (POINT PTR [ecx]).y
    add ebx, TILE_HEIGHT
    .IF (edx >= ebx)
        mov eax, 0
        jmp F1
    .ENDIF
    mov ecx, pb
    mov ebx, (POINT PTR [ecx]).y
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

        mov     esi, arr
        mov     ecx, n
        dec     ecx
        .WHILE ecx > 0
                push    ecx
                AINVOKE crt_rand
                pop     ecx
                mov     edx, 0
                div     n
                AINVOKE AA1, arr, ecx
                mov     esi, eax
                AINVOKE AA1, arr, edx
                mov     edi, eax

                mov     eax, [esi]
                mov     ebx, [edi]
                mov     [esi], ebx
                mov     [edi], eax
                dec     ecx
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


print_eip PROC USES eax ebx ecx edx esi edi 
        call next_
    next_:
        pop eax

        String  eipstr, "EIP: %h", 10, 13
        Print   OFFSET eipstr, eax
    ret
print_eip ENDP

insert_slot PROC uses eax ebx ecx edx esi edi btn_insert:DWORD, index_:DWORD
        String  insertprint, "Insert Slot", 0ah, 0dh
        String  inWhile, "inWile", 0ah, 0dh
        Print   OFFSET insertprint
        Print OFFSET decStr, slot_count
        Print OFFSET newline
        Print OFFSET decStr, index_
        Print OFFSET newline
        Print OFFSET newline

        mov ecx, slot_count
        dec ecx
        .IF ecx == 0FFFFFFFFh
            jmp L1
        .ENDIF      ;is this .IF needed?
        .WHILE (ecx >= index_)
            mov ebx, ecx
            inc ebx
            INVOKE slot_x, ebx
            mov edi, eax
            Print OFFSET inWhile
            mov eax, 4
            mul ecx
            mov esi, slot[eax]
            INVOKE move_button, esi, edi, SLOT_Y, SLOT_MOVE_TIME
            mov eax, 4
            mul ebx
            mov edi, slot[eax]
            mov edi, esi

            dec ecx
        .ENDW
        ; Print OFFSET decStr, slot_count
        ; Print OFFSET newline
        ; Print OFFSET decStr, index_
        ; Print OFFSET newline
        ; Print OFFSET newline
        L1:
        mov eax, 4
        mov ecx, index_
        mul ecx

        mov esi, btn_insert
        PINVOKE print_buttons
        Print   OFFSET hexStr, btn_insert
        Print   OFFSET newline
        mov slot[eax], esi
        mov (Button PTR [esi]).in_slot, 1
        mov (Button PTR [esi]).callback, 2
        inc slot_count
        dec total
        mov ecx, total
        .IF ecx == 0
            fldz
            fst realzero
            INVOKE slot_x, index_
            INVOKE move_button, esi, eax, SLOT_Y, realzero
        .ELSE
            INVOKE slot_x, index_
            INVOKE move_button, esi, eax, SLOT_Y, SLOT_MOVE_TIME
        .ENDIF
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
            INVOKE  delete_button, (Button PTR [ebx]).hWnd
            Print   OFFSET destroy_print, ecx
        .ENDIF
        inc ecx
    .ENDW
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
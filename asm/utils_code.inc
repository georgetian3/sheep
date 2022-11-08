
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
    mov ecx, pt_a
    INVOKE win_pos, ebx, ecx
    mov ebx, (Button PTR [esi]).hWnd
    mov ecx, pt_b
    INVOKE win_pos, ebx, ecx
    mov ecx, pt_b
    mov edx, (POINT PTR [ecx]).x
    mov ebx, TILE_WIDTH
    mov ecx, pt_b
    add ebx, (POINT PTR [ecx]).x
    .IF (edx >= ebx)
        mov eax, 0
        jmp F1
    .ENDIF
    mov ecx, pt_b
    mov ebx, (POINT PTR [ecx]).x
    sub ebx, TILE_WIDTH
    .IF (edx <= ebx)
        mov eax, 0
        jmp F1
    .ENDIF
    mov ecx, pt_b
    mov edx, (POINT PTR [ecx]).y
    mov ebx, (POINT PTR [ecx]).y
    add ebx, TILE_HEIGHT
    .IF (edx >= ebx)
        mov eax, 0
        jmp F1
    .ENDIF
    mov ecx, pt_b
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



insert_slot PROC uses eax ebx ecx edx esi edi btn_insert:DWORD, index:DWORD
        String  insertprint, "Insert Slot", 0ah, 0dh
        String  inWhile, "inWile", 0ah, 0dh
        Print   OFFSET insertprint
        mov ecx, slot_count
        dec ecx
        .IF ecx == 0FFFFFFFFh
            jmp L1
        .ENDIF
        .WHILE (ecx >= index)
            mov ebx, ecx
            inc ebx
            INVOKE slot_x, ebx
            Print OFFSET inWhile
            INVOKE move_button, slot[ecx], eax, SLOT_Y, SLOT_MOVE_TIME
            mov esi, slot[ecx]
            mov slot[ebx], esi
            dec ecx
        .ENDW
        L1:
        mov eax , 4
        mov ecx ,index
        mul ecx

        mov esi, btn_insert
        mov slot[eax], esi
        mov (Button PTR [esi]).in_slot, 1
        mov (Button PTR [esi]).callback, 2
        inc slot_count
        INVOKE slot_x, index
        INVOKE move_button, esi, eax, SLOT_Y, SLOT_MOVE_TIME
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
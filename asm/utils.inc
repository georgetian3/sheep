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
        .WHILE (ecx >= index)
            mov ebx, ecx
            inc ebx
            INVOKE slot_x, ebx
            INVOKE move_button, slot[ecx], eax, SLOT_Y, SLOT_MOVE_TIME
            mov esi, slot[ecx]
            mov slot[ebx], esi
            dec ecx
        .ENDW
        mov eax, btn_insert
        mov slot[index], eax
        mov (Button PTR [btn_insert]).in_slot, 1
        mov (Button PTR [btn_insert]).callback, match_slot
        inc slot_count
        INVOKE slot_x, index
        INVOKE move_button, btn_insert, eax, SLOT_Y, SLOT_MOVE_TIME
    ret
insert_slot ENDP

lose PROC
    String loseprint, "LOSE", 0ah, 0dh
    Print loseprint
lose ENDP


bmp_bg  DWORD ?

COMMENT `
void paint(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc_window = BeginPaint(hWnd, &ps);
    HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
    HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);
    HBITMAP blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
    SelectObject(hdc_memBuffer, blankBmp);
    SelectObject(hdc_loadBmp, bmp_bg);
    BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);
    BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);
    DeleteObject(blankBmp);
    DeleteDC(hdc_memBuffer);
    DeleteDC(hdc_loadBmp);
    EndPaint(hWnd, &ps);
}
`
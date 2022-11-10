overlap PROC uses ebx ecx edx esi edi a:DWORD, b:DWORD
    ; this is to check if two buttons/tiles are overlapped
    ; if they are overlapped by others, then change their status and update
    ; if they are no longer overlapped by others, then also change their status and update

    local pa:DWORD, pb:DWORD, a_x:DWORD, a_y:DWORD, b_x:DWORD, b_y:DWORD
    String pointpos , "point x = %x y= %x",0ah,0dh
    String overlapstr ,"overlap start",0ah,0dh

    INVOKE  crt_malloc, SIZEOF POINT
    mov     pa,eax
    INVOKE  crt_malloc, SIZEOF POINT
    mov     pb,eax


    mov edx , a
    mov esi , b 
    mov ebx, (Button PTR [edx]).k
    mov ecx, (Button PTR [esi]).k
    .IF (ebx <= ecx)
        mov eax, 0
        jmp F1
    .ENDIF
    mov ebx, (Button PTR [edx]).in_slot
    .IF (ebx)
        mov eax, 0
        jmp F1
    .ENDIF
    mov ebx, (Button PTR [edx]).hWnd
    mov ecx, pa

    ; call win_pos to check position
    INVOKE win_pos, ebx, ecx
    mov ebx, (Button PTR [esi]).hWnd
    mov ecx, pb
    INVOKE win_pos, ebx, ecx
    mov ecx, pb
    mov edx, (POINT PTR [ecx]).x
    mov ebx, TILE_WIDTH
    mov ecx, pa
    add ebx, (POINT PTR [ecx]).x
    .IF (edx >= ebx)
        mov eax, 0
        jmp F1
    .ENDIF
    mov ecx, pa
    mov ebx, (POINT PTR [ecx]).x
    sub ebx, TILE_WIDTH
    .IF (edx <= ebx)
        mov eax, 0
        jmp F1
    .ENDIF
    mov ecx, pb
    mov edx, (POINT PTR [ecx]).y
    mov ecx ,pa
    mov ebx, (POINT PTR [ecx]).y
    add ebx, TILE_HEIGHT
    .IF (edx >= ebx)
        mov eax, 0
        jmp F1
    .ENDIF
    mov ecx, pa
    mov ebx, (POINT PTR [ecx]).y
    sub ebx, TILE_HEIGHT
    .IF (edx <= ebx)
        mov eax, 0
        jmp F1
    .ENDIF

    mov eax, 1
    F1:
        PINVOKE crt_free , pa
        PINVOKE crt_free , pb
        ret
overlap ENDP

match_slot PROC uses eax ebx ecx edx esi edi
    ; called when dealing with problems in slot
    ; when there is still space, then buttons can be added to slot
    ; when three identical tiles/buttons are in slot, then delete them
    ; when exceeding the slot_count limit, then lose
    local count:DWORD, prev_type:DWORD
    String test_print , "test here" ,0ah,0dh
    String matching, "match slot", 0ah, 0dh
    String istr, "i=%d", 0ah, 0dh
    String jstr, "j=%d slotcount = %d", 0ah, 0dh

    Print offset matching
    Print offset newline
    mov count, 0
    mov prev_type, -1
    mov ecx, 0
    .WHILE (ecx < slot_count)
        AINVOKE AA1, OFFSET slot, ecx
        mov esi, [eax]
        mov edi, (Button PTR [esi]).type_
        .IF edi == prev_type
            inc     count
            Print offset istr,count
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
                        push    ebx
                        Print OFFSET here
                        mov     edx, ebx
                        sub     edx, count
                        AINVOKE slot_x, edx
                        mov     esi, eax
                        Print offset jstr,ebx,edx
                        AINVOKE AA1, OFFSET slot, ebx
                        mov     eax, [eax]
                        Print OFFSET hexStr, eax
                        Print offset newline
                        Print offset here
                        PINVOKE move_button, eax, esi, SLOT_Y, SLOT_MOVE_TIME
                        mov     edi, eax
                        AINVOKE AA1, OFFSET slot, edx
                        mov     [eax], edi
                        pop     ebx
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
    ;INVOKE print_buttons
    ret
match_slot ENDP

shuffle PROC uses eax ebx ecx edx esi edi arr:DWORD, n:DWORD
        ; this function is used to shuffle the deck when openning maps
        ; so that everytime players have different experiences
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
    ; this is for calculating the x-pos of the tile in slot
    mov ebx, SLOT_X_OFFSET
    add ebx, TILE_WIDTH
    mov eax, slot_index
    mul ebx
    add eax, SLOT_X
    ret
slot_x ENDP


print_eip PROC USES eax ebx ecx edx esi edi
    ; for debugging and testing purposes
        call next_
    next_:
        pop eax

        String  eipstr, "EIP: %h", 10, 13
        Print   OFFSET eipstr, eax
    ret
print_eip ENDP

insert_slot PROC uses eax ebx ecx edx esi edi btn_insert:DWORD, index_:DWORD
        ; called when need to insert a button/tile into the slot
        String  insertprint, "Insert Slot", 0ah, 0dh
        String  inWhile, "inWile", 0ah, 0dh
        String  stat , "slot count = %d ",0ah,0dh
        String  movstr,"move %d to %d",0ah,0dh
        String  stat1 ,"eax = %d btn = %x",0ah,0dh
        String  stat2 ,"insert btn to slot %d",0ah,0dh
        String  stat3 , "btn %x index %d",0ah,0dh
        Print   OFFSET insertprint
        ; Print OFFSET decStr, slot_count
        ; Print OFFSET newline
        ; Print OFFSET decStr, index_
        ; Print OFFSET newline
        ; Print OFFSET newline

        mov ecx, slot_count
        Print offset stat , slot_count
        dec ecx
        .IF ecx == 0FFFFFFFFh
            jmp L1
        .ENDIF      ;is this .IF needed?
        .WHILE (ecx >= index_)
            push ecx
            mov ebx, ecx
            inc ebx
            AINVOKE slot_x, ebx
            mov edi, eax
            mov eax, 4
            mul ecx
            mov esi, slot[eax]
            Print OFFSET movstr,ecx,ebx
            ;PINVOKE print_buttons
            mov (Button PTR [esi]).callback ,0
            PINVOKE move_button, esi, edi, SLOT_Y, SLOT_MOVE_TIME
            mov eax, 4
            mul ebx
            mov slot[eax],esi
            pop ecx
            dec ecx
            Print OFFSET stat3,slot[eax],ebx
        .ENDW
        ; Print OFFSET decStr, slot_count
        ; Print OFFSET newline
        ; Print OFFSET decStr, index_
        ; Print OFFSET newline
        ; Print OFFSET newline
        L1:
        
        mov esi, btn_insert
        ; PINVOKE print_buttons
        Print   OFFSET decStr ,eax
        Print   Offset here
        Print   OFFSET newline
        mov eax, 4
        mov ecx, index_
        mul ecx
        Print offset stat2,index_
        mov slot[eax], esi
        mov (Button PTR [esi]).in_slot, 1
        mov (Button PTR [esi]).callback, 2
        inc slot_count
        Print offset stat,slot_count
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



paint PROC uses eax ebx ecx edx esi edi hWnd:DWORD
    ; for painting the game
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
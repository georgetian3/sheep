lost PROC USES eax ebx ecx edx esi edi hWnd:DWORD
        invoke ExitProcess, 0
        ret
lost ENDP

win PROC USES eax ebx ecx edx esi edi hWnd:DWORD
        mov eax, stage
        .IF eax == 1
                INVOKE  delete_all_buttons
                mov     slot_count , 0
                AINVOKE build_map, hWnd, OFFSET MAP2
                mov     eax, total
                INVOKE  update
                inc     stage
                mov     total, eax
                ret
        .ELSE
                PINVOKE delete_all_buttons
                PINVOKE create_button, hWnd, TYPE_START, 320, 320, -1, 0, 0
                PINVOKE create_button, hWnd, TYPE_END, 320, 400, -1, 0, 0
        .ENDIF
        ret
win ENDP

update PROC USES eax ebx ecx edx esi edi
    LOCAL a:DWORD, b:DWORD, flag:DWORD
    String update_begin, "begin update", 0ah, 0dh
    String no_button, "no button found", 0ah, 0dh
    String update_end, "update end", 0ah, 0dh
    Print OFFSET update_begin
    mov ecx, 0
    .WHILE ecx < N_BUTTONS
        push ecx
        INVOKE get_button_at_index, ecx
        .IF eax == 0
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
            jmp S1
        .ENDIF
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
            .IF eax == 0
                jmp S2
            .ENDIF
            mov a, eax
            mov ebx, b
            cmp eax, ebx
            je S2
            INVOKE is_tile, a   ;note!!
            .IF eax == 0
                mov ebx, 1
            .ELSEIF eax == 1
                mov ebx, 0
            .ENDIF
            .IF ebx
                jmp S2
            .ENDIF
            mov ebx, a
            mov eax, (Button PTR [ebx]).moving
            .IF eax
                jmp S2
            .ENDIF

            mov ebx, b
            mov edx, a
            mov edi, (Button PTR [ebx]).in_slot
            AINVOKE overlap, edx, ebx
            .IF edi
                PINVOKE set_active, ebx, 0
                mov (Button PTR [ebx]).gray, 0
                mov flag, 0
            .ELSEIF eax
                PINVOKE set_active, ebx, 0
                mov (Button PTR [ebx]).gray, 1
                mov flag, 0
                mov edi, (Button PTR [ebx]).hWnd
                PINVOKE win_pos, edi, ebx
            .ENDIF
            S2:
                pop esi
                inc esi 
        .ENDW

        mov eax, flag
        .IF eax == 1
            mov ebx, b
            PINVOKE set_active, ebx, 1
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
            jmp S3
        .ENDIF
        .IF eax
            mov edx, (Button PTR [eax]).hWnd
            INVOKE InvalidateRect, edx, 0, 0
        .ENDIF
        S3:
        pop ecx
        inc ecx
    .ENDW

    Print OFFSET update_end
    ret
update ENDP

update_slot PROC USES eax ebx ecx edx esi edi btn1:DWORD
    local __insert_index:DWORD, __count:DWORD
        String update_slot_print, "Update Slot", 0ah, 0dh
        String insert_slot_adddr, "currently inserting: %x", 0ah, 0dh
        String updating_slot_btn, "currently update_slot while button addr: %x", 0ah, 0dh
        String updating_slot_type, "currently update_slot while button type: %x", 0ah, 0dh
        ; Print offset newline
        ; Print offset newline
        ; Print offset hexStr ,btn1
        mov     __count, 1
        mov     ecx, 0
        Print   OFFSET newline
        Print   OFFSET update_slot_print
        Print OFFSET insert_slot_adddr, btn1

        .WHILE ecx < slot_count

                AINVOKE AA1, OFFSET slot, ecx
                Print offset countstr ,eax,ecx
                mov     eax, [eax]
                mov     esi ,(Button PTR [eax]).type_
                Print OFFSET  updating_slot_btn, eax
                Print OFFSET updating_slot_type, esi
                mov     ebx , btn1
                mov     edi , (Button PTR [ebx]).type_
                .IF esi == edi
                        inc     __count
                .ELSEIF __count > 1
                    ;Print OFFSET MAP1
                    jmp S1
                .ENDIF
                inc ecx
        .ENDW

    S1:
        String update_slot_input, "Update: slot input info:", 0ah, 0dh
        Print offset update_slot_input
        Print offset updating_slot_btn, ebx
        Print OFFSET updating_slot_type, edi
        String ecx_print, "update_slot to inser_slot ecx: %d", 0ah, 0dh
        Print OFFSET ecx_print, ecx
        PINVOKE insert_slot, btn1, ecx
        mov     last_index , ecx 
        ret 
update_slot ENDP

handle_button_click PROC USES eax ebx ecx edx esi edi parent:DWORD,btn2:DWORD
        String handle_click, "handle_button_click", 10, 13
        Print OFFSET handle_click

        Print   OFFSET parStr, parent

        mov     esi, btn2
        push    esi
        AINVOKE is_tile, btn2
        .IF eax != 0
                mov     eax, (Button PTR [esi]).in_slot
                .IF eax != 0
                        ret
                .ENDIF
                mov     eax ,(Button PTR [esi]).hWnd
                PINVOKE win_pos, eax, esi
                mov     eax, (Button PTR [esi]).start_pos.x
                mov     last_x , eax
                mov     eax, (Button PTR [esi]).start_pos.y
                mov     last_y , eax
                mov     last_button, esi
                PINVOKE update_slot, esi
                PINVOKE print_buttons
                PINVOKE update
                ret
                mov     eax, slot_count
                mov     ecx , SLOT_SIZE
                String cur_slot, "current slot_count is: %d", 0ah, 0dh
                Print OFFSET cur_slot, eax
                .IF eax > ecx
                        mov     edx, parent
                        PINVOKE lost, edx
                .ENDIF
                mov eax ,total
                .IF eax == 0 
                        PINVOKE win, parent
                        mov     total , eax
                .ENDIF
        .ENDIF

        ret
        pop     esi
        mov     ebx, (Button PTR [esi]).type_
        .IF ebx == TYPE_START
                AINVOKE delete_all_buttons
                AINVOKE build_map, parent, OFFSET MAP1
                
                mov     total, eax
                AINVOKE create_button, parent, TYPE_UNDO, 500, 730, -1, TILE_WIDTH, TILE_HEIGHT
                PINVOKE update 
        .ELSEIF ebx == TYPE_UNDO
                mov     eax, last_button
                .IF eax != 0
                        mov     (Button PTR [ebx]).callback, 1
                        mov     (Button PTR [ebx]).in_slot, 0
                        PINVOKE move_button, eax, last_x, last_y, TIME_DOUBLE

                        inc total
                        mov last_button , 0
                        mov ecx , last_index
                        inc ecx
                        mov eax, slot_count
                        .WHILE ecx < eax
                            push ecx
                            mov eax, 4
                            mul ecx
                            mov esi, slot[eax]
                            mov (Button PTR [esi]).callback , 0
                            mov ebx, ecx
                            dec ebx 
                            INVOKE slot_x, ebx
                            INVOKE move_button, esi, eax, SLOT_Y, SLOT_MOVE_TIME              
                            mov eax, 4
                            mul ebx
                            mov edi, slot[eax]
                            mov edi, esi
                            pop ecx 
                            inc ecx
                        .ENDW
                        dec slot_count
                    .ENDIF
            .ELSEIF ebx == edx
                    Print   OFFSET MAP1
                    PINVOKE ExitProcess, 0
        .ENDIF
        PINVOKE print_buttons

handle_button_click ENDP

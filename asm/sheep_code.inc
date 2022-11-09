


lost PROC USES eax ebx ecx edx esi edi hWnd:DWORD
    invoke ExitProcess, 0
    ret
lost ENDP

win PROC USES eax ebx ecx edx esi edi hWnd:DWORD
    mov eax, stage
    .IF eax == 1
        INVOKE destroy_all_button
        mov slot_count , 0
        ;INVOKE build_map ,hWnd, OFFSET MAP1
        mov eax , total
        INVOKE update
        inc stage
        mov total , eax
        ret
    .ELSE
        INVOKE destroy_all_button
        INVOKE create_button , hWnd , TYPE_START ,320,320,-1,0, 0
        mov start_game , eax
        INVOKE create_button , hWnd , TYPE_END ,320,400,-1,0, 0
        mov end_game ,eax
    .ENDIF
    ret
win ENDP

update_slot PROC USES eax ebx ecx edx esi edi btn1:DWORD
    local __insert_index:DWORD, __count:DWORD
    ; Print offset newline
    ; Print offset newline
    ; Print offset hexStr ,btn1
    mov __count ,1
    mov ecx, 0
    .WHILE ecx < slot_count
        mov eax, 4
        mul ecx
        mov ebx, slot[eax]
        mov edx , (Button PTR [ebx]).type_
        mov esi , btn1
        mov edi , (Button PTR [esi]).type_
        .IF edx == edi
            inc __count
        .ELSEIF __count > 1
            ;Print OFFSET MAP1
            jmp S1
        .ENDIF
        inc ecx
    .ENDW
    S1:
        mov __insert_index, ecx
        ;Print OFFSET decStr , insert_index
        INVOKE insert_slot, btn1, __insert_index
        mov ebx , __insert_index
        mov last_index , ebx 
    ret 
update_slot ENDP

handle_button_click PROC USES eax ebx ecx edx esi edi parent:DWORD,btn2:DWORD
    LOCAL time_handle : real8
    String handle_click, "handle_button_click start", 0ah, 0dh
    String undo_print, "undo %x", 0ah, 0dh
    Print OFFSET handle_click

    INVOKE is_tile , btn2
    mov ebx , btn2
    mov ecx , start_game
    mov edx , end_game
    mov esi , undo_btn
    .IF eax != 0
        mov edi , (Button PTR [ebx]).in_slot
        .IF edi !=0
            ret
        .ENDIF
        mov edi , (Button PTR [ebx]).hWnd
        INVOKE win_pos , edi , ebx
        mov edx , (Button PTR [ebx]).start_pos.x
        mov last_x , edx
        mov edx , (Button PTR [ebx]).start_pos.y
        mov last_y , edx
        mov last_button , ebx
        INVOKE update_slot , ebx
        ret         ;checked here
        INVOKE update
        mov eax, slot_count
        mov ecx , SLOT_SIZE
        .IF  eax > ecx
            mov edx, parent
            INVOKE lost, edx
        .ENDIF
        mov eax ,total
        .IF eax == 0 
            INVOKE win, parent
            mov total , eax
        .ENDIF
    .ELSEIF ebx == ecx
        INVOKE destroy_all_button
        ;INVOKE build_map,parent ,MAP2
        ;mov total , eax
        Print offset undo_print, esi
        INVOKE create_button, parent, TYPE_UNDO, 500, 730, -1, TILE_WIDTH, TILE_HEIGHT
        mov undo_btn , eax 

        INVOKE update 
    .ELSEIF ebx == esi
        mov eax, last_button
        .IF eax != 0
            mov (Button PTR [ebx]).callback ,1
            mov (Button PTR [ebx]).in_slot , 0
            INVOKE move_button, eax, last_x, last_y, TIME_DOUBLE

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
        Print OFFSET MAP1
        invoke ExitProcess, 0
    .ENDIF
handle_button_click ENDP

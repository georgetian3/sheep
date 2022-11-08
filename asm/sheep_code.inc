
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
        not eax
        .IF eax
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
            .IF eax == -1
                jmp S2
            .ENDIF
            mov a, eax
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
            cmp eax, ebx
            je S2
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
                mov esi, b[0]
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


lost PROC USES eax ebx ecx edx esi edi hWnd:DWORD
    invoke ExitProcess, 0
lost ENDP

win PROC USES eax ebx ecx edx esi edi hWnd:DWORD
    mov eax, stage
    .IF eax == 1
        INVOKE destroy_all_button
        mov slot_count , 0
        INVOKE build_map ,hWnd, OFFSET MAP1
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
    local insert_index:DWORD,count:DWORD
    mov count ,1
    mov ecx, 0
    .WHILE ecx < slot_count
        push ecx
        mov eax,4
        mul ecx
        mov ebx,slot[eax]
        mov edx , (Button PTR [ebx]).type_
        mov esi , btn1
        mov edi , (Button PTR [esi]).type_
        .IF edx == edi
            inc count
        .ELSEIF count > 1
            jmp S1
        .ENDIF
        pop ecx
        inc ecx
    .ENDW
    S1:
        INVOKE insert_slot ,btn1,insert_index
        mov eax , insert_index
        mov  last_index , eax 
    ret 
update_slot ENDP

handle_button_click PROC USES eax ebx ecx edx esi edi parent:DWORD,btn1:DWORD
    LOCAL time_double : real8
    INVOKE is_tile , btn1
    mov ebx , btn1
    mov ecx , start_game
    mov edx , end_game 
    mov esi , undo_btn
    .IF eax != 0
        mov esi , btn1
        mov edi , (Button PTR [esi]).in_slot
        .IF edi !=0
            ret
        .ENDIF
        mov esi , btn1
        mov ebx , (Button PTR [esi]).hWnd
        mov ecx , btn1[0]
        INVOKE win_pos , ebx , ecx
        mov esi , btn1
        mov ebx , (Button PTR [esi]).start_pos.x
        mov ecx , (Button PTR [esi]).start_pos.y
        mov last_x , ebx
        mov last_y , ecx
        mov last_button , esi
        INVOKE update_slot , btn1
        INVOKE update
        mov eax, slot_count
        mov ebx , SLOT_SIZE
        .IF  eax > ebx
            INVOKE lost , parent
        .ENDIF
        mov eax ,total
        .IF eax == 0 
            INVOKE win,parent
            mov total , eax
        .ENDIF
    .ELSEIF ebx == ecx
        INVOKE destroy_all_button
        INVOKE create_button , parent , TYPE_UNDO,500,730,-1,TILE_WIDTH,TILE_HEIGHT
        mov undo_btn , eax 
        INVOKE build_map,parent ,MAP2
        mov total , eax
        INVOKE update 
    .ELSEIF ebx == edx
        mov eax , last_button
        .IF eax != 0
            mov esi , btn1
            mov (Button PTR [esi]).callback ,1
            mov (Button PTR [esi]).in_slot , 0
            mov ebx ,last_x
            mov ecx ,last_y
            INVOKE move_button ,eax,last_x,last_y,TIME_DOUBLE

            inc total
            mov last_button , 0
            mov ecx , last_index+1
            mov eax ,slot_count
            .WHILE ecx < eax
                push ecx
                mov eax,4
                mul ecx
                mov esi , slot[eax]
                mov (Button PTR [esi]).callback , 0
                mov ebx , ecx
                dec ebx 
                INVOKE slot_x , ebx
                INVOKE move_button ,esi,eax,SLOT_Y,SLOT_MOVE_TIME              
                mov eax , 4
                mul ebx
                mov edi , slot[eax]
                mov edi , esi
                pop ecx 
                inc ecx
            .ENDW
            dec slot_count
        .ENDIF
    .ELSEIF ebx == esi
        invoke ExitProcess, 0
    .ENDIF
handle_button_click ENDP
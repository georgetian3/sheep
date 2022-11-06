WINDOW_WIDTH equ 768
WINDOW_HEIGHT equ 1024
STAGE_STARTMENU equ 0
STAGE_1 equ 1
UNIT_SIZE_X equ 80
UNIT_SIZE_Y equ 73
BUTTON_STARTGAME equ 1001
BUTTON_STARTGAME_WIDTH equ 212
BUTTON_STARTGAME_HEIGHT equ 76

MAP_WIDTH equ 8
MAP_LENGTH equ 8
MAP_HEIGHT equ 2

SLOT_X equ 100
SLOT_X_OFFSET equ 20
SLOT_Y equ 809
SLOT_SIZE equ 7
MATCH_COUNT equ 3

SLOT_MOVE_TIME equ 0.5

slot_count DWORD 0
slot DWORD SLOT_SIZE DUP(0)

Stage Struct
    id DWORD ?
    timeCountDown DWORD ?
    bg DWORD ?
Stage ENDS

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
                    push eax
                    mov  eax, 0.2
                    INVOKE move_button, slot[edx], eax, SLOT_Y, eax; SLOT_MOVE_TIME
                    pop eax
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

insert_slot PROC USES eax ebx ecx edx esi edi btn:DWORD, index:DWORD
    String insertprint, "Insert Slot", 0ah, 0dh
    Print insertprint
    mov ecx, slot_count
    dec ecx
    .WHILE (ecx >= index)
        
        dec ecx
    .ENDW
insert_slot ENDP

lose PROC
    String loseprint, "LOSE", 0ah, 0dh
    Print loseprint
lose ENDP

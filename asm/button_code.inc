__search_hwnd PROC uses ebx ecx edx esi edi hWnd:DWORD

        COMMENT !
            hWnd: hWnd of the button
            Returns: in eax, pointer to element
            containing button with hWnd in __buttons,
            -1 if not found
        !
        mov     eax, OFFSET __buttons
        lea     ebx, [eax + N_BUTTONS * SIZEOF DWORD]
        mov     ecx, hWnd
        .WHILE eax < ebx
                mov     edx, [eax]
                .IF edx != 0
                        mov     edx, (Button PTR [edx]).hWnd
                        .IF ecx == edx
                                ret
                        .ENDIF
                .ENDIF
                add     eax, SIZEOF DWORD
        .ENDW
        mov     eax, 0
        ret
__search_hwnd ENDP

win_pos PROC uses eax ebx ecx edx esi edi hWnd:DWORD, point:DWORD
        LOCAL rect:RECT
        INVOKE  GetWindowRect, hWnd, ADDR rect
        INVOKE  GetParent, hWnd
        mov     ebx, eax
        INVOKE  MapWindowPoints, HWND_DESKTOP, ebx, ADDR rect, 2
        mov     ecx, rect.left
        mov     edx , point
        mov     (POINT PTR [edx]).x, ecx
        mov     ecx, rect.top
        mov     (POINT PTR [edx]).y, ecx
        ret
win_pos ENDP

id_to_hwnd PROC USES ebx ecx edx esi edi parent:DWORD, id:DWORD
        INVOKE  GetDlgItem, parent, id
        ret
id_to_hwnd ENDP

get_button_at_index PROC uses ebx ecx edx esi edi index:DWORD
        String err, "get_button bad index", 0ah, 0dh
        .IF     index < 0
                Print   OFFSET err
                INVOKE  ExitProcess, 1
        .ENDIF
        .IF index >= N_BUTTONS
                Print   OFFSET err
                INVOKE  ExitProcess, 1
        .ENDIF
        AINVOKE AA1, OFFSET __buttons, index
        mov     eax, [eax]
        ret
get_button_at_index ENDP

get_button PROC uses ebx ecx edx esi edi hWnd:DWORD
        AINVOKE __search_hwnd, hWnd
        .IF eax == 0
                ret
        .ENDIF
        mov     eax, [eax]
        ret
get_button ENDP

create_button PROC uses ebx ecx edx esi edi parent:DWORD, type_:DWORD, x:DWORD, y:DWORD, k:DWORD, wd:DWORD, ht:DWORD
        local new_btn:DWORD, index:DWORD

        String  createbutton, "create_button", 10, 13
        ;Print   OFFSET createbutton

        String empty_struct, "No Empty Button Struct???", 0ah, 0dh
        String window_failed, "CreateWindow failed", 0ah, 0dh
        String malloc_failed, "malloc failed", 10, 13
        String cbf, "CreateButton failed", 10, 13

        Print OFFSET hexStr, __button_count
        Print OFFSET newline
        

        .IF __button_count >= N_BUTTONS
                Print   OFFSET cbf
                INVOKE  ExitProcess, 1
        .ENDIF

        INVOKE  crt_malloc, SIZEOF Button
        .IF eax == 0
                Print   OFFSET malloc_failed
                INVOKE  ExitProcess, 1
        .ENDIF
        mov     esi, eax        ; esi holds ptr to new button for rest of proc
        
        ; finding a free id
        AINVOKE id_to_hwnd, parent, __id_count
        .WHILE eax != 0
                ;Print   OFFSET hexStr, __id_count
                ;Print   OFFSET newline
                inc     __id_count
                AINVOKE id_to_hwnd, parent, __id_count
        .ENDW

        ; create a hWnd
        String  cwstr, "CreateWindow x %d y %d wd %d ht %d parent %x id %d", 10, 13
        Print   OFFSET cwstr, x, y, wd, ht, parent, __id_count
        AINVOKE CreateWindowEx, 0, ADDR btnStr, 0, \
                WS_CHILD or WS_VISIBLE or BS_BITMAP or BS_OWNERDRAW, \
                x, y, wd, ht, parent, __id_count, 0, 0
        .IF eax == 0
                Print OFFSET window_failed
                INVOKE  ExitProcess, 1
        .ENDIF


        mov     (Button PTR [esi]).hWnd, eax
        mov     eax, k
        mov     (Button PTR [esi]).k, eax
        mov     eax, type_
        mov     (Button PTR [esi]).type_, eax


        mov     (Button PTR [esi]).gray, 0
        mov     (Button PTR [esi]).moving, 0
        mov     (Button PTR [esi]).active, 1
        mov     (Button PTR [esi]).in_slot, 0

        ; search for empty btn
        mov     edi, 0
        AINVOKE AA1, OFFSET __buttons, edi
        mov     ebx, [eax]
        .WHILE  ebx != 0
                inc     edi
                AINVOKE AA1, OFFSET __buttons, edi
                mov     ebx, [eax]
                .IF edi >= N_BUTTONS
                        Print   OFFSET empty_struct
                        PINVOKE  ExitProcess, 0
                .ENDIF
        .ENDW

        inc     __button_count

        mov     [eax], esi
        mov     eax, esi
    
        ret

create_button ENDP


print_buttons PROC USES eax ebx ecx edi
        mov     ecx, 0
        String  separator1, 10, 13, "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv", 10, 13
        String  separator2, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^", 10, 13, 10, 13

        String  pbtn, "print_buttons", 10, 13

        Print   OFFSET separator1
        Print   OFFSET pbtn

        .WHILE ecx < N_BUTTONS
                AINVOKE get_button_at_index, ecx
                .IF eax == 0
                        jmp     continue
                .ENDIF
                String  binfo, "btn i %02d ptr %07x hWnd %07x type %02d gray %d active %d in_slot %d", 10, 13
                Print   OFFSET binfo, ecx, eax, (Button PTR [eax]).hWnd, (Button PTR [eax]).type_, (Button PTR [eax]).gray, (Button PTR [eax]).active, (Button PTR [eax]).in_slot
            continue:
                inc     ecx
        .ENDW 

        Print   OFFSET separator2
        ret
print_buttons ENDP



delete_button PROC uses eax ebx ecx edx esi edi hWnd:DWORD
        PINVOKE print_buttons
        String  delbtn, "Deleting button hwnd %x", 10, 13
        Print   OFFSET delbtn, hWnd


        String del_hwnd_err, "Delete hwnd not found", 0ah, 0dh

        AINVOKE __search_hwnd, hWnd
        .IF eax == 0
                Print   OFFSET del_hwnd_err
                INVOKE  ExitProcess, 1
        .ENDIF
        mov     ebx, [eax]
        mov     ecx, (Button PTR [ebx]).hWnd
        PINVOKE DestroyWindow, ecx
        PINVOKE crt_free, ebx
        mov     ebx, 0
        mov     [eax], ebx
        dec     __button_count
        ret
delete_button ENDP

delete_all_buttons PROC uses eax ebx ecx edx esi edi
        String  dab, "Delete all buttons", 10, 13
        Print   OFFSET dab

        mov ecx, 0
        .WHILE ecx < N_BUTTONS
                AINVOKE get_button_at_index, ecx
                .IF eax != 0
                        mov     eax, (Button PTR [eax]).hWnd
                        INVOKE  delete_button, eax
                .ENDIF
        
                inc ecx
        .ENDW
        ret
delete_all_buttons ENDP

draw_button PROC uses eax ebx ecx edx edi esi parent:DWORD, id:DWORD, dis:DWORD
        String  drawbutton, "draw_button", 10, 13
        Print   OFFSET drawbutton
        PINVOKE print_buttons
        ; struct Button* btn = get_button(id_to_hwnd(parent, id));

        AINVOKE id_to_hwnd, parent, id
        .IF eax == 0
                String  ith, "id_to_hwnd failed", 10, 13
                Print   OFFSET ith
        .ENDIF
        AINVOKE get_button, eax
        .IF eax == 0    ; if (btn == 0) 
                String  notfound, "Draw button not found", 10, 13
                Print   OFFSET notfound
        .ENDIF
        mov     esi, eax        ; esi will hold btn for the rest of this proc



        mov     ebx, (Button PTR [esi]).gray
        .IF ebx == 0
                mov     ebx, 1
        .ELSE
                mov     ebx, 0
        .ENDIF
        mov     ecx, (Button PTR [esi]).type_

        String  gtb, "gray %d type %d", 10, 13
        ;Print   OFFSET gtb, ebx, ecx

        AINVOKE AA2, ADDR bitmaps, ebx, ecx

        String  gbm, "got bitmap: %x", 10, 13
        ;Print   OFFSET gbm, eax

        
        ;ret
        mov     eax, [eax]

        mov     edi, dis
        mov     esi, (DRAWITEMSTRUCT PTR [edi]).hdc

        AINVOKE DrawStateW, esi, 0, 0, eax, 0, 0, 0, 0, 0, DST_BITMAP
        .IF eax == 0
                String  drawstate, "Draw state 0", 10, 13
                Print   OFFSET drawstate
                INVOKE  ExitProcess, 0
        .ENDIF

        ret
draw_button ENDP

set_active PROC USES eax ebx ecx edx esi edi btn_active:DWORD, active:DWORD
        mov     edi, btn_active
        mov     esi, active
        mov     (Button PTR [edi]).active, esi
        INVOKE  EnableWindow, (Button PTR [edi]).hWnd, active
    ret
set_active ENDP

is_tile PROC uses esi btn_istile:DWORD

    mov esi, btn_istile
    mov esi, (Button PTR [esi]).type_

    mov     eax, 0
    .IF esi < 0
            ret
    .ENDIF
    .IF esi >= N_TILE_TYPES
            ret
    .ENDIF

    mov     eax, 1
    ret

is_tile ENDP
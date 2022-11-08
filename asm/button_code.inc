__search_struct PROC uses ebx ecx edx esi edi btn:DWORD
        mov ecx, 0
        .WHILE (ecx < N_BUTTONS)
                mov edx, 4
                mul ecx
                mov eax, btn
                .IF __buttons[edx] == eax
                        mov eax, edx    ;if want to have exact loca, then mov eax, edx
                        jmp F1
                .ENDIF
                inc ecx
        .ENDW
        mov eax, -1
        F1:
                ret
__search_struct ENDP

__search_hwnd PROC uses ebx ecx edx esi edi hWnd:DWORD
        mov     ebx, hWnd
        mov     eax, 0
        .WHILE eax < N_BUTTONS
                mov     esi, __buttons[ecx * SIZEOF DWORD]
                .IF esi != 0
                        mov     ecx, (Button PTR [esi]).hWnd
                        .IF ebx == ecx
                                ret
                        .ENDIF
                .ENDIF
                inc     eax
        .ENDW
        mov     eax, -1
        ret
__search_hwnd ENDP

win_pos PROC uses eax ebx ecx edx esi edi hWnd:DWORD, point:DWORD
        LOCAL rect:RECT
        INVOKE  GetWindowRect, hWnd,ADDR rect
        INVOKE  GetParent, hWnd
        mov     ebx, eax
        INVOKE  MapWindowPoints, HWND_DESKTOP, ebx,ADDR rect, 2
        mov     ecx, rect.left
        mov     (POINT PTR [point]).x, ecx
        mov     ecx, rect.top
        mov     (POINT PTR [point]).y, ecx
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
        lea     eax, [index + eax * SIZEOF DWORD]
        mov     eax, __buttons[eax]
        ret
get_button_at_index ENDP

get_button PROC uses ebx ecx edx esi edi hWnd:DWORD
        INVOKE  __search_hwnd, hWnd
        .IF eax == -1
                mov eax, 0
                ret
        .ENDIF
        mov     eax, __buttons[eax]
        ret
get_button ENDP

get_free_id PROC uses eax ebx ecx edx esi edi parent:DWORD
        INVOKE  id_to_hwnd, parent, __id_count
        .WHILE eax != 0
                inc     __id_count
                INVOKE  id_to_hwnd, parent, __id_count
        .ENDW
        mov     eax, __id_count
        ret
get_free_id ENDP

create_button PROC uses ebx ecx edx esi edi parent:DWORD, type_:DWORD, x:DWORD, y:DWORD, k:DWORD, wd:DWORD, ht:DWORD
    local new_btn:DWORD, index:DWORD

        String  createbutton, "create_button", 10, 13
        Print   OFFSET createbutton

        String empty_struct, "No Empty Button Struct???", 0ah, 0dh
        String window_failed, "CreateWindow failed", 0ah, 0dh
        String btn, "button"
        String malloc_failed, "malloc failed", 10, 13
        String btnErr, "CreateButton failed", 10, 13

        .IF __button_count >= N_BUTTONS
                Print   OFFSET btnErr
                ret
        .ENDIF


        INVOKE  __search_struct, 0
        mov     index, eax
        .IF (index == -1)
                Print   empty_struct
        .ENDIF

        mov     eax, 4
        mul     index
        mov     index, eax


        INVOKE  crt_malloc, SIZEOF Button
        .IF eax == 0
                Print   OFFSET malloc_failed
        .ENDIF
        mov     new_btn, eax
        mov     edi, eax
        mov     [edi], eax

        INVOKE  get_free_id, parent
        Print   OFFSET hexStr, eax
        Print   OFFSET newline
        INVOKE  CreateWindowEx,0,
                ADDR btn, 0,
                WS_CHILD or WS_VISIBLE or BS_BITMAP or BS_OWNERDRAW,
                x,y,wd,ht,parent,eax,0, 0
        ret

        mov     edi, new_btn

        .IF eax == 0
                Print window_failed
        .ENDIF

        mov     (Button PTR [edi]).hWnd, eax
        mov     eax, k
        mov     (Button PTR [edi]).k, eax
        mov     eax, type_
        mov     (Button PTR [edi]).type_, eax
        mov     (Button PTR [edi]).gray, 0
        mov     (Button PTR [edi]).moving, 0
        mov     (Button PTR [edi]).active, 1
        mov     (Button PTR [edi]).in_slot, 0

        mov     edi, OFFSET __buttons
        add     edi, index
        mov     eax, new_btn
        mov     [edi], eax
        inc     __button_count        

    ret
create_button ENDP

__delete_button PROC uses eax ebx ecx edx esi edi ind:DWORD
        mov     eax, 4
        mul     ind
        mov     ebx, (Button PTR [__buttons[eax]]).hWnd
        INVOKE  DestroyWindow, ebx
        INVOKE  crt_free, __buttons[eax]
        mov     __buttons[ind], 0
        dec     __button_count
        ret
__delete_button ENDP


delete_button PROC uses eax hWnd:DWORD
        local index:DWORD
        String del_hwnd_err, "Delete hwnd not found", 0ah, 0dh
        INVOKE  __search_hwnd, hWnd
        .IF eax == -1
                Print   OFFSET del_hwnd_err
                INVOKE  ExitProcess, 1
        .ENDIF
        INVOKE  __delete_button, eax
        ret
delete_button ENDP

draw_button PROC uses eax ebx ecx edx edi esi parent:DWORD, id:DWORD, dis:DWORD
        String  drawbutton, "draw_button", 10, 13
        ;Print   OFFSET drawbutton
        ; struct Button* btn = get_button(id_to_hwnd(parent, id));
        INVOKE  id_to_hwnd, parent, id

        .IF eax == 0
                String  ith, "id_to_hwnd failed", 10, 13
                Print   OFFSET ith
        .ENDIF

        INVOKE  get_button, eax
        mov     esi, eax        ; esi will hold btn for the rest of this proc

        .IF eax == 0    ; if (btn == 0) 
                String  notfound, "Draw button not found", 10, 13
                Print   OFFSET notfound
        .ENDIF

        String  bm, "bitmap", 10, 13

        mov     ebx, (Button PTR [esi]).gray
        .IF ebx == 0
                mov     ebx, 1
        .ELSE
                mov     ebx, 0
        .ENDIF

        mov     ecx, (Button PTR [esi]).type_


        INVOKE  AA2, ADDR bitmaps, ebx, ecx
        mov     eax, [eax]

        mov     edi, dis
        mov     esi, (DRAWITEMSTRUCT PTR [edi]).hdc

        INVOKE  DrawStateW, esi, 0, 0, eax, 0, 0, 0, 0, 0, DST_BITMAP
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

is_tile PROC uses ebx ecx edx esi edi btn_istile:DWORD
    mov edi, btn_istile
    mov esi, (Button PTR [edi]).type_
    .IF esi > 0
        jge S1
    .ENDIF
    mov edx, 0
    S1:
    mov ebx, N_TILE_TYPES
    .IF esi > ebx
        jl S2
    .ENDIF
    mov edx, 0
    S2:
    mov edx, 1
    mov eax, edx
    ret
is_tile ENDP
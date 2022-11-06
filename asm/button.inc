
STATE_DISABLE equ 0
STATE_ENABLE equ 1
STATE_MOVING equ 2
STATE_HIDDEN equ 3
N_STATES equ 4


Button STRUCT
    hWnd DWORD ?

    active BYTE ?
    moving BYTE ?
    gray BYTE ?
    in_slot BYTE ?
    callback DWORD ?
    type_ DWORD ?
    id DWORD ?
    k DWORD ?
    start_time DWORD ?
    dxpf REAL8 ?
    dypf REAL8 ?
    frame DWORD ?
    frames DWORD ?
    start_pos POINT <>
Button ENDS

__search_struct PROC uses ebx ecx edx esi edi btn:DWORD
    mov ecx, 0
    .WHILE (ecx < N_BUTTONS)
        mov edx, 4
        mul ecx
        mov eax, btn
        .IF (__buttons[edx] == eax)
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
    mov ecx, 0
    .WHILE (ecx < N_BUTTONS)
        mov edx, 4
        mul ecx
        mov eax, hWnd
        mov ebx, (Button PTR [__buttons[edx]]).hWnd
        .IF (ebx == eax)
            .IF (__buttons[edx])
                mov eax, edx
                jmp F1
            .ENDIF
        .ENDIF
        inc ecx
    .ENDW
    mov eax, -1
    F1:
        ret
__search_hwnd ENDP

win_pos PROC uses eax ebx ecx edx esi edi hWnd:DWORD, point:DWORD
    local rect:DWORD
    INVOKE GetWindowRect, hWnd, rect
    INVOKE GetParent, hWnd
    mov ebx, eax
    INVOKE MapWindowPoints, HWND_DESKTOP, ebx, rect, 2
    mov ecx, (RECT PTR [rect]).left
    mov (POINT PTR [point]).x, ecx
    mov ecx, (RECT PTR [rect]).top
    mov (POINT PTR [point]).y, ecx
    ret
win_pos ENDP

id_to_hwnd PROC uses ebx ecx edx esi edi parent:DWORD, id:DWORD
    INVOKE GetDlgItem, parent, id
    ret
id_to_hwnd ENDP

get_button_index PROC uses ebx ecx edx esi edi index:DWORD
    String err, "get_button bad index", 0ah, 0dh
    .IF (index < 0)
        mov eax, 1
    .ENDIF
    mov ebx, index
    .IF (ebx >= N_BUTTONS)
        mov eax, 1
    .ENDIF
    .IF (eax)
        CheckError addr err
    .ENDIF
    mov eax, 4
    mul index
    mov eax, __buttons[edx]
    ret
get_button_index ENDP

get_button PROC uses ebx ecx edx esi edi hWnd:DWORD
    INVOKE __search_hwnd, hWnd
    mov ebx, eax
    .IF (ebx == -1)
        mov eax, 0
        jmp F1
    .ENDIF
    mov eax, __buttons[ebx]
    F1:
        ret
get_button ENDP


create_button PROC uses ebx ecx edx esi edi parent:DWORD, type_:DWORD, x:DWORD, y:DWORD, k:DWORD, wd:DWORD, ht:DWORD
    local new_btn:DWORD, index:DWORD
    String empty_struct, "No Empty Button Struct???", 0ah, 0dh
    String window_failed, "CreateWindow failed", 0ah, 0dh
    String btn, "BUTTON"
    String btnErr, "CreateButton failed", 10, 13

        
        String  hexStr, "hexStr %x", 10, 13
        String  decStr, "decStr %d", 10, 13

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

        Print   OFFSET decStr, index
        
        INVOKE  id_to_hwnd, parent, __id_count
        .WHILE eax
                inc     __id_count
                INVOKE  id_to_hwnd, parent, __id_count
        .ENDW

        INVOKE  crt_malloc, SIZEOF Button
        .IF eax == 0
                Print   OFFSET malloc_failed
        .ENDIF
        mov     new_btn, eax
        Print   OFFSET hexStr, new_btn
        mov     edi, eax
        mov     [edi], eax
        INVOKE  CreateWindowEx,0,
                ADDR btn, 0,
                WS_CHILD or WS_VISIBLE or BS_BITMAP or BS_OWNERDRAW,
                x,y,wd,ht,parent,__id_count,
                0, 0

        .IF eax == 0
                Print window_failed
        .ENDIF

        mov     (Button PTR [new_btn]).hWnd, eax
        mov     eax, k
        mov     (Button PTR [new_btn]).k, eax
        mov     eax, type_
        mov     (Button PTR [new_btn]).type_, eax
        mov     (Button PTR [new_btn]).active, 1
        mov     (Button PTR [new_btn]).gray, 0
        mov     (Button PTR [new_btn]).moving, 0
        mov     (Button PTR [new_btn]).in_slot, 0
        ;lea eax, new_btn
        mov     edi, OFFSET __buttons
        add     edi, index
        mov     eax, new_btn
        mov     [edi], eax
        inc     __button_count
        Print   OFFSET here

    ret
create_button ENDP

__delete_button PROC uses eax ebx ecx edx esi edi ind:DWORD
    mov eax, 4
    mul ind
    mov ebx, (Button PTR [__buttons[eax]]).hWnd
    INVOKE DestroyWindow, ebx
    INVOKE crt_free, __buttons[eax]
    mov __buttons[ind], 0
    inc __button_count
    ret
__delete_button ENDP


delete_button_struct PROC uses eax ebx ecx edx esi edi btn_del:DWORD
    local index:DWORD
    String del_err, "Delete Struct not found", 0ah, 0dh
    INVOKE __search_struct, btn_del
    mov index, eax
    .IF (index == -1)
        CheckError addr del_err
    .ENDIF
    INVOKE __delete_button, index
    ret
delete_button_struct ENDP

delete_button_hwnd PROC uses eax ebx ecx edx esi edi hWnd:DWORD
    local index:DWORD
    String del_hwnd_err, "Delete hwnd not found", 0ah, 0dh
    INVOKE __search_hwnd, hWnd
    mov index, eax
    .IF (index == -1)
        CheckError addr del_hwnd_err
    .ENDIF
    INVOKE __delete_button, index
    ret
delete_button_hwnd ENDP

draw_button PROC uses eax ebx ecx edx edi esi parent:DWORD, id:DWORD, dis:DWORD
    local btn_draw:DWORD
    String error, "Draw button not found", 0ah, 0dh
    String msg_draw, "Drawing button ptr %d hwnd %d state %u type %d", 0ah, 0dh
    INVOKE id_to_hwnd, parent, id
    mov ebx, eax
    INVOKE get_button, ebx
    mov btn_draw, eax
    .IF (eax == 0)
        CheckError error
    .ELSE
        ; pushad
        ; INVOKE crt_printf, offset msg, btn, btn.hWnd, btn.state, btn.type
        ; popad
    .ENDIF
    lea ebx, (Button PTR [btn_draw]).gray
    not ebx
    mov esi, dword ptr bitmaps[ebx]
    mov edx, (Button PTR [btn_draw]).type_
    mov eax, 0
    .WHILE (edx >= 0)
        add eax, 4
        dec edx
    .ENDW
    mov ecx, [esi + eax]
    mov eax, (DRAWITEMSTRUCT PTR [dis]).hdc
    INVOKE DrawStateW, eax, 0, 0, ecx, 0, 0, 0, 0, 0, DST_BITMAP
    ret
draw_button ENDP

set_active PROC USES eax ebx ecx edx esi edi btn_active:DWORD, active:BYTE
    mov eax, 0
    mov al, active
    mov btn_active, eax
    mov ebx, (Button PTR [btn_active]).hWnd
    INVOKE  EnableWindow, ebx, active
    ret
set_active ENDP

is_tile PROC uses ebx ecx edx esi edi btn_istile:DWORD
    mov eax, (Button PTR [btn_istile]).type_
    mov ebx, 0
    cmp eax, ebx
    jge S1
    mov edx, 0
    S1:
    mov edx, 1
    mov ebx, N_TILE_TYPES
    cmp eax, ebx
    jl S2
    mov ecx, 0
    S2:
    mov ecx, 1
    and edx, ecx
    mov eax, edx
    ret
is_tile ENDP
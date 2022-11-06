N_BUTTONS equ 100

STATE_DISABLE equ 0
STATE_ENABLE equ 1
STATE_MOVING equ 2
STATE_HIDDEN equ 3
N_STATES equ 4


Button STRUCT
    hWnd DWORD ?
    state DWORD ?
    type DWORD ?
    id DWORD ?
    in_slot BYTE ?
    start_pos BYTE ?
    start_time DWORD ?
    dxpf REAL8 ?
    dypf REAL8 ?
    frame DWORD ?
    frames DWORD ?
Button ENDS

__search_struct PROC uses ebx ecx edx esi edi btn:DWORD
    mov ecx, 0
    .WHILE (ecx < N_BUTTONS)
        mov edx, ecx
        mul 4
        .IF (__buttons[edx] == btn)
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
        .IF (__buttons[edx].hWnd == hWnd)
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

win_pos ENDP

id_to_hwnd PROC uses ebx ecx edx esi edi parent:DWORD, id:DWORD
    INVOKE GetDlgItem, parent, id
    ret
id_to_hwnd ENDP

get_button_index PROC uses ebx ecx edx esi edi index:DWORD
    String err, "get_button bad index", 0ah, 0dh
    .IF (index < 0 or N_BUTTONS < index)
        INVOKE crt_printf, offset err
        INVOKE ExitProcess, 1
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
get_button ENDP


CreateButton PROC uses ebx, ecx, edx, esi, edi parent:DWORD, state:DWORD, type:DWORD, x:DWORD, y:DWORD, k:DWORD, wd:DWORD, ht:DWORD
    String moving_error, "Don't create moving button", 0ah, 0dh
    String empty_struct, "No Empty Button Struct???", 0ah, 0dh
    String window_failed, "CreateWindow failed", 0ah, 0dh
    String btn, "BUTTON"
    String btnErr, "CreateButton failed", 10, 13

    local new_btn:Button, index:DWORD
    .IF (state == STATE_MOVING)
        INVOKE crt_printf, offset moving_error
        INVOKE ExitProcess, 1
    .ENDIF
    .if(__button_count >= N_BUTTONS)
        mov eax, 0
        jmp F1
    .ENDIF
    INVOKE __search_struct, 0
    mov index, eax
    .IF (index == -1)
        INVOKE crt_printf, offset empty_struct
        INVOKE ExitProcess, 1
    .ENDIF
    
    INVOKE id_to_hwnd, parent, __id_count
    mov ebx, eax
    .WHILE(ebx)
        inc __id_count
        INVOKE id_to_hwnd, parent, __id_count
        mov ebx, eax
    .ENDW

    INVOKE crt_malloc, SIZEOF Button

    mov new_btn, eax
    INVOKE CreateWindowEx,0,
            ADDR btn, 0,
            WS_CHILD or WS_VISIBLE or BS_BITMAP or BS_OWNERDRAW,
            x,y,wd,ht,parent,__id_count,
            0, 0
    mov new_btn.hWnd, eax

    .IF (new_btn.hWnd == 0)
        INVOKE crt_printf, offset window_failed
        INVOKE ExitProcess, 1
    .ENDIF

    mov new_btn.k, k
    mov new_btn.state, state
    mov new_btn.in_slot, in_slot
    mov new_btn.type, type
    mov __buttons[index], new_btn
    inc __button_count
    mov eax, new_btn
    F1:
        ret
CreateButton ENDP

__delete_button PROC uses eax ebx ecx edx esi edi ind:DWORD
    INVOKE DestroyWindow, __buttons[ind].hWnd
    INVOKE crt_free, __buttons[ind]
    mov __buttons[ind], 0
    inc __button_count
    ret
__delete_button ENDP


delete_button_struct PROC uses eax ebx ecx edx esi edi btn:DWORD
    local index:DWORD
    String err, "Delete Struct not found", 0ah, 0dh
    INVOKE __search_struct, btn
    mov index, eax
    .IF (index == -1)
        INVOKE crt_printf, offset err
        INVOKE ExitProcess, 1
    .ENDIF
    INVOKE __delete_button, index
    ret
delete_button_struct ENDP

delete_button_hwnd PROC uses eax ebx ecx edx esi edi hWnd:DWORD
    local index:DWORD
    String err, "Delete hwnd not found", 0ah, 0dh
    INVOKE __search_hwnd, hWnd
    mov index, eax
    .IF (index == -1)
        INVOKE crt_printf, offset err
        INVOKE ExitProcess, 1
    .ENDIF
    INVOKE __delete_button, index
    ret
delete_button_hwnd ENDP

draw_button PROC uses eax ebx ecx edx edi esi parent:DWORD, id:DWORD, dis:DWORD
    local btn:Button
    String error, "Draw button not found", 0ah, 0dh
    String msg, "Drawing button ptr %d hwnd %d state %u type %d", 0ah, 0dh
    INVOKE id_to_hwnd, parent, id
    mov ebx, eax
    INVOKE get_button, ebx
    mov btn, eax
    .IF (btn == 0)
        INVOKE
    .ELSE
    .ENDIF
    
    
    ret
draw_button ENDP

ShowButton PROC USES eax ebx ecx edx esi edi hWnd:DWORD, visible:BYTE
        INVOKE  ShowWindow, hWnd, visible
    ret
ShowButton ENDP

EnableButton PROC USES eax ebx ecx edx esi edi hWnd:DWORD, enable:BYTE
        INVOKE  EnableWindow, hWnd, enable
    ret
EnableButton ENDP
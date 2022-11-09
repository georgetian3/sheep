N_BUTTONS = 100
__button_count DWORD 0
__id_count DWORD 0
__buttons DWORD N_BUTTONS DUP(?)

STATE_DISABLE = 0
STATE_ENABLE = 1
STATE_MOVING = 2
STATE_HIDDEN = 3
N_STATES = 4


Button STRUCT
    start_pos POINT <>
    hWnd DWORD ?
    active DWORD ?
    moving DWORD ?
    gray DWORD ?
    in_slot DWORD ?
    callback DWORD ?
    type_ DWORD ?
    id DWORD ?
    k DWORD ?
    start_time DWORD ?
    dxpf REAL8 ?
    dypf REAL8 ?
    frame DWORD ?
    frames DWORD ?
Button ENDS

last_x DWORD 0
last_y DWORD 0
last_index DWORD -1
last_button DWORD 0

btnStr BYTE "button", 0


win_pos PROTO hWnd:DWORD, point:DWORD
id_to_hwnd PROTO parent:DWORD, id:DWORD
get_button_at_index PROTO index:DWORD
get_button PROTO hWnd:DWORD
create_button PROTO parent:DWORD, type_:DWORD, x:DWORD, y:DWORD, k:DWORD, wd:DWORD, ht:DWORD
print_buttons PROTO
delete_button PROTO hWnd:DWORD
draw_button PROTO parent:DWORD, id:DWORD, dis:DWORD
set_active PROTO btn_active:DWORD, active:DWORD
is_tile PROTO btn_istile:DWORD
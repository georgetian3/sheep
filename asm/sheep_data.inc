MAP1 BYTE "map1.txt" , 0
MAP2 BYTE "map2.txt" , 0
MAP3 BYTE "map3.txt" , 0
TIME_DOUBLE real8 0.2

lost PROTO hWnd:DWORD
win PROTO hWnd:DWORD
update_slot PROTO btn1:DWORD
handle_button_click PROTO parent:DWORD, btn2:DWORD
FPS = 120
MSPF = 8
;for comparing double with zero
epsilon REAL8 1.0E-13
floattest REAL8 ?

slot_x PROTO slot_index:DWORD
overlap PROTO a:DWORD, b:DWORD
update PROTO
match_slot PROTO
move_button PROTO hWnd:DWORD, x:DWORD, y:DWORD, time:REAL8
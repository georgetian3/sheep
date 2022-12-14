SLOT_MOVE_TIME REAL8 0.2

; for building purposes
WINDOW_WIDTH = 768
WINDOW_HEIGHT = 1024
STAGE_STARTMENU = 0
STAGE_1 = 1
UNIT_SIZE_X = 80
UNIT_SIZE_Y = 73
BUTTON_STARTGAME = 1001
BUTTON_STARTGAME_WIDTH = 212
BUTTON_STARTGAME_HEIGHT = 76

; for buildmaps
MAP_WIDTH = 8
MAP_LENGTH = 8
MAP_HEIGHT = 2

; for the location of the slot
SLOT_X = 114
SLOT_X_OFFSET = 18
SLOT_Y = 812
SLOT_SIZE = 7
MATCH_COUNT = 3

;SLOT_MOVE_TIME = 0.5
; stage, slot_count, and total are important for condition checking
slot_count DWORD 0
stage DWORD 1
total DWORD 0
realzero REAL8 0.0

index_insert DWORD -1
slot DWORD SLOT_SIZE DUP(0)

bmp_bg  DWORD ?

overlap PROTO a:DWORD, b:DWORD
shuffle PROTO arr:DWORD, n:DWORD
slot_x PROTO slot_index:DWORD
insert_slot PROTO btn_insert:DWORD, index:DWORD
delete_all_buttons PROTO
paint PROTO hWnd:DWORD
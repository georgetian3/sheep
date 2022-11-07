N_TILE_TYPES = 16
N_TILES = 18
N_BUTTON_TYPES = 18
TILE_WIDTH = 64
TILE_HEIGHT = 64

TYPE_CARROT = 0
TYPE_CORN = 1
TYPE_GRASS = 2
TYPE_TREE = 3
TYPE_FORK = 4
TYPE_BELL = 5
TYPE_BRUSH = 6
TYPE_BUCKET = 7
TYPE_CABBAGE = 8
TYPE_FIRE = 9
TYPE_HAY = 10
TYPE_GLOVE = 11
TYPE_MILK = 12
TYPE_PLIERS = 13
TYPE_RICE = 14
TYPE_ROPE = 15
TYPE_START = 16
TYPE_UNDO = 17

MAX_FILENAME_LEN = 255

String  carrot, "carrot"
String  corn, "corn"
String  grass, "grass"
String  tree, "tree"
String  fork, "fork"
String  bell, "bell"
String  brush, "brush"
String  bucket, "bucket"
String  cabbage, "cabbage"
String  fire, "fire"
String  hay, "hay"
String  gloves, "gloves"
String  milk, "milk"
String  pliers, "pliers"
String  rice, "rice"
String  rope, "rope"
String  start, "start"
String  undo, "undo"

String  dir, "../res/"
String  ext, ".bmp"
String  dark, "_dark"

bitmap_names DWORD OFFSET carrot, OFFSET corn, OFFSET grass, OFFSET tree, OFFSET fork, OFFSET bell 
             DWORD OFFSET brush, OFFSET bucket, OFFSET cabbage, OFFSET fire, OFFSET hay, OFFSET gloves
             DWORD OFFSET milk, OFFSET pliers, OFFSET rice, OFFSET rope, OFFSET start, OFFSET undo



bitmaps_tile_type   DWORD N_TILE_TYPES DUP(?)
bitmaps             DWORD 2 DUP(bitmaps_tile_type)
bitmap_filename     BYTE MAX_FILENAME_LEN DUP(?)


load_bitmap PROC uses ebx ecx edx esi edi filename:DWORD
    INVOKE LoadImageA, 0, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE
    ret
load_bitmap ENDP

load_bitmaps PROC uses eax ebx ecx edx esi edi

        String loadfailed, "LoadImage failed", 0ah, 0dh
        String test1, "%d", 0ah, 0dh
        String loadbitmaps, "load_bitmaps", 10, 13

        Print   OFFSET loadbitmaps
        
        mov     ecx, 0
        .WHILE ecx < 2
                mov     edx, 0
                .WHILE edx < N_BUTTON_TYPES
                        pushad
                        INVOKE  crt_strcpy, OFFSET bitmap_filename, OFFSET dir
                        popad
                        Print   OFFSET here
                        mov eax, 4
                        mul esi
                        mov ebx, eax
                        pushad
                        INVOKE crt_strcat, bitmap_filename, bitmap_names[ebx]
                        popad
                        .IF ecx == 0
                            pushad
                            INVOKE crt_strcat, bitmap_filename, addr dark
                            popad
                        .ENDIF
                        pushad
                        INVOKE crt_strcat, bitmap_filename, addr ext
                        popad
                        Print bitmap_filename
                        ;aboves are fine
                        COMMENT `
                        mov eax, 4
                        mul ecx
                        mov edi, dword ptr bitmaps[eax]
                        mov edx, dword ptr edi[ebx]
                        INVOKE load_bitmap, bitmap_filename
                        `
                
                        
                        inc esi
                .ENDW
                inc ecx
        .ENDW
    ret
load_bitmaps ENDP
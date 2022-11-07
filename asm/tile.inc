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

carrot BYTE "carrot", 0
corn BYTE "corn", 0
grass BYTE "grass", 0
tree BYTE "tree", 0
fork BYTE "fork", 0
bell BYTE "bell", 0
brush BYTE "brush", 0
bucket BYTE "bucket", 0
cabbage BYTE "cabbage", 0
fire BYTE "fire", 0
hay BYTE "hay", 0
gloves BYTE "gloves", 0
milk BYTE "milk", 0
pliers BYTE "pliers", 0
rice BYTE "rice", 0
rope BYTE "rope", 0
start BYTE "start", 0
undo BYTE "undo", 0
bitmap_names DWORD OFFSET carrot, OFFSET corn, OFFSET grass, OFFSET tree, OFFSET fork, OFFSET bell, 
             DWORD OFFSET brush, OFFSET bucket, OFFSET cabbage, OFFSET fire, OFFSET hay, OFFSET gloves
             DWORD OFFSET milk, OFFSET pliers, OFFSET rice, OFFSET rope, OFFSET start, OFFSET undo
dir DWORD "../res/"
ext DWORD ".bmp"
dark DWORD "_dark"


bitmaps_tile_type DWORD N_TILE_TYPES DUP(?)
bitmaps DWORD 2 DUP(bitmaps_tile_type)
LOAD_PRINT BYTE "LoadImage failed", 0ah, 0dh, 0

load_tiles PROC USES eax ebx ecx edx esi edi
    mov ecx, 0
    .WHILE (ecx < 2)
        mov ebx, 0
        .WHILE (ebx < N_TILE_TYPES)
            push ebx
            push ecx
            mov eax, 4
            mul ebx
            mov ebx, eax
            mov eax, 4
            mul ecx
            mov ecx, eax
            mov esi, dword ptr filenames[ecx]
            mov edx, [esi]
            INVOKE LoadImageA, 0, edx, IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE
            .IF (eax == 0)
                pushad
                INVOKE crt_printf, offset LOAD_PRINT
                popad
                jmp F1
            .ENDIF
            pop ecx
            pop ebx
            inc ebx
        .ENDW
        inc ecx
    .ENDW
    F1:
        ret
load_tiles ENDP

load_bitmap PROC uses ebx ecx edx esi edi filename:DWORD, wd:DWORD, ht:DWORD
    INVOKE LoadImageA, 0, filename, IMAGE_BITMAP, wd, ht, LR_LOADFROMFILE
    ret
load_bitmap ENDP

load_bitmaps PROC uses eax ebx ecx edx esi edi
    ret
load_bitmaps ENDP
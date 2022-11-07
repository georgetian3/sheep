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
bitmap_names DWORD OFFSET carrot, OFFSET corn, OFFSET grass, OFFSET tree, OFFSET fork, OFFSET bell 
             DWORD OFFSET brush, OFFSET bucket, OFFSET cabbage, OFFSET fire, OFFSET hay, OFFSET gloves
             DWORD OFFSET milk, OFFSET pliers, OFFSET rice, OFFSET rope, OFFSET start, OFFSET undo
dir BYTE "../res/", 0
ext BYTE ".bmp", 0
dark BYTE "_dark", 0


bitmaps_tile_type DWORD N_TILE_TYPES DUP(?)
bitmaps DWORD 2 DUP(bitmaps_tile_type)


load_bitmap PROC uses ebx ecx edx esi edi filename:DWORD
    INVOKE LoadImageA, 0, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE
    ret
load_bitmap ENDP

load_bitmaps PROC uses eax ebx ecx edx esi edi
    local filename:DWORD
    String loadfailed, "LoadImage failed", 0ah, 0dh
    String test1, "%d", 0ah, 0dh
    
    INVOKE crt_malloc, MAX_FILENAME_LEN
    .IF eax == 0
        Print OFFSET loadfailed
    .ENDIF
    mov filename, eax
    mov ecx, 0
    .WHILE ecx < 2
        mov esi, 0
        .WHILE esi < N_BUTTON_TYPES
            pushad
            INVOKE crt_strcpy, filename, addr dir
            popad
            mov eax, 4
            mul esi
            mov ebx, eax
            pushad
            INVOKE crt_strcat, filename, bitmap_names[ebx]
            popad
            .IF ecx == 0
                pushad
                INVOKE crt_strcat, filename, addr dark
                popad
            .ENDIF
            INVOKE crt_strcat, filename, addr ext
            ;Print filename for checking
            ;aboves are fine
            mov eax, 4
            mul ecx
            mov edi, dword ptr bitmaps[eax]
            mov edx, dword ptr edi[ebx]
            INVOKE load_bitmap, filename
            
    
            
            inc esi
        .ENDW
        inc ecx
    .ENDW
    ret
load_bitmaps ENDP
N_TILE_TYPES = 5
N_TILES = 18
N_BUTTON_TYPES = 4
TILE_WIDTH = 64
TILE_HEIGHT = 64

TYPE_START = -2
TYPE_SETTINGS = -1
TYPE_CARROT = 0
TYPE_CORN = 1
TYPE_GRASS = 2
TYPE_TREE = 3
TYPE_FORK = 4
TYPE_FIRE = 5
TYPE_GLOVE = 6
TYPE_COTTON = 7
TYPE_RICE = 8
TYPE_HAY = 9

MAX_FILENAME_LEN = 255

filenames_carrot BYTE "../res/carrot.bmp", 0
filenames_corn BYTE "../res/corn.bmp", 0
filenames_grass BYTE "../res/grass.bmp", 0
filenames_regular DWORD offset filenames_carrot, offset filenames_corn, offset filenames_grass
filenames_dark_carrot BYTE "../res/carrot_dark.bmp", 0
filenames_dark_corn BYTE "../res/corn_dark.bmp", 0
filenames_dark_grass BYTE "../res/grass_dark.bmp", 0
filenames_dark DWORD offset filenames_dark_carrot, offset filenames_dark_corn, offset filenames_dark_grass
filenames DWORD offset filenames_regular, offset filenames_dark
dir BYTE "../res/", 0
ext BYTE ".bmp", 0
dark BYTE "_dark", 0

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
N_TILE_TYPES equ 5
N_TILES equ 18
N_BUTTON_TYPES equ 4
TILE_WIDTH equ 64
TILE_HEIGHT equ 64

TYPE_START equ -2
TYPE_SETTINGS equ -1
TYPE_CARROT equ 0
TYPE_CORN equ 1
TYPE_GRASS equ 2
TYPE_TREE equ 3
TYPE_FORK equ 4
TYPE_FIRE equ 5
TYPE_GLOVE equ 6
TYPE_COTTON equ 7
TYPE_RICE equ 8
TYPE_HAY equ 9

MAX_FILENAME_LEN equ 255

filenames_carrot BYTE "../res/carrot.bmp", 0
filenames_corn BYTE "../res/corn.bmp", 0
filenames_grass BYTE "../res/grass.bmp", 0
filenames_regular DWORD offset filenames_carrot, offset filenames_corn, offset filenames_grass
filenames_dark_carrot BYTE "../res/carrot_dark.bmp", 0
filenames_dark_corn BYTE "../res/corn_dark.bmp", 0
filenames_dark_grass BYTE "../res/grass_dark.bmp", 0
filenames_dark DWORD offset filenames_dark_carrot, offset filenames_dark_corn, offset filenames_dark_grass
filenames DWORD offset filenames_regular, offset filenames_dark

bitmaps_tile_type DWORD N_TILE_TYPES DUP(?)
bitmaps DWORD 2 DUP(bitmaps_tile_type)
LOAD_PRINT BYTE "LoadImage failed", 0ah, 0dh, 0
stry BYTE "%s", 0ah, 0dh, 0


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

load_bitmaps ENDP
N_TILE_TYPES equ 3
N_TILES equ 9
TILE_WIDTH equ 64
TILE_HEIGHT equ 64

filenames_carrot BYTE "../res/carrot.bmp", 0
filenames_corn BYTE "../res/corn.bmp", 0
filenames_grass BYTE "../res/grass.bmp", 0
filenames_regular DWORD offset filenames_carrot, offset filenames_corn, offset filenames_grass
filenames_dark_carrot BYTE "../res/carrot_dark.bmp", 0
filenames_dark_corn BYTE "../res/corn_dark.bmp", 0
filenames_dark_grass BYTE "../res/grass_dark.bmp", 0
filenames_dark DWORD offset filenames_dark_carrot, offset filenames_dark_corn, offset filenames_dark_grass
filenames DWORD offset filenames_regular, offset filenames_dark

tile_bitmaps_tile_type DWORD N_TILE_TYPES DUP(?)
tile_bitmaps DWORD 2 DUP(tile_bitmaps_tile_type)
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
N_TILE_TYPES BYTE 3
N_TILES BYTE 9
TILE_WIDTH BYTE 64
TILE_HEIGHT BYTE 64

filenames_carrot BYTE "../res/carrot.bmp", 0
filenames_corn BYTE "../res/corn.bmp", 0
filenames_grass BYTE "../res/grass.bmp", 0
filenames DWORD offset filenames_carrot, offset filenames_corn, offset filenames_grass
tile_bitmaps DWORD 3 DUP(?)
tile_buttons DWORD 9 DUP(?)
LOAD_PRINT BYTE "LoadImage failed", 0ah, 0dh, 0


load_tiles PROC USES eax ebx ecx edx esi edi
    invoke LoadImageA, 0, offset filenames_carrot, IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE
    .IF eax == 0
        pushad
        invoke crt_printf, offset LOAD_PRINT
        popad
        jmp check4
    .ENDIF
    check2:
        invoke LoadImageA, 0, offset filenames_corn, IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE
        .IF eax == 0
            pushad
            invoke crt_printf, offset LOAD_PRINT
            popad
        .ENDIF
        jmp check4
    check3:
        invoke LoadImageA, 0, offset filenames_grass, IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE
        .IF eax == 0
            pushad
            invoke crt_printf, offset LOAD_PRINT
            popad
        .ENDIF
    check4:
    ret
load_tiles ENDP

tile_type PROC USES ebx ecx edx esi edi hWnd:DWORD
    LOCAL bm:DWORD
    invoke SendMessage, hWnd, BM_GETIMAGE, IMAGE_BITMAP, 0
    mov bm, eax
    mov esi, offset tile_bitmaps
    mov edx, 0
    mov ebx, [esi]
    .IF bm == ebx
        jmp S1
    .ELSE
        inc edx
        add esi, 4
        mov ebx, [esi]
    .ENDIF
    .IF bm == ebx
        jmp S1
    .ELSE
        inc edx
        add esi, 4
        mov ebx, [esi]
    .ENDIF
    .IF bm == ebx
        jmp S1
    .ELSE
        jmp S2
    .ENDIF
    S2:
        mov eax, -1
    S1:
        mov eax, edx
    ret
tile_type ENDP
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
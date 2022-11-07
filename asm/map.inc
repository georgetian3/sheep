MAX_SIZE = 64
res_dir BYTE "../res/"
read_mode BYTE "r"

build_map PROC uses ebx ecx edx esi edi hwnd:DWORD, map_name:DWORD
    local i:DWORD, j:DWORD, k:DWORD, fp:DWORD, map_filename:DWORD, total:DWORD, offset_x:DWORD, offset_y:DWORD, 
        tiles_num:DWORD, item:DWORD, flag:DWORD, map_width:DWORD, map_length:DWORD, map_height:DWORD
    String build_map_failed, "failed", 0ah, 0dh
    String file_open_failed, "file open failed", 0ah, 0dh
    String one_int, "%d"
    String two_int, "%d %d"
    mov fp, 0
    INVOKE crt_malloc, MAX_FILENAME_LEN
    .IF eax == 0
        Print OFFSET build_map_failed
    .ENDIF
    mov map_filename, eax
    INVOKE crt_strcat, map_filename, addr res_dir
    INVOKE crt_strcat, map_filename, map_name
    INVOKE crt_fopen, map_filename, addr read_mode
    .IF eax == 0
        Print offset file_open_failed
    .ENDIF
    mov fp, eax
    INVOKE crt_fscanf, fp, addr one_int, addr total
    INVOKE crt_fscanf, fp, addr two_int, addr offset_x, addr offset_y
    
    mov ecx, 0
    .WHILE ecx < N_TILE_TYPES
        mov eax, 4
        mul ecx
        pushad
            INVOKE crt_fscanf, fp, addr one_int, addr tiles_num[eax]
        popad
        inc ecx
    .ENDW

    mov eax, 4
    mul total
    INVOKE crt_malloc, eax
    .IF eax == 0
        Print OFFSET build_map_failed
    .ENDIF
    mov item, eax

    mov ecx, 0
    .WHILE ecx < total
        mov esi, 0
        .WHILE esi < N_TILE_TYPES
            mov edi, 0
            mov eax, 3
            mul tiles_num[esi]
            .WHILE edi < eax
                mov item[ecx], esi
                inc edi
                inc ecx
            .ENDW
            inc esi
        .ENDW
    .ENDW

    INVOKE shuffle, item, total

    ret
build_map ENDP

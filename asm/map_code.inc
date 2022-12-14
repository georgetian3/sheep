coordinate PROC uses ebx ecx edx esi edi x:DWORD, y:DWORD, z:DWORD
        COMMENT !
            helper function for build_map to calculate the correct coordinates
        !
        mov     eax, y
        mov     ebx, z
        mul     ebx
        add     eax, x
        ret
coordinate ENDP

build_map PROC uses ebx ecx edx esi edi hWnd:DWORD, map_name:DWORD

        COMMENT !
            map_name: string with the filename of the map
            creates the tiles and places them correctly
            returns in eax the number of tiles in map
        !
        local i:DWORD, j:DWORD, k:DWORD, x:DWORD, y:DWORD, fp:DWORD, map_filename:DWORD, total_:DWORD, offset_x:DWORD, offset_y:DWORD, type_count:DWORD,
            tile_type_count:DWORD, item:DWORD, flag:DWORD, map_width:DWORD, map_length:DWORD, map_height:DWORD

        

        String file_open_failed, "Failed to open map file", 10, 13
        String one_int, "%d"
        String two_int, "%d %d"
        String three_int, "%d %d %d"
        ;mov fp, 0
        INVOKE  crt_malloc, MAX_FILENAME_LEN
        .IF eax == 0
                Print   OFFSET malloc_f
                INVOKE  ExitProcess, 1
        .ENDIF
        mov     map_filename, eax
        PINVOKE crt_strcpy, map_filename, ADDR res_dir
        PINVOKE crt_strcat, map_filename, map_name
        INVOKE  crt_fopen, map_filename, ADDR read_mode
        .IF eax == 0
                Print   OFFSET file_open_failed
                INVOKE  ExitProcess, 1
        .ENDIF
        mov     fp, eax
        PINVOKE crt_fscanf, fp, ADDR one_int, ADDR offset_x
        PINVOKE crt_fscanf, fp, ADDR one_int, ADDR offset_y

        AINVOKE crt_malloc, N_BUTTONS * SIZEOF DWORD
        .IF eax == 0
                Print OFFSET malloc_f
                INVOKE  ExitProcess, 1
        .ENDIF
        mov     item, eax

        mov     total_, 0
        mov     ecx, 0
        .WHILE ecx < N_TILE_TYPES
                PINVOKE crt_fscanf, fp, ADDR one_int, ADDR type_count
                mov     eax, type_count
                mov     ebx, MATCH_COUNT
                mul     ebx
                mov     edx, eax
                ;Print   OFFSET decStr, eax
                ;Print   OFFSET newline
                mov     ebx, 0
                .WHILE ebx < edx
                        INVOKE  AA1, item, total_
                        mov     [eax], ecx
                        
                        inc     ebx
                        inc     total_
                .ENDW
                inc ecx
        .ENDW

        


        INVOKE  shuffle, item, total_



        PINVOKE crt_fscanf, fp, ADDR one_int, ADDR map_width
        PINVOKE crt_fscanf, fp, ADDR one_int, ADDR map_length
        PINVOKE crt_fscanf, fp, ADDR one_int, ADDR map_height

        ;Print   OFFSET here

        mov     total_, 0

        mov     edx, 0
        .WHILE edx < map_height
                mov     ecx, 0
                .WHILE ecx < map_width
                        mov     ebx, 0
                        .WHILE ebx < map_length
                                PINVOKE crt_fscanf, fp, ADDR one_int, ADDR flag
                                .IF flag == 0
                                        jmp     continue
                                .ENDIF

                                pushad

                                AINVOKE coordinate, offset_x, ebx, TILE_HWIDTH
                                mov     x, eax
                                AINVOKE coordinate, offset_y, ecx, TILE_HHEIGHT
                                mov     y, eax

                                AINVOKE AA1, item, total_

                                mov     eax, [eax]
                                PINVOKE create_button, hWnd, eax, x, y, edx, TILE_WIDTH, TILE_HEIGHT
                                popad

                                inc     total_
                            continue:
                                inc     ebx
                        .ENDW
                        inc     ecx
                .ENDW
                inc     edx
        .ENDW
        INVOKE crt_free ,map_filename
        INVOKE crt_free ,item
        
        mov     eax, total_
        ret
build_map ENDP


load_bitmap PROC uses ebx ecx edx esi edi filename:DWORD
    INVOKE LoadImageA, 0, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE
    ret
load_bitmap ENDP

load_bitmaps PROC uses eax ebx ecx edx esi edi
        local filename:DWORD
        String loadfailed, "LoadImage failed", 0ah, 0dh
        String test1, "%d", 0ah, 0dh

        String  bd, "bitmaps_dark "
        String  bl_, "bitmaps_light "


        Print   OFFSET undo
        Print   OFFSET newline

        mov     undo, 41h
        Print   OFFSET undo
        Print   OFFSET newline
        ret


        INVOKE crt_malloc, MAX_FILENAME_LEN
        .IF eax == 0
                Print   OFFSET loadfailed
        .ENDIF
        mov filename, eax

        
        mov ecx, 0
        .WHILE ecx < 2
                mov     esi, 0
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

                        pushad
                        INVOKE crt_strcat, filename, addr ext
                        popad

                        Print   filename
                        Print   OFFSET newline
                        INVOKE  load_bitmap, filename
                        .IF eax == 0
                                Print   ADDR loadfailed
                        .ENDIF
                        mov     edi, eax

                        Print   OFFSET hexStr, OFFSET bitmaps
                        Print   OFFSET newline



                        Print   OFFSET hexStr, eax
                        Print   OFFSET newline


                        mov     [eax], edi
                        
                        inc esi
                .ENDW
                inc ecx
        .ENDW
    ret
load_bitmaps ENDP
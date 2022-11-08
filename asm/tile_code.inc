

load_bitmap PROC uses ebx ecx edx edx edi filename:DWORD
        INVOKE  LoadImageA, 0, filename, IMAGE_BITMAP,
                0, 0, LR_LOADFROMFILE
     ret
load_bitmap ENDP

load_bitmaps PROC uses eax ebx ecx edx edx esi edi
        LOCAL filename:DWORD
        String loadfailed, "LoadImage failed", 0ah, 0dh


        INVOKE  crt_malloc, MAX_FILENAME_LEN
        mov     filename, eax
        .IF eax == 0
                String malloc_f, "malloc failed", 10, 13
                Print   OFFSET malloc_f
                INVOKE  ExitProcess, 1
        .ENDIF

        mov     ecx, 0
        .WHILE ecx < 2
                mov     edx, 0
                .WHILE edx < N_BUTTON_TYPES
                
                        PINVOKE crt_strcpy, filename, ADDR dir

                        PINVOKE crt_strcat, filename, bitmap_names[edx * SIZEOF DWORD]

                        .IF ecx == 0
                                PINVOKE crt_strcat, filename, addr dark
                        .ENDIF

                        PINVOKE crt_strcat, filename, ADDR ext

                        INVOKE  load_bitmap, filename

                        .IF eax == 0
                                Print   ADDR loadfailed
                                INVOKE  ExitProcess, 1
                        .ENDIF

                        String  bitm, "Bitmap, %x", 10, 13
                        Print   OFFSET bitm, eax

                        mov     esi, eax

                        INVOKE  AA2, OFFSET bitmaps, ecx, edx

                        mov     [eax], esi
                        
                        inc     edx
                .ENDW
                inc ecx
        .ENDW
        INVOKE  crt_free, filename
    ret
load_bitmaps ENDP
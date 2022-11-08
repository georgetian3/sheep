




bitmap_names DWORD OFFSET carrot, OFFSET corn, OFFSET grass, OFFSET tree, OFFSET fork, OFFSET bell 
             DWORD OFFSET brush, OFFSET bucket, OFFSET cabbage, OFFSET fire, OFFSET hay, OFFSET gloves
             DWORD OFFSET milk, OFFSET pliers, OFFSET rice, OFFSET rope, OFFSET start, OFFSET undo

bitmaps_tile_type DWORD N_TILE_TYPES DUP(?)


bitmaps DWORD 2 DUP(bitmaps_tile_type)


load_bitmap PROC uses ebx ecx edx esi edi filename_:DWORD
    INVOKE LoadImageA, 0, filename_, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE
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
            pushad
            INVOKE crt_strcat, filename, addr ext
            popad
            Print filename
            ;aboves are fine
            ; mov eax, 4
            ; mul ecx
            ; mov edi, dword ptr bitmaps[eax]
            ; mov edx, dword ptr edi[ebx]
            ; INVOKE load_bitmap, filename
            
    
        
        inc esi
        .ENDW
    inc ecx
    .ENDW
    ret
load_bitmaps ENDP
        

strcpy PROC uses eax ebx ecx edx esi edi dst:DWORD, src:DWORD
        String  scp, "strcpy", 10, 13
        ;Print   OFFSET scp
        mov     esi, src
        mov     edi, dst
        mov     al, BYTE PTR [esi]
        .WHILE al != 0
                ;Print OFFSET here
                mov     al, BYTE PTR [esi]
                mov     [edi], al
                inc     esi
                inc     edi
        .ENDW
        mov     [edi], al
        ;Print   OFFSET done
    ret
strcpy ENDP

strcat PROC uses eax ebx ecx edx esi edi dst:DWORD, src:DWORD
        mov     esi, src
        mov     edi, dst
        mov     al, [edi]
        .WHILE  al != 0
                inc     edi
                mov     al, [edi]
        .ENDW
        INVOKE  strcpy, edi, esi
    ret
strcat ENDP




; load_bitmaps PROC uses eax ebx ecx edx esi edi
;         LOCAL filename:DWORD

;         String loadfailed, "LoadImage failed", 0ah, 0dh
;         String test1, "%d", 0ah, 0dh
;         String loadbitmaps, "load_bitmaps", 10, 13
;         Print   OFFSET loadbitmaps
        
;         INVOKE  crt_malloc, MAX_FILENAME_LEN
;         .IF eax == 0
;             Print OFFSET loadfailed
;         .ENDIF
;         mov filename, eax




;         mov     ecx, 0

;         .WHILE  ecx < 100
;                 mov     filename, 0
;                 inc     filename
;                 inc     ecx
;         .ENDW
;         mov     ecx, 0
;         .WHILE  ecx < 100
;                 mov     al, BYTE PTR filename
;                 inc     filename
;                 inc     ecx
;         .ENDW

;         mov     ecx, 0
;         .WHILE ecx < 2
;                 mov     edx, 0
;                 .WHILE edx < N_BUTTON_TYPES

;                         Print   OFFSET here
;                         INVOKE  strcpy, ADDR filename, ADDR dir

;                         lea     eax, [OFFSET bitmap_names + edx * 4]

;                         INVOKE  strcat, ADDR filename, [eax]


;                         .IF ecx == 0
;                                 INVOKE  strcat, filename, ADDR dark
;                         .ENDIF
;                         Print   OFFSET here

;                         INVOKE  strcat, filename, ADDR ext

;                         Print   filename
;                         Print   OFFSET newline
;                         INVOKE  load_bitmap, filename
;                         .IF eax == 0
;                                 String  lbf, "LoadBitmap failed"
;                                 Print   OFFSET lbf
;                                 ;INVOKE  ExitProcess, 0
;                         .ENDIF
;                         COMMENT `
;                         `
;                         inc     edx
;                 .ENDW
;                 inc ecx
;         .ENDW
;         INVOKE  crt_free, ADDR filename 
;         Print   OFFSET done


;     ret
; load_bitmaps ENDP
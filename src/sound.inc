play_sound PROC uses eax ebx ecx edx esi edi sound:DWORD, bloop:DWORD, interrupt:DWORD
    .IF (interrupt == 1)
    
    .ENDIF
    .IF (sound == -1)
        jmp F1
    .ENDIF
    mov eax, SND_LOOP
    mul bloop
    mov ebx, eax
    mov eax, 4
    mul sound
    or ebx, SND_ASYNC
    or ebx, SND_FILENAME
    or ebx, SND_NOSTOP
    INVOKE PlaySoundA, dword ptr sound_files[edx], 0, ebx
    F1:
play_sound ENDP
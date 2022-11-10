
play_sound PROC uses eax ebx ecx edx esi edi sound:DWORD, bloop:DWORD, interrupt:DWORD
    COMMENT !
        plays the theme song
    !
    .IF (interrupt == 1)
        INVOKE PlaySoundA, 0, 0, 0
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
    or ebx, SND_LOOP
    INVOKE PlaySoundA, dword ptr sound_files[eax], 0, ebx
    F1:
        ret
play_sound ENDP

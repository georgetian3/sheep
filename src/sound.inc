sound_files_welcome BYTE "../res/welcome.wav", 0
sound_files_game BYTE "../res/game.wav", 0
sound_files_clicked BYTE "../res/clicked.wav", 0
sound_files_match BYTE "../res/match.wav", 0
sound_files DWORD offset sound_files_welcome, offset sound_files_game, offset sound_files_clicked, offset sound_files_match

play_sound PROC uses eax ebx ecx edx esi edi sound:DWORD, bloop:DWORD, interrupt:DWORD
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
    INVOKE PlaySoundA, dword ptr sound_files[eax], 0, 20019h
    F1:
        ret
play_sound ENDP

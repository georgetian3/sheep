rand_int PROC USES ebx ecx edx esi edi min:DWORD, max:DWORD
    mov ebx, max
    sub ebx, min
    INVOKE crt_rand
    div ebx
    add edx, min
    mov eax, edx
    ret
rand_int ENDP
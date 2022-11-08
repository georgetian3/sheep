String MACRO Name, Text:VARARG
    LOCAL __lbl__
        jmp __lbl__
        Name byte Text, 0
    __lbl__:
ENDM

PINVOKE MACRO cmd:VARARG
    pushad
    INVOKE  cmd
    popad
ENDM

AINVOKE MACRO cmd:VARARG
    push    ebx
    push    ecx
    push    edx
    push    esi
    push    edi
    push    ebp
    push    esp
    INVOKE  cmd
    pop     esp
    pop     ebp
    pop     edi
    pop     esi
    pop     edx
    pop     ecx
    pop     ebx
ENDM



Print MACRO strPtrs:VARARG
    pushad
    INVOKE  crt_printf, strPtrs
    popad
ENDM
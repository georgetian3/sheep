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

Print MACRO strPtrs:VARARG
    pushad
    INVOKE  crt_printf, strPtrs
    popad
ENDM
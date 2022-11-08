String MACRO Name, Text:VARARG
    LOCAL __lbl__
        jmp __lbl__
        Name byte Text, 0
    __lbl__:
ENDM

PInvoke MACRO cmd_:VARARG
    pushad
    INVOKE  cmd_
    popad
ENDM

Print MACRO strPtrs:VARARG
    PInvoke crt_printf, strPtrs
ENDM
String MACRO Name, Text:VARARG
    LOCAL __lbl__
        jmp __lbl__
        Name byte Text, 0
    __lbl__:
ENDM

Print MACRO strPtrs:VARARG
    pushad
    invoke crt_printf, strPtrs
    popad
ENDM
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

CheckError MACRO errmsg:VARARG
    .IF eax == 0
        Print errmsg
        INVOKE ExitProcess, 1
    .ENDIF
ENDM
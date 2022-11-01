CreateButton PROC hParent:DWORD, x:DWORD, y:DWORD, wd:DWORD, ht:DWORD, id:DWORD
        String btn, "BUTTON"
        String btnErr, "CreateButton failed", 10, 13
        INVOKE CreateWindowEx,0,
                ADDR btn,ADDR emptyStr,
                WS_CHILD or WS_VISIBLE or BS_BITMAP,
                x,y,wd,ht,hParent,id,
                hInstance,NULL
        CheckError ADDR btnErr
    ret
CreateButton ENDP

ShowButton PROC USES eax ebx ecx edx esi edi hWnd:DWORD, visible:BYTE
        INVOKE  ShowWindow, hWnd, visible
    ret
ShowButton ENDP

EnableButton PROC USES eax ebx ecx edx esi edi hWnd:DWORD, enable:BYTE
        INVOKE  EnableWindow, hWnd, enable
    ret
EnableButton ENDP
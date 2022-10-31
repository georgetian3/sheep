@echo off

if exist %1.exe del %1.exe

C:\MASM32\BIN\Ml.exe /c /coff /Fl %1.asm
if errorlevel 1 goto err

C:\MASM32\BIN\Link.exe /SUBSYSTEM:CONSOLE %1.obj
if errorlevel 1 goto err

echo Build completed successfully!
echo -----------------------------

goto cleanup

:err

echo.
echo There has been an error while building this project.
echo.

:cleanup

if exist %1.lst del %1.lst
if exist %1.obj del %1.obj
if exist %1.exe start /WAIT /B %1.exe
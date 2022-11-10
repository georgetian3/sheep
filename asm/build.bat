@echo off

if exist main.exe del main.exe

C:\MASM32\BIN\Ml.exe /c /coff main.asm
if errorlevel 1 goto err

C:\MASM32\BIN\Link.exe /debug /SUBSYSTEM:WINDOWS main.obj
if errorlevel 1 goto err

echo Build completed successfully!
echo -----------------------------

goto cleanup

:err

echo.
echo There has been an error while building this project.
echo.

:cleanup

::if exist main.lst del main.lst
if exist main.obj del main.obj
if exist main.exe start /WAIT /B main.exe
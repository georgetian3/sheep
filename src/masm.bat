@echo off

C:\MASM32\BIN\Ml.exe /c /coff /Fl main.asm
if errorlevel 1 goto errml
C:\MASM32\BIN\Link.exe /SUBSYSTEM:CONSOLE main.obj
if errorlevel 1 goto errlink

goto end

:errml
echo.
echo There has been an error while assembling this project.
echo.
goto end

:errlink
echo.
echo There has been an error while linking this project.
echo.
goto end

:end

if exist main.lst del main.lst
if exist main.obj del main.obj
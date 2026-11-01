@echo off
chcp 65001 > nul
rem ============================================================
rem  Демо 3. CreateFile: dwCreationDisposition, ReadFile, SetFilePointerEx, WriteFile.
rem  До показу спрогнозуйте: який рядок таблиці відповідає режиму "w"?
rem ============================================================
cd /d "%~dp0..\Файли"
set "BIN=%~dp0..\x64\Debug"
if not exist "%BIN%\Demo3_create.exe" ( echo [!] Зберіть рішення в конфігурації Debug. & exit /b 1 )
echo ^> Demo3_create.exe
"%BIN%\Demo3_create.exe"
pause
echo.
echo === ReadFile порціями по 16 байтів ===
echo ^> Demo3_create.exe read
"%BIN%\Demo3_create.exe" read
pause
echo.
echo === WriteFile у кінець копії ===
copy /y marks.bin copy.bin > nul
echo ^> Demo3_create.exe append copy.bin 88
"%BIN%\Demo3_create.exe" append copy.bin 88
dir copy.bin | find "copy.bin"
pause

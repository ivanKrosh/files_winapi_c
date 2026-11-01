@echo off
chcp 65001 > nul
rem ============================================================
rem  Демо 2. Прямий доступ: fseek, зміна оцінки на місці, пастка читання-запис.
rem  Програма змінює копії fix.bin, bad.bin, good.bin; marks.bin не чіпає.
rem ============================================================
cd /d "%~dp0..\Файли"
set "BIN=%~dp0..\x64\Debug"
if not exist "%BIN%\Demo2_seek.exe" ( echo [!] Зберіть рішення в конфігурації Debug. & exit /b 1 )
echo ^> Demo2_seek.exe
"%BIN%\Demo2_seek.exe"
pause
echo.
echo === Виправити оцінку з номером 8: 26 -^> 62 ===
copy /y marks.bin fix.bin > nul
echo ^> Demo2_seek.exe set fix.bin 8 62
"%BIN%\Demo2_seek.exe" set fix.bin 8 62
dir fix.bin | find "fix.bin"
pause
echo.
echo === Запис одразу після читання, без fseek ===
echo   Прогноз: куди потрапить 99?
echo   У Debug з'явиться вікно Debug Assertion Failed - натисніть Ignore.
pause
copy /y marks.bin bad.bin > nul
echo ^> Demo2_seek.exe bad bad.bin
"%BIN%\Demo2_seek.exe" bad bad.bin
pause
echo.
echo === Те саме з fseek(f, 0, SEEK_CUR) між читанням і записом ===
copy /y marks.bin good.bin > nul
echo ^> Demo2_seek.exe good good.bin
"%BIN%\Demo2_seek.exe" good good.bin
pause

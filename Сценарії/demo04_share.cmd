@echo off
chcp 65001 > nul
rem ============================================================
rem  Демо 4. Спільний доступ: dwShareMode.
rem  Сценарій відкриває друге вікно - у ньому програма тримає marks.bin відкритим.
rem ============================================================
cd /d "%~dp0..\Файли"
set "BIN=%~dp0..\x64\Debug"
if not exist "%BIN%\Demo4_share.exe" ( echo [!] Зберіть рішення в конфігурації Debug. & exit /b 1 )
echo === Перший процес: share = 0 ===
start "Demo4 - holder" /d "%CD%" "%BIN%\Demo4_share.exe" hold
timeout /t 1 > nul
echo   Прогноз: чи зможе другий процес хоча б прочитати файл?
pause
echo ^> Demo4_share.exe read
"%BIN%\Demo4_share.exe" read
echo ^> Demo4_share.exe fopen
"%BIN%\Demo4_share.exe" fopen
echo.
echo   Натисніть Enter у другому вікні, потім продовжуйте тут.
pause
echo.
echo === Перший процес: share = FILE_SHARE_READ ===
start "Demo4 - holder" /d "%CD%" "%BIN%\Demo4_share.exe" holdread
timeout /t 1 > nul
pause
echo ^> Demo4_share.exe read
"%BIN%\Demo4_share.exe" read
echo ^> Demo4_share.exe write
"%BIN%\Demo4_share.exe" write
echo ^> Demo4_share.exe fopen
"%BIN%\Demo4_share.exe" fopen
echo.
echo   Натисніть Enter у другому вікні.
pause

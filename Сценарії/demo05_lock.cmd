@echo off
chcp 65001 > nul
rem ============================================================
rem  Демо 5. LockFileEx: блокування однієї оцінки, а не всього файлу.
rem  Друге вікно блокує оцінку з номером 8 (байти 32..35).
rem ============================================================
cd /d "%~dp0..\Файли"
set "BIN=%~dp0..\x64\Debug"
if not exist "%BIN%\Demo5_lock.exe" ( echo [!] Зберіть рішення в конфігурації Debug. & exit /b 1 )
start "Demo5 - lock 8" /d "%CD%" "%BIN%\Demo5_lock.exe" lock 8
timeout /t 1 > nul
echo   Прогноз: яку оцінку вдасться прочитати - 7, 8 чи обидві?
pause
echo ^> Demo5_lock.exe read 7
"%BIN%\Demo5_lock.exe" read 7
echo ^> Demo5_lock.exe read 8
"%BIN%\Demo5_lock.exe" read 8
echo ^> Demo5_lock.exe write 8 62
"%BIN%\Demo5_lock.exe" write 8 62
pause
echo.
echo === Заблокувати ту саму оцінку ===
echo ^> Demo5_lock.exe trylock 8
echo. | "%BIN%\Demo5_lock.exe" trylock 8
echo.
echo   Без LOCKFILE_FAIL_IMMEDIATELY програма чекатиме.
echo   Запустіть, а потім натисніть Enter у другому вікні.
echo ^> Demo5_lock.exe lock 8
"%BIN%\Demo5_lock.exe" lock 8
pause

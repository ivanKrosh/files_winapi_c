@echo off
chcp 65001 > nul
rem ============================================================
rem  Демо 6. Файл як ціле: резервна копія marks.bin у теку backup.
rem ============================================================
cd /d "%~dp0..\Файли"
set "BIN=%~dp0..\x64\Debug"
if not exist "%BIN%\Demo6_manage.exe" ( echo [!] Зберіть рішення в конфігурації Debug. & exit /b 1 )
if exist backup ( attrib -r backup\*.* > nul & rd /s /q backup )
echo ^> Demo6_manage.exe info marks.bin
"%BIN%\Demo6_manage.exe" info marks.bin
pause
echo.
echo === Резервна копія двічі поспіль ===
echo ^> Demo6_manage.exe backup
"%BIN%\Demo6_manage.exe" backup
echo   Прогноз: що зробить другий запуск?
pause
echo ^> Demo6_manage.exe backup
"%BIN%\Demo6_manage.exe" backup
pause
echo.
echo === marks.bin змінився: оновлюємо час зміни ===
powershell -NoProfile -Command "(Get-Item marks.bin).LastWriteTime = Get-Date"
echo ^> Demo6_manage.exe info marks.bin
"%BIN%\Demo6_manage.exe" info marks.bin
echo ^> Demo6_manage.exe backup
"%BIN%\Demo6_manage.exe" backup
pause
echo.
echo === CopyFile з bFailIfExists = TRUE ===
echo ^> Demo6_manage.exe copynew
"%BIN%\Demo6_manage.exe" copynew
pause
echo.
echo === MoveFile: двічі ===
echo ^> Demo6_manage.exe move
"%BIN%\Demo6_manage.exe" move
echo ^> Demo6_manage.exe backup
"%BIN%\Demo6_manage.exe" backup
echo ^> Demo6_manage.exe move
"%BIN%\Demo6_manage.exe" move
pause
echo.
echo === DeleteFile файлу "лише для читання" ===
echo ^> attrib +r backup\old.bin
attrib +r backup\old.bin
echo ^> Demo6_manage.exe delete backup\old.bin
"%BIN%\Demo6_manage.exe" delete backup\old.bin
echo ^> attrib -r backup\old.bin
attrib -r backup\old.bin
echo ^> Demo6_manage.exe delete backup\old.bin
"%BIN%\Demo6_manage.exe" delete backup\old.bin
echo ^> dir /b backup
dir /b backup
pause

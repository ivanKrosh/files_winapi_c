@echo off
chcp 65001 > nul
rem ============================================================
rem  Демо 1. fread: скільки елементів прочитано.
rem  До показу спрогнозуйте: що поверне третій виклик fread, якщо оцінок 10?
rem ============================================================
cd /d "%~dp0..\Файли"
set "BIN=%~dp0..\x64\Debug"
if not exist "%BIN%\Demo1_fread.exe" ( echo [!] Зберіть рішення в конфігурації Debug. & exit /b 1 )
echo ^> dir marks.bin
dir marks.bin | find "marks.bin"
pause
echo.
echo ^> Demo1_fread.exe
"%BIN%\Demo1_fread.exe"
pause
echo.
echo === Той самий файл без "b" ===
echo   Прогноз: скільки оцінок прочитає fread у текстовому режимі?
pause
echo ^> Demo1_fread.exe text
"%BIN%\Demo1_fread.exe" text
echo.
echo   Дев'ята оцінка 26 = 0x1A (Ctrl+Z). У текстовому режимі бібліотека C вважає цей байт кінцем файлу.
pause

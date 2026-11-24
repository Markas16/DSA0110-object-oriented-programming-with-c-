@echo off
echo Compiling Task A3: Payer Hierarchy...
g++ -std=c++17 -Wall -Wextra payer_hierarchy.cpp -o payer_hierarchy.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] payer_hierarchy.exe:
    payer_hierarchy.exe
)
pause

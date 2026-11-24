@echo off
echo =======================================================================
echo Compiling Task B4: Enterprise Hospital System (-fsanitize=address)...
echo =======================================================================
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g enterprise_hospital_system.cpp -o enterprise_hospital_system.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] enterprise_hospital_system.exe:
    enterprise_hospital_system.exe
) else (
    echo Compilation failed!
)
pause

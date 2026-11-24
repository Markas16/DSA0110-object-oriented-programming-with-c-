@echo off
echo Compiling Task 1: College Payroll Redesign...
g++ -std=c++17 -Wall -Wextra payroll_redesign.cpp -o payroll_redesign.exe
if %ERRORLEVEL% equ 0 (
    echo Compilation successful! Running program...
    payroll_redesign.exe
) else (
    echo Compilation failed!
)

echo.
echo Compiling Task 1 Alternative Concept: Monolithic Switch...
g++ -std=c++17 -Wall -Wextra payroll_monolithic_alt.cpp -o payroll_monolithic_alt.exe
if %ERRORLEVEL% equ 0 (
    echo Compilation successful! Running alternative program...
    payroll_monolithic_alt.exe
)
pause

@echo off
echo =======================================================================
echo 1. Compiling Task B3: Virtual vs Switch vs Variant (-O2 Optimized)...
echo =======================================================================
g++ -std=c++17 -O2 -Wall -Wextra benchmark_virtual_vs_switch.cpp -o benchmark_virtual_vs_switch.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] benchmark_virtual_vs_switch.exe:
    benchmark_virtual_vs_switch.exe
)

echo.
echo =======================================================================
echo 2. Compiling Task B3: Exceptions vs Return Codes (-O2 Optimized)...
echo =======================================================================
g++ -std=c++17 -O2 -Wall -Wextra benchmark_exceptions_vs_codes.cpp -o benchmark_exceptions_vs_codes.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] benchmark_exceptions_vs_codes.exe:
    benchmark_exceptions_vs_codes.exe
)
pause

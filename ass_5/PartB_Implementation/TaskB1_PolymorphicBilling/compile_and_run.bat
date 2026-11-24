@echo off
echo ===================================================
echo 1. Compiling Task B1: Complete the Code (complete_billing.cpp)...
echo ===================================================
g++ -std=c++17 -Wall -Wextra complete_billing.cpp -o complete_billing.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] complete_billing.exe:
    complete_billing.exe
)

echo.
echo ===================================================
echo 2. Compiling Full Polymorphic Billing Matrix (full_billing_system.cpp)...
echo ===================================================
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g full_billing_system.cpp -o full_billing_system.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] full_billing_system.exe:
    full_billing_system.exe
)
pause

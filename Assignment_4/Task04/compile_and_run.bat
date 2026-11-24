@echo off
echo ===================================================
echo 1. Compiling Buggy Account with AddressSanitizer...
echo ===================================================
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g account_buggy.cpp -o account_buggy.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] account_buggy.exe:
    account_buggy.exe
)

echo.
echo ===================================================
echo 2. Compiling Repaired Account Hierarchy...
echo ===================================================
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g account_repaired.cpp -o account_repaired.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] account_repaired.exe:
    account_repaired.exe
)

echo.
echo ===================================================
echo 3. Compiling Predict the Output Program...
echo ===================================================
g++ -std=c++17 -Wall -Wextra account_predict.cpp -o account_predict.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] account_predict.exe:
    account_predict.exe
)
pause

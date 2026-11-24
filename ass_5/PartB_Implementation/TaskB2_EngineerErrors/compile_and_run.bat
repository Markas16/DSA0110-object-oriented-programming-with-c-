@echo off
echo ===================================================
echo 1. Compiling Task B2: Complete the Code (complete_claims.cpp)...
echo ===================================================
g++ -std=c++17 -Wall -Wextra complete_claims.cpp -o complete_claims.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] complete_claims.exe:
    complete_claims.exe
)

echo.
echo ===================================================
echo 2. Compiling Task B2: Predict the Output (predict_validate_process.cpp)...
echo ===================================================
g++ -std=c++17 -Wall -Wextra predict_validate_process.cpp -o predict_validate_process.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] predict_validate_process.exe:
    predict_validate_process.exe
)

echo.
echo ===================================================
echo 3. Compiling Task B2: Stress Testing Engineered Errors...
echo ===================================================
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g stress_error_conditions.cpp -o stress_error_conditions.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] stress_error_conditions.exe:
    stress_error_conditions.exe
)
pause

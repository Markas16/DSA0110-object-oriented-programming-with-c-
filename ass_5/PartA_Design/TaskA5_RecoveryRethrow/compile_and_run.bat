@echo off
echo Compiling Task A5: Predict the Output (predict_stack_unwinding.cpp)...
g++ -std=c++17 -Wall -Wextra predict_stack_unwinding.cpp -o predict_stack_unwinding.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] predict_stack_unwinding.exe:
    predict_stack_unwinding.exe
)
pause

@echo off
echo Compiling Task A2: Predict the Output (predict_vtable.cpp)...
g++ -std=c++17 -Wall -Wextra predict_vtable.cpp -o predict_vtable.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] predict_vtable.exe:
    predict_vtable.exe
)
pause

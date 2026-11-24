@echo off
echo ===================================================
echo 1. Compiling Complete the Code (student_complete.cpp)...
echo ===================================================
g++ -std=c++17 -Wall -Wextra student_complete.cpp -o student_complete.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] student_complete.exe:
    student_complete.exe
)

echo.
echo ===================================================
echo 2. Demonstrating -Wreorder Warning (reorder_warning_demo.cpp)...
echo ===================================================
g++ -std=c++17 -Wall -Wextra reorder_warning_demo.cpp -o reorder_warning_demo.exe

echo.
echo ===================================================
echo 3. Compiling Predict the Output (engine_vehicle_predict.cpp)...
echo ===================================================
g++ -std=c++17 -Wall -Wextra engine_vehicle_predict.cpp -o engine_vehicle_predict.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] engine_vehicle_predict.exe:
    engine_vehicle_predict.exe
)

echo.
echo ===================================================
echo 4. Compiling Construction & Destruction Trace (construction_trace.cpp)...
echo ===================================================
g++ -std=c++17 -Wall -Wextra construction_trace.cpp -o construction_trace.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] construction_trace.exe:
    construction_trace.exe
)
pause

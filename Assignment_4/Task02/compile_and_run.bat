@echo off
echo ===================================================
echo Compiling Task 2: University Hierarchy...
echo ===================================================
g++ -std=c++17 -Wall -Wextra university_hierarchy.cpp -o university_hierarchy.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] university_hierarchy.exe:
    university_hierarchy.exe
)

echo.
echo ===================================================
echo Compiling Task 2: Predict the Output (Virtual Base)...
echo ===================================================
g++ -std=c++17 -Wall -Wextra diamond_prediction.cpp -o diamond_prediction.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] diamond_prediction.exe:
    diamond_prediction.exe
)

echo.
echo ===================================================
echo Compiling Task 2: Non-Virtual Experiment...
echo ===================================================
g++ -std=c++17 -Wall -Wextra diamond_nonvirtual.cpp -o diamond_nonvirtual.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] diamond_nonvirtual.exe:
    diamond_nonvirtual.exe
)
pause

@echo off
echo =======================================================================
echo Compiling Task 6: Vehicle Rental Fleet System...
echo =======================================================================
g++ -std=c++17 -Wall -Wextra vehicle_rental_fleet.cpp -o vehicle_rental_fleet.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] vehicle_rental_fleet.exe:
    vehicle_rental_fleet.exe
) else (
    echo Compilation failed!
)
pause

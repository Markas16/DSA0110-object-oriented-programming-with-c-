@echo off
echo ===================================================
echo 1. Compiling Complete the Code (cart_complete.cpp)...
echo ===================================================
g++ -std=c++17 -Wall -Wextra cart_complete.cpp -o cart_complete.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] cart_complete.exe:
    cart_complete.exe
)

echo.
echo ===================================================
echo 2. Compiling Four Ways Demonstration (cart_four_ways.cpp)...
echo ===================================================
g++ -std=c++17 -Wall -Wextra cart_four_ways.cpp -o cart_four_ways.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] cart_four_ways.exe:
    cart_four_ways.exe
)

echo.
echo ===================================================
echo 3. Compiling Array of Pointers Search (cart_array_search.cpp)...
echo ===================================================
g++ -std=c++17 -Wall -Wextra cart_array_search.cpp -o cart_array_search.exe
if %ERRORLEVEL% equ 0 (
    echo [RUN] cart_array_search.exe:
    cart_array_search.exe
)
pause

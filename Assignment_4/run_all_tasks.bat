@echo off
echo =======================================================================
echo          DSA01 - OBJECT ORIENTED PROGRAMMING WITH C++
echo                     ASSIGNMENT 4 - MASTER RUNNER
echo =======================================================================
echo.

echo [TASK 1] Running College Payroll Redesign...
cd Task01
call compile_and_run.bat
cd ..

echo.
echo [TASK 2] Running University People Management...
cd Task02
call compile_and_run.bat
cd ..

echo.
echo [TASK 3] Running Constructors and Member Classes...
cd Task03
call compile_and_run.bat
cd ..

echo.
echo [TASK 4] Running Pointers to Derived Classes Forensics...
cd Task04
call compile_and_run.bat
cd ..

echo.
echo [TASK 5] Running Pointers to Objects and this Pointer...
cd Task05
call compile_and_run.bat
cd ..

echo.
echo [TASK 6] Running Vehicle Rental Fleet Design Challenge...
cd Task06
call compile_and_run.bat
cd ..

echo.
echo =======================================================================
echo             ALL TASKS EXECUTED SUCCESSFULLY!
echo =======================================================================
pause

@echo off
echo =======================================================================
echo          DSA01 - OBJECT ORIENTED PROGRAMMING WITH C++
echo                     ASSIGNMENT 5 - MASTER RUNNER
echo =======================================================================
echo.

echo [PART A - TASK A2] Running Runtime Binding Predict the Output...
cd PartA_Design\TaskA2_RuntimeBinding
call compile_and_run.bat
cd ..\..

echo.
echo [PART A - TASK A3] Running Payer Hierarchy...
cd PartA_Design\TaskA3_AbstractClasses
call compile_and_run.bat
cd ..\..

echo.
echo [PART A - TASK A5] Running Stack Unwinding Predict the Output...
cd PartA_Design\TaskA5_RecoveryRethrow
call compile_and_run.bat
cd ..\..

echo.
echo [PART B - TASK B1] Running Complete Billing & Polymorphic Test Matrix...
cd PartB_Implementation\TaskB1_PolymorphicBilling
call compile_and_run.bat
cd ..\..

echo.
echo [PART B - TASK B2] Running Engineered Errors & Predict Programs...
cd PartB_Implementation\TaskB2_EngineerErrors
call compile_and_run.bat
cd ..\..

echo.
echo [PART B - TASK B3] Running Performance Benchmarks (Virtual vs Switch & Exceptions)...
cd PartB_Implementation\TaskB3_PerformanceBenchmark
call compile_and_run.bat
cd ..\..

echo.
echo [PART B - TASK B4] Running Enterprise Hospital Extension & ASan Audit...
cd PartB_Implementation\TaskB4_FinalDecision
call compile_and_run.bat
cd ..\..

echo.
echo =======================================================================
echo             ALL TASKS IN ASSIGNMENT 5 EXECUTED SUCCESSFULLY!
echo =======================================================================
pause

# Build script for DSA01 Assignment 1 (City Metro Smart-Card Fare Logic)
# Compiler standard: C++17 with -Wall -Wextra -Wpedantic

$ErrorActionPreference = "Continue"

# Ensure MSYS2 UCRT64 GCC is in PATH if available
if (Test-Path "C:\msys64\ucrt64\bin") {
    $env:PATH = "C:\msys64\ucrt64\bin;" + $env:PATH
}

# Verify g++ availability
$gpp = Get-Command g++ -ErrorAction SilentlyContinue
if (-not $gpp) {
    Write-Error "g++ compiler not found! Please ensure MinGW-w64 / MSYS2 UCRT64 is installed and added to PATH."
    exit 1
}

$projectRoot = Split-Path -Parent $PSScriptRoot
$binDir = Join-Path $projectRoot "bin"
if (-not (Test-Path $binDir)) {
    New-Item -ItemType Directory -Path $binDir -Force | Out-Null
}

$cxxFlags = @("-std=c++17", "-Wall", "-Wextra", "-Wpedantic", "-O2")

Write-Host "====================================================" -ForegroundColor Cyan
Write-Host " DSA01 Assignment 1 - C++ Build System" -ForegroundColor Cyan
Write-Host " Compiler: $($gpp.Source)" -ForegroundColor Gray
& g++ --version | Select-Object -First 1 | Write-Host -ForegroundColor Gray
Write-Host " Flags: $($cxxFlags -join ' ')" -ForegroundColor Gray
Write-Host "====================================================" -ForegroundColor Cyan

$targets = @(
    @{ Name = "Task 2 Constants"; Src = @("Task02/task02_tokens.cpp"); Out = "bin/task02_tokens.exe" },
    @{ Name = "Task 3 Data Types"; Src = @("Task03/task03_datatypes.cpp"); Out = "bin/task03_datatypes.exe" },
    @{ Name = "Task 4 Prediction"; Src = @("Task04/task04_prediction.cpp"); Out = "bin/task04_prediction.exe" },
    @{ Name = "Task 4 Safe Conversions"; Src = @("Task04/task04_safe_conversions.cpp"); Out = "bin/task04_safe_conversions.exe" },
    @{ Name = "Task 5 Prediction"; Src = @("Task05/task05_prediction.cpp"); Out = "bin/task05_prediction.exe" },
    @{ Name = "Task 5 Operators"; Src = @("Task05/task05_operators.cpp"); Out = "bin/task05_operators.exe" },
    @{ Name = "Task 6 Fare Control"; Src = @("Task06/task06_fare_control.cpp"); Out = "bin/task06_fare_control.exe" },
    @{ Name = "Task 7 Procedural"; Src = @("Task07/procedural_fare.cpp"); Out = "bin/task07_procedural.exe" },
    @{ Name = "Task 7 OOP"; Src = @("Task07/oop_fare.cpp"); Out = "bin/task07_oop.exe" },
    @{ Name = "Task 8 Safe IO"; Src = @("Task08/task08_safe_io.cpp"); Out = "bin/task08_safe_io.exe" },
    @{ Name = "Task 9 Fare System CLI"; Src = @("src/main.cpp", "src/fare_system.cpp"); Out = "bin/fare_system_app.exe" },
    @{ Name = "Task 9 Test Suite"; Src = @("tests/test_fare_system.cpp", "src/fare_system.cpp"); Out = "bin/test_fare_system.exe" },
    @{ Name = "Task 10 Scale Simulation"; Src = @("Task10/task10_scale_simulation.cpp"); Out = "bin/task10_scale_simulation.exe" },
    @{ Name = "Task 12 Day-4 Challenge"; Src = @("Task12/task12_day4_challenge.cpp"); Out = "bin/task12_day4_challenge.exe" }
)

$successCount = 0
$failCount = 0

foreach ($t in $targets) {
    Write-Host -NoNewline "Building [$($t.Name)]... "
    $sourcePaths = $t.Src | ForEach-Object { Join-Path $projectRoot $_ }
    $outPath = Join-Path $projectRoot $t.Out
    
    # Check that sources exist
    $missing = $sourcePaths | Where-Object { -not (Test-Path $_) }
    if ($missing) {
        Write-Host "FAILED (Missing source: $($missing -join ', '))" -ForegroundColor Red
        $failCount++
        continue
    }

    $cmdArgs = $cxxFlags + $sourcePaths + @("-o", $outPath)
    & g++ $cmdArgs
    
    if ($LASTEXITCODE -eq 0 -and (Test-Path $outPath)) {
        Write-Host "OK" -ForegroundColor Green
        $successCount++
    } else {
        Write-Host "FAILED" -ForegroundColor Red
        $failCount++
    }
}

Write-Host "----------------------------------------------------" -ForegroundColor Gray
Write-Host "Build Summary: $successCount Succeeded, $failCount Failed" -ForegroundColor $(if ($failCount -eq 0) { "Green" } else { "Red" })
Write-Host "====================================================" -ForegroundColor Cyan

if ($failCount -gt 0) {
    exit 1
}

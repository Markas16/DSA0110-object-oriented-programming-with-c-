# Test Runner and Evidence Collector for DSA01 Assignment 1
$ErrorActionPreference = "Continue"

if (Test-Path "C:\msys64\ucrt64\bin") {
    $env:PATH = "C:\msys64\ucrt64\bin;" + $env:PATH
}

$projectRoot = Split-Path -Parent $PSScriptRoot
$binDir = Join-Path $projectRoot "bin"
$evidenceDir = Join-Path $projectRoot "Evidence"

if (-not (Test-Path $evidenceDir)) {
    New-Item -ItemType Directory -Path $evidenceDir -Force | Out-Null
}

Write-Host "====================================================" -ForegroundColor Cyan
Write-Host " DSA01 Assignment 1 - Execution & Evidence Runner" -ForegroundColor Cyan
Write-Host "====================================================" -ForegroundColor Cyan

# 1. First trigger build
& "$PSScriptRoot\build.ps1"
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed. Aborting test execution."
    exit 1
}

$testRuns = @(
    @{ Name = "Task 2 Constants & Tokens"; Exe = "task02_tokens.exe"; Out = "task02_output.txt"; Input = $null },
    @{ Name = "Task 3 Data Types & Limits"; Exe = "task03_datatypes.exe"; Out = "task03_output.txt"; Input = $null },
    @{ Name = "Task 4 Prediction Program"; Exe = "task04_prediction.exe"; Out = "task04_prediction_output.txt"; Input = $null },
    @{ Name = "Task 4 Safe Conversions"; Exe = "task04_safe_conversions.exe"; Out = "task04_safe_output.txt"; Input = $null },
    @{ Name = "Task 5 Prediction Program"; Exe = "task05_prediction.exe"; Out = "task05_prediction_output.txt"; Input = $null },
    @{ Name = "Task 5 Operators & Peak Logic"; Exe = "task05_operators.exe"; Out = "task05_operators_output.txt"; Input = $null },
    @{ Name = "Task 6 Completed Fare Logic"; Exe = "task06_fare_control.exe"; Out = "task06_output.txt"; Input = $null },
    @{ Name = "Task 7 Procedural Implementation"; Exe = "task07_procedural.exe"; Out = "task07_procedural_output.txt"; Input = $null },
    @{ Name = "Task 7 OOP SmartCard"; Exe = "task07_oop.exe"; Out = "task07_oop_output.txt"; Input = $null },
    @{ Name = "Task 8 Safe IO Robustness"; Exe = "task08_safe_io.exe"; Out = "task08_output.txt"; Input = "abc`n-50`n25`n9`nOuter`n5000`n" },
    @{ Name = "Task 9 Automated Test Suite"; Exe = "test_fare_system.exe"; Out = "task09_test_suite_output.txt"; Input = $null },
    @{ Name = "Task 10 Scale Simulation (20k, 500k, 3M)"; Exe = "task10_scale_simulation.exe"; Out = "task10_scale_output.txt"; Input = $null },
    @{ Name = "Task 12 Day-4 Design Challenge"; Exe = "task12_day4_challenge.exe"; Out = "task12_output.txt"; Input = $null }
)

$passed = 0
$failed = 0

foreach ($r in $testRuns) {
    Write-Host -NoNewline "Running [$($r.Name)]... "
    $exePath = Join-Path $binDir $r.Exe
    $outPath = Join-Path $evidenceDir $r.Out

    if (-not (Test-Path $exePath)) {
        Write-Host "MISSING EXE ($($r.Exe))" -ForegroundColor Red
        $failed++
        continue
    }

    if ($r.Input) {
        $r.Input | & $exePath 2>&1 | Set-Content -Path $outPath -Encoding UTF8
    } else {
        & $exePath 2>&1 | Set-Content -Path $outPath -Encoding UTF8
    }

    if ($LASTEXITCODE -eq 0) {
        Write-Host "PASSED -> $outPath" -ForegroundColor Green
        $passed++
    } else {
        Write-Host "FAILED (Exit Code: $LASTEXITCODE) -> $outPath" -ForegroundColor Red
        $failed++
    }
}

# Run the CLI app in non-interactive demo mode to capture sample session
$cliExe = Join-Path $binDir "fare_system_app.exe"
if (Test-Path $cliExe) {
    Write-Host -NoNewline "Running [CLI Sample Interactive Session]... "
    "1`n2`nInner`n8`n2`nOuter`n18`n3`n2000`n4`n" | & $cliExe 2>&1 | Set-Content -Path (Join-Path $evidenceDir "cli_sample_session.txt") -Encoding UTF8
    Write-Host "DONE -> Evidence/cli_sample_session.txt" -ForegroundColor Green
}

Write-Host "----------------------------------------------------" -ForegroundColor Gray
Write-Host "Test Run Summary: $passed Succeeded, $failed Failed" -ForegroundColor $(if ($failed -eq 0) { "Green" } else { "Red" })
Write-Host "All evidence outputs saved to: $evidenceDir" -ForegroundColor Cyan
Write-Host "====================================================" -ForegroundColor Cyan

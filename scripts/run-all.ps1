#!/usr/bin/env pwsh
#
# Compile and run every solution in the repository.
#
#   .\scripts\run-all.ps1              # all of them
#   .\scripts\run-all.ps1 easy medium  # only the given difficulties
#
# Building is delegated to scripts\build.ps1, so CXX and CXXSTD work here too. A
# solution reports its checks as PASS/FAIL lines rather than through its exit
# code, so a run counts as failed if the process fails *or* if it printed FAIL.

[CmdletBinding()]
param(
    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$Difficulties = @()
)

Set-StrictMode -Version Latest

# Compiler warnings reach us as error records once build.ps1's output is
# captured below, and a solution that crashes is a result to report rather than
# an exception to propagate. Failure is read from exit codes throughout.
$ErrorActionPreference = 'Continue'
$PSNativeCommandUseErrorActionPreference = $false

function Write-Err {
    param([string]$Message)
    [Console]::Error.WriteLine($Message)
}

# Run a command and hand back everything it printed as one string.
#
# Merging stderr into the pipeline turns each line a native program wrote there
# into an error record, which Windows PowerShell would otherwise render with the
# script line and a CategoryInfo block wrapped around it. Compiler diagnostics
# are worth reading as the compiler wrote them, so unwrap them back to text.
function Invoke-Captured {
    param([string]$Command, [string[]]$Arguments = @())

    $lines = & $Command @Arguments 2>&1 | ForEach-Object {
        if ($_ -is [System.Management.Automation.ErrorRecord]) { $_.ToString() } else { [string]$_ }
    }
    return (@($lines) -join [Environment]::NewLine).TrimEnd()
}

$root = (Resolve-Path (Join-Path $PSScriptRoot '..')).ProviderPath
$build = Join-Path $PSScriptRoot 'build.ps1'

if ($Difficulties.Count -eq 0) { $Difficulties = @('easy', 'medium', 'hard') }

Push-Location $root
try {
    # Kept as repo-relative paths with forward slashes so the log reads the same
    # as the one the shell script produces.
    $sources = @(
        foreach ($difficulty in $Difficulties) {
            $dir = Join-Path $root "solutions\$difficulty"
            if (-not (Test-Path -LiteralPath $dir -PathType Container)) { continue }
            Get-ChildItem -LiteralPath $dir -File -Filter '*.cpp' |
                Sort-Object Name |
                ForEach-Object { "solutions/$difficulty/$($_.Name)" }
        }
    )

    if ($sources.Count -eq 0) {
        Write-Err "no solutions found for: $($Difficulties -join ' ')"
        exit 1
    }

    $passed = 0
    $failed = @()

    foreach ($src in $sources) {
        Write-Output "::group::$src"
        Write-Output "=== $src"

        $output = Invoke-Captured $build @($src)
        if ($LASTEXITCODE -ne 0) {
            Write-Output $output
            $failed += "$src (compile)"
            Write-Output '::endgroup::'
            continue
        }

        $binary = Join-Path $root ($src -replace '\.cpp$', '.exe')
        $obj = Join-Path $root ($src -replace '\.cpp$', '.obj')

        $output = Invoke-Captured $binary
        Write-Output $output

        if ($LASTEXITCODE -ne 0) {
            $failed += "$src (crashed or exited non-zero)"
        }
        elseif ($output -match 'FAIL') {
            $failed += "$src (check failed)"
        }
        else {
            $passed++
        }

        Remove-Item -LiteralPath $binary, $obj -Force -ErrorAction SilentlyContinue
        Write-Output '::endgroup::'
    }

    Write-Output ''
    Write-Output "$passed/$($sources.Count) solutions passed"

    if ($failed.Count -gt 0) {
        Write-Err 'failed:'
        foreach ($entry in $failed) { Write-Err "  $entry" }
        exit 1
    }
}
finally {
    Pop-Location
}

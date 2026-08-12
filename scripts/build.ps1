#!/usr/bin/env pwsh
#
# Build one solution file.
#
#   .\scripts\build.ps1 solutions\easy\0001-two-sum.cpp [extra compiler flags...]
#   .\scripts\build.ps1 0001-two-sum     # searched for under the repo
#
# Runs from any working directory: paths are tried as given, then relative to
# the repo root, then matched by name against the .cpp files in the repo.
#
# The executable is written next to the source and named after it. Set CXX to
# pick a specific compiler; otherwise the first of g++, clang++ or cl that is on
# PATH is used. Set CXXSTD to force a standard instead of probing for the
# newest one the compiler accepts.

[CmdletBinding()]
param(
    [Parameter(Position = 0)]
    [string]$Solution,

    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$CompilerFlags = @()
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

# A compiler that rejects a flag is a result to inspect, not an exception to
# throw: the native calls below are checked through $LASTEXITCODE instead. The
# variable only exists in PowerShell 7.3+; setting it under 5.1 is harmless.
$PSNativeCommandUseErrorActionPreference = $false

# Diagnostics of our own go straight to the process's stderr rather than through
# Write-Error, which would dress them up as multi-line error records.
function Write-Err {
    param([string]$Message)
    [Console]::Error.WriteLine($Message)
}

if (-not $Solution) {
    Write-Err "usage: $(Split-Path -Leaf $PSCommandPath) <path-to-solution>.cpp [compiler flags...]"
    exit 2
}

# Where the script itself lives, following a symlink if we were invoked through
# one. Everything below is anchored here rather than to the working directory.
$self = $PSCommandPath
while ($true) {
    $item = Get-Item -LiteralPath $self -Force
    if (-not ($item.Attributes -band [System.IO.FileAttributes]::ReparsePoint)) { break }
    $link = @($item.Target)[0]
    if (-not $link) { break }
    if ([System.IO.Path]::IsPathRooted($link)) { $self = $link }
    else { $self = Join-Path (Split-Path -Parent $self) $link }
}
$root = (Resolve-Path (Join-Path (Split-Path -Parent $self) '..')).ProviderPath

function Resolve-Solution {
    param([string]$Query)

    # As typed, relative to the working directory or absolute.
    if (Test-Path -LiteralPath $Query -PathType Leaf) {
        return (Resolve-Path -LiteralPath $Query).ProviderPath
    }

    # Relative to the repo root.
    $atRoot = Join-Path $root $Query
    if (Test-Path -LiteralPath $atRoot -PathType Leaf) {
        return (Resolve-Path -LiteralPath $atRoot).ProviderPath
    }

    # By name anywhere in the repo, with or without the extension. Sorted so a
    # given input always picks the same file.
    $base = $Query -replace '\.cpp$', ''
    $hits = @(
        Get-ChildItem -LiteralPath $root -Recurse -File -Filter "$base.cpp" |
            Where-Object { $_.FullName -notmatch '(^|\\)\.git(\\|$)' } |
            Sort-Object FullName
    )

    if ($hits.Count -eq 1) { return $hits[0].FullName }
    if ($hits.Count -gt 1) {
        Write-Err "$Query is ambiguous:"
        foreach ($hit in $hits) { Write-Err "  $($hit.FullName)" }
    }
    return $null
}

$src = Resolve-Solution $Solution
if (-not $src) {
    Write-Err "${Solution}: no such file under $root"
    exit 1
}

$cxx = $env:CXX
if (-not $cxx) {
    foreach ($candidate in 'g++', 'clang++', 'cl') {
        if (Get-Command $candidate -CommandType Application -ErrorAction SilentlyContinue) {
            $cxx = $candidate
            break
        }
    }
}
if (-not $cxx) {
    Write-Err 'no C++ compiler on PATH; set CXX'
    exit 1
}
# CXX may name something that is not there. Saying so once beats letting every
# invocation below fail with its own CommandNotFoundException.
if (-not (Get-Command $cxx -ErrorAction SilentlyContinue)) {
    Write-Err "${cxx}: not found on PATH (from CXX)"
    exit 1
}

$out = [System.IO.Path]::ChangeExtension($src, 'exe')
$obj = [System.IO.Path]::ChangeExtension($src, 'obj')

# From here on the compiler does the talking, and everything it writes to stderr
# — warnings included — becomes an error record in a caller that captures this
# script with 2>&1. Under 'Stop' the first warning would end the run before the
# exit code is ever read, so leave those records non-terminating.
$ErrorActionPreference = 'Continue'

# Compilers older than the flag they are handed fail on it, so ask for the
# newest standard this one actually accepts.
function Select-Std {
    param([string]$Prefix)

    # Probed against a throwaway file rather than stdin: piping into a native
    # command hands it whatever encoding the console happens to use, BOM and all.
    $probe = Join-Path ([System.IO.Path]::GetTempPath()) "leetcode-std-probe-$PID.cpp"
    Set-Content -LiteralPath $probe -Value 'int main(){}' -Encoding Ascii
    try {
        foreach ($std in 'c++23', 'c++20', 'c++17') {
            & $cxx "$Prefix$std" -fsyntax-only $probe 2>$null | Out-Null
            if ($LASTEXITCODE -eq 0) { return $std }
        }
    } finally {
        Remove-Item -LiteralPath $probe -Force -ErrorAction SilentlyContinue
    }
    return $null
}

if ([System.IO.Path]::GetFileNameWithoutExtension($cxx) -eq 'cl') {
    $std = if ($env:CXXSTD) { $env:CXXSTD } else { 'c++latest' }
    # cl scatters its output over the working directory unless told where to put
    # it, so name both the executable and the object file explicitly.
    & $cxx -nologo "-std:$std" -EHsc -O2 @CompilerFlags $src "-Fe:$out" "-Fo:$obj"
}
else {
    $std = $env:CXXSTD
    if (-not $std) {
        $std = Select-Std '-std='
        if (-not $std) { $std = 'c++17' }
    }
    & $cxx "-std=$std" -O2 -Wall -Wextra -o $out $src @CompilerFlags
}

if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Output "built $out"

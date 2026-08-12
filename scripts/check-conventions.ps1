#!/usr/bin/env pwsh
#
# Check that every solution follows the repository conventions:
#
#   * it lives in solutions\<easy|medium|hard>\
#   * it is named <4-digit-problem-number>-<title-slug>.cpp
#   * it opens with a header comment naming the problem, difficulty and URL
#   * the number and difficulty in the header match the ones in the path
#
# Prints one line per violation and exits non-zero if there was any.

[CmdletBinding()]
param()

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$root = (Resolve-Path (Join-Path $PSScriptRoot '..')).ProviderPath

$status = 0
function Add-Failure {
    param([string]$Message)
    [Console]::Error.WriteLine($Message)
    $script:status = 1
}

# Paths are reported relative to the repo root, forward-slashed, so the output
# does not depend on where the script was started from.
function Get-RelativePath {
    param([string]$Path)
    './' + $Path.Substring($root.Length).TrimStart('\', '/').Replace('\', '/')
}

$solutionRoots = @('easy', 'medium', 'hard') | ForEach-Object { Join-Path $root "solutions\$_" }

# A .cpp anywhere else is either a stray file or a directory that was never
# meant to hold solutions; both are worth reporting.
$allSources = @(
    Get-ChildItem -LiteralPath $root -Recurse -File -Filter '*.cpp' |
        Where-Object { $_.FullName -notmatch '(^|\\)\.git(\\|$)' } |
        Sort-Object FullName
)
foreach ($file in $allSources) {
    if ($solutionRoots -notcontains $file.DirectoryName) {
        Add-Failure "$(Get-RelativePath $file.FullName): solutions belong in solutions/{easy,medium,hard}/"
    }
}

$solutions = @(
    Get-ChildItem -LiteralPath (Join-Path $root 'solutions') -Directory -ErrorAction SilentlyContinue |
        Sort-Object Name |
        ForEach-Object { Get-ChildItem -LiteralPath $_.FullName -File -Filter '*.cpp' | Sort-Object Name }
)

foreach ($file in $solutions) {
    $difficulty = Split-Path -Leaf $file.DirectoryName
    $name = $file.Name
    $relative = "solutions/$difficulty/$name"

    if ($name -notmatch '^([0-9]{4})-([a-z0-9]+(-[a-z0-9]+)*)\.cpp$') {
        Add-Failure "${relative}: name must be <NNNN>-<title-slug>.cpp"
        continue
    }
    $number = $Matches[1]
    $slug = $Matches[2]

    # Leading zeros are padding for sorting, not part of the problem number, so
    # the header carries the number as LeetCode writes it.
    $unpadded = [int]$number
    $titled = $difficulty.Substring(0, 1).ToUpperInvariant() + $difficulty.Substring(1)

    $header = @(Get-Content -LiteralPath $file.FullName -TotalCount 2)
    $expectedTitle = "^// $unpadded\. .+ \($titled\)$"
    $expectedUrl = "^// https://leetcode\.com/problems/$slug/$"

    if ($header.Count -lt 1 -or $header[0] -notmatch $expectedTitle) {
        Add-Failure "${relative}: first line must be '// $unpadded. <Title> ($titled)'"
    }
    if ($header.Count -lt 2 -or $header[1] -notmatch $expectedUrl) {
        Add-Failure "${relative}: second line must be '// https://leetcode.com/problems/$slug/'"
    }
}

# Two files claiming the same problem number is nearly always a copy that was
# renamed but never finished.
$duplicates = @(
    $solutions |
        Group-Object { ($_.Name -split '-')[0] } |
        Where-Object { $_.Count -gt 1 } |
        Sort-Object Name
)
foreach ($group in $duplicates) {
    Add-Failure "problem $($group.Name) has more than one solution file"
}

if ($status -eq 0) {
    Write-Output "conventions OK: $($solutions.Count) solutions"
}
exit $status

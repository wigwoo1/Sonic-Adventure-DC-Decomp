param(
    [Parameter(Mandatory = $true)][ValidatePattern('^[A-Z0-9_]+\.BIN$')][string]$ModuleName,
    [string]$LoadAddress = '0x8c900000',
    [ValidatePattern('^\d+\.\d+\.\d+_PUBLIC$')][string]$GhidraVersion = '12.1.4_PUBLIC',
    [string]$Workspace
)
if ([string]::IsNullOrWhiteSpace($Workspace)) {
    $Workspace = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
}
$ErrorActionPreference = 'Stop'
$toolRoot = Join-Path $Workspace 'work/sonic-adventure/toolchain'
$ghidraRoot = Join-Path $toolRoot "ghidra_$GhidraVersion"
$jdkRoot = Get-ChildItem $toolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
if (!(Test-Path -LiteralPath $ghidraRoot) -or !$jdkRoot) { throw "Portable Ghidra $GhidraVersion and JDK have not been extracted." }
$env:JAVA_HOME = $jdkRoot.FullName
$runId = [guid]::NewGuid().ToString('N')
$env:GHIDRA_USER_HOME = Join-Path $toolRoot "user-home-$GhidraVersion-$runId"
New-Item -ItemType Directory -Force $env:GHIDRA_USER_HOME | Out-Null
$env:_JAVA_OPTIONS = '-Duser.home="' + $env:GHIDRA_USER_HOME + '"'
$projectRoot = Join-Path $Workspace 'work/sonic-adventure/ghidra-projects'
$scriptRoot = Join-Path $Workspace 'work/sonic-adventure/ghidra-scripts'
$module = Join-Path $Workspace "work/sonic-adventure/decoded/SONICADV/$ModuleName"
$outputRoot = Join-Path $Workspace 'outputs/sonic-adventure/analysis/additional-modules'
$output = $outputRoot
$moduleOutput = Join-Path $outputRoot $ModuleName
$projectName = 'SonicAdventureModule_' + [IO.Path]::GetFileNameWithoutExtension($ModuleName)
$logPath = Join-Path $Workspace "work/sonic-adventure/ghidra-module-$ModuleName.log"
if (!(Test-Path -LiteralPath $module)) { throw "Decoded module not found: $module" }
New-Item -ItemType Directory -Force $projectRoot, $scriptRoot, $outputRoot | Out-Null
$outputRootFull = [IO.Path]::GetFullPath($outputRoot).TrimEnd([IO.Path]::DirectorySeparatorChar) + [IO.Path]::DirectorySeparatorChar
$moduleOutputFull = [IO.Path]::GetFullPath($moduleOutput)
if (!$moduleOutputFull.StartsWith($outputRootFull, [StringComparison]::OrdinalIgnoreCase)) { throw "Refusing to clear output outside module index directory: $moduleOutputFull" }
if (Test-Path -LiteralPath $moduleOutputFull) { Remove-Item -LiteralPath $moduleOutputFull -Recurse -Force }
Remove-Item -LiteralPath $logPath -Force -ErrorAction SilentlyContinue
Copy-Item (Join-Path $PSScriptRoot '*.java') $scriptRoot -Force
& (Join-Path $ghidraRoot 'support/analyzeHeadless.bat') $projectRoot $projectName -import $module -overwrite -loader BinaryLoader -loader-baseAddr $LoadAddress -processor 'SuperH4:LE:32:default' -cspec default -analysisTimeoutPerFile 300 -preScript SeedOverlay.java -scriptPath $scriptRoot -postScript ExportInitialDecompilation.java $output 0 per-program -max-cpu 4 -log $logPath
if ($LASTEXITCODE -ne 0) { throw "Ghidra $ModuleName analysis failed: $LASTEXITCODE" }
if (!(Test-Path -LiteralPath $logPath)) { throw "Ghidra did not produce an analysis log for $ModuleName." }
if (Select-String -LiteralPath $logPath -Pattern 'ERROR REPORT SCRIPT ERROR' -Quiet) { throw "Ghidra $ModuleName analysis reported a script failure." }
$index = Join-Path $moduleOutput 'function_index.tsv'
if (!(Test-Path -LiteralPath $index)) { throw "Ghidra $ModuleName analysis did not create function_index.tsv." }
$rows = (Import-Csv -LiteralPath $index -Delimiter "`t").Count
if ($rows -lt 1) {
    Remove-Item -LiteralPath $moduleOutputFull -Recurse -Force -ErrorAction SilentlyContinue
    throw "Ghidra $ModuleName produced an empty function index; the empty output was removed and is not counted as indexed."
}
Write-Output "Ghidra indexed $rows functions in $ModuleName at $LoadAddress. Draft pseudocode remains unverified."

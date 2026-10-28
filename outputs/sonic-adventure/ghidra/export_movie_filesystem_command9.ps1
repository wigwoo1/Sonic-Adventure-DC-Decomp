param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskTools=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskTools 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskTools -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskTools 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskScripts=Join-Path $Workspace 'outputs/sonic-adventure/ghidra'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/movie-filesystem-command9'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $taskScripts -postScript SeedCallbackFunctions.java 8c64a828 8c64a3fa 8c64a7ba 8c64a6ae 8c64a6fc 8c64b080 8c64a74e -postScript ExportFailedFunctionEvidence.java $taskOutput 8c64a828 8c64a3fa 8c64a7ba 8c64a6ae 8c64a6fc 8c64b080 8c64a74e -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Filesystem command9 handler export failed'}
$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 7 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Filesystem command9 handler export incomplete'}

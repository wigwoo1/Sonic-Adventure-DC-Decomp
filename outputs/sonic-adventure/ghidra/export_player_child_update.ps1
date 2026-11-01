param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskTools=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskTools 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskTools -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskTools 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskScripts=Join-Path $Workspace 'outputs/sonic-adventure/ghidra'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/player-child-update'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $taskScripts -preScript SeedCallbackFunctions.java 8c0b3b1a 8c0b3afc 8c0b3f2c 8c0b33ce 8c0b37f2 8c0b3480 8c09e778 8c098786 8c07cfde 8c0b33a0 8c09e880 8c09eb08 8c606a88 8c6064b0 8c6065b0 8c09ee44 -postScript ExportFailedFunctionEvidence.java $taskOutput 8c0b3b1a 8c0b3afc 8c0b3f2c 8c0b33ce 8c0b37f2 8c0b3480 8c09e778 8c098786 8c07cfde 8c0b33a0 8c09e880 8c09eb08 8c606a88 8c6064b0 8c6065b0 8c09ee44 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Player child update export failed'}
$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 16 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Player child update export incomplete'}








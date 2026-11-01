param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskTools=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskTools 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskTools -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskTools 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskScripts=Join-Path $Workspace 'outputs/sonic-adventure/ghidra'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/player-texture-decode-family'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $taskScripts -preScript SeedCallbackFunctions.java 8c606f14 8c606f20 8c606f42 8c606fe4 8c607244 8c607ebc 8c607f90 8c6080c8 8c64da9e 8c64df66 8c64e61e 8c64e7a2 8c64e828 8c64e870 8c64e8e4 8c64edfc 8c64ee32 8c64ef64 -postScript ExportFailedFunctionEvidence.java $taskOutput 8c606f14 8c606f20 8c606f42 8c606fe4 8c607244 8c607ebc 8c607f90 8c6080c8 8c64da9e 8c64df66 8c64e61e 8c64e7a2 8c64e828 8c64e870 8c64e8e4 8c64edfc 8c64ee32 8c64ef64 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Player texture decode family export failed'}
$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 18 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Player texture decode family export incomplete'}








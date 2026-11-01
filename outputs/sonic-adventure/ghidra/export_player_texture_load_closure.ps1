param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskTools=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskTools 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskTools -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskTools 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskScripts=Join-Path $Workspace 'outputs/sonic-adventure/ghidra'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/player-texture-load-closure'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $taskScripts -preScript SeedCallbackFunctions.java 8c098ee0 8c606f66 8c606f6c 8c64da90 8c64db42 8c64dbbc 8c64df10 8c64e9b2 8c64ee8c 8c64ef08 8c660e80 -postScript ExportFailedFunctionEvidence.java $taskOutput 8c098ee0 8c606f66 8c606f6c 8c64da90 8c64db42 8c64dbbc 8c64df10 8c64e9b2 8c64ee8c 8c64ef08 8c660e80 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Player texture-loading closure export failed'}
$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 11 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Player texture-loading closure export incomplete'}








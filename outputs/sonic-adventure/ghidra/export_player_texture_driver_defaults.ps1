param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskTools=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskTools 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskTools -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskTools 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskScripts=Join-Path $Workspace 'outputs/sonic-adventure/ghidra'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/player-texture-driver-defaults'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $taskScripts -preScript SeedCallbackFunctions.java 8c641698 8c64169c 8c6416a0 8c6416a4 8c6416e0 8c6416e4 8c6416e8 8c6416ec 8c6416f0 -postScript ExportFailedFunctionEvidence.java $taskOutput 8c641698 8c64169c 8c6416a0 8c6416a4 8c6416e0 8c6416e4 8c6416e8 8c6416ec 8c6416f0 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Player texture driver default callbacks export failed'}
$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 9 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Player texture driver default callbacks export incomplete'}








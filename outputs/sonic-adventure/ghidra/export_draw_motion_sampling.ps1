param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/draw-motion-sampling'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript SeedCallbackFunctions.java 8c03feb8 8c040a60 8c03ff2c 8c03ff90 8c040c5c 8c040d60 8c60b596 8c60b4a8 8c60b81c -postScript ExportFailedFunctionEvidence.java $taskOutput 8c03feb8 8c040a60 8c03ff2c 8c03ff90 8c040c5c 8c040d60 8c60b596 8c60b4a8 8c60b81c -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Motion sampling read-only export failed'}

$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 9 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Motion sampling export diagnostics incomplete'}

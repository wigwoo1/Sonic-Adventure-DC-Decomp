param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/boot-file-driver'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript SeedCallbackFunctions.java 8c644fe8 8c644ff6 8c6451a4 8c6451c8 8c6451fc 8c6452b4 8c6452d6 8c6452ea 8c645328 8c6455ce 8c600464 8c5ffa08 8c600d82 8c6013a4 -postScript ExportFailedFunctionEvidence.java $taskOutput 8c644fe8 8c644ff6 8c6451a4 8c6451c8 8c6451fc 8c6452b4 8c6452d6 8c6452ea 8c645328 8c6455ce 8c600464 8c5ffa08 8c600d82 8c6013a4 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Boot file read-only export failed'}

$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 14 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Boot file export diagnostics incomplete'}

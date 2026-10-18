param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/draw-primitive-frontier'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript SeedCallbackFunctions.java 8c037990 8c037c3c 8c037ccc 8c038668 8c037ee0 8c038802 8c0382ac 8c60b462 8c037ff4 8c038516 -postScript ExportFailedFunctionEvidence.java $taskOutput 8c037990 8c037c3c 8c037ccc 8c038668 8c037ee0 8c038802 8c0382ac 8c60b462 8c037ff4 8c038516 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Material/primitive frontier read-only export failed'}

$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 10 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Material/primitive frontier export diagnostics incomplete'}

param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/draw-material-frontier'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript SeedCallbackFunctions.java 8c037618 8c037638 8c03767c 8c0376b0 8c037460 8c0374d4 8c037538 8c0375b0 8c037872 8c037990 8c037c3c 8c037ccc 8c038668 -postScript ExportFailedFunctionEvidence.java $taskOutput 8c037618 8c037638 8c03767c 8c0376b0 8c037460 8c0374d4 8c037538 8c0375b0 8c037872 8c037990 8c037c3c 8c037ccc 8c038668 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Material/primitive frontier read-only export failed'}

$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 13 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Material/primitive frontier export diagnostics incomplete'}

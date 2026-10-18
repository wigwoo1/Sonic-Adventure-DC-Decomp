param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/draw-frame-dependencies'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript SeedCallbackFunctions.java 8c10d39c 8c60526a 8c6054c6 8c60551e 8c60556c 8c6055ba 8c6055d4 8c608c2a 8c60ede0 8c60ede6 8c60ee0a 8c638fe4 8c63fb18 8c63fdc0 8c63fe20 8c63fe3c 8c63fe48 8c64036c 8c6403b0 8c640500 8c640528 8c640540 8c64075a -postScript ExportFailedFunctionEvidence.java $taskOutput 8c10d39c 8c60526a 8c6054c6 8c60551e 8c60556c 8c6055ba 8c6055d4 8c608c2a 8c60ede0 8c60ede6 8c60ee0a 8c638fe4 8c63fb18 8c63fdc0 8c63fe20 8c63fe3c 8c63fe48 8c64036c 8c6403b0 8c640500 8c640528 8c640540 8c64075a -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Frame dependency read-only export failed'}

$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 23 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Frame dependency export diagnostics incomplete'}

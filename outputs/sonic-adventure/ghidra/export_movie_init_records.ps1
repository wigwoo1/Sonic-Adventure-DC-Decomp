param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/movie-init-records'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript SeedCallbackFunctions.java 8c093e60 8c093c74 8c092fe8 8c094e20 8c09306c -postScript ExportFailedFunctionEvidence.java $taskOutput 8c093e60 8c093c74 8c092fe8 8c094e20 8c09306c -postScript ExportOverlaySelectorReferences.java (Join-Path $taskOutput 'references.tsv') 8c093e60 8c093c74 8c092fe8 8c094e20 8c09306c -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Original initializer caller read-only export failed'}
$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 5 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Initializer caller diagnostics incomplete'}

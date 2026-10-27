param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskScript=Join-Path $Workspace 'outputs/sonic-adventure/ghidra'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/movie-record-address'
$taskEntries=@('8c601874','8c601f5a')
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $taskScript -postScript SeedCallbackFunctions.java @taskEntries -postScript ExportFailedFunctionEvidence.java $taskOutput @taskEntries -postScript ExportOverlaySelectorReferences.java (Join-Path $taskOutput 'references.tsv') @taskEntries -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Next601874 export failed'}
$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 2 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Next601874 diagnostic incomplete'}

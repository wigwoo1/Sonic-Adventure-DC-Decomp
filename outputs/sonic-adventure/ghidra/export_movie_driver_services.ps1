param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/movie-driver-services'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript SeedCallbackFunctions.java 8c641160 8c66120c 8c64241a 8c653440 8c6656e2 8c6666c0 8c663140 8c651520 8c665a00 8c666706 -postScript ExportFailedFunctionEvidence.java $taskOutput 8c641160 8c66120c 8c64241a 8c653440 8c6656e2 8c6666c0 8c663140 8c651520 8c665a00 8c666706 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'MOVIE driver services read-only export failed'}

$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 10 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'MOVIE driver services export diagnostics incomplete'}

param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/boot-input-getters'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript SeedCallbackFunctions.java 8c65f4da 8c65f46c 8c65f43a 8c65f480 8c65f4c0 8c65f4ac 8c65f4f8 8c65f50c 8c65f546 -postScript ExportFailedFunctionEvidence.java $taskOutput 8c65f4da 8c65f46c 8c65f43a 8c65f480 8c65f4c0 8c65f4ac 8c65f4f8 8c65f50c 8c65f546 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Boot input read-only export failed'}

$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 9 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Boot input export diagnostics incomplete'}

param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskTools=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskTools 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskTools -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskTools 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskScripts=Join-Path $Workspace 'outputs/sonic-adventure/ghidra'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/movie-input-pump-canonical'
$taskEntries=@('8c646654','8c65f1aa','8c65f202','8c6461ae','8c65f1ec','8c65f50c','8c64658a','8c65fd4e','8c65fe14','8c65ff12','8c65ff5a','8c65ffb8','8c66005c','8c66018e','8c668b24')
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $taskScripts -postScript SeedCallbackFunctions.java @taskEntries -postScript ExportFailedFunctionEvidence.java $taskOutput @taskEntries -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Input pump canonical export failed'}
$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 15 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Input pump canonical export incomplete'}

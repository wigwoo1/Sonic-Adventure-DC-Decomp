param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/draw-gbr-geometry'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript SeedCallbackFunctions.java 8c036f60 8c03718c 8c60a3f8 8c64f31c 8c64f244 8c64f2dc 8c60b3e2 -postScript ExportFailedFunctionEvidence.java $taskOutput 8c036f60 8c03718c 8c60a3f8 8c64f31c 8c64f244 8c64f2dc 8c60b3e2 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'GBR geometry read-only export failed'}

$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 7 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'GBR geometry export diagnostics incomplete'}

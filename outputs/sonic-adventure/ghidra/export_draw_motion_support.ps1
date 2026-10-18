param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/draw-motion-support'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript SeedCallbackFunctions.java 8c03700c 8c037098 8c0400a0 8c0400f8 8c040150 8c0401a8 8c04057a 8c040588 8c040596 8c0405a4 8c0405b2 8c0405d2 8c040612 8c0406a0 8c0406e0 8c040720 8c04073c 8c040760 8c040df0 8c040e74 8c040ef8 8c0414a6 8c0414cc 8c0414f0 8c041516 8c04154e 8c041600 8c041666 8c0416cc 8c041732 8c04175c 8c041792 8c0417c8 -postScript ExportFailedFunctionEvidence.java $taskOutput 8c03700c 8c037098 8c0400a0 8c0400f8 8c040150 8c0401a8 8c04057a 8c040588 8c040596 8c0405a4 8c0405b2 8c0405d2 8c040612 8c0406a0 8c0406e0 8c040720 8c04073c 8c040760 8c040df0 8c040e74 8c040ef8 8c0414a6 8c0414cc 8c0414f0 8c041516 8c04154e 8c041600 8c041666 8c0416cc 8c041732 8c04175c 8c041792 8c0417c8 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Motion support read-only export failed'}

$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 33 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Motion support export diagnostics incomplete'}

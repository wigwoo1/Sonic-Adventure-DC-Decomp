param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/overlay-draw-unindexed-targets'
# Function discovery is temporary in a read-only session. Neither the saved
# project nor the frozen global function index is changed. These are inferred
# CFG bodies, not an exhaustive code/data boundary or recovered-source claim.
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript SeedCallbackFunctions.java 8c60e5e4 8c5fde20 -postScript ExportFailedFunctionEvidence.java $taskOutput 8c60e5e4 8c5fde20 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Unindexed draw-target temporary discovery/export failed'}

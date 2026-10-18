param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/overlay-draw-missing-helpers'
# Export only existing function boundaries, read-only. Targets 8c5fde20 and
# 8c60e5e4 lack indexed boundaries and need separate discovery, not guessed ends.
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript ExportFailedFunctionEvidence.java $taskOutput 8c038d00 8c0409c0 8c041c92 8c043a20 8c0583a0 8c638ecc 8c640128 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Main object-draw helper export failed'}
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevACommonOverlays/common-overlays' -process AL_MAIN.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript ExportFailedFunctionEvidence.java $taskOutput 0c909d80 0c90c384 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Overlay object-draw helper export failed'}

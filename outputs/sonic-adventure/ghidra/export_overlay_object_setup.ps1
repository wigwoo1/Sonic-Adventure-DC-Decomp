param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/overlay-object-setup'
# Read-only evidence export; no saved project or original bytes are modified.
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevACommonOverlays/common-overlays' -process AL_MAIN.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript ExportFailedFunctionEvidence.java $taskOutput 0c9044a0 0c9044e2 0c90451c 0c904d22 -max-cpu 4
if ($LASTEXITCODE -ne 0) { throw 'Overlay object-setup evidence export failed' }

param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/draw-motion-setup'
# These callback boundaries are temporary CFG proposals; all project changes
# are discarded by -readOnly. The global snapshot index is not regenerated.
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript SeedCallbackFunctions.java 8c60e5e4 8c60c322 8c60cfc4 8c60c526 8c60c488 8c60cffc 8c60cfe2 8c60c3b8 8c60d016 8c60d034 -postScript ExportFailedFunctionEvidence.java $taskOutput 8c60e5e4 8c60e31c 8c605dac 8c605e0a 8c60c322 8c60cfc4 8c60c526 8c60c488 8c60cffc 8c60cfe2 8c60c3b8 8c60d016 8c60d034 8c621574 8c639cf4 8c639ff8 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Motion setup/dispatcher read-only evidence export failed'}

param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/draw-hierarchy'
# These callback boundaries are temporary CFG proposals; all project changes
# are discarded by -readOnly. The global snapshot index is not regenerated.
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript SeedCallbackFunctions.java 8c0583a0 8c057bc0 8c63a770 8c058434 8c63a5ec 8c638f94 8c055d4e -postScript ExportFailedFunctionEvidence.java $taskOutput 8c0583a0 8c057bc0 8c63a770 8c058434 8c63a5ec 8c638f94 8c055d4e -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Hierarchy matrix read-only export failed'}

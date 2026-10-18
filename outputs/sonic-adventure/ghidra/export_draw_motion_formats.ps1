param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/draw-motion-formats'
# These callback boundaries are temporary CFG proposals; all project changes
# are discarded by -readOnly. The global snapshot index is not regenerated.
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript SeedCallbackFunctions.java 8c60d054 8c60d094 8c60d0d4 8c60d140 8c60d180 8c60d19e 8c60d1c8 8c60c322 8c60c3b8 8c60c488 8c60c526 8c60beb0 8c60c026 8c60bf5e 8c60c19c 8c10cc80 8c10cd38 8c10cd8e -postScript ExportFailedFunctionEvidence.java $taskOutput 8c60d054 8c60d094 8c60d0d4 8c60d140 8c60d180 8c60d19e 8c60d1c8 8c60c322 8c60c3b8 8c60c488 8c60c526 8c60beb0 8c60c026 8c60bf5e 8c60c19c 8c10cc80 8c10cd38 8c10cd8e -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Motion format/interpolation read-only evidence export failed'}

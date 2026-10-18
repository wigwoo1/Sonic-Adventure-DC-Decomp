param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path,[switch]$MainOnly)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/draw-object-effects'
# Existing indexed boundaries only. The shared database and frozen global
# function index remain unchanged; these exports grant no recovery credit.
if(-not $MainOnly){
 & (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevACommonOverlays/common-overlays' -process AL_MAIN.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript ExportFailedFunctionEvidence.java $taskOutput 0c909d80 0c908f3e 0c909054 0c9092da 0c909560 0c9097e4 0c909aa0 0c908b00 0c908d80 -max-cpu 4
 if($LASTEXITCODE -ne 0){throw 'Original object-effect dependency read-only export failed'}
}
$taskMainOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/draw-object-effects-main'
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript ExportFailedFunctionEvidence.java $taskMainOutput 8c605628 8c608d14 8c63d414 -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Original main object-effect dependency read-only export failed'}

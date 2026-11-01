param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskTools=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskTools 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskTools -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskTools 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskProject=Join-Path $Workspace 'outputs/sonic-adventure'
$taskOutput=Join-Path $taskProject 'analysis/player-texture-draw-references'
New-Item -ItemType Directory -Force $taskOutput | Out-Null
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript ExportResourceReferences.java (Join-Path $taskProject 'analysis/player-texture-draw-reference-inputs.tsv') (Join-Path $taskOutput 'references.tsv') -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Original renderer header/cursor reference export failed'}

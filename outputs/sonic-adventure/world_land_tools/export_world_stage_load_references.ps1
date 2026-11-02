param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $Workspace 'work/sonic-adventure/world-stage-load-references'
# Read-only evidence of the actual land/registered-object producers.
if (Test-Path -LiteralPath $taskOutput) { throw "Preserve earlier land producer evidence" }
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath (Join-Path $Workspace 'outputs/sonic-adventure/ghidra') -postScript ExportGlobalSlotReferences.java $taskOutput 8c051a52 8c09d9c6 -max-cpu 4
if ($LASTEXITCODE -ne 0) { throw 'Model camera slot reference export failed' }


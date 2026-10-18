param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $PSScriptRoot))))
$ErrorActionPreference = 'Stop'
$toolRoot = Join-Path $Workspace 'work/sonic-adventure/toolchain'
$env:JAVA_HOME = (Get-ChildItem $toolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1).FullName
$env:_JAVA_OPTIONS = '-Duser.home="' + (Join-Path $toolRoot 'user-home') + '" -Djava.io.tmpdir="' + (Join-Path $toolRoot 'tmp') + '"'
& (Join-Path $toolRoot 'ghidra_12.1.4_PUBLIC/support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $PSScriptRoot -postScript ExportRoutineInstructions.java (Join-Path $Workspace 'outputs/sonic-adventure/analysis/instructions') 8c600cf0 8c6013a4 -max-cpu 2 -log (Join-Path $Workspace 'work/sonic-adventure/request-finalizer-export.log')
if ($LASTEXITCODE -ne 0) { throw "Request-finalizer export failed: $LASTEXITCODE" }

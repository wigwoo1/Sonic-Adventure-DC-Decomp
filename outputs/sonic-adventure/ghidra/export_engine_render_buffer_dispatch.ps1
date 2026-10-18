param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $PSScriptRoot))))
$ErrorActionPreference = 'Stop'
$toolRoot = Join-Path $Workspace 'work/sonic-adventure/toolchain'
$ghidraRoot = Get-ChildItem $toolRoot -Directory -Filter 'ghidra_*' | Select-Object -First 1
$jdkRoot = Get-ChildItem $toolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME = $jdkRoot.FullName
$env:_JAVA_OPTIONS = '-Duser.home="' + (Join-Path $toolRoot 'user-home') + '"'
$env:_JAVA_OPTIONS += ' -Djava.io.tmpdir="' + (Join-Path $toolRoot 'tmp') + '"'
$scriptRoot = Join-Path $Workspace 'work/sonic-adventure/linear-source-constructor-scripts'
Copy-Item (Join-Path $PSScriptRoot 'ExportInstructionRange.java') $scriptRoot -Force
& (Join-Path $ghidraRoot.FullName 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -noanalysis -scriptPath $scriptRoot -postScript ExportInstructionRange.java (Join-Path $Workspace 'outputs/sonic-adventure/analysis/instructions/engine_render_buffer_dispatch.tsv') 8c6536ec 8c653738 -max-cpu 4 -log (Join-Path $Workspace 'work/sonic-adventure/ghidra-render-dispatch.log')
if ($LASTEXITCODE -ne 0) { throw "Ghidra export failed: $LASTEXITCODE" }

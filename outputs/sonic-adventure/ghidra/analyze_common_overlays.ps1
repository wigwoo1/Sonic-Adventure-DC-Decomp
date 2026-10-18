param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $PSScriptRoot))))
$ErrorActionPreference = 'Stop'
$toolRoot = Join-Path $Workspace 'work/sonic-adventure/toolchain'
$ghidraRoot = Get-ChildItem $toolRoot -Directory -Filter 'ghidra_*' | Select-Object -First 1
$jdkRoot = Get-ChildItem $toolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME = $jdkRoot.FullName
$env:_JAVA_OPTIONS = '-Duser.home="' + (Join-Path $toolRoot 'user-home') + '"'
$scriptRoot = Join-Path $Workspace 'work/sonic-adventure/ghidra-scripts'
Copy-Item (Join-Path $PSScriptRoot '*.java') $scriptRoot
& (Join-Path $ghidraRoot.FullName 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevACommonOverlays' -import (Join-Path $Workspace 'work/sonic-adventure/common-overlays') -recursive -loader BinaryLoader -loader-baseAddr 0x0c900000 -processor 'SuperH4:LE:32:default' -cspec default -analysisTimeoutPerFile 300 -preScript SeedOverlay.java -scriptPath $scriptRoot -postScript ExportInitialDecompilation.java (Join-Path $Workspace 'outputs/sonic-adventure/analysis/common-overlays') 0 per-program -max-cpu 4 -log (Join-Path $Workspace 'work/sonic-adventure/ghidra-common-overlays.log')
if ($LASTEXITCODE -ne 0) { throw "Ghidra export failed: $LASTEXITCODE" }



param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $PSScriptRoot))))
$ErrorActionPreference = 'Stop'
$toolRoot = Join-Path $Workspace 'work/sonic-adventure/toolchain'
$ghidraRoot = Get-ChildItem $toolRoot -Directory -Filter 'ghidra_*' | Select-Object -First 1
$jdkRoot = Get-ChildItem $toolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME = $jdkRoot.FullName
$env:_JAVA_OPTIONS = '-Duser.home="' + (Join-Path $toolRoot 'user-home') + '"'
$scriptRoot = Join-Path $Workspace 'work/sonic-adventure/ghidra-scripts'
Copy-Item (Join-Path $PSScriptRoot '*.java') $scriptRoot
& (Join-Path $ghidraRoot.FullName 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -noanalysis -scriptPath $scriptRoot -postScript ExportSchedulerReferences.java (Join-Path $Workspace 'outputs/sonic-adventure/analysis/timing-settings-references') 8c051760 8c754ec4 8c754ebc -max-cpu 4 -log (Join-Path $Workspace 'work/sonic-adventure/ghidra-timing-settings-references.log')
if ($LASTEXITCODE -ne 0) { throw "Ghidra export failed: $LASTEXITCODE" }









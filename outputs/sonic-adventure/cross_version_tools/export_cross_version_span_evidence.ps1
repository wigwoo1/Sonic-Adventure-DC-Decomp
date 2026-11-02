$ErrorActionPreference='Stop'
$taskWorkspace=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$taskTools=Join-Path $PSScriptRoot 'toolchain'
$taskGhidra=Join-Path $taskTools 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskTools -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskTools 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskOutput=Join-Path $PSScriptRoot 'cross-version-span-evidence'
if (Test-Path -LiteralPath $taskOutput) { throw 'Evidence directory already exists; inspect before rerunning.' }
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $PSScriptRoot 'ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath (Join-Path $PSScriptRoot 'cross-version-ghidra-scripts') -postScript ExportCrossVersionSpanEvidence.java (Join-Path $PSScriptRoot 'cross_version_unique_span_input.tsv') $taskOutput -max-cpu 4
if ($LASTEXITCODE -ne 0) { throw 'Read-only cross-version reference export failed.' }

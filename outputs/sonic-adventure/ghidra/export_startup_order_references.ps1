param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $PSScriptRoot))))
$ErrorActionPreference = 'Stop'
$toolRoot = Join-Path $Workspace 'work/sonic-adventure/toolchain'
$ghidraRoot = Get-ChildItem $toolRoot -Directory -Filter 'ghidra_*' | Select-Object -First 1
$jdkRoot = Get-ChildItem $toolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME = $jdkRoot.FullName
$env:_JAVA_OPTIONS = '-Duser.home="' + (Join-Path $toolRoot 'startup-order-user-home') + '"'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$env:_JAVA_OPTIONS += ' -Djava.io.tmpdir="' + $env:TMP + '"'
$scriptRoot = Join-Path $Workspace 'work/sonic-adventure/startup-order-scripts'
New-Item -ItemType Directory -Force $scriptRoot | Out-Null
Copy-Item (Join-Path $PSScriptRoot 'ExportResourceReferences.java') $scriptRoot
& (Join-Path $ghidraRoot.FullName 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -noanalysis -scriptPath $scriptRoot -postScript ExportResourceReferences.java (Join-Path $Workspace 'outputs/sonic-adventure/analysis/startup_order_targets.tsv') (Join-Path $Workspace 'outputs/sonic-adventure/analysis/startup_order_references.tsv') -max-cpu 4 -log (Join-Path $Workspace 'work/sonic-adventure/ghidra-startup-order-references.log')
if (!(Test-Path (Join-Path $Workspace 'outputs/sonic-adventure/analysis/startup_order_references.tsv'))) { throw 'Reference export missing; inspect Ghidra log' }
if ($LASTEXITCODE -ne 0) { throw "Ghidra export failed: $LASTEXITCODE" }








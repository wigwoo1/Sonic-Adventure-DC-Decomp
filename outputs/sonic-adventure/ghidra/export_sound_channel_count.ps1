param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $PSScriptRoot))))
$ErrorActionPreference = 'Stop'
$toolRoot = Join-Path $Workspace 'work/sonic-adventure/toolchain'
$ghidraRoot = Get-ChildItem $toolRoot -Directory -Filter 'ghidra_*' | Select-Object -First 1
$jdkRoot = Get-ChildItem $toolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME = $jdkRoot.FullName
$env:_JAVA_OPTIONS = '-Duser.home="' + (Join-Path $toolRoot 'user-home') + '"'
$ghidraTemporaryDirectory = Join-Path $toolRoot 'tmp'
New-Item -ItemType Directory -Force $ghidraTemporaryDirectory | Out-Null
$env:_JAVA_OPTIONS += ' -Djava.io.tmpdir=' + [char]34 + $ghidraTemporaryDirectory + [char]34
$scriptRoot = Join-Path $Workspace 'work/sonic-adventure/input-status-scripts'
New-Item -ItemType Directory -Force -Path $scriptRoot | Out-Null
Copy-Item (Join-Path $PSScriptRoot 'SeedOwnerHelperBranches.java') $scriptRoot
Copy-Item (Join-Path $PSScriptRoot 'ExportInstructionRange.java') $scriptRoot
& (Join-Path $ghidraRoot.FullName 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -noanalysis -scriptPath $scriptRoot -postScript ExportInstructionRange.java (Join-Path $Workspace 'outputs/sonic-adventure/analysis/instructions/sound_channel_count.tsv') 8c66b5ac 8c66b66c -max-cpu 4 -log (Join-Path $Workspace 'work/sonic-adventure/ghidra-owner-helper-dependencies.log')
if ($LASTEXITCODE -ne 0) { throw "Ghidra export failed: $LASTEXITCODE" }

















































param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $PSScriptRoot))))
$ErrorActionPreference = 'Stop'
$toolRoot = Join-Path $Workspace 'work/sonic-adventure/toolchain'
$ghidraRoot = Get-ChildItem $toolRoot -Directory -Filter 'ghidra_*' | Select-Object -First 1
$jdkRoot = Get-ChildItem $toolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME = $jdkRoot.FullName
$venv = Join-Path $toolRoot 'pyghidra-venv'
$env:PATH = (Join-Path $venv 'Scripts') + ';' + $env:PATH
$env:_JAVA_OPTIONS = '-Duser.home="' + (Join-Path $toolRoot 'user-home') + '"'
$temp = Join-Path $toolRoot 'tmp'
New-Item -ItemType Directory -Force $temp | Out-Null
$env:_JAVA_OPTIONS += ' -Djava.io.tmpdir=' + [char]34 + $temp + [char]34
$output = Join-Path $PSScriptRoot '..\analysis\instructions\8c0a966c.tsv'
$log = Join-Path $Workspace 'work/sonic-adventure/ghidra-failed-small-function.log'
& (Join-Path $ghidraRoot.FullName 'support/pyghidraRun.bat') $ghidraRoot.FullName --headless (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -noanalysis -scriptPath $PSScriptRoot -postScript ExportInstructionRange.py $output 8c0a966c 8c0a976a -max-cpu 4 -log $log
if ($LASTEXITCODE -ne 0) { throw "Ghidra instruction export failed: $LASTEXITCODE" }

param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $PSScriptRoot))))
$ErrorActionPreference = 'Stop'
$toolRoot = Join-Path $Workspace 'work/sonic-adventure/toolchain'
$ghidraRoot = Get-ChildItem $toolRoot -Directory -Filter 'ghidra_*' | Select-Object -First 1
$jdkRoot = Get-ChildItem $toolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
if (!$ghidraRoot -or !$jdkRoot) { throw 'Portable Ghidra and JDK have not been extracted.' }
$env:JAVA_HOME = $jdkRoot.FullName
$env:GHIDRA_USER_HOME = Join-Path $toolRoot 'user-home'
New-Item -ItemType Directory -Force $env:GHIDRA_USER_HOME | Out-Null
$env:_JAVA_OPTIONS = '-Duser.home="' + $env:GHIDRA_USER_HOME + '"'
$projectRoot = Join-Path $Workspace 'work/sonic-adventure/ghidra-projects'
New-Item -ItemType Directory -Force $projectRoot | Out-Null
& (Join-Path $ghidraRoot.FullName 'support/analyzeHeadless.bat') $projectRoot 'SonicAdventureUSARevASeeded' -import (Join-Path $Workspace 'work/sonic-adventure/extracted/1ST_READ.BIN') -loader BinaryLoader -loader-baseAddr 0x8c010000 -processor 'SuperH4:LE:32:default' -cspec default -scriptPath (Join-Path $Workspace 'work/sonic-adventure/ghidra-scripts') -preScript SeedDreamcast.java -postScript ExportInitialDecompilation.java (Join-Path $Workspace 'outputs/sonic-adventure/analysis/ghidra') -analysisTimeoutPerFile 600 -max-cpu 4 -log (Join-Path $Workspace 'work/sonic-adventure/ghidra-analysis.log')
if ($LASTEXITCODE -ne 0) { throw "Ghidra failed: $LASTEXITCODE" }






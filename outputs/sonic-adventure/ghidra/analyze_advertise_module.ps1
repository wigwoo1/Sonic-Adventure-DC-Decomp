param(
    [string]$Workspace,
    [ValidatePattern('^\d+\.\d+\.\d+_PUBLIC$')][string]$GhidraVersion = '12.1.4_PUBLIC'
)
if ([string]::IsNullOrWhiteSpace($Workspace)) {
    $Workspace = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
}
$ErrorActionPreference = 'Stop'
$toolRoot = Join-Path $Workspace 'work/sonic-adventure/toolchain'
$ghidraRoot = Join-Path $toolRoot "ghidra_$GhidraVersion"
$jdkRoot = Get-ChildItem $toolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
if (!(Test-Path -LiteralPath $ghidraRoot) -or !$jdkRoot) { throw "Portable Ghidra $GhidraVersion and JDK have not been extracted." }
$env:JAVA_HOME = $jdkRoot.FullName
$runId = [guid]::NewGuid().ToString('N')
$env:GHIDRA_USER_HOME = Join-Path $toolRoot "user-home-$GhidraVersion-$runId"
New-Item -ItemType Directory -Force $env:GHIDRA_USER_HOME | Out-Null
$env:_JAVA_OPTIONS = '-Duser.home="' + $env:GHIDRA_USER_HOME + '"'
$projectRoot = Join-Path $Workspace 'work/sonic-adventure/ghidra-projects'
$scriptRoot = Join-Path $Workspace 'work/sonic-adventure/ghidra-scripts'
$module = Join-Path $Workspace 'work/sonic-adventure/decoded/SONICADV/ADVERTISE.BIN'
$output = Join-Path $Workspace 'outputs/sonic-adventure/analysis/additional-modules'
$logPath = Join-Path $Workspace 'work/sonic-adventure/ghidra-advertise-module.log'
New-Item -ItemType Directory -Force $projectRoot, $scriptRoot, $output | Out-Null
Remove-Item -LiteralPath $logPath -Force -ErrorAction SilentlyContinue
Copy-Item (Join-Path $PSScriptRoot '*.java') $scriptRoot -Force
& (Join-Path $ghidraRoot 'support/analyzeHeadless.bat') $projectRoot 'SonicAdventureAdvertiseModule' -import $module -overwrite -loader BinaryLoader -loader-baseAddr 0x8c900000 -processor 'SuperH4:LE:32:default' -cspec default -analysisTimeoutPerFile 300 -preScript SeedOverlay.java -scriptPath $scriptRoot -postScript ExportInitialDecompilation.java $output 0 per-program -max-cpu 4 -log $logPath
if ($LASTEXITCODE -ne 0) { throw "Ghidra ADVERTISE analysis failed: $LASTEXITCODE" }
if (!(Test-Path -LiteralPath $logPath)) { throw 'Ghidra did not produce an analysis log.' }
if (Select-String -LiteralPath $logPath -Pattern 'ERROR REPORT SCRIPT ERROR' -Quiet) { throw 'Ghidra ADVERTISE analysis reported a script failure.' }
$index = Join-Path $output 'ADVERTISE.BIN/function_index.tsv'
if (!(Test-Path -LiteralPath $index)) { throw 'Ghidra ADVERTISE analysis did not create function_index.tsv.' }

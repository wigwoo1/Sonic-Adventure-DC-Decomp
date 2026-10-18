param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $PSScriptRoot))))
$ErrorActionPreference = 'Stop'
$toolRoot = Join-Path $Workspace 'work/sonic-adventure/toolchain'
$ghidraRoot = Get-ChildItem $toolRoot -Directory -Filter 'ghidra_*' | Select-Object -First 1
$jdkRoot = Get-ChildItem $toolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME = $jdkRoot.FullName
$env:_JAVA_OPTIONS = '-Duser.home="' + (Join-Path $toolRoot 'user-home') + '"'
$scriptRoot = Join-Path $Workspace 'work/sonic-adventure/owner-helper-scripts'
New-Item -ItemType Directory -Force -Path $scriptRoot | Out-Null
Copy-Item (Join-Path $PSScriptRoot 'SeedOwnerHelperBranches.java') $scriptRoot
Copy-Item (Join-Path $PSScriptRoot 'ExportInstructionRange.java') $scriptRoot
& (Join-Path $ghidraRoot.FullName 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -noanalysis -scriptPath $scriptRoot -postScript SeedOwnerHelperBranches.java -postScript ExportInstructionRange.java (Join-Path $Workspace 'outputs/sonic-adventure/analysis/instructions/owner_helper_full_range.tsv') 8c079476 8c079bf8 -max-cpu 4 -log (Join-Path $Workspace 'work/sonic-adventure/ghidra-owner-helper-range.log')
if ($LASTEXITCODE -ne 0) { throw "Ghidra export failed: $LASTEXITCODE" }












$taskExportRows = Import-Csv -Delimiter "`t" (Join-Path $Workspace 'outputs/sonic-adventure/analysis/instructions/owner_helper_full_range.tsv')
foreach ($taskTarget in @('8c07952e','8c079586','8c0795c8','8c079608','8c079840','8c0799c8','8c0798c0','8c079a1c','8c0798fa','8c0796c0','8c07975a','8c0797f4','8c079a80','8c079b04')) {
 if ($taskTarget -notin $taskExportRows.address) { throw "Missing helper switch target: $taskTarget" }
}

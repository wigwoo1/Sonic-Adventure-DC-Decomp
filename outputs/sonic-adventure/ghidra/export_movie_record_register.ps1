param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskScript=Join-Path $Workspace 'outputs/sonic-adventure/ghidra'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/movie-record-register'
$taskEntries=@('8c010b00','8c602b68','8c6477cc','8c647e16','8c646eee','8c010f22','8c602bf0','8c602dda','8c647c6c','8c647de8','8c647f10','8c647f4c','8c647534','8c660898','8c647c50','8c66082c','8c660842','8c668b38','8c668b58','8c668bee','8c110954')
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') (Join-Path $Workspace 'work/sonic-adventure/ghidra-projects') 'SonicAdventureUSARevASeeded' -process 1ST_READ.BIN -readOnly -noanalysis -scriptPath $taskScript -postScript SeedCallbackFunctions.java @taskEntries -postScript ExportFailedFunctionEvidence.java $taskOutput @taskEntries -postScript ExportOverlaySelectorReferences.java (Join-Path $taskOutput 'references.tsv') @taskEntries -max-cpu 4
if($LASTEXITCODE -ne 0){throw 'Record registration export failed'}
$taskRows=@(Import-Csv -LiteralPath (Join-Path $taskOutput 'diagnostics.tsv') -Delimiter "`t")
if($taskRows.Count -ne 21 -or @($taskRows|Where-Object {$_.completed -ne 'true' -or $_.error}).Count){throw 'Record registration diagnostic incomplete'}

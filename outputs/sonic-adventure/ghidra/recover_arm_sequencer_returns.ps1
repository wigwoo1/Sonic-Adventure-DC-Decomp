param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $PSScriptRoot))))
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
if (!$taskJdk) { throw 'Existing JDK unavailable' }
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskProject=Join-Path $Workspace 'work/sonic-adventure/ghidra-projects'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/arm-sequencer-enrichment'
foreach ($taskName in @('MANATEE.DRV','MASARU.DRV')) {
 & (Join-Path $taskGhidra 'support/analyzeHeadless.bat') $taskProject "SonicAdventureARM_$taskName" -process "$taskName.arm.bin" -noanalysis -scriptPath $PSScriptRoot -preScript SeedArmSequencerReturns.java -postScript ExportArmSoundDriverEvidence.java $taskOutput -max-cpu 4
 if ($LASTEXITCODE -ne 0) { throw "Sequencer return recovery failed: $taskName" }
 if (!(Test-Path -LiteralPath (Join-Path $taskOutput "$taskName.arm.bin/instructions.tsv"))) { throw "Missing enriched evidence: $taskName" }
}

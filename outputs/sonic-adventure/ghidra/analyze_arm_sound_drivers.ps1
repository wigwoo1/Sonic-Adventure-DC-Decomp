param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $PSScriptRoot))),
      [switch]$RefreshEvidenceOnly)
$ErrorActionPreference='Stop'
$toolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$ghidraRoot=Join-Path $toolRoot 'ghidra_12.1.4_PUBLIC'
$jdkRoot=Get-ChildItem $toolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
if (!(Test-Path -LiteralPath $ghidraRoot) -or !$jdkRoot) { throw 'Existing Ghidra/JDK toolchain unavailable' }
$env:JAVA_HOME=$jdkRoot.FullName
$env:GHIDRA_USER_HOME=Join-Path $toolRoot 'arm-sound-driver-user-home'
New-Item -ItemType Directory -Force $env:GHIDRA_USER_HOME | Out-Null
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$projectRoot=Join-Path $Workspace 'work/sonic-adventure/ghidra-projects'
$output=Join-Path $Workspace 'outputs/sonic-adventure/analysis/arm-sound-drivers'
New-Item -ItemType Directory -Force $projectRoot,$output | Out-Null
foreach ($name in @('MANATEE.DRV','MASARU.DRV')) {
    $inputFile=Join-Path $Workspace "work/sonic-adventure/arm-sound-drivers/$name.arm.bin"
    if (!(Test-Path -LiteralPath $inputFile)) { throw "Run prepare_arm_sound_drivers.py first: $inputFile" }
    $log=Join-Path $Workspace "work/sonic-adventure/ghidra-arm-$name.log"
    $arguments=@($projectRoot,"SonicAdventureARM_$name")
    if ($RefreshEvidenceOnly) {
        $arguments+=@('-process',"$name.arm.bin",'-noanalysis')
    } else {
        $arguments+=@('-import',$inputFile,'-overwrite','-loader','BinaryLoader','-loader-baseAddr','0x0','-processor','ARM:LE:32:v4','-cspec','default','-analysisTimeoutPerFile','300','-preScript','SeedArmSoundDriver.java')
    }
    $arguments+=@('-scriptPath',$PSScriptRoot)
    if (!$RefreshEvidenceOnly) { $arguments+=@('-postScript','ExportInitialDecompilation.java',$output,'0','per-program') }
    $arguments+=@('-postScript','ExportArmSoundDriverEvidence.java',$output,'-max-cpu','4','-log',$log)
    & (Join-Path $ghidraRoot 'support/analyzeHeadless.bat') @arguments
    if ($LASTEXITCODE -ne 0) { throw "ARM driver analysis failed: $name" }
    if (Select-String -LiteralPath $log -Pattern 'ERROR REPORT SCRIPT ERROR' -Quiet) { throw "ARM driver script failed: $name" }
    $index=Join-Path $output "$name.arm.bin/function_index.tsv"
    if (!(Test-Path -LiteralPath $index)) { throw "No ARM function index: $name" }
    if ((Import-Csv -LiteralPath $index -Delimiter "`t").Count -lt 1) { throw "Empty ARM function index: $name" }
}

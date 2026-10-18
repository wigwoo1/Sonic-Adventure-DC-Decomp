param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference='Stop'
$taskVendor=Join-Path $PSScriptRoot 'third_party/skmp-sh4-fpu/sh4_fpu_approx.h'
$taskExpected='cc95d6a3ede9ab57b6f332fcec5dacc97cda16515699dcdcd4bcf4978c869d2f'
if(-not(Test-Path -LiteralPath $taskVendor)){
 New-Item -ItemType Directory -Force (Split-Path -Parent $taskVendor) | Out-Null
 Invoke-WebRequest -Uri 'https://gist.githubusercontent.com/skmp/4bd7b38149fc455978b5104201573236/raw/29f17b480213f063c6db998ecba5745d3f4ad4b3/sh4_fpu_approx.h' -OutFile $taskVendor
}
if((Get-FileHash -Algorithm SHA256 -LiteralPath $taskVendor).Hash.ToLowerInvariant() -ne $taskExpected){
 throw 'SH-4 dependency hash differs from the reviewed revision; existing file preserved for inspection.'
}
'Pinned MIT-licensed SH-4 math header present and hash verified; original license notice retained.'

param([string]$Workspace = (Split-Path (Split-Path $PSScriptRoot -Parent) -Parent))
$ErrorActionPreference = 'Stop'
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$buildTemp = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TMP = $buildTemp
$env:TEMP = $buildTemp
New-Item -ItemType Directory -Force $buildTemp | Out-Null
$output = Join-Path $Workspace 'work/sonic-adventure/build/stg09_nested_signed_range_predicate_0c91a75c.dll'
New-Item -ItemType Directory -Force (Split-Path -Parent $output) | Out-Null
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' `
    (Join-Path $PSScriptRoot 'source/sa_stg09_nested_signed_range_predicate_0c91a75c.c') -o $output
if ($LASTEXITCODE -ne 0) { throw 'STG09 nested signed-range predicate build failed' }

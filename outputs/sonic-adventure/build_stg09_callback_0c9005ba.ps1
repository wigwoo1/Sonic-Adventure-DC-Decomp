param([string]$Workspace = (Split-Path (Split-Path $PSScriptRoot -Parent) -Parent))
$ErrorActionPreference = 'Stop'
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
New-Item -ItemType Directory -Force $env:TMP | Out-Null
$output = Join-Path $Workspace 'work/sonic-adventure/build/stg09_callback_0c9005ba.dll'
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' `
    (Join-Path $PSScriptRoot 'source/sa_stg09_callback_0c9005ba.c') -o $output
if ($LASTEXITCODE -ne 0) { throw 'STG09 callback build failed' }

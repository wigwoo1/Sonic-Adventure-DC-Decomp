param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$source = Join-Path $PSScriptRoot 'source/sa_input_metadata_bit_locator_8c646b42.c'
$output = Join-Path $Workspace 'work/sonic-adventure/build/sa_input_metadata_bit_locator_8c646b42.dll'
New-Item -ItemType Directory -Force (Split-Path -Parent $output) | Out-Null
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' $source -o $output
if ($LASTEXITCODE -ne 0) { throw 'Input metadata bit locator build failed' }
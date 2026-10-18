param([string]$Workspace = (Split-Path (Split-Path $PSScriptRoot -Parent) -Parent))
$ErrorActionPreference = 'Stop'
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$output = Join-Path $Workspace 'work/sonic-adventure/build/sa_update_object_mode_and_table_8c01b3e0.dll'
New-Item -ItemType Directory -Force (Split-Path -Parent $output) | Out-Null
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib `
    '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_update_object_mode_and_table_8c01b3e0.c') -o $output
if ($LASTEXITCODE -ne 0) { throw 'Object mode/table helper compilation failed' }

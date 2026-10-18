param([string]$Workspace = (Split-Path (Split-Path $PSScriptRoot -Parent) -Parent))
$ErrorActionPreference = 'Stop'
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_copy_global_pair_bytes_8c08ab02.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/copy_global_pair_bytes_8c08ab02.dll')
if ($LASTEXITCODE -ne 0) { throw 'Global pair byte copy build failed' }

param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/native_disc_sector_reader.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/native_disc_sector_reader.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native disc sector reader build failed' }

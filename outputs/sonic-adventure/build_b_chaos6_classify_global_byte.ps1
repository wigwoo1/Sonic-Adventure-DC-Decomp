param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$output = Join-Path $Workspace 'work/sonic-adventure/build/b_chaos6_classify_global_byte.dll'
New-Item -ItemType Directory -Force (Split-Path -Parent $output) | Out-Null
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib (Join-Path $PSScriptRoot 'source/b_chaos6_classify_global_byte.c') '-Wl,/noentry' -o $output
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 global-byte classifier compilation failed' }

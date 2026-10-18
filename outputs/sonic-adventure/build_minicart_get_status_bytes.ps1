param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$output = Join-Path $Workspace 'work/sonic-adventure/build/minicart_get_status_bytes.dll'
New-Item -ItemType Directory -Force (Split-Path -Parent $output) | Out-Null
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib (Join-Path $PSScriptRoot 'source/minicart_get_status_bytes.c') '-Wl,/noentry' -o $output
if ($LASTEXITCODE -ne 0) { throw 'MINICART status-byte getter compilation failed' }

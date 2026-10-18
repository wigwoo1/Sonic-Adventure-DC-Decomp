param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/startup_register_internal_name.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/startup_register_internal_name.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native startup internal-name registration build failed' }

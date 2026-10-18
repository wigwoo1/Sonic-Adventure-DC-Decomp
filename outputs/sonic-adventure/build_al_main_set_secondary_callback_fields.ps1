param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$output = Join-Path $Workspace 'work/sonic-adventure/build/al_main_set_secondary_callback_fields.dll'
New-Item -ItemType Directory -Force (Split-Path -Parent $output) | Out-Null
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib (Join-Path $PSScriptRoot 'source/al_main_set_secondary_callback_fields.c') '-Wl,/noentry' -o $output
if ($LASTEXITCODE -ne 0) { throw 'AL_MAIN secondary callback-field setter compilation failed' }

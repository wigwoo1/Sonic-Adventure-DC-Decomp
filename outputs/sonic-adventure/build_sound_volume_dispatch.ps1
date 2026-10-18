param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$python = 'C:\Users\wigwo\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe'
$sources = @('sa_sound_volume_dispatch','sa_signed_division') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o (Join-Path $Workspace 'work/sonic-adventure/build/sound_volume_dispatch.dll')
if ($LASTEXITCODE -ne 0) { throw 'Type-2 sound command build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_volume_dispatch_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Type-2 sound command verification failed' }

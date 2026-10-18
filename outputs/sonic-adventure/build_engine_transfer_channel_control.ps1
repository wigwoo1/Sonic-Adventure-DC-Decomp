param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$python = 'C:\Users\wigwo\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe'
$sources = @('sa_engine_transfer_channel_arm_io.c','sa_engine_transfer_channel_release_io.c','sa_engine_transfer_busy_get_io.c') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_) }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_transfer_channel_control.dll')
if ($LASTEXITCODE -ne 0) { throw 'Transfer channel control build failed' }

$ErrorActionPreference = 'Stop'
$workspace = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$env:TMP = Join-Path $workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_sound_snd_transfer8_step.c') (Join-Path $PSScriptRoot 'source/sa_file_pending_state_get.c') (Join-Path $PSScriptRoot 'source/sa_sound_snd_transfer16_step.c') (Join-Path $PSScriptRoot 'source/sa_sound_snd_transfer_dispatch.c') (Join-Path $PSScriptRoot 'source/sa_sound_snd_table_transfer_step.c') -o (Join-Path $workspace 'work/sonic-adventure/build/sound_snd_transfer_dispatch.dll')
if ($LASTEXITCODE -ne 0) { throw 'Sound driver control build failed' }

$ErrorActionPreference = 'Stop'
$workspace = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$env:TMP = Join-Path $workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_sound_voice_slot_select.c') (Join-Path $PSScriptRoot 'source/sa_sound_voice_record_find.c') (Join-Path $PSScriptRoot 'source/sa_sound_voice_record_refresh.c') -o (Join-Path $workspace 'work/sonic-adventure/build/sound_voice_record_refresh.dll')
if ($LASTEXITCODE -ne 0) { throw 'Sound record refresh build failed' }

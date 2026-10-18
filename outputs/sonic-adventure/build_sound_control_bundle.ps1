$ErrorActionPreference = 'Stop'
$workspace = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$env:TMP = Join-Path $workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_sound_channel_volume.c') -o (Join-Path $workspace 'work/sonic-adventure/build/sound_channel_volume.dll')
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_sound_voice_mute_all.c') (Join-Path $PSScriptRoot 'source/sa_sound_voice_restore_all.c') (Join-Path $PSScriptRoot 'source/sa_sound_voice_stop_all.c') (Join-Path $PSScriptRoot 'source/sa_sound_mute_service.c') (Join-Path $PSScriptRoot 'source/sa_sound_restore_service.c') (Join-Path $PSScriptRoot 'source/sa_sound_handle_volume.c') (Join-Path $PSScriptRoot 'source/sa_sound_handle_volume_thunk.c') (Join-Path $PSScriptRoot 'source/sa_sound_subordinate_volume.c') (Join-Path $PSScriptRoot 'source/sa_sound_driver_volume.c') (Join-Path $PSScriptRoot 'source/sa_sound_channel_volume.c') (Join-Path $PSScriptRoot 'source/sa_sound_channel_volume_thunk.c') -o (Join-Path $workspace 'work/sonic-adventure/build/sound_control_bundle.dll')
if ($LASTEXITCODE -ne 0) { throw 'Sound control bundle build failed' }

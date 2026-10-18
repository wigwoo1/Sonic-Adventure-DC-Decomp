$ErrorActionPreference = 'Stop'
$workspace = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$env:TMP = Join-Path $workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' outputs/sonic-adventure/source/sa_engine_frame_transfer_submit.c outputs/sonic-adventure/source/sa_engine_transfer_copy_strided_io.c outputs/sonic-adventure/source/sa_engine_transfer_channels_arm.c outputs/sonic-adventure/source/sa_engine_transfer_channel_arm_io.c outputs/sonic-adventure/source/sa_engine_transfer_ready_wait.c outputs/sonic-adventure/source/sa_engine_transfer_ready_get_io.c outputs/sonic-adventure/source/sa_engine_transfer_fifo_wait.c outputs/sonic-adventure/source/sa_engine_transfer_fifo_ready_get_io.c outputs/sonic-adventure/source/sa_engine_transfer_channels_release.c outputs/sonic-adventure/source/sa_engine_transfer_channel_release_io.c outputs/sonic-adventure/source/sa_engine_transfer_busy_get_io.c outputs/sonic-adventure/source/sa_engine_error_record.c -o work/sonic-adventure/build/engine_frame_transfer_submit.dll
if ($LASTEXITCODE -ne 0) { throw 'Frame transfer submission build failed' }

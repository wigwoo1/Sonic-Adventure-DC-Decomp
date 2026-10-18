$ErrorActionPreference = 'Stop'
$workspace = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$env:TMP = Join-Path $workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_sound_decode_block_mono.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_sqrt.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/sa_sound_decode_block_stereo.c') -o (Join-Path $workspace 'work/sonic-adventure/build/sound_decode_blocks_draft.dll')
if ($LASTEXITCODE -ne 0) { throw 'Audio format initializer build failed' }



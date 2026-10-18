$ErrorActionPreference = 'Stop'
$workspace = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$env:TMP = Join-Path $workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_sound_data_chunk_state_initialize.c') (Join-Path $PSScriptRoot 'source/sa_sound_data_chunk_metadata.c') (Join-Path $PSScriptRoot 'source/sa_signed_division.c') (Join-Path $PSScriptRoot 'source/sa_unsigned_quotient.c') (Join-Path $PSScriptRoot 'source/sa_memory_fill.c') -o (Join-Path $workspace 'work/sonic-adventure/build/sound_data_chunk_state_initialize.dll')
if ($LASTEXITCODE -ne 0) { throw 'Data-chunk state initializer build failed' }

from pathlib import Path
p=Path(__file__).resolve().parent
b=(p/'build_sound_snd_state_initialize.ps1').read_text()
a=b.index('& $clang');z=b.index('\nif ($LASTEXITCODE',a)
b=b[:a]+"& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_sound_packed_state_initialize.c') (Join-Path $PSScriptRoot 'source/sa_sound_packed_header_metadata.c') (Join-Path $PSScriptRoot 'source/sa_memory_fill.c') -o (Join-Path $workspace 'work/sonic-adventure/build/sound_packed_state_initialize.dll')"+b[z:]
(p/'build_sound_packed_state_initialize.ps1').write_text(b)

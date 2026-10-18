$ErrorActionPreference = 'Stop'
$workspace = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$env:TMP = Join-Path $workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
& $clang -c -O2 -Wall -Wextra -Werror (Join-Path $PSScriptRoot 'source/sa_sound_format_engine.c') -o (Join-Path $workspace 'work/sonic-adventure/build/sound_format_engine.obj')
if ($LASTEXITCODE -ne 0) { throw 'Sound global volume build failed' }

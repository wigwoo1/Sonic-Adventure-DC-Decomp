$ErrorActionPreference = 'Stop'
$workspace = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$env:TMP = Join-Path $workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_math_cosine_polynomial.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sa_math_sine_polynomial.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $workspace 'work/sonic-adventure/build/math_trig_polynomials.dll')
if ($LASTEXITCODE -ne 0) { throw 'Audio format initializer build failed' }



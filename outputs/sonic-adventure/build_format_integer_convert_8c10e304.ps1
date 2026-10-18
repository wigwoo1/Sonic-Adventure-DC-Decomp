param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$sources = @(
    'source/sa_format_integer_convert.c',
    'source/sa_unsigned_quotient.c',
    'source/sa_unsigned_remainder.c'
) | ForEach-Object { Join-Path $PSScriptRoot $_ }
$output = Join-Path $Workspace 'work/sonic-adventure/build/format_integer_convert_8c10e304.dll'
New-Item -ItemType Directory -Force (Split-Path -Parent $output) | Out-Null
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o $output
if ($LASTEXITCODE -ne 0) { throw 'Formatter integer-conversion routine build failed' }

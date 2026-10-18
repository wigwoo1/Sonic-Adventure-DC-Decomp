param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$sources = @('source/sa_format_emit_text_8c10e50e.c', 'source/sa_byte_move.c') |
    ForEach-Object { Join-Path $PSScriptRoot $_ }
$output = Join-Path $Workspace 'work/sonic-adventure/build/format_emit_text_8c10e50e.dll'
New-Item -ItemType Directory -Force (Split-Path -Parent $output) | Out-Null
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o $output
if ($LASTEXITCODE -ne 0) { throw 'Formatter text-emission helper build failed' }

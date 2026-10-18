param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$sources = @(
    'sa_stream_request_seek_query_8c65bfa4.c',
    'sa_stream_request_position_update_8c600e7a.c',
    'sa_stream_request_open_prepare_8c65bf42.c'
) | ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
$output = Join-Path $Workspace 'work/sonic-adventure/build/stream_request_seek_query_8c65bfa4.dll'
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o $output
if ($LASTEXITCODE -ne 0) { throw 'Native seek/query caller build failed' }

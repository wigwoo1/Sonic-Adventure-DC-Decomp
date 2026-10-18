param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$sources = @('sa_request_abort_8c6013d2.c','sa_mmio_complement_sub_8c60260e.c','retail_scalar_helpers.c') |
    ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
$output = Join-Path $Workspace 'work/sonic-adventure/build/request_abort_8c6013d2.dll'
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o $output
if ($LASTEXITCODE -ne 0) { throw 'Request-abort native control-flow build failed' }

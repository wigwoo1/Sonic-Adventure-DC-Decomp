param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$sources = @(
    'source/sa_input_device_report_callback_8c65f348.c',
    'source/sa_debug_log_forward_8c10d400.c'
) | ForEach-Object { Join-Path $PSScriptRoot $_ }
$output = Join-Path $Workspace 'work/sonic-adventure/build/input_device_report_callback_8c65f348.dll'
New-Item -ItemType Directory -Force (Split-Path -Parent $output) | Out-Null
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o $output
if ($LASTEXITCODE -ne 0) { throw 'Input-device report callback build failed' }

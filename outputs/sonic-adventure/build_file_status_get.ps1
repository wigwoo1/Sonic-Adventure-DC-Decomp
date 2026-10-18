param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$python = 'C:\Users\wigwo\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe'
$sources = @((Join-Path $PSScriptRoot 'source/sa_file_status_get.c'))
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o (Join-Path $Workspace 'work/sonic-adventure/build/file_status_get.dll')
if ($LASTEXITCODE -ne 0) { throw 'File status getter build failed' }
# File-service/download dependencies and behavioral verification remain pending.

param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$python = 'C:\Users\wigwo\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe'
$sources = @((Join-Path $PSScriptRoot 'source/sa_file_name_upper.c'), (Join-Path $PSScriptRoot 'source/sa_file_name_equal.c'), (Join-Path $PSScriptRoot 'source/sa_file_name_equal_folded.c'), (Join-Path $PSScriptRoot 'source/sa_file_name_checksum.c'), (Join-Path $PSScriptRoot 'source/sa_file_filename_lookup.c'))
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o (Join-Path $Workspace 'work/sonic-adventure/build/file_filename_lookup.dll')
if ($LASTEXITCODE -ne 0) { throw 'Filename lookup build failed' }
# File-service/download dependencies and behavioral verification remain pending.

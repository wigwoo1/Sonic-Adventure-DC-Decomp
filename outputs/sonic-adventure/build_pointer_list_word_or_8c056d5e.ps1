param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$source = Join-Path $PSScriptRoot 'source/sa_pointer_list_word_or_8c056d5e.c'
$output = Join-Path $Workspace 'work/sonic-adventure/build/pointer_list_word_or_8c056d5e.dll'
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' $source -o $output
if ($LASTEXITCODE -ne 0) { throw 'Pointer-list word OR build failed' }

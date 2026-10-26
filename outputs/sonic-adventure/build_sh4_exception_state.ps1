param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path)
$ErrorActionPreference='Stop'
$taskBuild=Join-Path $Workspace 'work/sonic-adventure/build'
$taskClang='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
& $taskClang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sh4_exception_state.c') (Join-Path $PSScriptRoot 'source/sh4_integer_state.c') (Join-Path $PSScriptRoot 'source/sh4_signed_product.c') -o (Join-Path $taskBuild 'sh4_exception_state.dll')
if($LASTEXITCODE -ne 0){throw 'Exception state build failed'}

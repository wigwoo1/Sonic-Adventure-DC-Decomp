param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path)
$ErrorActionPreference='Stop'
$taskBuild=Join-Path $Workspace 'work/sonic-adventure/build'
[void][System.IO.Directory]::CreateDirectory($taskBuild)
$taskClang='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
& $taskClang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/flash_storage.c') -o (Join-Path $taskBuild 'flash_storage.dll')
if($LASTEXITCODE -ne 0){throw 'Authored flash-storage build failed'}

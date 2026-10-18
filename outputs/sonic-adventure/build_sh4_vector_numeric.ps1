param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference='Stop'
$taskBuild=Join-Path $Workspace 'work/sonic-adventure/build'
$env:TMP=Join-Path $taskBuild 'tmp'
$env:TEMP=$env:TMP
New-Item -ItemType Directory -Force $env:TMP | Out-Null
$taskClang='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
& $taskClang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sh4_vector_numeric.c') -o (Join-Path $taskBuild 'sh4_vector_numeric.dll')
if($LASTEXITCODE -ne 0){throw 'Native SH-4 numerical instructions build failed'}
& $taskClang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sh4_vector_numeric_reference.c') -o (Join-Path $taskBuild 'sh4_vector_numeric_reference.dll')
if($LASTEXITCODE -ne 0){throw 'SH-4 primitive plumbing oracle build failed'}

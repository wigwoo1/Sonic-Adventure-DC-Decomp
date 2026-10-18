param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference='Stop'
$taskBuild=Join-Path $Workspace 'work/sonic-adventure/build'
$env:TMP=Join-Path $taskBuild 'tmp'
$env:TEMP=$env:TMP
New-Item -ItemType Directory -Force $env:TMP | Out-Null
$taskClang='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$taskSources=@((Join-Path $PSScriptRoot 'source/sa_main_dispatch_transition_8c0aff1a.c'),(Join-Path $PSScriptRoot 'source/sh4_add_sub.c'))
& $taskClang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @taskSources -o (Join-Path $taskBuild 'main_dispatch_transition.dll')
if($LASTEXITCODE -ne 0){throw 'Main dispatcher transition native build failed'}

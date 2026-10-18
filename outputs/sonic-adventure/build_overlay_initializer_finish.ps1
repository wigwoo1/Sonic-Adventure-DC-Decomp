param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference='Stop'
$taskBuild=Join-Path $Workspace 'work/sonic-adventure/build'
$env:TMP=Join-Path $taskBuild 'tmp'
$env:TEMP=$env:TMP
New-Item -ItemType Directory -Force $env:TMP | Out-Null
$taskClang='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$taskSources=@('sa_overlay_initializer_finish.c','sa_overlay_initializer_finish_connected.c','sa_random_u15_8c10d334.c','sh4_add_sub.c','sh4_multiply_extended.c','sh4_multiply.c','sh4_fmac.c','sh4_divide.c','sh4_float_i32.c','sh4_truncate_i32.c','sh4_compare.c') | ForEach-Object {Join-Path $PSScriptRoot ('source/'+$_)}
& $taskClang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @taskSources -o (Join-Path $taskBuild 'overlay_initializer_finish.dll')
if($LASTEXITCODE -ne 0){throw 'Native overlay model-blend build failed'}

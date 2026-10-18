param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference='Stop'
$taskBuild=Join-Path $Workspace 'work/sonic-adventure/build'
$env:TMP=Join-Path $taskBuild 'tmp'
$env:TEMP=$env:TMP
New-Item -ItemType Directory -Force $env:TMP | Out-Null
$taskClang='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$taskSources=@('sa_main_dispatch_transition_8c0aff1a.c','sa_main_dispatch_early_gate_8c0afe72.c','sa_main_dispatch_transition_early_native.c','sa_main_dispatch_value_native.c','sh4_add_sub.c') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_) }
& $taskClang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @taskSources -o (Join-Path $taskBuild 'main_dispatch_early_native.dll')
if($LASTEXITCODE -ne 0){throw 'Main dispatcher early-dependency native build failed'}

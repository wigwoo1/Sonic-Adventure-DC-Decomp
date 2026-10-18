param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference='Stop'
$taskBuild=Join-Path $Workspace 'work/sonic-adventure/build'
$env:TMP=Join-Path $taskBuild 'tmp'
$env:TEMP=$env:TMP
New-Item -ItemType Directory -Force $env:TMP | Out-Null
$taskClang='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$taskSources=@('sa_main_dispatch_shared_helpers.c','sa_main_dispatch_status_query.c','sa_main_dispatch_shared_check.c','sa_main_dispatch_active_query.c','sa_main_dispatch_reset.c','sa_main_dispatch_detach.c','sa_main_dispatch_transform.c','sa_main_dispatch_matrix_init.c','sa_main_dispatch_scalar_complete_connected.c','sa_matrix_rotate_x.c','sa_matrix_rotate_y.c','sa_matrix_rotate_z.c','model_matrix_stack_pop.c','model_direction_transform_extended.c','sh4_float_i32.c','sh4_multiply.c','sh4_multiply_extended.c','sh4_fmac.c','sh4_vector_numeric.c') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_) }
& $taskClang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @taskSources -o (Join-Path $taskBuild 'main_dispatch_scalar_complete.dll')
if($LASTEXITCODE -ne 0){throw 'Extended scalar numerical transform chain build failed'}

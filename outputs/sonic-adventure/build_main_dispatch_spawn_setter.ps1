param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference='Stop'
$taskBuild=Join-Path $Workspace 'work/sonic-adventure/build'
$env:TMP=Join-Path $taskBuild 'tmp'
$env:TEMP=$env:TMP
New-Item -ItemType Directory -Force $env:TMP | Out-Null
$taskClang='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$taskSources=@('sa_main_dispatch_notification.c','sa_main_dispatch_spawn_setter.c','sa_main_dispatch_spawn_setter_connected.c','sa_input_channel_info_pointer.c','sa_input_device_record_index_helpers.c','callback_tail_thunks.c','sa_signed_division.c','sa_task_delete.c','sa_task_data_release.c','sa_task_children_delete.c','sa_task_children_update.c','sa_heap_release.c','sa_task_create.c','sa_task_create_base.c','sa_heap_allocate.c','sa_heap_allocate_zeroed.c','sa_main_dispatch_prefix_gate.c','sa_main_dispatch_sound.c','sh4_multiply_extended.c','sh4_multiply.c','sh4_fmac.c') | ForEach-Object {Join-Path $PSScriptRoot ('source/'+$_)}
& $taskClang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @taskSources -o (Join-Path $taskBuild 'main_dispatch_spawn_setter.dll')
if($LASTEXITCODE -ne 0){throw 'Main dispatcher native spawn and callback setter build failed'}

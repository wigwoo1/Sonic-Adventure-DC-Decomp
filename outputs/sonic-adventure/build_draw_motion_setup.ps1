param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference='Stop'
$taskBuild=Join-Path $Workspace 'work/sonic-adventure/build'
$env:TMP=Join-Path $taskBuild 'tmp'
$env:TEMP=$env:TMP
New-Item -ItemType Directory -Force $env:TMP | Out-Null
$taskClang='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$taskSources=@('sa_global_word_bit_updates_8c056986.c','sa_store_nonzero_third_global_pointer_8c608ccc.c','sa_copy_global_pair_8c056926.c','sa_global_pair_flags_or_8c05694a.c','sa_restore_global_pair_8c056938.c','model_matrix_stack_push.c','model_matrix_stack_pop.c','model_matrix_translate.c','sa_matrix_rotate_x.c','sa_matrix_rotate_y.c','sa_matrix_rotate_z.c','sa_draw_matrix_registers.c','sa_overlay_object_draw_candidate.c','sa_draw_motion_setup_connected.c','sa_draw_motion_setup.c','sa_main_dispatch_particle_callback.c','model_matrix_rotate.c','model_matrix_rotate_implicit.c','sa_matrix_rotate_yxz.c','retail_empty_stubs.c','sa_draw_distance_helpers.c','sh4_sqrt.c','sa_overlay_parameter_update.c','sa_draw_predicate_support.c','sh4_add_sub.c','sh4_multiply_extended.c','sh4_multiply.c','sh4_fmac.c','sh4_divide.c','sh4_vector_numeric.c','sh4_compare.c','sh4_float_i32.c','sh4_truncate_i32.c') | ForEach-Object {Join-Path $PSScriptRoot ('source/'+$_)}
& $taskClang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @taskSources -o (Join-Path $taskBuild 'draw_motion_setup.dll')
if($LASTEXITCODE -ne 0){throw 'Partial object-draw gate build failed'}

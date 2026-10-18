param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference='Stop'
$taskBuild=Join-Path $Workspace 'work/sonic-adventure/build'
$env:TMP=Join-Path $taskBuild 'tmp'
$env:TEMP=$env:TMP
New-Item -ItemType Directory -Force $env:TMP | Out-Null
$taskClang='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$taskSources=@('sa_overlay_object_variants.c','sa_overlay_object_variants_connected.c','sa_overlay_object_setup.c','sa_overlay_object_setup_connected.c','sa_overlay_object_initializer_connected.c','sa_overlay_selector_initializer.c','sa_overlay_face_model_selector_0c905548.c','sa_overlay_selector_guard.c','retail_return_delay_stubs.c','sa_heap_allocate.c','sa_heap_release.c','byte_copy_forward.c') | ForEach-Object {Join-Path $PSScriptRoot ('source/'+$_)}
& $taskClang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @taskSources -o (Join-Path $taskBuild 'overlay_object_initializer.dll')
if($LASTEXITCODE -ne 0){throw 'Native overlay object initializer build failed'}

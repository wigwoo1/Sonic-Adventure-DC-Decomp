param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$source = Join-Path $PSScriptRoot 'source/sa_record_setup_initialize_8c647e16.c'
$helpers = @(
    (Join-Path $PSScriptRoot 'source/sa_record_callback_mode_select_8c647de8.c'),
    (Join-Path $PSScriptRoot 'source/sa_record_mode_value_store_8c647f10.c'),
    (Join-Path $PSScriptRoot 'source/sa_record_buffer_descriptors_build_8c647f4c.c'),
    (Join-Path $PSScriptRoot 'source/global_setter_wrappers.c'),
    (Join-Path $PSScriptRoot 'source/retail_constant_setters.c')
)
$output = Join-Path $Workspace 'work/sonic-adventure/build/sa_record_setup_initialize_8c647e16.dll'
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' $source $helpers -o $output
if ($LASTEXITCODE -ne 0) { throw 'Record setup initializer build failed' }

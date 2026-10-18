param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$output = Join-Path $Workspace 'work/sonic-adventure/build/input_service_cleanup_native.dll'
$sources = @(
    'source/sa_input_service_cleanup_8c646654.c', 'source/input_service_cleanup_native.c',
    'source/sa_input_device_submit_begin_8c65f1aa.c',
    'source/sa_input_device_response_complete.c', 'source/sa_input_device_presence_decode.c',
    'source/sa_input_device_response_header.c', 'source/sa_input_device_primary_response.c',
    'source/sa_input_device_sub_response.c', 'source/sa_input_device_info_decode.c',
    'source/sa_input_response_word_swap.c', 'source/sa_input_device_info_initialize.c',
    'source/sa_input_word_fill.c', 'source/input_device_response_native.c',
    'source/sa_input_device_responses_process.c', 'source/sa_input_device_port_response_mask.c',
    'source/input_device_responses_process_native.c', 'source/sa_input_device_status_publish.c',
    'source/sa_input_device_submit_rearm.c', 'source/sa_input_active_device_select_8c64658a.c',
    'source/sa_input_request_completion_impl.c', 'source/sa_input_request_completion.c',
    'source/sa_input_channel_status_pointer.c', 'source/sa_input_channel_info_pointer.c',
    'source/sa_input_device_record_pointer.c'
) | ForEach-Object { Join-Path $PSScriptRoot $_ }
New-Item -ItemType Directory -Force (Split-Path -Parent $output) | Out-Null
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o $output
if ($LASTEXITCODE -ne 0) { throw 'Native input-service cleanup build failed' }

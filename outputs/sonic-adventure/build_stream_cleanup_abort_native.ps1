param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)),
      [switch]$WithDiscPlatform,
      [ValidateSet('stream_cleanup_abort_native.dll','stream_cleanup_disc_native.dll')][string]$OutputName = 'stream_cleanup_abort_native.dll')
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$sources = @('sa_stream_cleanup_abort_native.c','sa_sound_stream_pending_cleanup_8c65bf60.c',
             'sa_request_abort_8c6013d2.c','sa_mmio_complement_sub_8c60260e.c','retail_scalar_helpers.c') |
    ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
$sources += @('sa_request_finalize_8c600cf0.c','sa_request_status_8c6013a4.c','sa_device_cancel_path.c','sa_file_backend_poll_state.c') |
    ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
$sources += Join-Path $PSScriptRoot 'source/sa_indirect_service_tail_wrappers_8c10d938.c'
$sources += @('sa_file_completion_process.c','sa_file_completion_connected.c','sa_file_device_status.c') |
    ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
if ($WithDiscPlatform) {
    $sources += @('native_disc_queue.c','native_disc_sector_reader.c','native_disc_read_command.c','native_disc_service_bridge.c','sa_file_close.c') |
        ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
}
$output = Join-Path $Workspace "work/sonic-adventure/build/$OutputName"
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o $output
if ($LASTEXITCODE -ne 0) { throw 'Connected native cleanup/abort build failed' }

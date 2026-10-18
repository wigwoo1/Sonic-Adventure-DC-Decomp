param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$sources = @('native_disc_queue.c','native_disc_sector_reader.c','native_disc_read_command.c','sa_file_device_read.c','sa_file_bios_request.c','sa_file_device_status.c','sa_file_bios_status.c') | ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
$sources += @('sa_file_handle_open_initialize.c','sa_file_read_aligned.c','sa_file_bios_execute.c','sa_callback_service_call_return_one_8c6452d6.c') |
    ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
$sources += @('sa_file_directory_load.c','sa_file_directory_record_parse.c','sa_file_directory_bytes_copy.c','sa_file_name_upper.c','sa_file_name_equal.c','sa_file_name_equal_folded.c','sa_file_name_checksum.c','sa_file_filename_lookup.c') |
    ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
$sources += @('sa_file_mount_sector_read.c','sa_file_reserved_handle_acquire.c','sa_file_read_sectors.c') |
    ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
$sources += @('sa_file_context_initialize.c','sa_file_directory_storage_initialize.c','sa_file_device_interface_get.c','sa_file_device_initialize.c','sa_file_device_low_initialize.c') |
    ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
$sources += @('sa_file_device_disc_request.c','sa_file_device_disc_info.c','sa_file_bios_disc_info.c') |
    ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
$sources += Join-Path $PSScriptRoot 'source/sa_file_device_version_request.c'
$sources += @('sa_file_mount.c','sa_file_volume_signature_check.c','sa_file_volume_word_get.c','sa_file_directory_parent_sector_adjust.c','sa_file_directory_select.c',
              'sa_interrupt_callback_create.c','sa_interrupt_handler_primary.c','sa_interrupt_handler_set.c','sa_mount_driver_enable.c') |
    ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
$sources += @('sa_file_backend_install.c','sa_file_open_dispatch.c','sa_file_backend_open.c','sa_file_error_set.c') |
    ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
$sources += Join-Path $PSScriptRoot 'source/sa_file_close.c'
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o (Join-Path $Workspace 'work/sonic-adventure/build/native_disc_queue.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native disc queue build failed' }

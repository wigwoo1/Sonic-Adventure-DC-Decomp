param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$sources = @(
    'sa_engine_context_initialize.c','sa_engine_option_select.c','sa_engine_context_parameters_initialize.c',
    'sa_interrupt_defaults_initialize.c','sa_string_compare_wrapper.c','sa_string_compare.c',
    'sa_engine_transfer_initialize.c','sa_engine_transfer_descriptor_reset_io.c','sa_engine_transfer_address_window_set_io.c',
    'sa_indirect_service_tail_wrappers_8c10d938.c','sa_file_close.c','sa_file_device_version_request.c','native_disc_queue.c',
    'native_disc_sector_reader.c','native_disc_read_command.c','sa_file_device_read.c','sa_file_bios_request.c',
    'sa_file_device_status.c','sa_file_bios_status.c','sa_file_handle_open_initialize.c','sa_file_read_aligned.c',
    'sa_file_bios_execute.c','sa_callback_service_call_return_one_8c6452d6.c','sa_file_directory_load.c','sa_file_directory_record_parse.c',
    'sa_file_directory_bytes_copy.c','sa_file_name_upper.c','sa_file_name_equal.c','sa_file_name_equal_folded.c',
    'sa_file_name_checksum.c','sa_file_filename_lookup.c','sa_file_mount_sector_read.c','sa_file_reserved_handle_acquire.c',
    'sa_file_read_sectors.c','sa_file_context_initialize.c','sa_file_directory_storage_initialize.c','sa_file_device_interface_get.c',
    'sa_file_device_initialize.c','sa_file_device_low_initialize.c','sa_file_device_disc_request.c','sa_file_device_disc_info.c',
    'sa_file_bios_disc_info.c','sa_file_mount.c','sa_file_volume_signature_check.c','sa_file_volume_word_get.c',
    'sa_file_directory_parent_sector_adjust.c','sa_file_directory_select.c','sa_interrupt_callback_create.c','sa_interrupt_handler_primary.c',
    'sa_interrupt_handler_set.c','sa_mount_driver_enable.c','sa_file_backend_install.c','sa_file_open_dispatch.c',
    'sa_file_backend_open.c','sa_file_error_set.c','sa_stream_cleanup_abort_native.c','sa_sound_stream_pending_cleanup_8c65bf60.c',
    'sa_request_abort_8c6013d2.c','sa_mmio_complement_sub_8c60260e.c','retail_scalar_helpers.c','sa_request_finalize_8c600cf0.c',
    'sa_request_status_8c6013a4.c','sa_device_cancel_path.c','sa_file_backend_poll_state.c','sa_file_completion_process.c',
    'sa_file_completion_connected.c','sa_sound_bank_load.c','sa_file_size_get.c','sa_file_device_start.c',
    'native_sound_bank_load.c','engine_context_initialize_native.c','sa_engine_error_record.c','sa_engine_frame_debug_log.c',
    'sa_engine_frame_device_layout_initialize.c','sa_engine_frame_initialize.c','sa_engine_frame_object_allocate.c','sa_engine_frame_object_configure.c',
    'sa_engine_frame_object_pool_initialize.c','sa_engine_frame_object_release.c','sa_engine_frame_queue_initialize.c','sa_engine_frame_request_enqueue.c',
    'sa_engine_frame_request_pool_initialize.c','sa_engine_frame_services.c','sa_engine_frame_transfer_begin.c','sa_engine_frame_transfer_submit.c',
    'sa_engine_frame_work_initialize.c','sa_engine_object_address_set.c','sa_engine_object_callback_dispatch.c','sa_engine_object_callback_set.c',
    'sa_engine_object_length_set.c','sa_engine_object_state_get.c','sa_engine_object_state_set.c','sa_engine_pending_count_get.c',
    'sa_engine_queue_consume_direct.c','sa_engine_record_copy28_tail.c','sa_engine_record_copy32_tail.c','sa_engine_record_copy8_tail.c',
    'sa_engine_record_copy_dispatch.c','sa_engine_record_copy_loaded_dispatch.c','sa_engine_transfer_busy_get_io.c','sa_engine_transfer_channel_arm_io.c',
    'sa_engine_transfer_channel_release_io.c','sa_engine_transfer_channels_arm.c','sa_engine_transfer_channels_release.c','sa_engine_transfer_copy_strided_io.c',
    'sa_engine_transfer_descriptor_acquire.c','sa_engine_transfer_fifo_ready_get_io.c','sa_engine_transfer_fifo_wait.c','engine_transfer_initialize_host.c',
    'sa_engine_transfer_read_strided_io.c','sa_engine_transfer_ready_get_io.c','sa_engine_transfer_ready_wait.c','sa_file_read_unaligned.c',
    'sa_file_status_get.c','sa_memory_fill.c','sa_memory_move.c','sa_sound_bank_download.c',
    'sa_sound_bank_section_configure.c','sa_sound_bank_section_lookup.c','sa_sound_bank_section_transfer.c','sa_sound_bank_word_write.c',
    'sa_sound_command_complete.c','sa_sound_command_type1.c','sa_sound_command_type2.c','sa_sound_low_index.c',
    'sa_sound_midi_enqueue.c','sa_sound_midi_pack.c','sa_sound_multi_unit_download.c','sa_sound_request_commit.c',
    'sa_sound_request_copy20.c','sa_sound_slot_acquire.c','sa_sound_slot_acquire_wrapper.c','sa_sound_status_byte_get.c',
    'sa_sound_status_byte_set.c','sa_sound_status_word_get.c','sa_sound_status_word_set.c','sa_sound_volume_control.c',
    'sa_sound_work_dispatch.c'
) | ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o (Join-Path $Workspace 'work/sonic-adventure/build/native_sound_bank_load.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native sound bank load build failed' }

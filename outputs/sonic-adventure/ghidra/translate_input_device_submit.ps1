param([Parameter(Mandatory=$true)][string]$Python)
$ErrorActionPreference='Stop'
foreach ($range in @(@('submit','8c65efda','8c65f1aa'),@('commands_prepare','8c65eeda','8c65efda'),@('error_report','8c65f348','8c65f390'))) {
 & $Python "$PSScriptRoot/translate_tikal_helper.py" "sa_input_device_$($range[0])" $range[1] $range[2] input_device_submission.tsv
 if ($LASTEXITCODE -ne 0) { throw "Submission translation failed: $($range[0])" }
}

foreach ($range in @(@('features','8c66034e','8c66036c'),@('four','8c6603f4','8c66040c'),@('three','8c660438','8c660450'),@('two','8c66047c','8c660494'))) {
 & $Python "$PSScriptRoot/translate_tikal_helper.py" "sa_input_device_command_$($range[0])" $range[1] $range[2] input_device_discovery_commands.tsv
 if ($LASTEXITCODE -ne 0) { throw 'Device automatic command translation failed' }
}
& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_input_device_submit_rearm 8c65f1ec 8c65f202 input_device_processing.tsv
if ($LASTEXITCODE -ne 0) { throw 'Device submit rearm translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_input_device_wait_clock 8c60260e 8c602618 input_device_wait_clock.tsv
if ($LASTEXITCODE -ne 0) { throw 'Wait clock translation failed' }
& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_input_device_elapsed_ticks 8c602618 8c60261e input_device_wait_clock.tsv
if ($LASTEXITCODE -ne 0) { throw 'Elapsed ticks translation failed' }
& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_input_device_shutdown 8c65f2a4 8c65f348 input_device_processing.tsv
if ($LASTEXITCODE -ne 0) { throw 'Device shutdown translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_input_device_service_shutdown 8c6467f0 8c646856 input_status_table_writers.tsv
if ($LASTEXITCODE -ne 0) { throw 'Input service shutdown translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_input_service_initialize 8c6461f0 8c646294 input_status_table_writers.tsv
if ($LASTEXITCODE -ne 0) { throw 'Input service initialization translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_input_service_start_explicit 8c646356 8c6463ac input_status_table_writers.tsv
if ($LASTEXITCODE -ne 0) { throw 'Outer input service startup translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_input_service_start_contiguous 8c646304 8c646356 input_status_table_writers.tsv
if ($LASTEXITCODE -ne 0) { throw 'Contiguous input startup translation failed' }
& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_input_device_contiguous_initialize 8c65ebb0 8c65ec9c input_device_allocate_initialize.tsv
if ($LASTEXITCODE -ne 0) { throw 'Contiguous backend initialization translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_interrupt_defaults_initialize 8c641160 8c6411b0 interrupt_exception_setup.tsv
if ($LASTEXITCODE -ne 0) { throw 'Interrupt defaults translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_context_initialize 8c6510a4 8c651368 8c6510a4.tsv
if ($LASTEXITCODE -ne 0) { throw 'Engine context translation failed' }
& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_option_select 8c665a80 8c665aac 8c665a80.tsv
if ($LASTEXITCODE -ne 0) { throw 'Engine option selector translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_context_parameters_initialize 8c66120c 8c66126c engine_context_dependency.tsv
if ($LASTEXITCODE -ne 0) { throw 'Context parameters translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_string_compare 8c10db1c 8c10dbc4 8c10db1c.tsv
if ($LASTEXITCODE -ne 0) { throw 'String comparison translation failed' }
& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_string_compare_wrapper 8c10d3c8 8c10d3d6 8c10d3c8.tsv
if ($LASTEXITCODE -ne 0) { throw 'String comparison wrapper translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_frame_services 8c668f40 8c668fda input_work_lifecycle_tail.tsv
if ($LASTEXITCODE -ne 0) { throw 'Engine frame services translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_object_callback_dispatch 8c65a518 8c65a55a engine_frame_common.tsv
if ($LASTEXITCODE -ne 0) { throw 'Object callback dispatch translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_pending_count_get 8c66abfc 8c66ac2a engine_frame_pending.tsv
if ($LASTEXITCODE -ne 0) { throw 'Pending count translation failed' }
& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_error_record 8c65acac 8c65acb6 engine_error_report.tsv
if ($LASTEXITCODE -ne 0) { throw 'Engine error record translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_queue_pop 8c666f98 8c66706a engine_frame_queue_values.tsv
if ($LASTEXITCODE -ne 0) { throw 'Engine queue pop translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_queue_consume_context 8c666d10 8c666e02 engine_frame_consumer.tsv
if ($LASTEXITCODE -ne 0) { throw 'Context consumer translation failed' }
& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_object_state_get 8c65a568 8c65a59a engine_frame_common.tsv
if ($LASTEXITCODE -ne 0) { throw 'Object state getter translation failed' }
& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_object_state_set 8c65a5cc 8c65a5d2 engine_frame_common.tsv
if ($LASTEXITCODE -ne 0) { throw 'Object state setter translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_transfer_descriptor_initialize 8c10f9f4 8c10fa52 engine_context_dispatch.tsv
if ($LASTEXITCODE -ne 0) { throw 'Transfer descriptor translation failed' }
& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_transfer_submit 8c10faa4 8c10fb32 engine_context_dispatch.tsv
if ($LASTEXITCODE -ne 0) { throw 'Transfer submit translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_transfer_busy_get 8c1101f0 8c110214 engine_transfer_state.tsv
if ($LASTEXITCODE -ne 0) { throw 'Transfer busy getter translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_cache_writeback 8c5fc58a 8c5fc7de engine_transfer_cache.tsv
if ($LASTEXITCODE -ne 0) { throw 'Cache writeback translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_transfer_channels_arm 8c10ee9c 8c10eed0 engine_transfer_dispatch_core.tsv
if ($LASTEXITCODE -ne 0) { throw "sa_engine_transfer_channels_arm translation failed" }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_transfer_channel_arm 8c10fa60 8c10fa96 engine_context_dispatch.tsv
if ($LASTEXITCODE -ne 0) { throw "sa_engine_transfer_channel_arm translation failed" }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_transfer_ready_get 8c10fb48 8c10fb56 engine_context_dispatch.tsv
if ($LASTEXITCODE -ne 0) { throw "sa_engine_transfer_ready_get translation failed" }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_transfer_ready_wait 8c10eee0 8c10ef12 engine_transfer_dispatch_core.tsv
if ($LASTEXITCODE -ne 0) { throw "sa_engine_transfer_ready_wait translation failed" }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_transfer_complete 8c10ebe8 8c10ec36 engine_transfer_controller.tsv
if ($LASTEXITCODE -ne 0) { throw 'Transfer completion translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_transfer_descriptor_acquire 8c10ec40 8c10ed4a engine_transfer_controller.tsv
if ($LASTEXITCODE -ne 0) { throw 'Transfer descriptor acquisition translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_transfer_initialize 8c10d5a8 8c10d72a engine_transfer_executor.tsv
if ($LASTEXITCODE -ne 0) { throw "sa_engine_transfer_initialize translation failed" }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_transfer_descriptor_reset 8c10ed54 8c10ee1a engine_transfer_controller.tsv
if ($LASTEXITCODE -ne 0) { throw "sa_engine_transfer_descriptor_reset translation failed" }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_transfer_address_window_set 8c10ee64 8c10ee8c engine_transfer_dispatch_core.tsv
if ($LASTEXITCODE -ne 0) { throw "sa_engine_transfer_address_window_set translation failed" }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_interrupt_event_dispatch 8c645afa 8c645b1e interrupt_event_entry.tsv
if ($LASTEXITCODE -ne 0) { throw 'Event dispatcher translation failed' }

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_engine_queue_consume_direct 8c65a5dc 8c65a724 engine_frame_common.tsv
if ($LASTEXITCODE -ne 0) { throw 'Direct queue consumer translation failed' }

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_queue_push 8c667080 8c667134 engine_frame_queue_values.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_frame_transfer_begin 8c666c2c 8c666ce2 engine_frame_gate.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_frame_transfer_submit 8c669088 8c6690f4 engine_frame_transfer_submit.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_transfer_copy_strided 8c10d770 8c10d908 engine_transfer_executor.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_transfer_fifo_wait 8c10ebac 8c10ebde engine_transfer_controller.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_transfer_channels_release 8c10ee20 8c10ee54 engine_transfer_dispatch_core.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_transfer_fifo_ready_get 8c10f9a8 8c10f9b6 engine_transfer_copy_control.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_transfer_channel_release 8c10f9bc 8c10f9ec engine_transfer_copy_control.tsv
& $python outputs/sonic-adventure/ghidra/specialize_input_io.py

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_transfer_error_count 8c666e06 8c666e10 engine_frame_consumer.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_frame_initialize 8c65acc0 8c65ad26 engine_frame_initialize.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_frame_object_pool_initialize 8c666e50 8c666ed0 engine_frame_pool_queue_setup.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_frame_queue_initialize 8c666ef0 8c666f72 engine_frame_pool_queue_setup.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_record_copy_dispatch 8c10cd38 8c10cd44 engine_frame_pool_links.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_record_copy32_tail 8c10cd76 8c10cd98 engine_frame_pool_links.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_record_copy8_tail 8c10cd8e 8c10cd98 engine_frame_pool_links.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_frame_request_pool_initialize 8c6669c4 8c666a46 engine_frame_initialize_helpers.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_record_copy_loaded_dispatch 8c10cd44 8c10cd50 engine_frame_pool_links.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_record_copy28_tail 8c10cd7a 8c10cd98 engine_frame_pool_links.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_frame_device_layout_initialize 8c667240 8c6672de engine_frame_pool_queue_setup.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_frame_work_initialize 8c666b34 8c666b84 engine_frame_initialize_helpers.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_frame_object_allocate 8c6434e0 8c643588 engine_frame_object_allocate.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_frame_debug_log 8c65a2fc 8c65a33e engine_frame_debug_log.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_frame_mode_set 8c6436d0 8c6436e0 engine_frame_mode_set.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_frame_object_release 8c6435bc 8c64360c engine_frame_object_setup.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_frame_object_configure 8c643624 8c6436a4 engine_frame_object_setup.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_object_address_set 8c65a5ac 8c65a5b4 engine_frame_common.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_object_callback_set 8c65a5b4 8c65a5cc engine_frame_common.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_object_length_set 8c65a5d4 8c65a5dc engine_frame_common.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_frame_request_enqueue 8c666ba8 8c666c14 engine_frame_initialize_helpers.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_sound_request_process 8c65a378 8c65a43a sound_request_process.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_sound_work_dispatch 8c666a68 8c666ad0 engine_frame_initialize_helpers.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_sound_status_byte_get 8c6671c4 8c6671f8 engine_frame_pool_queue_setup.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_sound_status_word_get 8c66910c 8c669138 engine_frame_transfer_submit.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_engine_transfer_read_strided 8c10d460 8c10d59a engine_transfer_executor.tsv
& $python outputs/sonic-adventure/ghidra/specialize_input_io.py

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_sound_status_word_set 8c66ac3c 8c66ac6e engine_frame_pending.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_sound_status_byte_set 8c669144 8c669186 engine_frame_transfer_submit.tsv

& $python outputs/sonic-adventure/ghidra/translate_tikal_helper.py sa_sound_command_complete 8c666ad0 8c666b14 engine_frame_initialize_helpers.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_request_commit 8c65a464 8c65a4fa sound_request_commit.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_request_copy20 8c10cd82 8c10cd98 sound_request_copy20.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_command_type2 8c643e08 8c643e4c sound_command_type2.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_index_dispatch 8c093b70 8c093b90 sound_index_dispatch.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_low_index 8c0933ba 8c09340c sound_low_index.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_midi_enqueue 8c643b9c 8c643bf2 sound_midi_dependencies.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_midi_pack 8c643c18 8c643d78 sound_midi_dependencies.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_object_table_initialize 8c093c74 8c093ca4 sound_object_table_initialize.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_slot_acquire_wrapper 8c643ac0 8c643af4 sound_slot_acquire_wrapper.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_slot_acquire 8c65aa30 8c65aade sound_slot_acquire.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_slot_release 8c66b888 8c66b8b8 sound_slot_release.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_slot_validate 8c669194 8c6691c0 sound_slot_validate.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_note_on 8c093360 8c0933ba sound_note_on.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_volume_control 8c093240 8c09328e sound_volume_control.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_pan_control 8c09328e 8c0932e0 sound_pan_control.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_pitch_bend 8c0932e0 8c093340 sound_pitch_bend.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_channels_reset 8c09306c 8c093094 sound_channel_reset.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_channel_reset 8c093094 8c0930e0 sound_channel_reset.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_program_setup 8c093140 8c093240 sound_program_setup.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_volume_dispatch 8c093be0 8c093c32 sound_volume_dispatch.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_command_type5 8c643d88 8c643de2 sound_command_type5.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_command_type1 8c643b08 8c643b74 sound_command_type1.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_play_dispatch 8c093b2a 8c093b70 sound_play_dispatch.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_bank_load 8c0939b0 8c0939f2 sound_bank_load.tsv

& $python (Join-Path $PSScriptRoot "translate_tikal_helper.py") sa_sound_bank_download 8c093ce0 8c093d2c sound_bank_download.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_sound_multi_unit_download 8c643970 8c643a7a sound_multi_unit_download.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_sound_bank_section_configure 8c65a730 8c65a8a4 sound_bank_section_configure.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_sound_bank_section_transfer 8c643904 8c64395c sound_bank_section_transfer.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_sound_bank_section_lookup 8c65a900 8c65aa22 sound_bank_section_lookup.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_sound_bank_word_write 8c667200 8c667232 sound_bank_word_write.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_file_close 8c600df4 8c600e2c file_close_and_size.tsv
& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_file_size_get 8c600e2c 8c600e46 file_close_and_size.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_file_close_cleanup 8c6013d2 8c601500 file_close_cleanup.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_file_clock_get 8c60260e 8c602618 file_timer_helpers.tsv
& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_file_elapsed_get 8c602618 8c60261e file_timer_helpers.tsv
& $python (Join-Path $PSScriptRoot 'specialize_file_clock_io.py')

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_file_release_wait 8c600cf0 8c600d82 file_release_wait.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_file_status_get 8c6013a4 8c6013bc file_status_get.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_file_read_sectors 8c600f1e 8c600fac file_read_sectors.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_file_read_aligned 8c601084 8c60116e file_read_aligned.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_file_progress_get 8c600ec6 8c600edc file_read_complete.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_file_read_unaligned 8c600fac 8c601084 file_read_sectors.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_file_open_dispatch 8c6016d6 8c6016ec file_open_dispatch.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_file_backend_open 8c6004d4 8c60054c file_backend_open.tsv

& $python (Join-Path $PSScriptRoot 'translate_tikal_helper.py') sa_file_handle_open_initialize 8c600464 8c6004d4 file_handle_open_initialize.tsv

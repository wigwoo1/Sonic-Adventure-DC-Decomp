param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$buildTemporaryDirectory = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
New-Item -ItemType Directory -Force $buildTemporaryDirectory | Out-Null
$env:TMP = $buildTemporaryDirectory
$env:TEMP = $buildTemporaryDirectory
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$python = 'C:\Users\wigwo\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe'
$output = Join-Path $Workspace 'work/sonic-adventure/build/startup_sections.dll'
New-Item -ItemType Directory -Force (Split-Path -Parent $output) | Out-Null
& (Join-Path $PSScriptRoot 'build_startup_entry.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_entry.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup entry equivalence check failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_entry.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup entry registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_context_copy.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_context_copy.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup context-copy verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_context_copy.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup context-copy registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_context_restore.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_context_restore.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup context-restore verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_context_restore.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup context-restore registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_retry_helper.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_retry_helper.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup retry helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_retry_helper.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup retry helper registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_transfer_entry.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_transfer_entry.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup transfer coordinator verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_transfer_entry.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup transfer coordinator registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_wrapper_8c011828.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_wrapper_8c011828.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup service wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_wrapper_8c011828.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup service wrapper registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_copy_device_info.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_copy_device_info.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup device-info copy verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_copy_device_info.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup device-info copy registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_hex_digit_index.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_hex_digit_index.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup hex-digit lookup verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_hex_digit_index.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup hex-digit lookup registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_register_internal_name.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_register_internal_name.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup internal-name registration verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_register_internal_name.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup internal-name registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_wait_for_registration.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_wait_for_registration.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup registration-wait verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_wait_for_registration.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup registration-wait registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_retry_after_registration.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_retry_after_registration.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup post-registration retry verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_retry_after_registration.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup post-registration retry registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_retry_after_registration_alt.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_retry_after_registration_alt.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup alternate-initializer retry verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_retry_after_registration_alt.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup alternate-initializer retry registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_retry_after_registration_simple.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_retry_after_registration_simple.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup single-return retry verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_retry_after_registration_simple.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup single-return retry registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_dispatch.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup dispatch wrapper equivalence check failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup dispatch wrapper registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_initialize.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Full startup initializer equivalence check failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup initializer registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_exception.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_exception.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup exception equivalence check failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_exception.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup exception registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_predicate.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_predicate.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup predicate verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_predicate.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup predicate registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_service_wrapper.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_service_wrapper.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup service wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_service_wrapper.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup service wrapper registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_state_initialize.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_state_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup state initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_state_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup state initializer registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_event_poll.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_event_poll.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup event-poll verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_event_poll.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup event-poll registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_boot.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_boot.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup boot orchestrator equivalence check failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_boot.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup boot orchestrator registration failed' }
& (Join-Path $PSScriptRoot 'build_startup_callback_flag_reset_8c107aaa.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_callback_flag_reset_8c107aaa.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup callback flag-reset verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_callback_flag_reset_8c107aaa.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup callback flag-reset registration failed' }
& $clang -shared -O2 -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/startup_sections.c') -o $output
if ($LASTEXITCODE -ne 0) { throw 'Native startup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_startup.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup equivalence check failed' }
& $python (Join-Path $PSScriptRoot 'verify_sh4_macl_stack_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'MACL stack transfer interpreter check failed' }
& $clang -shared -O2 -Wall -Wextra -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/prs_decompress.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/prs_decompress.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native PRS compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_prs_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native PRS content verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_prs_instructions.py')
if ($LASTEXITCODE -ne 0) { throw 'PRS instruction comparison failed' }
& (Join-Path $PSScriptRoot 'build_callback_table_tail_8c0a9660.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_callback_table_tail_8c0a9660.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback-table tail thunk verification failed' }
& $python (Join-Path $PSScriptRoot 'register_callback_table_tail_8c0a9660.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback-table tail thunk registration failed' }
& (Join-Path $PSScriptRoot 'build_retail_empty_stubs.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_retail_empty_stubs.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail empty-stub verification failed' }
& $python (Join-Path $PSScriptRoot 'register_retail_empty_stubs.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail empty-stub registration failed' }
& (Join-Path $PSScriptRoot 'build_global_setter_wrappers.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_setter_wrappers.py')
if ($LASTEXITCODE -ne 0) { throw 'Global setter wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_setter_wrappers.py')
if ($LASTEXITCODE -ne 0) { throw 'Global setter wrapper registration failed' }
& (Join-Path $PSScriptRoot 'build_callback_tail_thunks.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_callback_tail_thunks.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback tail-thunk verification failed' }
& $python (Join-Path $PSScriptRoot 'register_callback_tail_thunks.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback tail-thunk registration failed' }
& (Join-Path $PSScriptRoot 'build_global_constant_setters.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_constant_setters.py')
if ($LASTEXITCODE -ne 0) { throw 'Global constant-setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_constant_setters.py')
if ($LASTEXITCODE -ne 0) { throw 'Global constant-setter registration failed' }
& (Join-Path $PSScriptRoot 'build_sa_initialize_object_flags_8c0e65be.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Object field initializer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sa_initialize_object_flags_8c0e65be.py')
if ($LASTEXITCODE -ne 0) { throw 'Object field initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_initialize_object_flags_8c0e65be.py')
if ($LASTEXITCODE -ne 0) { throw 'Object field initializer registration failed' }
& (Join-Path $PSScriptRoot 'build_sa_copy_object_field_when_active_8c5ffcfe.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Conditional object helper build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sa_copy_object_field_when_active_8c5ffcfe.py')
if ($LASTEXITCODE -ne 0) { throw 'Conditional object helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_copy_object_field_when_active_8c5ffcfe.py')
if ($LASTEXITCODE -ne 0) { throw 'Conditional object helper registration failed' }
& (Join-Path $PSScriptRoot 'build_sa_clear_global_pointer_set_8c08a05c.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Global pointer-set clear build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sa_clear_global_pointer_set_8c08a05c.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pointer-set clear verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_clear_global_pointer_set_8c08a05c.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pointer-set clear registration failed' }
& (Join-Path $PSScriptRoot 'build_sa_bcd_nibble_to_int_8c64b3b4.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Packed decimal conversion build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sa_bcd_nibble_to_int_8c64b3b4.py')
if ($LASTEXITCODE -ne 0) { throw 'Packed decimal conversion verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_bcd_nibble_to_int_8c64b3b4.py')
if ($LASTEXITCODE -ne 0) { throw 'Packed decimal conversion registration failed' }
& (Join-Path $PSScriptRoot 'build_sa_character_table_uppercase_8c10f98c.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Character table helper build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sa_character_table_uppercase_8c10f98c.py')
if ($LASTEXITCODE -ne 0) { throw 'Character table helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_character_table_uppercase_8c10f98c.py')
if ($LASTEXITCODE -ne 0) { throw 'Character table helper registration failed' }
& (Join-Path $PSScriptRoot 'build_sa_write_u32_le_8c6089ec.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Little-endian word writer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sa_write_u32_le_8c6089ec.py')
if ($LASTEXITCODE -ne 0) { throw 'Little-endian word writer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_write_u32_le_8c6089ec.py')
if ($LASTEXITCODE -ne 0) { throw 'Little-endian word writer registration failed' }
& (Join-Path $PSScriptRoot 'build_sa_masked_global_pair_set_8c608c14.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Masked global pair setter build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sa_masked_global_pair_set_8c608c14.py')
if ($LASTEXITCODE -ne 0) { throw 'Masked global pair setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_masked_global_pair_set_8c608c14.py')
if ($LASTEXITCODE -ne 0) { throw 'Masked global pair setter registration failed' }
& (Join-Path $PSScriptRoot 'build_retail_constant_setters.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_retail_constant_setters.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail constant-setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_retail_constant_setters.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail constant-setter registration failed' }
& (Join-Path $PSScriptRoot 'build_retail_compact_state_setters.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_retail_compact_state_setters.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail compact-setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_retail_compact_state_setters.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail compact-setter registration failed' }
& (Join-Path $PSScriptRoot 'build_retail_global_state_wrappers.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_retail_global_state_wrappers.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail global-state wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_retail_global_state_wrappers.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail global-state wrapper registration failed' }
& (Join-Path $PSScriptRoot 'build_retail_byte_field_setter.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_retail_byte_field_setter.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail byte field-setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_retail_byte_field_setter.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail byte field-setter registration failed' }
& (Join-Path $PSScriptRoot 'build_indexed_field_setters.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_indexed_field_setters.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed field-setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_indexed_field_setters.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed field-setter registration failed' }
& (Join-Path $PSScriptRoot 'build_retail_global_indirect_getters.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_retail_global_indirect_getters.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail global indirect-getter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_retail_global_indirect_getters.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail global indirect-getter registration failed' }
& (Join-Path $PSScriptRoot 'build_retail_return_delay_stubs.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_retail_return_delay_stubs.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail return-delay stub verification failed' }
& $python (Join-Path $PSScriptRoot 'register_retail_return_delay_stubs.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail return-delay stub registration failed' }
& (Join-Path $PSScriptRoot 'build_retail_branch_thunks.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_retail_branch_thunks.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail branch-thunk verification failed' }
& $python (Join-Path $PSScriptRoot 'register_retail_branch_thunks.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail branch-thunk registration failed' }
& (Join-Path $PSScriptRoot 'build_retail_literal_return_getters.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_retail_literal_return_getters.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail literal-return getter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_retail_literal_return_getters.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail literal-return getter registration failed' }
& (Join-Path $PSScriptRoot 'build_input_bitset_helpers.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_input_bitset_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'Input bitset helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_input_bitset_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'Input bitset helper registration failed' }
& (Join-Path $PSScriptRoot 'build_input_mapping_value.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_input_mapping_value.py')
if ($LASTEXITCODE -ne 0) { throw 'Input mapping-value verification failed' }
& $python (Join-Path $PSScriptRoot 'register_input_mapping_value.py')
if ($LASTEXITCODE -ne 0) { throw 'Input mapping-value registration failed' }
& (Join-Path $PSScriptRoot 'build_input_bitset_set.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_input_bitset_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Input bit setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_input_bitset_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Input bit setter registration failed' }
& (Join-Path $PSScriptRoot 'build_input_mapping_query.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_input_mapping_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Input mapping-query verification failed' }
& $python (Join-Path $PSScriptRoot 'register_input_mapping_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Input mapping-query registration failed' }
& (Join-Path $PSScriptRoot 'build_input_mapping_set.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_input_mapping_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Input mapping-set verification failed' }
& $python (Join-Path $PSScriptRoot 'register_input_mapping_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Input mapping-set registration failed' }
& (Join-Path $PSScriptRoot 'build_retail_scalar_helpers.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Retail scalar-helper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_retail_scalar_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail scalar-helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_retail_scalar_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail scalar-helper registration failed' }
& (Join-Path $PSScriptRoot 'build_signed_table_lookup.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Signed table lookup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_signed_table_lookup.py')
if ($LASTEXITCODE -ne 0) { throw 'Signed table lookup verification failed' }
& $python (Join-Path $PSScriptRoot 'register_signed_table_lookup.py')
if ($LASTEXITCODE -ne 0) { throw 'Signed table lookup registration failed' }
& (Join-Path $PSScriptRoot 'build_adv00_index_clamp.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'ADV00 index-clamp compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_adv00_index_clamp.py')
if ($LASTEXITCODE -ne 0) { throw 'ADV00 index-clamp verification failed' }
& $python (Join-Path $PSScriptRoot 'register_adv00_index_clamp.py')
if ($LASTEXITCODE -ne 0) { throw 'ADV00 index-clamp registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_field_copy_helpers.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 field-copy helper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_field_copy_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 field-copy helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_field_copy_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 field-copy helper registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_short_pair_offset.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 short-pair offset compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_short_pair_offset.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 short-pair offset verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_short_pair_offset.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 short-pair offset registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_field_refresh_helpers.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 field-refresh helper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_field_refresh_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 field-refresh helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_field_refresh_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 field-refresh helper registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_callback_0c9005e4.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 transition callback compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_callback_0c9005e4.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 transition callback verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_callback_0c9005e4.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 transition callback registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_dispatch_helper_0c900c14.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 dispatch-helper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_dispatch_helper_0c900c14.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 dispatch-helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_dispatch_helper_0c900c14.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 dispatch-helper registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_task_callback_0c90116c.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 task-callback compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_task_callback_0c90116c.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 task-callback verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_task_callback_0c90116c.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 task-callback registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_task_callback_0c901192.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 0x1234 task-callback compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_task_callback_0c901192.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 0x1234 task-callback verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_task_callback_0c901192.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 0x1234 task-callback registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_table_row_copy_0c906248.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 table-row helper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_table_row_copy_0c906248.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 table-row helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_table_row_copy_0c906248.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 table-row helper registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_clear_tikal_flag_0c903c4e.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 Tikal flag-helper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_clear_tikal_flag_0c903c4e.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 Tikal flag-helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_clear_tikal_flag_0c903c4e.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 Tikal flag-helper registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_tikal_flag_and_byte_cleanup_0c903c34.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 Tikal wrapper/helper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_tikal_flag_and_byte_cleanup_0c903c34.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 Tikal wrapper/helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_tikal_flag_and_byte_cleanup_0c903c34.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 Tikal wrapper/helper registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_tikal_status_and_byte_cleanup_0c903c0c.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 Tikal status/two-leaf compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_tikal_status_and_byte_cleanup_0c903c0c.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 Tikal status/two-leaf verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_tikal_status_and_byte_cleanup_0c903c0c.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 Tikal status/two-leaf registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_tikal_compare_counter_0c903be0.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 Tikal compare-counter compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_tikal_compare_counter_0c903be0.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 Tikal compare-counter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_tikal_compare_counter_0c903be0.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 Tikal compare-counter registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_tikal_compare_logic_0c903a7c.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 Tikal comparison-logic compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_tikal_compare_logic_0c903a7c.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 Tikal comparison-logic verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_tikal_compare_logic_0c903a7c.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 Tikal comparison-logic registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_float_global_return_0c91dca8.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 global FPU return helper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_float_global_return_0c91dca8.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 global FPU return helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_float_global_return_0c91dca8.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 global FPU return helper registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_clamped_global_value_update_0c91dcae.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 clamped global-value helper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_clamped_global_value_update_0c91dcae.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 clamped global-value helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_clamped_global_value_update_0c91dcae.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 clamped global-value helper registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_event_cleanup_dispatch_0c91dd74.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 event-cleanup dispatch compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_event_cleanup_dispatch_0c91dd74.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 event-cleanup dispatch verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_event_cleanup_dispatch_0c91dd74.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 event-cleanup dispatch registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_callback_initializer_0c90277c.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 sibling callback-initializer compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_callback_initializer_0c90277c.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 sibling callback-initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_callback_initializer_0c90277c.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 sibling callback-initializer registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_callback_initializer_0c9027c4.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 callback-initializer compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_callback_initializer_0c9027c4.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 callback-initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_callback_initializer_0c9027c4.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 callback-initializer registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_callback_initializer_flag_0c902860.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 callback initializer/flag wrapper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_callback_initializer_flag_0c902860.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 callback initializer/flag wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_callback_initializer_flag_0c902860.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 callback initializer/flag wrapper registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_callback_initializer_flag_0c90287e.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 alternate callback initializer/flag wrapper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_callback_initializer_flag_0c90287e.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 alternate callback initializer/flag wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_callback_initializer_flag_0c90287e.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 alternate callback initializer/flag wrapper registration failed' }

& (Join-Path $PSScriptRoot 'build_external_overlay_tail_thunks.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'External overlay tail-thunk compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_external_overlay_tail_thunks.py')
if ($LASTEXITCODE -ne 0) { throw 'External overlay tail-thunk verification failed' }
& $python (Join-Path $PSScriptRoot 'register_external_overlay_tail_thunks.py')
if ($LASTEXITCODE -ne 0) { throw 'External overlay tail-thunk registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_leaf_0c901d20.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 leaf compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_leaf_0c901d20.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 leaf verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_leaf_0c901d20.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 leaf registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_cleanup_callback_0c901d2a.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 cleanup-callback compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_cleanup_callback_0c901d2a.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 cleanup-callback verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_cleanup_callback_0c901d2a.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 cleanup-callback registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_callback_setup_0c901c98.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 callback-setup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_callback_setup_0c901c98.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 callback-setup verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_callback_setup_0c901c98.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 callback-setup registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_callback_0c9005ba.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 callback compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_callback_0c9005ba.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 callback verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_callback_0c9005ba.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 callback registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos4_flag_propagation.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 flag-propagation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos4_flag_propagation.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 flag-propagation verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos4_flag_propagation.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 flag-propagation registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos4_template_initialize.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 template-initializer compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos4_template_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 template-initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos4_template_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 template-initializer registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos4_global_store.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 global-store compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos4_global_store.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 global-store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos4_global_store.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 global-store registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos4_counters.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 counter-helper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos4_counters.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 counter-helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos4_counters.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 counter-helper registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos4_external_thunks.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 external-thunk compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos4_external_thunks.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 external-thunk verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos4_external_thunks.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 external-thunk registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos4_flag_table_callback.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 flag-table callback compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos4_flag_table_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 flag-table callback verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos4_flag_table_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 flag-table callback registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos4_list_callback_update.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 list-callback updater compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos4_list_callback_update.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 list-callback updater verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos4_list_callback_update.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 list-callback updater registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos4_template_copy.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 template copier compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos4_template_copy.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 template copier verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos4_template_copy.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 template copier registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos4_callback_candidate.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 callback candidate compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos4_callback_candidate.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 callback candidate verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos4_callback_candidate.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 callback candidate registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos4_state_limit.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 state-limit compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos4_state_limit.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 state-limit verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos4_state_limit.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS4 state-limit registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos2_write_state_bytes.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS2 state-byte writer compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos2_write_state_bytes.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS2 state-byte writer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos2_write_state_bytes.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS2 state-byte writer registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos0_set_state_bit.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS0 state-bit routine compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos0_set_state_bit.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS0 state-bit routine verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos0_set_state_bit.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS0 state-bit routine registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos0_state_word_mode.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS0 state-word mode compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos0_state_word_mode.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS0 state-word mode verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos0_state_word_mode.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS0 state-word mode registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos0_fill_global_bytes.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS0 global-byte initializer compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos0_fill_global_bytes.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS0 global-byte initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos0_fill_global_bytes.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS0 global-byte initializer registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos6_clear_global_byte.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 global-byte clear compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos6_clear_global_byte.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 global-byte clear verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos6_clear_global_byte.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 global-byte clear registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos6_classify_global_byte.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 global-byte classifier compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos6_classify_global_byte.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 global-byte classifier verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos6_classify_global_byte.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 global-byte classifier registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos6_sync_global_bytes.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 global-byte synchronization compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos6_sync_global_bytes.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 global-byte synchronization verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos6_sync_global_bytes.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 global-byte synchronization registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos6_countdown_accumulator.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 countdown accumulator compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos6_countdown_accumulator.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 countdown accumulator verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos6_countdown_accumulator.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 countdown accumulator registration failed' }
& (Join-Path $PSScriptRoot 'build_b_chaos6_state_and_vector_clear.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 state/vector-clear compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_chaos6_state_and_vector_clear.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 state/vector-clear verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_chaos6_state_and_vector_clear.py')
if ($LASTEXITCODE -ne 0) { throw 'B_CHAOS6 state/vector-clear registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_clear_flagged_byte.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 flagged-byte clear compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_clear_flagged_byte.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 flagged-byte clear verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_clear_flagged_byte.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 flagged-byte clear registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_clear_shifted_flagged_byte.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'STG09 shifted flagged-byte clear compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stg09_clear_shifted_flagged_byte.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 shifted flagged-byte clear verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_clear_shifted_flagged_byte.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 shifted flagged-byte clear registration failed' }
& (Join-Path $PSScriptRoot 'build_b_egm2_clear_global_state.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'B_EGM2 global-state clear compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_b_egm2_clear_global_state.py')
if ($LASTEXITCODE -ne 0) { throw 'B_EGM2 global-state clear verification failed' }
& $python (Join-Path $PSScriptRoot 'register_b_egm2_clear_global_state.py')
if ($LASTEXITCODE -ne 0) { throw 'B_EGM2 global-state clear registration failed' }
& (Join-Path $PSScriptRoot 'build_al_main_set_callback_fields.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'AL_MAIN callback-field setter compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_al_main_set_callback_fields.py')
if ($LASTEXITCODE -ne 0) { throw 'AL_MAIN callback-field setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_al_main_set_callback_fields.py')
if ($LASTEXITCODE -ne 0) { throw 'AL_MAIN callback-field setter registration failed' }
& (Join-Path $PSScriptRoot 'build_al_main_set_secondary_callback_fields.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'AL_MAIN secondary callback-field setter compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_al_main_set_secondary_callback_fields.py')
if ($LASTEXITCODE -ne 0) { throw 'AL_MAIN secondary callback-field setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_al_main_set_secondary_callback_fields.py')
if ($LASTEXITCODE -ne 0) { throw 'AL_MAIN secondary callback-field setter registration failed' }
& (Join-Path $PSScriptRoot 'build_al_main_reset_callback_work.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'AL_MAIN callback-work reset compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_al_main_reset_callback_work.py')
if ($LASTEXITCODE -ne 0) { throw 'AL_MAIN callback-work reset verification failed' }
& $python (Join-Path $PSScriptRoot 'register_al_main_reset_callback_work.py')
if ($LASTEXITCODE -ne 0) { throw 'AL_MAIN callback-work reset registration failed' }
& (Join-Path $PSScriptRoot 'build_al_main_copy_vector_fields.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'AL_MAIN vector-field copy compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_al_main_copy_vector_fields.py')
if ($LASTEXITCODE -ne 0) { throw 'AL_MAIN vector-field copy verification failed' }
& $python (Join-Path $PSScriptRoot 'register_al_main_copy_vector_fields.py')
if ($LASTEXITCODE -ne 0) { throw 'AL_MAIN vector-field copy registration failed' }
& (Join-Path $PSScriptRoot 'build_minicart_get_status_bytes.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'MINICART status-byte getter compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_minicart_get_status_bytes.py')
if ($LASTEXITCODE -ne 0) { throw 'MINICART status-byte getter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_minicart_get_status_bytes.py')
if ($LASTEXITCODE -ne 0) { throw 'MINICART status-byte getter registration failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/module_loader.c') (Join-Path $PSScriptRoot 'source/prs_decompress.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/module_loader.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native module loader compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_module_loader.py')
if ($LASTEXITCODE -ne 0) { throw 'Native module loader verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/level_variant.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/level_variant.dll')
if ($LASTEXITCODE -ne 0) { throw 'Packed-field getter compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_level_variant.py')
if ($LASTEXITCODE -ne 0) { throw 'Packed-field getter comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/stage00_task.c') (Join-Path $PSScriptRoot 'source/level_variant.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/stage00_task.dll')
if ($LASTEXITCODE -ne 0) { throw 'STG00 task compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stage00_task.py')
if ($LASTEXITCODE -ne 0) { throw 'STG00 task comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_stage00_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected STG00 task comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/stage00_initialize.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/stage00_initialize.dll')
if ($LASTEXITCODE -ne 0) { throw 'STG00 initializer compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stage00_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'STG00 initializer comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/stage00_global_initialize.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/stage00_global_initialize.dll')
if ($LASTEXITCODE -ne 0) { throw 'STG00 global initializer compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stage00_global_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'STG00 global initializer comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/main_ready_flag.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/main_ready_flag.dll')
if ($LASTEXITCODE -ne 0) { throw 'Main ready-flag routine compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_ready_flag.py')
if ($LASTEXITCODE -ne 0) { throw 'Main ready-flag retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_main_ready_flag.py')
if ($LASTEXITCODE -ne 0) { throw 'Main ready-flag registry update failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/stage00_timer.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/stage00_timer.dll')
if ($LASTEXITCODE -ne 0) { throw 'STG00 timer compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stage00_timer.py')
if ($LASTEXITCODE -ne 0) { throw 'STG00 timer comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/stage_work_flags.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/stage_work_flags.dll')
if ($LASTEXITCODE -ne 0) { throw 'Stage work flag compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_stage_work_flags.py')
if ($LASTEXITCODE -ne 0) { throw 'Stage work flag comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/task_create.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/task_create.dll')
if ($LASTEXITCODE -ne 0) { throw 'Task creation wrapper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_create.py')
if ($LASTEXITCODE -ne 0) { throw 'Task creation wrapper comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/task_lists.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/task_lists.dll')
if ($LASTEXITCODE -ne 0) { throw 'Base task list compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_lists.py')
if ($LASTEXITCODE -ne 0) { throw 'Base task list comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/task_allocate.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/task_allocate.dll')
if ($LASTEXITCODE -ne 0) { throw 'Task allocation zero-fill compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_allocate.py')
if ($LASTEXITCODE -ne 0) { throw 'Task allocation zero-fill comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/task_heap.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/task_heap.dll')
if ($LASTEXITCODE -ne 0) { throw 'Task heap allocation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_heap.py')
if ($LASTEXITCODE -ne 0) { throw 'Task heap allocation comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_heap_free.py')
if ($LASTEXITCODE -ne 0) { throw 'Task heap release comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_heap_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected task heap lifecycle comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/task_heap.c') (Join-Path $PSScriptRoot 'source/task_allocate.c') (Join-Path $PSScriptRoot 'source/task_heap_adapter.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/task_heap_connected.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected task heap compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_heap_lifecycle.py') --zero-fill
if ($LASTEXITCODE -ne 0) { throw 'Connected zero-fill heap comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/task_work_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/task_work_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Task work release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_work_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Task work release comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/work_resource_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/work_resource_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Work resource release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_work_resource_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Work resource release comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/secondary_resource_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/secondary_resource_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Secondary resource release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_secondary_resource_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Secondary resource release comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/fpu_cleanup_transfer.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/fpu_cleanup_transfer.dll')
if ($LASTEXITCODE -ne 0) { throw 'FPU cleanup transfer support compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_fpu_cleanup_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'FPU cleanup transfer vectors failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/resource_list_release.c') (Join-Path $PSScriptRoot 'source/fpu_cleanup_transfer.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/resource_list_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Resource list release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_resource_list_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Resource list release comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_resource_list_release.py') --primary
if ($LASTEXITCODE -ne 0) { throw 'Primary resource list cleanup comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_resource_list_release.py') --combined
if ($LASTEXITCODE -ne 0) { throw 'Connected secondary resource cleanup comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/task_dispose.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/task_dispose.dll')
if ($LASTEXITCODE -ne 0) { throw 'Task disposal compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_dispose.py')
if ($LASTEXITCODE -ne 0) { throw 'Task disposal comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/task_children.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/task_children.dll')
if ($LASTEXITCODE -ne 0) { throw 'Child task cleanup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_children.py')
if ($LASTEXITCODE -ne 0) { throw 'Child task cleanup comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_process_children.py')
if ($LASTEXITCODE -ne 0) { throw 'Child callback traversal comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/task_runtime.c') (Join-Path $PSScriptRoot 'source/task_dispose.c') (Join-Path $PSScriptRoot 'source/task_children.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/task_runtime.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected task runtime compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_runtime.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected task runtime comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/task_scheduler.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/task_scheduler.dll')
if ($LASTEXITCODE -ne 0) { throw 'Primary scheduler compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_scheduler.py')
if ($LASTEXITCODE -ne 0) { throw 'Primary scheduler comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_secondary_scheduler.py')
if ($LASTEXITCODE -ne 0) { throw 'Secondary scheduler comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/frame_countdown.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/frame_countdown.dll')
if ($LASTEXITCODE -ne 0) { throw 'Countdown configuration compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_frame_countdown.py')
if ($LASTEXITCODE -ne 0) { throw 'Countdown configuration comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_frame_tick.py')
if ($LASTEXITCODE -ne 0) { throw 'Countdown tick comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/frame_countdown.c') (Join-Path $PSScriptRoot 'source/frame_runtime.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/frame_runtime.dll')
if ($LASTEXITCODE -ne 0) { throw 'Frame runtime compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_frame_registration.py')
if ($LASTEXITCODE -ne 0) { throw 'Frame registration chain comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_frame_tick.py') --runtime
if ($LASTEXITCODE -ne 0) { throw 'Frame runtime tick comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/vblank_dispatch.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/vblank_dispatch.dll')
if ($LASTEXITCODE -ne 0) { throw 'Event chain dispatcher compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_vblank_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Event chain dispatcher comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/callback_install.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/callback_install.dll')
if ($LASTEXITCODE -ne 0) { throw 'Callback table installation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_callback_install.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback table installation comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_callback_install.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected callback installation comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_callback_register_primary.py')
if ($LASTEXITCODE -ne 0) { throw 'Primary callback wrapper comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_callback_primary.py')
if ($LASTEXITCODE -ne 0) { throw 'Primary callback wrapper registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_callback_register_index_wrappers.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed callback wrapper comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_callback_index_wrappers.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed callback wrapper registration failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/callback_configuration.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/callback_configuration.dll')
if ($LASTEXITCODE -ne 0) { throw 'Callback configuration compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_callback_configuration.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback configuration comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_callback_configuration.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback configuration registration failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/object_accessors.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/object_accessors.dll')
if ($LASTEXITCODE -ne 0) { throw 'Object accessor compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_object_accessors.py')
if ($LASTEXITCODE -ne 0) { throw 'Object accessor comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_object_accessors.py')
if ($LASTEXITCODE -ne 0) { throw 'Object accessor registration failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/record_accessors.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/record_accessors.dll')
if ($LASTEXITCODE -ne 0) { throw 'Record accessor compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_record_accessors.py')
if ($LASTEXITCODE -ne 0) { throw 'Record accessor comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_record_accessors.py')
if ($LASTEXITCODE -ne 0) { throw 'Record accessor registration failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/record_pair_accessor.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/record_pair_accessor.dll')
if ($LASTEXITCODE -ne 0) { throw 'Record pair accessor compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_record_pair_accessor.py')
if ($LASTEXITCODE -ne 0) { throw 'Record pair accessor comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_record_pair_accessor.py')
if ($LASTEXITCODE -ne 0) { throw 'Record pair accessor registration failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/record_pair_result.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/record_pair_result.dll')
if ($LASTEXITCODE -ne 0) { throw 'Record pair result compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_record_pair_result.py')
if ($LASTEXITCODE -ne 0) { throw 'Record pair result comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_record_pair_result.py')
if ($LASTEXITCODE -ne 0) { throw 'Record pair result registration failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/global_state_pair_set.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/global_state_pair_set.dll')
if ($LASTEXITCODE -ne 0) { throw 'Global-state pair setter compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_global_state_pair_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Global-state pair setter comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_global_state_pair_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Global-state pair setter registration failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/record_value_accessors.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/record_value_accessors.dll')
if ($LASTEXITCODE -ne 0) { throw 'Record value accessor compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_record_value_accessors.py')
if ($LASTEXITCODE -ne 0) { throw 'Record value accessor comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_record_value_accessors.py')
if ($LASTEXITCODE -ne 0) { throw 'Record value accessor registration failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/mask_input.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/mask_input.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input mask helper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_mask_input.py')
if ($LASTEXITCODE -ne 0) { throw 'Input mask helper comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_mask_input.py')
if ($LASTEXITCODE -ne 0) { throw 'Input mask helper registration failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/byte_copy_forward.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/byte_copy_forward.dll')
if ($LASTEXITCODE -ne 0) { throw 'Forward byte-copy compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_byte_copy_forward.py')
if ($LASTEXITCODE -ne 0) { throw 'Forward byte-copy comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_byte_copy_forward.py')
if ($LASTEXITCODE -ne 0) { throw 'Forward byte-copy registration failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/word_copy_forward.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/word_copy_forward.dll')
if ($LASTEXITCODE -ne 0) { throw 'Forward word-copy compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_word_copy_forward.py')
if ($LASTEXITCODE -ne 0) { throw 'Forward word-copy comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_word_copy_forward.py')
if ($LASTEXITCODE -ne 0) { throw 'Forward word-copy registration failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/byte_fill_backward.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/byte_fill_backward.dll')
if ($LASTEXITCODE -ne 0) { throw 'Backward byte-fill compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_byte_fill_backward.py')
if ($LASTEXITCODE -ne 0) { throw 'Backward byte-fill comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_byte_fill_backward.py')
if ($LASTEXITCODE -ne 0) { throw 'Backward byte-fill registration failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/memory_positive_loops.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/memory_positive_loops.dll')
if ($LASTEXITCODE -ne 0) { throw 'Positive memory loop compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_memory_positive_loops.py')
if ($LASTEXITCODE -ne 0) { throw 'Positive memory loop comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_memory_positive_loops.py')
if ($LASTEXITCODE -ne 0) { throw 'Positive memory loop registration failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/zero_prefix_bits.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/zero_prefix_bits.dll')
if ($LASTEXITCODE -ne 0) { throw 'Zero-prefix bit scanner compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_zero_prefix_bits.py')
if ($LASTEXITCODE -ne 0) { throw 'Zero-prefix bit scanner comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_zero_prefix_bits.py')

& (Join-Path $PSScriptRoot 'build_bitstream_pad.ps1')
& $python (Join-Path $PSScriptRoot 'verify_bitstream_pad.py')
if ($LASTEXITCODE -ne 0) { throw 'Bitstream padding helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_bitstream_pad.py')
if ($LASTEXITCODE -ne 0) { throw 'Bitstream padding helper registration failed' }

& (Join-Path $PSScriptRoot 'build_packet_checksum.ps1')
& $python (Join-Path $PSScriptRoot 'verify_packet_checksum.py')
if ($LASTEXITCODE -ne 0) { throw 'Packet checksum verification failed' }
& $python (Join-Path $PSScriptRoot 'register_packet_checksum.py')
if ($LASTEXITCODE -ne 0) { throw 'Packet checksum registration failed' }

& (Join-Path $PSScriptRoot 'build_packet_requests.ps1')
& $python (Join-Path $PSScriptRoot 'verify_packet_requests.py')
if ($LASTEXITCODE -ne 0) { throw 'Packet request verification failed' }
& $python (Join-Path $PSScriptRoot 'register_packet_requests.py')
if ($LASTEXITCODE -ne 0) { throw 'Packet request registration failed' }

& (Join-Path $PSScriptRoot 'build_packet_global_setter.ps1')
& $python (Join-Path $PSScriptRoot 'verify_packet_global_setter.py')
if ($LASTEXITCODE -ne 0) { throw 'Packet global setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_packet_global_setter.py')
if ($LASTEXITCODE -ne 0) { throw 'Packet global setter registration failed' }

& (Join-Path $PSScriptRoot 'build_packet_mmio.ps1')
& $python (Join-Path $PSScriptRoot 'verify_packet_mmio.py')
if ($LASTEXITCODE -ne 0) { throw 'Packet MMIO verification failed' }
& $python (Join-Path $PSScriptRoot 'register_packet_mmio.py')
if ($LASTEXITCODE -ne 0) { throw 'Packet MMIO registration failed' }

& (Join-Path $PSScriptRoot 'build_string_helpers.ps1')
& $python (Join-Path $PSScriptRoot 'verify_string_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'String helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_string_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'String helper registration failed' }

& (Join-Path $PSScriptRoot 'build_directory_entries.ps1')
& $python (Join-Path $PSScriptRoot 'verify_directory_entries.py')
if ($LASTEXITCODE -ne 0) { throw 'Directory entry verification failed' }
& $python (Join-Path $PSScriptRoot 'register_directory_entries.py')
if ($LASTEXITCODE -ne 0) { throw 'Directory entry registration failed' }

& (Join-Path $PSScriptRoot 'build_directory_lookup.ps1')
& $python (Join-Path $PSScriptRoot 'verify_directory_lookup.py')
if ($LASTEXITCODE -ne 0) { throw 'Directory lookup verification failed' }
& $python (Join-Path $PSScriptRoot 'register_directory_lookup.py')
if ($LASTEXITCODE -ne 0) { throw 'Directory lookup registration failed' }

& (Join-Path $PSScriptRoot 'build_directory_slot_init.ps1')
& $python (Join-Path $PSScriptRoot 'verify_directory_slot_init.py')
if ($LASTEXITCODE -ne 0) { throw 'Directory slot initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_directory_slot_init.py')
if ($LASTEXITCODE -ne 0) { throw 'Directory slot initializer registration failed' }

& (Join-Path $PSScriptRoot 'build_directory_callbacks.ps1')
& $python (Join-Path $PSScriptRoot 'verify_directory_callbacks.py')
if ($LASTEXITCODE -ne 0) { throw 'Directory callback dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'register_directory_callbacks.py')
if ($LASTEXITCODE -ne 0) { throw 'Directory callback dispatcher registration failed' }

& (Join-Path $PSScriptRoot 'build_object_table_compact.ps1')
& $python (Join-Path $PSScriptRoot 'verify_object_table_compact.py')
if ($LASTEXITCODE -ne 0) { throw 'Object table compaction verification failed' }
& $python (Join-Path $PSScriptRoot 'register_object_table_compact.py')
if ($LASTEXITCODE -ne 0) { throw 'Object table compaction registration failed' }
& (Join-Path $PSScriptRoot 'build_object_table_stride.ps1')
& $python (Join-Path $PSScriptRoot 'verify_object_table_stride.py')
if ($LASTEXITCODE -ne 0) { throw 'Object table stride helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_object_table_stride.py')
if ($LASTEXITCODE -ne 0) { throw 'Object table stride helper registration failed' }
if ($LASTEXITCODE -ne 0) { throw 'Zero-prefix bit scanner registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_driver_registration.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver callback registration comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_driver_registration.py') --secondary
if ($LASTEXITCODE -ne 0) { throw 'Secondary driver callback registration comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/event_registry.c') (Join-Path $PSScriptRoot 'source/event_registry_runtime.c') (Join-Path $PSScriptRoot 'source/callback_install.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/event_registry.dll')
if ($LASTEXITCODE -ne 0) { throw 'Event registry insertion compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_event_registry.py')
if ($LASTEXITCODE -ne 0) { throw 'Event registry insertion comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_event_registry.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected event registry comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_event_remove.py')
if ($LASTEXITCODE -ne 0) { throw 'Event registry removal comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_event_remove.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected event registry removal comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/countdown_registry_runtime.c') (Join-Path $PSScriptRoot 'source/frame_countdown.c') (Join-Path $PSScriptRoot 'source/event_registry.c') (Join-Path $PSScriptRoot 'source/event_registry_runtime.c') (Join-Path $PSScriptRoot 'source/callback_install.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/countdown_registry_runtime.dll')
if ($LASTEXITCODE -ne 0) { throw 'Countdown registry connection compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_countdown_registry.py')
if ($LASTEXITCODE -ne 0) { throw 'Countdown registry replacement sequence comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/driver_dispatch.c') (Join-Path $PSScriptRoot 'source/callback_install.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/driver_dispatch.dll')
if ($LASTEXITCODE -ne 0) { throw 'Indexed driver callback dispatch compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_driver_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed driver callback dispatch comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_driver_dispatch.py') --installed
if ($LASTEXITCODE -ne 0) { throw 'Installed driver callback dispatch comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/driver_event_runtime.c') (Join-Path $PSScriptRoot 'source/driver_dispatch.c') (Join-Path $PSScriptRoot 'source/vblank_dispatch.c') (Join-Path $PSScriptRoot 'source/event_registry.c') (Join-Path $PSScriptRoot 'source/event_registry_runtime.c') (Join-Path $PSScriptRoot 'source/callback_install.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/driver_event_runtime.dll')
if ($LASTEXITCODE -ne 0) { throw 'Driver event chain connection compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_driver_event_runtime.py')
if ($LASTEXITCODE -ne 0) { throw 'Installed driver event chain comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/tikal_cleanup.c') (Join-Path $PSScriptRoot 'source/tikal_cleanup_runtime.c') (Join-Path $PSScriptRoot 'source/tikal_cleanup_full_runtime.c') (Join-Path $PSScriptRoot 'source/descriptor_release.c') (Join-Path $PSScriptRoot 'source/entity_allocation_release.c') (Join-Path $PSScriptRoot 'source/owner_cleanup.c') (Join-Path $PSScriptRoot 'source/owner_table_release.c') (Join-Path $PSScriptRoot 'source/table_reference_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_cleanup.dll')
if ($LASTEXITCODE -ne 0) { throw 'Tikal cleanup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_cleanup.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal cleanup comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_cleanup.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal cleanup comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_cleanup_full.py')
if ($LASTEXITCODE -ne 0) { throw 'Full Tikal cleanup chain comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_table_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_table_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Owner table release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_table_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Owner table release comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/table_reference_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/table_reference_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Reference table release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_table_reference_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Reference table release comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_cleanup.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_cleanup.dll')
if ($LASTEXITCODE -ne 0) { throw 'Owner cleanup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_cleanup.py')
if ($LASTEXITCODE -ne 0) { throw 'Owner cleanup comparison failed' }
& $python (Join-Path $PSScriptRoot 'audit_recovery_coverage.py')
if ($LASTEXITCODE -ne 0) { throw 'Recovery registry evidence audit failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/descriptor_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/descriptor_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Descriptor release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_descriptor_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Descriptor release comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/entity_allocation_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/entity_allocation_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Entity allocation release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_entity_allocation_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Entity allocation release comparison failed' }

& $python (Join-Path $PSScriptRoot 'verify_descriptor_item_evidence.py')
if ($LASTEXITCODE -ne 0) { throw 'Descriptor item instruction evidence verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/resource_type_classifier.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/resource_type_classifier.dll')
if ($LASTEXITCODE -ne 0) { throw 'Resource type classifier compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_resource_type_classifier.py')
if ($LASTEXITCODE -ne 0) { throw 'Resource type classifier comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/resource_release.c') (Join-Path $PSScriptRoot 'source/resource_type_classifier.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/resource_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Resource release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_resource_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Resource release comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/resource_payload_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/resource_payload_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Resource payload release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_resource_payload_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Resource payload release comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/resource_release_runtime.c') (Join-Path $PSScriptRoot 'source/resource_release.c') (Join-Path $PSScriptRoot 'source/resource_payload_release.c') (Join-Path $PSScriptRoot 'source/resource_type_classifier.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/resource_release_runtime.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected resource release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_resource_release_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected resource release comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/descriptor_reset.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/descriptor_reset.dll')
if ($LASTEXITCODE -ne 0) { throw 'Descriptor reset compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_descriptor_reset.py')
if ($LASTEXITCODE -ne 0) { throw 'Descriptor reset comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/resource_reset_runtime.c') (Join-Path $PSScriptRoot 'source/resource_release_runtime.c') (Join-Path $PSScriptRoot 'source/resource_release.c') (Join-Path $PSScriptRoot 'source/resource_payload_release.c') (Join-Path $PSScriptRoot 'source/resource_type_classifier.c') (Join-Path $PSScriptRoot 'source/descriptor_reset.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/resource_reset_runtime.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected resource reset compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_resource_release_reset.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected resource reset comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/unsigned_division.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/unsigned_division.dll')
if ($LASTEXITCODE -ne 0) { throw 'Unsigned division compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_unsigned_division.py')
if ($LASTEXITCODE -ne 0) { throw 'Unsigned division comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/resource_division_runtime.c') (Join-Path $PSScriptRoot 'source/unsigned_division.c') (Join-Path $PSScriptRoot 'source/resource_reset_runtime.c') (Join-Path $PSScriptRoot 'source/resource_release_runtime.c') (Join-Path $PSScriptRoot 'source/resource_release.c') (Join-Path $PSScriptRoot 'source/resource_payload_release.c') (Join-Path $PSScriptRoot 'source/resource_type_classifier.c') (Join-Path $PSScriptRoot 'source/descriptor_reset.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/resource_division_runtime.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected resource division compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_resource_release_division.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected resource division comparison failed' }

& $python (Join-Path $PSScriptRoot 'verify_payload_release_evidence.py')
if ($LASTEXITCODE -ne 0) { throw 'Payload release dependency instruction evidence verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/block_unlink.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/block_unlink.dll')
if ($LASTEXITCODE -ne 0) { throw 'Block unlink compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_block_unlink.py')
if ($LASTEXITCODE -ne 0) { throw 'Block unlink comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/block_pool_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/block_pool_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Block pool release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_block_pool_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Block pool release comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/payload_metadata.c') (Join-Path $PSScriptRoot 'source/unsigned_division.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/payload_metadata.dll')
if ($LASTEXITCODE -ne 0) { throw 'Payload metadata compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_payload_metadata.py')
if ($LASTEXITCODE -ne 0) { throw 'Payload metadata comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/allocation_lookup.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/allocation_lookup.dll')
if ($LASTEXITCODE -ne 0) { throw 'Allocation lookup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_allocation_lookup.py')
if ($LASTEXITCODE -ne 0) { throw 'Allocation lookup comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/allocation_list_insert.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/allocation_list_insert.dll')
if ($LASTEXITCODE -ne 0) { throw 'Allocation list insertion compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_allocation_list_insert.py')
if ($LASTEXITCODE -ne 0) { throw 'Allocation list insertion comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/allocation_list_remove.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/allocation_list_remove.dll')
if ($LASTEXITCODE -ne 0) { throw 'Allocation list removal compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_allocation_list_remove.py')
if ($LASTEXITCODE -ne 0) { throw 'Allocation list removal comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/allocation_pool_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/allocation_pool_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Allocation pool release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_allocation_pool_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Allocation pool release comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/allocation_release.c') (Join-Path $PSScriptRoot 'source/allocation_list_remove.c') (Join-Path $PSScriptRoot 'source/allocation_list_insert.c') (Join-Path $PSScriptRoot 'source/allocation_pool_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/allocation_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Allocation coalescing compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_allocation_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Allocation coalescing comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/paired_allocation_release.c') (Join-Path $PSScriptRoot 'source/allocation_lookup.c') (Join-Path $PSScriptRoot 'source/allocation_release.c') (Join-Path $PSScriptRoot 'source/allocation_list_remove.c') (Join-Path $PSScriptRoot 'source/allocation_list_insert.c') (Join-Path $PSScriptRoot 'source/allocation_pool_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/paired_allocation_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Paired allocation release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_paired_allocation_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Paired allocation release comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/linked_payload_release.c') (Join-Path $PSScriptRoot 'source/payload_metadata.c') (Join-Path $PSScriptRoot 'source/unsigned_division.c') (Join-Path $PSScriptRoot 'source/paired_allocation_release.c') (Join-Path $PSScriptRoot 'source/allocation_lookup.c') (Join-Path $PSScriptRoot 'source/allocation_release.c') (Join-Path $PSScriptRoot 'source/allocation_list_remove.c') (Join-Path $PSScriptRoot 'source/allocation_list_insert.c') (Join-Path $PSScriptRoot 'source/allocation_pool_release.c') (Join-Path $PSScriptRoot 'source/block_unlink.c') (Join-Path $PSScriptRoot 'source/block_pool_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/linked_payload_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Linked payload cleanup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_linked_payload_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Linked payload cleanup comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/payload_release_runtime.c') (Join-Path $PSScriptRoot 'source/resource_payload_release.c') (Join-Path $PSScriptRoot 'source/linked_payload_release.c') (Join-Path $PSScriptRoot 'source/payload_metadata.c') (Join-Path $PSScriptRoot 'source/unsigned_division.c') (Join-Path $PSScriptRoot 'source/paired_allocation_release.c') (Join-Path $PSScriptRoot 'source/allocation_lookup.c') (Join-Path $PSScriptRoot 'source/allocation_release.c') (Join-Path $PSScriptRoot 'source/allocation_list_remove.c') (Join-Path $PSScriptRoot 'source/allocation_list_insert.c') (Join-Path $PSScriptRoot 'source/allocation_pool_release.c') (Join-Path $PSScriptRoot 'source/block_unlink.c') (Join-Path $PSScriptRoot 'source/block_pool_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/payload_release_runtime.dll')
if ($LASTEXITCODE -ne 0) { throw 'Payload dispatcher connection compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_payload_release_special.py')
if ($LASTEXITCODE -ne 0) { throw 'Special payload path comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_payload_release_normal.py')
if ($LASTEXITCODE -ne 0) { throw 'Normal payload path comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/resource_release_complete_runtime.c') (Join-Path $PSScriptRoot 'source/resource_division_runtime.c') (Join-Path $PSScriptRoot 'source/resource_reset_runtime.c') (Join-Path $PSScriptRoot 'source/resource_release_runtime.c') (Join-Path $PSScriptRoot 'source/resource_release.c') (Join-Path $PSScriptRoot 'source/resource_type_classifier.c') (Join-Path $PSScriptRoot 'source/descriptor_reset.c') (Join-Path $PSScriptRoot 'source/resource_payload_release.c') (Join-Path $PSScriptRoot 'source/linked_payload_release.c') (Join-Path $PSScriptRoot 'source/payload_metadata.c') (Join-Path $PSScriptRoot 'source/unsigned_division.c') (Join-Path $PSScriptRoot 'source/paired_allocation_release.c') (Join-Path $PSScriptRoot 'source/allocation_lookup.c') (Join-Path $PSScriptRoot 'source/allocation_release.c') (Join-Path $PSScriptRoot 'source/allocation_list_remove.c') (Join-Path $PSScriptRoot 'source/allocation_list_insert.c') (Join-Path $PSScriptRoot 'source/allocation_pool_release.c') (Join-Path $PSScriptRoot 'source/block_unlink.c') (Join-Path $PSScriptRoot 'source/block_pool_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/resource_release_complete_runtime.dll')
if ($LASTEXITCODE -ne 0) { throw 'Complete resource release connection compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_resource_release_complete_special.py')
if ($LASTEXITCODE -ne 0) { throw 'Complete special resource path comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_resource_release_complete_normal.py')
if ($LASTEXITCODE -ne 0) { throw 'Complete normal resource path comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/tikal_resource_runtime.c') (Join-Path $PSScriptRoot 'source/tikal_cleanup.c') (Join-Path $PSScriptRoot 'source/tikal_cleanup_full_runtime.c') (Join-Path $PSScriptRoot 'source/owner_cleanup.c') (Join-Path $PSScriptRoot 'source/entity_allocation_release.c') (Join-Path $PSScriptRoot 'source/descriptor_release.c') (Join-Path $PSScriptRoot 'source/owner_table_release.c') (Join-Path $PSScriptRoot 'source/table_reference_release.c') (Join-Path $PSScriptRoot 'source/resource_release_complete_runtime.c') (Join-Path $PSScriptRoot 'source/resource_division_runtime.c') (Join-Path $PSScriptRoot 'source/resource_reset_runtime.c') (Join-Path $PSScriptRoot 'source/resource_release_runtime.c') (Join-Path $PSScriptRoot 'source/resource_release.c') (Join-Path $PSScriptRoot 'source/resource_type_classifier.c') (Join-Path $PSScriptRoot 'source/descriptor_reset.c') (Join-Path $PSScriptRoot 'source/resource_payload_release.c') (Join-Path $PSScriptRoot 'source/linked_payload_release.c') (Join-Path $PSScriptRoot 'source/payload_metadata.c') (Join-Path $PSScriptRoot 'source/unsigned_division.c') (Join-Path $PSScriptRoot 'source/paired_allocation_release.c') (Join-Path $PSScriptRoot 'source/allocation_lookup.c') (Join-Path $PSScriptRoot 'source/allocation_release.c') (Join-Path $PSScriptRoot 'source/allocation_list_remove.c') (Join-Path $PSScriptRoot 'source/allocation_list_insert.c') (Join-Path $PSScriptRoot 'source/allocation_pool_release.c') (Join-Path $PSScriptRoot 'source/block_unlink.c') (Join-Path $PSScriptRoot 'source/block_pool_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_resource_runtime.dll')
if ($LASTEXITCODE -ne 0) { throw 'Tikal resource connection compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_cleanup_resources.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal resource connection comparison failed' }

& $python (Join-Path $PSScriptRoot 'verify_tikal_cleanup_special_resources.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal special resource connection comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/heap_free_ram.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/heap_free_ram.dll')
if ($LASTEXITCODE -ne 0) { throw 'RAM heap release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_heap_free_ram.py')
if ($LASTEXITCODE -ne 0) { throw 'RAM heap release comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_heap_runtime.c') (Join-Path $PSScriptRoot 'source/heap_free_ram.c') (Join-Path $PSScriptRoot 'source/entity_allocation_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_heap_runtime.dll')
if ($LASTEXITCODE -ne 0) { throw 'Owner heap connection compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_heap_runtime.py')
if ($LASTEXITCODE -ne 0) { throw 'Owner heap connection comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/tikal_heap_resource_runtime.c') (Join-Path $PSScriptRoot 'source/tikal_cleanup.c') (Join-Path $PSScriptRoot 'source/tikal_heap_full_runtime.c') (Join-Path $PSScriptRoot 'source/owner_heap_runtime.c') (Join-Path $PSScriptRoot 'source/heap_free_ram.c') (Join-Path $PSScriptRoot 'source/entity_allocation_release.c') (Join-Path $PSScriptRoot 'source/descriptor_release.c') (Join-Path $PSScriptRoot 'source/owner_table_release.c') (Join-Path $PSScriptRoot 'source/table_reference_release.c') (Join-Path $PSScriptRoot 'source/resource_release_complete_runtime.c') (Join-Path $PSScriptRoot 'source/resource_division_runtime.c') (Join-Path $PSScriptRoot 'source/resource_reset_runtime.c') (Join-Path $PSScriptRoot 'source/resource_release_runtime.c') (Join-Path $PSScriptRoot 'source/resource_release.c') (Join-Path $PSScriptRoot 'source/resource_type_classifier.c') (Join-Path $PSScriptRoot 'source/descriptor_reset.c') (Join-Path $PSScriptRoot 'source/resource_payload_release.c') (Join-Path $PSScriptRoot 'source/linked_payload_release.c') (Join-Path $PSScriptRoot 'source/payload_metadata.c') (Join-Path $PSScriptRoot 'source/unsigned_division.c') (Join-Path $PSScriptRoot 'source/paired_allocation_release.c') (Join-Path $PSScriptRoot 'source/allocation_lookup.c') (Join-Path $PSScriptRoot 'source/allocation_release.c') (Join-Path $PSScriptRoot 'source/allocation_list_remove.c') (Join-Path $PSScriptRoot 'source/allocation_list_insert.c') (Join-Path $PSScriptRoot 'source/allocation_pool_release.c') (Join-Path $PSScriptRoot 'source/block_unlink.c') (Join-Path $PSScriptRoot 'source/block_pool_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_heap_resource_runtime.dll')
if ($LASTEXITCODE -ne 0) { throw 'Tikal heap resource compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_heap_resources.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal heap resource comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_heap_special_resources.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal heap resource comparison failed' }

& $python (Join-Path $PSScriptRoot 'verify_fpu_helper_transfers.py')
if ($LASTEXITCODE -ne 0) { throw 'FPU helper transfer verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/helper_record_release.c') (Join-Path $PSScriptRoot 'source/heap_free_ram.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/helper_record_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Helper record release compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_helper_record_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Helper record release verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_fpu_helper_comparisons.py')
if ($LASTEXITCODE -ne 0) { throw 'FPU helper comparison verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sh4_binary32.py')
if ($LASTEXITCODE -ne 0) { throw 'Exact binary32 arithmetic verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_fpu_helper_add_sub.py')
if ($LASTEXITCODE -ne 0) { throw 'FADD/FSUB instruction verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_fpu_double_add_sub.py')
if ($LASTEXITCODE -ne 0) { throw 'Double-precision FADD/FSUB instruction verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_fpu_double_compare.py')
if ($LASTEXITCODE -ne 0) { throw 'Double-precision FCMP instruction verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_fpu_double_mul_div.py')
if ($LASTEXITCODE -ne 0) { throw 'Double-precision FMUL/FDIV instruction verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_fpu_double_float.py')
if ($LASTEXITCODE -ne 0) { throw 'Double-precision FLOAT instruction verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_fpu_helper_fmac.py')
if ($LASTEXITCODE -ne 0) { throw 'Finite FMAC verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_fpu_helper_float.py')
if ($LASTEXITCODE -ne 0) { throw 'Helper verifier checks failed' }

& $python (Join-Path $PSScriptRoot 'verify_owner_helper_switch.py')
if ($LASTEXITCODE -ne 0) { throw 'Helper verifier checks failed' }

& $python (Join-Path $PSScriptRoot 'probe_owner_helper_states.py')
if ($LASTEXITCODE -ne 0) { throw 'Original helper state baseline failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/sh4_float_i32.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native integer conversion compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_native_sh4_float_i32.py')
if ($LASTEXITCODE -ne 0) { throw 'Native integer conversion verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/sh4_add_sub.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native arithmetic compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_native_sh4_add_sub.py')
if ($LASTEXITCODE -ne 0) { throw 'Native arithmetic verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/sh4_fmac.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native FMAC compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_native_sh4_fmac.py')
if ($LASTEXITCODE -ne 0) { throw 'Native FMAC verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_helper_prepare.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_helper_prepare.dll')
if ($LASTEXITCODE -ne 0) { throw 'Helper selection compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_prepare.py')
if ($LASTEXITCODE -ne 0) { throw 'Helper selection comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_helper_state0.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_helper_state0.dll')
if ($LASTEXITCODE -ne 0) { throw 'State 0 compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_state0.py')
if ($LASTEXITCODE -ne 0) { throw 'State 0 comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_helper_terminal.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_helper_terminal.dll')
if ($LASTEXITCODE -ne 0) { throw 'Terminal state compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_terminal.py')
if ($LASTEXITCODE -ne 0) { throw 'Terminal state comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_helper_state2.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_helper_state2.dll')
if ($LASTEXITCODE -ne 0) { throw 'State 2 compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_state2.py')
if ($LASTEXITCODE -ne 0) { throw 'State 2 comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_helper_tail.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_helper_tail.dll')
if ($LASTEXITCODE -ne 0) { throw 'Helper tail compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_tail.py')
if ($LASTEXITCODE -ne 0) { throw 'Helper tail verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_helper_state2_tail.c') (Join-Path $PSScriptRoot 'source/owner_helper_state2.c') (Join-Path $PSScriptRoot 'source/owner_helper_tail.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_helper_state2_tail.dll')
if ($LASTEXITCODE -ne 0) { throw 'Helper tail compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_state2_tail.py')
if ($LASTEXITCODE -ne 0) { throw 'Helper tail verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_helper_state1.c') (Join-Path $PSScriptRoot 'source/owner_helper_state2_tail.c') (Join-Path $PSScriptRoot 'source/owner_helper_state2.c') (Join-Path $PSScriptRoot 'source/owner_helper_tail.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_helper_state1.dll')
if ($LASTEXITCODE -ne 0) { throw 'State 1 compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_state1.py')
if ($LASTEXITCODE -ne 0) { throw 'State 1 comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_helper_repeat.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_helper_repeat.dll')
if ($LASTEXITCODE -ne 0) { throw 'Repeating states compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_repeat.py')
if ($LASTEXITCODE -ne 0) { throw 'Repeating states comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_helper_transition.c') (Join-Path $PSScriptRoot 'source/owner_helper_tail.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_helper_transition.dll')
if ($LASTEXITCODE -ne 0) { throw 'Index transition compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_transition.py')
if ($LASTEXITCODE -ne 0) { throw 'Index transition comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_helper_advance.c') (Join-Path $PSScriptRoot 'source/owner_helper_transition.c') (Join-Path $PSScriptRoot 'source/owner_helper_tail.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_helper_advance.dll')
if ($LASTEXITCODE -ne 0) { throw 'Advancing states compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_advance.py')
if ($LASTEXITCODE -ne 0) { throw 'Advancing states comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_helper_clamp.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_helper_clamp.dll')
if ($LASTEXITCODE -ne 0) { throw 'Clamping states compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_clamp.py')
if ($LASTEXITCODE -ne 0) { throw 'Clamping states comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_clamp_flags.py')
if ($LASTEXITCODE -ne 0) { throw 'Clamping signed flags comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_helper_state8.c') (Join-Path $PSScriptRoot 'source/owner_helper_state2_tail.c') (Join-Path $PSScriptRoot 'source/owner_helper_state2.c') (Join-Path $PSScriptRoot 'source/owner_helper_tail.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_helper_state8.dll')
if ($LASTEXITCODE -ne 0) { throw 'State 8 compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_state8.py')
if ($LASTEXITCODE -ne 0) { throw 'State 8 comparison failed' }

$helperSourceNames = @('owner_helper_dispatch','owner_helper_prepare','owner_helper_state0','owner_helper_state1','owner_helper_state2','owner_helper_state2_tail','owner_helper_terminal','owner_helper_repeat','owner_helper_advance','owner_helper_clamp','owner_helper_state8','owner_helper_transition','owner_helper_tail','sh4_float_i32','sh4_add_sub','sh4_fmac')
$helperSourcePaths = $helperSourceNames | ForEach-Object { Join-Path $PSScriptRoot "source/$_.c" }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @helperSourcePaths -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_helper_dispatch.dll')
if ($LASTEXITCODE -ne 0) { throw 'Integrated helper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Integrated helper comparison failed' }

$helperCleanupSources = @('owner_helper_native_cleanup','helper_record_release','heap_free_ram') | ForEach-Object { Join-Path $PSScriptRoot "source/$_.c" }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @helperSourcePaths @helperCleanupSources -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_helper_native_cleanup.dll')
if ($LASTEXITCODE -ne 0) { throw 'Helper native cleanup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_native_cleanup.py')
if ($LASTEXITCODE -ne 0) { throw 'Helper native cleanup comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_model_change_prepare.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_change_prepare.dll')
if ($LASTEXITCODE -ne 0) { throw 'Model change setup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_change_prepare.py')
if ($LASTEXITCODE -ne 0) { throw 'Model change setup comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_model_change_finish.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_change_finish.dll')
if ($LASTEXITCODE -ne 0) { throw 'Model change finalization compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_change_finish.py')
if ($LASTEXITCODE -ne 0) { throw 'Model change finalization comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_channel_difference.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_channel_difference.dll')
if ($LASTEXITCODE -ne 0) { throw 'Model channel difference compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_channel_difference.py')
if ($LASTEXITCODE -ne 0) { throw 'Model channel difference comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_descriptor_walk.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_descriptor_walk.dll')
if ($LASTEXITCODE -ne 0) { throw 'Descriptor traversal compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_descriptor_walk.py')
if ($LASTEXITCODE -ne 0) { throw 'Descriptor traversal comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_angle_unwrap.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_angle_unwrap.dll')
if ($LASTEXITCODE -ne 0) { throw 'Model angle unwrap compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_angle_unwrap.py')
if ($LASTEXITCODE -ne 0) { throw 'Model angle unwrap comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_model_change_initialize.c') (Join-Path $PSScriptRoot 'source/model_descriptor_walk.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_change_initialize.dll')
if ($LASTEXITCODE -ne 0) { throw 'Model worker initialization compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_change_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Model worker initialization comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_model_channel_begin.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_channel_begin.dll')
if ($LASTEXITCODE -ne 0) { throw 'First model channel allocation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_channel_begin.py')
if ($LASTEXITCODE -ne 0) { throw 'First model channel allocation comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_model_channel_select.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_channel_select.dll')
if ($LASTEXITCODE -ne 0) { throw 'Model keyframe selection compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_channel_select.py')
if ($LASTEXITCODE -ne 0) { throw 'Model keyframe selection comparison failed' }

& $python (Join-Path $PSScriptRoot 'verify_sh4_binary32_divide.py')
if ($LASTEXITCODE -ne 0) { throw 'Exact quotient verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_fpu_helper_divide.py')
if ($LASTEXITCODE -ne 0) { throw 'FDIV instruction verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sh4_divide.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/sh4_divide.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native division compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_native_sh4_divide.py')
if ($LASTEXITCODE -ne 0) { throw 'Native division comparison failed' }

$modelInterpolationSources = @('owner_model_channel_interpolate','sh4_float_i32','sh4_add_sub','sh4_divide','sh4_fmac') | ForEach-Object { Join-Path $PSScriptRoot "source/$_.c" }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @modelInterpolationSources -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_channel_interpolate.dll')
if ($LASTEXITCODE -ne 0) { throw 'Model interpolation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_channel_interpolate.py')
if ($LASTEXITCODE -ne 0) { throw 'Model interpolation comparison failed' }

$modelFirstChannelSources = @('owner_model_first_channel','owner_model_channel_begin','owner_model_channel_select','owner_model_channel_interpolate','sh4_float_i32','sh4_add_sub','sh4_divide','sh4_fmac') | ForEach-Object { Join-Path $PSScriptRoot "source/$_.c" }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @modelFirstChannelSources -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_first_channel.dll')
if ($LASTEXITCODE -ne 0) { throw 'First model channel compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_first_channel.py')
if ($LASTEXITCODE -ne 0) { throw 'First model channel comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_model_angle_select.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_angle_select.dll')
if ($LASTEXITCODE -ne 0) { throw 'Angle keyframe selection compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_angle_select.py')
if ($LASTEXITCODE -ne 0) { throw 'Angle keyframe selection comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/sh4_multiply.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native multiply compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_native_sh4_multiply.py')
if ($LASTEXITCODE -ne 0) { throw 'Native multiply comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/sh4_truncate_i32.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native truncation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_native_sh4_truncate_i32.py')
if ($LASTEXITCODE -ne 0) { throw 'Native truncation comparison failed' }

$modelAngleInterpolationSources = @('owner_model_angle_interpolate','model_channel_difference','sh4_float_i32','sh4_add_sub','sh4_divide','sh4_multiply','sh4_fmac','sh4_truncate_i32') | ForEach-Object { Join-Path $PSScriptRoot "source/$_.c" }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @modelAngleInterpolationSources -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_angle_interpolate.dll')
if ($LASTEXITCODE -ne 0) { throw 'Angle interpolation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_angle_interpolate.py')
if ($LASTEXITCODE -ne 0) { throw 'Angle interpolation comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_model_angle_finish.c') (Join-Path $PSScriptRoot 'source/model_angle_unwrap.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_angle_finish.dll')
if ($LASTEXITCODE -ne 0) { throw 'Angle finalization compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_angle_finish.py')
if ($LASTEXITCODE -ne 0) { throw 'Angle finalization comparison failed' }

$modelAngleChannelSources = @('owner_model_angle_channel','owner_model_angle_select','owner_model_angle_interpolate','owner_model_angle_finish','model_channel_difference','model_angle_unwrap','sh4_float_i32','sh4_add_sub','sh4_divide','sh4_multiply','sh4_fmac','sh4_truncate_i32') | ForEach-Object { Join-Path $PSScriptRoot "source/$_.c" }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @modelAngleChannelSources -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_angle_channel.dll')
if ($LASTEXITCODE -ne 0) { throw 'Angle channel compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_angle_channel.py')
if ($LASTEXITCODE -ne 0) { throw 'Angle channel comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_model_third_begin.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_third_begin.dll')
if ($LASTEXITCODE -ne 0) { throw 'Third channel setup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_third_begin.py')
if ($LASTEXITCODE -ne 0) { throw 'Third channel setup comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_model_third_select.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_third_select.dll')
if ($LASTEXITCODE -ne 0) { throw 'Third channel selection compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_third_select.py')
if ($LASTEXITCODE -ne 0) { throw 'Third channel selection comparison failed' }
$modelThirdInterpolationSources = @('owner_model_third_interpolate','sh4_float_i32','sh4_add_sub','sh4_divide','sh4_fmac') | ForEach-Object { Join-Path $PSScriptRoot "source/$_.c" }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @modelThirdInterpolationSources -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_third_interpolate.dll')
if ($LASTEXITCODE -ne 0) { throw 'Third channel interpolation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_third_interpolate.py')
if ($LASTEXITCODE -ne 0) { throw 'Third channel interpolation comparison failed' }

$modelThirdChannelSources = @('owner_model_third_channel','owner_model_third_begin','owner_model_third_select','owner_model_third_interpolate','sh4_float_i32','sh4_add_sub','sh4_divide','sh4_fmac') | ForEach-Object { Join-Path $PSScriptRoot "source/$_.c" }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @modelThirdChannelSources -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_third_channel.dll')
if ($LASTEXITCODE -ne 0) { throw 'Third channel compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_third_channel.py')
if ($LASTEXITCODE -ne 0) { throw 'Third channel comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/owner_model_change_advance.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_change_advance.dll')
if ($LASTEXITCODE -ne 0) { throw 'Model loop advance compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_change_advance.py')
if ($LASTEXITCODE -ne 0) { throw 'Model loop advance comparison failed' }

$modelWorkerSources = @('owner_model_change_worker','owner_model_change_initialize','owner_model_change_advance','owner_model_change_finish','model_descriptor_walk','owner_model_first_channel','owner_model_channel_begin','owner_model_channel_select','owner_model_channel_interpolate','owner_model_angle_channel','owner_model_angle_select','owner_model_angle_interpolate','owner_model_angle_finish','model_channel_difference','model_angle_unwrap','owner_model_third_channel','owner_model_third_begin','owner_model_third_select','owner_model_third_interpolate','sh4_float_i32','sh4_add_sub','sh4_divide','sh4_fmac','sh4_multiply','sh4_truncate_i32') | ForEach-Object { Join-Path $PSScriptRoot "source/$_.c" }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @modelWorkerSources -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_change_worker.dll')
if ($LASTEXITCODE -ne 0) { throw 'Model worker compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_change_worker.py')
if ($LASTEXITCODE -ne 0) { throw 'Model worker comparison failed' }

$modelEntrySources = @((Join-Path $PSScriptRoot 'source/owner_model_change_entry.c'),(Join-Path $PSScriptRoot 'source/owner_model_change_prepare.c')) + $modelWorkerSources
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @modelEntrySources -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_change_entry.dll')
if ($LASTEXITCODE -ne 0) { throw 'Model entry compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_change_entry.py')
if ($LASTEXITCODE -ne 0) { throw 'Model entry comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/heap_allocate_ram.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/heap_allocate_ram.dll')
if ($LASTEXITCODE -ne 0) { throw 'RAM allocator compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_heap_allocate_ram.py')
if ($LASTEXITCODE -ne 0) { throw 'RAM allocator comparison failed' }

$modelNativeHeapSources = @((Join-Path $PSScriptRoot 'source/owner_model_native_heap.c'),(Join-Path $PSScriptRoot 'source/heap_allocate_ram.c')) + $modelEntrySources
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @modelNativeHeapSources -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_model_native_heap.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native heap model compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_model_native_heap.py')
if ($LASTEXITCODE -ne 0) { throw 'Native heap model comparison failed' }

$helperNativeModelSources = @('heap_zero_allocate_ram','owner_helper_native_model','owner_helper_dispatch','owner_helper_prepare','owner_helper_state0','owner_helper_state1','owner_helper_state2','owner_helper_state2_tail','owner_helper_terminal','owner_helper_repeat','owner_helper_advance','owner_helper_clamp','owner_helper_state8','owner_helper_transition','owner_helper_tail','helper_record_release','heap_free_ram') | ForEach-Object { Join-Path $PSScriptRoot "source/$_.c" }
$helperNativeModelSources += $modelNativeHeapSources
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @helperNativeModelSources -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_helper_native_model.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native parent model compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_native_model.py')
if ($LASTEXITCODE -ne 0) { throw 'Native parent model comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/heap_zero_allocate_ram.c') (Join-Path $PSScriptRoot 'source/heap_allocate_ram.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/heap_zero_allocate_ram.dll')
if ($LASTEXITCODE -ne 0) { throw 'Zero allocator compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_heap_zero_allocate_ram.py')
if ($LASTEXITCODE -ne 0) { throw 'Zero allocator comparison failed' }

& $python (Join-Path $PSScriptRoot 'verify_owner_helper_native_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Native parent initialization comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_helper_native_initialize_cycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Native initialization cycle comparison failed' }

& $python (Join-Path $PSScriptRoot 'verify_owner_helper_native_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Native parent lifecycle comparison failed' }

& $python (Join-Path $PSScriptRoot 'verify_owner_helper_native_state8.py')
if ($LASTEXITCODE -ne 0) { throw 'Native parent state-8 comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_vector_restore.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_restore.dll')
if ($LASTEXITCODE -ne 0) { throw 'Vector restore compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_restore.py')
if ($LASTEXITCODE -ne 0) { throw 'Vector restore comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_vector_cleanup.c') (Join-Path $PSScriptRoot 'source/model_vector_restore.c') (Join-Path $PSScriptRoot 'source/heap_free_ram.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_cleanup.dll')
if ($LASTEXITCODE -ne 0) { throw 'Vector cleanup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_cleanup.py')
if ($LASTEXITCODE -ne 0) { throw 'Vector cleanup comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_vector_save.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_save.dll')
if ($LASTEXITCODE -ne 0) { throw 'Vector save compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_save.py')
if ($LASTEXITCODE -ne 0) { throw 'Vector save comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_descriptor_lookup.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_descriptor_lookup.dll')
if ($LASTEXITCODE -ne 0) { throw 'Descriptor lookup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_descriptor_lookup.py')
if ($LASTEXITCODE -ne 0) { throw 'Descriptor lookup comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_vector_initialize.c') (Join-Path $PSScriptRoot 'source/model_vector_save.c') (Join-Path $PSScriptRoot 'source/model_descriptor_lookup.c') (Join-Path $PSScriptRoot 'source/heap_allocate_ram.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_initialize.dll')
if ($LASTEXITCODE -ne 0) { throw 'Vector initialization compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Vector initialization comparison failed' }

& $python (Join-Path $PSScriptRoot 'verify_model_vector_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Vector lifecycle comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_vector_render_select.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_render_select.dll')
if ($LASTEXITCODE -ne 0) { throw 'Rendering branch compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_render_select.py')
if ($LASTEXITCODE -ne 0) { throw 'Rendering branch comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_attachment_route.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_attachment_route.dll')
if ($LASTEXITCODE -ne 0) { throw 'Attachment routing compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_attachment_route.py')
if ($LASTEXITCODE -ne 0) { throw 'Attachment routing comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_matrix_bank_save.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_matrix_bank_save.dll')
if ($LASTEXITCODE -ne 0) { throw 'Matrix bank compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_matrix_bank_save.py')
if ($LASTEXITCODE -ne 0) { throw 'Matrix bank comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_matrix_translation.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_matrix_translation.dll')
if ($LASTEXITCODE -ne 0) { throw 'Matrix translation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_matrix_translation.py')
if ($LASTEXITCODE -ne 0) { throw 'Matrix translation comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_matrix_stack_pop.dll')
if ($LASTEXITCODE -ne 0) { throw 'Matrix stack compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_matrix_stack_pop.py')
if ($LASTEXITCODE -ne 0) { throw 'Matrix stack comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_attachment_native_stack.c') (Join-Path $PSScriptRoot 'source/model_attachment_route.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_attachment_native_stack.dll')
if ($LASTEXITCODE -ne 0) { throw 'Attachment stack compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_attachment_native_stack.py')
if ($LASTEXITCODE -ne 0) { throw 'Attachment stack comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_matrix_stack_push.dll')
if ($LASTEXITCODE -ne 0) { throw 'Matrix push compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_matrix_stack_push.py')
if ($LASTEXITCODE -ne 0) { throw 'Matrix push comparison failed' }

& $python (Join-Path $PSScriptRoot 'verify_model_matrix_stack_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Matrix stack lifecycle comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_point_transform.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_point_transform.dll')
if ($LASTEXITCODE -ne 0) { throw 'Point transform compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_point_transform.py')
if ($LASTEXITCODE -ne 0) { throw 'Point transform dataflow comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_direction_transform.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_direction_transform.dll')
if ($LASTEXITCODE -ne 0) { throw 'Direction transform compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_direction_transform.py')
if ($LASTEXITCODE -ne 0) { throw 'Direction transform dataflow comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_attachment_transforms.c') (Join-Path $PSScriptRoot 'source/model_attachment_route.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') (Join-Path $PSScriptRoot 'source/model_point_transform.c') (Join-Path $PSScriptRoot 'source/model_direction_transform.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_attachment_transforms.dll')
if ($LASTEXITCODE -ne 0) { throw 'Attachment transform integration compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_attachment_transforms.py')
if ($LASTEXITCODE -ne 0) { throw 'Attachment transform integration comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_vector_type2.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_type2.dll')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_type2 compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_type2.py')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_type2 comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_vector_type2_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type2.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') (Join-Path $PSScriptRoot 'source/model_point_transform.c') (Join-Path $PSScriptRoot 'source/model_direction_transform.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/model_matrix_compose_implicit.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_type2_transforms.dll')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_type2_transforms compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_type2_transforms.py')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_type2_transforms comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_matrix_compose_implicit.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_matrix_compose_implicit.dll')
if ($LASTEXITCODE -ne 0) { throw 'Matrix composition compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_matrix_compose_implicit.py')
if ($LASTEXITCODE -ne 0) { throw 'Matrix composition comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_matrix_compose.c') (Join-Path $PSScriptRoot 'source/model_matrix_compose_implicit.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_matrix_compose.dll')
if ($LASTEXITCODE -ne 0) { throw 'Full matrix composition compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_matrix_compose.py')
if ($LASTEXITCODE -ne 0) { throw 'Full matrix composition comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_vector_type3.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_type3.dll')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_type3 compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_type3.py')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_type3 comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_vector_type3_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type3.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') (Join-Path $PSScriptRoot 'source/model_point_transform.c') (Join-Path $PSScriptRoot 'source/model_direction_transform.c') (Join-Path $PSScriptRoot 'source/model_matrix_compose_implicit.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_type3_transforms.dll')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_type3_transforms compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_type3_transforms.py')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_type3_transforms comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_vector_blend.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_blend.dll')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_blend compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_blend.py')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_blend comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_vector_type1.c') (Join-Path $PSScriptRoot 'source/model_vector_blend.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_type1.dll')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_type1 compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_type1.py')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_type1 comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_vector_normalize.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_normalize.dll')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_normalize compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_normalize.py')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_normalize comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_vector_type1_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type1.c') (Join-Path $PSScriptRoot 'source/model_vector_blend.c') (Join-Path $PSScriptRoot 'source/model_point_transform.c') (Join-Path $PSScriptRoot 'source/model_direction_transform.c') (Join-Path $PSScriptRoot 'source/model_vector_normalize.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_type1_transforms.dll')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_type1_transforms compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_type1_transforms.py')
if ($LASTEXITCODE -ne 0) { throw 'model_vector_type1_transforms comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_render_prepare.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_render_prepare.dll')
if ($LASTEXITCODE -ne 0) { throw 'model_render_prepare compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_render_prepare.py')
if ($LASTEXITCODE -ne 0) { throw 'model_render_prepare comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_render_frame_setup.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_render_frame_setup.dll')
if ($LASTEXITCODE -ne 0) { throw 'model_render_frame_setup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_render_frame_setup.py')
if ($LASTEXITCODE -ne 0) { throw 'model_render_frame_setup comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_render_single_mesh.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_render_single_mesh.dll')
if ($LASTEXITCODE -ne 0) { throw 'Single mesh compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_render_single_mesh.py')
if ($LASTEXITCODE -ne 0) { throw 'Single mesh comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_render_record_gate.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_render_record_gate.dll')
if ($LASTEXITCODE -ne 0) { throw 'Record gate compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_render_record_gate.py')
if ($LASTEXITCODE -ne 0) { throw 'Record gate comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_render_record_cache.c') (Join-Path $PSScriptRoot 'source/model_render_record_gate.c') (Join-Path $PSScriptRoot 'source/model_descriptor_lookup.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_render_record_cache.dll')
if ($LASTEXITCODE -ne 0) { throw 'Record cache compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_render_record_cache.py')
if ($LASTEXITCODE -ne 0) { throw 'Record cache comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_matrix.c') (Join-Path $PSScriptRoot 'source/model_matrix_copy.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_matrix.dll')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy matrix compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_matrix.py')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy matrix comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_render_record_setup.c') (Join-Path $PSScriptRoot 'source/model_render_record_cache.c') (Join-Path $PSScriptRoot 'source/model_render_record_gate.c') (Join-Path $PSScriptRoot 'source/model_descriptor_lookup.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_matrix.c') (Join-Path $PSScriptRoot 'source/model_matrix_copy.c') (Join-Path $PSScriptRoot 'source/model_render_single_mesh.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_render_record_setup.dll')
if ($LASTEXITCODE -ne 0) { throw 'Record setup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_render_record_setup.py')
if ($LASTEXITCODE -ne 0) { throw 'Record setup comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_matrix_inverse_singular.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_matrix_inverse_singular.dll')
if ($LASTEXITCODE -ne 0) { throw 'Singular inverse compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_matrix_inverse_singular.py')
if ($LASTEXITCODE -ne 0) { throw 'Singular inverse comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_matrix_determinant.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_matrix_determinant.dll')
if ($LASTEXITCODE -ne 0) { throw 'Determinant compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_matrix_determinant.py')
if ($LASTEXITCODE -ne 0) { throw 'Determinant comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_matrix_inverse.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse_singular.c') (Join-Path $PSScriptRoot 'source/model_matrix_determinant.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_matrix_inverse.dll')
if ($LASTEXITCODE -ne 0) { throw 'Inverse compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_matrix_inverse.py')
if ($LASTEXITCODE -ne 0) { throw 'Inverse comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_render_record_native.c') (Join-Path $PSScriptRoot 'source/model_render_record_setup.c') (Join-Path $PSScriptRoot 'source/model_render_record_cache.c') (Join-Path $PSScriptRoot 'source/model_render_record_gate.c') (Join-Path $PSScriptRoot 'source/model_descriptor_lookup.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_matrix.c') (Join-Path $PSScriptRoot 'source/model_matrix_copy.c') (Join-Path $PSScriptRoot 'source/model_render_single_mesh.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse_singular.c') (Join-Path $PSScriptRoot 'source/model_matrix_determinant.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_render_record_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native record setup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_render_record_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native record setup comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_render_record_loop.c') (Join-Path $PSScriptRoot 'source/model_render_record_native.c') (Join-Path $PSScriptRoot 'source/model_render_record_setup.c') (Join-Path $PSScriptRoot 'source/model_render_record_cache.c') (Join-Path $PSScriptRoot 'source/model_render_record_gate.c') (Join-Path $PSScriptRoot 'source/model_descriptor_lookup.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_matrix.c') (Join-Path $PSScriptRoot 'source/model_matrix_copy.c') (Join-Path $PSScriptRoot 'source/model_render_single_mesh.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse_singular.c') (Join-Path $PSScriptRoot 'source/model_matrix_determinant.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/model_vector_render_select.c') (Join-Path $PSScriptRoot 'source/model_vector_type1_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type1.c') (Join-Path $PSScriptRoot 'source/model_vector_blend.c') (Join-Path $PSScriptRoot 'source/model_point_transform.c') (Join-Path $PSScriptRoot 'source/model_direction_transform.c') (Join-Path $PSScriptRoot 'source/model_vector_normalize.c') (Join-Path $PSScriptRoot 'source/model_vector_type2_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type2.c') (Join-Path $PSScriptRoot 'source/model_matrix_compose_implicit.c') (Join-Path $PSScriptRoot 'source/model_vector_type3_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type3.c') (Join-Path $PSScriptRoot 'source/model_attachment_transforms.c') (Join-Path $PSScriptRoot 'source/model_attachment_route.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_render_record_loop.dll')
if ($LASTEXITCODE -ne 0) { throw 'Record loop compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_render_record_loop.py')
if ($LASTEXITCODE -ne 0) { throw 'Record loop comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') (Join-Path $PSScriptRoot 'source/model_matrix_copy.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_prepare.dll')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy preparation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_prepare.py')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy preparation comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_entry.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_generator_entry.dll')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy generator entry compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_generator_entry.py')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy generator entry comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_tail.c') (Join-Path $PSScriptRoot 'source/model_matrix_bank_save.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_generator_tail.dll')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy generator tail compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_generator_tail.py')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy generator tail comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_fallback.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_channel_fallback.dll')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy channel fallback compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_channel_fallback.py')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy channel fallback comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_matrix_scale.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_matrix_scale.dll')
if ($LASTEXITCODE -ne 0) { throw 'Matrix scale compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_matrix_scale.py')
if ($LASTEXITCODE -ne 0) { throw 'Matrix scale comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_matrix_translate.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_matrix_translate.dll')
if ($LASTEXITCODE -ne 0) { throw 'Matrix translation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_matrix_translate.py')
if ($LASTEXITCODE -ne 0) { throw 'Matrix translation comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_fallback.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale.c') (Join-Path $PSScriptRoot 'source/model_matrix_translate.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_channel_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native hierarchy channel compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_channel_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native hierarchy channel comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_matrix_rotate_implicit.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_matrix_rotate_implicit.dll')
if ($LASTEXITCODE -ne 0) { throw 'Implicit rotation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_matrix_rotate_implicit.py')
if ($LASTEXITCODE -ne 0) { throw 'Implicit rotation comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_matrix_rotate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate_implicit.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_matrix_rotate.dll')
if ($LASTEXITCODE -ne 0) { throw 'Matrix rotation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_matrix_rotate.py')
if ($LASTEXITCODE -ne 0) { throw 'Matrix rotation comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_transforms.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_fallback.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale.c') (Join-Path $PSScriptRoot 'source/model_matrix_translate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate_implicit.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_channel_transforms.dll')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy transforms compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_channel_transforms.py')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy transforms comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_keyframe_select.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_keyframe_select.dll')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy keyframe selection compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_keyframe_select.py')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy keyframe selection comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_factor.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_factor.dll')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy factor compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_factor.py')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy factor comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_position_interpolate.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_position_interpolate.dll')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy position interpolation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_position_interpolate.py')
if ($LASTEXITCODE -ne 0) { throw 'Hierarchy position interpolation comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_matrix_scale_registers.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_matrix_scale_registers.dll')
if ($LASTEXITCODE -ne 0) { throw 'Scale channel compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_matrix_scale_registers.py')
if ($LASTEXITCODE -ne 0) { throw 'Scale channel comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_scale_interpolate.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale_registers.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_scale_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Scale channel compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_scale_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Scale channel comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_interpolate.c') (Join-Path $PSScriptRoot 'source/model_channel_difference.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_angle_interpolate.dll')
if ($LASTEXITCODE -ne 0) { throw 'Angle interpolation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_angle_interpolate.py')
if ($LASTEXITCODE -ne 0) { throw 'Angle interpolation comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_interpolate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate_implicit.c') (Join-Path $PSScriptRoot 'source/model_channel_difference.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_angle_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native angle channel compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_angle_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native angle channel comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_position_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_interpolate.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_position_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native position channel compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_position_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native position channel comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_frame_convert.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_frame_convert.dll')
if ($LASTEXITCODE -ne 0) { throw 'Frame conversion compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_frame_convert.py')
if ($LASTEXITCODE -ne 0) { throw 'Frame conversion comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_prepare.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_frame_convert.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_keyframe_select.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_factor.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_channel_prepare.dll')
if ($LASTEXITCODE -ne 0) { throw 'Channel preparation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_channel_prepare.py')
if ($LASTEXITCODE -ne 0) { throw 'Channel preparation comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_nonempty.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_prepare.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_frame_convert.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_keyframe_select.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_factor.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_scale_interpolate.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale_registers.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate_implicit.c') (Join-Path $PSScriptRoot 'source/model_channel_difference.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_channel_nonempty.dll')
if ($LASTEXITCODE -ne 0) { throw 'Nonempty channel compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_channel_nonempty.py')
if ($LASTEXITCODE -ne 0) { throw 'Nonempty channel comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_nonempty.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_prepare.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_frame_convert.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_keyframe_select.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_factor.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_scale_interpolate.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale_registers.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate_implicit.c') (Join-Path $PSScriptRoot 'source/model_channel_difference.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_loop.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_transforms.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_fallback.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale.c') (Join-Path $PSScriptRoot 'source/model_matrix_translate.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_channel_loop.dll')
if ($LASTEXITCODE -ne 0) { throw 'Channel loop compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_channel_loop.py')
if ($LASTEXITCODE -ne 0) { throw 'Channel loop comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_nonempty.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_prepare.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_frame_convert.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_keyframe_select.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_factor.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_scale_interpolate.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale_registers.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate_implicit.c') (Join-Path $PSScriptRoot 'source/model_channel_difference.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_loop.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_transforms.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_fallback.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale.c') (Join-Path $PSScriptRoot 'source/model_matrix_translate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_entry.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_tail.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') (Join-Path $PSScriptRoot 'source/model_matrix_bank_save.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_generator.dll')
if ($LASTEXITCODE -ne 0) { throw 'Recursive generator compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_generator.py')
if ($LASTEXITCODE -ne 0) { throw 'Recursive generator comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_nonempty.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_prepare.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_frame_convert.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_keyframe_select.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_factor.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_scale_interpolate.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale_registers.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate_implicit.c') (Join-Path $PSScriptRoot 'source/model_channel_difference.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_loop.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_transforms.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_fallback.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale.c') (Join-Path $PSScriptRoot 'source/model_matrix_translate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_entry.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_tail.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') (Join-Path $PSScriptRoot 'source/model_matrix_bank_save.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare.c') (Join-Path $PSScriptRoot 'source/model_matrix_copy.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_hierarchy_prepare_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native hierarchy preparation compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_hierarchy_prepare_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native hierarchy preparation comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_nonempty.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_prepare.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_frame_convert.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_keyframe_select.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_factor.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_scale_interpolate.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale_registers.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate_implicit.c') (Join-Path $PSScriptRoot 'source/model_channel_difference.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_loop.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_transforms.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_fallback.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale.c') (Join-Path $PSScriptRoot 'source/model_matrix_translate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_entry.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_tail.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') (Join-Path $PSScriptRoot 'source/model_matrix_bank_save.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare.c') (Join-Path $PSScriptRoot 'source/model_matrix_copy.c') (Join-Path $PSScriptRoot 'source/model_render_hierarchy_native.c') (Join-Path $PSScriptRoot 'source/model_render_prepare.c') (Join-Path $PSScriptRoot 'source/model_render_frame_setup.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_render_hierarchy_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Render hierarchy compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_render_hierarchy_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Render hierarchy comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_nonempty.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_prepare.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_frame_convert.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_keyframe_select.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_factor.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_scale_interpolate.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale_registers.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate_implicit.c') (Join-Path $PSScriptRoot 'source/model_channel_difference.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_loop.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_transforms.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_fallback.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale.c') (Join-Path $PSScriptRoot 'source/model_matrix_translate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_entry.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_tail.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') (Join-Path $PSScriptRoot 'source/model_matrix_bank_save.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare.c') (Join-Path $PSScriptRoot 'source/model_matrix_copy.c') (Join-Path $PSScriptRoot 'source/model_render_hierarchy_native.c') (Join-Path $PSScriptRoot 'source/model_render_prepare.c') (Join-Path $PSScriptRoot 'source/model_render_frame_setup.c') (Join-Path $PSScriptRoot 'source/model_render_record_loop.c') (Join-Path $PSScriptRoot 'source/model_render_record_native.c') (Join-Path $PSScriptRoot 'source/model_render_record_setup.c') (Join-Path $PSScriptRoot 'source/model_render_record_cache.c') (Join-Path $PSScriptRoot 'source/model_render_record_gate.c') (Join-Path $PSScriptRoot 'source/model_descriptor_lookup.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_matrix.c') (Join-Path $PSScriptRoot 'source/model_render_single_mesh.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse_singular.c') (Join-Path $PSScriptRoot 'source/model_matrix_determinant.c') (Join-Path $PSScriptRoot 'source/model_vector_render_select.c') (Join-Path $PSScriptRoot 'source/model_vector_type1_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type1.c') (Join-Path $PSScriptRoot 'source/model_vector_blend.c') (Join-Path $PSScriptRoot 'source/model_point_transform.c') (Join-Path $PSScriptRoot 'source/model_direction_transform.c') (Join-Path $PSScriptRoot 'source/model_vector_normalize.c') (Join-Path $PSScriptRoot 'source/model_vector_type2_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type2.c') (Join-Path $PSScriptRoot 'source/model_matrix_compose_implicit.c') (Join-Path $PSScriptRoot 'source/model_vector_type3_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type3.c') (Join-Path $PSScriptRoot 'source/model_attachment_transforms.c') (Join-Path $PSScriptRoot 'source/model_attachment_route.c') (Join-Path $PSScriptRoot 'source/model_render_native.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_render_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native render compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_render_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native render comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_vector_frame.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_frame.dll')
if ($LASTEXITCODE -ne 0) { throw 'Model vector frame compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_frame.py')
if ($LASTEXITCODE -ne 0) { throw 'Model vector frame comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_nonempty.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_prepare.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_frame_convert.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_keyframe_select.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_factor.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_scale_interpolate.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale_registers.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate_implicit.c') (Join-Path $PSScriptRoot 'source/model_channel_difference.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_loop.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_transforms.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_fallback.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale.c') (Join-Path $PSScriptRoot 'source/model_matrix_translate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_entry.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_tail.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') (Join-Path $PSScriptRoot 'source/model_matrix_bank_save.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare.c') (Join-Path $PSScriptRoot 'source/model_matrix_copy.c') (Join-Path $PSScriptRoot 'source/model_render_hierarchy_native.c') (Join-Path $PSScriptRoot 'source/model_render_prepare.c') (Join-Path $PSScriptRoot 'source/model_render_frame_setup.c') (Join-Path $PSScriptRoot 'source/model_render_record_loop.c') (Join-Path $PSScriptRoot 'source/model_render_record_native.c') (Join-Path $PSScriptRoot 'source/model_render_record_setup.c') (Join-Path $PSScriptRoot 'source/model_render_record_cache.c') (Join-Path $PSScriptRoot 'source/model_render_record_gate.c') (Join-Path $PSScriptRoot 'source/model_descriptor_lookup.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_matrix.c') (Join-Path $PSScriptRoot 'source/model_render_single_mesh.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse_singular.c') (Join-Path $PSScriptRoot 'source/model_matrix_determinant.c') (Join-Path $PSScriptRoot 'source/model_vector_render_select.c') (Join-Path $PSScriptRoot 'source/model_vector_type1_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type1.c') (Join-Path $PSScriptRoot 'source/model_vector_blend.c') (Join-Path $PSScriptRoot 'source/model_point_transform.c') (Join-Path $PSScriptRoot 'source/model_direction_transform.c') (Join-Path $PSScriptRoot 'source/model_vector_normalize.c') (Join-Path $PSScriptRoot 'source/model_vector_type2_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type2.c') (Join-Path $PSScriptRoot 'source/model_matrix_compose_implicit.c') (Join-Path $PSScriptRoot 'source/model_vector_type3_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type3.c') (Join-Path $PSScriptRoot 'source/model_attachment_transforms.c') (Join-Path $PSScriptRoot 'source/model_attachment_route.c') (Join-Path $PSScriptRoot 'source/model_render_native.c') (Join-Path $PSScriptRoot 'source/model_vector_frame.c') (Join-Path $PSScriptRoot 'source/model_vector_render_call.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_render_call.dll')
if ($LASTEXITCODE -ne 0) { throw 'Vector render call compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_render_call.py')
if ($LASTEXITCODE -ne 0) { throw 'Vector render call comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_nonempty.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_prepare.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_frame_convert.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_keyframe_select.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_factor.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_scale_interpolate.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale_registers.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate_implicit.c') (Join-Path $PSScriptRoot 'source/model_channel_difference.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_loop.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_transforms.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_fallback.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale.c') (Join-Path $PSScriptRoot 'source/model_matrix_translate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_entry.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_tail.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') (Join-Path $PSScriptRoot 'source/model_matrix_bank_save.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare.c') (Join-Path $PSScriptRoot 'source/model_matrix_copy.c') (Join-Path $PSScriptRoot 'source/model_render_hierarchy_native.c') (Join-Path $PSScriptRoot 'source/model_render_prepare.c') (Join-Path $PSScriptRoot 'source/model_render_frame_setup.c') (Join-Path $PSScriptRoot 'source/model_render_record_loop.c') (Join-Path $PSScriptRoot 'source/model_render_record_native.c') (Join-Path $PSScriptRoot 'source/model_render_record_setup.c') (Join-Path $PSScriptRoot 'source/model_render_record_cache.c') (Join-Path $PSScriptRoot 'source/model_render_record_gate.c') (Join-Path $PSScriptRoot 'source/model_descriptor_lookup.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_matrix.c') (Join-Path $PSScriptRoot 'source/model_render_single_mesh.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse_singular.c') (Join-Path $PSScriptRoot 'source/model_matrix_determinant.c') (Join-Path $PSScriptRoot 'source/model_vector_render_select.c') (Join-Path $PSScriptRoot 'source/model_vector_type1_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type1.c') (Join-Path $PSScriptRoot 'source/model_vector_blend.c') (Join-Path $PSScriptRoot 'source/model_point_transform.c') (Join-Path $PSScriptRoot 'source/model_direction_transform.c') (Join-Path $PSScriptRoot 'source/model_vector_normalize.c') (Join-Path $PSScriptRoot 'source/model_vector_type2_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type2.c') (Join-Path $PSScriptRoot 'source/model_matrix_compose_implicit.c') (Join-Path $PSScriptRoot 'source/model_vector_type3_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type3.c') (Join-Path $PSScriptRoot 'source/model_attachment_transforms.c') (Join-Path $PSScriptRoot 'source/model_attachment_route.c') (Join-Path $PSScriptRoot 'source/model_render_native.c') (Join-Path $PSScriptRoot 'source/model_vector_frame.c') (Join-Path $PSScriptRoot 'source/model_vector_render_call.c') (Join-Path $PSScriptRoot 'source/model_vector_call.c') (Join-Path $PSScriptRoot 'source/model_vector_initialize.c') (Join-Path $PSScriptRoot 'source/model_vector_save.c') (Join-Path $PSScriptRoot 'source/model_vector_cleanup.c') (Join-Path $PSScriptRoot 'source/model_vector_restore.c') (Join-Path $PSScriptRoot 'source/heap_allocate_ram.c') (Join-Path $PSScriptRoot 'source/heap_free_ram.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/model_vector_call.dll')
if ($LASTEXITCODE -ne 0) { throw 'Model vector call compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_call.py')
if ($LASTEXITCODE -ne 0) { throw 'Model vector call rendering comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_model_vector_call_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Model vector call lifecycle comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_nonempty.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_prepare.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_frame_convert.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_keyframe_select.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_factor.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_scale_interpolate.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale_registers.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate_implicit.c') (Join-Path $PSScriptRoot 'source/model_channel_difference.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_loop.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_transforms.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_fallback.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale.c') (Join-Path $PSScriptRoot 'source/model_matrix_translate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_entry.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_tail.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') (Join-Path $PSScriptRoot 'source/model_matrix_bank_save.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare.c') (Join-Path $PSScriptRoot 'source/model_matrix_copy.c') (Join-Path $PSScriptRoot 'source/model_render_hierarchy_native.c') (Join-Path $PSScriptRoot 'source/model_render_prepare.c') (Join-Path $PSScriptRoot 'source/model_render_frame_setup.c') (Join-Path $PSScriptRoot 'source/model_render_record_loop.c') (Join-Path $PSScriptRoot 'source/model_render_record_native.c') (Join-Path $PSScriptRoot 'source/model_render_record_setup.c') (Join-Path $PSScriptRoot 'source/model_render_record_cache.c') (Join-Path $PSScriptRoot 'source/model_render_record_gate.c') (Join-Path $PSScriptRoot 'source/model_descriptor_lookup.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_matrix.c') (Join-Path $PSScriptRoot 'source/model_render_single_mesh.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse_singular.c') (Join-Path $PSScriptRoot 'source/model_matrix_determinant.c') (Join-Path $PSScriptRoot 'source/model_vector_render_select.c') (Join-Path $PSScriptRoot 'source/model_vector_type1_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type1.c') (Join-Path $PSScriptRoot 'source/model_vector_blend.c') (Join-Path $PSScriptRoot 'source/model_point_transform.c') (Join-Path $PSScriptRoot 'source/model_direction_transform.c') (Join-Path $PSScriptRoot 'source/model_vector_normalize.c') (Join-Path $PSScriptRoot 'source/model_vector_type2_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type2.c') (Join-Path $PSScriptRoot 'source/model_matrix_compose_implicit.c') (Join-Path $PSScriptRoot 'source/model_vector_type3_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type3.c') (Join-Path $PSScriptRoot 'source/model_attachment_transforms.c') (Join-Path $PSScriptRoot 'source/model_attachment_route.c') (Join-Path $PSScriptRoot 'source/model_render_native.c') (Join-Path $PSScriptRoot 'source/model_vector_frame.c') (Join-Path $PSScriptRoot 'source/model_vector_render_call.c') (Join-Path $PSScriptRoot 'source/model_vector_call.c') (Join-Path $PSScriptRoot 'source/model_vector_initialize.c') (Join-Path $PSScriptRoot 'source/model_vector_save.c') (Join-Path $PSScriptRoot 'source/model_vector_cleanup.c') (Join-Path $PSScriptRoot 'source/model_vector_restore.c') (Join-Path $PSScriptRoot 'source/heap_allocate_ram.c') (Join-Path $PSScriptRoot 'source/heap_free_ram.c') (Join-Path $PSScriptRoot 'source/owner_vector_runtime.c') (Join-Path $PSScriptRoot 'source/owner_heap_runtime.c') (Join-Path $PSScriptRoot 'source/entity_allocation_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_vector_runtime.dll')
if ($LASTEXITCODE -ne 0) { throw 'Owner vector integration compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_vector_runtime.py')
if ($LASTEXITCODE -ne 0) { throw 'Owner vector integration comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_nonempty.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_prepare.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_frame_convert.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_keyframe_select.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_factor.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_scale_interpolate.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale_registers.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate_implicit.c') (Join-Path $PSScriptRoot 'source/model_channel_difference.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_loop.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_transforms.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_fallback.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale.c') (Join-Path $PSScriptRoot 'source/model_matrix_translate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_entry.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_tail.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') (Join-Path $PSScriptRoot 'source/model_matrix_bank_save.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare.c') (Join-Path $PSScriptRoot 'source/model_matrix_copy.c') (Join-Path $PSScriptRoot 'source/model_render_hierarchy_native.c') (Join-Path $PSScriptRoot 'source/model_render_prepare.c') (Join-Path $PSScriptRoot 'source/model_render_frame_setup.c') (Join-Path $PSScriptRoot 'source/model_render_record_loop.c') (Join-Path $PSScriptRoot 'source/model_render_record_native.c') (Join-Path $PSScriptRoot 'source/model_render_record_setup.c') (Join-Path $PSScriptRoot 'source/model_render_record_cache.c') (Join-Path $PSScriptRoot 'source/model_render_record_gate.c') (Join-Path $PSScriptRoot 'source/model_descriptor_lookup.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_matrix.c') (Join-Path $PSScriptRoot 'source/model_render_single_mesh.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse_singular.c') (Join-Path $PSScriptRoot 'source/model_matrix_determinant.c') (Join-Path $PSScriptRoot 'source/model_vector_render_select.c') (Join-Path $PSScriptRoot 'source/model_vector_type1_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type1.c') (Join-Path $PSScriptRoot 'source/model_vector_blend.c') (Join-Path $PSScriptRoot 'source/model_point_transform.c') (Join-Path $PSScriptRoot 'source/model_direction_transform.c') (Join-Path $PSScriptRoot 'source/model_vector_normalize.c') (Join-Path $PSScriptRoot 'source/model_vector_type2_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type2.c') (Join-Path $PSScriptRoot 'source/model_matrix_compose_implicit.c') (Join-Path $PSScriptRoot 'source/model_vector_type3_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type3.c') (Join-Path $PSScriptRoot 'source/model_attachment_transforms.c') (Join-Path $PSScriptRoot 'source/model_attachment_route.c') (Join-Path $PSScriptRoot 'source/model_render_native.c') (Join-Path $PSScriptRoot 'source/model_vector_frame.c') (Join-Path $PSScriptRoot 'source/model_vector_render_call.c') (Join-Path $PSScriptRoot 'source/model_vector_call.c') (Join-Path $PSScriptRoot 'source/model_vector_initialize.c') (Join-Path $PSScriptRoot 'source/model_vector_save.c') (Join-Path $PSScriptRoot 'source/model_vector_cleanup.c') (Join-Path $PSScriptRoot 'source/model_vector_restore.c') (Join-Path $PSScriptRoot 'source/heap_allocate_ram.c') (Join-Path $PSScriptRoot 'source/heap_free_ram.c') (Join-Path $PSScriptRoot 'source/owner_vector_runtime.c') (Join-Path $PSScriptRoot 'source/owner_heap_runtime.c') (Join-Path $PSScriptRoot 'source/entity_allocation_release.c') (Join-Path $PSScriptRoot 'source/owner_cleanup_native.c') (Join-Path $PSScriptRoot 'source/owner_helper_prepare.c') (Join-Path $PSScriptRoot 'source/owner_helper_terminal.c') (Join-Path $PSScriptRoot 'source/helper_record_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/owner_cleanup_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native owner cleanup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_owner_cleanup_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native owner cleanup comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/tikal_heap_resource_runtime.c') (Join-Path $PSScriptRoot 'source/tikal_cleanup.c') (Join-Path $PSScriptRoot 'source/tikal_heap_full_runtime.c') (Join-Path $PSScriptRoot 'source/owner_heap_runtime.c') (Join-Path $PSScriptRoot 'source/heap_free_ram.c') (Join-Path $PSScriptRoot 'source/entity_allocation_release.c') (Join-Path $PSScriptRoot 'source/descriptor_release.c') (Join-Path $PSScriptRoot 'source/owner_table_release.c') (Join-Path $PSScriptRoot 'source/table_reference_release.c') (Join-Path $PSScriptRoot 'source/resource_release_complete_runtime.c') (Join-Path $PSScriptRoot 'source/resource_division_runtime.c') (Join-Path $PSScriptRoot 'source/resource_reset_runtime.c') (Join-Path $PSScriptRoot 'source/resource_release_runtime.c') (Join-Path $PSScriptRoot 'source/resource_release.c') (Join-Path $PSScriptRoot 'source/resource_type_classifier.c') (Join-Path $PSScriptRoot 'source/descriptor_reset.c') (Join-Path $PSScriptRoot 'source/resource_payload_release.c') (Join-Path $PSScriptRoot 'source/linked_payload_release.c') (Join-Path $PSScriptRoot 'source/payload_metadata.c') (Join-Path $PSScriptRoot 'source/unsigned_division.c') (Join-Path $PSScriptRoot 'source/paired_allocation_release.c') (Join-Path $PSScriptRoot 'source/allocation_lookup.c') (Join-Path $PSScriptRoot 'source/allocation_release.c') (Join-Path $PSScriptRoot 'source/allocation_list_remove.c') (Join-Path $PSScriptRoot 'source/allocation_list_insert.c') (Join-Path $PSScriptRoot 'source/allocation_pool_release.c') (Join-Path $PSScriptRoot 'source/block_unlink.c') (Join-Path $PSScriptRoot 'source/block_pool_release.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_nonempty.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_prepare.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_frame_convert.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_keyframe_select.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_factor.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_position_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_angle_interpolate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_scale_interpolate.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale_registers.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate.c') (Join-Path $PSScriptRoot 'source/model_matrix_rotate_implicit.c') (Join-Path $PSScriptRoot 'source/model_channel_difference.c') (Join-Path $PSScriptRoot 'source/sh4_float_i32.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') (Join-Path $PSScriptRoot 'source/sh4_divide.c') (Join-Path $PSScriptRoot 'source/sh4_truncate_i32.c') (Join-Path $PSScriptRoot 'source/sh4_multiply.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_loop.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_transforms.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_channel_fallback.c') (Join-Path $PSScriptRoot 'source/model_matrix_scale.c') (Join-Path $PSScriptRoot 'source/model_matrix_translate.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_entry.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_generator_tail.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_push.c') (Join-Path $PSScriptRoot 'source/model_matrix_stack_pop.c') (Join-Path $PSScriptRoot 'source/model_matrix_bank_save.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare_native.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_prepare.c') (Join-Path $PSScriptRoot 'source/model_matrix_copy.c') (Join-Path $PSScriptRoot 'source/model_render_hierarchy_native.c') (Join-Path $PSScriptRoot 'source/model_render_prepare.c') (Join-Path $PSScriptRoot 'source/model_render_frame_setup.c') (Join-Path $PSScriptRoot 'source/model_render_record_loop.c') (Join-Path $PSScriptRoot 'source/model_render_record_native.c') (Join-Path $PSScriptRoot 'source/model_render_record_setup.c') (Join-Path $PSScriptRoot 'source/model_render_record_cache.c') (Join-Path $PSScriptRoot 'source/model_render_record_gate.c') (Join-Path $PSScriptRoot 'source/model_descriptor_lookup.c') (Join-Path $PSScriptRoot 'source/model_hierarchy_matrix.c') (Join-Path $PSScriptRoot 'source/model_render_single_mesh.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse.c') (Join-Path $PSScriptRoot 'source/model_matrix_inverse_singular.c') (Join-Path $PSScriptRoot 'source/model_matrix_determinant.c') (Join-Path $PSScriptRoot 'source/model_vector_render_select.c') (Join-Path $PSScriptRoot 'source/model_vector_type1_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type1.c') (Join-Path $PSScriptRoot 'source/model_vector_blend.c') (Join-Path $PSScriptRoot 'source/model_point_transform.c') (Join-Path $PSScriptRoot 'source/model_direction_transform.c') (Join-Path $PSScriptRoot 'source/model_vector_normalize.c') (Join-Path $PSScriptRoot 'source/model_vector_type2_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type2.c') (Join-Path $PSScriptRoot 'source/model_matrix_compose_implicit.c') (Join-Path $PSScriptRoot 'source/model_vector_type3_transforms.c') (Join-Path $PSScriptRoot 'source/model_vector_type3.c') (Join-Path $PSScriptRoot 'source/model_attachment_transforms.c') (Join-Path $PSScriptRoot 'source/model_attachment_route.c') (Join-Path $PSScriptRoot 'source/model_render_native.c') (Join-Path $PSScriptRoot 'source/model_vector_frame.c') (Join-Path $PSScriptRoot 'source/model_vector_render_call.c') (Join-Path $PSScriptRoot 'source/model_vector_call.c') (Join-Path $PSScriptRoot 'source/model_vector_initialize.c') (Join-Path $PSScriptRoot 'source/model_vector_save.c') (Join-Path $PSScriptRoot 'source/model_vector_cleanup.c') (Join-Path $PSScriptRoot 'source/model_vector_restore.c') (Join-Path $PSScriptRoot 'source/heap_allocate_ram.c') (Join-Path $PSScriptRoot 'source/owner_vector_runtime.c') (Join-Path $PSScriptRoot 'source/owner_cleanup_native.c') (Join-Path $PSScriptRoot 'source/owner_helper_prepare.c') (Join-Path $PSScriptRoot 'source/owner_helper_terminal.c') (Join-Path $PSScriptRoot 'source/helper_record_release.c') (Join-Path $PSScriptRoot 'source/tikal_cleanup_native.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_cleanup_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native Tikal cleanup compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_cleanup_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native Tikal cleanup comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/tikal_transition.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_transition.dll')
if ($LASTEXITCODE -ne 0) { throw 'Tikal transition build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_transition.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal transition comparison failed' }


& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/tikal_controller.c') (Join-Path $PSScriptRoot 'source/tikal_transition.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_controller.dll')
if ($LASTEXITCODE -ne 0) { throw 'Tikal controller build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_controller.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal controller comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/tikal_preupdate.c') (Join-Path $PSScriptRoot 'source/tikal_transition.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_preupdate.dll')
if ($LASTEXITCODE -ne 0) { throw 'Tikal pre-update build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_preupdate.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal pre-update comparison failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/tikal_controller_preupdate.c') (Join-Path $PSScriptRoot 'source/tikal_controller.c') (Join-Path $PSScriptRoot 'source/tikal_preupdate.c') (Join-Path $PSScriptRoot 'source/tikal_transition.c') (Join-Path $PSScriptRoot 'source/sh4_fmac.c') (Join-Path $PSScriptRoot 'source/sh4_add_sub.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_controller_preupdate.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal controller build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_controller_preupdate.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal controller comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/tikal_state_predicates.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_state_predicates.dll')
if ($LASTEXITCODE -ne 0) { throw 'Tikal predicate build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_state_predicates.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal predicate comparison failed' }
$tikalPredicateSources = @('tikal_preupdate_predicates','tikal_state_predicates','tikal_preupdate','tikal_transition','sh4_fmac','sh4_add_sub') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalPredicateSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_preupdate_predicates.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal pre-update predicate build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_preupdate_predicates.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal pre-update predicate comparison failed' }
$tikalControllerPredicateSources = @((Join-Path $PSScriptRoot 'source/tikal_controller_predicates.c'),(Join-Path $PSScriptRoot 'source/tikal_controller.c')) + $tikalPredicateSources
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalControllerPredicateSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_controller_predicates.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal controller predicate build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_controller_predicates.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal controller predicate comparison failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/tikal_b64_transition.c') (Join-Path $PSScriptRoot 'source/tikal_c20_transition.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_additional_transitions.dll')
if ($LASTEXITCODE -ne 0) { throw 'Additional Tikal transition build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_additional_transitions.py')
if ($LASTEXITCODE -ne 0) { throw 'Additional Tikal transition comparison failed' }
$tikalSubordinateSources = @('tikal_preupdate_subordinates','tikal_b64_transition','tikal_c20_transition','tikal_state_predicates','tikal_preupdate','tikal_transition','sh4_fmac','sh4_add_sub') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalSubordinateSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_preupdate_subordinates.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal subordinate build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_preupdate_subordinates.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal subordinate comparison failed' }
$tikalControllerSubordinateSources = @((Join-Path $PSScriptRoot 'source/tikal_controller_subordinates.c'),(Join-Path $PSScriptRoot 'source/tikal_controller.c')) + $tikalSubordinateSources
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalControllerSubordinateSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_controller_subordinates.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal controller subordinate build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_controller_subordinates.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal controller subordinate comparison failed' }

$tikalEventSources = @('tikal_events_native','tikal_c64_event','tikal_cd2_event') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalEventSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_event_dispatch.dll')
if ($LASTEXITCODE -ne 0) { throw 'Tikal event dispatch build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_event_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal event dispatch comparison failed' }
$tikalLocalSources = @('tikal_preupdate_local','tikal_events_native','tikal_c64_event','tikal_cd2_event','tikal_b64_transition','tikal_c20_transition','tikal_state_predicates','tikal_preupdate','tikal_transition','sh4_fmac','sh4_add_sub') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalLocalSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_preupdate_local.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native local Tikal pre-update build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_preupdate_local.py')
if ($LASTEXITCODE -ne 0) { throw 'Native local Tikal pre-update comparison failed' }
$tikalControllerLocalSources = @((Join-Path $PSScriptRoot 'source/tikal_controller_local.c'),(Join-Path $PSScriptRoot 'source/tikal_controller.c')) + $tikalLocalSources
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalControllerLocalSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_controller_local.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native local Tikal controller build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_controller_local.py')
if ($LASTEXITCODE -ne 0) { throw 'Native local Tikal controller comparison failed' }

$tikalState11Sources = @('tikal_state11','sh4_fmac','sh4_multiply','sh4_divide','sh4_add_sub','sh4_float_i32','sh4_truncate_i32') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalState11Sources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_state11.dll')
if ($LASTEXITCODE -ne 0) { throw 'Tikal state11 build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_state11.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal state11 comparison failed' }
$tikalControllerState11Sources = @('tikal_controller_state11','tikal_state11','tikal_controller_local','tikal_controller','tikal_preupdate_local','tikal_events_native','tikal_c64_event','tikal_cd2_event','tikal_b64_transition','tikal_c20_transition','tikal_state_predicates','tikal_preupdate','tikal_transition','sh4_fmac','sh4_multiply','sh4_divide','sh4_add_sub','sh4_float_i32','sh4_truncate_i32') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalControllerState11Sources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_controller_state11.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal state11 build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_controller_state11.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal state11 comparison failed' }

$tikalPositionSources = @('tikal_position_native','tikal_position','model_matrix_stack_push','model_matrix_stack_pop','model_direction_transform','sh4_add_sub','sh4_multiply','sh4_fmac') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalPositionSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_position_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Tikal position build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_position_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal position comparison failed' }
$tikalControllerPositionExtra = @('tikal_controller_position','tikal_position_native','tikal_position','model_matrix_stack_push','model_matrix_stack_pop','model_direction_transform') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
$tikalControllerPositionSources = $tikalControllerPositionExtra + $tikalControllerState11Sources
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalControllerPositionSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_controller_position.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal position build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_controller_position.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal position comparison failed' }

$tikalDisplaySources = @('tikal_display_native','tikal_display','model_matrix_stack_push','model_matrix_stack_pop','model_matrix_translate') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalDisplaySources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_display_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Tikal display build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_display_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal display comparison failed' }
$tikalControllerDisplayExtra = @('tikal_controller_display','tikal_display_native','tikal_display','model_matrix_translate') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
$tikalControllerDisplaySources = $tikalControllerDisplayExtra + $tikalControllerPositionSources
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalControllerDisplaySources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_controller_display.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal display build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_controller_display.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal display comparison failed' }

$tikalInitializerSources = @('tikal_child_initialize','tikal_effect_create','tikal_effect_apply') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalInitializerSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_initializers.dll')
if ($LASTEXITCODE -ne 0) { throw 'Tikal initializer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_initializers.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal initializer comparison failed' }
$tikalControllerInitializerExtra = @('tikal_controller_initialize','tikal_effect_create') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
$tikalControllerInitializerSources = $tikalControllerInitializerExtra + $tikalControllerDisplaySources
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalControllerInitializerSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_controller_initialize.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal initializer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_controller_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected Tikal initializer comparison failed' }

$tikalEffectUpdateSources = @('atan2_full_native','sa_atan2_entry','sa_atan2_quadrants','atan_core_native','sa_atan_core','sa_atan_reduced','sa_atan_classifier','sa_atan_scaling','sh4_divide','sh4_float_i32','scalar_sqrt_wrapper','sa_camera_position','camera_position_native','camera_matrix_prepare','sa_vector_to_angles','sh4_truncate_i32','sa_vector_add','sa_vector_subtract','sh4_add_sub','sa_matrix_rotate_x','sa_matrix_rotate_y','sa_matrix_rotate_z','sa_angle_approach','sa_effect_primary_angles','sa_effect_secondary_angles','effect_angles_native','tikal_effect_update_native','tikal_effect_update','model_channel_difference','model_matrix_stack_push','model_matrix_stack_pop','model_direction_transform','sh4_multiply','sh4_fmac') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalEffectUpdateSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_effect_update_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Tikal effect update build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_effect_update_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal effect update comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_effect_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Tikal effect lifecycle comparison failed' }

# Complete integer angle helper recovery and unhooked retail comparisons.
$effectAngleSources = @('sa_angle_approach','sa_effect_primary_angles','sa_effect_secondary_angles','effect_angles_native') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @effectAngleSources -o (Join-Path $Workspace 'work/sonic-adventure/build/effect_angles_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Effect angle helpers build failed' }
& $python (Join-Path $PSScriptRoot 'verify_effect_angles_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Effect angle helpers verification failed' }

# Complete retail axis rotation bodies; vector numerical outputs remain explicit services.
$axisRotationSources = @('sa_matrix_rotate_x','sa_matrix_rotate_y','sa_matrix_rotate_z','sa_matrix_rotate_yxz') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @axisRotationSources -o (Join-Path $Workspace 'work/sonic-adventure/build/axis_rotation_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Axis rotation build failed' }
& $python (Join-Path $PSScriptRoot 'verify_axis_rotation_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Axis rotation verification failed' }

# Native axis rotations connected through positioning, display and controller.
$tikalPositionRotationSources = @('sa_matrix_rotate_x','sa_matrix_rotate_y','sa_matrix_rotate_z','tikal_position_rotations','tikal_position','model_matrix_stack_push','model_matrix_stack_pop','model_direction_transform','sh4_add_sub','sh4_multiply','sh4_fmac') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalPositionRotationSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_position_rotations.dll')
if ($LASTEXITCODE -ne 0) { throw 'Position rotation build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_position_rotations.py')
if ($LASTEXITCODE -ne 0) { throw 'Position rotation verification failed' }
$tikalDisplayRotationSources = @('sa_matrix_rotate_x','sa_matrix_rotate_y','sa_matrix_rotate_z','tikal_display_rotations','tikal_display','model_matrix_stack_push','model_matrix_stack_pop','model_matrix_translate') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalDisplayRotationSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_display_rotations.dll')
if ($LASTEXITCODE -ne 0) { throw 'Display rotation build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_display_rotations.py')
if ($LASTEXITCODE -ne 0) { throw 'Display rotation verification failed' }
$tikalControllerRotationSourceNames = Get-Content (Join-Path $PSScriptRoot 'analysis/tikal_controller_rotations_sources.json') -Raw | ConvertFrom-Json
$tikalControllerRotationSources = @($tikalControllerRotationSourceNames | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_) })
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @tikalControllerRotationSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_controller_rotations.dll')
if ($LASTEXITCODE -ne 0) { throw 'Controller rotation build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_controller_rotations.py')
if ($LASTEXITCODE -ne 0) { throw 'Controller rotation verification failed' }

# Vector add/subtract leaves with recovered scalar binary32 arithmetic.
$vectorAddSubtractSources = @('sa_vector_add','sa_vector_subtract','sh4_add_sub') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @vectorAddSubtractSources -o (Join-Path $Workspace 'work/sonic-adventure/build/vector_add_subtract.dll')
if ($LASTEXITCODE -ne 0) { throw 'Vector add/subtract build failed' }
& $python (Join-Path $PSScriptRoot 'verify_vector_add_subtract.py')
if ($LASTEXITCODE -ne 0) { throw 'Vector add/subtract verification failed' }

# Whole vector-to-angle routine with sqrt/atan2 services explicit.
$vectorToAnglesSources = @('sa_vector_to_angles','sh4_multiply','sh4_fmac','sh4_truncate_i32') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @vectorToAnglesSources -o (Join-Path $Workspace 'work/sonic-adventure/build/vector_to_angles.dll')
if ($LASTEXITCODE -ne 0) { throw 'Vector-to-angle build failed' }
& $python (Join-Path $PSScriptRoot 'verify_vector_to_angles.py')
if ($LASTEXITCODE -ne 0) { throw 'Vector-to-angle verification failed' }

# Camera vector transform, identity matrix prepare and native subordinate bodies.
$cameraPositionSources = @('sa_camera_position','camera_position_native','camera_matrix_prepare','sa_matrix_rotate_x','sa_matrix_rotate_y','sa_matrix_rotate_z','model_matrix_stack_pop','model_direction_transform','sh4_multiply','sh4_fmac') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @cameraPositionSources -o (Join-Path $Workspace 'work/sonic-adventure/build/camera_position_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Camera vector transform build failed' }
& $python (Join-Path $PSScriptRoot 'verify_camera_position_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Camera vector transform verification failed' }

# Retail signed square-root wrapper; instruction numerical service explicit.
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/scalar_sqrt_wrapper.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/scalar_sqrt_wrapper.dll')
if ($LASTEXITCODE -ne 0) { throw 'Signed sqrt wrapper build failed' }
& $python (Join-Path $PSScriptRoot 'verify_scalar_sqrt_wrapper.py')
if ($LASTEXITCODE -ne 0) { throw 'Signed sqrt wrapper verification failed' }

# Atan2 public entry and nonfinite result/error paths; numerical core explicit.
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_atan2_entry.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/atan2_entry.dll')
if ($LASTEXITCODE -ne 0) { throw 'Atan2 entry build failed' }
& $python (Join-Path $PSScriptRoot 'verify_atan2_entry.py')
if ($LASTEXITCODE -ne 0) { throw 'Atan2 entry verification failed' }

# Atan2 quotient/quadrant body and connected public entry; atan core explicit.
$atan2QuadrantSources = @('sa_atan2_quadrants','sh4_divide','sh4_add_sub') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @atan2QuadrantSources -o (Join-Path $Workspace 'work/sonic-adventure/build/atan2_quadrants.dll')
if ($LASTEXITCODE -ne 0) { throw 'Atan2 quadrant build failed' }
& $python (Join-Path $PSScriptRoot 'verify_atan2_quadrants.py')
if ($LASTEXITCODE -ne 0) { throw 'Atan2 quadrant verification failed' }
$atan2ConnectedSources = @((Join-Path $PSScriptRoot 'source/sa_atan2_entry.c'),(Join-Path $PSScriptRoot 'source/atan2_entry_native.c')) + $atan2QuadrantSources
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @atan2ConnectedSources -o (Join-Path $Workspace 'work/sonic-adventure/build/atan2_entry_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected atan2 build failed' }
& $python (Join-Path $PSScriptRoot 'verify_atan2_entry_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected atan2 verification failed' }

# Complete atan body; classifier, reduced approximation and scaling helpers explicit.
$atanCoreSources = @('sa_atan_core','sh4_divide','sh4_add_sub','sh4_fmac') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @atanCoreSources -o (Join-Path $Workspace 'work/sonic-adventure/build/atan_core.dll')
if ($LASTEXITCODE -ne 0) { throw 'Atan core build failed' }
& $python (Join-Path $PSScriptRoot 'verify_atan_core.py')
if ($LASTEXITCODE -ne 0) { throw 'Atan core verification failed' }

# Reduced atan polynomial with recovered scalar arithmetic and outer atan connection.
$atanReducedSources = @('sa_atan_reduced','sh4_multiply','sh4_fmac') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @atanReducedSources -o (Join-Path $Workspace 'work/sonic-adventure/build/atan_reduced.dll')
if ($LASTEXITCODE -ne 0) { throw 'Reduced atan build failed' }
& $python (Join-Path $PSScriptRoot 'verify_atan_reduced.py')
if ($LASTEXITCODE -ne 0) { throw 'Reduced atan verification failed' }
$atanCoreReducedSources = @('atan_core_reduced','sa_atan_core','sa_atan_reduced','sh4_divide','sh4_add_sub','sh4_multiply','sh4_fmac') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @atanCoreReducedSources -o (Join-Path $Workspace 'work/sonic-adventure/build/atan_core_reduced.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected reduced atan build failed' }
& $python (Join-Path $PSScriptRoot 'verify_atan_core_reduced.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected reduced atan verification failed' }

# Native atan quotient/remainder helper and actual classifier connection.
$atanClassifierSources = @('sa_atan_classifier','sh4_divide','sh4_add_sub','sh4_float_i32','sh4_truncate_i32','sh4_multiply','sh4_fmac') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @atanClassifierSources -o (Join-Path $Workspace 'work/sonic-adventure/build/atan_classifier.dll')
if ($LASTEXITCODE -ne 0) { throw 'Atan quotient/remainder build failed' }
& $python (Join-Path $PSScriptRoot 'verify_atan_classifier.py')
if ($LASTEXITCODE -ne 0) { throw 'Atan quotient/remainder verification failed' }
$atanClassifiedSources = @('atan_core_classified','sa_atan_core','sa_atan_reduced','sa_atan_classifier','sh4_divide','sh4_add_sub','sh4_multiply','sh4_fmac','sh4_float_i32','sh4_truncate_i32') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @atanClassifiedSources -o (Join-Path $Workspace 'work/sonic-adventure/build/atan_core_classified.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native classified atan build failed' }
& $python (Join-Path $PSScriptRoot 'verify_atan_core_classified.py')
if ($LASTEXITCODE -ne 0) { throw 'Native classified atan verification failed' }

# Scaling leaf and fully native finite atan/atan2 arithmetic chain.
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_atan_scaling.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/atan_scaling.dll')
if ($LASTEXITCODE -ne 0) { throw 'Atan scaling build failed' }
& $python (Join-Path $PSScriptRoot 'verify_atan_scaling.py')
if ($LASTEXITCODE -ne 0) { throw 'Atan scaling verification failed' }
$atanNativeSources = @('atan_core_native','sa_atan_core','sa_atan_reduced','sa_atan_classifier','sa_atan_scaling','sh4_divide','sh4_add_sub','sh4_multiply','sh4_fmac','sh4_float_i32','sh4_truncate_i32') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @atanNativeSources -o (Join-Path $Workspace 'work/sonic-adventure/build/atan_core_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native atan build failed' }
& $python (Join-Path $PSScriptRoot 'verify_atan_core_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native atan verification failed' }
$atan2NativeSources = @((Join-Path $PSScriptRoot 'source/atan2_full_native.c'),(Join-Path $PSScriptRoot 'source/sa_atan2_entry.c'),(Join-Path $PSScriptRoot 'source/sa_atan2_quadrants.c')) + $atanNativeSources
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @atan2NativeSources -o (Join-Path $Workspace 'work/sonic-adventure/build/atan2_full_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native atan2 build failed' }
& $python (Join-Path $PSScriptRoot 'verify_atan2_full_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native atan2 verification failed' }

# Manual-derived finite zero-divisor FDIV and V/Z exception preservation vectors.
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sh4_divide.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/sh4_divide_zero.dll')
if ($LASTEXITCODE -ne 0) { throw 'Zero-divisor FDIV build failed' }
& $python (Join-Path $PSScriptRoot 'verify_native_sh4_divide_zero.py')
if ($LASTEXITCODE -ne 0) { throw 'Zero-divisor FDIV verification failed' }

# Manual-derived nonfinite/signed-zero FCMP vectors.
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sh4_compare.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/sh4_compare.dll')
if ($LASTEXITCODE -ne 0) { throw 'SH-4 comparison build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sh4_compare.py')
if ($LASTEXITCODE -ne 0) { throw 'SH-4 comparison verification failed' }

# Independent manual-derived NaN arithmetic vectors using the native add/sub DLL.
& $python (Join-Path $PSScriptRoot 'verify_native_sh4_add_sub_nan.py')
if ($LASTEXITCODE -ne 0) { throw 'NaN add/sub verification failed' }

# Manual-derived NaN multiplication and enabled exception vectors.
& $python (Join-Path $PSScriptRoot 'verify_native_sh4_multiply_nan.py')
if ($LASTEXITCODE -ne 0) { throw 'NaN multiplication verification failed' }

# State11 input movement connected to the full recovered native atan2 chain.
$state11AtanSources = @('sa_position_task_create','sa_signed_division','sa_counter_award','sa_stage_mode_query','sa_counter_add','sa_counter_get','sa_timer_set','sa_timer_get','sa_saved_position_reset','sa_stage_act_key','stage_position_restore_saved','sa_saved_position_available','sa_saved_position_load','sa_saved_position_store','sa_stage_position_restore','scalar_sqrt_wrapper','angle_trig_wrappers','tikal_state11_atan2','tikal_state11','atan2_full_native','sa_atan2_entry','sa_atan2_quadrants','atan_core_native','sa_atan_core','sa_atan_reduced','sa_atan_classifier','sa_atan_scaling','sh4_divide','sh4_add_sub','sh4_multiply','sh4_fmac','sh4_float_i32','sh4_truncate_i32') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @state11AtanSources -o (Join-Path $Workspace 'work/sonic-adventure/build/tikal_state11_atan2.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native state11 atan2 build failed' }
& $python (Join-Path $PSScriptRoot 'verify_tikal_state11_atan2.py')
if ($LASTEXITCODE -ne 0) { throw 'Native state11 atan2 verification failed' }

# Retail unsigned16-angle sine/cosine instruction wrappers.
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/angle_trig_wrappers.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/angle_trig_wrappers.dll')
if ($LASTEXITCODE -ne 0) { throw 'Angle trig wrappers build failed' }
& $python (Join-Path $PSScriptRoot 'verify_angle_trig_wrappers.py')
if ($LASTEXITCODE -ne 0) { throw 'Angle trig wrapper verification failed.' }
& $python (Join-Path $PSScriptRoot 'register_angle_trig_wrappers.py')
if ($LASTEXITCODE -ne 0) { throw 'Angle trig wrapper registration failed.' }
if ($LASTEXITCODE -ne 0) { throw 'Angle trig wrappers verification failed' }

# Complete stage position restoration body, including branches beyond the indexed span.
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_stage_position_restore.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/stage_position_restore.dll')
if ($LASTEXITCODE -ne 0) { throw 'Stage position restoration build failed' }
& $python (Join-Path $PSScriptRoot 'verify_stage_position_restore.py')
if ($LASTEXITCODE -ne 0) { throw 'Stage position restoration verification failed' }

# Saved position availability, load and store dependencies.
$savedPositionSources = @('sa_saved_position_available','sa_saved_position_load','sa_saved_position_store') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @savedPositionSources -o (Join-Path $Workspace 'work/sonic-adventure/build/saved_position.dll')
if ($LASTEXITCODE -ne 0) { throw 'Saved position dependencies build failed' }
& $python (Join-Path $PSScriptRoot 'verify_saved_position.py')
if ($LASTEXITCODE -ne 0) { throw 'Saved position dependencies verification failed' }

# Restoration parent connected to native saved-position helpers.
$restoreSavedSources = @('sa_position_task_create','sa_signed_division','sa_counter_award','sa_stage_mode_query','sa_counter_add','sa_counter_get','sa_timer_set','sa_timer_get','sa_saved_position_reset','sa_stage_act_key','stage_position_restore_saved','sa_stage_position_restore','sa_saved_position_available','sa_saved_position_load','sa_saved_position_store') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @restoreSavedSources -o (Join-Path $Workspace 'work/sonic-adventure/build/stage_position_restore_saved.dll')
if ($LASTEXITCODE -ne 0) { throw 'Saved position restoration integration build failed' }
& $python (Join-Path $PSScriptRoot 'verify_stage_position_restore_saved.py')
if ($LASTEXITCODE -ne 0) { throw 'Saved position restoration integration verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_stage_act_key.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/stage_act_key.dll')
if ($LASTEXITCODE -ne 0) { throw 'Stage act key build failed' }
& $python (Join-Path $PSScriptRoot 'verify_stage_act_key.py')
if ($LASTEXITCODE -ne 0) { throw 'Stage act key verification failed' }

# Exact 36-byte saved-position reset loop.
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_saved_position_reset.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/saved_position_reset.dll')
if ($LASTEXITCODE -ne 0) { throw 'Saved position reset build failed' }
& $python (Join-Path $PSScriptRoot 'verify_saved_position_reset.py')
if ($LASTEXITCODE -ne 0) { throw 'Saved position reset verification failed' }

# Exact timer-byte setter/getter used by saved-position load/store.
$timerFieldSources = @('sa_timer_set','sa_timer_get') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @timerFieldSources -o (Join-Path $Workspace 'work/sonic-adventure/build/timer_fields.dll')
if ($LASTEXITCODE -ne 0) { throw 'Timer field helpers build failed' }
& $python (Join-Path $PSScriptRoot 'verify_timer_fields.py')
if ($LASTEXITCODE -ne 0) { throw 'Timer field helpers verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_counter_get.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/counter_get.dll')
if ($LASTEXITCODE -ne 0) { throw 'Counter getter build failed' }
& $python (Join-Path $PSScriptRoot 'verify_counter_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Counter getter verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_counter_add.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/counter_add.dll')
if ($LASTEXITCODE -ne 0) { throw 'Counter update build failed' }
& $python (Join-Path $PSScriptRoot 'verify_counter_add.py')
if ($LASTEXITCODE -ne 0) { throw 'Counter update verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_stage_mode_query.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/stage_mode_query.dll')
if ($LASTEXITCODE -ne 0) { throw 'Stage mode query build failed' }
& $python (Join-Path $PSScriptRoot 'verify_stage_mode_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Stage mode query verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_counter_award.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/counter_award.dll')
if ($LASTEXITCODE -ne 0) { throw 'Counter award build failed' }
& $python (Join-Path $PSScriptRoot 'verify_counter_award.py')
if ($LASTEXITCODE -ne 0) { throw 'Counter award verification failed' }

# Counter update connected to award and native mode query; uses restoration DLL.
& $python (Join-Path $PSScriptRoot 'verify_counter_add_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native counter award integration verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_signed_division.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/signed_division.dll')
if ($LASTEXITCODE -ne 0) { throw 'Signed division build failed' }
& $python (Join-Path $PSScriptRoot 'verify_signed_division.py')
if ($LASTEXITCODE -ne 0) { throw 'Signed division verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_position_task_create.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/position_task_create.dll')
if ($LASTEXITCODE -ne 0) { throw 'Position task creation build failed' }
& $python (Join-Path $PSScriptRoot 'verify_position_task_create.py')
if ($LASTEXITCODE -ne 0) { throw 'Position task creation verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_position_task_callback.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/position_task_callback.dll')
if ($LASTEXITCODE -ne 0) { throw 'Position task callback build failed' }
& $python (Join-Path $PSScriptRoot 'verify_position_task_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Position task callback verification failed' }

$positionCallbackSources = @('sa_position_task_callback','sa_task_remove_request','position_task_callback_native') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @positionCallbackSources -o (Join-Path $Workspace 'work/sonic-adventure/build/position_task_callback_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native position task callback build failed' }
& $python (Join-Path $PSScriptRoot 'verify_position_task_callback_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native position task callback verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_task_delete.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/task_delete.dll')
if ($LASTEXITCODE -ne 0) { throw 'Task deletion build failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_delete.py')
if ($LASTEXITCODE -ne 0) { throw 'Task deletion verification failed' }

$taskDeleteNativeSources = @('sa_setup_callback_register','sa_setup_callback_state','sa_setup_counter_set','sa_engine_setup','sa_interrupt_callback_create','sa_interrupt_callback_remove','sa_interrupt_handler_primary','sa_interrupt_handler_secondary','sa_interrupt_handler_set','sa_interrupt_handler_reset','sa_interrupt_dispatch_primary','sa_interrupt_dispatch_secondary','sa_interrupt_gameplay_callback','sa_stage_act_key','sa_interrupt_service_643f98','sa_interrupt_input_events','sa_interrupt_tail_dispatch','sa_interrupt_job_query','sa_interrupt_job_dispatch','sa_input_press_primary','sa_input_press_alternate','sa_input_channel_state_get','sa_input_channel_event_set','sa_input_channel_request_set','sa_input_channel_available','sa_input_sample_mask','sa_input_channel_query','sa_input_channel_status_pointer','sa_input_channel_info_pointer','sa_input_device_info_pointer','sa_input_channel_callback','sa_input_channel_secondary_callback','sa_input_frame_process','sa_input_frame_backend','sa_input_frame_manager','sa_input_work_dispatch','sa_input_context_save','sa_input_context_restore','sa_input_work_yield','input_frame_context_native','sa_input_work_create','sa_input_work_object_create','sa_input_work_pool_initialize','sa_input_work_backend_initialize','sa_input_work_backend_post','sa_input_channel_worker','sa_input_channels_initialize','sa_memory_move','sa_memory_fill','sa_task_delete_regular','sa_task_delete_all','sa_task_delete_list','sa_task_update_all','sa_task_pool_initialize','sa_heap_initialize','sa_position_task_create','sa_heap_allocate','sa_heap_allocate_zeroed','sa_task_create','sa_task_create_base','sa_task_update_list','sa_position_task_callback','sa_task_remove_request','sa_task_delete','sa_task_children_delete','sa_task_children_update','sa_task_data_release','sa_heap_release','sa_task_optional_cleanup','sa_task_cleanup_list','sa_task_cleanup_secondary','sa_task_payload_cleanup','task_delete_native') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
$taskDeleteNativeSources += (Join-Path $PSScriptRoot 'source/sa_input_request_completion.c'), (Join-Path $PSScriptRoot 'source/sa_input_request_completion_impl.c'), (Join-Path $PSScriptRoot 'source/sa_input_device_record_pointer.c'), (Join-Path $PSScriptRoot 'source/sa_input_device_busy_read.c'), (Join-Path $PSScriptRoot 'source/sa_input_device_request_mark.c'), (Join-Path $PSScriptRoot 'source/sa_input_device_busy.c'), (Join-Path $PSScriptRoot 'source/sa_input_device_mark.c'), (Join-Path $PSScriptRoot 'source/sa_input_device_available.c'), (Join-Path $PSScriptRoot 'source/sa_input_backend_finalize.c'), (Join-Path $PSScriptRoot 'source/sa_input_packet_row_copy.c'), (Join-Path $PSScriptRoot 'source/sa_input_device_backend_check.c'), (Join-Path $PSScriptRoot 'source/sa_input_backend_request.c'), (Join-Path $PSScriptRoot 'source/sa_input_device_request.c'), (Join-Path $PSScriptRoot 'source/sa_input_press_service.c'), (Join-Path $PSScriptRoot 'source/sa_input_event_service.c'), (Join-Path $PSScriptRoot 'source/sa_input_event_error.c')
$taskDeleteNativeSources += @((Join-Path $PSScriptRoot 'source/sa_input_request_allocate.c'),(Join-Path $PSScriptRoot 'source/sa_input_request_lock.c'),(Join-Path $PSScriptRoot 'source/sa_input_request_pack.c'),(Join-Path $PSScriptRoot 'source/input_request_allocate_native.c'),(Join-Path $PSScriptRoot 'source/sa_input_request_allocate_simple.c'),(Join-Path $PSScriptRoot 'source/sa_input_request_simple_lock.c'),(Join-Path $PSScriptRoot 'source/sa_input_request_simple_pack.c'),(Join-Path $PSScriptRoot 'source/input_request_allocate_simple_native.c'),(Join-Path $PSScriptRoot 'source/sa_input_request_queue.c'),(Join-Path $PSScriptRoot 'source/sa_input_queue_address.c'),(Join-Path $PSScriptRoot 'source/sa_input_queue_flush.c'),(Join-Path $PSScriptRoot 'source/input_request_queue_native.c'))
$taskDeleteNativeSources += @((Join-Path $PSScriptRoot 'source/sa_input_device_metadata.c'),(Join-Path $PSScriptRoot 'source/sa_input_metadata_locator.c'),(Join-Path $PSScriptRoot 'source/input_device_metadata_native.c'),(Join-Path $PSScriptRoot 'source/sa_signed_division.c'))
$taskDeleteNativeSources += @((Join-Path $PSScriptRoot 'source/sa_input_packet_read_be.c'),(Join-Path $PSScriptRoot 'source/sa_input_packet_read_le.c'))
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @taskDeleteNativeSources -o (Join-Path $Workspace 'work/sonic-adventure/build/task_delete_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Recursive task deletion build failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_delete_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Recursive task deletion verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_heap_release.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/heap_release.dll')
if ($LASTEXITCODE -ne 0) { throw 'Heap release build failed' }
& $python (Join-Path $PSScriptRoot 'verify_heap_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Heap release verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_task_update_list.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/task_update_list.dll')
if ($LASTEXITCODE -ne 0) { throw 'Task update list build failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_update_list.py')
if ($LASTEXITCODE -ne 0) { throw 'Task update list verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_task_position_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Native position task lifecycle verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_task_create_base.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/task_create_base.dll')
if ($LASTEXITCODE -ne 0) { throw 'Base task creation build failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_create_base.py')
if ($LASTEXITCODE -ne 0) { throw 'Base task creation verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_task_create_heap.py')
if ($LASTEXITCODE -ne 0) { throw 'Native task creation verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_heap_allocate_zeroed.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/heap_allocate_zeroed.dll')
if ($LASTEXITCODE -ne 0) { throw 'Zeroed heap allocation build failed' }
& $python (Join-Path $PSScriptRoot 'verify_heap_allocate_zeroed.py')
if ($LASTEXITCODE -ne 0) { throw 'Zeroed heap allocation verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_heap_allocate.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/heap_allocate.dll')
if ($LASTEXITCODE -ne 0) { throw 'Core heap allocation build failed' }
& $python (Join-Path $PSScriptRoot 'verify_heap_allocate.py')
if ($LASTEXITCODE -ne 0) { throw 'Core heap allocation verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_task_position_created_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Created position task lifecycle verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_heap_initialize.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/heap_initialize.dll')
if ($LASTEXITCODE -ne 0) { throw 'Heap initialization build failed' }
& $python (Join-Path $PSScriptRoot 'verify_heap_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Heap initialization verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_task_position_initialized_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Initialized heap position task lifecycle verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_task_pool_initialize.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/task_pool_initialize.dll')
if ($LASTEXITCODE -ne 0) { throw 'Task pool initialization build failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_pool_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Task pool initialization verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_task_position_bootstrapped_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Heap and task pool lifecycle verification failed' }

$taskDisplaySources = @('sa_task_display_all','sa_task_display_list','sa_task_display_children','task_display_native') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @taskDisplaySources -o (Join-Path $Workspace 'work/sonic-adventure/build/task_display_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Task display traversal build failed' }
& $python (Join-Path $PSScriptRoot 'verify_task_display_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Task display traversal verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_task_children_delete_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Deferred child task verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_task_update_all.py')
if ($LASTEXITCODE -ne 0) { throw 'All priority task update verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_task_display_all.py')
if ($LASTEXITCODE -ne 0) { throw 'All priority task display verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_task_delete_list.py')
if ($LASTEXITCODE -ne 0) { throw 'Priority task cleanup verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_task_delete_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Cleanup dispatcher verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_setup_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine setup initialization integration verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_interrupt_callback_create.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/interrupt_constructor.dll')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt constructor build failed' }
& $python (Join-Path $PSScriptRoot 'verify_interrupt_constructor.py')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt constructor verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_setup_interrupt_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine interrupt registration integration verification failed' }


& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_interrupt_callback_remove.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/interrupt_remove.dll')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt removal build failed' }
& $python (Join-Path $PSScriptRoot 'verify_interrupt_remove.py')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt removal verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_interrupt_handlers.py')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt handler installation verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_interrupt_dispatch_primary.c') (Join-Path $PSScriptRoot 'source/sa_interrupt_dispatch_secondary.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/interrupt_dispatch.dll')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt dispatch build failed' }
& $python (Join-Path $PSScriptRoot 'verify_interrupt_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt dispatch verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_interrupt_gameplay_callback.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/interrupt_gameplay.dll')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt gameplay callback build failed' }
& $python (Join-Path $PSScriptRoot 'verify_interrupt_gameplay_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt gameplay callback verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_interrupt_gameplay_callback_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected interrupt gameplay callback verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_interrupt_service_643f98.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/interrupt_service_643f98.dll')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt lower service build failed' }
& $python (Join-Path $PSScriptRoot 'verify_interrupt_service_643f98.py')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt lower service verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_interrupt_input_events.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/interrupt_input_events.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input event build failed' }
& $python (Join-Path $PSScriptRoot 'verify_interrupt_input_events.py')
if ($LASTEXITCODE -ne 0) { throw 'Input event verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_interrupt_tail_dispatch.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/interrupt_tail_dispatch.dll')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt tail dispatch build failed' }
& $python (Join-Path $PSScriptRoot 'verify_interrupt_tail_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt tail dispatch verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_interrupt_job_query.c') (Join-Path $PSScriptRoot 'source/sa_interrupt_job_dispatch.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/interrupt_jobs.dll')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt job wrappers build failed' }
& $python (Join-Path $PSScriptRoot 'verify_interrupt_jobs.py')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt job wrappers verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_press_primary.c') (Join-Path $PSScriptRoot 'source/sa_input_press_alternate.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_press.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input press helpers build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_press.py')
if ($LASTEXITCODE -ne 0) { throw 'Input press helpers verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_interrupt_input_events_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected input event verification failed' }

$inputRecordSources=@('state_get','event_set','request_set') | ForEach-Object { Join-Path $PSScriptRoot ('source/sa_input_channel_'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @inputRecordSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_channel_records.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input channel record build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channel_records.py')
if ($LASTEXITCODE -ne 0) { throw 'Input channel record verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_channel_available.py')
if ($LASTEXITCODE -ne 0) { throw 'Input channel availability verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_sample_mask.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_sample_mask.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input mask sampling build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_sample_mask.py')
if ($LASTEXITCODE -ne 0) { throw 'Input mask sampling verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_channel_query.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_channel_query.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input channel query build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channel_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Input channel query verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channel_query_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected input channel query verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_channel_callback.c') (Join-Path $PSScriptRoot 'source/sa_input_channel_secondary_callback.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_channel_callbacks.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input channel callbacks build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channel_callbacks.py')
if ($LASTEXITCODE -ne 0) { throw 'Input channel callbacks verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_frame_manager.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_frame_manager.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input frame manager build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_frame_manager.py')
if ($LASTEXITCODE -ne 0) { throw 'Input frame manager verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_interrupt_input_events_work_queue.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected input work queue verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_work_dispatch.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_work_dispatch.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input work dispatcher build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_work_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Input work dispatcher verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_context_save.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_context_save.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input context save build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_context_save.py')
if ($LASTEXITCODE -ne 0) { throw 'Input context save verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_context_restore.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_context_restore.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input context restore build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_context_restore.py')
if ($LASTEXITCODE -ne 0) { throw 'Input context restore verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_context_roundtrip.py')
if ($LASTEXITCODE -ne 0) { throw 'Input context roundtrip verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_frame_manager.c') (Join-Path $PSScriptRoot 'source/sa_input_context_save.c') (Join-Path $PSScriptRoot 'source/sa_input_context_restore.c') (Join-Path $PSScriptRoot 'source/sa_input_work_dispatch.c') (Join-Path $PSScriptRoot 'source/sa_input_work_yield.c') (Join-Path $PSScriptRoot 'source/sa_input_work_create.c') (Join-Path $PSScriptRoot 'source/sa_input_work_object_create.c') (Join-Path $PSScriptRoot 'source/sa_input_work_pool_initialize.c') (Join-Path $PSScriptRoot 'source/sa_input_work_backend_initialize.c') (Join-Path $PSScriptRoot 'source/sa_input_work_backend_post.c') (Join-Path $PSScriptRoot 'source/sa_input_channel_worker.c') (Join-Path $PSScriptRoot 'source/sa_input_channels_initialize.c') (Join-Path $PSScriptRoot 'source/sa_memory_move.c') (Join-Path $PSScriptRoot 'source/sa_memory_fill.c') (Join-Path $PSScriptRoot 'source/sa_input_event_service.c') (Join-Path $PSScriptRoot 'source/sa_input_event_error.c') (Join-Path $PSScriptRoot 'source/sa_input_channel_callback.c') (Join-Path $PSScriptRoot 'source/sa_input_channel_secondary_callback.c') (Join-Path $PSScriptRoot 'source/sa_input_press_service.c') (Join-Path $PSScriptRoot 'source/sa_input_device_request.c') (Join-Path $PSScriptRoot 'source/sa_input_backend_request.c') (Join-Path $PSScriptRoot 'source/sa_input_channel_status_pointer.c') (Join-Path $PSScriptRoot 'source/sa_input_channel_info_pointer.c') (Join-Path $PSScriptRoot 'source/sa_input_device_info_pointer.c') (Join-Path $PSScriptRoot 'source/sa_input_device_backend_check.c') (Join-Path $PSScriptRoot 'source/sa_input_backend_finalize.c') (Join-Path $PSScriptRoot 'source/sa_input_packet_row_copy.c') (Join-Path $PSScriptRoot 'source/sa_input_device_available.c') (Join-Path $PSScriptRoot 'source/sa_input_device_record_pointer.c') (Join-Path $PSScriptRoot 'source/sa_input_device_busy_read.c') (Join-Path $PSScriptRoot 'source/sa_input_device_request_mark.c') (Join-Path $PSScriptRoot 'source/sa_input_device_busy.c') (Join-Path $PSScriptRoot 'source/sa_input_device_mark.c') (Join-Path $PSScriptRoot 'source/sa_input_request_completion.c') (Join-Path $PSScriptRoot 'source/sa_input_request_completion_impl.c') (Join-Path $PSScriptRoot 'source/input_frame_context_native.c') (Join-Path $PSScriptRoot 'source/sa_input_request_allocate.c') (Join-Path $PSScriptRoot 'source/sa_input_request_lock.c') (Join-Path $PSScriptRoot 'source/sa_input_request_pack.c') (Join-Path $PSScriptRoot 'source/input_request_allocate_native.c') (Join-Path $PSScriptRoot 'source/sa_input_request_allocate_simple.c') (Join-Path $PSScriptRoot 'source/sa_input_request_simple_lock.c') (Join-Path $PSScriptRoot 'source/sa_input_request_simple_pack.c') (Join-Path $PSScriptRoot 'source/input_request_allocate_simple_native.c') (Join-Path $PSScriptRoot 'source/sa_input_request_queue.c') (Join-Path $PSScriptRoot 'source/sa_input_queue_address.c') (Join-Path $PSScriptRoot 'source/sa_input_queue_flush.c') (Join-Path $PSScriptRoot 'source/input_request_queue_native.c') (Join-Path $PSScriptRoot 'source/sa_input_device_metadata.c') (Join-Path $PSScriptRoot 'source/sa_input_metadata_locator.c') (Join-Path $PSScriptRoot 'source/input_device_metadata_native.c') (Join-Path $PSScriptRoot 'source/sa_signed_division.c') (Join-Path $PSScriptRoot 'source/sa_input_packet_read_be.c') (Join-Path $PSScriptRoot 'source/sa_input_packet_read_le.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_frame_context_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native frame context build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_frame_context_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native frame context verification failed' }




& $python (Join-Path $PSScriptRoot 'verify_input_frame_context_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Native frame transfer verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_frame_resume.py')
if ($LASTEXITCODE -ne 0) { throw 'Native frame continuation verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_frame_restore_resume.py')
if ($LASTEXITCODE -ne 0) { throw 'Native frame restore/resume verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_work_yield.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_work_yield.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input work yield build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_work_yield.py')
if ($LASTEXITCODE -ne 0) { throw 'Input work yield verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_work_suspension.py')
if ($LASTEXITCODE -ne 0) { throw 'Native work suspension verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_work_resume.py')
if ($LASTEXITCODE -ne 0) { throw 'Native work resume verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_work_auto_resume.py')
if ($LASTEXITCODE -ne 0) { throw 'Automatic work resume verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_interrupt_input_events_extended.py')
if ($LASTEXITCODE -ne 0) { throw 'Extended input event verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_interrupt_input_events_extended_queue.py')
if ($LASTEXITCODE -ne 0) { throw 'Extended input queue verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_interrupt_input_events_due_work.py')
if ($LASTEXITCODE -ne 0) { throw 'Input due work verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_work_create.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_work_create.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input work creation build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_work_create.py')
if ($LASTEXITCODE -ne 0) { throw 'Input work creation verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_memory_move.c') (Join-Path $PSScriptRoot 'source/sa_memory_fill.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/work_memory_services.dll')
if ($LASTEXITCODE -ne 0) { throw 'Work memory services build failed' }
& $python (Join-Path $PSScriptRoot 'verify_work_memory_services.py')
if ($LASTEXITCODE -ne 0) { throw 'Work memory services verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_work_create_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native work creation verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_work_create_schedule.py')
if ($LASTEXITCODE -ne 0) { throw 'Native work scheduling verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_work_object_create.py')
if ($LASTEXITCODE -ne 0) { throw 'Native work object verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_work_pool_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Native work pool verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_work_initialized_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Initialized work lifecycle verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_work_backend_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Work backend lifecycle verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_channel_worker.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_channel_worker.dll')
if ($LASTEXITCODE -ne 0) { throw 'Channel worker build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channel_worker.py')
if ($LASTEXITCODE -ne 0) { throw 'Channel worker verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channel_worker_paths.py')
if ($LASTEXITCODE -ne 0) { throw 'Channel worker paths verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channel_worker_yield.py')
if ($LASTEXITCODE -ne 0) { throw 'Native channel worker yield verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channel_worker_resume.py')
if ($LASTEXITCODE -ne 0) { throw 'Native channel worker resume verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channel_worker_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Native channel worker lifecycle verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channels_initialized_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Initialized channels lifecycle verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channels_multichannel.py')
if ($LASTEXITCODE -ne 0) { throw 'Multichannel lifecycle verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channels_initialized_input.py')
if ($LASTEXITCODE -ne 0) { throw 'Initialized input graph verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_event_service.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_event_service.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input event service build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_event_service.py')
if ($LASTEXITCODE -ne 0) { throw 'Input event service verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_event_error.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_event_error.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input event error build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_event_error.py')
if ($LASTEXITCODE -ne 0) { throw 'Input event error verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channel_worker_event.py')
if ($LASTEXITCODE -ne 0) { throw 'Native worker event verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_press_service.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_press_service.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input press service build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_press_service.py')
if ($LASTEXITCODE -ne 0) { throw 'Input press service verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channel_worker_press.py')
if ($LASTEXITCODE -ne 0) { throw 'Native worker press verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_device_request.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_request.dll')
if ($LASTEXITCODE -ne 0) { throw 'Device request build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_request.py')
if ($LASTEXITCODE -ne 0) { throw 'Device request verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channel_worker_request.py')
if ($LASTEXITCODE -ne 0) { throw 'Native worker request verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_backend_request.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_backend_request.dll')
if ($LASTEXITCODE -ne 0) { throw 'Backend request build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_backend_request.py')
if ($LASTEXITCODE -ne 0) { throw 'Backend request verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_backend_packet.py')
if ($LASTEXITCODE -ne 0) { throw 'Backend packet verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_backend_resume.py')
if ($LASTEXITCODE -ne 0) { throw 'Backend continuation verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_backend_context.py')
if ($LASTEXITCODE -ne 0) { throw 'Backend saved-context verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_backend_wait_context.py')
if ($LASTEXITCODE -ne 0) { throw 'Backend wait saved-context verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_request_resume.py')
if ($LASTEXITCODE -ne 0) { throw 'Device request caller continuation verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_press_service_resume.py')
if ($LASTEXITCODE -ne 0) { throw 'Press caller continuation verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_worker_nested_backend.py')
if ($LASTEXITCODE -ne 0) { throw 'Nested worker backend verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channels_initialized_backend.py')
if ($LASTEXITCODE -ne 0) { throw 'Initialized backend lifecycle verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_channels_queued_backend.py')
if ($LASTEXITCODE -ne 0) { throw 'Queued input backend verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_device_backend_check.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_backend_check.dll')
if ($LASTEXITCODE -ne 0) { throw 'Device backend check build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_backend_check.py')
if ($LASTEXITCODE -ne 0) { throw 'Device backend check verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_press_validated.py')
if ($LASTEXITCODE -ne 0) { throw 'Validated press verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_backend_finalize.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_backend_finalize.dll')
if ($LASTEXITCODE -ne 0) { throw 'Backend finalize build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_backend_finalize.py')
if ($LASTEXITCODE -ne 0) { throw 'Backend finalize verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_packet_row_copy.c') (Join-Path $PSScriptRoot 'source/sa_input_backend_finalize.c') (Join-Path $PSScriptRoot 'source/input_backend_finalize_native.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_backend_finalize_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native backend rows build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_packet_row_copy.py')
if ($LASTEXITCODE -ne 0) { throw 'Row copy verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_backend_finalize_rows.py')
if ($LASTEXITCODE -ne 0) { throw 'Native backend rows verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_backend_finalize_resume.py')
if ($LASTEXITCODE -ne 0) { throw 'Finalize continuation verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_backend_finalize_context.py')
if ($LASTEXITCODE -ne 0) { throw 'Finalize saved-context verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_device_available.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_available.dll')
if ($LASTEXITCODE -ne 0) { throw 'Device availability build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_available.py')
if ($LASTEXITCODE -ne 0) { throw 'Device availability verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_backend_finalize_device.py')
if ($LASTEXITCODE -ne 0) { throw 'Native final backend device verification failed' }
$deviceStateSources=@('sa_input_request_completion','sa_input_request_completion_impl','sa_input_device_record_pointer','sa_input_device_busy_read','sa_input_device_request_mark','sa_input_device_busy','sa_input_device_mark','input_device_state_native') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @deviceStateSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_state_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Device state build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_state.py')
if ($LASTEXITCODE -ne 0) { throw 'Device state verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_available_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native device availability verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_request_completion.py')
if ($LASTEXITCODE -ne 0) { throw 'Request completion verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_backend_completion_context.py')
if ($LASTEXITCODE -ne 0) { throw 'Backend completion saved-context verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_backend_completed_rows.py')
if ($LASTEXITCODE -ne 0) { throw 'Completed row context verification failed' }
$requestAllocationSources=@('sa_input_request_allocate','sa_input_request_lock','sa_input_request_pack','input_request_allocate_native','sa_input_request_queue','sa_input_queue_address','sa_input_queue_flush','input_request_queue_native') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @requestAllocationSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_request_allocate_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Request allocation build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_request_allocate.py')
if ($LASTEXITCODE -ne 0) { throw 'Request allocation verification failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_request_queue.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_request_queue.dll')
if ($LASTEXITCODE -ne 0) { throw 'Request queue build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_request_queue.py')
if ($LASTEXITCODE -ne 0) { throw 'Request queue verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_request_queue_tail.py')
if ($LASTEXITCODE -ne 0) { throw 'Request queue trailing payload verification failed' }
$requestQueueSources=@('sa_input_request_queue','sa_input_queue_address','sa_input_queue_flush','input_request_queue_native') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @requestQueueSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_request_queue_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native request queue build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_request_queue_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native request queue verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_queue_flush.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_queue_flush.dll')
if ($LASTEXITCODE -ne 0) { throw 'Queue flush build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_queue_flush.py')
if ($LASTEXITCODE -ne 0) { throw 'Queue flush verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_request_queue_preflush.py')
if ($LASTEXITCODE -ne 0) { throw 'Queue preflush verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_request_allocate_queued.py')
if ($LASTEXITCODE -ne 0) { throw 'Queued allocation verification failed' }

$simpleAllocationSources=@('sa_input_request_allocate_simple','sa_input_request_simple_lock','sa_input_request_simple_pack','input_request_allocate_simple_native','sa_input_request_queue','sa_input_queue_address','sa_input_queue_flush','input_request_queue_native') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @simpleAllocationSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_request_allocate_simple_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Simple allocation build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_request_allocate_simple.py')
if ($LASTEXITCODE -ne 0) { throw 'Simple allocation verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_backend_queued_rows.py')
if ($LASTEXITCODE -ne 0) { throw 'Queued backend rows verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_backend_request_queued.py')
if ($LASTEXITCODE -ne 0) { throw 'Queued backend request verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_device_metadata.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_metadata.dll')
if ($LASTEXITCODE -ne 0) { throw 'Device metadata build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_metadata.py')
if ($LASTEXITCODE -ne 0) { throw 'Device metadata verification failed' }

$metadataSources=@('sa_input_device_metadata','sa_input_metadata_locator','input_device_metadata_native') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @metadataSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_metadata_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native metadata build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_metadata_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native metadata verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_backend_request_metadata.py')
if ($LASTEXITCODE -ne 0) { throw 'Backend metadata integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_backend_request_packet_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native packet backend verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_packet_mode_set.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_packet_mode_set.dll')
if ($LASTEXITCODE -ne 0) { throw 'Packet mode build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_packet_mode.py')
if ($LASTEXITCODE -ne 0) { throw 'Packet mode verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_backend_mode_initialized.py')
if ($LASTEXITCODE -ne 0) { throw 'Mode-initialized backend verification failed' }

$systemInitializeSources=@('sa_input_system_initialize','sa_memory_fill','sa_input_packet_mode_set','sa_input_system_type_set','sa_input_retry_limit_set','sa_input_backend_reset','sa_input_system_configure','input_system_initialize_native') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @systemInitializeSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_system_initialize_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'System initialization build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_system_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'System initialization verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_system_type_set.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_system_type_set.dll')
if ($LASTEXITCODE -ne 0) { throw 'System type build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_system_type.py')
if ($LASTEXITCODE -ne 0) { throw 'System type verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_backend_system_initialized.py')
if ($LASTEXITCODE -ne 0) { throw 'System-initialized backend verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_device_backend_initialize.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_backend_initialize.dll')
if ($LASTEXITCODE -ne 0) { throw 'Device backend initialization build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_backend_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Device backend initialization verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_device_state_initialize.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_state_initialize.dll')
if ($LASTEXITCODE -ne 0) { throw 'Device state initialization build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_state_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Device state initialization verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_word_fill.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_word_fill.dll')
if ($LASTEXITCODE -ne 0) { throw 'Word fill build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_word_fill.py')
if ($LASTEXITCODE -ne 0) { throw 'Word fill verification failed' }
$deviceStateInitSources=@('sa_input_device_state_initialize','sa_input_word_fill','sa_input_device_mode_set','sa_input_device_control_assign','sa_input_device_info_initialize','sa_input_setup_copy','input_device_state_initialize_native') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @deviceStateInitSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_state_initialize_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native state initialization build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_state_fill.py')
if ($LASTEXITCODE -ne 0) { throw 'Native state fill verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_device_mode_set.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_mode_set.dll')
if ($LASTEXITCODE -ne 0) { throw 'Device mode build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_mode.py')
if ($LASTEXITCODE -ne 0) { throw 'Device mode verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_setup_copy.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_setup_copy.dll')
if ($LASTEXITCODE -ne 0) { throw 'Setup copy build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_setup_copy.py')
if ($LASTEXITCODE -ne 0) { throw 'Setup copy verification failed' }

$commandOneSources=@('sa_input_device_command_one','input_device_command_one_native','sa_input_request_queue','sa_input_queue_address','sa_input_queue_flush','input_request_queue_native') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @commandOneSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_command_one_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Command one build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_command_one.py')
if ($LASTEXITCODE -ne 0) { throw 'Command one verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_command_list_terminate.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_command_list_terminate.dll')
if ($LASTEXITCODE -ne 0) { throw 'Command terminator build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_command_list_terminate.py')
if ($LASTEXITCODE -ne 0) { throw 'Command terminator verification failed' }

$connectedBackendSources=@('sa_input_device_backend_initialize','input_device_backend_initialize_native','sa_input_device_state_initialize','sa_input_word_fill','sa_input_device_mode_set','sa_input_device_control_assign','sa_input_device_info_initialize','sa_input_setup_copy','input_device_state_initialize_native','sa_input_device_command_one','input_device_command_one_native','sa_input_request_queue','sa_input_queue_address','sa_input_queue_flush','input_request_queue_native','sa_input_command_list_terminate') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @connectedBackendSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_backend_initialize_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected backend build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_backend_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected backend verification failed' }

& $python (Join-Path $PSScriptRoot 'ghidra/specialize_input_io.py')
if ($LASTEXITCODE -ne 0) { throw 'Input IO specialization failed' }

$hostBackendSources=@('sa_input_device_contiguous_initialize_io','sa_input_device_backend_initialize_io','sa_input_device_state_initialize_io','input_device_backend_host','sa_input_word_fill','sa_input_device_mode_set','sa_input_device_control_assign','sa_input_device_info_initialize','sa_input_setup_copy','sa_input_device_command_one','input_device_command_one_native','sa_input_request_queue','sa_input_queue_address','sa_input_queue_flush','input_request_queue_native','sa_input_command_list_terminate') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @hostBackendSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_backend_host.dll')
if ($LASTEXITCODE -ne 0) { throw 'Host backend build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_backend_host.py')
if ($LASTEXITCODE -ne 0) { throw 'Host backend verification failed' }

# Response completion dependencies; leaf verification is narrower than full completion.
foreach ($responseName in @('response_complete','presence_decode','response_header','primary_response','sub_response','info_decode')) {
 & $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot ('source/sa_input_device_'+$responseName+'.c')) -o (Join-Path $Workspace ('work/sonic-adventure/build/input_device_'+$responseName+'.dll'))
 if ($LASTEXITCODE -ne 0) { throw "Device response build failed: $responseName" }
}
& $python (Join-Path $PSScriptRoot 'verify_input_device_response_leaves.py')
if ($LASTEXITCODE -ne 0) { throw 'Device response leaf verification failed' }

$responseSources=@('sa_input_device_response_complete','sa_input_device_presence_decode','sa_input_device_response_header','sa_input_device_primary_response','sa_input_device_sub_response','sa_input_device_info_decode','sa_input_response_word_swap','sa_input_device_info_initialize','sa_input_word_fill','input_device_response_native') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @responseSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_response_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Connected device response build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_response_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected device response verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_backend_native_completion.py')
if ($LASTEXITCODE -ne 0) { throw 'Backend native completion lifecycle verification failed' }

$submitSources=@('sa_input_device_submit','sa_input_device_commands_prepare','sa_input_device_error_report','sa_signed_division','sa_input_queue_address','sa_input_queue_flush','sa_input_device_command_one','input_device_command_one_native','sa_input_command_list_terminate','sa_input_request_queue','input_request_queue_native','input_device_submit_native','sa_input_device_command_features','sa_input_device_command_two','sa_input_device_command_three','sa_input_device_command_four') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @submitSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_submit_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Device submission build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_submit.py')
if ($LASTEXITCODE -ne 0) { throw 'Device submission verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_backend_native_submission.py')
if ($LASTEXITCODE -ne 0) { throw 'Native submission/completion lifecycle verification failed' }

$hostSubmitSources=@('sa_input_device_submit_io','sa_input_device_commands_prepare','sa_input_device_error_report','sa_signed_division','sa_input_queue_address','sa_input_queue_flush','sa_input_device_command_one','input_device_command_one_native','sa_input_command_list_terminate','sa_input_request_queue','input_request_queue_native','input_device_submit_host','sa_input_device_command_features','sa_input_device_command_two','sa_input_device_command_three','sa_input_device_command_four') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @hostSubmitSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_submit_host.dll')
if ($LASTEXITCODE -ne 0) { throw 'Host submission build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_submit_host.py')
if ($LASTEXITCODE -ne 0) { throw 'Host submission verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_backend_host_submission.py')
if ($LASTEXITCODE -ne 0) { throw 'Host submission lifecycle verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_device_status_publish.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_status_publish.dll')
if ($LASTEXITCODE -ne 0) { throw 'Device status publisher build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_discovery_host.py')
if ($LASTEXITCODE -ne 0) { throw 'Host backend discovery lifecycle verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_device_port_response_mask.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_port_response_mask.dll')
if ($LASTEXITCODE -ne 0) { throw 'Port mask decoder build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_port_response_mask.py')
if ($LASTEXITCODE -ne 0) { throw 'Port mask decoder verification failed' }
$portResponseSources=@('sa_input_device_response_complete','sa_input_device_presence_decode','sa_input_device_response_header','sa_input_device_primary_response','sa_input_device_sub_response','sa_input_device_info_decode','sa_input_response_word_swap','sa_input_device_info_initialize','sa_input_word_fill','input_device_response_native','sa_input_device_responses_process','sa_input_device_port_response_mask','input_device_responses_process_native') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @portResponseSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_responses_process_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native response dispatcher build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_dispatcher_discovery.py')
if ($LASTEXITCODE -ne 0) { throw 'Native response dispatcher verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_device_submit_rearm.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_submit_rearm.dll')
if ($LASTEXITCODE -ne 0) { throw 'Device submission rearm build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_vmu_discovery_host.py')
if ($LASTEXITCODE -ne 0) { throw 'Native VMU discovery verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_backend_vmu_discovered.py')
if ($LASTEXITCODE -ne 0) { throw 'Discovered VMU request lifecycle verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/input_command_transport.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_command_transport.dll')
if ($LASTEXITCODE -ne 0) { throw 'Host command transport build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_command_transport.py')
if ($LASTEXITCODE -ne 0) { throw 'Host command transport verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_backend_command_transport.py')
if ($LASTEXITCODE -ne 0) { throw 'Command transport VMU lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_device_automatic_commands.py')
if ($LASTEXITCODE -ne 0) { throw 'Automatic device command verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_input_device_shutdown_io.c') (Join-Path $PSScriptRoot 'source/sa_input_device_wait_clock_io.c') (Join-Path $PSScriptRoot 'source/sa_input_device_elapsed_ticks.c') (Join-Path $PSScriptRoot 'source/input_device_shutdown_host.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_shutdown_host.dll')
if ($LASTEXITCODE -ne 0) { throw 'Host device shutdown build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_shutdown_host.py')
if ($LASTEXITCODE -ne 0) { throw 'Host device shutdown verification failed' }

$serviceShutdownSources=@('sa_input_device_service_shutdown_io','input_device_service_shutdown_host','sa_interrupt_callback_remove','sa_memory_fill','sa_input_device_shutdown_io','sa_input_device_wait_clock_io','sa_input_device_elapsed_ticks','input_device_shutdown_host','sa_input_service_initialize','sa_interrupt_callback_create','sa_interrupt_handler_primary','sa_interrupt_handler_set','sa_input_service_start_explicit','sa_input_device_status_publish','sa_input_service_start_contiguous') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @serviceShutdownSources @hostBackendSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_device_service_shutdown_host.dll')
if ($LASTEXITCODE -ne 0) { throw 'Input service shutdown build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_service_shutdown_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Input service shutdown lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_service_shutdown_registered.py')
if ($LASTEXITCODE -ne 0) { throw 'Registered interrupt teardown lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_service_initialize_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Service initialize/request/shutdown lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_service_start_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Outer input service startup lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_device_contiguous_host.py')
if ($LASTEXITCODE -ne 0) { throw 'Contiguous backend host verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_service_contiguous_host.py')
if ($LASTEXITCODE -ne 0) { throw 'Contiguous service startup/shutdown verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_interrupt_defaults_initialize.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/interrupt_defaults_initialize.dll')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt defaults build failed' }
& $python (Join-Path $PSScriptRoot 'verify_interrupt_defaults_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Interrupt defaults verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_engine_context_parameters_initialize.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_context_parameters_initialize.dll')
if ($LASTEXITCODE -ne 0) { throw 'Context parameters build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_context_parameters_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Context parameters verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_string_compare.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/string_compare.dll')
if ($LASTEXITCODE -ne 0) { throw 'String comparison build failed' }
& $python (Join-Path $PSScriptRoot 'verify_string_compare.py')
if ($LASTEXITCODE -ne 0) { throw 'String comparison verification failed' }
& $python (Join-Path $PSScriptRoot 'register_string_compare.py')
if ($LASTEXITCODE -ne 0) { throw 'String comparison registry update failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/engine_callback_flag_reset.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_callback_flag_reset.dll')
if ($LASTEXITCODE -ne 0) { throw 'Engine callback flag-reset build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_callback_flag_reset.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine callback flag-reset verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_callback_flag_reset.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine callback flag-reset registry update failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/engine_state_mirror.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_state_mirror.dll')
if ($LASTEXITCODE -ne 0) { throw 'Engine state mirror build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_state_mirror.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine state mirror verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_state_mirror.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine state mirror registry update failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/engine_dispatch_wrappers.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_dispatch_wrappers.dll')
if ($LASTEXITCODE -ne 0) { throw 'Engine dispatch wrappers build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_dispatch_wrappers.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine dispatch wrappers verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_dispatch_wrappers.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine dispatch wrappers registry update failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/engine_dispatch_float_78.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_dispatch_float_78.dll')
if ($LASTEXITCODE -ne 0) { throw 'Engine float dispatch wrapper build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_dispatch_float_78.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine float dispatch wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_dispatch_float_78.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine float dispatch wrapper registry update failed' }

$engineContextSources=@('sa_engine_context_initialize','sa_engine_option_select','sa_engine_context_parameters_initialize','sa_interrupt_defaults_initialize','sa_memory_fill','sa_string_compare_wrapper','sa_string_compare','engine_context_initialize_native') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @engineContextSources -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_context_initialize_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Engine context native build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_context_initialize_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine context native verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_option_select.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine option selector verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_option_select.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine option selector registry update failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_context_service_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine context/input service lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_context_configurations.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine context configuration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_discovered_work_schedule.py')
if ($LASTEXITCODE -ne 0) { throw 'Discovered input native work scheduling verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_scheduled_request.py')
if ($LASTEXITCODE -ne 0) { throw 'Naturally scheduled VMU request verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_manager_resumed_request.py')
if ($LASTEXITCODE -ne 0) { throw 'Manager resumed VMU request verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_valid_worker_request.py')
if ($LASTEXITCODE -ne 0) { throw 'Valid active worker request verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_worker_native_event.py')
if ($LASTEXITCODE -ne 0) { throw 'Worker native event lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_worker_initialized_readiness.py')
if ($LASTEXITCODE -ne 0) { throw 'Worker initialized readiness verification failed' }

$alternatePressSources=@('input_press_alternate_native','sa_input_press_alternate','sa_input_channel_state_get','sa_input_channel_event_set','sa_input_channel_request_set') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @alternatePressSources -o (Join-Path $Workspace 'work/sonic-adventure/build/input_press_alternate_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native alternate press build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_worker_press_caller.py')
if ($LASTEXITCODE -ne 0) { throw 'Worker top-level press lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_worker_complete_teardown.py')
if ($LASTEXITCODE -ne 0) { throw 'Successful worker/service teardown verification failed' }

$frameHelperSources=@('sa_engine_pending_count_get','sa_engine_error_record','sa_engine_object_callback_dispatch') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @frameHelperSources -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_frame_helpers.dll')
if ($LASTEXITCODE -ne 0) { throw 'Frame helper build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_pending_count_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Pending count verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_object_callback_dispatch_errors.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback dispatch error verification failed' }

$engineQueuePopSources=@('sa_engine_queue_pop','sa_engine_error_record') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @engineQueuePopSources -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_queue_pop.dll')
if ($LASTEXITCODE -ne 0) { throw 'Engine queue pop build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_queue_pop.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine queue pop verification failed' }

$engineConsumerSources=@('sa_engine_queue_consume_context','sa_engine_object_state_get','sa_engine_object_state_set','sa_engine_error_record') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @engineConsumerSources -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_queue_consume_context.dll')
if ($LASTEXITCODE -ne 0) { throw 'Context consumer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_queue_consume_context_rejections.py')
if ($LASTEXITCODE -ne 0) { throw 'Context consumer rejection verification failed' }

$engineTransferSources=@('sa_engine_transfer_descriptor_initialize','sa_engine_transfer_submit') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @engineTransferSources -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_transfer.dll')
if ($LASTEXITCODE -ne 0) { throw 'Engine transfer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_descriptor_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Transfer descriptor verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_engine_transfer_busy_get.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_transfer_busy.dll')
if ($LASTEXITCODE -ne 0) { throw 'Transfer busy getter build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_busy_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Transfer busy getter verification failed' }

& $python (Join-Path $PSScriptRoot 'ghidra/specialize_cache_io.py')
if ($LASTEXITCODE -ne 0) { throw 'Cache IO specialization failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_engine_cache_writeback_io.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_cache_writeback_io.dll')
if ($LASTEXITCODE -ne 0) { throw 'Cache IO build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_cache_writeback.py')
if ($LASTEXITCODE -ne 0) { throw 'Cache writeback verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_cache_writeback_large.py')
if ($LASTEXITCODE -ne 0) { throw 'Large-range cache writeback verification failed' }

$nativeEngineTransferSources=@('sa_engine_transfer_descriptor_initialize','sa_engine_transfer_submit','sa_engine_transfer_busy_get','sa_engine_cache_writeback_io') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @nativeEngineTransferSources -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_transfer_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native engine transfer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native engine transfer verification failed' }

$engineArmSources=@('sa_engine_transfer_channels_arm','sa_engine_transfer_channel_arm','sa_engine_transfer_busy_get') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @engineArmSources -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_transfer_channels_arm_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native channel arm build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_channels_arm.py')
if ($LASTEXITCODE -ne 0) { throw 'Channel arm verification failed' }
& $python (Join-Path $PSScriptRoot 'ghidra/specialize_input_io.py')
if ($LASTEXITCODE -ne 0) { throw 'Ready getter IO specialization failed' }
$engineReadySources=@('sa_engine_transfer_ready_get_io','sa_engine_transfer_ready_wait') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @engineReadySources -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_transfer_ready_io.dll')
if ($LASTEXITCODE -ne 0) { throw 'Ready wait build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_ready_wait.py')
if ($LASTEXITCODE -ne 0) { throw 'Ready wait verification failed' }

& $python (Join-Path $PSScriptRoot 'ghidra/specialize_input_io.py')
if ($LASTEXITCODE -ne 0) { throw 'Transfer completion IO specialization failed' }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_engine_transfer_complete_io.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_transfer_complete_io.dll')
if ($LASTEXITCODE -ne 0) { throw 'Transfer completion build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_complete.py')
if ($LASTEXITCODE -ne 0) { throw 'Transfer completion verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_engine_transfer_descriptor_acquire.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_transfer_descriptor_acquire.dll')
if ($LASTEXITCODE -ne 0) { throw 'Transfer descriptor acquisition build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_descriptor_acquire.py')
if ($LASTEXITCODE -ne 0) { throw 'Transfer descriptor acquisition verification failed' }

$acquiredTransferSources=@('sa_engine_transfer_descriptor_acquire','sa_engine_transfer_descriptor_initialize','sa_engine_transfer_submit','sa_engine_transfer_busy_get','sa_engine_cache_writeback_io') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @acquiredTransferSources -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_transfer_acquired_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Acquired transfer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_acquired_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Acquired transfer verification failed' }

$acquiredLifecycleSources=@('sa_engine_transfer_descriptor_acquire','sa_engine_transfer_descriptor_initialize','sa_engine_transfer_submit','sa_engine_transfer_busy_get','sa_engine_cache_writeback_io','sa_engine_transfer_channels_arm','sa_engine_transfer_channel_arm','sa_engine_transfer_complete_io') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @acquiredLifecycleSources -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_transfer_acquired_lifecycle.dll')
if ($LASTEXITCODE -ne 0) { throw 'Acquired transfer lifecycle build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_acquired_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Acquired transfer lifecycle verification failed' }

$engineTransferInitSources=@('engine_transfer_initialize_host','sa_engine_transfer_initialize','sa_engine_transfer_descriptor_reset_io','sa_engine_transfer_address_window_set_io','sa_memory_fill','sa_interrupt_callback_create','sa_interrupt_handler_primary','sa_interrupt_handler_set') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @engineTransferInitSources -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_transfer_initialize_host.dll')
if ($LASTEXITCODE -ne 0) { throw 'Engine transfer initialization build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_initialize_host.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine transfer initialization verification failed' }

$initializedTransferSources=@('sa_engine_transfer_descriptor_acquire','sa_engine_transfer_descriptor_initialize','sa_engine_transfer_submit_io','sa_engine_transfer_busy_get_io','sa_engine_cache_writeback_io','sa_engine_transfer_channels_arm','sa_engine_transfer_channel_arm_io','sa_engine_transfer_complete_io') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @initializedTransferSources -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_transfer_initialized_lifecycle.dll')
if ($LASTEXITCODE -ne 0) { throw 'Initialized transfer lifecycle build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_initialized_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Initialized transfer lifecycle verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_interrupt_event_dispatch.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/interrupt_event_dispatch.dll')
if ($LASTEXITCODE -ne 0) { throw 'Registered event dispatcher build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_registered_completion.py')
if ($LASTEXITCODE -ne 0) { throw 'Registered transfer completion verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/g2_transfer_host.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/g2_transfer_host.dll')
if ($LASTEXITCODE -ne 0) { throw 'Host G2 transfer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_g2_transfer_host.py')
if ($LASTEXITCODE -ne 0) { throw 'Host G2 byte transfer verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_host_data_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Host-data transfer lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_bidirectional_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Bidirectional transfer lifecycle verification failed' }

$directConsumerSources=@('sa_engine_queue_consume_direct','sa_engine_object_state_get','sa_engine_error_record') | ForEach-Object { Join-Path $PSScriptRoot ('source/'+$_+'.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @directConsumerSources -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_queue_consume_direct.dll')
if ($LASTEXITCODE -ne 0) { throw 'Direct queue consumer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_queue_consume_direct_rejections.py')
if ($LASTEXITCODE -ne 0) { throw 'Direct consumer rejection verification failed' }

# Queue push recovery: retail instruction comparison.
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' outputs/sonic-adventure/source/sa_engine_queue_push.c outputs/sonic-adventure/source/sa_engine_error_record.c -o work/sonic-adventure/build/engine_queue_push.dll
if ($LASTEXITCODE -ne 0) { throw 'Queue push compilation failed' }
& $python outputs/sonic-adventure/verify_engine_queue_push.py
if ($LASTEXITCODE -ne 0) { throw 'Queue push verification failed' }

# Frame transfer begin rejection paths.
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' outputs/sonic-adventure/source/sa_engine_frame_transfer_begin.c outputs/sonic-adventure/source/sa_engine_object_state_get.c outputs/sonic-adventure/source/sa_engine_error_record.c -o work/sonic-adventure/build/engine_frame_transfer_begin.dll
if ($LASTEXITCODE -ne 0) { throw 'Frame transfer begin compilation failed' }
& $python outputs/sonic-adventure/verify_engine_frame_transfer_begin_rejections.py
if ($LASTEXITCODE -ne 0) { throw 'Frame transfer begin verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' outputs/sonic-adventure/source/sa_engine_frame_transfer_submit.c outputs/sonic-adventure/source/sa_engine_transfer_copy_strided_io.c outputs/sonic-adventure/source/sa_engine_transfer_channels_arm.c outputs/sonic-adventure/source/sa_engine_transfer_channel_arm_io.c outputs/sonic-adventure/source/sa_engine_transfer_ready_wait.c outputs/sonic-adventure/source/sa_engine_transfer_ready_get_io.c outputs/sonic-adventure/source/sa_engine_transfer_fifo_wait.c outputs/sonic-adventure/source/sa_engine_transfer_fifo_ready_get_io.c outputs/sonic-adventure/source/sa_engine_transfer_channels_release.c outputs/sonic-adventure/source/sa_engine_transfer_channel_release_io.c outputs/sonic-adventure/source/sa_engine_transfer_busy_get_io.c outputs/sonic-adventure/source/sa_engine_error_record.c -o work/sonic-adventure/build/engine_frame_transfer_submit.dll
if ($LASTEXITCODE -ne 0) { throw 'Transfer compilation failed' }
& $python outputs/sonic-adventure/verify_engine_frame_transfer_submit.py
if ($LASTEXITCODE -ne 0) { throw 'Transfer verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' outputs/sonic-adventure/source/sa_engine_frame_transfer_begin.c outputs/sonic-adventure/source/sa_engine_frame_services.c outputs/sonic-adventure/source/sa_engine_object_callback_dispatch.c outputs/sonic-adventure/source/sa_engine_pending_count_get.c outputs/sonic-adventure/source/sa_engine_object_state_get.c outputs/sonic-adventure/source/sa_engine_object_state_set.c outputs/sonic-adventure/source/sa_engine_frame_transfer_submit.c outputs/sonic-adventure/source/sa_engine_transfer_copy_strided_io.c outputs/sonic-adventure/source/sa_engine_transfer_channels_arm.c outputs/sonic-adventure/source/sa_engine_transfer_channel_arm_io.c outputs/sonic-adventure/source/sa_engine_transfer_ready_wait.c outputs/sonic-adventure/source/sa_engine_transfer_ready_get_io.c outputs/sonic-adventure/source/sa_engine_transfer_fifo_wait.c outputs/sonic-adventure/source/sa_engine_transfer_fifo_ready_get_io.c outputs/sonic-adventure/source/sa_engine_transfer_channels_release.c outputs/sonic-adventure/source/sa_engine_transfer_channel_release_io.c outputs/sonic-adventure/source/sa_engine_transfer_busy_get_io.c outputs/sonic-adventure/source/sa_engine_error_record.c -o work/sonic-adventure/build/engine_frame_transfer_begin_connected.dll
if ($LASTEXITCODE -ne 0) { throw 'Transfer compilation failed' }
& $python outputs/sonic-adventure/verify_engine_frame_transfer_begin_connected.py
if ($LASTEXITCODE -ne 0) { throw 'Transfer verification failed' }

& outputs/sonic-adventure/build_engine_queue_consume_direct_connected.ps1
& $python outputs/sonic-adventure/verify_engine_queue_consume_direct_connected.py
if ($LASTEXITCODE -ne 0) { throw 'Frame continuation verification failed' }

& outputs/sonic-adventure/build_engine_frame_queue_continuation.ps1
& $python outputs/sonic-adventure/verify_engine_frame_queue_continuation.py
if ($LASTEXITCODE -ne 0) { throw 'Frame continuation verification failed' }

& outputs/sonic-adventure/build_engine_frame_async_submit.ps1
& $python outputs/sonic-adventure/verify_engine_frame_async_submit.py
if ($LASTEXITCODE -ne 0) { throw 'Asynchronous frame submission verification failed' }

& outputs/sonic-adventure/build_engine_frame_async_completion.ps1
& $python outputs/sonic-adventure/verify_engine_frame_async_completion.py
if ($LASTEXITCODE -ne 0) { throw 'Asynchronous frame completion verification failed' }

& $python outputs/sonic-adventure/verify_engine_frame_async_event_completion.py
if ($LASTEXITCODE -ne 0) { throw 'Event frame continuation verification failed' }

& $python outputs/sonic-adventure/verify_engine_frame_async_queued_completion.py
if ($LASTEXITCODE -ne 0) { throw 'Event frame continuation verification failed' }

& $python outputs/sonic-adventure/verify_engine_frame_async_initialized_continuation.py
if ($LASTEXITCODE -ne 0) { throw 'Initialized frame continuation verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' outputs/sonic-adventure/source/sa_engine_frame_initialize.c -o work/sonic-adventure/build/engine_frame_initialize.dll
if ($LASTEXITCODE -ne 0) { throw 'Frame initializer compilation failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' outputs/sonic-adventure/source/sa_engine_frame_object_pool_initialize.c outputs/sonic-adventure/source/sa_engine_frame_queue_initialize.c outputs/sonic-adventure/source/sa_engine_record_copy_dispatch.c outputs/sonic-adventure/source/sa_engine_record_copy32_tail.c outputs/sonic-adventure/source/sa_engine_record_copy8_tail.c outputs/sonic-adventure/source/sa_engine_error_record.c -o work/sonic-adventure/build/engine_frame_pool_queue_initialize.dll
if ($LASTEXITCODE -ne 0) { throw 'Pool/queue build failed' }
& $python outputs/sonic-adventure/verify_engine_frame_pool_queue_initialize.py
if ($LASTEXITCODE -ne 0) { throw 'Pool/queue verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' outputs/sonic-adventure/source/sa_engine_frame_request_pool_initialize.c outputs/sonic-adventure/source/sa_engine_record_copy_loaded_dispatch.c outputs/sonic-adventure/source/sa_engine_record_copy28_tail.c outputs/sonic-adventure/source/sa_engine_frame_device_layout_initialize.c outputs/sonic-adventure/source/sa_memory_fill.c outputs/sonic-adventure/source/sa_engine_error_record.c -o work/sonic-adventure/build/engine_frame_request_layout_initialize.dll
if ($LASTEXITCODE -ne 0) { throw 'Request/layout build failed' }
& $python outputs/sonic-adventure/verify_engine_frame_request_pool_initialize.py
if ($LASTEXITCODE -ne 0) { throw 'Request/layout verification failed' }
& $python outputs/sonic-adventure/verify_engine_frame_device_layout_initialize.py
if ($LASTEXITCODE -ne 0) { throw 'Request/layout verification failed' }

& outputs/sonic-adventure/build_engine_frame_initialize_native.ps1
& $python outputs/sonic-adventure/verify_engine_frame_initialize_native.py
if ($LASTEXITCODE -ne 0) { throw 'Native frame startup verification failed' }

& $python outputs/sonic-adventure/verify_engine_frame_native_startup_continuation.py
if ($LASTEXITCODE -ne 0) { throw 'Native frame startup continuation failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' outputs/sonic-adventure/source/sa_engine_frame_mode_set.c -o work/sonic-adventure/build/engine_frame_mode_set.dll
if ($LASTEXITCODE -ne 0) { throw 'Mode setter compilation failed' }
& $python outputs/sonic-adventure/verify_engine_frame_native_mode_continuation.py
if ($LASTEXITCODE -ne 0) { throw 'Native mode continuation failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' outputs/sonic-adventure/source/sa_engine_frame_object_configure.c outputs/sonic-adventure/source/sa_engine_frame_object_release.c outputs/sonic-adventure/source/sa_engine_object_address_set.c outputs/sonic-adventure/source/sa_engine_object_callback_set.c outputs/sonic-adventure/source/sa_engine_object_length_set.c outputs/sonic-adventure/source/sa_engine_object_state_get.c outputs/sonic-adventure/source/sa_engine_object_state_set.c outputs/sonic-adventure/source/sa_engine_error_record.c -o work/sonic-adventure/build/engine_frame_object_configure.dll
if ($LASTEXITCODE -ne 0) { throw 'Object configure build failed' }
& $python outputs/sonic-adventure/verify_engine_frame_object_configure.py
if ($LASTEXITCODE -ne 0) { throw 'Object configure verification failed' }

& $python outputs/sonic-adventure/verify_engine_frame_native_object_continuation.py
if ($LASTEXITCODE -ne 0) { throw 'Native allocated object lifecycle failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' outputs/sonic-adventure/source/sa_engine_frame_request_enqueue.c outputs/sonic-adventure/source/sa_memory_move.c outputs/sonic-adventure/source/sa_engine_error_record.c -o work/sonic-adventure/build/engine_frame_request_enqueue.dll
if ($LASTEXITCODE -ne 0) { throw 'Request enqueue build failed' }
& $python outputs/sonic-adventure/verify_engine_frame_request_enqueue.py
if ($LASTEXITCODE -ne 0) { throw 'Request enqueue verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' outputs/sonic-adventure/source/sa_sound_request_process.c outputs/sonic-adventure/source/sa_sound_work_dispatch.c -o work/sonic-adventure/build/sound_request_process.dll
if ($LASTEXITCODE -ne 0) { throw 'Sound request processing build failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' outputs/sonic-adventure/source/sa_sound_status_byte_get.c outputs/sonic-adventure/source/sa_sound_status_word_get.c outputs/sonic-adventure/source/sa_engine_transfer_read_strided_io.c -o work/sonic-adventure/build/sound_status_get.dll
if ($LASTEXITCODE -ne 0) { throw 'Sound status build failed' }
& $python outputs/sonic-adventure/verify_sound_status_get.py
if ($LASTEXITCODE -ne 0) { throw 'Sound status verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' outputs/sonic-adventure/source/sa_sound_status_word_set.c outputs/sonic-adventure/source/sa_sound_status_byte_set.c outputs/sonic-adventure/source/sa_sound_command_complete.c -o work/sonic-adventure/build/sound_status_set.dll
if ($LASTEXITCODE -ne 0) { throw 'Sound status write build failed' }
& $python outputs/sonic-adventure/verify_sound_status_set.py
if ($LASTEXITCODE -ne 0) { throw 'Sound write/completion verification failed' }
& $python outputs/sonic-adventure/verify_sound_command_complete.py
if ($LASTEXITCODE -ne 0) { throw 'Sound write/completion verification failed' }

& $python outputs/sonic-adventure/verify_sound_work_dispatch.py
if ($LASTEXITCODE -ne 0) { throw 'Sound work dispatch verification failed' }

& $python outputs/sonic-adventure/verify_sound_request_process.py
if ($LASTEXITCODE -ne 0) { throw 'Sound request processing verification failed' }

# Sound request processing with native startup and 256-record pool.
& $Python (Join-Path $PSScriptRoot "verify_sound_request_process_initialized.py")
if ($LASTEXITCODE -ne 0) { throw "Initialized sound request verification failed" }

& (Join-Path $PSScriptRoot "build_sound_request_commit.ps1") -Workspace $Workspace

& $python (Join-Path $PSScriptRoot "verify_sound_request_producer_process.py")
if ($LASTEXITCODE -ne 0) { throw "Native sound producer/process verification failed" }

& (Join-Path $PSScriptRoot "build_sound_command_type2.ps1") -Workspace $Workspace

& $python (Join-Path $PSScriptRoot "verify_sound_command_type2_connected.py")
if ($LASTEXITCODE -ne 0) { throw "Connected type-2 sound API verification failed" }

& (Join-Path $PSScriptRoot "build_sound_index_dispatch.ps1") -Workspace $Workspace

& (Join-Path $PSScriptRoot "build_sound_low_index.ps1") -Workspace $Workspace

& $python (Join-Path $PSScriptRoot "verify_sound_midi_pack.py")
if ($LASTEXITCODE -ne 0) { throw "MIDI message-class verification failed" }

& $python (Join-Path $PSScriptRoot "verify_sound_midi_classes_connected.py")
if ($LASTEXITCODE -ne 0) { throw "Connected MIDI message-class verification failed" }

& (Join-Path $PSScriptRoot "build_sound_slot_setup.ps1") -Workspace $Workspace

& $python (Join-Path $PSScriptRoot "verify_sound_slot_acquire.py")
if ($LASTEXITCODE -ne 0) { throw "Typed sound slot allocation verification failed" }

& (Join-Path $PSScriptRoot "build_sound_slot_release.ps1") -Workspace $Workspace
& $python (Join-Path $PSScriptRoot "verify_sound_slot_lifecycle.py")
if ($LASTEXITCODE -ne 0) { throw "Sound slot lifecycle verification failed" }

& $python (Join-Path $PSScriptRoot "verify_sound_slot_lifecycle_initialized.py")
if ($LASTEXITCODE -ne 0) { throw "Initialized sound slot lifecycle verification failed" }

& (Join-Path $PSScriptRoot "build_sound_note_on.ps1") -Workspace $Workspace

& $python (Join-Path $PSScriptRoot "verify_sound_note_pair_connected.py")
if ($LASTEXITCODE -ne 0) { throw "Note-on/off pair verification failed" }

& (Join-Path $PSScriptRoot "build_sound_volume_control.ps1") -Workspace $Workspace

& (Join-Path $PSScriptRoot "build_sound_pan_control.ps1") -Workspace $Workspace

& (Join-Path $PSScriptRoot "build_sound_pitch_bend.ps1") -Workspace $Workspace

& (Join-Path $PSScriptRoot "build_sound_channel_reset.ps1") -Workspace $Workspace

& (Join-Path $PSScriptRoot "build_sound_program_setup.ps1") -Workspace $Workspace

& $python (Join-Path $PSScriptRoot "verify_sound_program_setup_sequence.py")
if ($LASTEXITCODE -ne 0) { throw "Program setup sequence verification failed" }

& (Join-Path $PSScriptRoot "build_sound_volume_dispatch.ps1") -Workspace $Workspace

& (Join-Path $PSScriptRoot "build_sound_command_type5.ps1") -Workspace $Workspace

& $python (Join-Path $PSScriptRoot "verify_sound_command_type5.py")
if ($LASTEXITCODE -ne 0) { throw "Type-5 API verification failed" }

& $python (Join-Path $PSScriptRoot "verify_sound_command_type5_extra_connected.py")
if ($LASTEXITCODE -ne 0) { throw "Connected type5 extra-field verification failed" }

& (Join-Path $PSScriptRoot "build_sound_command_type1.ps1") -Workspace $Workspace

& $python (Join-Path $PSScriptRoot "verify_sound_command_type1_connected.py")
if ($LASTEXITCODE -ne 0) { throw "Connected type1 verification failed" }

& (Join-Path $PSScriptRoot "build_sound_play_dispatch.ps1") -Workspace $Workspace

& $python (Join-Path $PSScriptRoot "verify_sound_play_dispatch_midi.py")
if ($LASTEXITCODE -ne 0) { throw "MIDI play-dispatch verification failed" }

# Compile recovered bank loader; not a behavioral verification gate.
& (Join-Path $PSScriptRoot "build_sound_bank_load.ps1") -Workspace $Workspace

# Compilation only; multi-unit download verification pending.
& (Join-Path $PSScriptRoot "build_sound_bank_download.ps1") -Workspace $Workspace

& (Join-Path $PSScriptRoot 'build_sound_multi_unit_download.ps1') -Workspace $Workspace

& (Join-Path $PSScriptRoot 'build_sound_bank_section_configure.ps1') -Workspace $Workspace

& (Join-Path $PSScriptRoot 'build_sound_bank_section_transfer.ps1') -Workspace $Workspace

& (Join-Path $PSScriptRoot 'build_sound_bank_section_lookup.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sound_bank_section_lookup.py')
if ($LASTEXITCODE -ne 0) { throw 'Bank section lookup verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_bank_word_write.ps1') -Workspace $Workspace

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_section_configure.py')
if ($LASTEXITCODE -ne 0) { throw 'Bank configuration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_word_write.py')
if ($LASTEXITCODE -ne 0) { throw 'Bank word-write verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_section_configure_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected bank configuration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_section_configure_lookup.py')
if ($LASTEXITCODE -ne 0) { throw 'Bank configuration/lookup verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_section_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Bank section transfer verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_section_transfer_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected bank section transfer verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_multi_unit_download_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected multi-unit download verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_multi_unit_download_records.py')
if ($LASTEXITCODE -ne 0) { throw 'Multi-record download verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_download_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Game-facing bank download verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_stream_bank_download.py')
if ($LASTEXITCODE -ne 0) { throw 'Real stream bank verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_real_payload_bank_download.py')
if ($LASTEXITCODE -ne 0) { throw 'Real payload bank download verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_host_services.py')
if ($LASTEXITCODE -ne 0) { throw 'Bank load host-service verification failed' }

& (Join-Path $PSScriptRoot 'build_file_handle_metadata.ps1') -Workspace $Workspace

& $python (Join-Path $PSScriptRoot 'verify_file_handle_metadata.py')
if ($LASTEXITCODE -ne 0) { throw 'File metadata verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_native_metadata.py')
if ($LASTEXITCODE -ne 0) { throw 'Bank load native metadata verification failed' }

& (Join-Path $PSScriptRoot 'build_file_close_cleanup.ps1') -Workspace $Workspace

& (Join-Path $PSScriptRoot 'build_file_timer_helpers.ps1') -Workspace $Workspace

& $python (Join-Path $PSScriptRoot 'verify_file_timer_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'File timer verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_close_cleanup_immediate.py')
if ($LASTEXITCODE -ne 0) { throw 'Immediate close cleanup verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_close_cleanup_timeout.py')
if ($LASTEXITCODE -ne 0) { throw 'Owned file close timeout verification failed' }

& (Join-Path $PSScriptRoot 'build_file_release_wait.ps1') -Workspace $Workspace

& (Join-Path $PSScriptRoot 'build_file_status_get.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_status_get.py')
if ($LASTEXITCODE -ne 0) { throw 'File status getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_release_wait.py')
if ($LASTEXITCODE -ne 0) { throw 'File release wait verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_release_wait_progress.py')
if ($LASTEXITCODE -ne 0) { throw 'File release progress verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_close_owned_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected owned file close verification failed' }

& (Join-Path $PSScriptRoot 'build_file_read_sectors.ps1') -Workspace $Workspace

& (Join-Path $PSScriptRoot 'build_file_read_aligned.ps1') -Workspace $Workspace

& (Join-Path $PSScriptRoot 'build_file_progress_get.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_progress_get.py')
if ($LASTEXITCODE -ne 0) { throw 'File progress verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_read_aligned.py')
if ($LASTEXITCODE -ne 0) { throw 'Aligned read verification failed' }

& (Join-Path $PSScriptRoot 'build_file_read_unaligned.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_read_unaligned.py')
if ($LASTEXITCODE -ne 0) { throw 'Unaligned read verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_read_sectors_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected sector read verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_native_sector_read.py')
if ($LASTEXITCODE -ne 0) { throw 'Bank load native sector read verification failed' }

& (Join-Path $PSScriptRoot 'build_file_open_dispatch.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_open_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'File open dispatch verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_native_open_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Bank load native open dispatch verification failed' }

& (Join-Path $PSScriptRoot 'build_file_backend_open.ps1') -Workspace $Workspace

& (Join-Path $PSScriptRoot 'build_file_handle_open_initialize.ps1') -Workspace $Workspace

& $python (Join-Path $PSScriptRoot 'verify_file_handle_open_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Handle initialization verification failed' }

& (Join-Path $PSScriptRoot 'build_file_error_set.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_backend_open.py')
if ($LASTEXITCODE -ne 0) { throw 'Backend file open verification failed' }

& (Join-Path $PSScriptRoot 'build_file_filename_lookup.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_filename_lookup.py')
if ($LASTEXITCODE -ne 0) { throw 'Filename lookup verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_file_backend_open_native_lookup.py')
if ($LASTEXITCODE -ne 0) { throw 'Backend open native lookup verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_native_backend.py')
if ($LASTEXITCODE -ne 0) { throw 'Bank load native backend verification failed' }

& (Join-Path $PSScriptRoot 'build_file_backend_lifecycle.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_backend_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Backend lifecycle verification failed' }

& (Join-Path $PSScriptRoot 'build_file_context_initialize.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_context_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Filesystem context initialization verification failed' }

& (Join-Path $PSScriptRoot 'build_file_directory_storage.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_context_initialize_native_directory.py')
if ($LASTEXITCODE -ne 0) { throw 'Native directory context initialization verification failed' }

& (Join-Path $PSScriptRoot 'build_file_mount.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_mount_no_handle.py')
if ($LASTEXITCODE -ne 0) { throw 'Mount no-handle verification failed' }

& (Join-Path $PSScriptRoot 'build_file_reserved_handle.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_reserved_handle.py')
if ($LASTEXITCODE -ne 0) { throw 'Reserved handle verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_file_mount_native_reserved_handle.py')
if ($LASTEXITCODE -ne 0) { throw 'Mount native reserved handle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_mount_native_request_errors.py')
if ($LASTEXITCODE -ne 0) { throw 'Mount native request errors verification failed' }

& (Join-Path $PSScriptRoot 'build_file_volume_helpers.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_volume_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'Volume helpers verification failed' }

& (Join-Path $PSScriptRoot 'build_file_directory_parser.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_directory_record.py')
if ($LASTEXITCODE -ne 0) { throw 'Directory record verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_directory_load.py')
if ($LASTEXITCODE -ne 0) { throw 'Directory loader verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_directory_load_multisector.py')
if ($LASTEXITCODE -ne 0) { throw 'Multi-sector directory loader verification failed' }

& (Join-Path $PSScriptRoot 'build_file_mount_sector_read.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_mount_sector_read.py')
if ($LASTEXITCODE -ne 0) { throw 'Mount sector bridge verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_mount_sector_read_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected mount sector read verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_directory_load_native_pipeline.py')
if ($LASTEXITCODE -ne 0) { throw 'Native pipeline directory verification failed' }

& (Join-Path $PSScriptRoot 'build_file_directory_parent_sector_adjust.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_directory_parent_sector_adjust.py')
if ($LASTEXITCODE -ne 0) { throw 'Directory parent sector adjustment verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_mount_native_volume_rejection.py')
if ($LASTEXITCODE -ne 0) { throw 'Mount native volume rejection verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_mount_native_success.py')
if ($LASTEXITCODE -ne 0) { throw 'Native mount success verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_mount_original_disc.py')
if ($LASTEXITCODE -ne 0) { throw 'Original disc native mount verification failed' }

& (Join-Path $PSScriptRoot 'build_file_directory_select.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_mount_original_boot_directory.py')
if ($LASTEXITCODE -ne 0) { throw 'Original boot directory native mount verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_context_original_mount.py')
if ($LASTEXITCODE -ne 0) { throw 'Original disc context initialization and mount verification failed' }

& (Join-Path $PSScriptRoot 'build_file_device_interface_get.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_context_native_interface.py')
if ($LASTEXITCODE -ne 0) { throw 'Native interface context initialization and mount verification failed' }

& (Join-Path $PSScriptRoot 'build_file_device_initialize.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_context_native_device_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Native device initialization and mount verification failed' }

& (Join-Path $PSScriptRoot 'build_file_device_low_initialize.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_context_bios_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'BIOS boundary initialization and mount verification failed' }

& (Join-Path $PSScriptRoot 'build_file_device_read.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_device_read.py')
if ($LASTEXITCODE -ne 0) { throw 'Device read verification failed' }

& (Join-Path $PSScriptRoot 'build_file_bios_request.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_device_read_bios_request.py')
if ($LASTEXITCODE -ne 0) { throw 'Device read BIOS request verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_context_bios_sector_read.py')
if ($LASTEXITCODE -ne 0) { throw 'BIOS sector boundary initialization and mount verification failed' }

& (Join-Path $PSScriptRoot 'build_file_device_start.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_context_bios_execute.py')
if ($LASTEXITCODE -ne 0) { throw 'BIOS execution boundary initialization and mount verification failed' }

& (Join-Path $PSScriptRoot 'build_file_device_command24.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_device_command24.py')
if ($LASTEXITCODE -ne 0) { throw 'Device command24 verification failed' }

& (Join-Path $PSScriptRoot 'build_file_registered_callback.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_registered_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Registered filesystem callback verification failed' }

& (Join-Path $PSScriptRoot 'build_file_backend_poll.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_backend_poll.py')
if ($LASTEXITCODE -ne 0) { throw 'Focused backend poll verification failed' }
& $python (Join-Path $PSScriptRoot 'register_file_backend_poll.py')
if ($LASTEXITCODE -ne 0) { throw 'Backend poll registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_file_registered_backend_poll.py')
if ($LASTEXITCODE -ne 0) { throw 'Registered callback/backend poll verification failed' }

& (Join-Path $PSScriptRoot 'build_file_completion_process.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_completion_process_no_handle.py')
if ($LASTEXITCODE -ne 0) { throw 'Completion processor no-active-handle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_completion_process_active.py')
if ($LASTEXITCODE -ne 0) { throw 'Active-handle completion processor verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_completion_process_pending_errors.py')
if ($LASTEXITCODE -ne 0) { throw 'Completion processor pending_errors verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_completion_process_error_remap.py')
if ($LASTEXITCODE -ne 0) { throw 'Completion processor error_remap verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_completion_process_operations.py')
if ($LASTEXITCODE -ne 0) { throw 'file_completion_process_operations verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_registered_native_completion.py')
if ($LASTEXITCODE -ne 0) { throw 'file_registered_native_completion verification failed' }

& (Join-Path $PSScriptRoot 'build_file_device_status.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_device_status.py')
if ($LASTEXITCODE -ne 0) { throw 'Device status verification failed' }

& (Join-Path $PSScriptRoot 'build_file_bios_status.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_device_status_bios.py')
if ($LASTEXITCODE -ne 0) { throw 'file_device_status_bios verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_file_registered_completion_bios_status.py')
if ($LASTEXITCODE -ne 0) { throw 'file_registered_completion_bios_status verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_registered_completion_bios_pipeline.py')
if ($LASTEXITCODE -ne 0) { throw 'Registered completion BIOS pipeline verification failed' }

& (Join-Path $PSScriptRoot 'build_file_device_pending_query.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_registered_completion_pending_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Native pending query verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_context_native_completion.py')
if ($LASTEXITCODE -ne 0) { throw 'Original disc native completion mount verification failed' }

& (Join-Path $PSScriptRoot 'build_file_device_disc_info.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_device_disc_info.py')
if ($LASTEXITCODE -ne 0) { throw 'file_device_disc_info verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_file_context_native_disc_info.py')
if ($LASTEXITCODE -ne 0) { throw 'file_context_native_disc_info verification failed' }

& (Join-Path $PSScriptRoot 'build_file_device_disc_request.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_device_disc_request.py')
if ($LASTEXITCODE -ne 0) { throw 'file_device_disc_request verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_file_context_native_disc_request.py')
if ($LASTEXITCODE -ne 0) { throw 'file_context_native_disc_request verification failed' }

& (Join-Path $PSScriptRoot 'build_file_device_version_request.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_device_version_request.py')
if ($LASTEXITCODE -ne 0) { throw 'file_device_version_request verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_file_context_native_version_request.py')
if ($LASTEXITCODE -ne 0) { throw 'file_context_native_version_request verification failed' }

& (Join-Path $PSScriptRoot 'build_file_callback_register.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_callback_register.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback registration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_callback_register_empty.py')
if ($LASTEXITCODE -ne 0) { throw 'Empty callback registration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_callback_register_mmio.py')
if ($LASTEXITCODE -ne 0) { throw 'file_callback_register_mmio verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_context_native_registration.py')
if ($LASTEXITCODE -ne 0) { throw 'file_context_native_registration verification failed' }

& (Join-Path $PSScriptRoot 'build_file_driver_callback_install.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_file_context_native_driver_install.py')
if ($LASTEXITCODE -ne 0) { throw 'Native driver callback mount verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_mounted_original_file_read.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted original file read verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_mounted_sequential_read.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted sequential file verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_mounted_unaligned_read.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted unaligned file verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_mounted_multiple_files.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted multiple-file verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_mounted_prs_decode.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted PRS decode verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_mounted_disc.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted-disc sound bank load verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_mounted_download.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted native bank download wrapper verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_mounted_multi.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted native multi-unit bank verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_mounted_section_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted native bank section transfer verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_mounted_table_write.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted native bank table write verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_mounted_queue.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted native bank queue and payload transfer verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_mounted_bank_suite.py')
if ($LASTEXITCODE -ne 0) { throw 'Additional original-disc sound bank verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_mounted_initialized.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted bank load with native sound initialization verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_mounted_full_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted bank load with engine transfer sound initialization verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_mode_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_mode_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine mode initializer verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_mode_initialize_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected engine mode initializer verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_device_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_device_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine device initializer verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_callback_state_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_callback_state_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback-state initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_callback_state_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback-state initializer registry update failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_mode_initialize_device_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Mode device callback-state integration verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_callback_register.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_callback_register.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine callback registrar verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_mode_initialize_registered.py')
if ($LASTEXITCODE -ne 0) { throw 'Mode initializer native interrupt registration verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_memory_layout_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_memory_layout_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine memory-layout initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_memory_layout_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine memory-layout initializer registry update failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_mode_initialize_layout.py')
if ($LASTEXITCODE -ne 0) { throw 'Mode initializer memory-layout integration verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_register_table_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_mode_initialize_table.py')
if ($LASTEXITCODE -ne 0) { throw 'Mode initializer register-table integration verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_video_state_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_video_state_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Full video state initialization verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_video_state_identity.py')
if ($LASTEXITCODE -ne 0) { throw 'Video dispatcher thunk verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_video_state_identity.py')
if ($LASTEXITCODE -ne 0) { throw 'Video dispatcher thunk registry update failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_mode_initialize_video_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Mode initializer full video integration verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_memory_manager_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_memory_manager_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Memory manager initializer verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_memory_manager_reset.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_memory_manager_reset_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Memory manager native reset verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_memory_manager_reset_lists.py')
if ($LASTEXITCODE -ne 0) { throw 'Memory manager live-list reset verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_mode_initialize_manager_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Mode video memory-manager integration verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_descriptor_lifecycle.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_descriptor_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Native descriptor lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_mode_initialize_pool_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Mode native descriptor pool integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_memory_manager_reset_native_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Live manager native descriptor release verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_auxiliary_descriptor_release.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_memory_manager_reset_all_native.py')
if ($LASTEXITCODE -ne 0) { throw 'All-native memory manager live-list reset verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_graphics_register_io.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_graphics_register_io.py')
if ($LASTEXITCODE -ne 0) { throw 'Graphics register primitive verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_mode_initialize_register_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Mode graphics register integration verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_graphics_state_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_mode_initialize_graphics_state.py')
if ($LASTEXITCODE -ne 0) { throw 'Mode graphics state integration verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_render_layout_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_mode_initialize_render_layout.py')
if ($LASTEXITCODE -ne 0) { throw 'Mode rendering layout integration verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_render_buffer_size.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_size.py')
if ($LASTEXITCODE -ne 0) { throw 'Render buffer size verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_mode_initialize_buffer_size.py')
if ($LASTEXITCODE -ne 0) { throw 'Mode buffer size integration verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_render_buffer_registers.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_registers.py')
if ($LASTEXITCODE -ne 0) { throw 'Render buffer register verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_render_buffer_setup.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_mode_initialize_buffer_setup.py')
if ($LASTEXITCODE -ne 0) { throw 'Mode buffer setup integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_setup_tiles.py')
if ($LASTEXITCODE -ne 0) { throw 'Nonzero buffer tile setup verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_render_buffer_transfer.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_setup_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Native buffer transfer integration verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_word_copy.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_word_copy.py')
if ($LASTEXITCODE -ne 0) { throw 'Native engine word copy verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected render-buffer transfer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_render_buffer_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Render-buffer transfer registration failed' }

& (Join-Path $PSScriptRoot 'build_engine_render_buffer_readback.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_readback.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected render-buffer readback verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_render_buffer_readback.py')
if ($LASTEXITCODE -ne 0) { throw 'Render-buffer readback registration failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_roundtrip.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected upload/readback roundtrip verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_render_buffer_cached_transfer.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_cached_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Cached render-buffer transfer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_render_buffer_cached_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Cached render-buffer transfer registration failed' }

& (Join-Path $PSScriptRoot 'build_engine_render_buffer_dma_setup.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_dma_setup.py')
if ($LASTEXITCODE -ne 0) { throw 'Render-buffer DMA setup verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_render_buffer_dma_setup.py')
if ($LASTEXITCODE -ne 0) { throw 'Render-buffer DMA setup registration failed' }

& (Join-Path $PSScriptRoot 'build_engine_render_buffer_dispatch.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Render-buffer dispatch verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_render_buffer_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Render-buffer dispatch registration failed' }

& (Join-Path $PSScriptRoot 'build_engine_render_buffer_ready_query.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_ready_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Render-buffer readiness query verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_render_buffer_ready_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Render-buffer readiness query registration failed' }

& (Join-Path $PSScriptRoot 'build_engine_render_ready_table_scan.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_ready_table_scan.py')
if ($LASTEXITCODE -ne 0) { throw 'Render-ready table scan differential verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_render_ready_table_scan.py')
if ($LASTEXITCODE -ne 0) { throw 'Render-ready table scan registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_render_ready_query_integrated.py')
if ($LASTEXITCODE -ne 0) { throw 'Integrated render-readiness verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_render_ready_query_integrated.py')
if ($LASTEXITCODE -ne 0) { throw 'Integrated render-readiness registration failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_setup_copy.py')
if ($LASTEXITCODE -ne 0) { throw 'Native buffer word-copy integration verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_render_buffer_header.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_header.py')
if ($LASTEXITCODE -ne 0) { throw 'Buffer header verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_setup_header_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected buffer header mode verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_setup_packing.py')
if ($LASTEXITCODE -ne 0) { throw 'Buffer packing-mode verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_setup_ordering.py')
if ($LASTEXITCODE -ne 0) { throw 'Buffer coordinate ordering verification failed' }

& (Join-Path $PSScriptRoot 'build_unsigned_division_helpers.ps1')
& $python (Join-Path $PSScriptRoot 'verify_unsigned_division_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'Unsigned division helpers verification failed' }
& $python (Join-Path $PSScriptRoot 'register_unsigned_remainder.py')
if ($LASTEXITCODE -ne 0) { throw 'Unsigned remainder registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_render_buffer_setup_native_division.py')
if ($LASTEXITCODE -ne 0) { throw 'Native tile division integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_mode_initialize_native_clear.py')
if ($LASTEXITCODE -ne 0) { throw 'Native startup VRAM clear verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_render_record_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_record_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Record initializer verification failed' }

& (Join-Path $PSScriptRoot 'build_record_copy_28.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_record_copy_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Native record copy integration verification failed' }

& (Join-Path $PSScriptRoot 'build_byte_fill.ps1')
& $python (Join-Path $PSScriptRoot 'verify_byte_fill.py')
if ($LASTEXITCODE -ne 0) { throw 'Byte fill verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_render_record_fill_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Native record fill integration verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_error_register.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_record_all_native.py')
if ($LASTEXITCODE -ne 0) { throw 'All-native record initializer verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_render_record_submit.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_record_submit.py')
if ($LASTEXITCODE -ne 0) { throw 'Record submission verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_render_record_complete.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_record_complete.py')
if ($LASTEXITCODE -ne 0) { throw 'Record completion verification failed' }

& (Join-Path $PSScriptRoot 'build_device_byte_write.ps1')
& $python (Join-Path $PSScriptRoot 'verify_device_byte_write.py')
if ($LASTEXITCODE -ne 0) { throw 'Device byte write verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_render_record_complete_byte_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected completion byte-write verification failed' }

& (Join-Path $PSScriptRoot 'build_device_word_read.ps1')
& $python (Join-Path $PSScriptRoot 'verify_device_word_read.py')
if ($LASTEXITCODE -ne 0) { throw 'Device word read verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_transfer_read_strided_io.ps1')
& $python (Join-Path $PSScriptRoot 'verify_device_word_read_transfer_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Native device reader transfer integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_render_record_complete_read_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected completion native reader verification failed' }

& (Join-Path $PSScriptRoot 'build_device_word_write.ps1')
& $python (Join-Path $PSScriptRoot 'verify_device_word_write.py')
if ($LASTEXITCODE -ne 0) { throw 'Device word writer verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_render_record_complete_write_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected native word writer verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_transfer_copy_strided_io.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_record_complete_transfer_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected completion native write transfer failed' }

& (Join-Path $PSScriptRoot 'build_engine_transfer_control.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_record_complete_control_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Native completion transfer control verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_transfer_ready_getters.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_record_complete_status_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Native completion ready getter integration failed' }

& (Join-Path $PSScriptRoot 'build_engine_transfer_channel_control.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_render_record_complete_channels_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Native completion channel control integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_render_record_submit_configure_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Record submission native configuration integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_work_dispatch_channel_status.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound work dispatch channel/status integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_initialize_wrapper.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_initialize_wrapper.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound initialization wrapper verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_mounted_wrapper_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted bank load through native sound initialization wrapper failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_load_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound driver load initialization verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_configure_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver initialization native object configuration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_release_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver initialization native object release failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_transfer.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_transfer_rejection.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound driver transfer rejection verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_transfer_success.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound driver transfer success verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_transfer_configure_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver transfer native configuration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_transfer_release_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver transfer native release failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_transfer_allocate_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver transfer native allocation failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_transfer_queue_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver transfer native queue integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_memory_clear.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_memory_clear.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound memory clear wrapper verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_stop.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_stop.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound driver stop verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_stop_transfer_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound stop native transfer integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_start.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_start_transfer_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound driver start native transfer verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_transfer_stop_start_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver transfer stop/start integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_memory_clear_transfer_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Full native sound memory clear failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_transfer_memory_clear_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver transfer full memory clear integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_info.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_info_missing_context.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver info missing context verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_info_context_present.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver info context verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_getters.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_word_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver word getter failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_byte_get_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver byte getter failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_info_getters_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Native driver info getter integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_info_transfer_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Full native driver info transfer integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_transfer_all_native.py')
if ($LASTEXITCODE -ne 0) { throw 'All native driver transfer chain failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_transfer_retail_payload.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail driver payload native upload failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_callback_register.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_callback_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver load native callback registration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_callback_replace.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver load callback replacement failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_constructor_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver load native constructor failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_handler_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver loader handler_connected integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_native_replace.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver loader native_replace integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_native_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver loader native_initialize integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_mode_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_mode_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver context setter failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_native_mode.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver loader native setter failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_native_startup_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver native startup callback lifecycle failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_irq_callback.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_irq_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver IRQ callback failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_service.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_service.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound driver service failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_irq_callback_service_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver IRQ native service integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_irq_request_initialized.py')
if ($LASTEXITCODE -ne 0) { throw 'Initialized driver IRQ request processing failed' }

& (Join-Path $PSScriptRoot 'build_transfer_status_getters.ps1')
& $python (Join-Path $PSScriptRoot 'verify_transfer_status_getters.py')
if ($LASTEXITCODE -ne 0) { throw 'Transfer status getters failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_service_status_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound service native status integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_irq_request_status_initialized.py')
if ($LASTEXITCODE -ne 0) { throw 'Initialized IRQ request native status failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_frame_services_queue_routing.py')
if ($LASTEXITCODE -ne 0) { throw 'Frame services queue routing failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_frame_async_registry_completion.py')
if ($LASTEXITCODE -ne 0) { throw 'Async event registry completion failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_frame_async_registered_completion.py')
if ($LASTEXITCODE -ne 0) { throw 'Async native registered completion failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_frame_async_registered_queue_completion.py')
if ($LASTEXITCODE -ne 0) { throw 'Async registered queued completion failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_scheduled_upload.py')
if ($LASTEXITCODE -ne 0) { throw 'Scheduled native driver loader upload failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_registered_upload.py')
if ($LASTEXITCODE -ne 0) { throw 'Native registered driver upload failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_file_metadata.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_file_metadata.py')
if ($LASTEXITCODE -ne 0) { throw 'Native driver file metadata integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_native_open.py')
if ($LASTEXITCODE -ne 0) { throw 'Native driver open integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_native_read.py')
if ($LASTEXITCODE -ne 0) { throw 'Native driver sector-read integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_mounted_sound_driver_read.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted native sound-driver file read failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_mounted_sound_driver_dispatch_read.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted driver dispatcher read failed' }

& (Join-Path $PSScriptRoot 'build_file_cache_helpers.ps1')
& $python (Join-Path $PSScriptRoot 'verify_file_cache_invalidate.py')
if ($LASTEXITCODE -ne 0) { throw 'File cache invalidate failed' }
& $python (Join-Path $PSScriptRoot 'verify_file_cache_purge.py')
if ($LASTEXITCODE -ne 0) { throw 'File cache purge failed' }
& $python (Join-Path $PSScriptRoot 'verify_file_mounted_sound_driver_cache_read.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted driver cache read failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_cache_invalidate_large.py')
if ($LASTEXITCODE -ne 0) { throw 'Whole-cache invalidate failed' }
& $python (Join-Path $PSScriptRoot 'verify_file_cache_purge_large.py')
if ($LASTEXITCODE -ne 0) { throw 'Whole-cache purge failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_mounted_directory.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted directory driver loader failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_mounted_handles.py')
if ($LASTEXITCODE -ne 0) { throw 'Mounted handles driver loader failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_native_bios_request.py')
if ($LASTEXITCODE -ne 0) { throw 'Native driver BIOS request integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_native_device_start.py')
if ($LASTEXITCODE -ne 0) { throw 'Native driver device start failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_load_initialize_native_file_completion.py')
if ($LASTEXITCODE -ne 0) { throw 'Native driver file completion integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_slot_select.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_slot_select.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound slot selection failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_record_create.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_record_create.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound record creation failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_cancel_identity.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_cancel.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound create/cancel integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_record_find.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_find.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound record creation/lookup failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_record_refresh.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_record_refresh.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound record refresh failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_records_update.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_records_update.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound records update failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound creation/update integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_start_dispatch.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_start.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound create/update/start integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_program_setup.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound creation/update/program setup integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_midi_pack.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound creation/update/MIDI pack integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_midi_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound creation/update/MIDI enqueue integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_commit.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound creation/update/request commit integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_consume.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound creation/update/request consumption integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound creation/update/request dispatch integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound voice request transfer/completion integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_readiness.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound voice native readiness/transfer integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_note_on.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound voice native note-on/transfer integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_volume.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound voice native volume/transfer integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_global_volume.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_global_volume.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound voice global volume integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_global_volume_request.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_global_volume_request.py')
if ($LASTEXITCODE -ne 0) { throw 'Global volume request verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_global_commit.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound voice global request commit/transfer integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_alternate_bank_predicate.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_alternate_bank_predicate.py')
if ($LASTEXITCODE -ne 0) { throw 'Alternate bank predicate verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_alternate_bank.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice alternate bank integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_alternate_active.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice alternate activation integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_reserved_play.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice reserved playback integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_reserved_volume.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice reserved volume integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_reserved_alternate.py')
if ($LASTEXITCODE -ne 0) { throw 'Reserved alternate-bank request integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_debug_log.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice native debug log integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_debug_flush.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_debug_flush.py')
if ($LASTEXITCODE -ne 0) { throw 'Debug flush verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_debug_flush.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice debug flush integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_debug_format.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_debug_format.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice native debug formatting integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_format_engine.ps1')

& (Join-Path $PSScriptRoot 'build_sound_format_engine_runtime.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_format_engine.py')
if ($LASTEXITCODE -ne 0) { throw 'Formatting engine literal verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_format_engine_character.py')
if ($LASTEXITCODE -ne 0) { throw 'Formatting character conversion verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_format_engine_arguments.py')
if ($LASTEXITCODE -ne 0) { throw 'Formatting argument conversion verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_format_engine_integer.py')
if ($LASTEXITCODE -ne 0) { throw 'Formatting integer conversion verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_format_engine_width_precision.py')
if ($LASTEXITCODE -ne 0) { throw 'Formatting width precision verification failed' }

& (Join-Path $PSScriptRoot 'build_format_emit.ps1')
& $python (Join-Path $PSScriptRoot 'verify_format_emit.py')
if ($LASTEXITCODE -ne 0) { throw 'Format emitter verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_format_engine_native_emit.py')
if ($LASTEXITCODE -ne 0) { throw 'Native format emitter integration failed' }

& (Join-Path $PSScriptRoot 'build_format_output_helpers.ps1')
& $python (Join-Path $PSScriptRoot 'verify_format_output_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'Format output helper verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_format_engine_native_output.py')
if ($LASTEXITCODE -ne 0) { throw 'Native formatting output integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_format_engine_native_mixed.py')
if ($LASTEXITCODE -ne 0) { throw 'Native mixed formatting verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_debug_format_native_engine.py')
if ($LASTEXITCODE -ne 0) { throw 'Native mixed formatting verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_native_format.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice request native formatter integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_level.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_level.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice level verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_level_distance.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice distance level verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_native_level.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice request native level integration failed' }

& (Join-Path $PSScriptRoot 'build_sh4_sqrt.ps1')
& $python (Join-Path $PSScriptRoot 'verify_native_sh4_sqrt.py')
if ($LASTEXITCODE -ne 0) { throw 'Native square root verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_level_native_sqrt.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice level native square root integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_create_update_request_native_distance.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice request native distance integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_pan.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_pan.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice panning verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_position_transform.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_position_transform.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound position transform verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_pan_native_transform.py')
if ($LASTEXITCODE -ne 0) { throw 'Panning native transform integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_pan_native_normalize.py')
if ($LASTEXITCODE -ne 0) { throw 'Panning native normalization integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_reservation_release.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_reservation_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice reservation release verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_reservation_acquire.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_reservation_acquire.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice reservation acquisition verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_reservation_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice reservation lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_records_update_reservation.py')
if ($LASTEXITCODE -ne 0) { throw 'Updater reservation integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_records_update_native_pan.py')
if ($LASTEXITCODE -ne 0) { throw 'Updater native panning integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_stop_dispatch.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_stop_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice stop dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_records_update_native_stop.py')
if ($LASTEXITCODE -ne 0) { throw 'Updater native stop integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_records_update_native_note_off.py')
if ($LASTEXITCODE -ne 0) { throw 'Updater native note off integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_records_update_native_note_off_pack.py')
if ($LASTEXITCODE -ne 0) { throw 'Updater native note off packet verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_records_update_native_note_off_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Updater native note off enqueue verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_records_update_native_note_off_consume.py')
if ($LASTEXITCODE -ne 0) { throw 'Updater note off native consumption failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_records_update_native_note_off_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Updater native note off dispatch failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_reserved_stop_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Reserved voice stop enqueue verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_reserved_stop_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Reserved voice stop work dispatch verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_mute_all.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_mute_all.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice mute all verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_restore_all.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_restore_all.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice restore all verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_restore_all_native_volume.py')
if ($LASTEXITCODE -ne 0) { throw 'Restore all native volume integration failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_restore_all_native_midi.py')
if ($LASTEXITCODE -ne 0) { throw 'Restore all native MIDI verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_restore_all_native_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Restore all native enqueue verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_restore_all_native_consume.py')
if ($LASTEXITCODE -ne 0) { throw 'Restore all native consumption verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_restore_all_native_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Restore all native work dispatch verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_restore_all_native_reserved.py')
if ($LASTEXITCODE -ne 0) { throw 'Restore all native reserved volume verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_restore_all_native_second_batch.py')
if ($LASTEXITCODE -ne 0) { throw 'Restore all native second batch verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_mute_restore_native_batches.py')
if ($LASTEXITCODE -ne 0) { throw 'Mute restore native batch verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_voice_stop_all.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_stop_all.py')
if ($LASTEXITCODE -ne 0) { throw 'Voice stop all verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_stop_all_native_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Stop all native dispatch verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_stop_all_native_note_off.py')
if ($LASTEXITCODE -ne 0) { throw 'Stop all native note off verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_stop_all_native_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Stop all native enqueue verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_stop_all_native_batches.py')
if ($LASTEXITCODE -ne 0) { throw 'Stop all native batches verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_mute_service.ps1')
& (Join-Path $PSScriptRoot 'build_sound_restore_service.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_mute_restore_services.py')
if ($LASTEXITCODE -ne 0) { throw 'Mute restore final services verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_mute_restore_native_final_services.py')
if ($LASTEXITCODE -ne 0) { throw 'Mute restore native final services integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_handle_volume.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_handle_volume.py')
if ($LASTEXITCODE -ne 0) { throw 'Handle volume verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_mute_restore_native_handle_volume.py')
if ($LASTEXITCODE -ne 0) { throw 'Mute restore native handle volume integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_subordinate_volume.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_volume.py')
if ($LASTEXITCODE -ne 0) { throw 'Subordinate volume verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_handle_volume_thunk.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_voice_mute_restore_native_subordinate_volume.py')
if ($LASTEXITCODE -ne 0) { throw 'Mute restore native subordinate volume integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_volume.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_volume.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver volume verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_mute_restore_native_driver_volume.py')
if ($LASTEXITCODE -ne 0) { throw 'Mute restore native driver volume integration failed' }

& (Join-Path $PSScriptRoot 'build_sound_channel_volume.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_channel_volume.py')
if ($LASTEXITCODE -ne 0) { throw 'Channel volume verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_channel_volume_native_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Channel volume native enqueue verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_channel_volume_thunk.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_volume_native_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver volume native enqueue verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_mute_restore_native_full_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Mute restore native full enqueue verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_mute_restore_native_readiness.py')
if ($LASTEXITCODE -ne 0) { throw 'Mute restore native readiness verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_channel_volume_context_errors.py')
if ($LASTEXITCODE -ne 0) { throw 'Channel volume context errors verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_mute_restore_native_status_word.py')
if ($LASTEXITCODE -ne 0) { throw 'Mute restore native status word verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_mute_restore_native_status_bus.py')
if ($LASTEXITCODE -ne 0) { throw 'Mute restore native status bus verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_voice_mute_restore_native_status_modes.py')
if ($LASTEXITCODE -ne 0) { throw 'Mute restore native status modes verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_control_bundle.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_control_bundle.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound control bundle verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_control_6682be.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_control_6682be.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver control6682be verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_setup_wrapper.ps1')
& (Join-Path $PSScriptRoot 'build_sound_driver_level_wrapper.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_byte_wrappers.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver byte wrappers verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_control_native_wrappers.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver control native wrappers verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_control_setup_request.ps1')
& (Join-Path $PSScriptRoot 'build_sound_control_level_request.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_control_requests.py')
if ($LASTEXITCODE -ne 0) { throw 'Control requests verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_control_native_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver control native enqueue verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_control_native_consume.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver control native consume verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_control_native_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver control native dispatch verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_control_native_completion.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver control native completion verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_control_native_readiness.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver control native readiness verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_control_native_status_word.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver control native status_word verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_control_native_status_bus.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver control native status_bus verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_control_native_status_modes.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver control native status modes verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_control_bundle.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_control_bundle.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound driver control bundle verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_pan.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_pan.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound driver pan verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_pan_wrapper.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_pan_native_wrapper.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound driver pan wrapper verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_pan_request.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_pan_request.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound pan request verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_pan_request_native_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Pan request native_enqueue verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_pan_request_context_errors.py')
if ($LASTEXITCODE -ne 0) { throw 'Pan request context_errors verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_pan_native_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver pan native enqueue verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_pan_native_consume.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver pan native consume verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_pan_native_completion.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver pan native completion verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_pan_native_status_bus.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver pan native status_bus verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_pitch.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_pitch.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound driver pitch verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_pitch_wrapper.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_pitch_native_wrapper.py')
if ($LASTEXITCODE -ne 0) { throw 'Pitch wrapper verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_pitch_wrapper_direct.py')
if ($LASTEXITCODE -ne 0) { throw 'Pitch wrapper verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_pitch_request.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_pitch_request.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound pitch request verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_pitch_request_native_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Pitch native enqueue verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_pitch_native_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Pitch native enqueue verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_pitch_native_completion.py')
if ($LASTEXITCODE -ne 0) { throw 'Pitch completion verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_pitch_native_status_bus.py')
if ($LASTEXITCODE -ne 0) { throw 'Pitch status bus verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_channel_count.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_channel_count.py')
if ($LASTEXITCODE -ne 0) { throw 'Channel count verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_channel_count_wrapper.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_channel_count_native_wrapper.py')
if ($LASTEXITCODE -ne 0) { throw 'Channel count verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_channel_count.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_channel_count.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound channel count verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_channel_count_native_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Channel count native enqueue verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_channel_count_native_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Channel count native enqueue verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_flush.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_flush.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound driver flush verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_host_flush.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_host_flush.py')
if ($LASTEXITCODE -ne 0) { throw 'Host flush verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_flush_native_host.py')
if ($LASTEXITCODE -ne 0) { throw 'Host flush verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_pan_native_flush.py')
if ($LASTEXITCODE -ne 0) { throw 'Pan native flush verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_pitch_native_flush.py')
if ($LASTEXITCODE -ne 0) { throw 'Pitch native flush verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_block_transfer.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_block_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound driver block transfer verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_block_transfer_native_configure.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver block transfer native configure verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_block_transfer_native_clock.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver block transfer native clock verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_block_transfer_native_consume.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver block transfer native consume verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_block_transfer_native_begin.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver block transfer native begin verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_block_transfer_native_submit_alignment.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver block transfer submit alignment verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_block_transfer_native_submit.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver block transfer aligned submit verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_block_transfer_native_services.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver block transfer native frame services verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_block_transfer_native_bus.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver block transfer native bus verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_fixed_block_transfer.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_fixed_block_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Fixed block transfer verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_fixed_block_transfer_native_bus.py')
if ($LASTEXITCODE -ne 0) { throw 'Fixed block transfer native bus verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_state_query.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_state_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver state query verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_resource_query.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_resource_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver resource query verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_resource_query_native_lookup.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver native resource lookup verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_resource_index_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_resource_query_native_index.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver native resource index verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_zero_queries.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_zero_queries.py')
if ($LASTEXITCODE -ne 0) { throw 'Zero query verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_control_bundle_recent.py')
if ($LASTEXITCODE -ne 0) { throw 'Combined recent driver verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_power_two_factor.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_power_two_factor.py')
if ($LASTEXITCODE -ne 0) { throw 'Power two factor verification failed' }

& (Join-Path $PSScriptRoot 'build_integer_remainder.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_power_two_factor_native_remainder.py')
if ($LASTEXITCODE -ne 0) { throw 'Native remainder factor verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_integer_remainder.py')
if ($LASTEXITCODE -ne 0) { throw 'Integer remainder verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_buffered_transfer.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_buffered_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Buffered transfer verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_buffer_read.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_buffer_read.py')
if ($LASTEXITCODE -ne 0) { throw 'G2 buffer read verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_buffer_read_native_bus.py')
if ($LASTEXITCODE -ne 0) { throw 'Native G2 buffer read verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_buffered_transfer_native_read.py')
if ($LASTEXITCODE -ne 0) { throw 'Buffered native G2 read verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_buffered_transfer_native_block.py')
if ($LASTEXITCODE -ne 0) { throw 'Buffered native block verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_buffered_transfer_native_complete.py')
if ($LASTEXITCODE -ne 0) { throw 'Buffered native completion verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_buffered_dispatch.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_buffered_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Buffered dispatcher verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_buffered_dispatch_native_buffered.py')
if ($LASTEXITCODE -ne 0) { throw 'Native buffered dispatcher verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_buffered_dispatch_native_complete.py')
if ($LASTEXITCODE -ne 0) { throw 'Dispatcher native completion verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_frame_mode_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_buffered_dispatch_native_modes.py')
if ($LASTEXITCODE -ne 0) { throw 'Dispatcher native frame mode verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_position_adjust.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_position_adjust.py')
if ($LASTEXITCODE -ne 0) { throw 'Position adjustment verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_position_advance.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_position_advance.py')
if ($LASTEXITCODE -ne 0) { throw 'Position advance verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_split_range.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_split_range.py')
if ($LASTEXITCODE -ne 0) { throw 'Descriptor split verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_swap_words.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_swap_words.py')
if ($LASTEXITCODE -ne 0) { throw 'Word swap verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_work_reset.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_work_reset.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver work reset verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_slot_allocate.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_slot_allocate.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver slot allocation verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_work_enqueue.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_work_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Driver work entry verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_descriptor_byte_query.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_descriptor_byte_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Descriptor byte query verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_descriptor_byte_query_native_bus.py')
if ($LASTEXITCODE -ne 0) { throw 'Descriptor byte query native bus verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_channel_group_setup.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_channel_group_setup.py')
if ($LASTEXITCODE -ne 0) { throw 'Channel group setup verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_channel_group_setup_native_slot.py')
if ($LASTEXITCODE -ne 0) { throw 'Channel group native slot verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_group_control_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_group_control_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Group control verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_channel_group_setup_native_control.py')
if ($LASTEXITCODE -ne 0) { throw 'Group native control verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_channel_group_setup_native_complete.py')
if ($LASTEXITCODE -ne 0) { throw 'Channel group complete native setup verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_rate_table_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_rate_table_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound rate table verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_group_pitch_command.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_group_pitch_command.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound group pitch verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_rate_table_set_native_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Rate table native enqueue verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_rate_request_consume.py')
if ($LASTEXITCODE -ne 0) { throw 'Sample rate generation and consumption verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_descriptor_command2e.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_descriptor_command2e.py')
if ($LASTEXITCODE -ne 0) { throw 'Descriptor command2E verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_mixed_request_consume.py')
if ($LASTEXITCODE -ne 0) { throw 'Mixed sound generation/consumption verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_aiff_parse.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_aiff_parse.py')
if ($LASTEXITCODE -ne 0) { throw 'AIFF parser verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_aiff_parse_retail_assets.py')
if ($LASTEXITCODE -ne 0) { throw 'Retail AIFF asset verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_aiff_metadata.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_aiff_metadata_retail_assets.py')
if ($LASTEXITCODE -ne 0) { throw 'AIFF metadata verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_aiff_header_detect.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_aiff_header_detect.py')
if ($LASTEXITCODE -ne 0) { throw 'AIFF detector verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_aiff_state_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_aiff_state_initialize_retail_assets.py')
if ($LASTEXITCODE -ne 0) { throw 'AIFF state initialization verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_aiff_transfer_step.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_aiff_transfer_step.py')
if ($LASTEXITCODE -ne 0) { throw 'AIFF transfer step verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_aiff_transfer_step_native_pending.py')
if ($LASTEXITCODE -ne 0) { throw 'AIFF native pending verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_aiff_transfer8_step.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_aiff_transfer8_step_native_pending.py')
if ($LASTEXITCODE -ne 0) { throw 'AIFF8 transfer step verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_aiff_transfer_dispatch.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_aiff_transfer_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'AIFF dispatch verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_snd_parse.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_snd_parse.py')
if ($LASTEXITCODE -ne 0) { throw 'SND parser verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_snd_metadata.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_snd_metadata.py')
if ($LASTEXITCODE -ne 0) { throw 'SND metadata verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_snd_state_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_snd_state_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'SND state verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_snd_transfer16_step.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_snd_transfer16_step_native_pending.py')
if ($LASTEXITCODE -ne 0) { throw 'SND transfer verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_snd_transfer8_step.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_snd_transfer8_step_native_pending.py')
if ($LASTEXITCODE -ne 0) { throw 'SND transfer verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_snd_transfer_dispatch.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_snd_transfer_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'SND dispatch verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_packed_header_metadata.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_packed_header_metadata.py')
if ($LASTEXITCODE -ne 0) { throw 'Packed audio metadata verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_packed_state_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_packed_state_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Packed audio state verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_packed_transfer_step.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_packed_transfer_step_native_pending.py')
if ($LASTEXITCODE -ne 0) { throw 'Packed audio transfer verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_data_chunk_metadata.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_data_chunk_metadata.py')
if ($LASTEXITCODE -ne 0) { throw 'Data-chunk metadata verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_data_chunk_state_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_data_chunk_state_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Data-chunk state verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_data_transfer16_step.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_data_transfer16_step_native_pending.py')
if ($LASTEXITCODE -ne 0) { throw 'Data-chunk transfer verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_data_transfer8_step.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_data_transfer8_step_native_pending.py')
if ($LASTEXITCODE -ne 0) { throw 'Data-chunk transfer verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_data_bytepair_transfer_step.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_data_bytepair_transfer_step_native_pending.py')
if ($LASTEXITCODE -ne 0) { throw 'Data encoding verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_data_transfer_dispatch.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_data_transfer_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Data encoding verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_header_detectors.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_header_detectors.py')
if ($LASTEXITCODE -ne 0) { throw 'Audio detector verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_wave_retail_pipeline.py')
if ($LASTEXITCODE -ne 0) { throw 'Original WAVE pipeline verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_small_slot_pool.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_small_slot_pool.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound slot pool verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_format_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_format_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Audio format selector verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_wave_format_pipeline.py')
if ($LASTEXITCODE -ne 0) { throw 'Original WAVE format pipeline verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_magic_state_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_magic_state_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Magic audio-state verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_magic_header_metadata.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_magic_header_metadata.py')
if ($LASTEXITCODE -ne 0) { throw 'Magic audio metadata verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_magic_extension_word.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_magic_extension_word.py')
if ($LASTEXITCODE -ne 0) { throw 'Magic extension word verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_magic_stream_parameters.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_magic_stream_parameters.py')
if ($LASTEXITCODE -ne 0) { throw 'Magic stream parameters verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_magic_retail_headers.py')
if ($LASTEXITCODE -ne 0) { throw 'Original magic headers verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_magic_retail_initializer.py')
if ($LASTEXITCODE -ne 0) { throw 'Original magic initializer verification failed' }

& (Join-Path $PSScriptRoot 'build_file_audio_configuration_forward.ps1')
& $python (Join-Path $PSScriptRoot 'verify_file_audio_configuration_forward.py')
if ($LASTEXITCODE -ne 0) { throw 'File audio configuration forwarding verification failed' }

& (Join-Path $PSScriptRoot 'build_audio_filter_coefficients.ps1')
& $python (Join-Path $PSScriptRoot 'verify_audio_filter_coefficients.py')
if ($LASTEXITCODE -ne 0) { throw 'Audio coefficient verification failed' }

& (Join-Path $PSScriptRoot 'build_math_result_error_wrapper.ps1')
& $python (Join-Path $PSScriptRoot 'verify_math_result_error_wrapper.py')
if ($LASTEXITCODE -ne 0) { throw 'Math result wrapper verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_audio_filter_wrapper.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected audio filter wrapper verification failed' }

& (Join-Path $PSScriptRoot 'build_math_trig_octant_dispatch.ps1')
& $python (Join-Path $PSScriptRoot 'verify_math_trig_octant_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Math octant dispatch verification failed' }

& (Join-Path $PSScriptRoot 'build_math_trig_polynomials.ps1')
& $python (Join-Path $PSScriptRoot 'verify_math_trig_polynomials.py')
if ($LASTEXITCODE -ne 0) { throw 'Trigonometric polynomial verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_math_trig_native_polynomials.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected trig polynomial verification failed' }

& (Join-Path $PSScriptRoot 'build_math_range_reduce.ps1')
& $python (Join-Path $PSScriptRoot 'verify_math_range_reduce.py')
if ($LASTEXITCODE -ne 0) { throw 'Native range reducer verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_math_range_reduce_large.py')
if ($LASTEXITCODE -ne 0) { throw 'Large quotient range reduction verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_math_trig_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native trig integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_audio_filter_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native audio coefficient integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_audio_configuration_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native audio coefficient integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_magic_retail_native_initializer.py')
if ($LASTEXITCODE -ne 0) { throw 'Original-header native initializer verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_decode_blocks_draft.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_decode_block_mono.py')
if ($LASTEXITCODE -ne 0) { throw 'Mono decoder verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_decode_mono_scales.py')
if ($LASTEXITCODE -ne 0) { throw 'Mono decoder scales verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_decode_mono_retail.py')
if ($LASTEXITCODE -ne 0) { throw 'Mono decoder scales verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_decode_blocks_draft.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_decode_stereo_samples.py')
if ($LASTEXITCODE -ne 0) { throw 'Mono decoder verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_decode_stereo_retail.py')
if ($LASTEXITCODE -ne 0) { throw 'Mono decoder scales verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_decode_stereo_prefix.py')
if ($LASTEXITCODE -ne 0) { throw 'Mono decoder scales verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_decode_stereo_odd.py')
if ($LASTEXITCODE -ne 0) { throw 'Mono decoder scales verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_decode_stereo_termination.py')
if ($LASTEXITCODE -ne 0) { throw 'Mono decoder scales verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_decode_stereo_scales.py')
if ($LASTEXITCODE -ne 0) { throw 'Mono decoder scales verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_counter_increment.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_counter_increment.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound counter increment verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sound_counter_increment.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound counter increment registration failed' }

& (Join-Path $PSScriptRoot 'build_input_device_record_getters.ps1')
& $python (Join-Path $PSScriptRoot 'verify_input_device_record_getters.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-device record getter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_input_device_record_getters.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-device record getter registration failed' }

& (Join-Path $PSScriptRoot 'build_input_device_record_set_pair.ps1')
& $python (Join-Path $PSScriptRoot 'verify_input_device_record_set_pair.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-device pair setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_input_device_record_set_pair.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-device pair setter registration failed' }

& (Join-Path $PSScriptRoot 'build_global_word_store_8c043998.ps1')
& $python (Join-Path $PSScriptRoot 'verify_global_word_store_8c043998.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_word_store_8c043998.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word store registration failed' }

& (Join-Path $PSScriptRoot 'build_global_word_pair_store_8c019368.ps1')
& $python (Join-Path $PSScriptRoot 'verify_global_word_pair_store_8c019368.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word pair store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_word_pair_store_8c019368.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word pair store registration failed' }

& (Join-Path $PSScriptRoot 'build_global_word_flag_copy_8c036ffc.ps1')
& $python (Join-Path $PSScriptRoot 'verify_global_word_flag_copy_8c036ffc.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word flag copy verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_word_flag_copy_8c036ffc.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word flag copy registration failed' }

& (Join-Path $PSScriptRoot 'build_indirect_byte_getters_8c01b406.ps1')
& $python (Join-Path $PSScriptRoot 'verify_indirect_byte_getters_8c01b406.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect byte getters verification failed' }
& $python (Join-Path $PSScriptRoot 'register_indirect_byte_getters_8c01b406.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect byte getters registration failed' }

& (Join-Path $PSScriptRoot 'build_global_signed_byte_word_store_8c04f8ec.ps1')
& $python (Join-Path $PSScriptRoot 'verify_global_signed_byte_word_store_8c04f8ec.py')
if ($LASTEXITCODE -ne 0) { throw 'Global signed-byte word store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_signed_byte_word_store_8c04f8ec.py')
if ($LASTEXITCODE -ne 0) { throw 'Global signed-byte word store registration failed' }

& (Join-Path $PSScriptRoot 'build_global_u16_get_8c04f698.ps1')
& $python (Join-Path $PSScriptRoot 'verify_global_u16_get_8c04f698.py')
if ($LASTEXITCODE -ne 0) { throw 'Global unsigned-word getter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_u16_get_8c04f698.py')
if ($LASTEXITCODE -ne 0) { throw 'Global unsigned-word getter registration failed' }

& (Join-Path $PSScriptRoot 'build_global_u8_pair_store_8c04f786.ps1')
& $python (Join-Path $PSScriptRoot 'verify_global_u8_pair_store_8c04f786.py')
if ($LASTEXITCODE -ne 0) { throw 'Global unsigned-byte pair store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_u8_pair_store_8c04f786.py')
if ($LASTEXITCODE -ne 0) { throw 'Global unsigned-byte pair store registration failed' }

& (Join-Path $PSScriptRoot 'build_global_word_pair_copy_8c04f6a6.ps1')
& $python (Join-Path $PSScriptRoot 'verify_global_word_pair_copy_8c04f6a6.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word pair copy verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_word_pair_copy_8c04f6a6.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word pair copy registration failed' }

& (Join-Path $PSScriptRoot 'build_global_u8_get_8c0509b0.ps1')
& $python (Join-Path $PSScriptRoot 'verify_global_u8_get_8c0509b0.py')
if ($LASTEXITCODE -ne 0) { throw 'Global unsigned-byte getter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_u8_get_8c0509b0.py')
if ($LASTEXITCODE -ne 0) { throw 'Global unsigned-byte getter registration failed' }

& (Join-Path $PSScriptRoot 'build_global_word_copy_8c056972.ps1')
& $python (Join-Path $PSScriptRoot 'verify_global_word_copy_8c056972.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word copy verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_word_copy_8c056972.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word copy registration failed' }

& (Join-Path $PSScriptRoot 'build_global_word_copy_8c05697c.ps1')
& $python (Join-Path $PSScriptRoot 'verify_global_word_copy_8c05697c.py')
if ($LASTEXITCODE -ne 0) { throw 'Reverse global word copy verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_word_copy_8c05697c.py')
if ($LASTEXITCODE -ne 0) { throw 'Reverse global word copy registration failed' }

& (Join-Path $PSScriptRoot 'build_global_word_bit_updates_8c056986.ps1')
& $python (Join-Path $PSScriptRoot 'verify_global_word_bit_updates_8c056986.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word bit updates verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_word_bit_updates_8c056986.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word bit updates registration failed' }

& (Join-Path $PSScriptRoot 'build_constant_field_store_8c098786.ps1')
& $python (Join-Path $PSScriptRoot 'verify_constant_field_store_8c098786.py')
if ($LASTEXITCODE -ne 0) { throw 'Constant field store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_constant_field_store_8c098786.py')
if ($LASTEXITCODE -ne 0) { throw 'Constant field store registration failed' }

& (Join-Path $PSScriptRoot 'build_task_work_field_8c099d38.ps1')
& $python (Join-Path $PSScriptRoot 'verify_task_work_field_8c099d38.py')
if ($LASTEXITCODE -ne 0) { throw 'Task work field verification failed' }
& $python (Join-Path $PSScriptRoot 'register_task_work_field_8c099d38.py')
if ($LASTEXITCODE -ne 0) { throw 'Task work field registration failed' }

& (Join-Path $PSScriptRoot 'build_task_cursor_advance_8c099a1c.ps1')
& $python (Join-Path $PSScriptRoot 'verify_task_cursor_advance_8c099a1c.py')
if ($LASTEXITCODE -ne 0) { throw 'Task cursor advance verification failed' }
& $python (Join-Path $PSScriptRoot 'register_task_cursor_advance_8c099a1c.py')
if ($LASTEXITCODE -ne 0) { throw 'Task cursor advance registration failed' }

& (Join-Path $PSScriptRoot 'build_task_flag_access_8c091b54.ps1')
& $python (Join-Path $PSScriptRoot 'verify_task_flag_access_8c091b54.py')
if ($LASTEXITCODE -ne 0) { throw 'Task flag access verification failed' }
& $python (Join-Path $PSScriptRoot 'register_task_flag_access_8c091b54.py')
if ($LASTEXITCODE -ne 0) { throw 'Task flag access registration failed' }

& (Join-Path $PSScriptRoot 'build_task_count_predicate_8c021868.ps1')
& $python (Join-Path $PSScriptRoot 'verify_task_count_predicate_8c021868.py')
if ($LASTEXITCODE -ne 0) { throw 'Task count predicate verification failed' }
& $python (Join-Path $PSScriptRoot 'register_task_count_predicate_8c021868.py')
if ($LASTEXITCODE -ne 0) { throw 'Task count predicate registration failed' }

& (Join-Path $PSScriptRoot 'build_task_vector_copy_8c047d7e.ps1')
& $python (Join-Path $PSScriptRoot 'verify_task_vector_copy_8c047d7e.py')
if ($LASTEXITCODE -ne 0) { throw 'Task vector copy verification failed' }
& $python (Join-Path $PSScriptRoot 'register_task_vector_copy_8c047d7e.py')
if ($LASTEXITCODE -ne 0) { throw 'Task vector copy registration failed' }

& (Join-Path $PSScriptRoot 'build_task_vector_pair_copy_8c047d90.ps1')
& $python (Join-Path $PSScriptRoot 'verify_task_vector_pair_copy_8c047d90.py')
if ($LASTEXITCODE -ne 0) { throw 'Task vector pair verification failed' }
& $python (Join-Path $PSScriptRoot 'register_task_vector_pair_copy_8c047d90.py')
if ($LASTEXITCODE -ne 0) { throw 'Task vector pair registration failed' }

& (Join-Path $PSScriptRoot 'build_global_mode_predicates_8c04fe40.ps1')
& $python (Join-Path $PSScriptRoot 'verify_global_mode_predicates_8c04fe40.py')
if ($LASTEXITCODE -ne 0) { throw 'Global mode predicate verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_mode_predicates_8c04fe40.py')
if ($LASTEXITCODE -ne 0) { throw 'Global mode predicate registration failed' }

& (Join-Path $PSScriptRoot 'build_global_mode_nonzero_8c049a4e.ps1')
& $python (Join-Path $PSScriptRoot 'verify_global_mode_nonzero_8c049a4e.py')
if ($LASTEXITCODE -ne 0) { throw 'Global mode nonzero predicate verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_mode_nonzero_8c049a4e.py')
if ($LASTEXITCODE -ne 0) { throw 'Global mode nonzero predicate registration failed' }

& (Join-Path $PSScriptRoot 'build_global_vector_copy_8c049160.ps1')
& $python (Join-Path $PSScriptRoot 'verify_global_vector_copy_8c049160.py')
if ($LASTEXITCODE -ne 0) { throw 'Global vector copy verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_vector_copy_8c049160.py')
if ($LASTEXITCODE -ne 0) { throw 'Global vector copy registration failed' }

& (Join-Path $PSScriptRoot 'build_task_work_field_clear_8c04bd60.ps1')
& $python (Join-Path $PSScriptRoot 'verify_task_work_field_clear_8c04bd60.py')
if ($LASTEXITCODE -ne 0) { throw 'Task work field clear verification failed' }
& $python (Join-Path $PSScriptRoot 'register_task_work_field_clear_8c04bd60.py')
if ($LASTEXITCODE -ne 0) { throw 'Task work field clear registration failed' }

& (Join-Path $PSScriptRoot 'build_input_device_record_index_helpers.ps1')
& $python (Join-Path $PSScriptRoot 'verify_input_device_record_index_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-device index helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_input_device_record_index_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-device index helper registration failed' }

& (Join-Path $PSScriptRoot 'build_input_device_record_status.ps1')
& $python (Join-Path $PSScriptRoot 'verify_input_device_record_status.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-device record status verification failed' }
& $python (Join-Path $PSScriptRoot 'register_input_device_record_status.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-device record status registration failed' }

& (Join-Path $PSScriptRoot 'build_input_device_record_mode.ps1')
& $python (Join-Path $PSScriptRoot 'verify_input_device_record_mode.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-device record mode verification failed' }
& $python (Join-Path $PSScriptRoot 'register_input_device_record_mode.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-device record mode registration failed' }

& (Join-Path $PSScriptRoot 'build_file_decode_request_process.ps1')
& $python (Join-Path $PSScriptRoot 'verify_file_decode_request_process.py')
if ($LASTEXITCODE -ne 0) { throw 'File decode dispatcher verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_decode_retail_pipeline.py')
if ($LASTEXITCODE -ne 0) { throw 'File-dispatch stereo pipeline verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_decode_retained_history.py')
if ($LASTEXITCODE -ne 0) { throw 'File-dispatch stereo pipeline verification failed' }

& (Join-Path $PSScriptRoot 'build_file_decode_odd_flag_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_file_decode_odd_flag_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_decode_native_flag.py')
if ($LASTEXITCODE -ne 0) { throw 'Native file decoder flag integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_decode_native_odd.py')
if ($LASTEXITCODE -ne 0) { throw 'Native file decoder flag integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_decode_native_termination.py')
if ($LASTEXITCODE -ne 0) { throw 'Native file decoder flag integration verification failed' }

& (Join-Path $PSScriptRoot 'build_file_decode_stereo_request.ps1')
& $python (Join-Path $PSScriptRoot 'verify_file_decode_stereo_request.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_file_decode_request_activate.ps1')
& $python (Join-Path $PSScriptRoot 'verify_file_decode_request_activate.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_decode_request_sequence.py')
if ($LASTEXITCODE -ne 0) { throw 'File-dispatch stereo pipeline verification failed' }

& (Join-Path $PSScriptRoot 'build_file_decode_completion_acknowledge.ps1')
& $python (Join-Path $PSScriptRoot 'verify_file_decode_completion_acknowledge.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_decode_ack_sequence.py')
if ($LASTEXITCODE -ne 0) { throw 'File-dispatch stereo pipeline verification failed' }

& (Join-Path $PSScriptRoot 'build_file_decode_mono_request.ps1')
& $python (Join-Path $PSScriptRoot 'verify_file_decode_mono_request.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_file_decode_handle_reset.ps1')
& $python (Join-Path $PSScriptRoot 'verify_file_decode_handle_reset.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_decode_reset.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_decode_reset.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_decode_acknowledge.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_decode_acknowledge.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_mono_request.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_mono_request.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_stereo_request.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_stereo_request.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_stream_decode_pipeline.py')
if ($LASTEXITCODE -ne 0) { throw 'File-dispatch stereo pipeline verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_decode_schedule.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_decode_schedule.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_schedule_native_requests.py')
if ($LASTEXITCODE -ne 0) { throw 'File-dispatch stereo pipeline verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_scheduled_decode.py')
if ($LASTEXITCODE -ne 0) { throw 'File-dispatch stereo pipeline verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_sample_copy.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_sample_copy.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_mono_compact.ps1')
& (Join-Path $PSScriptRoot 'build_sound_stream_stereo_compact.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_compaction.py')
if ($LASTEXITCODE -ne 0) { throw 'Stream compaction verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_decode_complete.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_decode_complete.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_file_decode_result_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_file_decode_result_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_stream_completion_native_result.py')
if ($LASTEXITCODE -ne 0) { throw 'Native file decoder flag integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_stream_completion_native_compaction.py')
if ($LASTEXITCODE -ne 0) { throw 'Native file decoder flag integration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_update_dispatch.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_update_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_magic_update.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_magic_update.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_file_decode_phase_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_file_decode_phase_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_magic_update_native_phase.py')
if ($LASTEXITCODE -ne 0) { throw 'Native file decoder flag integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_magic_update_native_ack.py')
if ($LASTEXITCODE -ne 0) { throw 'Native file decoder flag integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_stream_dispatch_native_magic.py')
if ($LASTEXITCODE -ne 0) { throw 'Native file decoder flag integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_magic_update_retail_pipeline.py')
if ($LASTEXITCODE -ne 0) { throw 'File-dispatch stereo pipeline verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_magic_update_retail_compaction.py')
if ($LASTEXITCODE -ne 0) { throw 'File-dispatch stereo pipeline verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_start.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_start.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_input_callback_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_input_callback_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_output_callback_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_output_callback_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_magic_stream_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'File-dispatch stereo pipeline verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_input_provider.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_input_provider.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_channel_count_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_channel_count_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_position_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_position_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_provider_native_getters.py')
if ($LASTEXITCODE -ne 0) { throw 'Native file decoder flag integration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_buffer_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_buffer_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_provider_native_buffer.py')
if ($LASTEXITCODE -ne 0) { throw 'Native file decoder flag integration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_query.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound source query verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_provider_native_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_provider_native_remainder.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_lock_enter.ps1')
& (Join-Path $PSScriptRoot 'build_sound_source_lock_exit.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_locks.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound source interrupt mask verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_input_provider_native_locks.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_magic_lifecycle_native_input.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_default_output.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_default_output.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_magic_lifecycle_native_callbacks.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_create.ps1')
& (Join-Path $PSScriptRoot 'build_sound_stream_destroy.ps1')
& $python (Join-Path $PSScriptRoot 'verify_stream_allocation.py')
if ($LASTEXITCODE -ne 0) { throw 'Stream allocation verification failed' }

& (Join-Path $PSScriptRoot 'build_file_decode_handle_allocate.ps1')
& (Join-Path $PSScriptRoot 'build_file_decode_handle_release.ps1')
& $python (Join-Path $PSScriptRoot 'verify_file_handle_pool.py')
if ($LASTEXITCODE -ne 0) { throw 'File handle pool verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_file_handle_native_coefficients.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_stream_native_allocation.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_stream_pool_reuse.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_default_input.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_default_input.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_magic_lifecycle_default_callbacks.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_source_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_source_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_magic_lifecycle_native_source.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_magic_repeated_updates.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_magic_empty_tail.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_original_audio_terminal.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_work_complete.ps1')
& $python (Join-Path $PSScriptRoot 'verify_stream_work_complete.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_work_complete_native_ranges.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_commit.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_commit.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound source query verification failed' }

& (Join-Path $PSScriptRoot 'build_byte_move.ps1')
& $python (Join-Path $PSScriptRoot 'verify_byte_move.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_source_commit_native_copy.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_work_complete_native_ack.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_return.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_return.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound source query verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_work_complete_native_commits.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_total_samples_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_total_samples_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_decoded_bytes_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_decoded_bytes_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_decoded_samples_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_decoded_samples_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_work_complete_native_getters.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_work_complete_original_limits.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_work_tick.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_work_tick.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_work_feed.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_work_feed.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_capacity.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_capacity.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_feed_native_capacity.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_format_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_format_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_unit_count_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_unit_count_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_unit_count_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_unit_count_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_total_samples_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_total_samples_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_feed_native_metadata.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_feed_native_start.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_feed_native_reservations.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_feed_original_endings.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_phase_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_phase_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_tick_native_feed.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_tick_native_complete.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_tick_native_decode.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_tick_decode_complete.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_tick_idle_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_tick_native_provider.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_tick_final_pair.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_work_update.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_work_update.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_public_audio_final_pair.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_public_audio_terminal.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_work_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_work_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_initialize_native_source.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_initialize_original_headers.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_public_initialize_headers.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_public_initialize_retry.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_initialize_then_feed.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_pool_update.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_pool_update.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_pool_initialize_headers.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_pool_audio_final_pair.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_pool_audio_slot.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_sample_bits_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_sample_bits_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_initialize_buffer_bindings.py')
if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_initialized_decode.py')
if ($LASTEXITCODE -ne 0) { throw 'Persistent initialized audio decoding verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_initialized_backpressure.py')
if ($LASTEXITCODE -ne 0) { throw 'Persistent initialized audio decoding verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_initialized_two_chunks.py')
if ($LASTEXITCODE -ne 0) { throw 'Persistent initialized audio decoding verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_reset.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_reset.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound work reset verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_start.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_start.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound work start verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_create.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_create.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound work constructor verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_work_native_configuration.py')
if ($LASTEXITCODE -ne 0) { throw 'Native constructor callback configuration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_ring_size_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_ring_size_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_mirror_size_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_mirror_size_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_work_native_sizes.py')
if ($LASTEXITCODE -ne 0) { throw 'Native constructor callback configuration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_work_native_allocation.py')
if ($LASTEXITCODE -ne 0) { throw 'Native constructor callback configuration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_work_native_slots.py')
if ($LASTEXITCODE -ne 0) { throw 'Native constructor callback configuration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_destroy.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_destroy.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound work reset verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_critical_enter.ps1')
& (Join-Path $PSScriptRoot 'build_sound_critical_exit.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_critical_pair.py')
if ($LASTEXITCODE -ne 0) { throw 'Nested critical section verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_work_destroy_native_critical.py')
if ($LASTEXITCODE -ne 0) { throw 'Native constructor callback configuration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_work_destroy_native_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Native constructor callback configuration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_work_native_reuse.py')
if ($LASTEXITCODE -ne 0) { throw 'Native constructor callback configuration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_stop.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_stop.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound work reset verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_initialized_stop.py')
if ($LASTEXITCODE -ne 0) { throw 'Persistent initialized audio decoding verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_state_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_state_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_source_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_source_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_sample_limit_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_sample_limit_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_completion_callback_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_completion_callback_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_remaining_limit_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_remaining_limit_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_initialized_notification.py')
if ($LASTEXITCODE -ne 0) { throw 'Persistent initialized audio decoding verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_consumed_samples_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_consumed_samples_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_completed_samples_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_completed_samples_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_completed_bytes_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_completed_bytes_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_sample_count_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_sample_count_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_format_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_format_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_channel_count_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_channel_count_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_sample_bits_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_sample_bits_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_sample_rate_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_sample_rate_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_sample_rate_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_sample_rate_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_initialized_metadata.py')
if ($LASTEXITCODE -ne 0) { throw 'Native constructor callback configuration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_pool_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_pool_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound work reset verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_pool_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_pool_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound work reset verification failed' }

& (Join-Path $PSScriptRoot 'build_file_decode_pool_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_file_decode_pool_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound work reset verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_native_audio_pools.py')
if ($LASTEXITCODE -ne 0) { throw 'Native constructor callback configuration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_audio_initialized_allocation.py')
if ($LASTEXITCODE -ne 0) { throw 'Native constructor callback configuration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_audio_initialized_pool_capacity.py')
if ($LASTEXITCODE -ne 0) { throw 'Native constructor callback configuration verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_audio_full_pool_reuse.py')
if ($LASTEXITCODE -ne 0) { throw 'Native constructor callback configuration verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_channel_source_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_channel_source_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_initialized_channel_rebinding.py')
if ($LASTEXITCODE -ne 0) { throw 'Persistent initialized audio decoding verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_pool_clear.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_work_pool_clear.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound work reset verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_constructed_audio_decode.py')
if ($LASTEXITCODE -ne 0) { throw 'Persistent initialized audio decoding verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_constructed_audio_release.py')
if ($LASTEXITCODE -ne 0) { throw 'Persistent initialized audio decoding verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_contiguous_check.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_contiguous_check.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_reset.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_reset.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_create.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_create.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_pool_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_pool_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_destroy.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_destroy.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_source_pool_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Source pool connected lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_native_sources_audio_decode.py')
if ($LASTEXITCODE -ne 0) { throw 'Persistent initialized audio decoding verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_native_audio_consume_decode.py')
if ($LASTEXITCODE -ne 0) { throw 'Persistent initialized audio decoding verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_native_audio_refill.py')
if ($LASTEXITCODE -ne 0) { throw 'Persistent initialized audio decoding verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_native_audio_refilled_decode.py')
if ($LASTEXITCODE -ne 0) { throw 'Persistent initialized audio decoding verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_identity_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_identity_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_error_callback_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_error_callback_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_source_error_callback_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Source pool connected lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_native_audio_full_file.py')
if ($LASTEXITCODE -ne 0) { throw 'Complete native audio file verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_source_pool_counter_decrement.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_source_pool_counter_decrement.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_source_pool_reinitialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Source pool connected lifecycle verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_linear_source_identity_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_identity_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_linear_source_error_callback_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_error_callback_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_linear_source_reset.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_reset.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_linear_source_lock_enter.ps1')
& (Join-Path $PSScriptRoot 'build_sound_linear_source_lock_exit.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_locks.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound source interrupt mask verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_linear_source_capacity.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_capacity.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_linear_source_pool_initialize.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_pool_initialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_linear_source_destroy.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_destroy.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_linear_source_query.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_linear_source_commit.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_commit.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_linear_source_return.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_return.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_linear_source_contiguous_check.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_contiguous_check.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Linear source connected verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_linear_source_create.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_create.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_constructed_linear_source_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Linear source connected verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_pool_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Source pool connected lifecycle verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_linear_input_audio_decode.py')
if ($LASTEXITCODE -ne 0) { throw 'Persistent initialized audio decoding verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_linear_input_audio_full_file.py')
if ($LASTEXITCODE -ne 0) { throw 'Complete native audio file verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_linear_source_pool_counter_decrement.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_pool_counter_decrement.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_pool_reinitialize.py')
if ($LASTEXITCODE -ne 0) { throw 'Source pool connected lifecycle verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_subordinate_field8_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_field8_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_subordinate_fieldc_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_fieldc_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_error_callback_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_error_callback_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_error_report.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_error_report.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_field8_set_native_error.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_fieldc_set_native_error.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_field8_boundaries.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_subordinate_pan.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_pan.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_pan_native_enqueue.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_error_record.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_error_record.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_pan_native_diagnostics.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_subordinate_effect.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_effect.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_effect_native_queue.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_subordinate_field8_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_field8_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_subordinate_fieldc_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_fieldc_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_subordinate_volume_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_volume_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_subordinate_pan_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_pan_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_subordinate_effect_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_effect_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_pan_connected_readback.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_effect_connected_readback.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_subordinate_pitch.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_pitch.py')
if ($LASTEXITCODE -ne 0) { throw 'Pitch setter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_pitch_native_queue.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_subordinate_pitch_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_pitch_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_subordinate_aux_set.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_aux_set.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_subordinate_aux_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_aux_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_aux_connected_readback.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_aux_noop.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_aux_native_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_subordinate_apply.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_apply.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_apply_native_zero_queries.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_apply_noop.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_apply_native_gate.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_driver_gate_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Gate getter independent verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_fieldc_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_fieldc_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_volume_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_volume_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_readback_wrapper.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_volume_get_native_wrapper.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_backend_readback.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_backend_readback.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_descriptor.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_byte_read.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_byte_read.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_byte.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_word.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_backend_volume_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_backend_volume_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_volume.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_volume_byte_read.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_volume_byte_read.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_volume_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_driver_pan_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_driver_pan_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_pan_get_selectors.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_pan_get_native_wrapper.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_backend_pan_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_backend_pan_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_pan.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_pan_full_readback.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_volume_full_readback.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_volume_full_byte_range.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_backend_field_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_backend_field_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_field.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_field_word_read.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_field_word_read.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_field_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_backend_field8_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_backend_field8_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_field8_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_backend_fieldc_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_backend_fieldc_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_fieldc_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_backend_field14_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_backend_field14_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_field14_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_backend_field4_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_backend_field4_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_field4_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_backend_field5_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_backend_field5_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_field5_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_backend_field10_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_backend_field10_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_sound_resource_lookup.ps1')
& $python (Join-Path $PSScriptRoot 'verify_sound_resource_lookup.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_all_fields.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_volume_all_fields.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_sound_pan_all_fields.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_transfer_wait.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_wait.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_transfer_ready_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_ready_get.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_wait_native_ready.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_transfer_control_restore.ps1')

& (Join-Path $PSScriptRoot 'build_engine_transfer_control_install.ps1')

& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_control_pair.py')
if ($LASTEXITCODE -ne 0) { throw 'Transfer control verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_channel_control_restore.ps1')

& (Join-Path $PSScriptRoot 'build_engine_channel_control_install.ps1')

& $python (Join-Path $PSScriptRoot 'verify_engine_channel_control_pair.py')
if ($LASTEXITCODE -ne 0) { throw 'Transfer control verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_channel_status_get.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_channel_control_native_status.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_control_native_channels.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_transfer_controls.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& $python (Join-Path $PSScriptRoot 'verify_backend_readback_native_delayed_transfer.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound setter error verification failed' }

& (Join-Path $PSScriptRoot 'build_engine_transfer_register_config.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_register_config.py')
if ($LASTEXITCODE -ne 0) { throw 'Source size getter verification failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_engine_dispatch_option.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_dispatch_option.dll')
if ($LASTEXITCODE -ne 0) { throw 'Engine option-dispatch wrapper compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_dispatch_option.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine option-dispatch wrapper comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_dispatch_option.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine option-dispatch wrapper registration failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_engine_mmio_write32.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_mmio_write32.dll')
if ($LASTEXITCODE -ne 0) { throw 'Engine MMIO writer compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_mmio_write32.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine MMIO writer comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_mmio_write32.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine MMIO writer registration failed' }

& (Join-Path $PSScriptRoot 'build_engine_event_clear_pending.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_event_clear_pending.py')
if ($LASTEXITCODE -ne 0) { throw 'Event pending-clear verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_event_clear_pending.py')
if ($LASTEXITCODE -ne 0) { throw 'Event pending-clear registration failed' }

& (Join-Path $PSScriptRoot 'build_engine_event_count_callback.ps1')
& $python (Join-Path $PSScriptRoot 'verify_engine_event_count_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Event count/callback verification failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_event_count_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Event count/callback registration failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_engine_cache_writeback_range.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_cache_writeback_range.dll')
if ($LASTEXITCODE -ne 0) { throw 'Engine cache-writeback range compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_cache_writeback_range.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine cache-writeback range comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_cache_writeback_range.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine cache-writeback range registration failed' }

& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/sa_engine_mmio_read32.c') -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_mmio_read32.dll')
if ($LASTEXITCODE -ne 0) { throw 'Engine MMIO reader compilation failed' }
& $python (Join-Path $PSScriptRoot 'verify_engine_mmio_read32.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine MMIO reader comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_engine_mmio_read32.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine MMIO reader registration failed' }

& (Join-Path $PSScriptRoot 'build_task_character_decode_8c04bc20.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_task_character_decode_8c04bc20.py')
if ($LASTEXITCODE -ne 0) { throw 'Task character decode verification failed' }
& $python (Join-Path $PSScriptRoot 'register_task_character_decode_8c04bc20.py')
if ($LASTEXITCODE -ne 0) { throw 'Task character decode registration failed' }

& (Join-Path $PSScriptRoot 'build_global_fpu_store_8c09e760.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_fpu_store_8c09e760.py')
if ($LASTEXITCODE -ne 0) { throw 'Global FPU store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_fpu_store_8c09e760.py')
if ($LASTEXITCODE -ne 0) { throw 'Global FPU store registration failed' }

& (Join-Path $PSScriptRoot 'build_global_byte_triplet_set_8c08ab20.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_byte_triplet_set_8c08ab20.py')
if ($LASTEXITCODE -ne 0) { throw 'Global byte triplet setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_byte_triplet_set_8c08ab20.py')
if ($LASTEXITCODE -ne 0) { throw 'Global byte triplet setter registration failed' }

& (Join-Path $PSScriptRoot 'build_indexed_flag_set_8c09067e.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_indexed_flag_set_8c09067e.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed flag setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_indexed_flag_set_8c09067e.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed flag setter registration failed' }

& (Join-Path $PSScriptRoot 'build_signed_byte_table_lookup_8c0d2032.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_signed_byte_table_lookup_8c0d2032.py')
if ($LASTEXITCODE -ne 0) { throw 'Signed byte table lookup verification failed' }
& $python (Join-Path $PSScriptRoot 'register_signed_byte_table_lookup_8c0d2032.py')
if ($LASTEXITCODE -ne 0) { throw 'Signed byte table lookup registration failed' }

& (Join-Path $PSScriptRoot 'build_global_word_copy_8c06c182.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_word_copy_8c06c182.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word copy verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_word_copy_8c06c182.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word copy registration failed' }

& (Join-Path $PSScriptRoot 'build_global_counter_add_8c08a298.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_counter_add_8c08a298.py')
if ($LASTEXITCODE -ne 0) { throw 'Global counter add verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_counter_add_8c08a298.py')
if ($LASTEXITCODE -ne 0) { throw 'Global counter add registration failed' }

& (Join-Path $PSScriptRoot 'build_indexed_word_lookup_8c07cfa0.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_indexed_word_lookup_8c07cfa0.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed word lookup verification failed' }
& $python (Join-Path $PSScriptRoot 'register_indexed_word_lookup_8c07cfa0.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed word lookup registration failed' }

& (Join-Path $PSScriptRoot 'build_indexed_word_lookup_8c07cfde.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_indexed_word_lookup_8c07cfde.py')
if ($LASTEXITCODE -ne 0) { throw 'Second indexed word lookup verification failed' }
& $python (Join-Path $PSScriptRoot 'register_indexed_word_lookup_8c07cfde.py')
if ($LASTEXITCODE -ne 0) { throw 'Second indexed word lookup registration failed' }

& (Join-Path $PSScriptRoot 'build_write_u16_le_8c608a04.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_write_u16_le_8c608a04.py')
if ($LASTEXITCODE -ne 0) { throw 'Little-endian 16-bit writer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_write_u16_le_8c608a04.py')
if ($LASTEXITCODE -ne 0) { throw 'Little-endian 16-bit writer registration failed' }

& (Join-Path $PSScriptRoot 'build_context_field_set_8c5ffa08.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_context_field_set_8c5ffa08.py')
if ($LASTEXITCODE -ne 0) { throw 'Context field setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_context_field_set_8c5ffa08.py')
if ($LASTEXITCODE -ne 0) { throw 'Context field setter registration failed' }

& (Join-Path $PSScriptRoot 'build_global_word_decrement_store_8c653386.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_word_decrement_store_8c653386.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word decrement/store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_word_decrement_store_8c653386.py')
if ($LASTEXITCODE -ne 0) { throw 'Global word decrement/store registration failed' }

& (Join-Path $PSScriptRoot 'build_mmio_complement_sub_8c60260e.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_mmio_complement_sub_8c60260e.py')
if ($LASTEXITCODE -ne 0) { throw 'MMIO complement/subtract verification failed' }
& $python (Join-Path $PSScriptRoot 'register_mmio_complement_sub_8c60260e.py')
if ($LASTEXITCODE -ne 0) { throw 'MMIO complement/subtract registration failed' }

& (Join-Path $PSScriptRoot 'build_triplet_initialize_8c608922.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_triplet_initialize_8c608922.py')
if ($LASTEXITCODE -ne 0) { throw 'Triplet initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_triplet_initialize_8c608922.py')
if ($LASTEXITCODE -ne 0) { throw 'Triplet initializer registration failed' }

& (Join-Path $PSScriptRoot 'build_indexed_record_address_8c601f5a.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_indexed_record_address_8c601f5a.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed record address verification failed' }
& $python (Join-Path $PSScriptRoot 'register_indexed_record_address_8c601f5a.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed record address registration failed' }

& (Join-Path $PSScriptRoot 'build_global_pair_initialize_8c10ba12.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_pair_initialize_8c10ba12.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pair initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_pair_initialize_8c10ba12.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pair initializer registration failed' }

& (Join-Path $PSScriptRoot 'build_record_pool_allocate_8c052a1e.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_record_pool_allocate_8c052a1e.py')
if ($LASTEXITCODE -ne 0) { throw 'Record pool allocator verification failed' }
& $python (Join-Path $PSScriptRoot 'register_record_pool_allocate_8c052a1e.py')
if ($LASTEXITCODE -ne 0) { throw 'Record pool allocator registration failed' }

& (Join-Path $PSScriptRoot 'build_slot_list_available_8c05f458.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_slot_list_available_8c05f458.py')
if ($LASTEXITCODE -ne 0) { throw 'Slot-list availability verification failed' }
& $python (Join-Path $PSScriptRoot 'register_slot_list_available_8c05f458.py')
if ($LASTEXITCODE -ne 0) { throw 'Slot-list availability registration failed' }

& (Join-Path $PSScriptRoot 'build_record_initialize_8c600dd0.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_record_initialize_8c600dd0.py')
if ($LASTEXITCODE -ne 0) { throw 'Record initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_record_initialize_8c600dd0.py')
if ($LASTEXITCODE -ne 0) { throw 'Record initializer registration failed' }

& (Join-Path $PSScriptRoot 'build_resource_record_relocate_8c09f6c8.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_resource_record_relocate_8c09f6c8.py')
if ($LASTEXITCODE -ne 0) { throw 'Resource record relocation verification failed' }
& $python (Join-Path $PSScriptRoot 'register_resource_record_relocate_8c09f6c8.py')
if ($LASTEXITCODE -ne 0) { throw 'Resource record relocation registration failed' }

& (Join-Path $PSScriptRoot 'build_resource_record_header_relocate_8c09f708.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_resource_record_header_relocate_8c09f708.py')
if ($LASTEXITCODE -ne 0) { throw 'Resource record header relocation verification failed' }
& $python (Join-Path $PSScriptRoot 'register_resource_record_header_relocate_8c09f708.py')
if ($LASTEXITCODE -ne 0) { throw 'Resource record header relocation registration failed' }

& (Join-Path $PSScriptRoot 'build_sparse_pointer_relocate_8c09f73c.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sparse_pointer_relocate_8c09f73c.py')
if ($LASTEXITCODE -ne 0) { throw 'Sparse pointer relocation verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sparse_pointer_relocate_8c09f73c.py')
if ($LASTEXITCODE -ne 0) { throw 'Sparse pointer relocation registration failed' }

& (Join-Path $PSScriptRoot 'build_pair_pointer_relocate_8c09f75a.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_pair_pointer_relocate_8c09f75a.py')
if ($LASTEXITCODE -ne 0) { throw 'Pair pointer relocation verification failed' }
& $python (Join-Path $PSScriptRoot 'register_pair_pointer_relocate_8c09f75a.py')
if ($LASTEXITCODE -ne 0) { throw 'Pair pointer relocation registration failed' }

& (Join-Path $PSScriptRoot 'build_triple_pointer_relocate_8c09f7a0.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_triple_pointer_relocate_8c09f7a0.py')
if ($LASTEXITCODE -ne 0) { throw 'Triple pointer relocation verification failed' }
& $python (Join-Path $PSScriptRoot 'register_triple_pointer_relocate_8c09f7a0.py')
if ($LASTEXITCODE -ne 0) { throw 'Triple pointer relocation registration failed' }

& (Join-Path $PSScriptRoot 'build_quad_pointer_relocate_8c09f7e6.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_quad_pointer_relocate_8c09f7e6.py')
if ($LASTEXITCODE -ne 0) { throw 'Quad pointer relocation verification failed' }
& $python (Join-Path $PSScriptRoot 'register_quad_pointer_relocate_8c09f7e6.py')
if ($LASTEXITCODE -ne 0) { throw 'Quad pointer relocation registration failed' }

& (Join-Path $PSScriptRoot 'build_pair_field_relocate_8c09f9da.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_pair_field_relocate_8c09f9da.py')
if ($LASTEXITCODE -ne 0) { throw 'Pair field relocation verification failed' }
& $python (Join-Path $PSScriptRoot 'register_pair_field_relocate_8c09f9da.py')
if ($LASTEXITCODE -ne 0) { throw 'Pair field relocation registration failed' }

& (Join-Path $PSScriptRoot 'build_record_first_word_relocate_8c09f8e2.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_record_first_word_relocate_8c09f8e2.py')
if ($LASTEXITCODE -ne 0) { throw 'Record first-word relocation verification failed' }
& $python (Join-Path $PSScriptRoot 'register_record_first_word_relocate_8c09f8e2.py')
if ($LASTEXITCODE -ne 0) { throw 'Record first-word relocation registration failed' }

& (Join-Path $PSScriptRoot 'build_global_counter_increment_8c666e06.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_counter_increment_8c666e06.py')
if ($LASTEXITCODE -ne 0) { throw 'Global counter increment verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_counter_increment_8c666e06.py')
if ($LASTEXITCODE -ne 0) { throw 'Global counter increment registration failed' }

& (Join-Path $PSScriptRoot 'build_delay_counter_loop_8c10c10c.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_delay_counter_loop_8c10c10c.py')
if ($LASTEXITCODE -ne 0) { throw 'Delay counter loop verification failed' }
& $python (Join-Path $PSScriptRoot 'register_delay_counter_loop_8c10c10c.py')
if ($LASTEXITCODE -ne 0) { throw 'Delay counter loop registration failed' }

& (Join-Path $PSScriptRoot 'build_signed_u8_clamp_8c0548e0.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_signed_u8_clamp_8c0548e0.py')
if ($LASTEXITCODE -ne 0) { throw 'Signed byte clamp verification failed' }
& $python (Join-Path $PSScriptRoot 'register_signed_u8_clamp_8c0548e0.py')
if ($LASTEXITCODE -ne 0) { throw 'Signed byte clamp registration failed' }

& (Join-Path $PSScriptRoot 'build_byte_table_bit_test_8c067d84.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_byte_table_bit_test_8c067d84.py')
if ($LASTEXITCODE -ne 0) { throw 'Byte table bit-test verification failed' }
& $python (Join-Path $PSScriptRoot 'register_byte_table_bit_test_8c067d84.py')
if ($LASTEXITCODE -ne 0) { throw 'Byte table bit-test registration failed' }

& (Join-Path $PSScriptRoot 'build_linked_list_push_8c665466.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_linked_list_push_8c665466.py')
if ($LASTEXITCODE -ne 0) { throw 'Linked-list push verification failed' }
& $python (Join-Path $PSScriptRoot 'register_linked_list_push_8c665466.py')
if ($LASTEXITCODE -ne 0) { throw 'Linked-list push registration failed' }

& (Join-Path $PSScriptRoot 'build_masked_array_store_8c6611a8.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_masked_array_store_8c6611a8.py')
if ($LASTEXITCODE -ne 0) { throw 'Masked array-store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_masked_array_store_8c6611a8.py')
if ($LASTEXITCODE -ne 0) { throw 'Masked array-store registration failed' }

& (Join-Path $PSScriptRoot 'build_mmio_masked_status_8c10d926.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_mmio_masked_status_8c10d926.py')
if ($LASTEXITCODE -ne 0) { throw 'MMIO masked-status verification failed' }
& $python (Join-Path $PSScriptRoot 'register_mmio_masked_status_8c10d926.py')
if ($LASTEXITCODE -ne 0) { throw 'MMIO masked-status registration failed' }

& (Join-Path $PSScriptRoot 'build_mmio_status_bits_8c10d914.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_mmio_status_bits_8c10d914.py')
if ($LASTEXITCODE -ne 0) { throw 'MMIO status-bits verification failed' }
& $python (Join-Path $PSScriptRoot 'register_mmio_status_bits_8c10d914.py')
if ($LASTEXITCODE -ne 0) { throw 'MMIO status-bits registration failed' }

& (Join-Path $PSScriptRoot 'build_string_length_8c10d3f0.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_string_length_8c10d3f0.py')
if ($LASTEXITCODE -ne 0) { throw 'String length verification failed' }
& $python (Join-Path $PSScriptRoot 'register_string_length_8c10d3f0.py')
if ($LASTEXITCODE -ne 0) { throw 'String length registration failed' }

& (Join-Path $PSScriptRoot 'build_string_copy_aligned_8c10ef20.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_string_copy_aligned_8c10ef20.py')
if ($LASTEXITCODE -ne 0) { throw 'Aligned string-copy verification failed' }
& $python (Join-Path $PSScriptRoot 'register_string_copy_aligned_8c10ef20.py')
if ($LASTEXITCODE -ne 0) { throw 'Aligned string-copy registration failed' }

& (Join-Path $PSScriptRoot 'build_string_copy_8c10dacc.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_string_copy_8c10dacc.py')
if ($LASTEXITCODE -ne 0) { throw 'String-copy wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_string_copy_8c10dacc.py')
if ($LASTEXITCODE -ne 0) { throw 'String-copy wrapper registration failed' }

& (Join-Path $PSScriptRoot 'build_bounds_check_8c10c98c.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_bounds_check_8c10c98c.py')
if ($LASTEXITCODE -ne 0) { throw 'Bounds-check helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_bounds_check_8c10c98c.py')
if ($LASTEXITCODE -ne 0) { throw 'Bounds-check helper registration failed' }

& (Join-Path $PSScriptRoot 'build_random_u15_8c10d334.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_random_u15_8c10d334.py')
if ($LASTEXITCODE -ne 0) { throw 'Random-number helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_random_u15_8c10d334.py')
if ($LASTEXITCODE -ne 0) { throw 'Random-number helper registration failed' }

& (Join-Path $PSScriptRoot 'build_platform_signature_value_8c10ba1e.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_platform_signature_value_8c10ba1e.py')
if ($LASTEXITCODE -ne 0) { throw 'Platform-signature helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_platform_signature_value_8c10ba1e.py')
if ($LASTEXITCODE -ne 0) { throw 'Platform-signature helper registration failed' }

& (Join-Path $PSScriptRoot 'build_request_slot_find_8c10c4a4.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_request_slot_find_8c10c4a4.py')
if ($LASTEXITCODE -ne 0) { throw 'Request-slot search helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_request_slot_find_8c10c4a4.py')
if ($LASTEXITCODE -ne 0) { throw 'Request-slot search helper registration failed' }

& (Join-Path $PSScriptRoot 'build_request_slot_acquire_8c10c4ca.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_request_slot_acquire_8c10c4ca.py')
if ($LASTEXITCODE -ne 0) { throw 'Request-slot acquisition helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_request_slot_acquire_8c10c4ca.py')
if ($LASTEXITCODE -ne 0) { throw 'Request-slot acquisition helper registration failed' }

& (Join-Path $PSScriptRoot 'build_stream_position_set_8c65bfc2.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stream_position_set_8c65bfc2.py')
if ($LASTEXITCODE -ne 0) { throw 'Stream-position setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stream_position_set_8c65bfc2.py')
if ($LASTEXITCODE -ne 0) { throw 'Stream-position setter registration failed' }

& (Join-Path $PSScriptRoot 'build_nullable_field_get_8c10c962.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_nullable_field_get_8c10c962.py')
if ($LASTEXITCODE -ne 0) { throw 'Nullable field getter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_nullable_field_get_8c10c962.py')
if ($LASTEXITCODE -ne 0) { throw 'Nullable field getter registration failed' }

& (Join-Path $PSScriptRoot 'build_nullable_signed_byte_get_8c10c970.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_nullable_signed_byte_get_8c10c970.py')
if ($LASTEXITCODE -ne 0) { throw 'Nullable signed-byte getter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_nullable_signed_byte_get_8c10c970.py')
if ($LASTEXITCODE -ne 0) { throw 'Nullable signed-byte getter registration failed' }

& (Join-Path $PSScriptRoot 'build_conditional_byte_clear_8c10e566.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_conditional_byte_clear_8c10e566.py')
if ($LASTEXITCODE -ne 0) { throw 'Conditional byte-clear verification failed' }
& $python (Join-Path $PSScriptRoot 'register_conditional_byte_clear_8c10e566.py')
if ($LASTEXITCODE -ne 0) { throw 'Conditional byte-clear registration failed' }

& (Join-Path $PSScriptRoot 'build_clear_0x600_bytes_8c107ec0.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_clear_0x600_bytes_8c107ec0.py')
if ($LASTEXITCODE -ne 0) { throw 'Fixed-size clear verification failed' }
& $python (Join-Path $PSScriptRoot 'register_clear_0x600_bytes_8c107ec0.py')
if ($LASTEXITCODE -ne 0) { throw 'Fixed-size clear registration failed' }

& (Join-Path $PSScriptRoot 'build_zero_byte_range_check_8c11025c.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_zero_byte_range_check_8c11025c.py')
if ($LASTEXITCODE -ne 0) { throw 'Zero-byte range check verification failed' }
& $python (Join-Path $PSScriptRoot 'register_zero_byte_range_check_8c11025c.py')
if ($LASTEXITCODE -ne 0) { throw 'Zero-byte range check registration failed' }

& (Join-Path $PSScriptRoot 'build_store_u16_be_8c111174.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_store_u16_be_8c111174.py')
if ($LASTEXITCODE -ne 0) { throw '16-bit big-endian store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_store_u16_be_8c111174.py')
if ($LASTEXITCODE -ne 0) { throw '16-bit big-endian store registration failed' }

& (Join-Path $PSScriptRoot 'build_record_initialize_8c1082da.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_record_initialize_8c1082da.py')
if ($LASTEXITCODE -ne 0) { throw 'Record initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_record_initialize_8c1082da.py')
if ($LASTEXITCODE -ne 0) { throw 'Record initializer registration failed' }

& (Join-Path $PSScriptRoot 'build_store_u32_be_8c110c9c.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_store_u32_be_8c110c9c.py')
if ($LASTEXITCODE -ne 0) { throw '32-bit big-endian store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_store_u32_be_8c110c9c.py')
if ($LASTEXITCODE -ne 0) { throw '32-bit big-endian store registration failed' }

& (Join-Path $PSScriptRoot 'build_expand_bits_to_nibbles_8c108160.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_expand_bits_to_nibbles_8c108160.py')
if ($LASTEXITCODE -ne 0) { throw 'Bit expansion verification failed' }
& $python (Join-Path $PSScriptRoot 'register_expand_bits_to_nibbles_8c108160.py')
if ($LASTEXITCODE -ne 0) { throw 'Bit expansion registration failed' }

& (Join-Path $PSScriptRoot 'build_bounded_byte_compare_8c1107a4.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_bounded_byte_compare_8c1107a4.py')
if ($LASTEXITCODE -ne 0) { throw 'Bounded byte comparison verification failed' }
& $python (Join-Path $PSScriptRoot 'register_bounded_byte_compare_8c1107a4.py')
if ($LASTEXITCODE -ne 0) { throw 'Bounded byte comparison registration failed' }

& (Join-Path $PSScriptRoot 'build_bounded_string_copy_8c10f95c.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_bounded_string_copy_8c10f95c.py')
if ($LASTEXITCODE -ne 0) { throw 'Bounded string copy verification failed' }
& $python (Join-Path $PSScriptRoot 'register_bounded_string_copy_8c10f95c.py')
if ($LASTEXITCODE -ne 0) { throw 'Bounded string copy registration failed' }

& (Join-Path $PSScriptRoot 'build_pack_nonzero_flags_8c108680.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_pack_nonzero_flags_8c108680.py')
if ($LASTEXITCODE -ne 0) { throw 'Fixed flag-packing verification failed' }
& $python (Join-Path $PSScriptRoot 'register_pack_nonzero_flags_8c108680.py')
if ($LASTEXITCODE -ne 0) { throw 'Fixed flag-packing registration failed' }

& (Join-Path $PSScriptRoot 'build_pack_nonzero_flags_reverse_8c1086ba.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_pack_nonzero_flags_reverse_8c1086ba.py')
if ($LASTEXITCODE -ne 0) { throw 'Reverse flag-packing verification failed' }
& $python (Join-Path $PSScriptRoot 'register_pack_nonzero_flags_reverse_8c1086ba.py')
if ($LASTEXITCODE -ne 0) { throw 'Reverse flag-packing registration failed' }

& (Join-Path $PSScriptRoot 'build_shift_right_bytes_with_carry_8c111128.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_shift_right_bytes_with_carry_8c111128.py')
if ($LASTEXITCODE -ne 0) { throw 'Byte shift with carry verification failed' }
& $python (Join-Path $PSScriptRoot 'register_shift_right_bytes_with_carry_8c111128.py')
if ($LASTEXITCODE -ne 0) { throw 'Byte shift with carry registration failed' }

& (Join-Path $PSScriptRoot 'build_string_append_8c10d3a8.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_string_append_8c10d3a8.py')
if ($LASTEXITCODE -ne 0) { throw 'String append verification failed' }
& $python (Join-Path $PSScriptRoot 'register_string_append_8c10d3a8.py')
if ($LASTEXITCODE -ne 0) { throw 'String append registration failed' }

& (Join-Path $PSScriptRoot 'build_byte_copy_8c107fa4.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_byte_copy_8c107fa4.py')
if ($LASTEXITCODE -ne 0) { throw 'Byte-copy helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_byte_copy_8c107fa4.py')
if ($LASTEXITCODE -ne 0) { throw 'Byte-copy helper registration failed' }

& (Join-Path $PSScriptRoot 'build_free_list_pop_8c02cef4.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_free_list_pop_8c02cef4.py')
if ($LASTEXITCODE -ne 0) { throw 'Free-list pop verification failed' }
& $python (Join-Path $PSScriptRoot 'register_free_list_pop_8c02cef4.py')
if ($LASTEXITCODE -ne 0) { throw 'Free-list pop registration failed' }

& (Join-Path $PSScriptRoot 'build_free_list_push_8c02cf20.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_free_list_push_8c02cf20.py')
if ($LASTEXITCODE -ne 0) { throw 'Free-list push verification failed' }
& $python (Join-Path $PSScriptRoot 'register_free_list_push_8c02cf20.py')
if ($LASTEXITCODE -ne 0) { throw 'Free-list push registration failed' }

& (Join-Path $PSScriptRoot 'build_work_release_8c02cf48.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_work_release_8c02cf48.py')
if ($LASTEXITCODE -ne 0) { throw 'Work-release verification failed' }
& $python (Join-Path $PSScriptRoot 'register_work_release_8c02cf48.py')
if ($LASTEXITCODE -ne 0) { throw 'Work-release registration failed' }

& (Join-Path $PSScriptRoot 'build_work_id_clear_8c02ce18.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_work_id_clear_8c02ce18.py')
if ($LASTEXITCODE -ne 0) { throw 'Work-ID clear verification failed' }
& $python (Join-Path $PSScriptRoot 'register_work_id_clear_8c02ce18.py')
if ($LASTEXITCODE -ne 0) { throw 'Work-ID clear registration failed' }

& (Join-Path $PSScriptRoot 'build_work_pool_initialize_8c02ce80.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_work_pool_initialize_8c02ce80.py')
if ($LASTEXITCODE -ne 0) { throw 'Work-pool initialization verification failed' }
& $python (Join-Path $PSScriptRoot 'register_work_pool_initialize_8c02ce80.py')
if ($LASTEXITCODE -ne 0) { throw 'Work-pool initialization registration failed' }

& (Join-Path $PSScriptRoot 'build_work_active_pool_initialize_8c02ceca.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_work_active_pool_initialize_8c02ceca.py')
if ($LASTEXITCODE -ne 0) { throw 'Active work-pool initialization verification failed' }
& $python (Join-Path $PSScriptRoot 'register_work_active_pool_initialize_8c02ceca.py')
if ($LASTEXITCODE -ne 0) { throw 'Active work-pool initialization registration failed' }

& (Join-Path $PSScriptRoot 'build_clear_allocator_table_8c02cfa4.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_clear_allocator_table_8c02cfa4.py')
if ($LASTEXITCODE -ne 0) { throw 'Allocator table clear verification failed' }
& $python (Join-Path $PSScriptRoot 'register_clear_allocator_table_8c02cfa4.py')
if ($LASTEXITCODE -ne 0) { throw 'Allocator table clear registration failed' }

& (Join-Path $PSScriptRoot 'build_fpu_bucket_allocate_8c02cfc0.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_fpu_bucket_allocate_8c02cfc0.py')
if ($LASTEXITCODE -ne 0) { throw 'FPU bucket allocator verification failed' }
& $python (Join-Path $PSScriptRoot 'register_fpu_bucket_allocate_8c02cfc0.py')
if ($LASTEXITCODE -ne 0) { throw 'FPU bucket allocator registration failed' }

& (Join-Path $PSScriptRoot 'build_callback_tail_8c64e0ac.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_callback_tail_8c64e0ac.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback tail thunk verification failed' }
& $python (Join-Path $PSScriptRoot 'register_callback_tail_8c64e0ac.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback tail thunk registration failed' }

& (Join-Path $PSScriptRoot 'build_range_nonzero_validate_8c10c116.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_range_nonzero_validate_8c10c116.py')
if ($LASTEXITCODE -ne 0) { throw 'Range/nonzero validation verification failed' }
& $python (Join-Path $PSScriptRoot 'register_range_nonzero_validate_8c10c116.py')
if ($LASTEXITCODE -ne 0) { throw 'Range/nonzero validation registration failed' }

& (Join-Path $PSScriptRoot 'build_global_mask_accumulate_8c6578e0.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_mask_accumulate_8c6578e0.py')
if ($LASTEXITCODE -ne 0) { throw 'Global mask accumulator verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_mask_accumulate_8c6578e0.py')
if ($LASTEXITCODE -ne 0) { throw 'Global mask accumulator registration failed' }

& (Join-Path $PSScriptRoot 'build_bounded_array_store_8c646eee.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_bounded_array_store_8c646eee.py')
if ($LASTEXITCODE -ne 0) { throw 'Bounded array store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_bounded_array_store_8c646eee.py')
if ($LASTEXITCODE -ne 0) { throw 'Bounded array store registration failed' }

& (Join-Path $PSScriptRoot 'build_copy_global_pair_bytes_8c08ab02.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_copy_global_pair_bytes_8c08ab02.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pair byte copy verification failed' }
& $python (Join-Path $PSScriptRoot 'register_copy_global_pair_bytes_8c08ab02.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pair byte copy registration failed' }

& (Join-Path $PSScriptRoot 'build_initialize_record_fields_8c608914.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_initialize_record_fields_8c608914.py')
if ($LASTEXITCODE -ne 0) { throw 'Record field initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_initialize_record_fields_8c608914.py')
if ($LASTEXITCODE -ne 0) { throw 'Record field initializer registration failed' }

& (Join-Path $PSScriptRoot 'build_indexed_signed_byte_match_mask_8c109ce6.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_indexed_signed_byte_match_mask_8c109ce6.py')
if ($LASTEXITCODE -ne 0) { throw 'Signed-byte mask comparison verification failed' }
& $python (Join-Path $PSScriptRoot 'register_indexed_signed_byte_match_mask_8c109ce6.py')
if ($LASTEXITCODE -ne 0) { throw 'Signed-byte mask comparison registration failed' }

& (Join-Path $PSScriptRoot 'build_indexed_stride_value_8c64b274.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_indexed_stride_value_8c64b274.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed stride value verification failed' }
& $python (Join-Path $PSScriptRoot 'register_indexed_stride_value_8c64b274.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed stride value registration failed' }

& (Join-Path $PSScriptRoot 'build_bucket_list_merge_8c02d00e.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_bucket_list_merge_8c02d00e.py')
if ($LASTEXITCODE -ne 0) { throw 'Bucket-list merge verification failed' }
& $python (Join-Path $PSScriptRoot 'register_bucket_list_merge_8c02d00e.py')
if ($LASTEXITCODE -ne 0) { throw 'Bucket-list merge registration failed' }

& (Join-Path $PSScriptRoot 'build_add_fixed_globals_8c08a5be.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_add_fixed_globals_8c08a5be.py')
if ($LASTEXITCODE -ne 0) { throw 'Fixed global addition verification failed' }
& $python (Join-Path $PSScriptRoot 'register_add_fixed_globals_8c08a5be.py')
if ($LASTEXITCODE -ne 0) { throw 'Fixed global addition registration failed' }

& (Join-Path $PSScriptRoot 'build_global_masked_update_8c037618.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_masked_update_8c037618.py')
if ($LASTEXITCODE -ne 0) { throw 'Global masked update verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_masked_update_8c037618.py')
if ($LASTEXITCODE -ne 0) { throw 'Global masked update registration failed' }

& (Join-Path $PSScriptRoot 'build_global_pair_masked_update_8c03767c.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_pair_masked_update_8c03767c.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pair masked update verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_pair_masked_update_8c03767c.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pair masked update registration failed' }

& (Join-Path $PSScriptRoot 'build_global_r14_masked_or_8c0376b0.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_r14_masked_or_8c0376b0.py')
if ($LASTEXITCODE -ne 0) { throw 'Global R14 masked OR verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_r14_masked_or_8c0376b0.py')
if ($LASTEXITCODE -ne 0) { throw 'Global R14 masked OR registration failed' }

& (Join-Path $PSScriptRoot 'build_copy_global_pair_8c056926.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_copy_global_pair_8c056926.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pair copy verification failed' }
& $python (Join-Path $PSScriptRoot 'register_copy_global_pair_8c056926.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pair copy registration failed' }

& (Join-Path $PSScriptRoot 'build_restore_global_pair_8c056938.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_restore_global_pair_8c056938.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pair restore verification failed' }
& $python (Join-Path $PSScriptRoot 'register_restore_global_pair_8c056938.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pair restore registration failed' }

& (Join-Path $PSScriptRoot 'build_global_pair_flags_or_8c05694a.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_pair_flags_or_8c05694a.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pair flags OR verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_pair_flags_or_8c05694a.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pair flags OR registration failed' }

& (Join-Path $PSScriptRoot 'build_global_pair_flags_clear_8c05695c.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_pair_flags_clear_8c05695c.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pair flags clear verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_pair_flags_clear_8c05695c.py')
if ($LASTEXITCODE -ne 0) { throw 'Global pair flags clear registration failed' }

& (Join-Path $PSScriptRoot 'build_short_absolute_difference_8c055d5a.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_short_absolute_difference_8c055d5a.py')
if ($LASTEXITCODE -ne 0) { throw 'Short absolute difference verification failed' }
& $python (Join-Path $PSScriptRoot 'register_short_absolute_difference_8c055d5a.py')
if ($LASTEXITCODE -ne 0) { throw 'Short absolute difference registration failed' }

& (Join-Path $PSScriptRoot 'build_pointer_list_word_or_8c056d5e.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_pointer_list_word_or_8c056d5e.py')
if ($LASTEXITCODE -ne 0) { throw 'Pointer-list word OR verification failed' }
& $python (Join-Path $PSScriptRoot 'register_pointer_list_word_or_8c056d5e.py')
if ($LASTEXITCODE -ne 0) { throw 'Pointer-list word OR registration failed' }

& (Join-Path $PSScriptRoot 'build_pointer_list_word_and_8c056d76.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_pointer_list_word_and_8c056d76.py')
if ($LASTEXITCODE -ne 0) { throw 'Pointer-list word AND verification failed' }
& $python (Join-Path $PSScriptRoot 'register_pointer_list_word_and_8c056d76.py')
if ($LASTEXITCODE -ne 0) { throw 'Pointer-list word AND registration failed' }

& (Join-Path $PSScriptRoot 'build_bounded_global_pointer_append_8c055cde.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_bounded_global_pointer_append_8c055cde.py')
if ($LASTEXITCODE -ne 0) { throw 'Bounded global pointer append verification failed' }
& $python (Join-Path $PSScriptRoot 'register_bounded_global_pointer_append_8c055cde.py')
if ($LASTEXITCODE -ne 0) { throw 'Bounded global pointer append registration failed' }

& (Join-Path $PSScriptRoot 'build_clear_ten_global_words_8c0635c0.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_clear_ten_global_words_8c0635c0.py')
if ($LASTEXITCODE -ne 0) { throw 'Clear ten global words verification failed' }
& $python (Join-Path $PSScriptRoot 'register_clear_ten_global_words_8c0635c0.py')
if ($LASTEXITCODE -ne 0) { throw 'Clear ten global words registration failed' }

& (Join-Path $PSScriptRoot 'build_set_gbr_slot_scaled_pointer_8c038cec.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_set_gbr_slot_scaled_pointer_8c038cec.py')
if ($LASTEXITCODE -ne 0) { throw 'GBR scaled-pointer slot verification failed' }
& $python (Join-Path $PSScriptRoot 'register_set_gbr_slot_scaled_pointer_8c038cec.py')
if ($LASTEXITCODE -ne 0) { throw 'GBR scaled-pointer slot registration failed' }

& (Join-Path $PSScriptRoot 'build_test_byte_nonzero_8c09068e.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_test_byte_nonzero_8c09068e.py')
if ($LASTEXITCODE -ne 0) { throw 'Byte nonzero test verification failed' }
& $python (Join-Path $PSScriptRoot 'register_test_byte_nonzero_8c09068e.py')
if ($LASTEXITCODE -ne 0) { throw 'Byte nonzero test registration failed' }

& (Join-Path $PSScriptRoot 'build_initialize_three_global_fields_8c08a04a.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_initialize_three_global_fields_8c08a04a.py')
if ($LASTEXITCODE -ne 0) { throw 'Three global fields initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_initialize_three_global_fields_8c08a04a.py')
if ($LASTEXITCODE -ne 0) { throw 'Three global fields initializer registration failed' }

& (Join-Path $PSScriptRoot 'build_read_hardware_flag_boolean_8c10f9a8.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_read_hardware_flag_boolean_8c10f9a8.py')
if ($LASTEXITCODE -ne 0) { throw 'Hardware flag Boolean helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_read_hardware_flag_boolean_8c10f9a8.py')
if ($LASTEXITCODE -ne 0) { throw 'Hardware flag Boolean helper registration failed' }

& (Join-Path $PSScriptRoot 'build_store_nonzero_global_pointer_8c5fbcb6.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_store_nonzero_global_pointer_8c5fbcb6.py')
if ($LASTEXITCODE -ne 0) { throw 'Nonzero global pointer store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_store_nonzero_global_pointer_8c5fbcb6.py')
if ($LASTEXITCODE -ne 0) { throw 'Nonzero global pointer store registration failed' }

& (Join-Path $PSScriptRoot 'build_initialize_three_global_bytes_8c08ab10.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_initialize_three_global_bytes_8c08ab10.py')
if ($LASTEXITCODE -ne 0) { throw 'Three global bytes initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_initialize_three_global_bytes_8c08ab10.py')
if ($LASTEXITCODE -ne 0) { throw 'Three global bytes initializer registration failed' }

& (Join-Path $PSScriptRoot 'build_read_relative_table_word_8c10d428.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_read_relative_table_word_8c10d428.py')
if ($LASTEXITCODE -ne 0) { throw 'Relative table word lookup verification failed' }
& $python (Join-Path $PSScriptRoot 'register_read_relative_table_word_8c10d428.py')
if ($LASTEXITCODE -ne 0) { throw 'Relative table word lookup registration failed' }

& (Join-Path $PSScriptRoot 'build_get_nested_object_word_8c0d5d80.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_get_nested_object_word_8c0d5d80.py')
if ($LASTEXITCODE -ne 0) { throw 'Nested object word getter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_get_nested_object_word_8c0d5d80.py')
if ($LASTEXITCODE -ne 0) { throw 'Nested object word getter registration failed' }

& (Join-Path $PSScriptRoot 'build_store_nonzero_second_global_pointer_8c5ff2cc.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_store_nonzero_second_global_pointer_8c5ff2cc.py')
if ($LASTEXITCODE -ne 0) { throw 'Second global pointer store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_store_nonzero_second_global_pointer_8c5ff2cc.py')
if ($LASTEXITCODE -ne 0) { throw 'Second global pointer store registration failed' }

& (Join-Path $PSScriptRoot 'build_store_nonzero_third_global_pointer_8c608ccc.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_store_nonzero_third_global_pointer_8c608ccc.py')
if ($LASTEXITCODE -ne 0) { throw 'Third global pointer store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_store_nonzero_third_global_pointer_8c608ccc.py')
if ($LASTEXITCODE -ne 0) { throw 'Third global pointer store registration failed' }

& (Join-Path $PSScriptRoot 'build_read_relative_table_word_8c10d444.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_read_relative_table_word_8c10d444.py')
if ($LASTEXITCODE -ne 0) { throw 'Relative table word lookup 0xdc verification failed' }
& $python (Join-Path $PSScriptRoot 'register_read_relative_table_word_8c10d444.py')
if ($LASTEXITCODE -ne 0) { throw 'Relative table word lookup 0xdc registration failed' }

& (Join-Path $PSScriptRoot 'build_call_global_zero_8c600ddc.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_call_global_zero_8c600ddc.py')
if ($LASTEXITCODE -ne 0) { throw 'Global zero-argument callback verification failed' }
& $python (Join-Path $PSScriptRoot 'register_call_global_zero_8c600ddc.py')
if ($LASTEXITCODE -ne 0) { throw 'Global zero-argument callback registration failed' }

& (Join-Path $PSScriptRoot 'build_call_global_two_8c606f66.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_call_global_two_8c606f66.py')
if ($LASTEXITCODE -ne 0) { throw 'Global two-argument callback verification failed' }
& $python (Join-Path $PSScriptRoot 'register_call_global_two_8c606f66.py')
if ($LASTEXITCODE -ne 0) { throw 'Global two-argument callback registration failed' }

& (Join-Path $PSScriptRoot 'build_call_global_four_8c606f6c.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_call_global_four_8c606f6c.py')
if ($LASTEXITCODE -ne 0) { throw 'Global four-argument callback verification failed' }
& $python (Join-Path $PSScriptRoot 'register_call_global_four_8c606f6c.py')
if ($LASTEXITCODE -ne 0) { throw 'Global four-argument callback registration failed' }

& (Join-Path $PSScriptRoot 'build_cache_block_writeback_8c65254e.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_cache_block_writeback_8c65254e.py')
if ($LASTEXITCODE -ne 0) { throw 'Cache block writeback verification failed' }
& $python (Join-Path $PSScriptRoot 'register_cache_block_writeback_8c65254e.py')
if ($LASTEXITCODE -ne 0) { throw 'Cache block writeback registration failed' }

& (Join-Path $PSScriptRoot 'build_startup_tail_transfer_8c01c28e.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_startup_tail_transfer_8c01c28e.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup tail-transfer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_startup_tail_transfer_8c01c28e.py')
if ($LASTEXITCODE -ne 0) { throw 'Startup tail-transfer registration failed' }

& (Join-Path $PSScriptRoot 'build_indirect_data_tail_call_8c09f380.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_indirect_data_tail_call_8c09f380.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect data tail-call verification failed' }
& $python (Join-Path $PSScriptRoot 'register_indirect_data_tail_call_8c09f380.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect data tail-call registration failed' }

& (Join-Path $PSScriptRoot 'build_global_slot_tail_call_one_8c60434c.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_global_slot_tail_call_one_8c60434c.py')
if ($LASTEXITCODE -ne 0) { throw 'Global-slot callback verification failed' }
& $python (Join-Path $PSScriptRoot 'register_global_slot_tail_call_one_8c60434c.py')
if ($LASTEXITCODE -ne 0) { throw 'Global-slot callback registration failed' }

& (Join-Path $PSScriptRoot 'build_clear_global_then_branch_8c08a578.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_clear_global_then_branch_8c08a578.py')
if ($LASTEXITCODE -ne 0) { throw 'Clear-global branch verification failed' }
& $python (Join-Path $PSScriptRoot 'register_clear_global_then_branch_8c08a578.py')
if ($LASTEXITCODE -ne 0) { throw 'Clear-global branch registration failed' }

& (Join-Path $PSScriptRoot 'build_literal_pair_tail_call_8c63a980.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_literal_pair_tail_call_8c63a980.py')
if ($LASTEXITCODE -ne 0) { throw 'Literal-pair tail-call verification failed' }
& $python (Join-Path $PSScriptRoot 'register_literal_pair_tail_call_8c63a980.py')
if ($LASTEXITCODE -ne 0) { throw 'Literal-pair tail-call registration failed' }

& (Join-Path $PSScriptRoot 'build_literal_pair_tail_call_8c640752.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_literal_pair_tail_call_8c640752.py')
if ($LASTEXITCODE -ne 0) { throw 'Literal-pair tail-call 0x8c640752 verification failed' }
& $python (Join-Path $PSScriptRoot 'register_literal_pair_tail_call_8c640752.py')
if ($LASTEXITCODE -ne 0) { throw 'Literal-pair tail-call 0x8c640752 registration failed' }

& (Join-Path $PSScriptRoot 'build_indirect_global_tail_call_8c10c0d0.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_indirect_global_tail_call_8c10c0d0.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect global tail-call verification failed' }
& $python (Join-Path $PSScriptRoot 'register_indirect_global_tail_call_8c10c0d0.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect global tail-call registration failed' }

& (Join-Path $PSScriptRoot 'build_indirect_global_tail_call_three_8c10c0f8.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_indirect_global_tail_call_three_8c10c0f8.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect global tail-call three verification failed' }
& $python (Join-Path $PSScriptRoot 'register_indirect_global_tail_call_three_8c10c0f8.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect global tail-call three registration failed' }

& (Join-Path $PSScriptRoot 'build_indirect_pointer_tail_call_8c66085a.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_indirect_pointer_tail_call_8c66085a.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect pointer tail-call verification failed' }
& $python (Join-Path $PSScriptRoot 'register_indirect_pointer_tail_call_8c66085a.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect pointer tail-call registration failed' }

& (Join-Path $PSScriptRoot 'build_callback_tail_call_zero_hundred_8c65bdca.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_callback_tail_call_zero_hundred_8c65bdca.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback tail-call zero/hundred verification failed' }
& $python (Join-Path $PSScriptRoot 'register_callback_tail_call_zero_hundred_8c65bdca.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback tail-call zero/hundred registration failed' }

& (Join-Path $PSScriptRoot 'build_callback_tail_call_pointer_zero_two_8c04917a.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_callback_tail_call_pointer_zero_two_8c04917a.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback pointer/zero/two verification failed' }
& $python (Join-Path $PSScriptRoot 'register_callback_tail_call_pointer_zero_two_8c04917a.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback pointer/zero/two registration failed' }

& (Join-Path $PSScriptRoot 'build_callback_tail_call_two_pointer_8c0272cc.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_callback_tail_call_two_pointer_8c0272cc.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback two-pointer tail-call verification failed' }
& $python (Join-Path $PSScriptRoot 'register_callback_tail_call_two_pointer_8c0272cc.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback two-pointer tail-call registration failed' }

& (Join-Path $PSScriptRoot 'build_set_global_one_tail_call_8c05f210.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_set_global_one_tail_call_8c05f210.py')
if ($LASTEXITCODE -ne 0) { throw 'Set-global-one tail-call verification failed' }
& $python (Join-Path $PSScriptRoot 'register_set_global_one_tail_call_8c05f210.py')
if ($LASTEXITCODE -ne 0) { throw 'Set-global-one tail-call registration failed' }

& (Join-Path $PSScriptRoot 'build_branch_thunk_8c660760.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_branch_thunk_8c660760.py')
if ($LASTEXITCODE -ne 0) { throw 'Branch thunk verification failed' }
& $python (Join-Path $PSScriptRoot 'register_branch_thunk_8c660760.py')
if ($LASTEXITCODE -ne 0) { throw 'Branch thunk registration failed' }

& (Join-Path $PSScriptRoot 'build_stg12_thunk_ext_fun_8c01ae60_0c915a80.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg12_thunk_ext_fun_8c01ae60_0c915a80.py')
if ($LASTEXITCODE -ne 0) { throw 'STG12 external-function thunk verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg12_thunk_ext_fun_8c01ae60_0c915a80.py')
if ($LASTEXITCODE -ne 0) { throw 'STG12 external-function thunk registration failed' }
& (Join-Path $PSScriptRoot 'build_indirect_literal_tail_call_cluster.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_indirect_literal_tail_call_cluster.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect literal tail-call cluster verification failed' }
& $python (Join-Path $PSScriptRoot 'register_indirect_literal_tail_call_cluster.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect literal tail-call cluster registration failed' }
& (Join-Path $PSScriptRoot 'build_vector_length_squared_8c02cf68.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_vector_length_squared_8c02cf68.py')
if ($LASTEXITCODE -ne 0) { throw 'Vector-length-squared helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_vector_length_squared_8c02cf68.py')
if ($LASTEXITCODE -ne 0) { throw 'Vector-length-squared helper registration failed' }
& (Join-Path $PSScriptRoot 'build_tail_wrapper_8c644de4.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_tail_wrapper_8c644de4.py')
if ($LASTEXITCODE -ne 0) { throw 'Tail-wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_tail_wrapper_8c644de4.py')
if ($LASTEXITCODE -ne 0) { throw 'Tail-wrapper registration failed' }
& (Join-Path $PSScriptRoot 'build_indirect_tail_wrapper_8c64b2d4.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_indirect_tail_wrapper_8c64b2d4.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect tail-wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_indirect_tail_wrapper_8c64b2d4.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect tail-wrapper registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_resource_cleanup_0c9028a4.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_resource_cleanup_0c9028a4.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 resource cleanup verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_resource_cleanup_0c9028a4.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 resource cleanup registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_indirect_resource_tail_0c902d40.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_indirect_resource_tail_0c902d40.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 indirect resource tail verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_indirect_resource_tail_0c902d40.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 indirect resource tail registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_flagged_byte_clear_0c903a60.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_flagged_byte_clear_0c903a60.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 flagged-byte clear verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_flagged_byte_clear_0c903a60.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 flagged-byte clear registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_callback_cleanup_0c902d58.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_callback_cleanup_0c902d58.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 callback cleanup verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_callback_cleanup_0c902d58.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 callback cleanup registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_indirect_resource_tail_0c905180.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_indirect_resource_tail_0c905180.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 0x0c905180 indirect resource tail verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_indirect_resource_tail_0c905180.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 0x0c905180 resource tail registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_counter_step_0c905b06.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_counter_step_0c905b06.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 counter-step verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_counter_step_0c905b06.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 counter-step registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_global_signed_byte_get_0c914b60.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_global_signed_byte_get_0c914b60.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 signed-byte getter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_global_signed_byte_get_0c914b60.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 signed-byte getter registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_global_word_nonzero_0c910fd6.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_global_word_nonzero_0c910fd6.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 global-word boolean getter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_global_word_nonzero_0c910fd6.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 global-word boolean getter registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_word_array_find_0c9196d4.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_word_array_find_0c9196d4.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 word-array search verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_word_array_find_0c9196d4.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 word-array search registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_global_byte_set_if_below_five_0c914940.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_global_byte_set_if_below_five_0c914940.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 conditional byte-setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_global_byte_set_if_below_five_0c914940.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 conditional byte-setter registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_gated_global_word_set_0c91344e.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_gated_global_word_set_0c91344e.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 gated global-word setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_gated_global_word_set_0c91344e.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 gated global-word setter registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_signed_byte_word_lookup_0c918790.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_signed_byte_word_lookup_0c918790.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 signed-byte table lookup verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_signed_byte_word_lookup_0c918790.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 signed-byte table lookup registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_nested_signed_range_predicate_0c91a75c.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_nested_signed_range_predicate_0c91a75c.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 nested signed-range predicate verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_nested_signed_range_predicate_0c91a75c.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 nested signed-range predicate registration failed' }
& (Join-Path $PSScriptRoot 'build_stg09_recursive_tree_node_count_0c920d4c.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_recursive_tree_node_count_0c920d4c.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 recursive tree counter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_recursive_tree_node_count_0c920d4c.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 recursive tree counter registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_global_reference_release_0c918944.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_global_reference_release_0c918944.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 global reference-release verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_global_reference_release_0c918944.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 global reference-release registration failed' }

& (Join-Path $PSScriptRoot 'build_stg09_resource_release_wrapper_0c9189ae.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_stg09_resource_release_wrapper_0c9189ae.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 resource-release wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stg09_resource_release_wrapper_0c9189ae.py')
if ($LASTEXITCODE -ne 0) { throw 'STG09 resource-release wrapper registration failed' }

& (Join-Path $PSScriptRoot 'build_summary_accessors.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_summary_accessors.py')
if ($LASTEXITCODE -ne 0) { throw 'SUMMARY accessor verification failed' }
& $python (Join-Path $PSScriptRoot 'register_summary_accessors.py')
if ($LASTEXITCODE -ne 0) { throw 'SUMMARY accessor registration failed' }

& (Join-Path $PSScriptRoot 'build_advertise_accessors.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_advertise_accessors.py')
if ($LASTEXITCODE -ne 0) { throw 'ADVERTISE accessor verification failed' }
& $python (Join-Path $PSScriptRoot 'register_advertise_accessors.py')
if ($LASTEXITCODE -ne 0) { throw 'ADVERTISE accessor registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_update_object_accumulator_8c6404c8.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_update_object_accumulator_8c6404c8.py')
if ($LASTEXITCODE -ne 0) { throw 'Object accumulator verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_update_object_accumulator_8c6404c8.py')
if ($LASTEXITCODE -ne 0) { throw 'Object accumulator registration failed' }


& (Join-Path $PSScriptRoot 'build_sa_index_before_table_tail_8c021868.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_index_before_table_tail_8c021868.py')
if ($LASTEXITCODE -ne 0) { throw 'Index boundary predicate verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_index_before_table_tail_8c021868.py')
if ($LASTEXITCODE -ne 0) { throw 'Index boundary predicate registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_set_global_byte_high_8c042e20.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_set_global_byte_high_8c042e20.py')
if ($LASTEXITCODE -ne 0) { throw 'Global high-byte setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_set_global_byte_high_8c042e20.py')
if ($LASTEXITCODE -ne 0) { throw 'Global high-byte setter registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_input_flag_updates_8c0509b8.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_input_flag_updates_8c0509b8.py')
if ($LASTEXITCODE -ne 0) { throw 'Input flag helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_input_flag_updates_8c0509b8.py')
if ($LASTEXITCODE -ne 0) { throw 'Input flag helper registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_update_object_lifecycle_state_8c015ee0.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_update_object_lifecycle_state_8c015ee0.py')
if ($LASTEXITCODE -ne 0) { throw 'Object lifecycle state helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_update_object_lifecycle_state_8c015ee0.py')
if ($LASTEXITCODE -ne 0) { throw 'Object lifecycle state helper registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_classify_object_mode_byte_8c01b5e0.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_classify_object_mode_byte_8c01b5e0.py')
if ($LASTEXITCODE -ne 0) { throw 'Object mode byte classifier verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_classify_object_mode_byte_8c01b5e0.py')
if ($LASTEXITCODE -ne 0) { throw 'Object mode byte classifier registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_update_object_mode_and_table_8c01b3e0.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_update_object_mode_and_table_8c01b3e0.py')
if ($LASTEXITCODE -ne 0) { throw 'Object mode/table helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_update_object_mode_and_table_8c01b3e0.py')
if ($LASTEXITCODE -ne 0) { throw 'Object mode/table helper registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_indirect_service_tail_wrappers_8c10d938.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_indirect_service_tail_wrappers_8c10d938.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect service tail wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_indirect_service_tail_wrappers_8c10d938.py')
if ($LASTEXITCODE -ne 0) { throw 'Indirect service tail wrapper registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_engine_service_tail_8c644d84.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_engine_service_tail_8c644d84.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine service tail verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_engine_service_tail_8c644d84.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine service tail registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_indirect_indexed_dispatch_8c10cd38.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_indirect_indexed_dispatch_8c10cd38.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_indirect_indexed_dispatch_8c10cd38.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed dispatcher registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_initializer_service_wrapper_8c093b1e.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_initializer_service_wrapper_8c093b1e.py')
if ($LASTEXITCODE -ne 0) { throw 'Initializer service wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_initializer_service_wrapper_8c093b1e.py')
if ($LASTEXITCODE -ne 0) { throw 'Initializer service wrapper registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_write_engine_global_field_8c6607c8.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_write_engine_global_field_8c6607c8.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine global field writer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_write_engine_global_field_8c6607c8.py')
if ($LASTEXITCODE -ne 0) { throw 'Engine global field writer registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_sign_extend_register_byte_dispatch_8c669450.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_sign_extend_register_byte_dispatch_8c669450.py')
if ($LASTEXITCODE -ne 0) { throw 'Signed-byte dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_sign_extend_register_byte_dispatch_8c669450.py')
if ($LASTEXITCODE -ne 0) { throw 'Signed-byte dispatcher registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_object_callback_tail_dispatch_8c0663ca.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_object_callback_tail_dispatch_8c0663ca.py')
if ($LASTEXITCODE -ne 0) { throw 'Object callback dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_object_callback_tail_dispatch_8c0663ca.py')
if ($LASTEXITCODE -ne 0) { throw 'Object callback dispatcher registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_callback_tail_load_object_word_8c65bc9c.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_callback_tail_load_object_word_8c65bc9c.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback object-word tail verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_callback_tail_load_object_word_8c65bc9c.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback object-word tail registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_global_callback_tail_dispatch_8c6514e0.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_global_callback_tail_dispatch_8c6514e0.py')
if ($LASTEXITCODE -ne 0) { throw 'Global callback dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_global_callback_tail_dispatch_8c6514e0.py')
if ($LASTEXITCODE -ne 0) { throw 'Global callback dispatcher registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_init_object_then_callback_8c0b70e8.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_init_object_then_callback_8c0b70e8.py')
if ($LASTEXITCODE -ne 0) { throw 'Object initializer callback verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_init_object_then_callback_8c0b70e8.py')
if ($LASTEXITCODE -ne 0) { throw 'Object initializer callback registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_init_object_then_callback_r6_8c0cd09c.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_init_object_then_callback_r6_8c0cd09c.py')
if ($LASTEXITCODE -ne 0) { throw 'R6 object initializer callback verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_init_object_then_callback_r6_8c0cd09c.py')
if ($LASTEXITCODE -ne 0) { throw 'R6 object initializer callback registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_callback_service_call_wrapper_8c645218.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_callback_service_call_wrapper_8c645218.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback service wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_callback_service_call_wrapper_8c645218.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback service wrapper registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_callback_service_call_wrapper_8c10d3c8.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_callback_service_call_wrapper_8c10d3c8.py')
if ($LASTEXITCODE -ne 0) { throw 'Secondary callback service wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_callback_service_call_wrapper_8c10d3c8.py')
if ($LASTEXITCODE -ne 0) { throw 'Secondary callback service wrapper registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_callback_service_call_return_one_8c6452d6.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_callback_service_call_return_one_8c6452d6.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback return-one wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_callback_service_call_return_one_8c6452d6.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback return-one wrapper registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_tail_dispatch_object_signed_field_8c067c14.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_tail_dispatch_object_signed_field_8c067c14.py')
if ($LASTEXITCODE -ne 0) { throw 'Signed object-field tail dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_tail_dispatch_object_signed_field_8c067c14.py')
if ($LASTEXITCODE -ne 0) { throw 'Signed object-field tail dispatcher registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_byte_equal_two_tail_branch_8c65c190.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_byte_equal_two_tail_branch_8c65c190.py')
if ($LASTEXITCODE -ne 0) { throw 'Byte equality tail branch verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_byte_equal_two_tail_branch_8c65c190.py')
if ($LASTEXITCODE -ne 0) { throw 'Byte equality tail branch registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_helper_tail_dispatch_global_arg_8c05f000.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_helper_tail_dispatch_global_arg_8c05f000.py')
if ($LASTEXITCODE -ne 0) { throw 'Global argument helper dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_helper_tail_dispatch_global_arg_8c05f000.py')
if ($LASTEXITCODE -ne 0) { throw 'Global argument helper dispatcher registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_record_word_at_index_stride_0x80_8c647ca4.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_record_word_at_index_stride_0x80_8c647ca4.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed record accessor verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_record_word_at_index_stride_0x80_8c647ca4.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed record accessor registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_record_word_at_index_stride_0x80_store_offset_8_8c647cb4.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_record_word_at_index_stride_0x80_store_offset_8_8c647cb4.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed record setter verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_record_word_at_index_stride_0x80_store_offset_8_8c647cb4.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed record setter registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_record_initialize_index_stride_0x80_8c647cc4.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_record_initialize_index_stride_0x80_8c647cc4.py')
if ($LASTEXITCODE -ne 0) { throw 'Stride-0x80 record initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_record_initialize_index_stride_0x80_8c647cc4.py')
if ($LASTEXITCODE -ne 0) { throw 'Stride-0x80 record initializer registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_record_bounded_text_fields_8c647d14.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_record_bounded_text_fields_8c647d14.py')
if ($LASTEXITCODE -ne 0) { throw 'Bounded record text fields verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_record_bounded_text_fields_8c647d14.py')
if ($LASTEXITCODE -ne 0) { throw 'Bounded record text fields registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_record_copy_eight_bytes_tail_8c647d8a.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_record_copy_eight_bytes_tail_8c647d8a.py')
if ($LASTEXITCODE -ne 0) { throw 'Record byte-copy tail-wrapper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_record_copy_eight_bytes_tail_8c647d8a.py')
if ($LASTEXITCODE -ne 0) { throw 'Record byte-copy tail-wrapper registration failed' }


& (Join-Path $PSScriptRoot 'build_sa_record_callback_mode_select_8c647de8.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_record_callback_mode_select_8c647de8.py')
if ($LASTEXITCODE -ne 0) { throw 'Record callback mode selector verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_record_callback_mode_select_8c647de8.py')
if ($LASTEXITCODE -ne 0) { throw 'Record callback mode selector registration failed' }


& (Join-Path $PSScriptRoot 'build_sa_record_mode_value_store_8c647f10.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_record_mode_value_store_8c647f10.py')
if ($LASTEXITCODE -ne 0) { throw 'Record mode value-store verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_record_mode_value_store_8c647f10.py')
if ($LASTEXITCODE -ne 0) { throw 'Record mode value-store registration failed' }


& (Join-Path $PSScriptRoot 'build_sa_record_buffer_descriptors_build_8c647f4c.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_record_buffer_descriptors_build_8c647f4c.py')
if ($LASTEXITCODE -ne 0) { throw 'Record buffer descriptor builder verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_record_buffer_descriptors_build_8c647f4c.py')
if ($LASTEXITCODE -ne 0) { throw 'Record buffer descriptor builder registration failed' }



& (Join-Path $PSScriptRoot 'build_sa_record_setup_initialize_8c647e16.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_record_setup_initialize_8c647e16.py')
if ($LASTEXITCODE -ne 0) { throw 'Record setup initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_record_setup_initialize_8c647e16.py')
if ($LASTEXITCODE -ne 0) { throw 'Record setup initializer registration failed' }


& (Join-Path $PSScriptRoot 'build_sa_record_callback_tail_dispatchers_8c647bb8.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_record_callback_tail_dispatchers_8c647bb8.py')
if ($LASTEXITCODE -ne 0) { throw 'Record callback tail dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_record_callback_tail_dispatchers_8c647bb8.py')
if ($LASTEXITCODE -ne 0) { throw 'Record callback tail dispatcher registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_record_callback_initializers_8c647c50.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_record_callback_initializers_8c647c50.py')
if ($LASTEXITCODE -ne 0) { throw 'Record callback initializer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_record_callback_initializers_8c647c50.py')
if ($LASTEXITCODE -ne 0) { throw 'Record callback initializer registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_input_channel_callback_thunk_8c647908.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_input_channel_callback_thunk_8c647908.py')
if ($LASTEXITCODE -ne 0) { throw 'Input channel callback thunk verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_input_channel_callback_thunk_8c647908.py')
if ($LASTEXITCODE -ne 0) { throw 'Input channel callback thunk registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_input_record_request_store_8c646f68.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_input_record_request_store_8c646f68.py')
if ($LASTEXITCODE -ne 0) { throw 'Input request record writer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_input_record_request_store_8c646f68.py')
if ($LASTEXITCODE -ne 0) { throw 'Input request record writer registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_input_record_address_by_index_8c646856.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_input_record_address_by_index_8c646856.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed input-record address helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_input_record_address_by_index_8c646856.py')
if ($LASTEXITCODE -ne 0) { throw 'Indexed input-record address helper registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_input_callback_and_status_helpers.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_input_callback_and_status_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'Input callback and status helper verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_input_callback_and_status_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'Input callback and status helper registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_input_metadata_bit_locator_8c646b42.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_input_metadata_bit_locator_8c646b42.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata bit locator verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_input_metadata_bit_locator_8c646b42.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata bit locator registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_input_metadata_read_word_8c646b72.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_input_metadata_read_word_8c646b72.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata word reader verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_input_metadata_read_word_8c646b72.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata word reader registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_input_metadata_decode_axes_8c646bbc.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_input_metadata_decode_axes_8c646bbc.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata axes decoder verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_input_metadata_decode_axes_8c646bbc.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata axes decoder registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_input_metadata_decode_sticks_8c646c8a.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_input_metadata_decode_sticks_8c646c8a.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata stick decoder verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_input_metadata_decode_sticks_8c646c8a.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata stick decoder registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_input_metadata_read_word_8c646d30.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_input_metadata_read_word_8c646d30.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata word reader verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_input_metadata_read_word_8c646d30.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata word reader registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_input_metadata_read_two_bytes_8c646d7a.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_input_metadata_read_two_bytes_8c646d7a.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata two-byte reader verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_input_metadata_read_two_bytes_8c646d7a.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata two-byte reader registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_input_metadata_dispatch_setter_8c646db2.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_input_metadata_dispatch_setter_8c646db2.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata setter dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_input_metadata_dispatch_setter_8c646db2.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata setter dispatcher registration failed' }

& (Join-Path $PSScriptRoot 'build_sa_input_metadata_dispatch_getter_8c646e5a.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sa_input_metadata_dispatch_getter_8c646e5a.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata getter dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'register_sa_input_metadata_dispatch_getter_8c646e5a.py')
if ($LASTEXITCODE -ne 0) { throw 'Input metadata getter dispatcher registration failed' }

& (Join-Path $PSScriptRoot 'build_input_service_cleanup_native.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_input_service_cleanup_8c646654.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-service cleanup retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_input_service_cleanup_8c646654.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-service cleanup registration failed' }
& $python (Join-Path $PSScriptRoot 'audit_input_service_cleanup_dependencies.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-service cleanup dependency audit failed' }

& (Join-Path $PSScriptRoot 'build_format_output_ready_8c10e576.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Formatter output-ready helper build failed' }
& $python (Join-Path $PSScriptRoot 'verify_format_output_ready_8c10e576.py')
if ($LASTEXITCODE -ne 0) { throw 'Formatter output-ready helper comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_format_output_ready_8c10e576.py')
if ($LASTEXITCODE -ne 0) { throw 'Formatter output-ready helper registration failed' }

& (Join-Path $PSScriptRoot 'build_format_terminate_text_8c10e566.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Formatter text-termination helper build failed' }
& $python (Join-Path $PSScriptRoot 'verify_format_terminate_text_8c10e566.py')
if ($LASTEXITCODE -ne 0) { throw 'Formatter text-termination helper retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_format_terminate_text_8c10e566.py')
if ($LASTEXITCODE -ne 0) { throw 'Formatter text-termination helper registration failed' }

& (Join-Path $PSScriptRoot 'build_format_integer_convert_8c10e304.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Formatter integer-conversion routine build failed' }
& $python (Join-Path $PSScriptRoot 'verify_format_integer_convert_8c10e304.py')
if ($LASTEXITCODE -ne 0) { throw 'Formatter integer-conversion retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_format_integer_convert_8c10e304.py')
if ($LASTEXITCODE -ne 0) { throw 'Formatter integer-conversion registration failed' }

& (Join-Path $PSScriptRoot 'build_format_emit_text_8c10e50e.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Formatter text-emission helper build failed' }
& $python (Join-Path $PSScriptRoot 'verify_format_emit_text_8c10e50e.py')
if ($LASTEXITCODE -ne 0) { throw 'Formatter text-emission helper retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_format_emit_text_8c10e50e.py')
if ($LASTEXITCODE -ne 0) { throw 'Formatter text-emission helper registration failed' }

& (Join-Path $PSScriptRoot 'build_callback_pair_tail_8c038dfc.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Callback-pair tail wrapper build failed' }
& $python (Join-Path $PSScriptRoot 'verify_callback_pair_tail_8c038dfc.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback-pair tail wrapper retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_callback_pair_tail_8c038dfc.py')
if ($LASTEXITCODE -ne 0) { throw 'Callback-pair tail wrapper registration failed' }

& (Join-Path $PSScriptRoot 'build_debug_log_forward_8c10d400.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Debug-log forwarding wrapper build failed' }
& $python (Join-Path $PSScriptRoot 'verify_debug_log_forward_8c10d400.py')
if ($LASTEXITCODE -ne 0) { throw 'Debug-log forwarding wrapper retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_debug_log_forward_8c10d400.py')
if ($LASTEXITCODE -ne 0) { throw 'Debug-log forwarding wrapper registration failed' }

& (Join-Path $PSScriptRoot 'build_input_device_report_callback_8c65f348.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Input-device report callback build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_report_callback_8c65f348.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-device report callback retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_input_device_report_callback_8c65f348.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-device report callback registration failed' }

& (Join-Path $PSScriptRoot 'build_input_service_entry_8c647198.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Input-service entry build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_service_entry_8c647198.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-service entry retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_input_service_entry_8c647198.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-service entry registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_service_cleanup_integration.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-service cleanup integration comparison failed' }

& (Join-Path $PSScriptRoot 'build_input_device_wait_reset_8c65f2a4.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Input-device wait/reset build failed' }
& $python (Join-Path $PSScriptRoot 'verify_input_device_wait_reset_8c65f2a4.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-device wait/reset retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_input_device_wait_reset_8c65f2a4.py')
if ($LASTEXITCODE -ne 0) { throw 'Input-device wait/reset registration failed' }

& (Join-Path $PSScriptRoot 'build_stream_request_open_prepare_8c65bf42.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Stream request open helper build failed' }
& $python (Join-Path $PSScriptRoot 'verify_stream_request_open_prepare_8c65bf42.py')
if ($LASTEXITCODE -ne 0) { throw 'Stream request callback and helper retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_stream_request_open_prepare_8c65bf42.py')
if ($LASTEXITCODE -ne 0) { throw 'Stream request callback and helper registration failed' }

& (Join-Path $PSScriptRoot 'build_audio_buffer_initialize_8c65b434.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Audio buffer initializer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_audio_buffer_initialize_8c65b434.py')
if ($LASTEXITCODE -ne 0) { throw 'Audio buffer initializer retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_audio_buffer_initialize_8c65b434.py')
if ($LASTEXITCODE -ne 0) { throw 'Audio buffer initializer registration failed' }

& (Join-Path $PSScriptRoot 'build_sound_work_callback_thunks.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Sound-work callback thunk build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_work_callback_thunks.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound-work callback thunk retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_sound_work_callback_thunks.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound-work callback thunk registration failed' }

& (Join-Path $PSScriptRoot 'build_audio_work_buffer_initialize_8c65bd2c.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Audio work-buffer initializer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_audio_work_buffer_initialize_8c65bd2c.py')
if ($LASTEXITCODE -ne 0) { throw 'Audio work-buffer initializer retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_audio_work_buffer_initialize_8c65bd2c.py')
if ($LASTEXITCODE -ne 0) { throw 'Audio work-buffer initializer registration failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_record_allocate_8c65bd46.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Sound stream record allocator build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_record_allocate_8c65bd46.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound stream record allocator retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_sound_stream_record_allocate_8c65bd46.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound stream record allocator registration failed' }

& (Join-Path $PSScriptRoot 'build_stream_request_position_update_8c600e7a.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Stream request position updater build failed' }
& $python (Join-Path $PSScriptRoot 'verify_stream_request_position_update_8c600e7a.py')
if ($LASTEXITCODE -ne 0) { throw 'Stream request position updater retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_stream_request_position_update_8c600e7a.py')
if ($LASTEXITCODE -ne 0) { throw 'Stream request position updater registration failed' }

& (Join-Path $PSScriptRoot 'build_stream_request_seek_query_8c65bfa4.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Native seek/query caller build failed' }
& $python (Join-Path $PSScriptRoot 'verify_stream_request_seek_query_8c65bfa4.py')
if ($LASTEXITCODE -ne 0) { throw 'Native seek/query caller retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_stream_request_seek_query_8c65bfa4.py')
if ($LASTEXITCODE -ne 0) { throw 'Native seek/query caller registration failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_record_lifecycle.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Connected stream-record build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_record_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected stream-record retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_sound_stream_record_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected stream-record evidence registration failed' }

& (Join-Path $PSScriptRoot 'build_sound_stream_pending_cleanup_8c65bf60.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Pending stream cleanup build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_stream_pending_cleanup_8c65bf60.py')
if ($LASTEXITCODE -ne 0) { throw 'Pending stream cleanup retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_sound_stream_pending_cleanup_8c65bf60.py')
if ($LASTEXITCODE -ne 0) { throw 'Pending stream cleanup evidence registration failed' }

& $python (Join-Path $PSScriptRoot 'recover_request_abort_8c6013d2.py')
if ($LASTEXITCODE -ne 0) { throw 'Request-abort static C regeneration failed' }
& (Join-Path $PSScriptRoot 'build_request_abort_8c6013d2.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Request-abort native build failed' }
& $python (Join-Path $PSScriptRoot 'verify_request_abort_8c6013d2.py')
if ($LASTEXITCODE -ne 0) { throw 'Request-abort retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_request_abort_8c6013d2.py')
if ($LASTEXITCODE -ne 0) { throw 'Request-abort evidence registration failed' }

& $python (Join-Path $PSScriptRoot 'recover_request_abort_8c6013d2.py') --routine finalizer
if ($LASTEXITCODE -ne 0) { throw 'Request-finalizer C regeneration failed' }
& $python (Join-Path $PSScriptRoot 'recover_request_abort_8c6013d2.py') --routine status
if ($LASTEXITCODE -ne 0) { throw 'Request-status C regeneration failed' }
& $python (Join-Path $PSScriptRoot 'inventory_request_device_callbacks.py')
if ($LASTEXITCODE -ne 0) { throw 'Request-device static binding checks failed' }
& (Join-Path $PSScriptRoot 'build_stream_cleanup_abort_native.ps1') -Workspace $Workspace
& (Join-Path $PSScriptRoot 'build_native_disc_sector_reader.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Native disc sector reader build failed' }
& $python (Join-Path $PSScriptRoot 'verify_native_disc_sector_reader.py')
if ($LASTEXITCODE -ne 0) { throw 'Native disc sector reader verification failed' }
& $python (Join-Path $PSScriptRoot 'register_native_disc_sector_reader.py')
if ($LASTEXITCODE -ne 0) { throw 'Native disc sector reader evidence registration failed' }
& (Join-Path $PSScriptRoot 'build_native_disc_queue.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Native disc queue build failed' }
& $python (Join-Path $PSScriptRoot 'verify_native_disc_queue.py')
if ($LASTEXITCODE -ne 0) { throw 'Native disc queue verification failed' }
& $python (Join-Path $PSScriptRoot 'register_native_disc_queue.py')
if ($LASTEXITCODE -ne 0) { throw 'Native disc queue evidence registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_native_disc_read_command.py')
if ($LASTEXITCODE -ne 0) { throw 'Native read-command integration failed' }
& $python (Join-Path $PSScriptRoot 'register_native_disc_read_command.py')
if ($LASTEXITCODE -ne 0) { throw 'Native read-command evidence registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_native_disc_service.py')
if ($LASTEXITCODE -ne 0) { throw 'Native disc service integration failed' }
& $python (Join-Path $PSScriptRoot 'register_native_disc_service.py')
if ($LASTEXITCODE -ne 0) { throw 'Native disc service evidence registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_native_disc_mount_services.py')
if ($LASTEXITCODE -ne 0) { throw 'Native mount service verification failed' }
& $python (Join-Path $PSScriptRoot 'register_native_disc_mount_services.py')
if ($LASTEXITCODE -ne 0) { throw 'Native mount service evidence registration failed' }
& (Join-Path $PSScriptRoot 'build_file_mount_registration.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Mount registration build failed' }
& $python (Join-Path $PSScriptRoot 'verify_file_mount_registration.py')
if ($LASTEXITCODE -ne 0) { throw 'Mount registration verification failed' }
& $python (Join-Path $PSScriptRoot 'register_file_mount_registration.py')
if ($LASTEXITCODE -ne 0) { throw 'Mount registration evidence registration failed' }
& (Join-Path $PSScriptRoot 'build_stream_cleanup_abort_native.ps1') -Workspace $Workspace -WithDiscPlatform -OutputName 'stream_cleanup_disc_native.dll'
if ($LASTEXITCODE -ne 0) { throw 'Native cleanup/disc build failed' }
& $python (Join-Path $PSScriptRoot 'verify_stream_cleanup_disc_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native cleanup/disc verification failed' }
& $python (Join-Path $PSScriptRoot 'register_stream_cleanup_disc_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native cleanup/disc evidence registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_native_file_request_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Original file request lifecycle verification failed' }
& $python (Join-Path $PSScriptRoot 'register_native_file_request_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Original file request lifecycle evidence registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_native_boot_directory.py')
if ($LASTEXITCODE -ne 0) { throw 'Native boot directory selection verification failed' }
& $python (Join-Path $PSScriptRoot 'register_native_boot_directory.py')
if ($LASTEXITCODE -ne 0) { throw 'Native boot directory evidence registration failed' }
& (Join-Path $PSScriptRoot 'build_sound_bank_file_consumer.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Sound-bank file consumer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_native_sound_bank_file_consumer.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound-bank file consumer verification failed' }
& $python (Join-Path $PSScriptRoot 'register_native_sound_bank_file_consumer.py')
if ($LASTEXITCODE -ne 0) { throw 'Native sound-bank file consumer registration failed' }
& (Join-Path $PSScriptRoot 'build_native_sound_bank_file_consumer.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Native C sound-bank consumer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_bank_file_consumer_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native C sound-bank consumer verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_bank_file_consumer_native_guards.py')
if ($LASTEXITCODE -ne 0) { throw 'Native C sound-bank consumer guards failed' }
& $python (Join-Path $PSScriptRoot 'register_sound_bank_file_consumer_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native C sound-bank consumer registration failed' }
& (Join-Path $PSScriptRoot 'build_native_sound_bank_load.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Complete native sound-bank load build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Complete native sound-bank load verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_bank_load_native_guards.py')
if ($LASTEXITCODE -ne 0) { throw 'Complete native sound-bank load guards failed' }
& $python (Join-Path $PSScriptRoot 'register_sound_bank_load_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Complete native sound-bank load registration failed' }
& $python (Join-Path $PSScriptRoot 'prepare_arm_sound_drivers.py')
if ($LASTEXITCODE -ne 0) { throw 'ARM sound-driver payload preparation failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_driver_inventory.py')
if ($LASTEXITCODE -ne 0) { throw 'ARM sound-driver inventory validation failed; regenerate the Ghidra ARM exports' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_driver_inventory.py')
if ($LASTEXITCODE -ne 0) { throw 'ARM sound-driver inventory registration failed' }
& (Join-Path $PSScriptRoot 'build_arm_sound_boot_native.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Native ARM sound boot build failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_boot_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM sound boot comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_reset_services.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM reset-service comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_boot_native_guards.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM sound boot guard comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_boot_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM sound boot registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_commands.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM queued-command comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_commands_guards.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM queued-command guard comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_commands.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM queued-command registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_alignment.py')
if ($LASTEXITCODE -ne 0) { throw 'Legacy ARM memory-alignment comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_bank_select.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM bank-selection comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_bank_select_guards.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM bank-selection guard comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_bank_select.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM bank-selection registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_controls.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM control-command comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_control_scales.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM secondary-control dispatch comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_dsp_config.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM DSP configuration comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_controls_guards.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM control/DSP guard comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_controls.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM control/DSP registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_notes.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM note/allocation comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_command_stream.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM mixed command-stream comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_notes_guards.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM note/allocation guard comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_notes.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM note/allocation registration failed' }
& (Join-Path $PSScriptRoot 'build_arm_sound_status.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM status build failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_status.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM status comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_status.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected native ARM status comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_status.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected native ARM status registration failed' }
& (Join-Path $PSScriptRoot 'build_arm_sound_update.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM update build failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_update.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM update comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_update.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected native ARM update comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_update.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected native ARM update registration failed' }
& (Join-Path $PSScriptRoot 'build_arm_sound_snapshot.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM snapshot build failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_snapshot.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM snapshot comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_snapshot.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM snapshot registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_snapshot.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected native ARM snapshot comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_snapshot.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected native ARM snapshot registration failed' }
& (Join-Path $PSScriptRoot 'build_arm_sound_ramps.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM ramps build failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_ramps.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM ramps comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_ramps.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM ramps registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_ramps.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected native ARM ramps comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_ramp_snapshot_flow.py')
if ($LASTEXITCODE -ne 0) { throw 'Persistent native ARM ramps/snapshot comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_ramps.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected native ARM ramps registration failed' }
& (Join-Path $PSScriptRoot 'build_arm_sound_effects.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM effects build failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_effects.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM effects comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_effects.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM effects registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_effects.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected native ARM effects comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_effect_flow.py')
if ($LASTEXITCODE -ne 0) { throw 'Original-site native ARM effect-flow comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_effects.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected native ARM effects registration failed' }
& (Join-Path $PSScriptRoot 'build_arm_sound_stream_update.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM stream-update build failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_stream_update.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM stream-update comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_stream_update.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM stream-update registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_stream_update.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected native ARM stream-update comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_stream_effect_flow.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM stream/effect interaction comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_stream_update.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected native ARM stream-update registration failed' }
& (Join-Path $PSScriptRoot 'build_arm_sound_sequencer.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM sequencer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_sequencer.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM sequencer comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_sequencer_carry.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM sequencer carry-observability comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_sequencer.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native ARM sequencer registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_sequencer.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected native ARM sequencer comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_sequencer_carry.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected native ARM sequencer carry-observability comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_sequence_effect_flow.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM sequence/effect interaction comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_sequencer.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected native ARM sequencer registration failed' }
& (Join-Path $PSScriptRoot 'build_arm_sound_dispatcher.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Dispatcher overlay native build failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_dispatcher.py')
if ($LASTEXITCODE -ne 0) { throw 'Dispatcher overlay baseline comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_dispatcher_banks.py')
if ($LASTEXITCODE -ne 0) { throw 'Dispatcher SMSB bank/priority comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_dispatcher_streams.py')
if ($LASTEXITCODE -ne 0) { throw 'Dispatcher SOSB stream/DSP comparison failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_dispatcher_stereo.py')
if ($LASTEXITCODE -ne 0) { throw 'Dispatcher stereo/pool verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_dispatcher_effect_flow.py')
if ($LASTEXITCODE -ne 0) { throw 'Dispatcher/effects flow verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_dispatcher_positive_flow.py')
if ($LASTEXITCODE -ne 0) { throw 'Dispatcher positive SOSB flow verification failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_dispatcher.py')
if ($LASTEXITCODE -ne 0) { throw 'Dispatcher overlay baseline registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_dispatcher.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_dispatcher_banks.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_dispatcher_streams.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_dispatcher_stereo.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_dispatcher_effect_flow.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_dispatcher_positive_flow.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected dispatcher verification failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_dispatcher_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected dispatcher registration failed' }
& (Join-Path $PSScriptRoot 'build_arm_sound_input.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Standalone native input parser build failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_input.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native input parser verification failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_input.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native input parser registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_input.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected input verification failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_input.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected input registration failed' }
& (Join-Path $PSScriptRoot 'build_arm_sound_exception.ps1') -Workspace $Workspace
if ($LASTEXITCODE -ne 0) { throw 'Standalone native exception handler build failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_exception.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native exception handler verification failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_exception.py')
if ($LASTEXITCODE -ne 0) { throw 'Standalone native exception handler registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_exception.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected exception verification failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_exception.py') --connected
if ($LASTEXITCODE -ne 0) { throw 'Connected exception registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_interrupt_main_flow.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected interrupt/input/main flow verification failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_interrupt_main_flow.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected interrupt/input/main flow registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_arm_sound_release_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Release-mode computed dispatch verification failed' }
& $python (Join-Path $PSScriptRoot 'register_arm_sound_release_dispatch.py')
if ($LASTEXITCODE -ne 0) { throw 'Release-mode computed dispatch registration failed' }
& $python (Join-Path $PSScriptRoot 'audit_arm_sound_native_sites.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM site audit failed' }
& $python (Join-Path $PSScriptRoot 'audit_arm_sound_warnings.py')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM warning audit failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_discovery.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch discovery evidence verification failed; regenerate read-only Ghidra evidence' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_discovery.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch discovery registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_transition.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch transition static recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_transition.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_transition.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch transition retail comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_transition.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch transition registration failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_early_native.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_early_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch native early-dependency comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_early_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch native early-dependency registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_value_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch native value-chain comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_value_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch native value-chain registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_shared_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch shared helper static recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_shared_helpers.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_shared_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch shared helper retail comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_shared_helpers.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch shared helper registration failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_status_query.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_status_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch native status-query comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_status_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch native status-query registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_shared_check.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch shared-check static recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_shared_check.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_shared_check.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch shared-check native comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_shared_check.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch shared-check registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_active_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch active-query group recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_active_query.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_active_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch active-query group comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_active_query.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch active-query group registration failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_reset.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_reset.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch native reset comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_reset.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch native reset registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_detach.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch detach/vector static recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_detach.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_detach.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch detach/vector native comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_detach.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch detach/vector registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_transform.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch transform static recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_transform.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_transform.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch transform/native-pop comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_transform.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch transform registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_matrix_init.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch matrix-init inventory validation failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_matrix_init.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_matrix_init.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch matrix-init/native-chain comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_matrix_init.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch matrix-init registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_axes.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch axis inventory validation failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_axes.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_axes.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch native-axis comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_axes.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch native-axis registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_vector_calc.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch vector-calculation inventory failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_vector_calc.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_vector_calc.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch vector-calculation native comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_vector_calc.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatch vector-calculation registration failed' }
& (Join-Path $PSScriptRoot 'build_sh4_vector_numeric.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sh4_vector_numeric.py')
if ($LASTEXITCODE -ne 0) { throw 'SH-4 numerical instruction checks failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_numeric.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_numeric.py')
if ($LASTEXITCODE -ne 0) { throw 'Native numerical transform comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_numeric.py')
if ($LASTEXITCODE -ne 0) { throw 'Native numerical support evidence registration failed' }
& (Join-Path $PSScriptRoot 'build_sh4_multiply_extended.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sh4_multiply_extended.py')
if ($LASTEXITCODE -ne 0) { throw 'Extended scalar multiplication checks failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_scalar_complete.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_scalar_complete.py')
if ($LASTEXITCODE -ne 0) { throw 'Extended scalar numerical caller comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_scalar_complete.py')
if ($LASTEXITCODE -ne 0) { throw 'Extended scalar numerical caller registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_sound.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound request and selector byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_sound.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_sound.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound request and selector comparisons failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_sound_complete.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_sound_complete.py')
if ($LASTEXITCODE -ne 0) { throw 'Closed sound numerical caller comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_sound.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound request and caller registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_prefix_gate.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatcher prefix gate byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_prefix_gate.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_prefix_gate.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatcher prefix gate comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_prefix_gate.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatcher prefix gate registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_notification.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatcher notification byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_notification.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_notification.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatcher native notification comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_notification.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatcher notification registration failed' }
& $python (Join-Path $PSScriptRoot 'inventory_main_dispatch_task_disposal.py')
if ($LASTEXITCODE -ne 0) { throw 'Task disposal original-byte inventory failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_notification_cleanup.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_task_disposal.py')
if ($LASTEXITCODE -ne 0) { throw 'Native standalone task disposal comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_notification_cleanup.py')
if ($LASTEXITCODE -ne 0) { throw 'Native notification cleanup caller comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_notification_cleanup.py')
if ($LASTEXITCODE -ne 0) { throw 'Native task disposal and notification cleanup registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_spawn_setter.py')
if ($LASTEXITCODE -ne 0) { throw 'Native spawn and callback setter byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_spawn_setter.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_spawn_setter.py')
if ($LASTEXITCODE -ne 0) { throw 'Native spawn and callback setter comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_spawn_setter.py')
if ($LASTEXITCODE -ne 0) { throw 'Native spawn and callback setter registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_cancellation.py')
if ($LASTEXITCODE -ne 0) { throw 'Cancellation original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_cancellation.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_cancellation.py')
if ($LASTEXITCODE -ne 0) { throw 'Native cancellation comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_cancellation.py')
if ($LASTEXITCODE -ne 0) { throw 'Native cancellation registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_context_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Stored context callback original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_context_callback.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_context_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Stored context callback lifecycle comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_context_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Stored context callback registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_spawn_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Spawn callback/helpers original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_spawn_callback.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_spawn_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Spawn callback/helpers comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_spawn_callback_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Spawn callback lifecycle comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_spawn_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Spawn callback/helpers registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_particle_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Particle callback original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_particle_callback.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_particle_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Particle callback and render-state comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_particle_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Particle lifetime comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_particle_callback.py')
if ($LASTEXITCODE -ne 0) { throw 'Particle callback registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_particle_geometry.py')
if ($LASTEXITCODE -ne 0) { throw 'Particle geometry original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_particle_geometry.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_particle_geometry.py')
if ($LASTEXITCODE -ne 0) { throw 'Particle geometry/model wrapper comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_particle_geometry_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Geometry-connected particle lifetime comparisons failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_particle_geometry.py')
if ($LASTEXITCODE -ne 0) { throw 'Particle geometry registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_model_traversal.py')
if ($LASTEXITCODE -ne 0) { throw 'Model traversal original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_model_traversal.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_model_traversal.py')
if ($LASTEXITCODE -ne 0) { throw 'Recursive model traversal comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_model_traversal_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Model-traversal particle lifetimes failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_model_traversal.py')
if ($LASTEXITCODE -ne 0) { throw 'Recursive model traversal registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_mesh.py')
if ($LASTEXITCODE -ne 0) { throw 'Mesh/material-loop original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_mesh.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_mesh.py')
if ($LASTEXITCODE -ne 0) { throw 'Mesh/material-loop comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_mesh_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Mesh-connected particle lifetimes failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_mesh.py')
if ($LASTEXITCODE -ne 0) { throw 'Mesh/material-loop registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_mesh_services.py')
if ($LASTEXITCODE -ne 0) { throw 'Filter/material-services original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_mesh_services.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_mesh_services.py')
if ($LASTEXITCODE -ne 0) { throw 'Native filter/material-services comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_mesh_services_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Filter/material-services particle lifetimes failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_mesh_services.py')
if ($LASTEXITCODE -ne 0) { throw 'Filter/material-services registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_vertex_workers.py')
if ($LASTEXITCODE -ne 0) { throw 'Vertex-worker original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_vertex_workers.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_vertex_workers.py')
if ($LASTEXITCODE -ne 0) { throw 'Native vertex-worker/queue comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_vertex_workers_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Vertex-worker particle lifetimes failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_vertex_workers.py')
if ($LASTEXITCODE -ne 0) { throw 'Vertex-worker registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_acquisition.py')
if ($LASTEXITCODE -ne 0) { throw 'Header-acquisition original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_acquisition.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_acquisition.py')
if ($LASTEXITCODE -ne 0) { throw 'Native header-acquisition comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_acquisition_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Header-acquisition particle lifetimes failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_acquisition.py')
if ($LASTEXITCODE -ne 0) { throw 'Header-acquisition registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'Primitive original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_primitives.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'Native primitive comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_primitives_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Primitive particle lifetimes failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'Primitive registration failed' }
& $python (Join-Path $PSScriptRoot 'inventory_main_dispatch_clipping.py')
if ($LASTEXITCODE -ne 0) { throw 'Next clipping-dependency original-byte inventory failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_near_clip.py')
if ($LASTEXITCODE -ne 0) { throw 'Near-clip original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_near_clip.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_near_clip.py')
if ($LASTEXITCODE -ne 0) { throw 'Native near-plane clipping comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_near_clip_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Native near-clip particle lifetimes failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_near_clip.py')
if ($LASTEXITCODE -ne 0) { throw 'Near-plane clipper registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_alternate_strips.py')
if ($LASTEXITCODE -ne 0) { throw 'Alternate strip original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_alternate_strips.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_alternate_strips.py')
if ($LASTEXITCODE -ne 0) { throw 'Native alternate-strip comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_alternate_strips_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Alternate-strip linked particle lifetimes failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_alternate_strips.py')
if ($LASTEXITCODE -ne 0) { throw 'Alternate-strip registration failed' }
& $python (Join-Path $PSScriptRoot 'inventory_main_dispatch_acquisition_variants.py')
if ($LASTEXITCODE -ne 0) { throw 'Acquisition-variant original-byte inventory failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_acquisition_variants.py')
if ($LASTEXITCODE -ne 0) { throw 'Acquisition-variant static recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_acquisition_variants.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_acquisition_variants.py')
if ($LASTEXITCODE -ne 0) { throw 'Native acquisition-variant comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_acquisition_variants_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Acquisition-variant linked lifetimes failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_acquisition_variants.py')
if ($LASTEXITCODE -ne 0) { throw 'Acquisition-variant registration failed' }
& $python (Join-Path $PSScriptRoot 'inventory_main_dispatch_primitive_families.py')
if ($LASTEXITCODE -ne 0) { throw 'Next primitive-family original-byte inventory failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_lit_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'Normal-lit primitive original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_lit_primitives.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_lit_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'Normal-lit primitive comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_lit_primitives_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Normal-lit primitive linked lifetimes failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_lit_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'Normal-lit primitive registration failed' }
& $python (Join-Path $PSScriptRoot 'inventory_main_dispatch_lit_lower.py')
if ($LASTEXITCODE -ne 0) { throw 'Next normal-lit lower-helper inventory failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_lit_near_clip.py')
if ($LASTEXITCODE -ne 0) { throw 'Lit near-clip original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_lit_near_clip.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_lit_near_clip.py')
if ($LASTEXITCODE -ne 0) { throw 'Native lit near-clipping comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_lit_near_clip_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Lit near-clip lifetime regressions failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_lit_near_clip.py')
if ($LASTEXITCODE -ne 0) { throw 'Lit near-clip registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_lit_alternate_strips.py')
if ($LASTEXITCODE -ne 0) { throw 'Lit alternate-strip original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_lit_alternate_strips.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_lit_alternate_strips.py')
if ($LASTEXITCODE -ne 0) { throw 'Native lit alternate-strip comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_lit_alternate_strips_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Lit alternate-strip lifetime regressions failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_lit_alternate_strips.py')
if ($LASTEXITCODE -ne 0) { throw 'Lit alternate-strip registration failed' }
& $python (Join-Path $PSScriptRoot 'inventory_main_dispatch_next_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'Next primitive-pair inventory failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_uv_lit_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'UV/lit primitive original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_uv_lit_primitives.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_lit_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'Native UV/lit primitive comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_lit_primitives_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'UV/lit primitive lifetime regressions failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_uv_lit_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'UV/lit primitive registration failed' }
& $python (Join-Path $PSScriptRoot 'inventory_main_dispatch_uv_lower.py')
if ($LASTEXITCODE -ne 0) { throw 'Next UV lower-helper inventory failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_uv_near_clip.py')
if ($LASTEXITCODE -ne 0) { throw 'UV near-clip original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_uv_near_clip.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_near_clip.py')
if ($LASTEXITCODE -ne 0) { throw 'Native UV near-clipping comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_near_clip_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'UV near-clip lifetime regressions failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_uv_near_clip.py')
if ($LASTEXITCODE -ne 0) { throw 'UV near-clip registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_uv_alternate_strips.py')
if ($LASTEXITCODE -ne 0) { throw 'UV alternate-strip original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_uv_alternate_strips.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_alternate_strips.py')
if ($LASTEXITCODE -ne 0) { throw 'Native UV alternate-strip comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_alternate_strips_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'UV alternate-strip lifetime regressions failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_uv_alternate_strips.py')
if ($LASTEXITCODE -ne 0) { throw 'UV alternate-strip registration failed' }
& $python (Join-Path $PSScriptRoot 'inventory_main_dispatch_uv_family2.py')
if ($LASTEXITCODE -ne 0) { throw 'Next primitive-pair inventory failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_environment_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'Environment primitive original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_environment_primitives.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'Native environment primitive comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_primitives_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Environment primitive lifetime regressions failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_environment_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'Environment primitive registration failed' }
& $python (Join-Path $PSScriptRoot 'inventory_main_dispatch_environment_lower.py')
if ($LASTEXITCODE -ne 0) { throw 'Environment lower-helper inventory failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_environment_near_clip.py')
if ($LASTEXITCODE -ne 0) { throw 'Environment near-clip original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_environment_near_clip.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_near_clip.py')
if ($LASTEXITCODE -ne 0) { throw 'Native environment near-clip comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_near_clip_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Environment near-clip lifetime regressions failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_environment_near_clip.py')
if ($LASTEXITCODE -ne 0) { throw 'Environment near-clip registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_environment_alternate_strips.py')
if ($LASTEXITCODE -ne 0) { throw 'Environment alternate-strip original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_environment_alternate_strips.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_alternate_strips.py')
if ($LASTEXITCODE -ne 0) { throw 'Native environment alternate-strip comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_alternate_strips_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Environment alternate-strip lifetime regressions failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_environment_alternate_strips.py')
if ($LASTEXITCODE -ne 0) { throw 'Environment alternate-strip registration failed' }
& $python (Join-Path $PSScriptRoot 'inventory_main_dispatch_next_family.py')
if ($LASTEXITCODE -ne 0) { throw 'Next primitive-family inventory failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_face_lit_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'Face-lit original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_face_lit_primitives.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_face_lit_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'Native face-lit comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_face_lit_primitives_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Face-lit lifetime regressions failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_face_lit_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'Face-lit registration failed' }
& $python (Join-Path $PSScriptRoot 'inventory_main_dispatch_face_lit_lower.py')
if ($LASTEXITCODE -ne 0) { throw 'Face-lit lower-helper inventory failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_face_near_clip.py')
if ($LASTEXITCODE -ne 0) { throw 'Face-near original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_face_near_clip.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_face_near_clip.py')
if ($LASTEXITCODE -ne 0) { throw 'Native face-near comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_face_near_clip_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Face-near lifetime regressions failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_face_near_clip.py')
if ($LASTEXITCODE -ne 0) { throw 'Face-near registration failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_face_alternate_strips.py')
if ($LASTEXITCODE -ne 0) { throw 'Face alternate-strip original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_face_alternate_strips.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_face_alternate_strips.py')
if ($LASTEXITCODE -ne 0) { throw 'Native face alternate-strip comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_face_alternate_strips_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'Face alternate-strip lifetime regressions failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_face_alternate_strips.py')
if ($LASTEXITCODE -ne 0) { throw 'Face alternate-strip registration failed' }
& $python (Join-Path $PSScriptRoot 'inventory_main_dispatch_uv_face_family.py')
if ($LASTEXITCODE -ne 0) { throw 'Next UV/face-family inventory failed' }
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_uv_face_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'UV-face original-byte recovery failed' }
& (Join-Path $PSScriptRoot 'build_main_dispatch_uv_face_primitives.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_face_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'Native UV-face comparisons failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_face_primitives_lifecycle.py')
if ($LASTEXITCODE -ne 0) { throw 'UV-face lifetime regressions failed' }
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_uv_face_primitives.py')
if ($LASTEXITCODE -ne 0) { throw 'UV-face registration failed' }
& $python (Join-Path $PSScriptRoot 'inventory_main_dispatch_uv_face_lower.py')
if ($LASTEXITCODE -ne 0) { throw 'UV-face lower-helper inventory failed' }
# Native signed-UV clipping with supplied face factor; keep prior checkpoint DLLs.
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_uv_face_near_clip.py')
if($LASTEXITCODE -ne 0){throw 'UV-face clipper recovery failed'}
& (Join-Path $PSScriptRoot 'build_main_dispatch_uv_face_near_clip.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_face_near_clip.py')
if($LASTEXITCODE -ne 0){throw 'UV-face clipper comparison failed'}
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_face_near_clip_lifecycle.py')
if($LASTEXITCODE -ne 0){throw 'UV-face clipper lifetime regression failed'}
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_uv_face_near_clip.py')
if($LASTEXITCODE -ne 0){throw 'UV-face clipper registration failed'}

# Native alternate UV-face strips and recovered clipping/normalization dependencies.
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_uv_face_alternate_strips.py')
if($LASTEXITCODE -ne 0){throw 'UV-face alternate-strip recovery failed'}
& (Join-Path $PSScriptRoot 'build_main_dispatch_uv_face_alternate_strips.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_face_alternate_strips.py')
if($LASTEXITCODE -ne 0){throw 'UV-face alternate-strip comparison failed'}
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_face_alternate_strips_lifecycle.py')
if($LASTEXITCODE -ne 0){throw 'UV-face alternate-strip lifetime regression failed'}
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_uv_face_alternate_strips.py')
if($LASTEXITCODE -ne 0){throw 'UV-face alternate-strip registration failed'}

# Signed UV primitives with exact constant lighting factors and explicit lower helpers.
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_uv_constant_primitives.py')
if($LASTEXITCODE -ne 0){throw 'UV-constant primitive recovery failed'}
& (Join-Path $PSScriptRoot 'build_main_dispatch_uv_constant_primitives.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_constant_primitives.py')
if($LASTEXITCODE -ne 0){throw 'UV-constant primitive comparison failed'}
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_constant_primitives_lifecycle.py')
if($LASTEXITCODE -ne 0){throw 'UV-constant primitive lifetime regression failed'}
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_uv_constant_primitives.py')
if($LASTEXITCODE -ne 0){throw 'UV-constant primitive registration failed'}

# Native signed-UV constant-factor near clipping; retain prior pinned builds.
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_uv_constant_near_clip.py')
if($LASTEXITCODE -ne 0){throw 'UV-constant clipper recovery failed'}
& (Join-Path $PSScriptRoot 'build_main_dispatch_uv_constant_near_clip.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_constant_near_clip.py')
if($LASTEXITCODE -ne 0){throw 'UV-constant clipper comparison failed'}
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_constant_near_clip_lifecycle.py')
if($LASTEXITCODE -ne 0){throw 'UV-constant clipper lifetime regression failed'}
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_uv_constant_near_clip.py')
if($LASTEXITCODE -ne 0){throw 'UV-constant clipper registration failed'}

# Native alternate constant-factor UV strips and their recovered clipper.
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_uv_constant_alternate_strips.py')
if($LASTEXITCODE -ne 0){throw 'UV-constant alternate-strip recovery failed'}
& (Join-Path $PSScriptRoot 'build_main_dispatch_uv_constant_alternate_strips.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_constant_alternate_strips.py')
if($LASTEXITCODE -ne 0){throw 'UV-constant alternate-strip comparison failed'}
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_uv_constant_alternate_strips_lifecycle.py')
if($LASTEXITCODE -ne 0){throw 'UV-constant alternate-strip lifetime regression failed'}
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_uv_constant_alternate_strips.py')
if($LASTEXITCODE -ne 0){throw 'UV-constant alternate-strip registration failed'}

# Environment UV generation with normalized face factors and explicit lower services.
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_environment_face_primitives.py')
if($LASTEXITCODE -ne 0){throw 'Environment-face primitive recovery failed'}
& (Join-Path $PSScriptRoot 'build_main_dispatch_environment_face_primitives.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_face_primitives.py')
if($LASTEXITCODE -ne 0){throw 'Environment-face primitive comparison failed'}
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_face_primitives_lifecycle.py')
if($LASTEXITCODE -ne 0){throw 'Environment-face primitive lifetime regression failed'}
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_environment_face_primitives.py')
if($LASTEXITCODE -ne 0){throw 'Environment-face primitive registration failed'}

# Native environment-UV clipping with caller-supplied face factors.
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_environment_face_near_clip.py')
if($LASTEXITCODE -ne 0){throw 'Environment-face clipper recovery failed'}
& (Join-Path $PSScriptRoot 'build_main_dispatch_environment_face_near_clip.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_face_near_clip.py')
if($LASTEXITCODE -ne 0){throw 'Environment-face clipper comparison failed'}
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_face_near_clip_lifecycle.py')
if($LASTEXITCODE -ne 0){throw 'Environment-face clipper lifetime regression failed'}
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_environment_face_near_clip.py')
if($LASTEXITCODE -ne 0){throw 'Environment-face clipper registration failed'}

# Native alternate environment UV strips with face-normalized clipping.
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_environment_face_alternate_strips.py')
if($LASTEXITCODE -ne 0){throw 'Environment-face alternate-strip recovery failed'}
& (Join-Path $PSScriptRoot 'build_main_dispatch_environment_face_alternate_strips.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_face_alternate_strips.py')
if($LASTEXITCODE -ne 0){throw 'Environment-face alternate-strip comparison failed'}
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_face_alternate_strips_lifecycle.py')
if($LASTEXITCODE -ne 0){throw 'Environment-face alternate-strip lifetime regression failed'}
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_environment_face_alternate_strips.py')
if($LASTEXITCODE -ne 0){throw 'Environment-face alternate-strip registration failed'}

# Final mesh primitive parent pair: environment UVs with constant factors.
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_environment_constant_primitives.py')
if($LASTEXITCODE -ne 0){throw 'Environment-constant primitive recovery failed'}
& (Join-Path $PSScriptRoot 'build_main_dispatch_environment_constant_primitives.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_constant_primitives.py')
if($LASTEXITCODE -ne 0){throw 'Environment-constant primitive comparison failed'}
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_constant_primitives_lifecycle.py')
if($LASTEXITCODE -ne 0){throw 'Environment-constant primitive lifetime regression failed'}
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_environment_constant_primitives.py')
if($LASTEXITCODE -ne 0){throw 'Environment-constant primitive registration failed'}

# Native environment UV clipping with exact constant lighting factors.
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_environment_constant_near_clip.py')
if($LASTEXITCODE -ne 0){throw 'Environment-constant clipper recovery failed'}
& (Join-Path $PSScriptRoot 'build_main_dispatch_environment_constant_near_clip.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_constant_near_clip.py')
if($LASTEXITCODE -ne 0){throw 'Environment-constant clipper comparison failed'}
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_constant_near_clip_lifecycle.py')
if($LASTEXITCODE -ne 0){throw 'Environment-constant clipper lifetime regression failed'}
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_environment_constant_near_clip.py')
if($LASTEXITCODE -ne 0){throw 'Environment-constant clipper registration failed'}


# Native final environment-constant alternate strip pair.
& $python (Join-Path $PSScriptRoot 'recover_main_dispatch_environment_constant_alternate_strips.py')
if($LASTEXITCODE -ne 0){throw 'Environment-constant alternate strip recovery failed'}
& (Join-Path $PSScriptRoot 'build_main_dispatch_environment_constant_alternate_strips.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_constant_alternate_strips.py')
if($LASTEXITCODE -ne 0){throw 'Environment-constant alternate strip comparison failed'}
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_environment_constant_alternate_strips_lifecycle.py')
if($LASTEXITCODE -ne 0){throw 'Environment-constant alternate strip lifetime regression failed'}
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_environment_constant_alternate_strips.py')
if($LASTEXITCODE -ne 0){throw 'Environment-constant alternate strip registration failed'}

# Combined native material families and original live-register strip behavior.
& (Join-Path $PSScriptRoot 'build_main_dispatch_mesh_families.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_mesh_families.py')
if($LASTEXITCODE -ne 0){throw 'Combined mesh-family comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_mesh_families_lifecycle.py')
if($LASTEXITCODE -ne 0){throw 'Combined mesh-family lifetime regressions failed'}
& $python (Join-Path $PSScriptRoot 'register_main_dispatch_mesh_families.py')
if($LASTEXITCODE -ne 0){throw 'Combined mesh-family registration failed'}

# Original main-executable model records and native asset streams.
& $python (Join-Path $PSScriptRoot 'inventory_main_model_assets.py')
if($LASTEXITCODE -ne 0){throw 'Main model asset inventory failed'}
& $python (Join-Path $PSScriptRoot 'verify_main_model_inventory.py')
if($LASTEXITCODE -ne 0){throw 'Main model inventory validation failed'}
& (Join-Path $PSScriptRoot 'build_main_model_assets.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_main_model_assets.py')
if($LASTEXITCODE -ne 0){throw 'Original model native comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_main_model_assets.py')
if($LASTEXITCODE -ne 0){throw 'Original model evidence registration failed'}

# Original mapped overlay models and verified AL_MAIN model selector.
& $python (Join-Path $PSScriptRoot 'inventory_overlay_model_assets.py')
if($LASTEXITCODE -ne 0){throw 'Overlay model inventory failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_model_inventory.py')
if($LASTEXITCODE -ne 0){throw 'Overlay model inventory validation failed'}
& (Join-Path $PSScriptRoot 'build_overlay_model_assets.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_overlay_model_assets.py')
if($LASTEXITCODE -ne 0){throw 'Original overlay model comparisons failed'}
& (Join-Path $PSScriptRoot 'build_overlay_face_model_selector.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_overlay_face_model_selector.py')
if($LASTEXITCODE -ne 0){throw 'Overlay model-selector comparison failed'}
& $python (Join-Path $PSScriptRoot 'register_overlay_model_assets.py')
if($LASTEXITCODE -ne 0){throw 'Overlay model evidence registration failed'}

# AL_MAIN initialization caller with native selector, no-op and marker reset.
& $python (Join-Path $PSScriptRoot 'recover_overlay_selector_initializer.py')
if($LASTEXITCODE -ne 0){throw 'Overlay initializer recovery failed'}
& (Join-Path $PSScriptRoot 'build_overlay_selector_initializer.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_overlay_selector_initializer.py')
if($LASTEXITCODE -ne 0){throw 'Overlay initializer native comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_overlay_selector_initializer.py')
if($LASTEXITCODE -ne 0){throw 'Overlay initializer registration failed'}

# Native AL_MAIN object clone/table walks/setup, with real heap and byte copy.
& $python (Join-Path $PSScriptRoot 'recover_overlay_object_setup.py')
if($LASTEXITCODE -ne 0){throw 'Overlay object recovery failed'}
& (Join-Path $PSScriptRoot 'build_overlay_object_setup.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_overlay_object_setup.py')
if($LASTEXITCODE -ne 0){throw 'Overlay object native comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_overlay_object_setup.py')
if($LASTEXITCODE -ne 0){throw 'Overlay object registration failed'}

# Native object appearance variants and allocation-through-selector integration.
& $python (Join-Path $PSScriptRoot 'recover_overlay_object_variants.py')
if($LASTEXITCODE -ne 0){throw 'Overlay material variants recovery failed'}
& (Join-Path $PSScriptRoot 'build_overlay_object_initializer.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_overlay_object_variants.py')
if($LASTEXITCODE -ne 0){throw 'Overlay material variant comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_selector_guard.py')
if($LASTEXITCODE -ne 0){throw 'Guarded selector comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_object_initializer.py')
if($LASTEXITCODE -ne 0){throw 'Combined object initializer comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_overlay_object_initializer.py')
if($LASTEXITCODE -ne 0){throw 'Combined object initializer registration failed'}

& $python (Join-Path $PSScriptRoot 'inventory_overlay_initializer_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Initializer dependency inventory failed'}

# Native AL_MAIN model morph and full small/bulk word-copy dispatch.
& $python (Join-Path $PSScriptRoot 'recover_overlay_model_blend.py')
if($LASTEXITCODE -ne 0){throw 'Overlay morph recovery failed'}
& (Join-Path $PSScriptRoot 'build_overlay_model_blend.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_overlay_model_blend.py')
if($LASTEXITCODE -ne 0){throw 'Overlay morph/copy comparisons failed'}
& $python (Join-Path $PSScriptRoot 'audit_overlay_model_blend_coverage.py')
if($LASTEXITCODE -ne 0){throw 'Overlay morph coverage accounting failed'}
& (Join-Path $PSScriptRoot 'build_overlay_model_blend_initializer.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_overlay_model_blend_initializer.py')
if($LASTEXITCODE -ne 0){throw 'Morph initializer integration failed'}
& $python (Join-Path $PSScriptRoot 'register_overlay_model_blend.py')
if($LASTEXITCODE -ne 0){throw 'Morph initializer registration failed'}

# Fixture-free bounded AL_MAIN initializer, appearance/accessory/animation tails.
& $python (Join-Path $PSScriptRoot 'recover_overlay_initializer_finish.py')
if($LASTEXITCODE -ne 0){throw 'Final initializer dependency recovery failed'}
& (Join-Path $PSScriptRoot 'build_overlay_initializer_finish.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_overlay_initializer_finish.py')
if($LASTEXITCODE -ne 0){throw 'Initializer dependency comparisons failed'}
& (Join-Path $PSScriptRoot 'build_overlay_initializer_complete.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_overlay_initializer_complete.py')
if($LASTEXITCODE -ne 0){throw 'Fixture-free initializer integration failed'}
& $python (Join-Path $PSScriptRoot 'register_overlay_initializer_complete.py')
if($LASTEXITCODE -ne 0){throw 'Complete initializer registration failed'}

& $python (Join-Path $PSScriptRoot 'inventory_overlay_object_update_callers.py')
if($LASTEXITCODE -ne 0){throw 'Object update/caller inventory failed'}

# Native parameter/event update and scale smoothing through existing morph/copy.
& $python (Join-Path $PSScriptRoot 'recover_overlay_parameter_update.py')
if($LASTEXITCODE -ne 0){throw 'Overlay parameter-update recovery failed'}
& (Join-Path $PSScriptRoot 'build_overlay_parameter_update.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_overlay_parameter_update.py')
if($LASTEXITCODE -ne 0){throw 'Overlay parameter/scale original comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_parameter_lifecycle.py')
if($LASTEXITCODE -ne 0){throw 'Original-model parameter/scale lifecycle failed'}
& $python (Join-Path $PSScriptRoot 'register_overlay_parameter_update.py')
if($LASTEXITCODE -ne 0){throw 'Overlay parameter-update registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Overlay draw dependency inventory failed'}

# Complete register-input matrix paths and a separate native renderer bridge.
& $python (Join-Path $PSScriptRoot 'recover_draw_matrix_registers.py')
if($LASTEXITCODE -ne 0){throw 'Register-input draw matrix recovery failed'}
& (Join-Path $PSScriptRoot 'build_draw_matrix_registers.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_matrix_registers.py')
if($LASTEXITCODE -ne 0){throw 'Register-input matrix comparisons failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_matrix_renderer.py')
if($LASTEXITCODE -ne 0){throw 'Complete matrix renderer connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_matrix_renderer.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_matrix_model_callers.py')
if($LASTEXITCODE -ne 0){throw 'Transformed model matrix caller comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_draw_matrix_renderer_lifecycle.py')
if($LASTEXITCODE -ne 0){throw 'Complete matrix renderer lifetime regressions failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_matrix_registers.py')
if($LASTEXITCODE -ne 0){throw 'Register-input matrix registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_missing_helpers.py')
if($LASTEXITCODE -ne 0){throw 'Remaining draw helper byte checks failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Updated draw dependency inventory failed'}

# Native predicate/float-state helpers and qualified original draw-entry gate.
& $python (Join-Path $PSScriptRoot 'recover_draw_predicate_support.py')
if($LASTEXITCODE -ne 0){throw 'Draw predicate/float support recovery failed'}
& (Join-Path $PSScriptRoot 'build_draw_predicate_support.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_predicate_support.py')
if($LASTEXITCODE -ne 0){throw 'Draw predicate/float support comparisons failed'}
& $python (Join-Path $PSScriptRoot 'recover_overlay_draw_candidate.py')
if($LASTEXITCODE -ne 0){throw 'Candidate object-draw static translation failed'}
& (Join-Path $PSScriptRoot 'build_overlay_draw_gate.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_gate.py')
if($LASTEXITCODE -ne 0){throw 'Original object-draw gate comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_predicate_support.py')
if($LASTEXITCODE -ne 0){throw 'Draw helper/qualified gate registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Post-gate draw dependency inventory failed'}

# Expand accepted object drawing through existing state and matrix services.
& $python (Join-Path $PSScriptRoot 'connect_overlay_draw_state.py')
if($LASTEXITCODE -ne 0){throw 'Existing state/matrix draw connection failed'}
& (Join-Path $PSScriptRoot 'build_overlay_draw_state.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_state.py')
if($LASTEXITCODE -ne 0){throw 'Accepted-draw/state native comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_overlay_draw_state.py')
if($LASTEXITCODE -ne 0){throw 'Qualified accepted-draw integration registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_distance_helpers.py')
if($LASTEXITCODE -ne 0){throw 'Next distance/square-root helper inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Post-state draw dependency inventory failed'}

# Complete original distance/signed-sqrt and expanded partial object drawing.
& $python (Join-Path $PSScriptRoot 'recover_draw_distance_helpers.py')
if($LASTEXITCODE -ne 0){throw 'Distance/signed-sqrt recovery failed'}
& (Join-Path $PSScriptRoot 'build_draw_distance_helpers.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_distance_helpers.py')
if($LASTEXITCODE -ne 0){throw 'Distance/signed-sqrt original comparisons failed'}
& $python (Join-Path $PSScriptRoot 'connect_overlay_draw_distance.py')
if($LASTEXITCODE -ne 0){throw 'Distance draw connection failed'}
& (Join-Path $PSScriptRoot 'build_overlay_draw_distance.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_distance.py')
if($LASTEXITCODE -ne 0){throw 'Distance-connected original drawing comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_distance_helpers.py')
if($LASTEXITCODE -ne 0){throw 'Distance/sqrt registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_unindexed_targets.py')
if($LASTEXITCODE -ne 0){throw 'Temporary unindexed-target byte checks failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Post-distance draw dependency inventory failed'}

# Complete motion setup, recursive object control and base transform callbacks.
& $python (Join-Path $PSScriptRoot 'recover_draw_motion_setup.py')
if($LASTEXITCODE -ne 0){throw 'Motion setup/base callback recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_motion_setup.py')
if($LASTEXITCODE -ne 0){throw 'Motion native connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_motion_setup.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_motion_setup.py')
if($LASTEXITCODE -ne 0){throw 'Original motion setup/base comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_motion.py')
if($LASTEXITCODE -ne 0){throw 'Motion-connected original model comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_motion_setup.py')
if($LASTEXITCODE -ne 0){throw 'Motion setup/base registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Post-motion draw dependency inventory failed'}

# Complete original animation formats, keyframe samplers and interpolation.
& $python (Join-Path $PSScriptRoot 'recover_draw_motion_formats.py')
if($LASTEXITCODE -ne 0){throw 'Motion format/interpolation recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_motion_formats.py')
if($LASTEXITCODE -ne 0){throw 'Motion format native connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_motion_formats.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_motion_formats.py')
if($LASTEXITCODE -ne 0){throw 'Original motion format/interpolation comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_motion_formats.py')
if($LASTEXITCODE -ne 0){throw 'Animation-connected original model comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_motion_formats.py')
if($LASTEXITCODE -ne 0){throw 'Motion format/interpolation registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_motion_mesh.py')
if($LASTEXITCODE -ne 0){throw 'Next motion mesh proposal byte checks failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Post-animation draw dependency inventory failed'}

# Recover distinct motion mesh preprocessing and connect native packet output.
& $python (Join-Path $PSScriptRoot 'recover_draw_motion_mesh.py')
if($LASTEXITCODE -ne 0){throw 'Motion mesh preprocessing recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_motion_mesh.py')
if($LASTEXITCODE -ne 0){throw 'Motion mesh native pipeline connection failed'}
& $python (Join-Path $PSScriptRoot 'audit_draw_motion_reuse.py')
if($LASTEXITCODE -ne 0){throw 'Private reused source logic-preservation audit failed'}
& (Join-Path $PSScriptRoot 'build_draw_motion_mesh.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_motion_mesh.py')
if($LASTEXITCODE -ne 0){throw 'Motion mesh original/native packet comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_motion_mesh.py')
if($LASTEXITCODE -ne 0){throw 'Mesh-connected original-model comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_motion_mesh.py')
if($LASTEXITCODE -ne 0){throw 'Motion mesh preprocessing registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_motion_mesh.py')
if($LASTEXITCODE -ne 0){throw 'Registered motion mesh instruction inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_remaining_helpers.py')
if($LASTEXITCODE -ne 0){throw 'Remaining original draw helper byte checks failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Post-mesh original draw dependency inventory failed'}

# Recover original hierarchy preparation/generation and cache matrix helpers.
& $python (Join-Path $PSScriptRoot 'recover_draw_hierarchy.py')
if($LASTEXITCODE -ne 0){throw 'Hierarchy/matrix source recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_hierarchy.py')
if($LASTEXITCODE -ne 0){throw 'Hierarchy native connection failed'}
& $python (Join-Path $PSScriptRoot 'audit_draw_hierarchy_bounds.py')
if($LASTEXITCODE -ne 0){throw 'Qualified hierarchy channel-bound audit failed'}
& (Join-Path $PSScriptRoot 'build_draw_hierarchy.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_hierarchy.py')
if($LASTEXITCODE -ne 0){throw 'Original hierarchy/matrix comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_hierarchy.py')
if($LASTEXITCODE -ne 0){throw 'Hierarchy-connected original-model comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_hierarchy.py')
if($LASTEXITCODE -ne 0){throw 'Hierarchy/matrix registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_remaining_helpers.py')
if($LASTEXITCODE -ne 0){throw 'Post-hierarchy original helper inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Post-hierarchy draw dependency inventory failed'}

# Recover the next original point-transform dependency with real vector support.
& $python (Join-Path $PSScriptRoot 'recover_draw_point.py')
if($LASTEXITCODE -ne 0){throw 'Point-transform source recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_point.py')
if($LASTEXITCODE -ne 0){throw 'Point-transform native connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_point.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_point.py')
if($LASTEXITCODE -ne 0){throw 'Original point-transform comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_point.py')
if($LASTEXITCODE -ne 0){throw 'Point-connected original-model comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_point.py')
if($LASTEXITCODE -ne 0){throw 'Point-transform registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_remaining_helpers.py')
if($LASTEXITCODE -ne 0){throw 'Post-point-transform helper inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Post-point-transform draw dependency inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_object_effects.py')
if($LASTEXITCODE -ne 0){throw 'Next object-effect dependency byte checks failed'}

# Original object-effect profiles and explicit lower rendering boundaries.
& $python (Join-Path $PSScriptRoot 'recover_draw_object_effects.py')
if($LASTEXITCODE -ne 0){throw 'Object-effect profile/dispatcher source recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_object_effects.py')
if($LASTEXITCODE -ne 0){throw 'Object-effect native pipeline connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_object_effects.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_object_effects.py')
if($LASTEXITCODE -ne 0){throw 'Original effect-profile/native-prefix comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_object_effects.py')
if($LASTEXITCODE -ne 0){throw 'Effect-connected original-model comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_object_effects.py')
if($LASTEXITCODE -ne 0){throw 'Effect-profile registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_object_effects.py')
if($LASTEXITCODE -ne 0){throw 'Post-profile effect dependency inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Post-effect original draw dependency inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_effect_lower.py')
if($LASTEXITCODE -ne 0){throw 'Next lower renderer proposal byte checks failed'}

# Expanded original render-state tables, texture headers, visibility and sizes.
& $python (Join-Path $PSScriptRoot 'recover_draw_render_headers.py')
if($LASTEXITCODE -ne 0){throw 'Expanded renderer source recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_render_headers.py')
if($LASTEXITCODE -ne 0){throw 'Render-header native connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_render_headers.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_render_headers.py')
if($LASTEXITCODE -ne 0){throw 'Original render-header comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_render_headers.py')
if($LASTEXITCODE -ne 0){throw 'Render-header-connected original-model comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_render_headers.py')
if($LASTEXITCODE -ne 0){throw 'Render-header registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_effect_lower.py')
if($LASTEXITCODE -ne 0){throw 'Post-header lower renderer inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_object_effects.py')
if($LASTEXITCODE -ne 0){throw 'Post-header effect inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Post-header original draw dependency inventory failed'}

& $python (Join-Path $PSScriptRoot 'inventory_draw_polygon_lower.py')
if($LASTEXITCODE -ne 0){throw 'Next polygon/clipping proposal byte checks failed'}

# Original polygon clipping/header/projection primitives and reused lower services.
& $python (Join-Path $PSScriptRoot 'recover_draw_polygon_lower.py')
if($LASTEXITCODE -ne 0){throw 'Polygon primitive source recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_polygon_lower.py')
if($LASTEXITCODE -ne 0){throw 'Polygon primitive native connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_polygon_lower.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_polygon_lower.py')
if($LASTEXITCODE -ne 0){throw 'Original polygon primitive comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_polygon_lower.py')
if($LASTEXITCODE -ne 0){throw 'Polygon-connected original-model comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_polygon_lower.py')
if($LASTEXITCODE -ne 0){throw 'Polygon primitive registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_polygon_lower.py')
if($LASTEXITCODE -ne 0){throw 'Post-registration polygon instruction inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_object_effects.py')
if($LASTEXITCODE -ne 0){throw 'Post-polygon effect inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Post-polygon original draw dependency inventory failed'}

& $python (Join-Path $PSScriptRoot 'inventory_draw_overlay_fade.py')
if($LASTEXITCODE -ne 0){throw 'Next overlay fade/draw proposal byte checks failed'}

# Original overlay fade/text/quads plus guarded existing vector translation.
& $python (Join-Path $PSScriptRoot 'recover_draw_overlay_fade.py')
if($LASTEXITCODE -ne 0){throw 'Overlay fade/text source recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_overlay_fade.py')
if($LASTEXITCODE -ne 0){throw 'Overlay fade/text native connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_overlay_fade.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_overlay_fade.py')
if($LASTEXITCODE -ne 0){throw 'Original overlay fade/text comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_overlay_fade.py')
if($LASTEXITCODE -ne 0){throw 'Fade-connected original-model comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_overlay_fade.py')
if($LASTEXITCODE -ne 0){throw 'Overlay fade/text registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_overlay_fade.py')
if($LASTEXITCODE -ne 0){throw 'Post-fade byte inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_remaining_helpers.py')
if($LASTEXITCODE -ne 0){throw 'Post-fade remaining helper inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Post-fade original draw dependency inventory failed'}

& $python (Join-Path $PSScriptRoot 'inventory_draw_motion_variants.py')
if($LASTEXITCODE -ne 0){throw 'Next motion/draw dependency inventory failed'}

# Original motion/drawing variants: twelve complete primitives, partial traversal proposals.
& $python (Join-Path $PSScriptRoot 'recover_draw_motion_variants.py')
if($LASTEXITCODE -ne 0){throw 'Motion variant source recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_motion_variants.py')
if($LASTEXITCODE -ne 0){throw 'Motion variant native connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_motion_variants.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_motion_variants.py')
if($LASTEXITCODE -ne 0){throw 'Original motion variant comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_draw_motion_variants_extended.py')
if($LASTEXITCODE -ne 0){throw 'Supplemental motion primitive comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_motion_variants.py')
if($LASTEXITCODE -ne 0){throw 'Motion-connected original-model comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_motion_variants.py')
if($LASTEXITCODE -ne 0){throw 'Complete motion primitive registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_motion_variants.py')
if($LASTEXITCODE -ne 0){throw 'Post-motion byte inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_remaining_helpers.py')
if($LASTEXITCODE -ne 0){throw 'Post-motion remaining helper inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_overlay_draw_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Post-motion original draw inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_motion_next.py')
if($LASTEXITCODE -ne 0){throw 'Next motion service byte inventory failed'}

# Original key searches/interpolation and completed motion samplers/formats.
& $python (Join-Path $PSScriptRoot 'recover_draw_motion_sampling.py')
if($LASTEXITCODE -ne 0){throw 'Motion sampling source recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_motion_sampling.py')
if($LASTEXITCODE -ne 0){throw 'Motion sampling native connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_motion_sampling.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_motion_sampling.py')
if($LASTEXITCODE -ne 0){throw 'Original motion sampling comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_draw_motion_sampling_extended.py')
if($LASTEXITCODE -ne 0){throw 'Independent motion buffer/stream comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_motion_sampling.py')
if($LASTEXITCODE -ne 0){throw 'Affected motion caller/regression comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_motion_sampling.py')
if($LASTEXITCODE -ne 0){throw 'Complete motion sampler/format registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_motion_next.py')
if($LASTEXITCODE -ne 0){throw 'Post-sampling motion proposal inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_sampling_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Next drawing frontier byte inventory failed'}

# Original renderer/GBR initialization, geometry and explicit special-register state.
& $python (Join-Path $PSScriptRoot 'inventory_draw_gbr_state.py')
if($LASTEXITCODE -ne 0){throw 'Original GBR provenance byte checks failed'}
& $python (Join-Path $PSScriptRoot 'recover_draw_gbr_geometry.py')
if($LASTEXITCODE -ne 0){throw 'GBR geometry source recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_gbr_geometry.py')
if($LASTEXITCODE -ne 0){throw 'Explicit GBR native connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_gbr_geometry.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_gbr_geometry.py')
if($LASTEXITCODE -ne 0){throw 'Original GBR geometry comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_draw_gbr_relocation.py')
if($LASTEXITCODE -ne 0){throw 'GBR relocation and boundary comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_gbr_geometry.py')
if($LASTEXITCODE -ne 0){throw 'GBR initialized original-model comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_gbr_geometry.py')
if($LASTEXITCODE -ne 0){throw 'Complete GBR geometry registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_sampling_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Post-GBR drawing frontier inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_geometry_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Next geometry/view proposal byte inventory failed'}

# Original geometry projection, angle sampling, normal lighting and viewport reset.
& $python (Join-Path $PSScriptRoot 'recover_draw_geometry_stream.py')
if($LASTEXITCODE -ne 0){throw 'Geometry stream source recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_geometry_stream.py')
if($LASTEXITCODE -ne 0){throw 'Geometry stream native connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_geometry_stream.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_geometry_stream.py')
if($LASTEXITCODE -ne 0){throw 'Original geometry/view comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_geometry_stream.py')
if($LASTEXITCODE -ne 0){throw 'Original geometry-connected model comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_geometry_stream.py')
if($LASTEXITCODE -ne 0){throw 'Complete geometry/view helper registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_geometry_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Post-geometry proposal inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_material_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Next material/primitive byte inventory failed'}

# Original material selection, queue headers and colour/alpha cache preparation.
& $python (Join-Path $PSScriptRoot 'recover_draw_material_setup.py')
if($LASTEXITCODE -ne 0){throw 'Material setup source recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_material_setup.py')
if($LASTEXITCODE -ne 0){throw 'Material setup native connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_material_setup.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_material_setup.py')
if($LASTEXITCODE -ne 0){throw 'Original material/header/cache comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_material_setup.py')
if($LASTEXITCODE -ne 0){throw 'Material-connected original-model comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_material_setup.py')
if($LASTEXITCODE -ne 0){throw 'Complete material helper registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_material_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Post-material frontier inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_primitive_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Next primitive/sampler byte inventory failed'}

# Original primitive packet emission and default scale buffer.
& $python (Join-Path $PSScriptRoot 'recover_draw_primitive_stream.py')
if($LASTEXITCODE -ne 0){throw 'Primitive stream source recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_primitive_stream.py')
if($LASTEXITCODE -ne 0){throw 'Primitive stream native connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_primitive_stream.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_primitive_stream.py')
if($LASTEXITCODE -ne 0){throw 'Original primitive packet comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_primitive_stream.py')
if($LASTEXITCODE -ne 0){throw 'Primitive-connected original-model comparisons failed'}
# Isolated full MACL/MACH product support for subsequent clipping recovery.
& (Join-Path $PSScriptRoot 'build_sh4_signed_product.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sh4_signed_product.py')
if($LASTEXITCODE -ne 0){throw 'Independent signed wide-product qualification failed'}

& $python (Join-Path $PSScriptRoot 'inventory_draw_primitive_cpu.py')
if($LASTEXITCODE -ne 0){throw 'Pre-registration clipping CPU-state inventory failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_primitive_stream.py')
if($LASTEXITCODE -ne 0){throw 'Complete primitive helper registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_primitive_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Post-primitive byte inventory failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_primitive_cpu.py')
if($LASTEXITCODE -ne 0){throw 'Clipping CPU-state dependency inventory failed'}

& $python (Join-Path $PSScriptRoot 'inventory_draw_continuation_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Next observed drawing boundary inventory failed'}

# Original unsigned-label vector interpolation and affected callback regressions.
& $python (Join-Path $PSScriptRoot 'recover_draw_vector_curve.py')
if($LASTEXITCODE -ne 0){throw 'Vector curve source recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_vector_curve.py')
if($LASTEXITCODE -ne 0){throw 'Vector curve native connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_vector_curve.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_vector_curve.py')
if($LASTEXITCODE -ne 0){throw 'Original vector curve comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_vector_curve.py')
if($LASTEXITCODE -ne 0){throw 'Affected vector-curve callback comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_vector_curve.py')
if($LASTEXITCODE -ne 0){throw 'Complete vector curve registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_continuation_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Post-vector continuation byte inventory failed'}

& $python (Join-Path $PSScriptRoot 'inventory_draw_angle_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Next observed angle continuation inventory failed'}

# Original integer-angle interpolation and complete retained caller corpus.
& $python (Join-Path $PSScriptRoot 'recover_draw_angle_curve.py')
if($LASTEXITCODE -ne 0){throw 'Angle curve source recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_angle_curve.py')
if($LASTEXITCODE -ne 0){throw 'Angle curve native connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_angle_curve.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_angle_curve.py')
if($LASTEXITCODE -ne 0){throw 'Original angle curve comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_draw_angle_boundaries.py')
if($LASTEXITCODE -ne 0){throw 'Angle conversion boundary comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_angle_curve.py')
if($LASTEXITCODE -ne 0){throw 'Connected angle curve comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_angle_curve.py')
if($LASTEXITCODE -ne 0){throw 'Complete angle curve registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_angle_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Post-angle byte inventory failed'}

& $python (Join-Path $PSScriptRoot 'inventory_draw_post_angle_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Next post-angle boundary inventory failed'}

# Original sequential two-vector blend and retained callback corpus.
& $python (Join-Path $PSScriptRoot 'recover_draw_vector_blend.py')
if($LASTEXITCODE -ne 0){throw 'Vector blend recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_vector_blend.py')
if($LASTEXITCODE -ne 0){throw 'Vector blend connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_vector_blend.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_vector_blend.py')
if($LASTEXITCODE -ne 0){throw 'Vector blend instruction comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_vector_blend.py')
if($LASTEXITCODE -ne 0){throw 'Vector blend callback comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_vector_blend.py')
if($LASTEXITCODE -ne 0){throw 'Vector blend registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_post_angle_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Post-blend original-byte inventory failed'}

# Original signed integer-angle blend; all affected callers and eight regressions.
& $python (Join-Path $PSScriptRoot 'recover_draw_angle_blend.py')
if($LASTEXITCODE -ne 0){throw 'Integer-angle blend recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_angle_blend.py')
if($LASTEXITCODE -ne 0){throw 'Integer-angle blend connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_angle_blend.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_angle_blend.py')
if($LASTEXITCODE -ne 0){throw 'Integer-angle blend instruction comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_angle_blend.py')
if($LASTEXITCODE -ne 0){throw 'Integer-angle blend caller comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_angle_blend.py')
if($LASTEXITCODE -ne 0){throw 'Integer-angle blend registration failed'}
& $python (Join-Path $PSScriptRoot 'inventory_draw_post_blend_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Post-blend byte inventory failed'}

& $python (Join-Path $PSScriptRoot 'inventory_draw_primitive_sr.py')
if($LASTEXITCODE -ne 0){throw 'Primitive SR bank-transition inventory failed'}

# Explicit MACH/integer-bank foundation and scoped original clipper sequences.
& (Join-Path $PSScriptRoot 'build_sh4_integer_state.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_sh4_integer_state.py')
if($LASTEXITCODE -ne 0){throw 'Integer control-state verification failed'}
& $python (Join-Path $PSScriptRoot 'recover_draw_clipping_state.py')
if($LASTEXITCODE -ne 0){throw 'Clipping state proposal recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_clipping_state.py')
if($LASTEXITCODE -ne 0){throw 'Clipping state connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_clipping_state.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_clipping_state.py')
if($LASTEXITCODE -ne 0){throw 'Scoped clipping-state comparisons failed'}
& $python (Join-Path $PSScriptRoot 'record_draw_clipping_state.py')
if($LASTEXITCODE -ne 0){throw 'Clipping-state progress recording failed'}

# Complete clipping and real primitive-parent bodies, separate DLL/proof.
& (Join-Path $PSScriptRoot 'build_draw_clipping_complete.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_clipping_complete.py')
if($LASTEXITCODE -ne 0){throw 'Complete clipping/primitive comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_clipping_complete.py')
if($LASTEXITCODE -ne 0){throw 'Complete clipping/primitive registration failed'}
& $python (Join-Path $PSScriptRoot 'verify_overlay_draw_clipping.py')
if($LASTEXITCODE -ne 0){throw 'Complete original-object clipping comparisons failed'}
& $python (Join-Path $PSScriptRoot 'record_overlay_draw_clipping.py')
if($LASTEXITCODE -ne 0){throw 'Original-object clipping evidence recording failed'}
& $python (Join-Path $PSScriptRoot 'verify_draw_material_dispatch.py')
if($LASTEXITCODE -ne 0){throw 'Complete material dispatcher comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_material_dispatch.py')
if($LASTEXITCODE -ne 0){throw 'Complete material dispatcher registration failed'}
& $python (Join-Path $PSScriptRoot 'recover_draw_view_setup.py')
if($LASTEXITCODE -ne 0){throw 'View setup dependency recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_view_setup.py')
if($LASTEXITCODE -ne 0){throw 'View setup dependency connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_view_setup.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_view_setup.py')
if($LASTEXITCODE -ne 0){throw 'View setup dependency comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_draw_view_prefix.py')
if($LASTEXITCODE -ne 0){throw 'Original view-builder prefix comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_view_setup.py')
if($LASTEXITCODE -ne 0){throw 'View setup dependency registration failed'}
& $python (Join-Path $PSScriptRoot 'recover_draw_view_registers.py')
if($LASTEXITCODE -ne 0){throw 'View register service adaptation failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_view_registers.py')
if($LASTEXITCODE -ne 0){throw 'View register service connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_view_registers.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_view_registers.py')
if($LASTEXITCODE -ne 0){throw 'View register/backend comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_draw_view_complete.py')
if($LASTEXITCODE -ne 0){throw 'Complete original view-builder comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_view_complete.py')
if($LASTEXITCODE -ne 0){throw 'Complete original view-builder registration failed'}
& $python (Join-Path $PSScriptRoot 'recover_draw_frame_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Frame dependency recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_frame_dependencies.py')
if($LASTEXITCODE -ne 0){throw 'Frame dependency connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_frame_dependencies.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_frame_setup.py')
if($LASTEXITCODE -ne 0){throw 'Frame setup comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_draw_frame_prefix.py')
if($LASTEXITCODE -ne 0){throw 'Historical frame-prefix comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_frame_setup.py')
if($LASTEXITCODE -ne 0){throw 'Frame setup registration failed'}
& $python (Join-Path $PSScriptRoot 'recover_draw_frame_context.py')
if($LASTEXITCODE -ne 0){throw 'Frame context recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_draw_frame_context.py')
if($LASTEXITCODE -ne 0){throw 'Frame context connection failed'}
& (Join-Path $PSScriptRoot 'build_draw_frame_context.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_draw_frame_context.py')
if($LASTEXITCODE -ne 0){throw 'Frame context comparisons failed'}
& $python (Join-Path $PSScriptRoot 'verify_draw_frame_sequence.py')
if($LASTEXITCODE -ne 0){throw 'Persistent renderer-initialization comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_draw_frame_sequence.py')
if($LASTEXITCODE -ne 0){throw 'Renderer initializer registration failed'}
& $python (Join-Path $PSScriptRoot 'recover_boot_setup.py')
if($LASTEXITCODE -ne 0){throw 'Boot setup recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_boot_setup.py')
if($LASTEXITCODE -ne 0){throw 'Boot setup connection failed'}
& (Join-Path $PSScriptRoot 'build_boot_setup.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_boot_setup.py')
if($LASTEXITCODE -ne 0){throw 'Boot setup comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_boot_setup.py')
if($LASTEXITCODE -ne 0){throw 'Boot setup registration failed'}
& $python (Join-Path $PSScriptRoot 'recover_boot_wait.py')
if($LASTEXITCODE -ne 0){throw 'Boot wait recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_boot_wait.py')
if($LASTEXITCODE -ne 0){throw 'Boot wait connection failed'}
& (Join-Path $PSScriptRoot 'build_boot_wait.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_boot_wait.py')
if($LASTEXITCODE -ne 0){throw 'Boot wait comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_boot_wait.py')
if($LASTEXITCODE -ne 0){throw 'Boot wait helper registration failed'}
& $python (Join-Path $PSScriptRoot 'recover_boot_device.py')
if($LASTEXITCODE -ne 0){throw 'Frame command dependency recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_boot_device.py')
if($LASTEXITCODE -ne 0){throw 'Frame command dependency connection failed'}
& (Join-Path $PSScriptRoot 'build_boot_device.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_boot_device.py')
if($LASTEXITCODE -ne 0){throw 'Frame command and backend comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_boot_device.py')
if($LASTEXITCODE -ne 0){throw 'Frame command dependency registration failed'}
& $python (Join-Path $PSScriptRoot 'recover_boot_submit.py')
if($LASTEXITCODE -ne 0){throw 'Submission dependency recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_boot_submit.py')
if($LASTEXITCODE -ne 0){throw 'Submission dependency connection failed'}
& (Join-Path $PSScriptRoot 'build_boot_submit.ps1') -Workspace $Workspace
& $python (Join-Path $PSScriptRoot 'verify_boot_submit.py')
if($LASTEXITCODE -ne 0){throw 'Submission dependency comparisons failed'}
& $python (Join-Path $PSScriptRoot 'register_boot_submit.py')
if($LASTEXITCODE -ne 0){throw 'Submission helper registration failed'}
& $python (Join-Path $PSScriptRoot 'recover_boot_interrupt.py')
if($LASTEXITCODE -ne 0){throw 'Interrupt dependency recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_boot_interrupt.py')
if($LASTEXITCODE -ne 0){throw 'Interrupt dependency connection failed'}
& (Join-Path $PSScriptRoot 'build_boot_interrupt.ps1') -Workspace $Workspace
foreach($taskScript in @('verify_boot_interrupt.py','verify_boot_interrupt_bus.py','verify_boot_interrupt_prefix.py','qualify_boot_interrupt.py','register_boot_interrupt.py')){
 & $python (Join-Path $PSScriptRoot $taskScript)
 if($LASTEXITCODE -ne 0){throw "Interrupt/completion qualification failed: $taskScript"}
}
& $python (Join-Path $PSScriptRoot 'recover_boot_input.py')
if($LASTEXITCODE -ne 0){throw 'Input dependency recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_boot_input.py')
if($LASTEXITCODE -ne 0){throw 'Input dependency connection failed'}
& (Join-Path $PSScriptRoot 'build_boot_input.ps1') -Workspace $Workspace
foreach($taskScript in @('verify_boot_input.py','verify_boot_input_sequence.py','verify_boot_input_lifecycle.py','qualify_boot_input.py','register_boot_input.py')){
 & $python (Join-Path $PSScriptRoot $taskScript)
 if($LASTEXITCODE -ne 0){throw "Input/wait qualification failed: $taskScript"}
}
& $python (Join-Path $PSScriptRoot 'recover_boot_event.py')
if($LASTEXITCODE -ne 0){throw 'Event integration recovery failed'}
& $python (Join-Path $PSScriptRoot 'connect_boot_event.py')
if($LASTEXITCODE -ne 0){throw 'Event integration connection failed'}
& (Join-Path $PSScriptRoot 'build_boot_event.ps1') -Workspace $Workspace
foreach($taskScript in @('verify_boot_event.py','verify_boot_event_state.py','verify_boot_event_sequences.py','qualify_boot_event.py','register_boot_event.py')){
 & $python (Join-Path $PSScriptRoot $taskScript)
 if($LASTEXITCODE -ne 0){throw "Event integration qualification failed: $taskScript"}
}
& $python (Join-Path $PSScriptRoot 'inventory_draw_geometry_frontier.py')
if($LASTEXITCODE -ne 0){throw 'Post-view geometry inventory failed'}
foreach($inventory in @('inventory_draw_primitive_frontier.py','inventory_draw_primitive_cpu.py','inventory_draw_continuation_frontier.py','inventory_draw_angle_frontier.py','inventory_draw_post_angle_frontier.py','inventory_draw_post_blend_frontier.py')){
 & $python (Join-Path $PSScriptRoot $inventory)
 if($LASTEXITCODE -ne 0){throw "Post-clipping inventory failed: $inventory"}
}

# Registration changes the map's pinned registry input; regenerate it first.
& $python (Join-Path $PSScriptRoot 'map_main_dispatch_cases.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatcher case mapping failed' }
& $python (Join-Path $PSScriptRoot 'verify_main_dispatch_case_map.py')
if ($LASTEXITCODE -ne 0) { throw 'Main dispatcher case mapping verification failed' }





& $python (Join-Path $PSScriptRoot 'verify_file_completion_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Completion adapter verification failed' }
& $python (Join-Path $PSScriptRoot 'verify_device_status_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected device status verification failed' }
& $python (Join-Path $PSScriptRoot 'register_file_completion_connected.py')
if ($LASTEXITCODE -ne 0) { throw 'Completion adapter registration failed' }
if ($LASTEXITCODE -ne 0) { throw 'Connected native cleanup/abort build failed' }
& $python (Join-Path $PSScriptRoot 'verify_device_cancel_path.py')
if ($LASTEXITCODE -ne 0) { throw 'Device cancellation tail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_device_cancel_path.py')
if ($LASTEXITCODE -ne 0) { throw 'Device cancellation tail registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_request_finalize_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native finalizer/status retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_request_finalize_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Native finalizer/status registration failed' }
& $python (Join-Path $PSScriptRoot 'verify_stream_cleanup_abort_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected native cleanup/abort retail comparison failed' }
& $python (Join-Path $PSScriptRoot 'register_stream_cleanup_abort_native.py')
if ($LASTEXITCODE -ne 0) { throw 'Connected native cleanup/abort evidence registration failed' }

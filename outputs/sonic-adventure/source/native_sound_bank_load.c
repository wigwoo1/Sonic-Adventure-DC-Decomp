/* Native integration of recovered file services. Host callbacks provide only
 * disc read-at, timer/MMIO/bootstrap and observation; file/audio dispatch is C.
 * Cache maintenance is a coherent-host no-op; no firmware equivalence claim. */
#include "native_disc_queue.h"
#include "fpu_cleanup_transfer.h"
#include "interrupt_io.h"
#include "input_io.h"
struct sa_cpu_state {uint32_t r[16],sr,pr,pc;};
typedef uint32_t (*sa_read)(void *,uint32_t);
typedef void (*sa_observe)(const uint8_t *,const struct sa_cpu_state *);
typedef int (*sa_dispatch)(uint8_t *,struct sa_cpu_state *,uint32_t,void *);
typedef int (*legacy_call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *opaque,legacy_call call
int sa_file_backend_install(ARGS);
int sa_file_backend_open(ARGS);
int sa_file_bios_disc_info(ARGS);
int sa_file_bios_execute(ARGS);
int sa_file_bios_request(ARGS);
int sa_file_close(ARGS);
int sa_file_context_initialize(ARGS);
int sa_file_device_disc_info(ARGS);
int sa_file_device_disc_request(ARGS);
int sa_file_device_initialize(ARGS);
int sa_file_device_interface_get(ARGS);
int sa_file_device_low_initialize(ARGS);
int sa_file_device_read(ARGS);
int sa_file_device_start(ARGS);
int sa_file_device_version_request(ARGS);
int sa_file_directory_bytes_copy(ARGS);
int sa_file_directory_load(ARGS);
int sa_file_directory_parent_sector_adjust(ARGS);
int sa_file_directory_record_parse(ARGS);
int sa_file_directory_select(ARGS);
int sa_file_directory_storage_initialize(ARGS);
int sa_file_error_set(ARGS);
int sa_file_filename_lookup(ARGS);
int sa_file_handle_open_initialize(ARGS);
int sa_file_mount(ARGS);
int sa_file_mount_sector_read(ARGS);
int sa_file_name_checksum(ARGS);
int sa_file_name_equal(ARGS);
int sa_file_name_equal_folded(ARGS);
int sa_file_name_upper(ARGS);
int sa_file_open_dispatch(ARGS);
int sa_file_read_aligned(ARGS);
int sa_file_read_sectors(ARGS);
int sa_file_reserved_handle_acquire(ARGS);
int sa_file_size_get(ARGS);
int sa_file_volume_signature_check(ARGS);
int sa_file_volume_word_get(ARGS);
int sa_interrupt_handler_primary(ARGS);
int sa_interrupt_handler_set(ARGS);
int sa_mount_driver_enable(ARGS);
int sa_sound_bank_load(ARGS);
int sa_interrupt_callback_create(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,struct sa_interrupt_io *,void *,legacy_call);
int sa_request_finalize_native(uint8_t *,struct sa_cpu_state *,void *,sa_read,sa_dispatch,sa_observe,uint32_t);
int sa_request_abort_native(uint8_t *,struct sa_cpu_state *,void *,sa_read,sa_dispatch,sa_observe,uint32_t);
int sa_engine_context_initialize_native(ARGS);
int sa_engine_error_record(ARGS);
int sa_engine_frame_debug_log(ARGS);
int sa_engine_frame_device_layout_initialize(ARGS);
int sa_engine_frame_initialize(ARGS);
int sa_engine_frame_object_allocate(ARGS);
int sa_engine_frame_object_configure(ARGS);
int sa_engine_frame_object_pool_initialize(ARGS);
int sa_engine_frame_object_release(ARGS);
int sa_engine_frame_queue_initialize(ARGS);
int sa_engine_frame_request_enqueue(ARGS);
int sa_engine_frame_request_pool_initialize(ARGS);
int sa_engine_frame_services(ARGS);
int sa_engine_frame_transfer_begin(ARGS);
int sa_engine_frame_transfer_submit(ARGS);
int sa_engine_frame_work_initialize(ARGS);
int sa_engine_object_address_set(ARGS);
int sa_engine_object_callback_dispatch(ARGS);
int sa_engine_object_callback_set(ARGS);
int sa_engine_object_length_set(ARGS);
int sa_engine_object_state_get(ARGS);
int sa_engine_object_state_set(ARGS);
int sa_engine_pending_count_get(ARGS);
int sa_engine_queue_consume_direct(ARGS);
int sa_engine_record_copy28_tail(ARGS);
int sa_engine_record_copy32_tail(ARGS);
int sa_engine_record_copy8_tail(ARGS);
int sa_engine_record_copy_dispatch(ARGS);
int sa_engine_record_copy_loaded_dispatch(ARGS);
int sa_engine_transfer_busy_get_io(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,const struct sa_input_io *,void *,legacy_call);
int sa_engine_transfer_channel_arm_io(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,const struct sa_input_io *,void *,legacy_call);
int sa_engine_transfer_channel_release_io(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,const struct sa_input_io *,void *,legacy_call);
int sa_engine_transfer_channels_arm(ARGS);
int sa_engine_transfer_channels_release(ARGS);
int sa_engine_transfer_copy_strided_io(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,const struct sa_input_io *,void *,legacy_call);
int sa_engine_transfer_descriptor_acquire(ARGS);
int sa_engine_transfer_fifo_ready_get_io(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,const struct sa_input_io *,void *,legacy_call);
int sa_engine_transfer_fifo_wait(ARGS);
int sa_engine_transfer_read_strided_io(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,const struct sa_input_io *,void *,legacy_call);
int sa_engine_transfer_ready_get_io(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,const struct sa_input_io *,void *,legacy_call);
int sa_engine_transfer_ready_wait(ARGS);
int sa_file_read_unaligned(ARGS);
int sa_file_status_get(ARGS);
int sa_memory_fill(ARGS);
int sa_memory_move(ARGS);
int sa_sound_bank_download(ARGS);
int sa_sound_bank_section_configure(ARGS);
int sa_sound_bank_section_lookup(ARGS);
int sa_sound_bank_section_transfer(ARGS);
int sa_sound_bank_word_write(ARGS);
int sa_sound_command_complete(ARGS);
int sa_sound_command_type1(ARGS);
int sa_sound_command_type2(ARGS);
int sa_sound_low_index(ARGS);
int sa_sound_midi_enqueue(ARGS);
int sa_sound_midi_pack(ARGS);
int sa_sound_multi_unit_download(ARGS);
int sa_sound_request_commit(ARGS);
int sa_sound_request_copy20(ARGS);
int sa_sound_slot_acquire(ARGS);
int sa_sound_slot_acquire_wrapper(ARGS);
int sa_sound_status_byte_get(ARGS);
int sa_sound_status_byte_set(ARGS);
int sa_sound_status_word_get(ARGS);
int sa_sound_status_word_set(ARGS);
int sa_sound_volume_control(ARGS);
int sa_sound_work_dispatch(ARGS);
int sa_engine_transfer_initialize_host(ARGS,const struct sa_input_io *,uint32_t);
struct context {
    struct sa_cpu_state *state;struct sa_disc_queue *queue;struct sa_interrupt_io *interrupt;
    uint32_t *fpul,*macl;uint32_t vector,budget,remaining;
    void *user;sa_read timer;sa_observe observe;sa_dispatch boundary;const struct sa_input_io *io;
};
static uint32_t timer(void *opaque,uint32_t address) {
    struct context *ctx=(struct context *)opaque;
    return ctx->timer(ctx->user,address);
}
static int device(uint8_t *ram,struct sa_cpu_state *state,uint32_t target,void *opaque) {
    struct context *ctx=(struct context *)opaque;
    if(target!=ctx->vector || state->r[6]!=0)return -8;
    uint32_t value=0;
    int result=sa_disc_service(ctx->queue,ram,state->r[7],state->r[4],state->r[5],&value);
    if(result)return result;
    state->r[0]=value;state->pc=state->pr;return 0;
}
static int dispatch(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr) {
    struct context *ctx=(struct context *)opaque;
    ctx->state->pc=target;
    if(!ctx->remaining)return -3;
    --ctx->remaining;
    if(ctx->observe)ctx->observe(m,ctx->state);
    switch(target) {
    case 0x8c0939b0u:return sa_sound_bank_load(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10d938u:return sa_file_bios_request(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10d960u:return sa_file_bios_execute(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10d974u:return sa_file_device_low_initialize(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10d988u:return sa_file_bios_disc_info(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c5ff9c0u:return sa_file_volume_word_get(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c5ff9e6u:return sa_file_volume_signature_check(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c5ffa08u:return sa_file_error_set(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c5ffc98u:return sa_file_mount_sector_read(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c5ffcfeu:return sa_file_directory_parent_sector_adjust(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c5ffd28u:return sa_file_mount(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c600038u:return sa_file_context_initialize(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c60041au:return sa_file_directory_select(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c600464u:return sa_file_handle_open_initialize(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6004d4u:return sa_file_backend_open(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c600d82u:return sa_file_reserved_handle_acquire(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c600dd0u:return sa_file_directory_storage_initialize(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c600df4u:return sa_file_close(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c600e2cu:return sa_file_size_get(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c600f1eu:return sa_file_read_sectors(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c601084u:return sa_file_read_aligned(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c601618u:return sa_file_backend_install(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6016d6u:return sa_file_open_dispatch(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6412c2u:return sa_interrupt_handler_set(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c64135au:return sa_interrupt_handler_primary(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c644fe8u:return sa_file_device_initialize(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c644ff6u:return sa_file_device_read(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6451c8u:return sa_file_device_disc_request(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6451fcu:return sa_file_device_disc_info(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6452d6u:return sa_file_device_start(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c64530au:return sa_file_device_version_request(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c645328u:return sa_file_device_interface_get(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c645348u:return sa_file_name_upper(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c64535cu:return sa_file_directory_bytes_copy(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6453c0u:return sa_file_name_equal(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6453e2u:return sa_file_name_equal_folded(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c645442u:return sa_file_name_checksum(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c645468u:return sa_file_directory_record_parse(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6454feu:return sa_file_directory_load(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6455ceu:return sa_file_filename_lookup(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c658c20u:return sa_mount_driver_enable(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c64594cu:
        return sa_interrupt_callback_create(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx->interrupt,ctx,dispatch);
    case 0x8c5fc0d8u:case 0x8c5fc336u:r[0]=0;return 0;
    case 0x8c600cf0u:case 0x8c6013d2u: {
        struct sa_cpu_state state;
        for(unsigned i=0;i<16;i++)state.r[i]=r[i];
        state.sr=*sr;state.pr=*pr;state.pc=target;
        int result=target==0x8c600cf0u?
            sa_request_finalize_native(m,&state,ctx,timer,device,ctx->observe,ctx->budget):
            sa_request_abort_native(m,&state,ctx,timer,device,ctx->observe,ctx->budget);
        for(unsigned i=0;i<16;i++)r[i]=state.r[i];
        *sr=state.sr;*pr=state.pr;ctx->state->pc=state.pc;return result;
    }
    case 0x8c093240u:return sa_sound_volume_control(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c0933bau:return sa_sound_low_index(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c093ce0u:return sa_sound_bank_download(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10cd38u:return sa_engine_record_copy_dispatch(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10cd44u:return sa_engine_record_copy_loaded_dispatch(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10cd76u:return sa_engine_record_copy32_tail(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10cd7au:return sa_engine_record_copy28_tail(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10cd82u:return sa_sound_request_copy20(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10cd8eu:return sa_engine_record_copy8_tail(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10d25cu:return sa_memory_move(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10d31cu:return sa_memory_fill(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10d460u:return sa_engine_transfer_read_strided_io(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx->io,ctx,dispatch);
    case 0x8c10d770u:return sa_engine_transfer_copy_strided_io(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx->io,ctx,dispatch);
    case 0x8c10ebacu:return sa_engine_transfer_fifo_wait(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10ec40u:return sa_engine_transfer_descriptor_acquire(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10ee20u:return sa_engine_transfer_channels_release(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10ee9cu:return sa_engine_transfer_channels_arm(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10eee0u:return sa_engine_transfer_ready_wait(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10f9a8u:return sa_engine_transfer_fifo_ready_get_io(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx->io,ctx,dispatch);
    case 0x8c10f9bcu:return sa_engine_transfer_channel_release_io(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx->io,ctx,dispatch);
    case 0x8c10fa60u:return sa_engine_transfer_channel_arm_io(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx->io,ctx,dispatch);
    case 0x8c10fb48u:return sa_engine_transfer_ready_get_io(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx->io,ctx,dispatch);
    case 0x8c1101f0u:return sa_engine_transfer_busy_get_io(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx->io,ctx,dispatch);
    case 0x8c600facu:return sa_file_read_unaligned(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6013a4u:return sa_file_status_get(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6434e0u:return sa_engine_frame_object_allocate(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6435bcu:return sa_engine_frame_object_release(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c643624u:return sa_engine_frame_object_configure(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c643904u:return sa_sound_bank_section_transfer(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c643970u:return sa_sound_multi_unit_download(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c643ac0u:return sa_sound_slot_acquire_wrapper(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c643b9cu:return sa_sound_midi_enqueue(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c643c18u:return sa_sound_midi_pack(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c643d88u:return sa_sound_command_type1(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c643e08u:return sa_sound_command_type2(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6510a4u:return sa_engine_context_initialize_native(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c65a2fcu:return sa_engine_frame_debug_log(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c65a464u:return sa_sound_request_commit(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c65a518u:return sa_engine_object_callback_dispatch(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c65a568u:return sa_engine_object_state_get(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c65a5acu:return sa_engine_object_address_set(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c65a5b4u:return sa_engine_object_callback_set(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c65a5ccu:return sa_engine_object_state_set(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c65a5d4u:return sa_engine_object_length_set(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c65a5dcu:return sa_engine_queue_consume_direct(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c65a730u:return sa_sound_bank_section_configure(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c65a900u:return sa_sound_bank_section_lookup(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c65aa30u:return sa_sound_slot_acquire(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c65acacu:return sa_engine_error_record(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c65acc0u:return sa_engine_frame_initialize(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6669c4u:return sa_engine_frame_request_pool_initialize(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c666a68u:return sa_sound_work_dispatch(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c666ad0u:return sa_sound_command_complete(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c666b34u:return sa_engine_frame_work_initialize(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c666ba8u:return sa_engine_frame_request_enqueue(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c666c2cu:return sa_engine_frame_transfer_begin(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c666e50u:return sa_engine_frame_object_pool_initialize(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c666ef0u:return sa_engine_frame_queue_initialize(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c6671c4u:return sa_sound_status_byte_get(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c667200u:return sa_sound_bank_word_write(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c667240u:return sa_engine_frame_device_layout_initialize(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c668f40u:return sa_engine_frame_services(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c669088u:return sa_engine_frame_transfer_submit(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c66910cu:return sa_sound_status_word_get(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c669144u:return sa_sound_status_byte_set(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c66abfcu:return sa_engine_pending_count_get(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c66ac3cu:return sa_sound_status_word_set(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch);
    case 0x8c10d5a8u:return sa_engine_transfer_initialize_host(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch,ctx->io,ctx->interrupt->vbr);

    default:
        if(target==ctx->vector)return device(m,ctx->state,target,ctx);
        return -8;
    }
}
/* Entries: backend install 8c601618 or sound-bank loader 8c0939b0.
 * Missing services/unsupported root reject atomically. Later errors retain
 * partial CPU/RAM state. Budget bounds dispatch and each recovered core call.
 * Valid RAM, guest pointers and stable host buffers remain caller contracts. */
SA_DISC_EXPORT int sa_sound_bank_load_native(uint8_t *ram,struct sa_cpu_state *state,
    struct sa_fpu_banks *fpu,uint32_t *fpul,uint32_t *macl,struct sa_disc_queue *queue,
    uint32_t vector,void *user,sa_read read,struct sa_interrupt_io *interrupt,
    sa_observe observe,const struct sa_input_io *io,uint32_t budget)
{
    if(!ram || !state || !fpu || !fpul || !macl || !queue || !read || !interrupt ||
       !interrupt->read32 || !interrupt->write32 || !io || !io->read || !io->write)return -2;
    if(state->pc!=0x8c601618u && state->pc!=0x8c0939b0u && state->pc!=0x8c6510a4u &&
       state->pc!=0x8c10d5a8u && state->pc!=0x8c65acc0u)return -8;
    struct context ctx={state,queue,interrupt,fpul,macl,vector,budget,budget,user,read,observe,0,io};
    int result=dispatch(&ctx,state->pc,ram,fpu,&state->sr,state->r,&state->pr);
    if(!result)state->pc=state->pr;
    return result;
}

#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
typedef uint32_t (*sa_read)(void *, uint32_t);
typedef int (*sa_dispatch)(uint8_t *, struct sa_cpu_state *, uint32_t, void *);
typedef int (*sa_abort)(uint8_t *, struct sa_cpu_state *, void *);
typedef void (*sa_observer)(const uint8_t *, const struct sa_cpu_state *);
int sa_request_abort_8c6013d2(uint8_t *,struct sa_cpu_state *,sa_dispatch,void *,uint32_t);
int sa_request_finalize_8c600cf0(uint8_t *,struct sa_cpu_state *,sa_dispatch,void *,uint32_t);
int sa_request_status_8c6013a4(uint8_t *,struct sa_cpu_state *,sa_dispatch,void *,uint32_t);
int sa_sound_stream_pending_cleanup_8c65bf60(uint8_t *,struct sa_cpu_state *,void *,sa_read,sa_abort,sa_observer);
void sa_mmio_complement_sub_8c60260e(const uint8_t *,struct sa_cpu_state *,void *,sa_read);
void sa_subtract_arguments_8c602618(uint8_t *,struct sa_cpu_state *);
void sa_device_cancel_tail_8c6451a4(const uint8_t *,struct sa_cpu_state *);
void sa_device_cancel_service_8c10d9d8(const uint8_t *,struct sa_cpu_state *);
int sa_file_backend_poll_state(uint8_t *,struct sa_cpu_state *,sa_dispatch,void *);
int sa_device_pump_call_connected(uint8_t *,struct sa_cpu_state *,sa_dispatch,void *);
void sa_indirect_service_tail_8c10d960(const uint8_t *,struct sa_cpu_state *);
int sa_file_completion_connected(uint8_t *,struct sa_cpu_state *,sa_dispatch,void *);
int sa_file_device_status_connected(uint8_t *,struct sa_cpu_state *,sa_dispatch,void *);
void sa_indirect_service_tail_8c10d94c(const uint8_t *,struct sa_cpu_state *);

struct sa_services {
    void *user;
    sa_read read;
    sa_dispatch device;
    sa_observer observe;
    uint32_t budget;
    uint32_t dispatch_remaining;
};

static uint32_t read_timer(void *context,uint32_t address)
{
    struct sa_services *services=(struct sa_services *)context;
    return services->read(services->user,address);
}

static int dispatch(uint8_t *ram,struct sa_cpu_state *state,uint32_t target,void *context)
{
    struct sa_services *services=(struct sa_services *)context;
    if(!services->dispatch_remaining--)return -3;
    if(services->observe)services->observe(ram,state);
    if(target==0x8c60260eu)
        sa_mmio_complement_sub_8c60260e(ram,state,context,read_timer);
    else if(target==0x8c602618u)
        sa_subtract_arguments_8c602618(ram,state);
    else if(target==0x8c600cf0u)
        return sa_request_finalize_8c600cf0(ram,state,dispatch,context,services->budget);
    else if(target==0x8c6013a4u)
        return sa_request_status_8c6013a4(ram,state,dispatch,context,services->budget);
    else if(target==0x8c6005c2u)
        return sa_file_backend_poll_state(ram,state,dispatch,context);
    else if(target==0x8c5ffa12u)
        return sa_file_completion_connected(ram,state,dispatch,context);
    else if(target==0x8c6450eau)
        return sa_file_device_status_connected(ram,state,dispatch,context);
    else if(target==0x8c10d94cu) {
        sa_indirect_service_tail_8c10d94c(ram,state);
        return dispatch(ram,state,state->pc,context);
    }
    else if(target==0x8c6452d6u)
        return sa_device_pump_call_connected(ram,state,dispatch,context);
    else if(target==0x8c10d960u) {
        sa_indirect_service_tail_8c10d960(ram,state);
        return dispatch(ram,state,state->pc,context);
    }
    else if(target==0x8c6451a4u) {
        sa_device_cancel_tail_8c6451a4(ram,state);
        return dispatch(ram,state,state->pc,context);
    } else if(target==0x8c10d9d8u) {
        sa_device_cancel_service_8c10d9d8(ram,state);
        return dispatch(ram,state,state->pc,context);
    }
    else
        return services->device(ram,state,target,services->user);
    return 0;
}

SA_EXPORT int sa_request_finalize_native(
    uint8_t *ram,struct sa_cpu_state *state,void *user,sa_read read,
    sa_dispatch device,sa_observer observe,uint32_t budget)
{
    if(!read || !device)return -2;
    struct sa_services services={user,read,device,observe,budget,budget};
    return sa_request_finalize_8c600cf0(ram,state,dispatch,&services,budget);
}

SA_EXPORT int sa_request_abort_native(
    uint8_t *ram,struct sa_cpu_state *state,void *user,sa_read read,
    sa_dispatch device,sa_observer observe,uint32_t budget)
{
    if(!read || !device)return -2;
    struct sa_services services={user,read,device,observe,budget,budget};
    return sa_request_abort_8c6013d2(ram,state,dispatch,&services,budget);
}

static int abort_request(uint8_t *ram,struct sa_cpu_state *state,void *context)
{
    struct sa_services *services=(struct sa_services *)context;
    return sa_request_abort_8c6013d2(ram,state,dispatch,context,services->budget);
}

/* Native call chain: cleanup -> abort -> finalizer/status and timer/subtraction.
 * Services are per invocation; there is no global dispatch context.
 * Missing services reject atomically. Later host errors preserve partial state. */
SA_EXPORT int sa_stream_cleanup_abort_native(
    uint8_t *ram,struct sa_cpu_state *state,void *user,sa_read read,
    sa_dispatch device,sa_observer observe,uint32_t budget)
{
    if(!read || !device)return -2;
    struct sa_services services={user,read,device,observe,budget,budget};
    return sa_sound_stream_pending_cleanup_8c65bf60(ram,state,&services,read_timer,abort_request,observe);
}

#include "fpu_cleanup_transfer.h"

typedef int (*service)(void *, uint32_t, uint8_t *, struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *);
#define ARGS uint8_t *m, struct sa_fpu_banks *f, uint32_t *sr, uint32_t *r, uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *ctx, service call

int sa_input_service_cleanup_8c646654(ARGS);
int sa_input_device_submit_begin_8c65f1aa(ARGS);
int sa_input_device_responses_process_native(ARGS);
int sa_input_device_status_publish(ARGS);
int sa_input_device_submit_rearm(ARGS);
int sa_input_active_device_select_8c64658a(ARGS);
int sa_input_request_completion_impl(ARGS);
int sa_input_request_completion(ARGS);
int sa_input_channel_status_pointer(ARGS);
int sa_input_channel_info_pointer(ARGS);
int sa_input_device_record_pointer(ARGS);

struct cleanup_context { uint32_t *fpul, *macl; void *user; service external; };

static uint32_t rd(uint8_t *m, uint32_t a, unsigned n) {
    uint32_t v = 0;
    for (unsigned i = 0; i < n; ++i) v |= (uint32_t)m[(a & 0x00ffffffu) + i] << (i * 8u);
    return v;
}

static int dispatch(void *ctx, uint32_t t, uint8_t *m, struct sa_fpu_banks *f,
                    uint32_t *sr, uint32_t *r, uint32_t *pr) {
    struct cleanup_context *c = ctx;
    if (t == 0x8c65f1aau) return sa_input_device_submit_begin_8c65f1aa(m, f, sr, r, pr, c->fpul, c->macl, c, dispatch);
    if (t == 0x8c65f202u) return sa_input_device_responses_process_native(m, f, sr, r, pr, c->fpul, c->macl, c, dispatch);
    if (t == 0x8c6461aeu) return sa_input_device_status_publish(m, f, sr, r, pr, c->fpul, c->macl, c, dispatch);
    if (t == 0x8c65f1ecu) return sa_input_device_submit_rearm(m, f, sr, r, pr, c->fpul, c->macl, c, dispatch);
    if (t == 0x8c65f50cu) {
        uint32_t base = rd(m, 0x8c65f530u, 4);
        uint32_t offset = (uint32_t)(int32_t)(int16_t)rd(m, 0x8c65f52cu, 2);
        r[3] = base;
        r[1] = offset;
        r[0] = rd(m, rd(m, base, 4) + offset, 4);
        return 0;
    }
    if (t == 0x8c64658au) return sa_input_active_device_select_8c64658a(m, f, sr, r, pr, c->fpul, c->macl, c, dispatch);
    if (t == 0x8c65f3f4u) return sa_input_request_completion_impl(m, f, sr, r, pr, c->fpul, c->macl, c, dispatch);
    if (t == 0x8c646884u) return sa_input_request_completion(m, f, sr, r, pr, c->fpul, c->macl, c, dispatch);
    if (t == 0x8c646868u) return sa_input_channel_info_pointer(m, f, sr, r, pr, c->fpul, c->macl, c, dispatch);
    if (t == 0x8c646856u) return sa_input_channel_status_pointer(m, f, sr, r, pr, c->fpul, c->macl, c, dispatch);
    if (t == 0x8c65f390u) return sa_input_device_record_pointer(m, f, sr, r, pr, c->fpul, c->macl, c, dispatch);
    return c->external ? c->external(c->user, t, m, f, sr, r, pr) : -3;
}

SA_STAGE_EXPORT int sa_input_service_cleanup_native(ARGS) {
    struct cleanup_context c = {fpul, macl, ctx, call};
    return sa_input_service_cleanup_8c646654(m, f, sr, r, pr, fpul, macl, &c, dispatch);
}

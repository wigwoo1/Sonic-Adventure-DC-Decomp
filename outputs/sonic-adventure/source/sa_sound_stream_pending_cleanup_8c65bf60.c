#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
typedef uint32_t (*sa_mmio_read)(void *, uint32_t);
typedef int (*sa_abort_request)(uint8_t *, struct sa_cpu_state *, void *);
typedef void (*sa_observer)(const uint8_t *, const struct sa_cpu_state *);
void sa_mmio_complement_sub_8c60260e(const uint8_t *, struct sa_cpu_state *, void *, sa_mmio_read);
void sa_subtract_arguments_8c602618(uint8_t *, struct sa_cpu_state *);

static uint32_t at(uint32_t address) { return (address & 0x1fffffffu) - 0x0c000000u; }
static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t p = at(address);
    return (uint32_t)ram[p] | (uint32_t)ram[p+1] << 8 |
           (uint32_t)ram[p+2] << 16 | (uint32_t)ram[p+3] << 24;
}
static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t p = at(address);
    for (unsigned i = 0; i < 4; ++i) ram[p+i] = (uint8_t)(value >> (i*8));
}
static void enter(uint8_t *ram, struct sa_cpu_state *state, uint32_t target,
                  uint32_t return_pc, sa_observer observe)
{
    state->pr = return_pc;
    state->pc = target;
    if (observe) observe(ram, state);
}

/* Original 68-byte caller. Abort is a supplied native service dependency.
 * Host -1 rejects retargeted literals; -2 rejects missing required services.
 * Both checks happen before mutation. Host 0 means completion, not game R0. */
SA_EXPORT int sa_sound_stream_pending_cleanup_8c65bf60(
    uint8_t *ram, struct sa_cpu_state *state, void *context,
    sa_mmio_read read_register, sa_abort_request abort_request, sa_observer observe)
{
    const uint32_t clock = read32(ram, 0x8c65bff0u);
    const uint32_t abort = read32(ram, 0x8c65bff4u);
    const uint32_t subtract = read32(ram, 0x8c65bff8u);
    if (clock != 0x8c60260eu || abort != 0x8c6013d2u || subtract != 0x8c602618u)
        return -1;
    const uint32_t record = state->r[4];
    const int pending = ram[at(record + 2u)] == 1u;
    if (pending && read32(ram, record + 8u) && (!read_register || !abort_request))
        return -2;

    state->r[15] -= 4u;
    write32(ram, state->r[15], state->r[14]);
    state->r[15] -= 4u;
    write32(ram, state->r[15], state->pr);
    state->r[15] -= 4u;
    state->r[14] = record;
    state->r[0] = (uint32_t)(int32_t)(int8_t)ram[at(record + 2u)];
    state->sr = (state->sr & ~1u) | (uint32_t)(state->r[0] == 1u);
    if (state->sr & 1u) {
        state->r[2] = read32(ram, record + 8u);
        state->sr = (state->sr & ~1u) | (uint32_t)(state->r[2] == 0u);
        if (!(state->sr & 1u)) {
            state->r[3] = clock;
            enter(ram, state, clock, 0x8c65bf7au, observe);
            sa_mmio_complement_sub_8c60260e(ram, state, context, read_register);

            state->r[3] = abort;
            write32(ram, state->r[15], state->r[0]);
            state->r[4] = read32(ram, state->r[14] + 8u);
            enter(ram, state, abort, 0x8c65bf82u, observe);
            const int abort_status = abort_request(ram, state, context);
            if (abort_status) return abort_status;

            state->r[2] = clock;
            enter(ram, state, clock, 0x8c65bf88u, observe);
            sa_mmio_complement_sub_8c60260e(ram, state, context, read_register);
            state->r[3] = subtract;
            state->r[5] = state->r[0];
            state->r[4] = read32(ram, state->r[15]);
            enter(ram, state, subtract, 0x8c65bf90u, observe);
            sa_subtract_arguments_8c602618(ram, state);
            state->r[2] = read32(ram, 0x8c65bffcu);
            write32(ram, state->r[2], state->r[0]);
            state->r[0] = 0u;
            ram[at(state->r[14] + 2u)] = 0u;
        }
    }
    state->r[0] = 1u;
    ram[at(state->r[14] + 1u)] = 1u;
    state->r[15] += 4u;
    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->r[14] = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->pc = state->pr;
    return 0;
}

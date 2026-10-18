#include <stdint.h>

#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif

struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
typedef void (*sa_callback_dispatch)(uint8_t *, struct sa_cpu_state *, uint32_t, void *);

static uint32_t ram_offset(uint32_t address) {
    return (address & 0x1fffffffu) - 0x0c000000u;
}
static uint32_t read32(const uint8_t *ram, uint32_t address) {
    uint32_t offset=ram_offset(address);
    return (uint32_t)ram[offset] | ((uint32_t)ram[offset+1]<<8) |
           ((uint32_t)ram[offset+2]<<16) | ((uint32_t)ram[offset+3]<<24);
}

SA_EXPORT void sa_callback_tail_8c64e0ac(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context) {
    state->r[2]=read32(ram,0x8c64e15cu);
    state->r[3]=read32(ram,0x8c64e134u);
    state->r[4]=read32(ram,state->r[3]);
    state->pc=state->r[2];
    dispatch(ram,state,state->r[2],context);
    state->pc=state->pr;
}

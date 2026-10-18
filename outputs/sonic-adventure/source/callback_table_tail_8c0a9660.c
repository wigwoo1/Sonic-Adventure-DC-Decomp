#include <stdint.h>

#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif

struct sa_cpu_state {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
};

typedef void (*sa_callback_dispatch)(uint8_t *, struct sa_cpu_state *,
                                     uint32_t, void *);

static uint32_t ram_offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write16(uint8_t *ram, uint32_t address, uint16_t value)
{
    uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
}

SA_EXPORT void sa_callback_table_tail_8c0a9660(
        uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{
    uint32_t target;

    state->r[3] = 1u;
    ram[ram_offset(state->r[4])] = 1u;
    state->r[0] = 0u;
    target = read32(ram, 0x8c0a9740u);
    state->r[3] = target;
    /* Opcode 0x8167 stores r0 through r6 in the JMP delay slot. */
    write16(ram, state->r[6] + 0x0eu, 0u);
    state->pc = target;
    dispatch(ram, state, target, context);
    /* This is a tail jump: the callback returns to the original caller. */
    state->pc = state->pr;
}

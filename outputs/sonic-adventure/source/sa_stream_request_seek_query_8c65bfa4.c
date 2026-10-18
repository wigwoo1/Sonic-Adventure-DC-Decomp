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

void sa_stream_request_position_update_8c600e7a(uint8_t *, struct sa_cpu_state *);
void sa_stream_request_query_8c600ec6(uint8_t *, struct sa_cpu_state *);

/* Optional read-only observer for comparing the two native call boundaries. */
typedef void (*sa_entry_observer)(const uint8_t *, const struct sa_cpu_state *);

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

/*
 * Retail 0x8c65bfa4: seek to incoming R5 using origin 0, then tail-query.
 * Host status 0 means both native calls completed; -1 rejects retargeted
 * literal slots before changing RAM or CPU state. The game result is in R0.
 */
SA_EXPORT int sa_stream_request_seek_query_8c65bfa4(
    uint8_t *ram, struct sa_cpu_state *state, sa_entry_observer observe)
{
    const uint32_t update = read32(ram, 0x8c65c000u);
    const uint32_t query = read32(ram, 0x8c65bfecu);
    if (update != 0x8c600e7au || query != 0x8c600ec6u)
        return -1;

    const uint32_t stack = state->r[15];
    write32(ram, stack - 4u, state->r[14]);
    state->r[6] = 0u;
    write32(ram, stack - 8u, state->pr);
    state->r[3] = update;
    state->r[14] = state->r[4];
    state->r[4] = read32(ram, state->r[14] + 8u);
    state->r[15] = stack - 8u;
    state->pr = 0x8c65bfb2u;
    state->pc = update;
    if (observe)
        observe(ram, state);
    sa_stream_request_position_update_8c600e7a(ram, state);

    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->r[2] = query;
    state->r[4] = read32(ram, state->r[14] + 8u);
    state->pc = query;
    /* JMP delay slot: restore R14 while popping its saved stack word. */
    state->r[14] = read32(ram, state->r[15]);
    state->r[15] += 4u;
    if (observe)
        observe(ram, state);
    sa_stream_request_query_8c600ec6(ram, state);
    return 0;
}

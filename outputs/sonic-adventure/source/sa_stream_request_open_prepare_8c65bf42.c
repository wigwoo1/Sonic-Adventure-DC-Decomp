#include <stdint.h>

#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif

#define STREAM_OPEN_ADDRESS 0x8c65bf42u
#define REQUEST_QUERY_ADDRESS 0x8c600ec6u

struct sa_cpu_state {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
};

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint16_t read16(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint16_t)((uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8));
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void write8(uint8_t *ram, uint32_t address, uint8_t value)
{
    ram[ram_offset(address)] = value;
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

/* Retail service at 0x8c600ec6: return a ready request's field at +0x14. */
static uint32_t request_query(uint8_t *ram, struct sa_cpu_state *state,
                              uint32_t request)
{
    state->r[4] = request;
    if (request == 0u) {
        state->sr |= 1u;
        state->r[0] = 0u;
    } else {
        const uint32_t status = (uint32_t)(int32_t)(int16_t)read16(ram, request + 0x4cu);
        state->r[3] = status;
        if (status == 0u) {
            state->sr |= 1u;
            state->r[0] = 0u;
        } else {
            state->sr &= ~1u;
            state->r[0] = read32(ram, request + 0x14u);
        }
    }
    return state->r[0];
}

SA_EXPORT void sa_stream_request_query_8c600ec6(
    uint8_t *ram, struct sa_cpu_state *state)
{
    (void)request_query(ram, state, state->r[4]);
    state->pc = state->pr;
}

/* Retail stream-open helper at 0x8c65bf42, including its indirect service call. */
SA_EXPORT void sa_stream_request_open_prepare_8c65bf42(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t object = state->r[4];
    const uint32_t callback = read32(ram, 0x8c65bfecu);
    const uint32_t request = read32(ram, object + 8u);
    const uint32_t stack = state->r[15];

    /* The SH-4 prologue stores R14 and PR; the epilogue restores them. */
    write32(ram, stack - 4u, state->r[14]);
    write32(ram, stack - 8u, state->pr);
    state->r[3] = 0u;
    write8(ram, object + 1u, 2u);
    write32(ram, object + 0x34u, 0u);
    state->r[2] = callback;
    state->pr = 0x8c65bf56u;
    (void)request_query(ram, state, request);
    state->pr = read32(ram, stack - 8u);
    write32(ram, object + 0x0cu, state->r[0]);
    state->r[0] = 1u;
    state->pc = state->pr;
}


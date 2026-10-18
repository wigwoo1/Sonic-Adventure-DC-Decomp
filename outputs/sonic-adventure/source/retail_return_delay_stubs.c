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

static uint32_t ram_offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static void return_unchanged(struct sa_cpu_state *state)
{
    state->pc = state->pr;
}

static void return_zero(struct sa_cpu_state *state)
{
    state->r[0] = 0;
    state->pc = state->pr;
}

static void return_store32(uint8_t *ram, struct sa_cpu_state *state)
{
    uint32_t at = ram_offset(state->r[4]);
    uint32_t value = state->r[5];
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
    state->pc = state->pr;
}

#define RETURN_NOOP(name) \
    SA_EXPORT void name(uint8_t *ram, struct sa_cpu_state *state) \
    { (void)ram; return_unchanged(state); }
#define RETURN_ZERO(name) \
    SA_EXPORT void name(uint8_t *ram, struct sa_cpu_state *state) \
    { (void)ram; return_zero(state); }
#define RETURN_STORE32(name) \
    SA_EXPORT void name(uint8_t *ram, struct sa_cpu_state *state) \
    { return_store32(ram, state); }

RETURN_ZERO(sa_return_zero_8c042dc4)
RETURN_ZERO(sa_return_zero_8c094dc4)
RETURN_ZERO(sa_return_zero_8c094dc8)
RETURN_ZERO(sa_return_zero_8c094dcc)
RETURN_ZERO(sa_return_zero_8c64b2dc)
RETURN_ZERO(sa_return_zero_8c65b510)
RETURN_ZERO(sa_return_zero_8c65da68)
RETURN_ZERO(sa_return_zero_0c90867c)

RETURN_NOOP(sa_return_noop_0c90c560)
RETURN_NOOP(sa_return_noop_0c90c564)
RETURN_NOOP(sa_return_noop_0c90c568)
RETURN_NOOP(sa_return_noop_0c90d210)
RETURN_NOOP(sa_return_noop_0c906986)
RETURN_NOOP(sa_return_noop_0c900b20)
RETURN_NOOP(sa_return_noop_0c903ce0)
RETURN_NOOP(sa_return_noop_0c9049c0)
RETURN_NOOP(sa_return_noop_0c906b8e)
RETURN_NOOP(sa_return_noop_0c90bdd4)

RETURN_STORE32(sa_return_store32_0c900d94)

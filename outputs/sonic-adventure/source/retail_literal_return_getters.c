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

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = address & 0x00ffffffu;
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void literal_return(uint8_t *ram, struct sa_cpu_state *state,
                           uint32_t literal_address)
{
    state->r[0] = read32(ram, literal_address);
    state->pc = state->pr;
}

SA_EXPORT void sa_literal_return_getter_8c0dff4a(uint8_t *ram,
                                                 struct sa_cpu_state *state)
{ literal_return(ram, state, 0x8c0dff78u); }

SA_EXPORT void sa_literal_return_getter_0c901564(uint8_t *ram,
                                                  struct sa_cpu_state *state)
{ literal_return(ram, state, 0x0c901604u); }

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

SA_EXPORT void sa_retail_byte_field_setter_8c6446d2(
        uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[0] = 0x6du;
    uint32_t address = state->r[4] + state->r[0];
    ram[address & 0x00ffffffu] = (uint8_t)state->r[5];
    state->pc = state->pr;
}

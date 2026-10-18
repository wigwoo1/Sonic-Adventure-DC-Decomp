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

static uint32_t ram_index(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_index(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static void clear_matching_main_slot(uint8_t *ram, uint32_t value)
{
    uint32_t output_table = read32(ram, 0x8c052b60u);
    uint32_t input_base = read32(ram, 0x8c052b64u);

    for (int32_t index = 255; index >= 0; --index) {
        uint32_t entry = input_base + (uint32_t)index * 0x34u;
        if (value == entry) {
            uint32_t at = ram_index(output_table + (uint32_t)index);
            ram[at] = 0;
            break;
        }
    }
}

SA_EXPORT void sa_callback_candidate_8c052a5e(uint8_t *ram, uint32_t value)
{
    clear_matching_main_slot(ram, value);
}

SA_EXPORT void sa_b_chaos4_callback_candidate_c90701a(
    uint8_t *ram, struct sa_cpu_state *state)
{
    uint32_t task = state->r[4];
    uint32_t value = read32(ram, task + 0x2cu);

    clear_matching_main_slot(ram, value);
    write32(ram, task + 0x2cu, 0);
    state->pc = state->pr;
}

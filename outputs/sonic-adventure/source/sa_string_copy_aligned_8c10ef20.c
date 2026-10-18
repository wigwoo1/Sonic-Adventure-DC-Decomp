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
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static uint8_t read8(const uint8_t *ram, uint32_t address)
{
    return ram[ram_offset(address)];
}

static void write8(uint8_t *ram, uint32_t address, uint8_t value)
{
    ram[ram_offset(address)] = value;
}

/* Retail entry 0x8c10ef20. The caller supplies aligned destination R0 and
 * source R1. Copy stops after the source NUL byte. */
SA_EXPORT void sa_string_copy_aligned_8c10ef20(
    uint8_t *ram,
    struct sa_cpu_state *state)
{
    const uint32_t destination = state->r[0];
    uint32_t source = state->r[1];
    const uint32_t original_sp = state->r[15];
    uint32_t output = destination;
    uint32_t input = source;
    uint32_t length = 0u;
    const uint32_t saved_r5_address = original_sp - 4u;
    const uint32_t saved_r6_address = original_sp - 8u;

    write32(ram, saved_r5_address, state->r[5]);
    write32(ram, saved_r6_address, state->r[6]);

    for (;;) {
        const uint32_t word = read32(ram, input);
        if ((word & 0xff000000u) == 0u || (word & 0x00ff0000u) == 0u ||
            (word & 0x0000ff00u) == 0u || (word & 0x000000ffu) == 0u) {
            uint8_t byte;
            do {
                byte = read8(ram, input++);
                write8(ram, output++, byte);
                ++length;
            } while (byte != 0u);
            break;
        }

        write32(ram, output, word);
        input += 4u;
        output += 4u;
        length += 4u;
    }

    source += length;
    state->r[0] = destination;
    state->r[1] = source;
    state->r[3] = 0u;
    state->r[4] = 0u;
    state->r[5] = read32(ram, saved_r5_address);
    state->r[6] = read32(ram, saved_r6_address);
    state->r[15] = original_sp + 8u;
    state->sr = (state->sr & ~1u) | ((length & 3u) != 0u);
    state->pc = state->pr;
}

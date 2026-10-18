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

/* Retail entry 0x8c10dacc: aligned pointers tail-jump to 0x8c10ef20;
 * otherwise the routine copies one byte at a time through the NUL byte. */
SA_EXPORT void sa_string_copy_8c10dacc(uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t destination = state->r[0];
    const uint32_t source_start = state->r[1];
    const uint32_t original_sp = state->r[15];
    uint32_t source = source_start;
    uint32_t destination_cursor = destination;
    uint32_t copied = 0u;
    const int aligned = ((source_start | destination) & 3u) == 0u;

    /* The wrapper spills R3/R4 on both paths. The aligned child also spills
     * R5/R6 before restoring them. These writes are visible in RAM. */
    write32(ram, original_sp - 4u, state->r[3]);
    write32(ram, original_sp - 8u, state->r[4]);
    if (aligned) {
        write32(ram, original_sp - 12u, state->r[5]);
        write32(ram, original_sp - 16u, state->r[6]);
    }

    if (aligned) {
        for (;;) {
            const uint32_t word = read32(ram, source);
            if ((word & 0xff000000u) == 0u || (word & 0x00ff0000u) == 0u ||
                (word & 0x0000ff00u) == 0u || (word & 0x000000ffu) == 0u) {
                uint8_t byte;
                do {
                    byte = read8(ram, source++);
                    write8(ram, destination_cursor++, byte);
                    ++copied;
                } while (byte != 0u);
                break;
            }
            write32(ram, destination_cursor, word);
            source += 4u;
            destination_cursor += 4u;
            copied += 4u;
        }
        state->sr = (state->sr & ~1u) | ((copied & 3u) != 0u);
    } else {
        uint8_t byte;
        do {
            byte = read8(ram, source++);
            write8(ram, destination_cursor++, byte);
            ++copied;
        } while (byte != 0u);
        state->sr |= 1u;
    }

    state->r[0] = destination;
    state->r[1] = source_start + copied;
    state->r[3] = read32(ram, original_sp - 4u);
    state->r[4] = read32(ram, original_sp - 8u);
    state->pc = state->pr;
}

#include <stdint.h>

#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif

struct sa_startup_state {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
};

struct sa_startup_fpu {
    uint32_t banks[2][16];
    uint32_t fpscr;
    uint32_t fpul;
};

static uint32_t offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint16_t read16(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint16_t)((uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8));
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write16(uint8_t *ram, uint32_t address, uint16_t value)
{
    uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static void copy_fmov(uint8_t *ram, struct sa_startup_fpu *fpu,
                      uint32_t context, uint32_t field_offset,
                      uint32_t destination, uint32_t encoded_register)
{
    uint32_t active = (fpu->fpscr >> 21) & 1u;
    uint32_t reg = encoded_register & 15u;
    if (fpu->fpscr & 0x00100000u) {
        uint32_t bank = active ^ (reg & 1u);
        uint32_t first = reg & ~1u;
        uint32_t low = read32(ram, context + field_offset);
        uint32_t high = read32(ram, context + field_offset + 4u);
        fpu->banks[bank][first] = low;
        fpu->banks[bank][first + 1u] = high;
        write32(ram, destination, low);
        write32(ram, destination + 4u, high);
    } else {
        uint32_t value = read32(ram, context + field_offset);
        fpu->banks[active][reg] = value;
        write32(ram, destination, value);
    }
}

SA_EXPORT void sa_startup_context_copy_8c0107a6(
        uint8_t *ram, struct sa_startup_state *state,
        struct sa_startup_fpu *fpu)
{
    uint32_t mode = read32(ram, 0x8c19b468u);
    state->r[3] = 0x8c19b468u;
    state->r[0] = mode;
    state->sr = (state->sr & ~1u) | (uint32_t)(mode == 1u);
    state->pc = state->pr;
    if (mode != 1u) {
        return;
    }

    state->r[2] = 0x8c19b488u;
    state->r[0] = 0x8c6ac58cu;
    write16(ram, state->r[0], read16(ram, 0x8c19b488u));
    state->r[4] = 0x8c749130u;
    uint32_t context = read32(ram, state->r[4]);

    state->r[3] = context;
    state->r[1] = read32(ram, context + 0x14u);
    state->r[3] = 0x8c6ac58eu;
    write16(ram, state->r[3], (uint16_t)state->r[1]);

    state->r[2] = context;
    state->r[1] = read32(ram, context + 0x18u);
    state->r[2] = 0x8c6ac590u;
    write16(ram, state->r[2], (uint16_t)state->r[1]);

    state->r[0] = context;
    state->r[1] = read32(ram, context + 0x1cu);
    state->r[0] = 0x8c6ac592u;
    write16(ram, state->r[0], (uint16_t)state->r[1]);

    state->r[3] = context;
    state->r[0] = 0x20u;
    state->r[1] = 0x8c6ac594u;
    copy_fmov(ram, fpu, context, 0x20u, state->r[1], 3u);

    state->r[3] = context;
    state->r[0] = 0x24u;
    state->r[3] = 0x8c6ac598u;
    copy_fmov(ram, fpu, context, 0x24u, state->r[3], 3u);

    state->r[2] = context;
    state->r[0] = 0x28u;
    state->r[2] = 0x8c6ac59cu;
    copy_fmov(ram, fpu, context, 0x28u, state->r[2], 3u);

    state->r[1] = context;
    state->r[0] = 0x2cu;
    state->r[1] = 0x8c6ac5a0u;
    copy_fmov(ram, fpu, context, 0x2cu, state->r[1], 3u);

    state->r[3] = context;
    state->r[0] = 0x30u;
    state->r[3] = 0x8c6ac5a4u;
    copy_fmov(ram, fpu, context, 0x30u, state->r[3], 3u);

    state->r[2] = context;
    state->r[0] = 0x34u;
    state->r[2] = 0x8c6ac5a8u;
    copy_fmov(ram, fpu, context, 0x34u, state->r[2], 3u);
}

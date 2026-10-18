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

struct sa_fpu_state {
    uint32_t fpscr;
    uint32_t banks[2][16];
};

static uint32_t ram_index(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_global_fpu_store_8c09e760(
    uint8_t *ram, struct sa_cpu_state *cpu, const struct sa_fpu_state *fpu)
{
    const uint32_t target = 0x8c66c314u;
    const uint32_t active_bank = (fpu->fpscr >> 21) & 1u;
    cpu->r[0] = target;
    if ((fpu->fpscr & 0x00100000u) != 0u) {
        const uint32_t source_bank = active_bank ^ (4u & 1u);
        write32(ram, target, fpu->banks[source_bank][4]);
        write32(ram, target + 4u, fpu->banks[source_bank][5]);
    } else {
        write32(ram, target, fpu->banks[active_bank][4]);
    }
    cpu->pc = cpu->pr;
}

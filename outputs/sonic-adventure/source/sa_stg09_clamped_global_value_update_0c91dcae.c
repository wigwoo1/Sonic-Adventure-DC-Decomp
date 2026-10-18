#include <stdint.h>
#include "sh4_compare_bits.h"

#define RAM_BASE 0x0c000000u
#define VALUE_POINTER_LITERAL 0x0c91dd10u
#define CHANGE_FLAG_POINTER_LITERAL 0x0c91dd18u

typedef struct {
    uint32_t banks[2][16];
    uint32_t fpscr;
    uint32_t fpul;
} sa_fpu;

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu;

static uint32_t offset(uint32_t address)
{
    return (address & 0x1fffffffu) - RAM_BASE;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static void compare_gt(sa_cpu *cpu, sa_fpu *fpu, uint32_t lhs, uint32_t rhs)
{
    uint32_t bank = (fpu->fpscr >> 21) & 1u;
    (void)sa_sh4_compare_bits(fpu->banks[bank][lhs], fpu->banks[bank][rhs],
                              &fpu->fpscr, &cpu->sr, 1);
}

static void compare_eq(sa_cpu *cpu, sa_fpu *fpu, uint32_t lhs, uint32_t rhs)
{
    uint32_t bank = (fpu->fpscr >> 21) & 1u;
    (void)sa_sh4_compare_bits(fpu->banks[bank][lhs], fpu->banks[bank][rhs],
                              &fpu->fpscr, &cpu->sr, 0);
}

static void fmov_register(sa_fpu *fpu, uint32_t destination, uint32_t source)
{
    uint32_t active = (fpu->fpscr >> 21) & 1u;
    if (fpu->fpscr & 0x00100000u) {
        uint32_t src_bank = active ^ (source & 1u);
        uint32_t dst_bank = active ^ (destination & 1u);
        uint32_t src_pair = source & ~1u;
        uint32_t dst_pair = destination & ~1u;
        uint32_t first = fpu->banks[src_bank][src_pair];
        uint32_t second = fpu->banks[src_bank][src_pair + 1u];
        fpu->banks[dst_bank][dst_pair] = first;
        fpu->banks[dst_bank][dst_pair + 1u] = second;
    } else {
        fpu->banks[active][destination] = fpu->banks[active][source];
    }
}

static void fmov_memory_load(const uint8_t *ram, sa_fpu *fpu,
                             uint32_t address, uint32_t destination)
{
    uint32_t active = (fpu->fpscr >> 21) & 1u;
    if (fpu->fpscr & 0x00100000u) {
        uint32_t bank = active ^ (destination & 1u);
        uint32_t pair = destination & ~1u;
        fpu->banks[bank][pair] = read32(ram, address);
        fpu->banks[bank][pair + 1u] = read32(ram, address + 4u);
    } else {
        fpu->banks[active][destination] = read32(ram, address);
    }
}

static void fmov_memory_store(uint8_t *ram, const sa_fpu *fpu,
                              uint32_t address, uint32_t source)
{
    uint32_t active = (fpu->fpscr >> 21) & 1u;
    if (fpu->fpscr & 0x00100000u) {
        uint32_t bank = active ^ (source & 1u);
        uint32_t pair = source & ~1u;
        write32(ram, address, fpu->banks[bank][pair]);
        write32(ram, address + 4u, fpu->banks[bank][pair + 1u]);
    } else {
        write32(ram, address, fpu->banks[active][source]);
    }
}

__declspec(dllexport)
void sa_stg09_clamped_global_value_update_0c91dcae(uint8_t *ram,
                                                  sa_cpu *cpu,
                                                  sa_fpu *fpu)
{
    uint32_t active = (fpu->fpscr >> 21) & 1u;
    uint32_t value_pointer;

    /* FLDI1 FR5; FCMP.GT FR4,FR5; BF/S with FLDI0 FR3 in the delay slot. */
    fpu->banks[active][5] = 0x3f800000u;
    compare_gt(cpu, fpu, 4u, 5u);
    fpu->banks[active][3] = 0;

    /* If the input exceeds one, copy one into FR4 (including the SZ bank form). */
    if (cpu->sr & 1u) {
        fmov_register(fpu, 4u, 5u);
    }

    /* Clamp negative values to zero; otherwise retain the current FR4 value. */
    compare_gt(cpu, fpu, 3u, 4u);
    if (cpu->sr & 1u) {
        fmov_register(fpu, 4u, 3u);
    }

    value_pointer = read32(ram, VALUE_POINTER_LITERAL);
    cpu->r[4] = value_pointer;
    fmov_memory_load(ram, fpu, value_pointer, 3u);

    compare_eq(cpu, fpu, 3u, 4u);
    if (!(cpu->sr & 1u)) {
        uint32_t flag_pointer = read32(ram, CHANGE_FLAG_POINTER_LITERAL);
        cpu->r[3] = flag_pointer;
        cpu->r[2] = 1u;
        write32(ram, flag_pointer, 1u);
        fmov_memory_store(ram, fpu, value_pointer, 4u);
    }

    cpu->pc = cpu->pr;
}

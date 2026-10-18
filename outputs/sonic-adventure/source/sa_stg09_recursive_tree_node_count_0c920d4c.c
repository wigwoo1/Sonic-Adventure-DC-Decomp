#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu;

#define RAM_BASE 0x0c000000u
#define ROUTINE_RETURN_PC 0x0c920d68u

static uint32_t offset(uint32_t address)
{
    return (address & 0x1fffffffu) - RAM_BASE;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static uint32_t pop32(const uint8_t *ram, sa_cpu *cpu)
{
    uint32_t value = read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    return value;
}

static void push32(uint8_t *ram, sa_cpu *cpu, uint32_t value)
{
    cpu->r[15] -= 4u;
    write32(ram, cpu->r[15], value);
}

static void count_subtree(uint8_t *ram, sa_cpu *cpu)
{
    uint32_t node = cpu->r[4];
    uint32_t flags;
    uint32_t original_r13 = cpu->r[13];
    uint32_t original_r14 = cpu->r[14];
    uint32_t return_pc = cpu->pr;

    push32(ram, cpu, original_r14);
    push32(ram, cpu, original_r13);
    push32(ram, cpu, return_pc);

    flags = read32(ram, node);
    cpu->r[0] = flags & 0x10u;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(cpu->r[0] == 0u);
    cpu->r[13] = 1u;
    cpu->r[14] = (flags & 0x10u) != 0u ? 0u : read32(ram, node + 0x2cu);
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(cpu->r[14] == 0u);

    while (cpu->r[14] != 0u) {
        uint32_t child = cpu->r[14];
        cpu->r[4] = child;
        cpu->pr = ROUTINE_RETURN_PC;
        count_subtree(ram, cpu);

        /* The callee restores R14 to its entry value; the caller follows the
         * sibling link in the return-delay/loop sequence and accumulates R0. */
        cpu->r[14] = read32(ram, cpu->r[14] + 0x30u);
        cpu->sr = (cpu->sr & ~1u) | (uint32_t)(cpu->r[14] == 0u);
        cpu->r[13] += cpu->r[0];
    }

    cpu->pr = pop32(ram, cpu);
    cpu->r[0] = cpu->r[13];
    cpu->r[13] = pop32(ram, cpu);
    cpu->r[14] = pop32(ram, cpu);
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_stg09_recursive_tree_node_count_0c920d4c(uint8_t *ram, sa_cpu *cpu)
{
    count_subtree(ram, cpu);
}

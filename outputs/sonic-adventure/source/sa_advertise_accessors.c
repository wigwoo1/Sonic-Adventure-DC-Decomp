#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu;

#define RAM_BASE 0x0c000000u

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

__declspec(dllexport)
void sa_advertise_clear_word_plus18_8c904020(uint8_t *ram, sa_cpu *cpu)
{
    cpu->r[3] = 0u;
    write32(ram, cpu->r[4] + 0x18u, cpu->r[3]);
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_advertise_get_signed_byte_pointer_plus2c_8c90c34e(uint8_t *ram, sa_cpu *cpu)
{
    cpu->r[4] = read32(ram, cpu->r[4] + 0x2cu);
    /* MOV.B sign-extends; EXTU.B in the RTS delay slot clears the upper bits. */
    cpu->r[0] = ram[offset(cpu->r[4])];
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_advertise_clear_three_fields_8c90e3c0(uint8_t *ram, sa_cpu *cpu)
{
    cpu->r[0] = 0x34u;
    cpu->r[5] = 0u;
    ram[offset(cpu->r[4] + cpu->r[0])] = (uint8_t)cpu->r[5];
    cpu->r[0] = 0x29u;
    write32(ram, cpu->r[4] + 0x18u, cpu->r[5]);
    ram[offset(cpu->r[4] + cpu->r[0])] = (uint8_t)cpu->r[5];
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_advertise_global_nonzero_flag_8c9031c0(uint8_t *ram, sa_cpu *cpu)
{
    uint32_t value;
    cpu->r[3] = read32(ram, 0x8c9032a0u);
    value = ram[offset(cpu->r[3])];
    cpu->r[0] = value;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(value == 0u);
    cpu->r[0] = (cpu->sr & 1u) ^ 1u;
    cpu->pc = cpu->pr;
}

static void set_nested_word(uint8_t *ram, sa_cpu *cpu, uint32_t literal,
                            uint32_t field_offset, uint32_t value)
{
    cpu->r[2] = read32(ram, literal);
    cpu->r[0] = value;
    cpu->r[3] = read32(ram, cpu->r[2]);
    cpu->r[1] = read32(ram, cpu->r[3] + 0x2cu);
    write32(ram, cpu->r[1] + field_offset, cpu->r[0]);
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_advertise_set_nested_word_eight_8c905572(uint8_t *ram, sa_cpu *cpu)
{
    set_nested_word(ram, cpu, 0x8c905588u, 8u, 8u);
}

__declspec(dllexport)
void sa_advertise_set_nested_word_seven_8c909532(uint8_t *ram, sa_cpu *cpu)
{
    set_nested_word(ram, cpu, 0x8c909548u, 8u, 7u);
}

__declspec(dllexport)
void sa_advertise_set_nested_word_six_8c909d36(uint8_t *ram, sa_cpu *cpu)
{
    set_nested_word(ram, cpu, 0x8c909d78u, 4u, 6u);
}

__declspec(dllexport)
void sa_advertise_set_nested_word_eight_8c90bbda(uint8_t *ram, sa_cpu *cpu)
{
    set_nested_word(ram, cpu, 0x8c90bc24u, 4u, 8u);
}

__declspec(dllexport)
void sa_advertise_set_nested_input_word_8c903f12(uint8_t *ram, sa_cpu *cpu)
{
    cpu->r[2] = read32(ram, 0x8c903f28u);
    cpu->r[3] = read32(ram, cpu->r[2]);
    cpu->r[1] = read32(ram, cpu->r[3] + 0x2cu);
    write32(ram, cpu->r[1] + 8u, cpu->r[4]);
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_advertise_set_nested_input_byte_8c90314c(uint8_t *ram, sa_cpu *cpu)
{
    cpu->r[0] = read32(ram, 0x8c903194u);
    cpu->r[1] = read32(ram, cpu->r[0]);
    cpu->r[0] = 0x34u;
    cpu->r[3] = read32(ram, cpu->r[1] + 0x2cu);
    ram[offset(cpu->r[3] + cpu->r[0])] = (uint8_t)cpu->r[4];
    cpu->pc = cpu->pr;
}

static void get_global_flag_mask(uint8_t *ram, sa_cpu *cpu, uint32_t first_mask,
                                 uint32_t second_mask)
{
    cpu->r[4] = read32(ram, 0x8c900da0u);
    cpu->r[3] = first_mask;
    cpu->r[0] = second_mask;
    cpu->r[4] = read32(ram, cpu->r[4]);
    cpu->r[3] &= cpu->r[4];
    cpu->r[0] &= cpu->r[4];
    cpu->r[0] |= cpu->r[3];
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_advertise_get_flags_mask_0c_8c900d60(uint8_t *ram, sa_cpu *cpu)
{
    get_global_flag_mask(ram, cpu, 8u, 4u);
}

__declspec(dllexport)
void sa_advertise_get_flags_mask_402_8c900d70(uint8_t *ram, sa_cpu *cpu)
{
    get_global_flag_mask(ram, cpu, 0x400u, 2u);
}

__declspec(dllexport)
void sa_advertise_clear_literal_pair_8c900f20(uint8_t *ram, sa_cpu *cpu)
{
    cpu->r[5] = read32(ram, 0x8c901060u);
    cpu->r[4] = 0u;
    cpu->r[0] = cpu->r[4];
    ram[offset(cpu->r[5])] = (uint8_t)cpu->r[4];
    ram[offset(cpu->r[5] + 1u)] = (uint8_t)cpu->r[0];
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_advertise_sync_global_byte_to_object_8c90327e(uint8_t *ram, sa_cpu *cpu)
{
    cpu->r[5] = read32(ram, 0x8c9032a4u);
    cpu->r[0] = (uint32_t)(int32_t)(int8_t)ram[offset(cpu->r[5] + 0x0du)];
    cpu->r[0] = (uint8_t)cpu->r[0];
    write32(ram, cpu->r[4] + 0x20u, cpu->r[0]);
    ram[offset(cpu->r[5] + 0x0bu)] = (uint8_t)cpu->r[0];
    cpu->r[0] = read32(ram, cpu->r[4] + 0x20u);
    ram[offset(cpu->r[5] + 0x0du)] = (uint8_t)cpu->r[0];
    cpu->pc = cpu->pr;
}

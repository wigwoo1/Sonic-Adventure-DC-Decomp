#include <stdint.h>

typedef struct sa_cpu_state {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

void sa_record_callback_mode_select_8c647de8(uint8_t *, sa_cpu_state *);
void sa_record_mode_value_store_8c647f10(uint8_t *, sa_cpu_state *);
void sa_record_buffer_descriptors_build_8c647f4c(uint8_t *, sa_cpu_state *);
void sa_global_setter_8c647534(uint8_t *, sa_cpu_state *);
void sa_retail_constant_setter_8c660898(uint8_t *, sa_cpu_state *);

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static void fill(uint8_t *ram, uint32_t address, uint8_t value, uint32_t count)
{
    for (uint32_t i = 0; i < count; ++i) ram[ram_offset(address + i)] = value;
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_record_setup_initialize_8c647e16(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t input_mode = cpu->r[4];
    const uint32_t input_mask = cpu->r[5];
    const uint32_t input_buffer = cpu->r[6];
    const uint32_t initial_sp = cpu->r[15];
    const uint32_t initial_pr = cpu->pr;
    const uint32_t saved_r9 = cpu->r[9];
    const uint32_t saved_r10 = cpu->r[10];
    const uint32_t saved_r11 = cpu->r[11];
    const uint32_t saved_r12 = cpu->r[12];
    const uint32_t saved_r13 = cpu->r[13];
    const uint32_t saved_r14 = cpu->r[14];
    const uint32_t global_cell = read32(ram, 0x8c647e9cu);
    const uint32_t new_base = read32(ram, 0x8c647eb0u);
    const uint32_t zero_buffer = read32(ram, 0x8c647eb4u);
    const uint32_t byte_fill_target = read32(ram, 0x8c647eb8u);
    const uint32_t object_table = read32(ram, 0x8c647ebcu);
    const uint32_t selected_count = read32(ram, 0x8c647ec0u);
    const uint32_t fill_count = read32(ram, 0x8c647e8cu);
    const uint32_t final_count = read32(ram, 0x8c647fb4u);
    const uint32_t final_zero = read32(ram, 0x8c647fb8u);
    const uint32_t final_buffer = read32(ram, 0x8c647fc0u);

    for (uint32_t reg = 14u; reg >= 9u; --reg) {
        write32(ram, initial_sp - (15u - reg) * 4u, cpu->r[reg]);
    }
    write32(ram, initial_sp - 28u, initial_pr);
    write32(ram, initial_sp - 32u, input_mode);
    cpu->r[15] = initial_sp - 32u;

    for (uint32_t i = 0; i < 8u; ++i) write32(ram, zero_buffer + i * 4u, 0u);
    write32(ram, global_cell, new_base);
    fill(ram, new_base, 0u, 0x14u);
    fill(ram, object_table, 0u, fill_count);

    cpu->r[4] = 0u;
    cpu->pr = 0x8c647e5au;
    sa_record_callback_mode_select_8c647de8(ram, cpu);
    write32(ram, selected_count, 0u);
    uint32_t bits = input_mask;
    for (uint32_t i = 0; i < 8u; ++i) {
        if ((bits & 1u) != 0u) {
            write32(ram, selected_count, read32(ram, selected_count) + 1u);
        }
        bits >>= 1;
    }

    if (input_buffer == 0u) {
        cpu->r[4] = input_mode;
        cpu->r[5] = input_mask;
        cpu->r[6] = 0u;
        cpu->pr = 0x8c647e7eu;
        sa_record_mode_value_store_8c647f10(ram, cpu);
        write32(ram, new_base + 8u, 1u);
    } else {
        cpu->r[0] = 0x8c660816u;
        cpu->r[1] = 2u;
        cpu->r[2] = 0u;
        cpu->r[3] = input_mask;
        cpu->r[4] = input_mode;
        cpu->r[5] = input_mask;
        cpu->r[6] = input_buffer;
        cpu->r[7] = fill_count;
        cpu->r[9] = global_cell;
        cpu->r[10] = input_buffer;
        cpu->r[11] = byte_fill_target;
        cpu->r[12] = input_mask;
        cpu->r[13] = 8u;
        cpu->r[14] = 0u;
        cpu->pr = 0x8c647e86u;
        sa_record_buffer_descriptors_build_8c647f4c(ram, cpu);
        write32(ram, new_base + 8u, 0u);
    }

    write32(ram, final_count, 8u);
    write32(ram, final_zero, 0u);
    cpu->r[4] = 10u;
    cpu->pr = 0x8c647eacu;
    sa_global_setter_8c647534(ram, cpu);
    const uint32_t bytes = read32(ram, final_count) * 0x48u;
    fill(ram, final_buffer, 0u, bytes);
    cpu->pr = 0x8c647eb6u;
    sa_retail_constant_setter_8c660898(ram, cpu);

    cpu->r[0] = 0u;
    cpu->r[1] = 0x8c647534u;
    cpu->r[2] = 0x8c67c27cu;
    cpu->r[3] = 0u;
    cpu->r[4] = final_buffer;
    cpu->r[5] = 0u;
    cpu->r[6] = bytes;
    cpu->r[7] = bytes;
    cpu->r[9] = saved_r9;
    cpu->r[10] = saved_r10;
    cpu->r[11] = saved_r11;
    cpu->r[12] = saved_r12;
    cpu->r[13] = saved_r13;
    cpu->r[14] = saved_r14;
    cpu->sr = (cpu->sr & ~1u) | 1u;
    cpu->pr = initial_pr;
    cpu->r[15] = initial_sp;
    cpu->pc = initial_pr;
}

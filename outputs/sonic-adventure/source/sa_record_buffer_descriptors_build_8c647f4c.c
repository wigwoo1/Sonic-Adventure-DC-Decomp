#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

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

static uint16_t read16(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_offset(address);
    return (uint16_t)((uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8));
}

static void write8(uint8_t *ram, uint32_t address, uint8_t value)
{
    ram[ram_offset(address)] = value;
}

static void write16(uint8_t *ram, uint32_t address, uint16_t value)
{
    const uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
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
    for (uint32_t i = 0; i < count; ++i) write8(ram, address + i, value);
}

static void save_frame(uint8_t *ram, const sa_cpu_state *cpu, uint32_t input_mask)
{
    const uint32_t sp = cpu->r[15];
    for (uint32_t reg = 14u; reg >= 8u; --reg) write32(ram, sp - (15u - reg) * 4u, cpu->r[reg]);
    write32(ram, sp - 32u, cpu->pr);
    write32(ram, sp - 48u, input_mask);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_record_buffer_descriptors_build_8c647f4c(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t input_mode = cpu->r[4];
    const uint32_t input_mask = cpu->r[5];
    const uint32_t initial_buffer = cpu->r[6];
    const uint32_t initial_sp = cpu->r[15];
    const uint32_t global_cell = read32(ram, 0x8c647fc8u);
    const uint32_t object = read32(ram, global_cell);
    const uint32_t sizes = read32(ram, 0x8c647fd0u);
    const uint32_t limits = read32(ram, 0x8c647fd4u);
    const uint32_t descriptor_base = read32(ram, 0x8c647fccu);
    const uint32_t stride = read32(ram, 0x8c6480dcu);
    const uint32_t address_base = read32(ram, 0x8c6480e4u);
    uint32_t mode_index;

    save_frame(ram, cpu, input_mask);
    cpu->r[15] = initial_sp - 52u;
    if (input_mode == 0x0fu) mode_index = 0u;
    else if (input_mode == 0x21u) mode_index = 1u;
    else if (input_mode == 0x41u) mode_index = 2u;
    else if (input_mode == 0x7au) mode_index = 3u;
    else {
        cpu->r[0] = 0xffffff00u;
        cpu->r[3] = global_cell;
        cpu->r[5] = limits;
        cpu->r[7] = object;
        cpu->sr &= ~1u;
        cpu->r[15] = initial_sp;
        cpu->pc = cpu->pr;
        return;
    }

    const uint32_t mode_size = read32(ram, sizes + mode_index * 4u);
    const uint16_t mode_limit = (uint16_t)read32(ram, limits + mode_index * 4u);
    const uint32_t row_bytes = (mode_size + 1u) * 0x200u;
    const uint32_t step = input_mode * 0x200u + 0x20u;
    const uint32_t loop_bound = 0x100u;
    const uint32_t buffer_offset = read16(ram, 0x8c6480d8u);
    const uint32_t clear_count = read16(ram, 0x8c6480dau);
    const uint32_t descriptor_stride = stride;

    write32(ram, object + 4u, mode_index);
    write32(ram, initial_sp - 52u, loop_bound);
    write32(ram, initial_sp - 44u, step);
    write32(ram, initial_sp - 40u, row_bytes);
    write32(ram, initial_sp - 36u, mode_limit);

    uint32_t active_count = 0;
    uint32_t buffer = initial_buffer;
    for (uint32_t index = 0; index < 8u; ++index) {
        if ((input_mask & (1u << index)) != 0u) {
            const uint32_t record = descriptor_base + index * descriptor_stride;
            const uint32_t buffer20 = buffer + 0x20u;
            const uint32_t buffer220 = buffer + buffer_offset;
            const uint32_t buffer_row = buffer + row_bytes + 0x20u;
            write8(ram, record + 2u, 1u);
            write16(ram, record, (uint16_t)index);
            write32(ram, record + 0x1cu, address_base + index * 0x4cu);
            write8(ram, record + 10u, 0u);
            write8(ram, record + 0x0bu, 0u);
            write8(ram, record + 12u, 0u);
            write32(ram, record + 0x2cu, buffer);
            write32(ram, record + 0x20u, buffer20);
            write32(ram, record + 0x24u, buffer220);
            write32(ram, record + 0x28u, buffer_row);
            write8(ram, record + 8u, 0u);
            write8(ram, record + 9u, 0u);
            write16(ram, record + 0x0eu, 1u);
            write16(ram, record + 0x10u, (uint16_t)mode_size);
            write16(ram, record + 0x12u, mode_limit);
            write16(ram, record + 0x14u, 0x20u);

            fill(ram, record + 0x38u, 0u, 0x10u);
            fill(ram, record + 0x48u, 0u, clear_count);
            fill(ram, buffer, 0x47u, 0x20u);
            fill(ram, buffer20, 0x53u, 0x20u);
            fill(ram, buffer220, 0x46u, 0x20u);
            fill(ram, buffer_row, 0x44u, 0x20u);
            buffer += step;
            ++active_count;
        }
    }

    cpu->r[0] = 0u;
    cpu->r[2] = descriptor_stride;
    cpu->r[3] = 8u;
    if (active_count != 0u) {
        const uint32_t last_buffer = initial_buffer + (active_count - 1u) * step;
        cpu->r[1] = last_buffer + buffer_offset;
        cpu->r[4] = last_buffer + row_bytes + 0x20u;
        cpu->r[5] = 0x44u;
        cpu->r[6] = 0x20u;
        cpu->r[7] = 0x20u;
    } else {
        cpu->r[4] = step;
        if (mode_index >= 2u) cpu->r[1] = mode_limit;
        cpu->r[5] = limits;
        cpu->r[6] = initial_buffer;
        cpu->r[7] = object;
    }
    cpu->sr |= 1u;
    cpu->r[15] = initial_sp;
    cpu->pc = cpu->pr;
}

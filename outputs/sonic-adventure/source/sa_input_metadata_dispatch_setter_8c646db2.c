#include <stdint.h>

struct sa_cpu_state {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
};

extern void sa_indexed_field_setter_8c65f462(uint8_t *, struct sa_cpu_state *);
extern void sa_indexed_field_setter_8c65f476(uint8_t *, struct sa_cpu_state *);
extern void sa_indexed_field_setter_8c65f4a2(uint8_t *, struct sa_cpu_state *);
extern void sa_indexed_field_setter_8c65f4b6(uint8_t *, struct sa_cpu_state *);
extern void sa_indexed_field_setter_8c65f4ee(uint8_t *, struct sa_cpu_state *);
extern void sa_indexed_field_setter_8c65f502(uint8_t *, struct sa_cpu_state *);
extern void sa_indexed_field_setter_8c65f53c(uint8_t *, struct sa_cpu_state *);
extern void sa_input_device_record_mode_set_8c65f42e(uint8_t *, struct sa_cpu_state *);
extern void sa_input_device_record_flag_set_8c65f44a(uint8_t *, struct sa_cpu_state *);
extern void sa_input_device_record_set_pair_8c65f4ca(uint8_t *, struct sa_cpu_state *);

static uint32_t ram_offset(uint32_t address)
{
    return address & 0x00ffffffu;
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

static void restore_dispatch_frame(const uint8_t *ram, struct sa_cpu_state *cpu)
{
    cpu->pr = read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->r[14] = read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->pc = cpu->pr;
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_input_metadata_dispatch_setter_8c646db2(uint8_t *ram,
                                                 struct sa_cpu_state *cpu)
{
    static const uint32_t targets[10] = {
        0x8c65f4cau, 0x8c65f462u, 0x8c65f42eu, 0x8c65f476u,
        0x8c65f4b6u, 0x8c65f44au, 0x8c65f4a2u, 0x8c65f4eeu,
        0x8c65f502u, 0x8c65f53cu,
    };
    static const uint32_t return_addresses[10] = {
        0x8c646e28u, 0x8c646decu, 0x8c646df6u, 0x8c646e0au,
        0x8c646e00u, 0x8c646e1eu, 0x8c646e14u, 0x8c646e32u,
        0x8c646e3cu, 0x8c646e46u,
    };
    const uint32_t initial_sp = cpu->r[15];
    const uint32_t selector = cpu->r[4];

    cpu->r[15] = initial_sp - 4u;
    write32(ram, cpu->r[15], cpu->r[14]);
    cpu->r[15] = initial_sp - 8u;
    write32(ram, cpu->r[15], cpu->pr);
    cpu->r[0] = selector;
    cpu->r[14] = cpu->r[5];

    if (selector > 9u) {
        cpu->sr &= ~1u;
        cpu->r[0] = 0xffffffffu;
        restore_dispatch_frame(ram, cpu);
        return;
    }

    cpu->sr |= 1u;
    cpu->r[3] = targets[selector];
    cpu->r[4] = cpu->r[14];
    cpu->pr = return_addresses[selector];
    cpu->pc = targets[selector];
    switch (selector) {
    case 0u: sa_input_device_record_set_pair_8c65f4ca(ram, cpu); break;
    case 1u: sa_indexed_field_setter_8c65f462(ram, cpu); break;
    case 2u: sa_input_device_record_mode_set_8c65f42e(ram, cpu); break;
    case 3u: sa_indexed_field_setter_8c65f476(ram, cpu); break;
    case 4u: sa_indexed_field_setter_8c65f4b6(ram, cpu); break;
    case 5u: sa_input_device_record_flag_set_8c65f44a(ram, cpu); break;
    case 6u: sa_indexed_field_setter_8c65f4a2(ram, cpu); break;
    case 7u: sa_indexed_field_setter_8c65f4ee(ram, cpu); break;
    case 8u: sa_indexed_field_setter_8c65f502(ram, cpu); break;
    default: sa_indexed_field_setter_8c65f53c(ram, cpu); break;
    }

    cpu->r[0] = 0u;
    restore_dispatch_frame(ram, cpu);
}

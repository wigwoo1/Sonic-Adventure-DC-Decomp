#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

static uint32_t ram_offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint16_t read16(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint16_t)((uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8));
}

static void write8(uint8_t *ram, uint32_t address, uint8_t value)
{
    ram[ram_offset(address)] = value;
}

__declspec(dllexport)
void sa_update_object_lifecycle_state_8c015ee0(uint8_t *ram, sa_cpu_state *cpu)
{
    uint32_t object = cpu->r[5];
    int32_t state = (int8_t)ram[ram_offset(object)];

    cpu->r[0] = (uint32_t)state;
    if (state != 0) {
        cpu->sr = (cpu->sr & ~1u) | (uint32_t)(state == 1);
        cpu->pc = cpu->pr;
        return;
    }

    {
        uint16_t flags = read16(ram, object + 6u);
        cpu->r[0] = flags;
        cpu->r[3] = 0x100u;

        if ((flags & 0x100u) == 0u) {
            int32_t timer = (int16_t)read16(ram, object + 12u);
            cpu->r[0] = (uint32_t)timer;
            cpu->r[2] = 600u;
            if (timer > 600) {
                write8(ram, object, 1u);
                cpu->r[0] = 1u;
                cpu->sr |= 1u;
            } else {
                cpu->sr &= ~1u;
            }
        } else if ((flags & 0x40u) == 0u) {
            write8(ram, object, 2u);
            cpu->r[1] = 2u;
            cpu->sr |= 1u;
        } else {
            cpu->sr &= ~1u;
        }
    }

    cpu->pc = cpu->pr;
}

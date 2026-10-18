#include <stdint.h>

struct sa_cpu_state {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
};

extern void sa_input_device_record_get_8c65f458(uint8_t *, struct sa_cpu_state *);
extern void sa_input_device_record_get_8c65f46c(uint8_t *, struct sa_cpu_state *);
extern void sa_input_device_record_get_8c65f480(uint8_t *, struct sa_cpu_state *);
extern void sa_input_device_record_get_8c65f4ac(uint8_t *, struct sa_cpu_state *);
extern void sa_input_device_record_get_8c65f4c0(uint8_t *, struct sa_cpu_state *);
extern void sa_input_device_record_get_8c65f4da(uint8_t *, struct sa_cpu_state *);
extern void sa_input_device_record_get_8c65f4f8(uint8_t *, struct sa_cpu_state *);
extern void sa_input_device_record_get_8c65f50c(uint8_t *, struct sa_cpu_state *);
extern void sa_input_device_record_get_8c65f546(uint8_t *, struct sa_cpu_state *);
extern void sa_input_device_record_mode_get_8c65f43a(uint8_t *, struct sa_cpu_state *);

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_input_metadata_dispatch_getter_8c646e5a(uint8_t *ram,
                                                 struct sa_cpu_state *cpu)
{
    static const uint32_t targets[10] = {
        0x8c65f4dau, 0x8c65f46cu, 0x8c65f43au, 0x8c65f480u,
        0x8c65f4c0u, 0x8c65f458u, 0x8c65f4acu, 0x8c65f4f8u,
        0x8c65f50cu, 0x8c65f546u,
    };
    const uint32_t selector = cpu->r[4];
    cpu->r[0] = selector;

    if (selector > 9u) {
        cpu->sr &= ~1u;
        cpu->r[0] = 0xffffffffu;
        cpu->pc = cpu->pr;
        return;
    }

    cpu->sr |= 1u;
    if (selector == 2u || selector == 3u || selector == 5u ||
        selector == 7u || selector == 9u) {
        cpu->r[2] = targets[selector];
    } else {
        cpu->r[3] = targets[selector];
    }
    cpu->pc = targets[selector];
    switch (selector) {
    case 0u: sa_input_device_record_get_8c65f4da(ram, cpu); break;
    case 1u: sa_input_device_record_get_8c65f46c(ram, cpu); break;
    case 2u: sa_input_device_record_mode_get_8c65f43a(ram, cpu); break;
    case 3u: sa_input_device_record_get_8c65f480(ram, cpu); break;
    case 4u: sa_input_device_record_get_8c65f4c0(ram, cpu); break;
    case 5u: sa_input_device_record_get_8c65f458(ram, cpu); break;
    case 6u: sa_input_device_record_get_8c65f4ac(ram, cpu); break;
    case 7u: sa_input_device_record_get_8c65f4f8(ram, cpu); break;
    case 8u: sa_input_device_record_get_8c65f50c(ram, cpu); break;
    default: sa_input_device_record_get_8c65f546(ram, cpu); break;
    }
}

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

static uint32_t index_of(uint32_t address) {
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(uint8_t *ram, uint32_t address) {
    uint32_t offset = index_of(address);
    return (uint32_t)ram[offset] | ((uint32_t)ram[offset + 1] << 8) |
           ((uint32_t)ram[offset + 2] << 16) | ((uint32_t)ram[offset + 3] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value) {
    uint32_t offset = index_of(address);
    ram[offset] = (uint8_t)value;
    ram[offset + 1] = (uint8_t)(value >> 8);
    ram[offset + 2] = (uint8_t)(value >> 16);
    ram[offset + 3] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_bucket_list_merge_8c02d00e(uint8_t *ram, struct sa_cpu_state *state) {
    const uint32_t table = 0x8c02d560u;
    const uint32_t work = state->r[4];
    uint32_t tail = 0;
    for (uint32_t bucket = 0; bucket < 128; ++bucket) {
        uint32_t entry = table + bucket * 8u;
        uint32_t head = read32(ram, entry);
        if (head == 0) continue;
        write32(ram, work + 0x10u, head);
        tail = read32(ram, entry + 4u);
        for (uint32_t following = bucket + 1u; following < 128; ++following) {
            uint32_t next_entry = table + following * 8u;
            uint32_t next_head = read32(ram, next_entry);
            if (next_head != 0) {
                write32(ram, tail, next_head);
                write32(ram, next_head + 4u, tail);
                tail = read32(ram, next_entry + 4u);
            }
        }
        break;
    }
    write32(ram, work + 0x14u, tail);
    state->r[0] = read32(ram, table + 127u * 8u);
    state->r[1] = table + 128u * 8u;
    state->r[5] = tail;
    state->r[7] = 0;
    state->sr |= 1u;
    state->pc = state->pr;
}

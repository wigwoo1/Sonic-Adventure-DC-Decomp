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

static uint32_t ram_index(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_index(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static void copy_field(uint8_t *ram, struct sa_cpu_state *state,
                       uint32_t stride, uint32_t source_offset,
                       uint32_t destination_offset, uint32_t object_register)
{
    uint32_t count = state->r[5];
    while (count != 0u) {
        uint32_t record = state->r[4];
        uint32_t object = read32(ram, record);
        uint32_t value = read32(ram, record + source_offset);
        write32(ram, object + destination_offset, value);
        state->r[object_register] = object;
        state->r[3] = value;
        state->r[4] = record + stride;
        --count;
    }
    state->r[5] = UINT32_MAX;
    state->sr |= 1u;
    state->pc = state->pr;
}

#define SA_HELPER(name, stride, source, destination, object_reg) \
    SA_EXPORT void sa_##name(uint8_t *ram, struct sa_cpu_state *state) \
    { \
        copy_field(ram, state, stride, source, destination, object_reg); \
    }

SA_HELPER(stg09_copy_field14_stride20_source12_object_r2_0c90bcba,
          20u, 12u, 20u, 2u)
SA_HELPER(stg09_copy_field14_stride20_source16_object_r1_0c90bcd0,
          20u, 16u, 20u, 1u)
SA_HELPER(stg09_copy_field14_stride16_source12_object_r1_0c90bdca,
          16u, 12u, 20u, 1u)
SA_HELPER(stg09_copy_field0c_stride12_source04_object_r2_0c90bf88,
          12u, 4u, 12u, 2u)
SA_HELPER(stg09_copy_field0c_stride12_source08_object_r1_0c90bf9e,
          12u, 8u, 12u, 1u)

SA_EXPORT void sa_stg09_copy_field14_stride20_source12_object_r2_0c90bcae(
    uint8_t *ram, struct sa_cpu_state *state)
{ sa_stg09_copy_field14_stride20_source12_object_r2_0c90bcba(ram, state); }

SA_EXPORT void sa_stg09_copy_field14_stride20_source16_object_r1_0c90bcc4(
    uint8_t *ram, struct sa_cpu_state *state)
{ sa_stg09_copy_field14_stride20_source16_object_r1_0c90bcd0(ram, state); }

SA_EXPORT void sa_stg09_copy_field14_stride16_source12_object_r1_0c90bdbe(
    uint8_t *ram, struct sa_cpu_state *state)
{ sa_stg09_copy_field14_stride16_source12_object_r1_0c90bdca(ram, state); }

SA_EXPORT void sa_stg09_copy_field0c_stride12_source04_object_r2_0c90bf4c(
    uint8_t *ram, struct sa_cpu_state *state)
{ sa_stg09_copy_field0c_stride12_source04_object_r2_0c90bf88(ram, state); }

SA_EXPORT void sa_stg09_copy_field0c_stride12_source08_object_r1_0c90bf92(
    uint8_t *ram, struct sa_cpu_state *state)
{ sa_stg09_copy_field0c_stride12_source08_object_r1_0c90bf9e(ram, state); }

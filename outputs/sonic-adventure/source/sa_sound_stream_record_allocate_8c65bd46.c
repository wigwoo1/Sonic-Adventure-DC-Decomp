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

typedef void (*sa_object_query)(uint8_t *, struct sa_cpu_state *, uint32_t, uint32_t);

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint8_t read8(const uint8_t *ram, uint32_t address)
{
    return ram[ram_offset(address)];
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void write8(uint8_t *ram, uint32_t address, uint8_t value)
{
    ram[ram_offset(address)] = value;
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static uint32_t rounded_stream_position(uint32_t position, uint32_t bias)
{
    const uint32_t signed_position = position + bias;
    uint32_t rounded = signed_position;
    if ((rounded & 0x80000000u) != 0u)
        rounded += bias;
    if ((rounded & 0x80000000u) != 0u)
        rounded = (rounded >> 11) | 0xffe00000u;
    else
        rounded >>= 11;
    return rounded;
}

/*
 * Retail allocator at 0x8c65bd46. The vtable's +0x24 method is supplied by
 * the caller so a native runtime can provide the matching object operation.
 */
SA_EXPORT void sa_sound_stream_record_allocate_8c65bd46(
    uint8_t *ram, struct sa_cpu_state *state, sa_object_query object_query)
{
    const uint32_t original_pr = state->pr;
    const uint32_t original_stack = state->r[15];
    const uint32_t param1 = state->r[4];
    const uint32_t param2 = state->r[5];
    const uint32_t param3 = state->r[6];
    const uint32_t object = state->r[7];
    const uint32_t table = read32(ram, 0x8c65be44u);
    const uint32_t bias = read32(ram, 0x8c65be40u);
    const uint32_t table_limit = 32u;

    /* Retail prologue stores R14, R13, R11, R10, then PR. */
    write32(ram, original_stack - 4u, state->r[14]);
    write32(ram, original_stack - 8u, state->r[13]);
    write32(ram, original_stack - 12u, state->r[11]);
    write32(ram, original_stack - 16u, state->r[10]);
    write32(ram, original_stack - 20u, original_pr);

    uint32_t index = 0u;
    uint32_t record = table;
    while (index < table_limit && read8(ram, record) != 0u) {
        state->r[3] = record;
        ++index;
        state->r[7] = index;
        record += 100u;
    }

    state->r[7] = index;
    if (index == table_limit) {
        state->r[0] = 0u;
        state->r[2] = (uint32_t)(int32_t)(int8_t)read8(
            ram, table + (table_limit - 1u) * 100u);
        state->r[3] = table + (table_limit - 1u) * 100u;
        state->sr |= 1u;
    } else {
        const uint32_t rounded = rounded_stream_position(param3, bias);

        write8(ram, record, 1u);
        write8(ram, record + 1u, 1u);
        write8(ram, record + 2u, 0u);
        write32(ram, record + 4u, object);
        write32(ram, record + 8u, param1);
        write32(ram, record + 0x0cu, 0u);
        write32(ram, record + 0x10u, param2);
        write32(ram, record + 0x14u, param3);
        write32(ram, record + 0x2cu, read32(ram, 0x8c65be4cu));
        write32(ram, record + 0x30u, rounded);

        const uint32_t vtable = read32(ram, object);
        const uint32_t method = read32(ram, vtable + 0x24u);
        state->r[0] = 0u;
        state->r[1] = method;
        state->r[2] = vtable;
        state->r[3] = bias;
        state->r[4] = object;
        state->r[5] = 0u;
        state->r[6] = param3;
        state->r[7] = 1u;
        state->sr = (state->sr & ~1u) |
                    (uint32_t)(((int32_t)(param3 + bias) >= 0) ? 1u : 0u);
        /* These live values are observable by the object's indirect method.
         * The epilogue restores the saved registers after it returns. */
        state->r[10] = 0u;
        state->r[11] = object;
        state->r[13] = table;
        state->r[14] = record;
        state->r[15] = original_stack - 20u;
        state->pr = 0x8c65bdb8u;
        state->pc = method;
        object_query(ram, state, object, 0u);
        write32(ram, record + 0x18u, state->r[0]);
        write32(ram, record + 0x1cu, state->r[0]);
        state->r[0] = record;
    }

    state->r[15] = original_stack;
    state->r[10] = read32(ram, original_stack - 16u);
    state->r[11] = read32(ram, original_stack - 12u);
    state->r[13] = read32(ram, original_stack - 8u);
    state->r[14] = read32(ram, original_stack - 4u);
    state->pr = read32(ram, original_stack - 20u);
    state->pc = state->pr;
}


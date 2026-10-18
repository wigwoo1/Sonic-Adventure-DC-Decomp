#include <stdint.h>

/* The retail pointer literals resolve to three 16-byte stage tables. */
#define TABLE_A 0x0caf97d4u
#define TABLE_B 0x0caf97e4u
#define TABLE_C 0x0caf97f4u

static uint32_t offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_stg09_cpu;

static void push32(uint8_t *ram, uint32_t *sp, uint32_t value)
{
    *sp -= 4u;
    uint32_t at = offset(*sp);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static uint8_t table_byte(const uint8_t *ram, uint32_t table, uint32_t index)
{
    return ram[offset(table) + (index & 15u)];
}

/* Indices are outputs of the retail indirect service at 0x8c10ca5c. */
typedef uint32_t (*sa_tikal_sample_fn)(void *context, uint32_t first_bound);

__declspec(dllexport)
void sa_stg09_tikal_compare_logic_0c903a7c(uint8_t *ram, sa_stg09_cpu *cpu,
                                          sa_tikal_sample_fn sample,
                                          void *context)
{
    uint32_t object = cpu->r[4];
    uint32_t original_sp = cpu->r[15];
    uint32_t original_pr = cpu->pr;
    uint32_t sp = original_sp;
    push32(ram, &sp, cpu->r[14]);
    push32(ram, &sp, cpu->r[13]);
    push32(ram, &sp, cpu->r[12]);
    push32(ram, &sp, cpu->r[11]);
    push32(ram, &sp, cpu->r[9]);
    push32(ram, &sp, cpu->r[8]);
    push32(ram, &sp, original_pr);
    uint32_t i = sample(context, 0x1000u) & 15u;
    uint32_t j = sample(context, 0u) & 15u;
    uint32_t k = sample(context, 0u) & 15u;
    uint32_t o = offset(object);
    uint8_t a_next = table_byte(ram, TABLE_A, i + 1u);
    uint8_t b_next = table_byte(ram, TABLE_B, j + 1u);
    uint8_t c_next = table_byte(ram, TABLE_C, k + 1u);
    uint8_t a_here = table_byte(ram, TABLE_A, i);
    uint8_t a_prev = table_byte(ram, TABLE_A, i - 1u);
    uint8_t b_here = table_byte(ram, TABLE_B, j);
    uint8_t c_here = table_byte(ram, TABLE_C, k);
    uint8_t b_prev = table_byte(ram, TABLE_B, j - 1u);
    uint8_t c_prev = table_byte(ram, TABLE_C, k - 1u);
    uint8_t selected = a_next;

    if (a_next == b_next && b_next == c_next) {
        ram[o + 0x10u] = 0;
    } else if (a_here == b_here && b_here == c_here) {
        ram[o + 0x10u] = 1;
        ram[o + 0x12u] = a_here;
        goto finish;
    } else if (a_prev == b_prev && b_prev == c_prev) {
        ram[o + 0x10u] = 2;
        ram[o + 0x12u] = a_prev;
        goto finish;
    } else if (a_next == b_here && b_here == c_prev) {
        ram[o + 0x10u] = 3;
    } else if (c_next == b_here && b_here == a_prev) {
        ram[o + 0x10u] = 4;
        selected = c_next;
    } else {
        goto finish;
    }
    ram[o + 0x12u] = selected;

finish:
    if ((int8_t)ram[o + 0x12u] != (int8_t)'d') {
        ram[o + 10u] = 0;
    }
    cpu->r[15] = original_sp;
    cpu->pr = original_pr;
    cpu->pc = original_pr;
}

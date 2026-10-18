#include "main_ready_flag.h"

/* The literal at 0x8c04c780 points to a global 16-bit ready flag. The SH-4
 * RTS delay slot performs the store, so it is part of this routine's effect. */
void sa_main_ready_flag_8c04c680(uint8_t *ram, uint32_t *r) {
    r[2] = 0x8c19de26u;
    r[3] = 1u;
    ram[0x19de26u] = 1u;
    ram[0x19de27u] = 0u;
}

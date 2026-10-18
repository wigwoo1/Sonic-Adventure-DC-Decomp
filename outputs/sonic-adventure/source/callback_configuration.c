#include "callback_configuration.h"

static uint32_t read_word(const uint8_t *ram, uint32_t address)
{
    const uint8_t *p = ram + ((address & 0x1fffffffu) - 0x0c000000u);
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static void write_word(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint8_t *p = ram + ((address & 0x1fffffffu) - 0x0c000000u);
    for (unsigned i = 0; i < 4; ++i)
        p[i] = (uint8_t)(value >> (i * 8));
}

/* Retail 0x8c6418e8..0x8c641946; the system-mode read at 0xa05f689c is MMIO. */
SA_STAGE_EXPORT uint32_t sa_callback_configuration_8c6418e8(
    uint8_t *ram, uint32_t system_mode, uint32_t callback, uint32_t enabled)
{
    uint32_t callback_slot = read_word(ram, 0x8c64193cu);
    uint32_t enabled_slot = read_word(ram, 0x8c641940u);
    write_word(ram, callback_slot, callback);
    write_word(ram, enabled_slot, enabled);

    uint32_t flag_address = read_word(ram, enabled == 0u ? 0x8c641944u : 0x8c6419e4u);
    uint32_t value = read_word(ram, flag_address);
    if (enabled == 0u || system_mode == 4u || system_mode == 5u ||
        system_mode == 6u || system_mode == 8u)
        value |= 0x10u;
    else
        value &= ~0x10u;
    write_word(ram, flag_address, value);
    return 0u;
}

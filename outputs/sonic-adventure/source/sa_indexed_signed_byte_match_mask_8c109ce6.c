#include <stdint.h>

#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif

struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
static uint32_t offset(uint32_t address) { return (address&0x1fffffffu)-0x0c000000u; }
static uint32_t read32(const uint8_t *ram,uint32_t address) {
    uint32_t at=offset(address);
    return (uint32_t)ram[at]|((uint32_t)ram[at+1]<<8)|((uint32_t)ram[at+2]<<16)|((uint32_t)ram[at+3]<<24);
}
static uint8_t read8(const uint8_t *ram,uint32_t address) { return ram[offset(address)]; }

SA_EXPORT void sa_indexed_signed_byte_match_mask_8c109ce6(const uint8_t *ram,struct sa_cpu_state *state) {
    state->r[2]=read32(ram,0x8c109de4u);
    int32_t field=(int32_t)(int8_t)read8(ram,state->r[4]+0x602u);
    state->r[3]=(uint32_t)field;
    uint32_t masked=read32(ram,state->r[2])&7u;
    state->r[0]=(masked==(uint32_t)field)?1u:0u;
    if(state->r[0])state->sr|=1u;else state->sr&=~1u;
    state->pc=state->pr;
}

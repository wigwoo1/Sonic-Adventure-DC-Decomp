#include <stdint.h>

#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif

struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
static uint32_t offset(uint32_t address) { return (address&0x1fffffffu)-0x0c000000u; }
static uint16_t read16(const uint8_t *ram,uint32_t address) {
    uint32_t at=offset(address);return (uint16_t)(ram[at]|((uint16_t)ram[at+1]<<8));
}
static uint32_t read32(const uint8_t *ram,uint32_t address) {
    uint32_t at=offset(address);
    return (uint32_t)ram[at]|((uint32_t)ram[at+1]<<8)|((uint32_t)ram[at+2]<<16)|((uint32_t)ram[at+3]<<24);
}

SA_EXPORT void sa_indexed_stride_value_8c64b274(const uint8_t *ram,struct sa_cpu_state *state) {
    uint32_t scaled_value=state->r[5]<<5;
    uint32_t stride=(uint32_t)(int32_t)(int16_t)read16(ram,0x8c64b2e0u);
    uint32_t table=read32(ram,0x8c64b2e4u);
    state->r[0]=scaled_value;
    state->r[2]=table;
    state->r[3]=stride;
    state->r[4]=table+state->r[4]*stride;
    state->r[1]=read32(ram,state->r[4]+0x28u);
    state->r[0]+=state->r[1];
    if((state->r[5]>>27)&1u)state->sr|=1u;else state->sr&=~1u;
    state->pc=state->pr;
}

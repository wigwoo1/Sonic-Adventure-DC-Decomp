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

SA_EXPORT void sa_range_nonzero_validate_8c10c116(const uint8_t *ram,struct sa_cpu_state *state) {
    int32_t index=(int32_t)state->r[4];
    if(index<0) {
        state->r[0]=0xfffffffdu;
        state->sr&=~1u;
    } else {
        uint32_t limit=read16(ram,0x8c10c1b8u);
        state->r[2]=limit;
        if((uint32_t)index<limit && state->r[5]!=0u) {
            state->r[0]=0u;
            state->sr&=~1u;
        } else {
            state->r[0]=0xfffffffdu;
            state->sr|=1u;
        }
    }
    state->pc=state->pr;
}

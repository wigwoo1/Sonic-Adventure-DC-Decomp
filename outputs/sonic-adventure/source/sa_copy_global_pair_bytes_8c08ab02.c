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
static void write8(uint8_t *ram,uint32_t address,uint8_t value) { ram[offset(address)]=value; }

SA_EXPORT void sa_copy_global_pair_bytes_8c08ab02(uint8_t *ram,struct sa_cpu_state *state) {
    state->r[2]=read32(ram,0x8c08aba4u);
    state->r[0]=read32(ram,0x8c08aba8u);
    state->r[3]=(uint32_t)(int32_t)(int8_t)read8(ram,state->r[2]);
    write8(ram,state->r[5],(uint8_t)state->r[3]);
    state->r[1]=(uint32_t)(int32_t)(int8_t)read8(ram,state->r[0]);
    write8(ram,state->r[4],(uint8_t)state->r[1]);
    state->pc=state->pr;
}

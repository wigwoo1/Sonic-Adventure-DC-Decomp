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
static void write32(uint8_t *ram,uint32_t address,uint32_t value) {
    uint32_t at=offset(address);
    ram[at]=(uint8_t)value;ram[at+1]=(uint8_t)(value>>8);ram[at+2]=(uint8_t)(value>>16);ram[at+3]=(uint8_t)(value>>24);
}

SA_EXPORT void sa_initialize_record_fields_8c608914(uint8_t *ram,struct sa_cpu_state *state) {
    uint32_t record=state->r[4];
    state->r[6]<<=16;
    write32(ram,record,state->r[5]);
    write32(ram,record+16u,state->r[7]);
    state->r[3]=read32(ram,state->r[15]);
    write32(ram,record+20u,state->r[3]);
    write32(ram,record+4u,state->r[6]);
    state->pc=state->pr;
}

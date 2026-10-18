#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct sa_cpu_state { uint32_t r[16],sr,pr,pc; };
static uint32_t read32(const uint8_t *ram,uint32_t address)
{
    uint32_t p=(address&0x1fffffffu)-0x0c000000u;
    return (uint32_t)ram[p]|(uint32_t)ram[p+1]<<8|(uint32_t)ram[p+2]<<16|(uint32_t)ram[p+3]<<24;
}
/* 0x8c6451a4: MOV.L literal,R3; JMP @R3; NOP delay slot. */
SA_EXPORT void sa_device_cancel_tail_8c6451a4(const uint8_t *ram,struct sa_cpu_state *state)
{
    state->r[3]=read32(ram,0x8c6451f8u);
    state->pc=state->r[3];
}
/* 0x8c10d9d8: select operation 8 and tail-call the runtime service vector. */
SA_EXPORT void sa_device_cancel_service_8c10d9d8(const uint8_t *ram,struct sa_cpu_state *state)
{
    state->r[6]=0u;
    state->r[7]=read32(ram,0x8c10d9e4u);
    state->r[0]=read32(ram,0x8c10d9e8u);
    state->r[0]=read32(ram,state->r[0]);
    state->pc=state->r[0];
}

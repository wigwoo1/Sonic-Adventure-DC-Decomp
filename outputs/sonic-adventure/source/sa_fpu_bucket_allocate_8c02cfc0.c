#include <stdint.h>

#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif

struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
struct sa_fpu_state { uint32_t banks[2][16], fpscr, fpul; };

static uint32_t ram_offset(uint32_t address) {
    return (address & 0x1fffffffu) - 0x0c000000u;
}
static uint32_t read32(const uint8_t *ram, uint32_t address) {
    uint32_t o=ram_offset(address);
    return (uint32_t)ram[o] | ((uint32_t)ram[o+1]<<8) | ((uint32_t)ram[o+2]<<16) | ((uint32_t)ram[o+3]<<24);
}
static void write32(uint8_t *ram, uint32_t address, uint32_t value) {
    uint32_t o=ram_offset(address);
    ram[o]=(uint8_t)value;ram[o+1]=(uint8_t)(value>>8);ram[o+2]=(uint8_t)(value>>16);ram[o+3]=(uint8_t)(value>>24);
}
static uint32_t truncate_f32(uint32_t raw, uint32_t *invalid) {
    uint32_t absolute=raw&0x7fffffffu, negative=raw>>31;
    if (absolute>(negative?0x4f000000u:0x4effffffu)) {
        *invalid=1;
        return (!negative && absolute<=0x7f800000u)?0x7fffffffu:0x80000000u;
    }
    *invalid=0;
    uint32_t exponent=(absolute>>23)&255u, fraction=absolute&0x7fffffu;
    int power=(int)exponent-127-23;
    uint64_t coefficient=exponent?((uint64_t)fraction|0x800000u):fraction;
    if (!exponent) power=-149;
    uint64_t magnitude=power>=0?coefficient<<(unsigned)power:coefficient>>(unsigned)(-power);
    uint32_t result=(uint32_t)magnitude;
    return negative?0u-result:result;
}
static uint32_t truncate_f64(uint32_t high, uint32_t low, uint32_t *invalid) {
    uint64_t raw=((uint64_t)high<<32)|low,absolute=raw&UINT64_C(0x7fffffffffffffff);
    uint32_t negative=(uint32_t)(raw>>63),exponent=(uint32_t)((absolute>>52)&0x7ffu);
    uint64_t fraction=absolute&UINT64_C(0xfffffffffffff);
    if (exponent==0x7ffu) { *invalid=1;return (!negative && fraction==0)?0x7fffffffu:0x80000000u; }
    if (negative ? absolute>=UINT64_C(0x41e0000000200000) : absolute>=UINT64_C(0x41e0000000000000)) {
        *invalid=1;return negative?0x80000000u:0x7fffffffu;
    }
    *invalid=0;
    uint64_t coefficient;int power;
    if (exponent==0) { coefficient=fraction;power=-1074; }
    else { coefficient=(UINT64_C(1)<<52)|fraction;power=(int)exponent-1023-52; }
    uint64_t magnitude;
    if (power>=0)magnitude=coefficient<<(unsigned)power;
    else if (power<=-64)magnitude=0;
    else magnitude=coefficient>>(unsigned)(-power);
    uint32_t result=(uint32_t)magnitude;
    return negative?0u-result:result;
}

SA_EXPORT void sa_fpu_bucket_allocate_8c02cfc0(uint8_t *ram, struct sa_cpu_state *cpu, struct sa_fpu_state *fpu) {
    uint32_t invalid=0;
    unsigned bank=(fpu->fpscr>>21)&1u;
    uint32_t converted;
    if (fpu->fpscr&0x80000u) converted=truncate_f64(fpu->banks[bank][4],fpu->banks[bank][5],&invalid);
    else converted=truncate_f32(fpu->banks[bank][4],&invalid);
    fpu->fpul=converted;
    fpu->fpscr &= ~0x3f000u;
    if (invalid) fpu->fpscr |= 0x10040u;

    int32_t bucket_index=((int32_t)converted>>4)+64;
    if (bucket_index<0) bucket_index=0;
    if (bucket_index>127) bucket_index=127;
    uint32_t bucket=0x8c02d560u+(uint32_t)bucket_index*8u;
    uint32_t free_head=read32(ram,0x8c02d540u);
    cpu->sr |= 1u;
    cpu->r[0]=0;cpu->r[1]=0x8c02d540u;cpu->r[2]=127u;cpu->r[3]=0;cpu->r[4]=bucket;
    uint32_t next_free=read32(ram,free_head);
    if (next_free==0) { cpu->pc=cpu->pr;return; }
    write32(ram,0x8c02d540u,next_free);
    uint32_t old_head=read32(ram,bucket);
    if (old_head) cpu->sr &= ~1u;
    else cpu->sr |= 1u;
    write32(ram,free_head,old_head);
    if (old_head==0) write32(ram,bucket+4u,free_head);
    else write32(ram,old_head+4u,free_head);
    write32(ram,bucket,free_head);
    write32(ram,free_head+4u,0);
    cpu->r[0]=free_head;cpu->r[2]=old_head;
    cpu->pc=cpu->pr;
}

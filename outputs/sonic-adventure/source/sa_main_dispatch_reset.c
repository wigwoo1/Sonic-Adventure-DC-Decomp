#include "fpu_cleanup_transfer.h"

typedef void (*sa_reset_observer)(void *,uint32_t,const uint8_t *,const struct sa_fpu_banks *,const uint32_t *,const uint32_t *,const uint32_t *);
int sa_main_dispatch_clear_table_8c05e69e(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_reset_observer);

static uint32_t read_value(const uint8_t *ram,uint32_t address,unsigned width)
{
    uint32_t value=0;
    const uint32_t at=address&0x00ffffffu;
    for(unsigned i=0;i<width;++i)value|=(uint32_t)ram[at+i]<<(8u*i);
    return value;
}

static void write_value(uint8_t *ram,uint32_t address,uint32_t value,unsigned width)
{
    const uint32_t at=address&0x00ffffffu;
    for(unsigned i=0;i<width;++i)ram[at+i]=(uint8_t)(value>>(8u*i));
}

/* Retail 8c05e788..8c05e7d4, 38 integer instructions. Inferred name.
 * Native clear executes before the object chain/index is read. Index nine
 * selects zero; otherwise inspect current+1, selecting it if nonnull and
 * zero otherwise. Final selection sign-extends the low 16 bits, including
 * the 32767 -> -32768 case. No extra bounds/null fallback is introduced.
 * Host 0 means complete; this void retail routine leaves incidental GPR
 * results intact. PR/stack/SR effects are explicit; FPU/FPUL/MACL unchanged.
 * Valid 16 MiB guest RAM and retail pointer preconditions are required.
 */
SA_STAGE_EXPORT int sa_main_dispatch_reset_8c05e788(
    uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t *sr,uint32_t *r,
    uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *context,sa_reset_observer observe)
{
    r[15]-=4u;
    write_value(ram,r[15],*pr,4);
    r[15]-=4u;
    *pr=0x8c05e790u;
    write_value(ram,r[15],r[4],4); /* BSR delay slot */
    if(observe)observe(context,0x8c05e69eu,ram,fpu,sr,r,pr);
    int status=sa_main_dispatch_clear_table_8c05e69e(ram,fpu,sr,r,pr,fpul,macl,context,observe);
    if(status)return status;
    r[4]=read_value(ram,r[15],4);
    r[3]=1u;
    r[4]=read_value(ram,r[4]+0x38u,4);
    r[4]=read_value(ram,r[4]+0x18u,4);
    r[5]=read_value(ram,r[4]+0x10u,4);
    r[5]=read_value(ram,r[5]+0x20u,4);
    r[0]=(uint32_t)(int32_t)(int16_t)read_value(ram,r[5]+6u,2);
    write_value(ram,r[5],r[3],1);
    r[4]=r[0];
    r[6]=(uint32_t)(int32_t)(int16_t)r[4];
    r[0]=r[6];
    r[5]=read_value(ram,0x8c05e840u,4);
    *sr=(*sr&~1u)|(r[0]==9u);
    const unsigned take=*sr&1u;
    r[7]=0u; /* BT/S delay slot */
    if(take)r[4]=r[7];
    else{
        r[0]=r[6];
        r[0]+=1u;
        r[0]<<=2;
        r[3]=read_value(ram,r[5]+r[0],4);
        *sr=(*sr&~1u)|(r[3]==0u);
        if(!(*sr&1u))r[4]+=1u;
        else r[4]=r[7];
    }
    r[0]=(uint32_t)(int32_t)(int16_t)r[4];
    r[0]<<=2;
    r[3]=read_value(ram,r[5]+r[0],4);
    r[1]=2u;
    r[2]=read_value(ram,r[3]+0x20u,4);
    write_value(ram,r[2],r[1],1);
    r[15]+=4u;
    *pr=read_value(ram,r[15],4);
    r[15]+=4u;
    return 0;
}

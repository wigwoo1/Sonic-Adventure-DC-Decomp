#include "fpu_cleanup_transfer.h"

typedef int (*sa_query_service)(void *,uint32_t,uint8_t *,
    struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
typedef void (*sa_query_observer)(void *,uint32_t,const uint8_t *,
    const struct sa_fpu_banks *,const uint32_t *,const uint32_t *,const uint32_t *);

static uint32_t read_value(const uint8_t *ram,uint32_t address,unsigned width)
{
    uint32_t value=0;
    const uint32_t at=address&0x00ffffffu;
    for(unsigned i=0;i<width;++i)value|=(uint32_t)ram[at+i]<<(8u*i);
    return value;
}

static void write32(uint8_t *ram,uint32_t address,uint32_t value)
{
    const uint32_t at=address&0x00ffffffu;
    for(unsigned i=0;i<4;++i)ram[at+i]=(uint8_t)(value>>(8u*i));
}

/* Retail 8c06cebe..8c06cefe: 32 integer instructions. Symbol/field roles
 * are descriptive inferences. Test object flag 0x4000 and the intersection
 * of a per-byte-index table entry (stride 0x34) with a guest global mask.
 * Active entries call the literal target; positive signed R0 returns 2,
 * zero/negative returns 1, inactive/flagged returns 0. MULU.W changes MACL.
 * Host status 0 means completion; retail status is R0. Missing service -2
 * rejects atomically; service errors retain the exact partial guest state.
 * Valid 16 MiB guest RAM is required. No FPU instructions occur here.
 */
SA_STAGE_EXPORT int sa_main_dispatch_status_query_8c06cebe(
    uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t *sr,uint32_t *r,
    uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *context,sa_query_service call)
{
    (void)fpul;
    if(!call)return -2;
    r[3]=0x40u;
    r[15]-=4u;
    write32(ram,r[15],*pr);
    r[0]=(uint32_t)(int32_t)(int16_t)read_value(ram,r[4]+4u,2);
    r[3]<<=8;
    *sr=(*sr&~1u)|((r[0]&r[3])==0);
    if(!(*sr&1u))goto inactive;
    r[0]=(uint32_t)(int32_t)(int8_t)read_value(ram,r[4]+8u,1);
    r[3]=0x34u;
    r[1]=read_value(ram,0x8c06cf4cu,4);
    r[0]&=0xffu;
    *macl=(r[0]&0xffffu)*(r[3]&0xffffu);
    r[2]=read_value(ram,r[1],4);
    r[1]=read_value(ram,0x8c06cf50u,4);
    r[0]=*macl;
    r[1]=read_value(ram,r[0]+r[1],4);
    *sr=(*sr&~1u)|((r[1]&r[2])==0);
    if(*sr&1u)goto inactive;
    r[2]=read_value(ram,0x8c06cf54u,4);
    const uint32_t target=r[2];
    *pr=0x8c06cee6u;
    int status=call(context,target,ram,fpu,sr,r,pr);
    if(status)return status;
    *sr=(*sr&~1u)|((int32_t)r[0]>0);
    if(*sr&1u){
        *pr=read_value(ram,r[15],4);r[15]+=4u;
        r[0]=2u;return 0;
    }
    *pr=read_value(ram,r[15],4);r[15]+=4u;
    r[0]=1u;return 0;
inactive:
    r[0]=0u;
    *pr=read_value(ram,r[15],4);r[15]+=4u;
    return 0;
}

int sa_main_dispatch_status_transition_8c0afc98(uint8_t *,struct sa_fpu_banks *,
    uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_query_service);

struct query_services {
    void *user;
    sa_query_service external;
    sa_query_observer observe;
    uint32_t *fpul;
    uint32_t *macl;
};

static int dispatch(void *context,uint32_t target,uint8_t *ram,
    struct sa_fpu_banks *fpu,uint32_t *sr,uint32_t *r,uint32_t *pr)
{
    struct query_services *services=(struct query_services *)context;
    if(services->observe)services->observe(services->user,target,ram,fpu,sr,r,pr);
    if(target==0x8c06cebeu)
        return sa_main_dispatch_status_query_8c06cebe(ram,fpu,sr,r,pr,
            services->fpul,services->macl,context,dispatch);
    return services->external(services->user,target,ram,fpu,sr,r,pr);
}

/* Native connection support, not an extra retail game routine. The two
 * independent query call sites both execute the recovered query body.
 * Its active-entry target and the parent's other callees remain services.
 */
SA_STAGE_EXPORT int sa_main_dispatch_status_query_connected(
    uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t *sr,uint32_t *r,
    uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *user,
    sa_query_service external,sa_query_observer observe)
{
    if(!external)return -2;
    struct query_services services={user,external,observe,fpul,macl};
    return sa_main_dispatch_status_transition_8c0afc98(ram,fpu,sr,r,pr,
        fpul,macl,&services,dispatch);
}

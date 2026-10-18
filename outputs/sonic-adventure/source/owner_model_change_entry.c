#include "owner_helper_state0.h"
void sa_owner_model_change_prepare(uint8_t *,uint32_t *,uint32_t,uint32_t,uint32_t *);
int sa_owner_model_change_worker(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,void *,sa_owner_state0_service,uint32_t *);
static void wr(uint8_t *ram,uint32_t a,uint32_t v){uint8_t *p=ram+((a&0x1fffffffu)-0x0c000000u);for(unsigned i=0;i<4;i++)p[i]=(uint8_t)(v>>(8*i));}
/* Full entry 8c079c20 through return: explicit original callee-save writes.
 * The C ABI handles register restoration. Volatile integer/FR outputs other
 * than r0, SR, FPSCR and FPUL are not exposed by this interface. */
SA_STAGE_EXPORT int sa_owner_model_change_entry(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t stack,const uint32_t *callee,uint32_t fr15,uint32_t pr,void *context,sa_owner_state0_service service,uint32_t *r0){
 uint32_t registers[16]={0};sa_owner_model_change_prepare(ram,sr,config,stack,registers);
 for(unsigned i=0;i<7;i++)wr(ram,stack-4u*(i+1u),callee[6u-i]);
 wr(ram,stack-32u,fr15);wr(ram,stack-36u,pr);
 return sa_owner_model_change_worker(ram,sr,fpscr,fpul,config,stack-532u,context,service,r0);
}

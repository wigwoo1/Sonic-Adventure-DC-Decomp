/* Static native C recovery of original ARM reset flow and twelve leaf helpers.
 * Driver 0=MANATEE, 1=MASARU. Remaining calls are explicit external services.
 * Active ARM registers only; caller provides SVC/ARM state, stable 2 MiB RAM,
 * and MMIO callbacks. This component does not emulate timing or synthesize audio. */
#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct sa_arm_state {uint32_t r[16],cpsr;};
typedef int (*sa_arm_read)(void *,uint32_t,unsigned,uint32_t *);
typedef int (*sa_arm_write)(void *,uint32_t,uint32_t,unsigned);
typedef int (*sa_arm_service)(void *,uint8_t *,struct sa_arm_state *,uint32_t);
static uint32_t subtract(struct sa_arm_state *s,uint32_t a,uint32_t b){
 uint32_t v=a-b,flags=v&0x80000000u;
 if(!v)flags|=0x40000000u;if(a>=b)flags|=0x20000000u;
 flags|=((a^b)&(a^v)&0x80000000u)>>3;
 s->cpsr=(s->cpsr&0x0fffffffu)|flags;return v;
}
static void logic_flags(struct sa_arm_state *s,uint32_t v,int rotated,uint32_t immediate){
 uint32_t flags=(v&0x80000000u)|(!v?0x40000000u:0);
 s->cpsr=(s->cpsr&0x3fffffffu)|flags;
 if(rotated)s->cpsr=(s->cpsr&~0x20000000u)|((immediate>>31)<<29);
}
#include "arm_sound_memory.h"
#define LOAD(destination,address,length) do {uint32_t v;int rc=load(ram,user,read,address,length,&v);if(rc)return rc;destination=v;}while(0)
#define STORE(address,value,length) do {int rc=store(ram,user,write,address,value,length);if(rc)return rc;}while(0)
#include "arm_sound_exception.h"
int sa_arm_sound_exception_step(uint8_t *ram,struct sa_arm_state *state,struct sa_arm_exception_banks *banks,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
        case 0x4u: /* b 0x00000234 */
            r[15]=564u;return 0;
        case 0x8u: /* b 0x00000234 */
            r[15]=564u;return 0;
        case 0xcu: /* b 0x00000234 */
            r[15]=564u;return 0;
        case 0x10u: /* b 0x00000234 */
            r[15]=564u;return 0;
        case 0x14u: /* b 0x00000234 */
            r[15]=564u;return 0;
        case 0x18u: /* b 0x00000234 */
            r[15]=564u;return 0;
        case 0x1cu: /* b 0x00000248 */
            r[15]=584u;return 0;
        case 0x234u: /* sub lr,lr,#0x4 */
            r[14]=r[14]-4u;r[15]=0x238u;return 0;
        case 0x238u: /* adr sp,0x244 */
            r[13]=580u;r[15]=0x23cu;return 0;
        case 0x23cu: /* str lr,[sp,#0x0] */
            STORE(r[13],r[14],4);r[15]=0x240u;return 0;
        case 0x240u: /* ldmia sp,{pc}^ */
            {uint32_t target;LOAD(target,r[13],4);uint32_t mode=state->cpsr&31u;for(unsigned i=0;i<7;++i)banks->exception_r8_r14[i]=r[8+i];if(mode==0x11u){for(unsigned i=0;i<7;++i)r[8+i]=banks->svc_r8_r14[i];}else if(mode!=0x13u){r[13]=banks->svc_r8_r14[5];r[14]=banks->svc_r8_r14[6];}for(unsigned i=0;i<7;++i)banks->svc_r8_r14[i]=r[8+i];state->cpsr=banks->spsr;r[15]=target&~3u;banks->returned=1u;return 0;}
        case 0x248u: /* sub lr,lr,#0x4 */
            r[14]=r[14]-4u;r[15]=0x24cu;return 0;
        case 0x24cu: /* adr sp,0x3cc */
            r[13]=972u;r[15]=0x250u;return 0;
        case 0x250u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x254u;return 0;
        case 0x254u: /* mov r11,#0x0 */
            r[11]=0u;r[15]=0x258u;return 0;
        case 0x258u: /* ldr r8,[0x8fc] */
            LOAD(r[8],2300u,4);r[15]=0x25cu;return 0;
        case 0x25cu: /* ldr r9,[r8,#0x0] */
            LOAD(r[9],r[8]+0u,4);r[15]=0x260u;return 0;
        case 0x260u: /* add r9,r9,#0x1 */
            r[9]=r[9]+1u;r[15]=0x264u;return 0;
        case 0x264u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x268u;return 0;
        case 0x268u: /* ldr r8,[0x900] */
            LOAD(r[8],2304u,4);r[15]=0x26cu;return 0;
        case 0x26cu: /* ldr r10,[r8,#0x0] */
            LOAD(r[10],r[8]+0u,4);r[15]=0x270u;return 0;
        case 0x270u: /* and r10,r10,#0x7 */
            r[10]=r[10]&7u;r[15]=0x274u;return 0;
        case 0x274u: /* mov r10,r10, lsl #0x2 */
            r[10]=(r[10]<<2);r[15]=0x278u;return 0;
        case 0x278u: /* adr r8,0x284 */
            r[8]=644u;r[15]=0x27cu;return 0;
        case 0x27cu: /* add r8,r8,r10 */
            r[8]=r[8]+r[10];r[15]=0x280u;return 0;
        case 0x280u: /* mov pc,r8 */
            r[15]=r[8];return 0;
        case 0x284u: /* b 0x000002a4 */
            r[15]=676u;return 0;
        case 0x288u: /* b 0x000002a4 */
            r[15]=676u;return 0;
        case 0x28cu: /* b 0x000002a8 */
            r[15]=680u;return 0;
        case 0x290u: /* b 0x000002a4 */
            r[15]=676u;return 0;
        case 0x294u: /* b 0x000002a4 */
            r[15]=676u;return 0;
        case 0x298u: /* b 0x00000328 */
            r[15]=808u;return 0;
        case 0x29cu: /* b 0x000002a4 */
            r[15]=676u;return 0;
        case 0x2a0u: /* b 0x000002a4 */
            r[15]=676u;return 0;
        case 0x2a4u: /* b 0x0000034c */
            r[15]=844u;return 0;
        case 0x2a8u: /* ldr r8,[0x904] */
            LOAD(r[8],2308u,4);r[15]=0x2acu;return 0;
        case 0x2acu: /* ldr r10,[0x31c] */
            LOAD(r[10],796u,4);r[15]=0x2b0u;return 0;
        case 0x2b0u: /* str r10,[r8,#0x0] */
            STORE(r[8]+0u,r[10],4);r[15]=0x2b4u;return 0;
        case 0x2b4u: /* mov r9,#0x40 */
            r[9]=64u;r[15]=0x2b8u;return 0;
        case 0x2b8u: /* ldr r8,[0x908] */
            LOAD(r[8],2312u,4);r[15]=0x2bcu;return 0;
        case 0x2bcu: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x2c0u;return 0;
        case 0x2c0u: /* mov r9,#0xff */
            r[9]=255u;r[15]=0x2c4u;return 0;
        case 0x2c4u: /* strb r9,[0x22e] */
            STORE(558u,r[9],1);r[15]=0x2c8u;return 0;
        case 0x2c8u: /* ldr r9,[0x320] */
            LOAD(r[9],800u,4);r[15]=0x2ccu;return 0;
        case 0x2ccu: /* subs r9,r9,#0x1 */
            r[9]=subtract(state,r[9],1u);r[15]=0x2d0u;return 0;
        case 0x2d0u: /* ldreq r9,[0x324] */
            if(state->cpsr&0x40000000u){LOAD(r[9],804u,4);}r[15]=0x2d4u;return 0;
        case 0x2d4u: /* str r9,[0x320] */
            STORE(800u,r[9],4);r[15]=0x2d8u;return 0;
        case 0x2d8u: /* beq 0x0000034c */
            if(state->cpsr&0x40000000u){r[15]=844u;}else{r[15]=0x2dcu;}return 0;
        case 0x2dcu: /* ldr r8,[0x90c] */
            LOAD(r[8],2316u,4);r[15]=0x2e0u;return 0;
        case 0x2e0u: /* ldrb r9,[r8,#0x0] */
            LOAD(r[9],r[8]+0u,1);r[15]=0x2e4u;return 0;
        case 0x2e4u: /* add r9,r9,#0x1 */
            r[9]=r[9]+1u;r[15]=0x2e8u;return 0;
        case 0x2e8u: /* ands r9,r9,#0x3 */
            temporary=r[9]&3u;r[9]=temporary;logic_flags(state,temporary,0,3u);r[15]=0x2ecu;return 0;
        case 0x2ecu: /* strb r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],1);r[15]=0x2f0u;return 0;
        case 0x2f0u: /* bne 0x0000034c */
            if(!(state->cpsr&0x40000000u)){r[15]=844u;}else{r[15]=0x2f4u;}return 0;
        case 0x2f4u: /* ldr r8,[0x910] */
            LOAD(r[8],2320u,4);r[15]=0x2f8u;return 0;
        case 0x2f8u: /* ldr r9,[r8,#0x0] */
            LOAD(r[9],r[8]+0u,4);r[15]=0x2fcu;return 0;
        case 0x2fcu: /* add r9,r9,#0x1 */
            r[9]=r[9]+1u;r[15]=0x300u;return 0;
        case 0x300u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x304u;return 0;
        case 0x304u: /* mov r9,#0xff */
            r[9]=255u;r[15]=0x308u;return 0;
        case 0x308u: /* strb r9,[0x22c] */
            STORE(556u,r[9],1);r[15]=0x30cu;return 0;
        case 0x30cu: /* mov r9,#0x0 */
            r[9]=0u;r[15]=0x310u;return 0;
        case 0x310u: /* strb r9,[0x22e] */
            STORE(558u,r[9],1);r[15]=0x314u;return 0;
        case 0x314u: /* b 0x0000034c */
            r[15]=844u;return 0;
        case 0x328u: /* ldr r8,[0x914] */
            LOAD(r[8],2324u,4);r[15]=0x32cu;return 0;
        case 0x32cu: /* ldr r9,[r8,#0x0] */
            LOAD(r[9],r[8]+0u,4);r[15]=0x330u;return 0;
        case 0x330u: /* add r9,r9,#0x1 */
            r[9]=r[9]+1u;r[15]=0x334u;return 0;
        case 0x334u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x338u;return 0;
        case 0x338u: /* mvn r9,#0x0 */
            r[9]=~0u;r[15]=0x33cu;return 0;
        case 0x33cu: /* str r9,[0xfc] */
            STORE(252u,r[9],4);r[15]=0x340u;return 0;
        case 0x340u: /* stmdb sp!,{r0,r1,r2,r3,r4,r5,r6,r7} */
            STORE(r[13]-32u,r[0],4);STORE(r[13]-28u,r[1],4);STORE(r[13]-24u,r[2],4);STORE(r[13]-20u,r[3],4);STORE(r[13]-16u,r[4],4);STORE(r[13]-12u,r[5],4);STORE(r[13]-8u,r[6],4);STORE(r[13]-4u,r[7],4);r[13]-=32u;r[15]=0x344u;return 0;
        case 0x344u: /* bl 0x00001598 */
            r[14]=0x348u;r[15]=5528u;return 0;
        case 0x348u: /* ldmia sp!,{r0,r1,r2,r3,r4,r5,r6,r7} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[1],r[13]+4u,4);LOAD(r[2],r[13]+8u,4);LOAD(r[3],r[13]+12u,4);LOAD(r[4],r[13]+16u,4);LOAD(r[5],r[13]+20u,4);LOAD(r[6],r[13]+24u,4);LOAD(r[7],r[13]+28u,4);r[13]+=32u;r[15]=0x34cu;return 0;
        case 0x34cu: /* ldr r8,[0x918] */
            LOAD(r[8],2328u,4);r[15]=0x350u;return 0;
        case 0x350u: /* mov r9,#0x1 */
            r[9]=1u;r[15]=0x354u;return 0;
        case 0x354u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x358u;return 0;
        case 0x358u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x35cu;return 0;
        case 0x35cu: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x360u;return 0;
        case 0x360u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x364u;return 0;
        case 0x364u: /* ldmia sp!,{pc}^ */
            {uint32_t target;LOAD(target,r[13],4);r[13]+=4u;uint32_t mode=state->cpsr&31u;for(unsigned i=0;i<7;++i)banks->exception_r8_r14[i]=r[8+i];if(mode==0x11u){for(unsigned i=0;i<7;++i)r[8+i]=banks->svc_r8_r14[i];}else if(mode!=0x13u){r[13]=banks->svc_r8_r14[5];r[14]=banks->svc_r8_r14[6];}for(unsigned i=0;i<7;++i)banks->svc_r8_r14[i]=r[8+i];state->cpsr=banks->spsr;r[15]=target&~3u;banks->returned=1u;return 0;}
 default:*handled=0;return 0;
 }}else{switch(r[15]){
        case 0x4u: /* b 0x00000234 */
            r[15]=564u;return 0;
        case 0x8u: /* b 0x00000234 */
            r[15]=564u;return 0;
        case 0xcu: /* b 0x00000234 */
            r[15]=564u;return 0;
        case 0x10u: /* b 0x00000234 */
            r[15]=564u;return 0;
        case 0x14u: /* b 0x00000234 */
            r[15]=564u;return 0;
        case 0x18u: /* b 0x00000234 */
            r[15]=564u;return 0;
        case 0x1cu: /* b 0x00000248 */
            r[15]=584u;return 0;
        case 0x234u: /* sub lr,lr,#0x4 */
            r[14]=r[14]-4u;r[15]=0x238u;return 0;
        case 0x238u: /* adr sp,0x244 */
            r[13]=580u;r[15]=0x23cu;return 0;
        case 0x23cu: /* str lr,[sp,#0x0] */
            STORE(r[13],r[14],4);r[15]=0x240u;return 0;
        case 0x240u: /* ldmia sp,{pc}^ */
            {uint32_t target;LOAD(target,r[13],4);uint32_t mode=state->cpsr&31u;for(unsigned i=0;i<7;++i)banks->exception_r8_r14[i]=r[8+i];if(mode==0x11u){for(unsigned i=0;i<7;++i)r[8+i]=banks->svc_r8_r14[i];}else if(mode!=0x13u){r[13]=banks->svc_r8_r14[5];r[14]=banks->svc_r8_r14[6];}for(unsigned i=0;i<7;++i)banks->svc_r8_r14[i]=r[8+i];state->cpsr=banks->spsr;r[15]=target&~3u;banks->returned=1u;return 0;}
        case 0x248u: /* sub lr,lr,#0x4 */
            r[14]=r[14]-4u;r[15]=0x24cu;return 0;
        case 0x24cu: /* adr sp,0x3cc */
            r[13]=972u;r[15]=0x250u;return 0;
        case 0x250u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x254u;return 0;
        case 0x254u: /* mov r11,#0x0 */
            r[11]=0u;r[15]=0x258u;return 0;
        case 0x258u: /* ldr r8,[0x8fc] */
            LOAD(r[8],2300u,4);r[15]=0x25cu;return 0;
        case 0x25cu: /* ldr r9,[r8,#0x0] */
            LOAD(r[9],r[8]+0u,4);r[15]=0x260u;return 0;
        case 0x260u: /* add r9,r9,#0x1 */
            r[9]=r[9]+1u;r[15]=0x264u;return 0;
        case 0x264u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x268u;return 0;
        case 0x268u: /* ldr r8,[0x900] */
            LOAD(r[8],2304u,4);r[15]=0x26cu;return 0;
        case 0x26cu: /* ldr r10,[r8,#0x0] */
            LOAD(r[10],r[8]+0u,4);r[15]=0x270u;return 0;
        case 0x270u: /* and r10,r10,#0x7 */
            r[10]=r[10]&7u;r[15]=0x274u;return 0;
        case 0x274u: /* mov r10,r10, lsl #0x2 */
            r[10]=(r[10]<<2);r[15]=0x278u;return 0;
        case 0x278u: /* adr r8,0x284 */
            r[8]=644u;r[15]=0x27cu;return 0;
        case 0x27cu: /* add r8,r8,r10 */
            r[8]=r[8]+r[10];r[15]=0x280u;return 0;
        case 0x280u: /* mov pc,r8 */
            r[15]=r[8];return 0;
        case 0x284u: /* b 0x000002a4 */
            r[15]=676u;return 0;
        case 0x288u: /* b 0x000002a4 */
            r[15]=676u;return 0;
        case 0x28cu: /* b 0x000002a8 */
            r[15]=680u;return 0;
        case 0x290u: /* b 0x000002a4 */
            r[15]=676u;return 0;
        case 0x294u: /* b 0x000002a4 */
            r[15]=676u;return 0;
        case 0x298u: /* b 0x00000328 */
            r[15]=808u;return 0;
        case 0x29cu: /* b 0x000002a4 */
            r[15]=676u;return 0;
        case 0x2a0u: /* b 0x000002a4 */
            r[15]=676u;return 0;
        case 0x2a4u: /* b 0x0000034c */
            r[15]=844u;return 0;
        case 0x2a8u: /* ldr r8,[0x904] */
            LOAD(r[8],2308u,4);r[15]=0x2acu;return 0;
        case 0x2acu: /* ldr r10,[0x31c] */
            LOAD(r[10],796u,4);r[15]=0x2b0u;return 0;
        case 0x2b0u: /* str r10,[r8,#0x0] */
            STORE(r[8]+0u,r[10],4);r[15]=0x2b4u;return 0;
        case 0x2b4u: /* mov r9,#0x40 */
            r[9]=64u;r[15]=0x2b8u;return 0;
        case 0x2b8u: /* ldr r8,[0x908] */
            LOAD(r[8],2312u,4);r[15]=0x2bcu;return 0;
        case 0x2bcu: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x2c0u;return 0;
        case 0x2c0u: /* mov r9,#0xff */
            r[9]=255u;r[15]=0x2c4u;return 0;
        case 0x2c4u: /* strb r9,[0x22e] */
            STORE(558u,r[9],1);r[15]=0x2c8u;return 0;
        case 0x2c8u: /* ldr r9,[0x320] */
            LOAD(r[9],800u,4);r[15]=0x2ccu;return 0;
        case 0x2ccu: /* subs r9,r9,#0x1 */
            r[9]=subtract(state,r[9],1u);r[15]=0x2d0u;return 0;
        case 0x2d0u: /* ldreq r9,[0x324] */
            if(state->cpsr&0x40000000u){LOAD(r[9],804u,4);}r[15]=0x2d4u;return 0;
        case 0x2d4u: /* str r9,[0x320] */
            STORE(800u,r[9],4);r[15]=0x2d8u;return 0;
        case 0x2d8u: /* beq 0x0000034c */
            if(state->cpsr&0x40000000u){r[15]=844u;}else{r[15]=0x2dcu;}return 0;
        case 0x2dcu: /* ldr r8,[0x90c] */
            LOAD(r[8],2316u,4);r[15]=0x2e0u;return 0;
        case 0x2e0u: /* ldrb r9,[r8,#0x0] */
            LOAD(r[9],r[8]+0u,1);r[15]=0x2e4u;return 0;
        case 0x2e4u: /* add r9,r9,#0x1 */
            r[9]=r[9]+1u;r[15]=0x2e8u;return 0;
        case 0x2e8u: /* ands r9,r9,#0x3 */
            temporary=r[9]&3u;r[9]=temporary;logic_flags(state,temporary,0,3u);r[15]=0x2ecu;return 0;
        case 0x2ecu: /* strb r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],1);r[15]=0x2f0u;return 0;
        case 0x2f0u: /* bne 0x0000034c */
            if(!(state->cpsr&0x40000000u)){r[15]=844u;}else{r[15]=0x2f4u;}return 0;
        case 0x2f4u: /* ldr r8,[0x910] */
            LOAD(r[8],2320u,4);r[15]=0x2f8u;return 0;
        case 0x2f8u: /* ldr r9,[r8,#0x0] */
            LOAD(r[9],r[8]+0u,4);r[15]=0x2fcu;return 0;
        case 0x2fcu: /* add r9,r9,#0x1 */
            r[9]=r[9]+1u;r[15]=0x300u;return 0;
        case 0x300u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x304u;return 0;
        case 0x304u: /* mov r9,#0xff */
            r[9]=255u;r[15]=0x308u;return 0;
        case 0x308u: /* strb r9,[0x22c] */
            STORE(556u,r[9],1);r[15]=0x30cu;return 0;
        case 0x30cu: /* mov r9,#0x0 */
            r[9]=0u;r[15]=0x310u;return 0;
        case 0x310u: /* strb r9,[0x22e] */
            STORE(558u,r[9],1);r[15]=0x314u;return 0;
        case 0x314u: /* b 0x0000034c */
            r[15]=844u;return 0;
        case 0x328u: /* ldr r8,[0x914] */
            LOAD(r[8],2324u,4);r[15]=0x32cu;return 0;
        case 0x32cu: /* ldr r9,[r8,#0x0] */
            LOAD(r[9],r[8]+0u,4);r[15]=0x330u;return 0;
        case 0x330u: /* add r9,r9,#0x1 */
            r[9]=r[9]+1u;r[15]=0x334u;return 0;
        case 0x334u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x338u;return 0;
        case 0x338u: /* mvn r9,#0x0 */
            r[9]=~0u;r[15]=0x33cu;return 0;
        case 0x33cu: /* str r9,[0xfc] */
            STORE(252u,r[9],4);r[15]=0x340u;return 0;
        case 0x340u: /* stmdb sp!,{r0,r1,r2,r3,r4,r5,r6,r7} */
            STORE(r[13]-32u,r[0],4);STORE(r[13]-28u,r[1],4);STORE(r[13]-24u,r[2],4);STORE(r[13]-20u,r[3],4);STORE(r[13]-16u,r[4],4);STORE(r[13]-12u,r[5],4);STORE(r[13]-8u,r[6],4);STORE(r[13]-4u,r[7],4);r[13]-=32u;r[15]=0x344u;return 0;
        case 0x344u: /* bl 0x000015a4 */
            r[14]=0x348u;r[15]=5540u;return 0;
        case 0x348u: /* ldmia sp!,{r0,r1,r2,r3,r4,r5,r6,r7} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[1],r[13]+4u,4);LOAD(r[2],r[13]+8u,4);LOAD(r[3],r[13]+12u,4);LOAD(r[4],r[13]+16u,4);LOAD(r[5],r[13]+20u,4);LOAD(r[6],r[13]+24u,4);LOAD(r[7],r[13]+28u,4);r[13]+=32u;r[15]=0x34cu;return 0;
        case 0x34cu: /* ldr r8,[0x918] */
            LOAD(r[8],2328u,4);r[15]=0x350u;return 0;
        case 0x350u: /* mov r9,#0x1 */
            r[9]=1u;r[15]=0x354u;return 0;
        case 0x354u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x358u;return 0;
        case 0x358u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x35cu;return 0;
        case 0x35cu: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x360u;return 0;
        case 0x360u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x364u;return 0;
        case 0x364u: /* ldmia sp!,{pc}^ */
            {uint32_t target;LOAD(target,r[13],4);r[13]+=4u;uint32_t mode=state->cpsr&31u;for(unsigned i=0;i<7;++i)banks->exception_r8_r14[i]=r[8+i];if(mode==0x11u){for(unsigned i=0;i<7;++i)r[8+i]=banks->svc_r8_r14[i];}else if(mode!=0x13u){r[13]=banks->svc_r8_r14[5];r[14]=banks->svc_r8_r14[6];}for(unsigned i=0;i<7;++i)banks->svc_r8_r14[i]=r[8+i];state->cpsr=banks->spsr;r[15]=target&~3u;banks->returned=1u;return 0;}
 default:*handled=0;return 0;
 }}
}

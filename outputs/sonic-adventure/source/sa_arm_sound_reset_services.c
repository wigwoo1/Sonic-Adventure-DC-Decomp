/* Static native C recovery of the remaining reset callees and their dependencies.
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
int sa_arm_sound_reset_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
        case 0x5d54u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5d58u;return 0;
        case 0x5d58u: /* bl 0x000062b0 */
            r[14]=0x5d5cu;r[15]=25264u;return 0;
        case 0x5d5cu: /* bl 0x00005d78 */
            r[14]=0x5d60u;r[15]=23928u;return 0;
        case 0x5d60u: /* bl 0x00006274 */
            r[14]=0x5d64u;r[15]=25204u;return 0;
        case 0x5d64u: /* bl 0x00006310 */
            r[14]=0x5d68u;r[15]=25360u;return 0;
        case 0x5d68u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13],4);r[13]+=4u;r[15]=0x5d6cu;return 0;
        case 0x5d6cu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5d70u;return 0;
        case 0x5d70u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5d78u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5d7cu;return 0;
        case 0x5d7cu: /* mov r1,#0x800000 */
            r[1]=8388608u;r[15]=0x5d80u;return 0;
        case 0x5d80u: /* add r1,r1,#0x20 */
            r[1]=r[1]+32u;r[15]=0x5d84u;return 0;
        case 0x5d84u: /* mov r2,#0x40 */
            r[2]=64u;r[15]=0x5d88u;return 0;
        case 0x5d88u: /* str r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],4);r[15]=0x5d8cu;return 0;
        case 0x5d8cu: /* add r1,r1,#0x80 */
            r[1]=r[1]+128u;r[15]=0x5d90u;return 0;
        case 0x5d90u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x5d94u;return 0;
        case 0x5d94u: /* bne 0x00005d88 */
            r[15]=0x5d98u;if(!(state->cpsr&0x40000000u)){r[15]=23944u;}return 0;
        case 0x5d98u: /* ldr r1,[0x6130] */
            LOAD(r[1],24880u,4);r[15]=0x5d9cu;return 0;
        case 0x5d9cu: /* mov r2,#0x200 */
            r[2]=512u;r[15]=0x5da0u;return 0;
        case 0x5da0u: /* str r0,[r1],#0x4 */
            STORE(r[1],r[0],4);r[1]+=4u;r[15]=0x5da4u;return 0;
        case 0x5da4u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x5da8u;return 0;
        case 0x5da8u: /* bne 0x00005da0 */
            r[15]=0x5dacu;if(!(state->cpsr&0x40000000u)){r[15]=23968u;}return 0;
        case 0x5dacu: /* ldr r1,[0x6134] */
            LOAD(r[1],24884u,4);r[15]=0x5db0u;return 0;
        case 0x5db0u: /* mov r2,#0x80 */
            r[2]=128u;r[15]=0x5db4u;return 0;
        case 0x5db4u: /* str r0,[r1],#0x4 */
            STORE(r[1],r[0],4);r[1]+=4u;r[15]=0x5db8u;return 0;
        case 0x5db8u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x5dbcu;return 0;
        case 0x5dbcu: /* bne 0x00005db4 */
            r[15]=0x5dc0u;if(!(state->cpsr&0x40000000u)){r[15]=23988u;}return 0;
        case 0x5dc0u: /* ldr r1,[0x6138] */
            LOAD(r[1],24888u,4);r[15]=0x5dc4u;return 0;
        case 0x5dc4u: /* mov r2,#0x40 */
            r[2]=64u;r[15]=0x5dc8u;return 0;
        case 0x5dc8u: /* str r0,[r1],#0x4 */
            STORE(r[1],r[0],4);r[1]+=4u;r[15]=0x5dccu;return 0;
        case 0x5dccu: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x5dd0u;return 0;
        case 0x5dd0u: /* bne 0x00005dc8 */
            r[15]=0x5dd4u;if(!(state->cpsr&0x40000000u)){r[15]=24008u;}return 0;
        case 0x5dd4u: /* ldr r1,[0x613c] */
            LOAD(r[1],24892u,4);r[15]=0x5dd8u;return 0;
        case 0x5dd8u: /* mov r2,#0x100 */
            r[2]=256u;r[15]=0x5ddcu;return 0;
        case 0x5ddcu: /* str r0,[r1],#0x4 */
            STORE(r[1],r[0],4);r[1]+=4u;r[15]=0x5de0u;return 0;
        case 0x5de0u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x5de4u;return 0;
        case 0x5de4u: /* bne 0x00005ddc */
            r[15]=0x5de8u;if(!(state->cpsr&0x40000000u)){r[15]=24028u;}return 0;
        case 0x5de8u: /* ldr r1,[0x6140] */
            LOAD(r[1],24896u,4);r[15]=0x5decu;return 0;
        case 0x5decu: /* mov r0,#0xff */
            r[0]=255u;r[15]=0x5df0u;return 0;
        case 0x5df0u: /* strb r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],1);r[15]=0x5df4u;return 0;
        case 0x5df4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6274u: /* ldr r0,[0x6120] */
            LOAD(r[0],24864u,4);r[15]=0x6278u;return 0;
        case 0x6278u: /* ldr r1,[r0,#0x4] */
            LOAD(r[1],r[0]+4u,4);r[15]=0x627cu;return 0;
        case 0x627cu: /* ldr r0,[r0,#0x0] */
            LOAD(r[0],r[0]+0u,4);r[15]=0x6280u;return 0;
        case 0x6280u: /* ldr r2,[0x62ac] */
            LOAD(r[2],25260u,4);r[15]=0x6284u;return 0;
        case 0x6284u: /* tst r0,r2 */
            temporary=r[0]&r[2];logic_flags(state,temporary,0,0);r[15]=0x6288u;return 0;
        case 0x6288u: /* tsteq r1,r2 */
            if(state->cpsr&0x40000000u){temporary=r[1]&r[2];logic_flags(state,temporary,0,0);}r[15]=0x628cu;return 0;
        case 0x628cu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x6290u;}return 0;
        case 0x6290u: /* mov r1,r1, lsr #0x2 */
            r[1]=r[1]>>2;r[15]=0x6294u;return 0;
        case 0x6294u: /* ldr r2,[0x62a8] */
            LOAD(r[2],25256u,4);r[15]=0x6298u;return 0;
        case 0x6298u: /* str r2,[r0],#0x4 */
            STORE(r[0],r[2],4);r[0]+=4u;r[15]=0x629cu;return 0;
        case 0x629cu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x62a0u;return 0;
        case 0x62a0u: /* bne 0x00006298 */
            r[15]=0x62a4u;if(!(state->cpsr&0x40000000u)){r[15]=25240u;}return 0;
        case 0x62a4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x62b0u: /* ldr r8,[0x60f4] */
            LOAD(r[8],24820u,4);r[15]=0x62b4u;return 0;
        case 0x62b4u: /* adr r0,0x637c */
            r[0]=25468u;r[15]=0x62b8u;return 0;
        case 0x62b8u: /* mov r1,#0x10 */
            r[1]=16u;r[15]=0x62bcu;return 0;
        case 0x62bcu: /* ldr r2,[r8],#0x4 */
            LOAD(r[2],r[8],4);r[8]+=4u;r[15]=0x62c0u;return 0;
        case 0x62c0u: /* str r2,[r0],#0x4 */
            STORE(r[0],r[2],4);r[0]+=4u;r[15]=0x62c4u;return 0;
        case 0x62c4u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x62c8u;return 0;
        case 0x62c8u: /* bne 0x000062bc */
            r[15]=0x62ccu;if(!(state->cpsr&0x40000000u)){r[15]=25276u;}return 0;
        case 0x62ccu: /* mov r3,#0x10 */
            r[3]=16u;r[15]=0x62d0u;return 0;
        case 0x62d0u: /* ldr r8,[0x60f4] */
            LOAD(r[8],24820u,4);r[15]=0x62d4u;return 0;
        case 0x62d4u: /* mov r1,#0x10 */
            r[1]=16u;r[15]=0x62d8u;return 0;
        case 0x62d8u: /* ldrb r2,[r8,#0x1] */
            LOAD(r[2],r[8]+1u,1);r[15]=0x62dcu;return 0;
        case 0x62dcu: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x62e0u;return 0;
        case 0x62e0u: /* bmi 0x000062e8 */
            r[15]=0x62e4u;if(state->cpsr&0x80000000u){r[15]=25320u;}return 0;
        case 0x62e4u: /* strb r2,[r8,#0x1] */
            STORE(r[8]+1u,r[2],1);r[15]=0x62e8u;return 0;
        case 0x62e8u: /* add r8,r8,#0x4 */
            r[8]=r[8]+4u;r[15]=0x62ecu;return 0;
        case 0x62ecu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x62f0u;return 0;
        case 0x62f0u: /* bne 0x000062d8 */
            r[15]=0x62f4u;if(!(state->cpsr&0x40000000u)){r[15]=25304u;}return 0;
        case 0x62f4u: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x62f8u;return 0;
        case 0x62f8u: /* mov r0,r0 */
            r[0]=r[0];r[15]=0x62fcu;return 0;
        case 0x62fcu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x6300u;return 0;
        case 0x6300u: /* bne 0x000062f8 */
            r[15]=0x6304u;if(!(state->cpsr&0x40000000u)){r[15]=25336u;}return 0;
        case 0x6304u: /* subs r3,r3,#0x1 */
            r[3]=subtract(state,r[3],1u);r[15]=0x6308u;return 0;
        case 0x6308u: /* bne 0x000062d0 */
            r[15]=0x630cu;if(!(state->cpsr&0x40000000u)){r[15]=25296u;}return 0;
        case 0x630cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6310u: /* ldr r8,[0x60f4] */
            LOAD(r[8],24820u,4);r[15]=0x6314u;return 0;
        case 0x6314u: /* adr r0,0x637c */
            r[0]=25468u;r[15]=0x6318u;return 0;
        case 0x6318u: /* mov r1,#0x10 */
            r[1]=16u;r[15]=0x631cu;return 0;
        case 0x631cu: /* ldrb r2,[r0],#0x4 */
            LOAD(r[2],r[0],1);r[0]+=4u;r[15]=0x6320u;return 0;
        case 0x6320u: /* strb r2,[r8],#0x4 */
            STORE(r[8],r[2],1);r[8]+=4u;r[15]=0x6324u;return 0;
        case 0x6324u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x6328u;return 0;
        case 0x6328u: /* bne 0x0000631c */
            r[15]=0x632cu;if(!(state->cpsr&0x40000000u)){r[15]=25372u;}return 0;
        case 0x632cu: /* mov r3,#0x0 */
            r[3]=0u;r[15]=0x6330u;return 0;
        case 0x6330u: /* mov r4,#0x10 */
            r[4]=16u;r[15]=0x6334u;return 0;
        case 0x6334u: /* ldr r8,[0x60f4] */
            LOAD(r[8],24820u,4);r[15]=0x6338u;return 0;
        case 0x6338u: /* adr r0,0x637c */
            r[0]=25468u;r[15]=0x633cu;return 0;
        case 0x633cu: /* mov r1,#0x10 */
            r[1]=16u;r[15]=0x6340u;return 0;
        case 0x6340u: /* ldrb r2,[r0,#0x1] */
            LOAD(r[2],r[0]+1u,1);r[15]=0x6344u;return 0;
        case 0x6344u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x6348u;return 0;
        case 0x6348u: /* strbpl r3,[r8,#0x1] */
            if(!(state->cpsr&0x80000000u)){STORE(r[8]+1u,r[3],1);}r[15]=0x634cu;return 0;
        case 0x634cu: /* add r8,r8,#0x4 */
            r[8]=r[8]+4u;r[15]=0x6350u;return 0;
        case 0x6350u: /* add r0,r0,#0x4 */
            r[0]=r[0]+4u;r[15]=0x6354u;return 0;
        case 0x6354u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x6358u;return 0;
        case 0x6358u: /* bne 0x00006340 */
            r[15]=0x635cu;if(!(state->cpsr&0x40000000u)){r[15]=25408u;}return 0;
        case 0x635cu: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x6360u;return 0;
        case 0x6360u: /* mov r0,r0 */
            r[0]=r[0];r[15]=0x6364u;return 0;
        case 0x6364u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x6368u;return 0;
        case 0x6368u: /* bne 0x00006360 */
            r[15]=0x636cu;if(!(state->cpsr&0x40000000u)){r[15]=25440u;}return 0;
        case 0x636cu: /* add r3,r3,#0x1 */
            r[3]=r[3]+1u;r[15]=0x6370u;return 0;
        case 0x6370u: /* subs r4,r4,#0x1 */
            r[4]=subtract(state,r[4],1u);r[15]=0x6374u;return 0;
        case 0x6374u: /* bne 0x00006334 */
            r[15]=0x6378u;if(!(state->cpsr&0x40000000u)){r[15]=25396u;}return 0;
        case 0x6378u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x73b4u: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x73b8u;return 0;
        case 0x73b8u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x73bcu;return 0;
        case 0x73bcu: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x73c0u;return 0;
        case 0x73c0u: /* str r1,[r11,r0] */
            STORE(r[11]+r[0],r[1],4);r[15]=0x73c4u;return 0;
        case 0x73c4u: /* add r0,r0,#0x4 */
            r[0]=r[0]+4u;r[15]=0x73c8u;return 0;
        case 0x73c8u: /* cmp r0,#0x800 */
            (void)subtract(state,r[0],2048u);r[15]=0x73ccu;return 0;
        case 0x73ccu: /* bne 0x000073c0 */
            r[15]=0x73d0u;if(!(state->cpsr&0x40000000u)){r[15]=29632u;}return 0;
        case 0x73d0u: /* ldr r0,[0x76e8] */
            LOAD(r[0],30440u,4);r[15]=0x73d4u;return 0;
        case 0x73d4u: /* mov r1,#0x4000 */
            r[1]=16384u;r[15]=0x73d8u;return 0;
        case 0x73d8u: /* str r1,[r11,#0x3c] */
            STORE(r[11]+60u,r[1],4);r[15]=0x73dcu;return 0;
        case 0x73dcu: /* add r11,r11,#0x40 */
            r[11]=r[11]+64u;r[15]=0x73e0u;return 0;
        case 0x73e0u: /* cmp r11,r0 */
            (void)subtract(state,r[11],r[0]);r[15]=0x73e4u;return 0;
        case 0x73e4u: /* bne 0x000073d8 */
            r[15]=0x73e8u;if(!(state->cpsr&0x40000000u)){r[15]=29656u;}return 0;
        case 0x73e8u: /* ldr r9,[0x76e8] */
            LOAD(r[9],30440u,4);r[15]=0x73ecu;return 0;
        case 0x73ecu: /* ldr r0,[0x76ec] */
            LOAD(r[0],30444u,4);r[15]=0x73f0u;return 0;
        case 0x73f0u: /* str r0,[r9,#0x8] */
            STORE(r[9]+8u,r[0],4);r[15]=0x73f4u;return 0;
        case 0x73f4u: /* str r0,[r9,#0xc] */
            STORE(r[9]+12u,r[0],4);r[15]=0x73f8u;return 0;
        case 0x73f8u: /* mov r0,#0x10000 */
            r[0]=65536u;r[15]=0x73fcu;return 0;
        case 0x73fcu: /* str r0,[r9,#0x4] */
            STORE(r[9]+4u,r[0],4);r[15]=0x7400u;return 0;
        case 0x7400u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7700u: /* ldr r12,[0x7804] */
            LOAD(r[12],30724u,4);r[15]=0x7704u;return 0;
        case 0x7704u: /* mov r1,#0x8 */
            r[1]=8u;r[15]=0x7708u;return 0;
        case 0x7708u: /* ldr r9,[0x77f4] */
            LOAD(r[9],30708u,4);r[15]=0x770cu;return 0;
        case 0x770cu: /* mov r2,#0x58 */
            r[2]=88u;r[15]=0x7710u;return 0;
        case 0x7710u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7714u;return 0;
        case 0x7714u: /* str r6,[r12,r2] */
            STORE(r[12]+r[2],r[6],4);r[15]=0x7718u;return 0;
        case 0x7718u: /* subs r2,r2,#0x4 */
            r[2]=subtract(state,r[2],4u);r[15]=0x771cu;return 0;
        case 0x771cu: /* bmi 0x00007724 */
            r[15]=0x7720u;if(state->cpsr&0x80000000u){r[15]=30500u;}return 0;
        case 0x7720u: /* b 0x00007714 */
            r[15]=30484u;return 0;
        case 0x7724u: /* str r9,[r12,#0x18] */
            STORE(r[12]+24u,r[9],4);r[15]=0x7728u;return 0;
        case 0x7728u: /* strb r9,[r12,#0x1f] */
            STORE(r[12]+31u,r[9],1);r[15]=0x772cu;return 0;
        case 0x772cu: /* strb r9,[r12,#0x1e] */
            STORE(r[12]+30u,r[9],1);r[15]=0x7730u;return 0;
        case 0x7730u: /* strb r9,[r12,#0x29] */
            STORE(r[12]+41u,r[9],1);r[15]=0x7734u;return 0;
        case 0x7734u: /* strb r9,[r12,#0x41] */
            STORE(r[12]+65u,r[9],1);r[15]=0x7738u;return 0;
        case 0x7738u: /* strb r9,[r12,#0x45] */
            STORE(r[12]+69u,r[9],1);r[15]=0x773cu;return 0;
        case 0x773cu: /* strb r9,[r12,#0x4d] */
            STORE(r[12]+77u,r[9],1);r[15]=0x7740u;return 0;
        case 0x7740u: /* strb r9,[r12,#0x51] */
            STORE(r[12]+81u,r[9],1);r[15]=0x7744u;return 0;
        case 0x7744u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x7748u;return 0;
        case 0x7748u: /* mov r2,#0x58 */
            r[2]=88u;r[15]=0x774cu;return 0;
        case 0x774cu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x7750u;return 0;
        case 0x7750u: /* bne 0x00007714 */
            r[15]=0x7754u;if(!(state->cpsr&0x40000000u)){r[15]=30484u;}return 0;
        case 0x7754u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7758u;return 0;
        case 0x7758u: /* mov r1,#0x10 */
            r[1]=16u;r[15]=0x775cu;return 0;
        case 0x775cu: /* mov r4,#0x1f */
            r[4]=31u;r[15]=0x7760u;return 0;
        case 0x7760u: /* mov r5,#0xf */
            r[5]=15u;r[15]=0x7764u;return 0;
        case 0x7764u: /* str r6,[r12,r2] */
            STORE(r[12]+r[2],r[6],4);r[15]=0x7768u;return 0;
        case 0x7768u: /* subs r2,r2,#0x4 */
            r[2]=subtract(state,r[2],4u);r[15]=0x776cu;return 0;
        case 0x776cu: /* bmi 0x00007774 */
            r[15]=0x7770u;if(state->cpsr&0x80000000u){r[15]=30580u;}return 0;
        case 0x7770u: /* b 0x00007764 */
            r[15]=30564u;return 0;
        case 0x7774u: /* strb r4,[r12,#0x28] */
            STORE(r[12]+40u,r[4],1);r[15]=0x7778u;return 0;
        case 0x7778u: /* strb r5,[r12,#0x2a] */
            STORE(r[12]+42u,r[5],1);r[15]=0x777cu;return 0;
        case 0x777cu: /* str r9,[r12,#0x18] */
            STORE(r[12]+24u,r[9],4);r[15]=0x7780u;return 0;
        case 0x7780u: /* strb r9,[r12,#0x1f] */
            STORE(r[12]+31u,r[9],1);r[15]=0x7784u;return 0;
        case 0x7784u: /* strb r9,[r12,#0x1e] */
            STORE(r[12]+30u,r[9],1);r[15]=0x7788u;return 0;
        case 0x7788u: /* strb r9,[r12,#0x29] */
            STORE(r[12]+41u,r[9],1);r[15]=0x778cu;return 0;
        case 0x778cu: /* strb r9,[r12,#0x41] */
            STORE(r[12]+65u,r[9],1);r[15]=0x7790u;return 0;
        case 0x7790u: /* strb r9,[r12,#0x45] */
            STORE(r[12]+69u,r[9],1);r[15]=0x7794u;return 0;
        case 0x7794u: /* strb r9,[r12,#0x4d] */
            STORE(r[12]+77u,r[9],1);r[15]=0x7798u;return 0;
        case 0x7798u: /* strb r9,[r12,#0x51] */
            STORE(r[12]+81u,r[9],1);r[15]=0x779cu;return 0;
        case 0x779cu: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x77a0u;return 0;
        case 0x77a0u: /* mov r2,#0x58 */
            r[2]=88u;r[15]=0x77a4u;return 0;
        case 0x77a4u: /* eor r4,r4,#0x10 */
            r[4]=r[4]^16u;r[15]=0x77a8u;return 0;
        case 0x77a8u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x77acu;return 0;
        case 0x77acu: /* bne 0x00007764 */
            r[15]=0x77b0u;if(!(state->cpsr&0x40000000u)){r[15]=30564u;}return 0;
        case 0x77b0u: /* ldr r10,[0x77e4] */
            LOAD(r[10],30692u,4);r[15]=0x77b4u;return 0;
        case 0x77b4u: /* ldr r2,[0x77e0] */
            LOAD(r[2],30688u,4);r[15]=0x77b8u;return 0;
        case 0x77b8u: /* mov r1,#0x10 */
            r[1]=16u;r[15]=0x77bcu;return 0;
        case 0x77bcu: /* str r6,[r10,#0x0] */
            STORE(r[10]+0u,r[6],4);r[15]=0x77c0u;return 0;
        case 0x77c0u: /* str r6,[r10,#0x4] */
            STORE(r[10]+4u,r[6],4);r[15]=0x77c4u;return 0;
        case 0x77c4u: /* str r2,[r10,#0x8] */
            STORE(r[10]+8u,r[2],4);r[15]=0x77c8u;return 0;
        case 0x77c8u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x77ccu;return 0;
        case 0x77ccu: /* ldmiaeq sp!,{lr} */
            if(state->cpsr&0x40000000u){LOAD(r[14],r[13],4);r[13]+=4u;}r[15]=0x77d0u;return 0;
        case 0x77d0u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x77d4u;}return 0;
        case 0x77d4u: /* add r10,r10,#0xc */
            r[10]=r[10]+12u;r[15]=0x77d8u;return 0;
        case 0x77d8u: /* add r2,r2,#0x80 */
            r[2]=r[2]+128u;r[15]=0x77dcu;return 0;
        case 0x77dcu: /* b 0x000077bc */
            r[15]=30652u;return 0;
        default:*handled=0;return 0;
 }}else{switch(r[15]){
        case 0x5d5cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5d60u;return 0;
        case 0x5d60u: /* bl 0x000062b8 */
            r[14]=0x5d64u;r[15]=25272u;return 0;
        case 0x5d64u: /* bl 0x00005d80 */
            r[14]=0x5d68u;r[15]=23936u;return 0;
        case 0x5d68u: /* bl 0x0000627c */
            r[14]=0x5d6cu;r[15]=25212u;return 0;
        case 0x5d6cu: /* bl 0x00006318 */
            r[14]=0x5d70u;r[15]=25368u;return 0;
        case 0x5d70u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13],4);r[13]+=4u;r[15]=0x5d74u;return 0;
        case 0x5d74u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5d78u;return 0;
        case 0x5d78u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5d80u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5d84u;return 0;
        case 0x5d84u: /* mov r1,#0x800000 */
            r[1]=8388608u;r[15]=0x5d88u;return 0;
        case 0x5d88u: /* add r1,r1,#0x20 */
            r[1]=r[1]+32u;r[15]=0x5d8cu;return 0;
        case 0x5d8cu: /* mov r2,#0x40 */
            r[2]=64u;r[15]=0x5d90u;return 0;
        case 0x5d90u: /* str r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],4);r[15]=0x5d94u;return 0;
        case 0x5d94u: /* add r1,r1,#0x80 */
            r[1]=r[1]+128u;r[15]=0x5d98u;return 0;
        case 0x5d98u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x5d9cu;return 0;
        case 0x5d9cu: /* bne 0x00005d90 */
            r[15]=0x5da0u;if(!(state->cpsr&0x40000000u)){r[15]=23952u;}return 0;
        case 0x5da0u: /* ldr r1,[0x6138] */
            LOAD(r[1],24888u,4);r[15]=0x5da4u;return 0;
        case 0x5da4u: /* mov r2,#0x200 */
            r[2]=512u;r[15]=0x5da8u;return 0;
        case 0x5da8u: /* str r0,[r1],#0x4 */
            STORE(r[1],r[0],4);r[1]+=4u;r[15]=0x5dacu;return 0;
        case 0x5dacu: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x5db0u;return 0;
        case 0x5db0u: /* bne 0x00005da8 */
            r[15]=0x5db4u;if(!(state->cpsr&0x40000000u)){r[15]=23976u;}return 0;
        case 0x5db4u: /* ldr r1,[0x613c] */
            LOAD(r[1],24892u,4);r[15]=0x5db8u;return 0;
        case 0x5db8u: /* mov r2,#0x80 */
            r[2]=128u;r[15]=0x5dbcu;return 0;
        case 0x5dbcu: /* str r0,[r1],#0x4 */
            STORE(r[1],r[0],4);r[1]+=4u;r[15]=0x5dc0u;return 0;
        case 0x5dc0u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x5dc4u;return 0;
        case 0x5dc4u: /* bne 0x00005dbc */
            r[15]=0x5dc8u;if(!(state->cpsr&0x40000000u)){r[15]=23996u;}return 0;
        case 0x5dc8u: /* ldr r1,[0x6140] */
            LOAD(r[1],24896u,4);r[15]=0x5dccu;return 0;
        case 0x5dccu: /* mov r2,#0x40 */
            r[2]=64u;r[15]=0x5dd0u;return 0;
        case 0x5dd0u: /* str r0,[r1],#0x4 */
            STORE(r[1],r[0],4);r[1]+=4u;r[15]=0x5dd4u;return 0;
        case 0x5dd4u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x5dd8u;return 0;
        case 0x5dd8u: /* bne 0x00005dd0 */
            r[15]=0x5ddcu;if(!(state->cpsr&0x40000000u)){r[15]=24016u;}return 0;
        case 0x5ddcu: /* ldr r1,[0x6144] */
            LOAD(r[1],24900u,4);r[15]=0x5de0u;return 0;
        case 0x5de0u: /* mov r2,#0x100 */
            r[2]=256u;r[15]=0x5de4u;return 0;
        case 0x5de4u: /* str r0,[r1],#0x4 */
            STORE(r[1],r[0],4);r[1]+=4u;r[15]=0x5de8u;return 0;
        case 0x5de8u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x5decu;return 0;
        case 0x5decu: /* bne 0x00005de4 */
            r[15]=0x5df0u;if(!(state->cpsr&0x40000000u)){r[15]=24036u;}return 0;
        case 0x5df0u: /* ldr r1,[0x6148] */
            LOAD(r[1],24904u,4);r[15]=0x5df4u;return 0;
        case 0x5df4u: /* mov r0,#0xff */
            r[0]=255u;r[15]=0x5df8u;return 0;
        case 0x5df8u: /* strb r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],1);r[15]=0x5dfcu;return 0;
        case 0x5dfcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x627cu: /* ldr r0,[0x6128] */
            LOAD(r[0],24872u,4);r[15]=0x6280u;return 0;
        case 0x6280u: /* ldr r1,[r0,#0x4] */
            LOAD(r[1],r[0]+4u,4);r[15]=0x6284u;return 0;
        case 0x6284u: /* ldr r0,[r0,#0x0] */
            LOAD(r[0],r[0]+0u,4);r[15]=0x6288u;return 0;
        case 0x6288u: /* ldr r2,[0x62b4] */
            LOAD(r[2],25268u,4);r[15]=0x628cu;return 0;
        case 0x628cu: /* tst r0,r2 */
            temporary=r[0]&r[2];logic_flags(state,temporary,0,0);r[15]=0x6290u;return 0;
        case 0x6290u: /* tsteq r1,r2 */
            if(state->cpsr&0x40000000u){temporary=r[1]&r[2];logic_flags(state,temporary,0,0);}r[15]=0x6294u;return 0;
        case 0x6294u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x6298u;}return 0;
        case 0x6298u: /* mov r1,r1, lsr #0x2 */
            r[1]=r[1]>>2;r[15]=0x629cu;return 0;
        case 0x629cu: /* ldr r2,[0x62b0] */
            LOAD(r[2],25264u,4);r[15]=0x62a0u;return 0;
        case 0x62a0u: /* str r2,[r0],#0x4 */
            STORE(r[0],r[2],4);r[0]+=4u;r[15]=0x62a4u;return 0;
        case 0x62a4u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x62a8u;return 0;
        case 0x62a8u: /* bne 0x000062a0 */
            r[15]=0x62acu;if(!(state->cpsr&0x40000000u)){r[15]=25248u;}return 0;
        case 0x62acu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x62b8u: /* ldr r8,[0x60fc] */
            LOAD(r[8],24828u,4);r[15]=0x62bcu;return 0;
        case 0x62bcu: /* adr r0,0x6384 */
            r[0]=25476u;r[15]=0x62c0u;return 0;
        case 0x62c0u: /* mov r1,#0x10 */
            r[1]=16u;r[15]=0x62c4u;return 0;
        case 0x62c4u: /* ldr r2,[r8],#0x4 */
            LOAD(r[2],r[8],4);r[8]+=4u;r[15]=0x62c8u;return 0;
        case 0x62c8u: /* str r2,[r0],#0x4 */
            STORE(r[0],r[2],4);r[0]+=4u;r[15]=0x62ccu;return 0;
        case 0x62ccu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x62d0u;return 0;
        case 0x62d0u: /* bne 0x000062c4 */
            r[15]=0x62d4u;if(!(state->cpsr&0x40000000u)){r[15]=25284u;}return 0;
        case 0x62d4u: /* mov r3,#0x10 */
            r[3]=16u;r[15]=0x62d8u;return 0;
        case 0x62d8u: /* ldr r8,[0x60fc] */
            LOAD(r[8],24828u,4);r[15]=0x62dcu;return 0;
        case 0x62dcu: /* mov r1,#0x10 */
            r[1]=16u;r[15]=0x62e0u;return 0;
        case 0x62e0u: /* ldrb r2,[r8,#0x1] */
            LOAD(r[2],r[8]+1u,1);r[15]=0x62e4u;return 0;
        case 0x62e4u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x62e8u;return 0;
        case 0x62e8u: /* bmi 0x000062f0 */
            r[15]=0x62ecu;if(state->cpsr&0x80000000u){r[15]=25328u;}return 0;
        case 0x62ecu: /* strb r2,[r8,#0x1] */
            STORE(r[8]+1u,r[2],1);r[15]=0x62f0u;return 0;
        case 0x62f0u: /* add r8,r8,#0x4 */
            r[8]=r[8]+4u;r[15]=0x62f4u;return 0;
        case 0x62f4u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x62f8u;return 0;
        case 0x62f8u: /* bne 0x000062e0 */
            r[15]=0x62fcu;if(!(state->cpsr&0x40000000u)){r[15]=25312u;}return 0;
        case 0x62fcu: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x6300u;return 0;
        case 0x6300u: /* mov r0,r0 */
            r[0]=r[0];r[15]=0x6304u;return 0;
        case 0x6304u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x6308u;return 0;
        case 0x6308u: /* bne 0x00006300 */
            r[15]=0x630cu;if(!(state->cpsr&0x40000000u)){r[15]=25344u;}return 0;
        case 0x630cu: /* subs r3,r3,#0x1 */
            r[3]=subtract(state,r[3],1u);r[15]=0x6310u;return 0;
        case 0x6310u: /* bne 0x000062d8 */
            r[15]=0x6314u;if(!(state->cpsr&0x40000000u)){r[15]=25304u;}return 0;
        case 0x6314u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6318u: /* ldr r8,[0x60fc] */
            LOAD(r[8],24828u,4);r[15]=0x631cu;return 0;
        case 0x631cu: /* adr r0,0x6384 */
            r[0]=25476u;r[15]=0x6320u;return 0;
        case 0x6320u: /* mov r1,#0x10 */
            r[1]=16u;r[15]=0x6324u;return 0;
        case 0x6324u: /* ldrb r2,[r0],#0x4 */
            LOAD(r[2],r[0],1);r[0]+=4u;r[15]=0x6328u;return 0;
        case 0x6328u: /* strb r2,[r8],#0x4 */
            STORE(r[8],r[2],1);r[8]+=4u;r[15]=0x632cu;return 0;
        case 0x632cu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x6330u;return 0;
        case 0x6330u: /* bne 0x00006324 */
            r[15]=0x6334u;if(!(state->cpsr&0x40000000u)){r[15]=25380u;}return 0;
        case 0x6334u: /* mov r3,#0x0 */
            r[3]=0u;r[15]=0x6338u;return 0;
        case 0x6338u: /* mov r4,#0x10 */
            r[4]=16u;r[15]=0x633cu;return 0;
        case 0x633cu: /* ldr r8,[0x60fc] */
            LOAD(r[8],24828u,4);r[15]=0x6340u;return 0;
        case 0x6340u: /* adr r0,0x6384 */
            r[0]=25476u;r[15]=0x6344u;return 0;
        case 0x6344u: /* mov r1,#0x10 */
            r[1]=16u;r[15]=0x6348u;return 0;
        case 0x6348u: /* ldrb r2,[r0,#0x1] */
            LOAD(r[2],r[0]+1u,1);r[15]=0x634cu;return 0;
        case 0x634cu: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x6350u;return 0;
        case 0x6350u: /* strbpl r3,[r8,#0x1] */
            if(!(state->cpsr&0x80000000u)){STORE(r[8]+1u,r[3],1);}r[15]=0x6354u;return 0;
        case 0x6354u: /* add r8,r8,#0x4 */
            r[8]=r[8]+4u;r[15]=0x6358u;return 0;
        case 0x6358u: /* add r0,r0,#0x4 */
            r[0]=r[0]+4u;r[15]=0x635cu;return 0;
        case 0x635cu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x6360u;return 0;
        case 0x6360u: /* bne 0x00006348 */
            r[15]=0x6364u;if(!(state->cpsr&0x40000000u)){r[15]=25416u;}return 0;
        case 0x6364u: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x6368u;return 0;
        case 0x6368u: /* mov r0,r0 */
            r[0]=r[0];r[15]=0x636cu;return 0;
        case 0x636cu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x6370u;return 0;
        case 0x6370u: /* bne 0x00006368 */
            r[15]=0x6374u;if(!(state->cpsr&0x40000000u)){r[15]=25448u;}return 0;
        case 0x6374u: /* add r3,r3,#0x1 */
            r[3]=r[3]+1u;r[15]=0x6378u;return 0;
        case 0x6378u: /* subs r4,r4,#0x1 */
            r[4]=subtract(state,r[4],1u);r[15]=0x637cu;return 0;
        case 0x637cu: /* bne 0x0000633c */
            r[15]=0x6380u;if(!(state->cpsr&0x40000000u)){r[15]=25404u;}return 0;
        case 0x6380u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x73bcu: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x73c0u;return 0;
        case 0x73c0u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x73c4u;return 0;
        case 0x73c4u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x73c8u;return 0;
        case 0x73c8u: /* str r1,[r11,r0] */
            STORE(r[11]+r[0],r[1],4);r[15]=0x73ccu;return 0;
        case 0x73ccu: /* add r0,r0,#0x4 */
            r[0]=r[0]+4u;r[15]=0x73d0u;return 0;
        case 0x73d0u: /* cmp r0,#0x800 */
            (void)subtract(state,r[0],2048u);r[15]=0x73d4u;return 0;
        case 0x73d4u: /* bne 0x000073c8 */
            r[15]=0x73d8u;if(!(state->cpsr&0x40000000u)){r[15]=29640u;}return 0;
        case 0x73d8u: /* ldr r0,[0x76f0] */
            LOAD(r[0],30448u,4);r[15]=0x73dcu;return 0;
        case 0x73dcu: /* mov r1,#0x4000 */
            r[1]=16384u;r[15]=0x73e0u;return 0;
        case 0x73e0u: /* str r1,[r11,#0x3c] */
            STORE(r[11]+60u,r[1],4);r[15]=0x73e4u;return 0;
        case 0x73e4u: /* add r11,r11,#0x40 */
            r[11]=r[11]+64u;r[15]=0x73e8u;return 0;
        case 0x73e8u: /* cmp r11,r0 */
            (void)subtract(state,r[11],r[0]);r[15]=0x73ecu;return 0;
        case 0x73ecu: /* bne 0x000073e0 */
            r[15]=0x73f0u;if(!(state->cpsr&0x40000000u)){r[15]=29664u;}return 0;
        case 0x73f0u: /* ldr r9,[0x76f0] */
            LOAD(r[9],30448u,4);r[15]=0x73f4u;return 0;
        case 0x73f4u: /* ldr r0,[0x76f4] */
            LOAD(r[0],30452u,4);r[15]=0x73f8u;return 0;
        case 0x73f8u: /* str r0,[r9,#0x8] */
            STORE(r[9]+8u,r[0],4);r[15]=0x73fcu;return 0;
        case 0x73fcu: /* str r0,[r9,#0xc] */
            STORE(r[9]+12u,r[0],4);r[15]=0x7400u;return 0;
        case 0x7400u: /* mov r0,#0x10000 */
            r[0]=65536u;r[15]=0x7404u;return 0;
        case 0x7404u: /* str r0,[r9,#0x4] */
            STORE(r[9]+4u,r[0],4);r[15]=0x7408u;return 0;
        case 0x7408u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7708u: /* ldr r12,[0x780c] */
            LOAD(r[12],30732u,4);r[15]=0x770cu;return 0;
        case 0x770cu: /* mov r1,#0x8 */
            r[1]=8u;r[15]=0x7710u;return 0;
        case 0x7710u: /* ldr r9,[0x77fc] */
            LOAD(r[9],30716u,4);r[15]=0x7714u;return 0;
        case 0x7714u: /* mov r2,#0x58 */
            r[2]=88u;r[15]=0x7718u;return 0;
        case 0x7718u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x771cu;return 0;
        case 0x771cu: /* str r6,[r12,r2] */
            STORE(r[12]+r[2],r[6],4);r[15]=0x7720u;return 0;
        case 0x7720u: /* subs r2,r2,#0x4 */
            r[2]=subtract(state,r[2],4u);r[15]=0x7724u;return 0;
        case 0x7724u: /* bmi 0x0000772c */
            r[15]=0x7728u;if(state->cpsr&0x80000000u){r[15]=30508u;}return 0;
        case 0x7728u: /* b 0x0000771c */
            r[15]=30492u;return 0;
        case 0x772cu: /* str r9,[r12,#0x18] */
            STORE(r[12]+24u,r[9],4);r[15]=0x7730u;return 0;
        case 0x7730u: /* strb r9,[r12,#0x1f] */
            STORE(r[12]+31u,r[9],1);r[15]=0x7734u;return 0;
        case 0x7734u: /* strb r9,[r12,#0x1e] */
            STORE(r[12]+30u,r[9],1);r[15]=0x7738u;return 0;
        case 0x7738u: /* strb r9,[r12,#0x29] */
            STORE(r[12]+41u,r[9],1);r[15]=0x773cu;return 0;
        case 0x773cu: /* strb r9,[r12,#0x41] */
            STORE(r[12]+65u,r[9],1);r[15]=0x7740u;return 0;
        case 0x7740u: /* strb r9,[r12,#0x45] */
            STORE(r[12]+69u,r[9],1);r[15]=0x7744u;return 0;
        case 0x7744u: /* strb r9,[r12,#0x4d] */
            STORE(r[12]+77u,r[9],1);r[15]=0x7748u;return 0;
        case 0x7748u: /* strb r9,[r12,#0x51] */
            STORE(r[12]+81u,r[9],1);r[15]=0x774cu;return 0;
        case 0x774cu: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x7750u;return 0;
        case 0x7750u: /* mov r2,#0x58 */
            r[2]=88u;r[15]=0x7754u;return 0;
        case 0x7754u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x7758u;return 0;
        case 0x7758u: /* bne 0x0000771c */
            r[15]=0x775cu;if(!(state->cpsr&0x40000000u)){r[15]=30492u;}return 0;
        case 0x775cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7760u;return 0;
        case 0x7760u: /* mov r1,#0x10 */
            r[1]=16u;r[15]=0x7764u;return 0;
        case 0x7764u: /* mov r4,#0x1f */
            r[4]=31u;r[15]=0x7768u;return 0;
        case 0x7768u: /* mov r5,#0xf */
            r[5]=15u;r[15]=0x776cu;return 0;
        case 0x776cu: /* str r6,[r12,r2] */
            STORE(r[12]+r[2],r[6],4);r[15]=0x7770u;return 0;
        case 0x7770u: /* subs r2,r2,#0x4 */
            r[2]=subtract(state,r[2],4u);r[15]=0x7774u;return 0;
        case 0x7774u: /* bmi 0x0000777c */
            r[15]=0x7778u;if(state->cpsr&0x80000000u){r[15]=30588u;}return 0;
        case 0x7778u: /* b 0x0000776c */
            r[15]=30572u;return 0;
        case 0x777cu: /* strb r4,[r12,#0x28] */
            STORE(r[12]+40u,r[4],1);r[15]=0x7780u;return 0;
        case 0x7780u: /* strb r5,[r12,#0x2a] */
            STORE(r[12]+42u,r[5],1);r[15]=0x7784u;return 0;
        case 0x7784u: /* str r9,[r12,#0x18] */
            STORE(r[12]+24u,r[9],4);r[15]=0x7788u;return 0;
        case 0x7788u: /* strb r9,[r12,#0x1f] */
            STORE(r[12]+31u,r[9],1);r[15]=0x778cu;return 0;
        case 0x778cu: /* strb r9,[r12,#0x1e] */
            STORE(r[12]+30u,r[9],1);r[15]=0x7790u;return 0;
        case 0x7790u: /* strb r9,[r12,#0x29] */
            STORE(r[12]+41u,r[9],1);r[15]=0x7794u;return 0;
        case 0x7794u: /* strb r9,[r12,#0x41] */
            STORE(r[12]+65u,r[9],1);r[15]=0x7798u;return 0;
        case 0x7798u: /* strb r9,[r12,#0x45] */
            STORE(r[12]+69u,r[9],1);r[15]=0x779cu;return 0;
        case 0x779cu: /* strb r9,[r12,#0x4d] */
            STORE(r[12]+77u,r[9],1);r[15]=0x77a0u;return 0;
        case 0x77a0u: /* strb r9,[r12,#0x51] */
            STORE(r[12]+81u,r[9],1);r[15]=0x77a4u;return 0;
        case 0x77a4u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x77a8u;return 0;
        case 0x77a8u: /* mov r2,#0x58 */
            r[2]=88u;r[15]=0x77acu;return 0;
        case 0x77acu: /* eor r4,r4,#0x10 */
            r[4]=r[4]^16u;r[15]=0x77b0u;return 0;
        case 0x77b0u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x77b4u;return 0;
        case 0x77b4u: /* bne 0x0000776c */
            r[15]=0x77b8u;if(!(state->cpsr&0x40000000u)){r[15]=30572u;}return 0;
        case 0x77b8u: /* ldr r10,[0x77ec] */
            LOAD(r[10],30700u,4);r[15]=0x77bcu;return 0;
        case 0x77bcu: /* ldr r2,[0x77e8] */
            LOAD(r[2],30696u,4);r[15]=0x77c0u;return 0;
        case 0x77c0u: /* mov r1,#0x10 */
            r[1]=16u;r[15]=0x77c4u;return 0;
        case 0x77c4u: /* str r6,[r10,#0x0] */
            STORE(r[10]+0u,r[6],4);r[15]=0x77c8u;return 0;
        case 0x77c8u: /* str r6,[r10,#0x4] */
            STORE(r[10]+4u,r[6],4);r[15]=0x77ccu;return 0;
        case 0x77ccu: /* str r2,[r10,#0x8] */
            STORE(r[10]+8u,r[2],4);r[15]=0x77d0u;return 0;
        case 0x77d0u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x77d4u;return 0;
        case 0x77d4u: /* ldmiaeq sp!,{lr} */
            if(state->cpsr&0x40000000u){LOAD(r[14],r[13],4);r[13]+=4u;}r[15]=0x77d8u;return 0;
        case 0x77d8u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x77dcu;}return 0;
        case 0x77dcu: /* add r10,r10,#0xc */
            r[10]=r[10]+12u;r[15]=0x77e0u;return 0;
        case 0x77e0u: /* add r2,r2,#0x80 */
            r[2]=r[2]+128u;r[15]=0x77e4u;return 0;
        case 0x77e4u: /* b 0x000077c4 */
            r[15]=30660u;return 0;
        default:*handled=0;return 0;
 }}
}

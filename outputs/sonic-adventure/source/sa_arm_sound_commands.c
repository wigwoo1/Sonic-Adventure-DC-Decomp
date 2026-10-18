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
int sa_arm_sound_reset_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
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
int sa_arm_sound_command_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
        case 0x9a4u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x9a8u;return 0;
        case 0x9a8u: /* bl 0x00001558 */
            r[14]=0x9acu;r[15]=5464u;return 0;
        case 0x9acu: /* bl 0x000009d0 */
            r[14]=0x9b0u;r[15]=2512u;return 0;
        case 0x9b0u: /* bl 0x00001558 */
            r[14]=0x9b4u;r[15]=5464u;return 0;
        case 0x9b4u: /* bl 0x000009d0 */
            r[14]=0x9b8u;r[15]=2512u;return 0;
        case 0x9b8u: /* bl 0x00001558 */
            r[14]=0x9bcu;r[15]=5464u;return 0;
        case 0x9bcu: /* bl 0x000009d0 */
            r[14]=0x9c0u;r[15]=2512u;return 0;
        case 0x9c0u: /* bl 0x00001558 */
            r[14]=0x9c4u;r[15]=5464u;return 0;
        case 0x9c4u: /* bl 0x000009d0 */
            r[14]=0x9c8u;r[15]=2512u;return 0;
        case 0x9c8u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x9ccu;return 0;
        case 0x9ccu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x9d0u: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x9d4u;return 0;
        case 0x9d4u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x9d8u;}return 0;
        case 0x9d8u: /* mov r0,r1, lsr #0x16 */
            r[0]=(r[1]>>22);r[15]=0x9dcu;return 0;
        case 0x9dcu: /* and r0,r0,#0x1c */
            r[0]=r[0]&28u;r[15]=0x9e0u;return 0;
        case 0x9e0u: /* mov r2,r1, lsr #0x10 */
            r[2]=(r[1]>>16);r[15]=0x9e4u;return 0;
        case 0x9e4u: /* and r2,r2,#0xf */
            r[2]=r[2]&15u;r[15]=0x9e8u;return 0;
        case 0x9e8u: /* mov r4,#0x48 */
            r[4]=72u;r[15]=0x9ecu;return 0;
        case 0x9ecu: /* mul r5,r2,r4 */
            r[5]=r[2]*r[4];r[15]=0x9f0u;return 0;
        case 0x9f0u: /* mov r3,r1, lsr #0x14 */
            r[3]=(r[1]>>20);r[15]=0x9f4u;return 0;
        case 0x9f4u: /* and r3,r3,#0x7 */
            r[3]=r[3]&7u;r[15]=0x9f8u;return 0;
        case 0x9f8u: /* mov r4,#0x480 */
            r[4]=1152u;r[15]=0x9fcu;return 0;
        case 0x9fcu: /* mov r6,#0xc400 */
            r[6]=50176u;r[15]=0xa00u;return 0;
        case 0xa00u: /* mla r6,r3,r4,r6 */
            r[6]=r[3]*r[4]+r[6];r[15]=0xa04u;return 0;
        case 0xa04u: /* mov r4,r1, lsr #0x1b */
            r[4]=(r[1]>>27);r[15]=0xa08u;return 0;
        case 0xa08u: /* and r4,r4,#0x1f */
            r[4]=r[4]&31u;r[15]=0xa0cu;return 0;
        case 0xa0cu: /* adr r7,0xa18 */
            r[7]=2584u;r[15]=0xa10u;return 0;
        case 0xa10u: /* add r7,r7,r0 */
            r[7]=r[7]+r[0];r[15]=0xa14u;return 0;
        case 0xa14u: /* mov pc,r7 */
            r[15]=r[7];return 0;
        case 0xa18u: /* b 0x00000a38 */
            r[15]=2616u;return 0;
        case 0xa1cu: /* b 0x00000ad4 */
            r[15]=2772u;return 0;
        case 0xa20u: /* b 0x0000135c */
            r[15]=4956u;return 0;
        case 0xa24u: /* b 0x00001818 */
            r[15]=6168u;return 0;
        case 0xa28u: /* b 0x00001360 */
            r[15]=4960u;return 0;
        case 0xa2cu: /* b 0x000013e0 */
            r[15]=5088u;return 0;
        case 0xa30u: /* b 0x000013e4 */
            r[15]=5092u;return 0;
        case 0xa34u: /* b 0x000014ac */
            r[15]=5292u;return 0;
        case 0xa38u: /* mov r0,r1, lsr #0x8 */
            r[0]=(r[1]>>8);r[15]=0xa3cu;return 0;
        case 0xa3cu: /* and r0,r0,#0x7f */
            r[0]=r[0]&127u;r[15]=0xa40u;return 0;
        case 0xa40u: /* add r5,r5,r6 */
            r[5]=r[5]+r[6];r[15]=0xa44u;return 0;
        case 0xa44u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0xa48u;return 0;
        case 0xa48u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0xa4cu;return 0;
        case 0xa4cu: /* mov r9,#0x800000 */
            r[9]=8388608u;r[15]=0xa50u;return 0;
        case 0xa50u: /* ldrb r8,[r6,#0x0] */
            LOAD(r[8],r[6]+0u,1);r[15]=0xa54u;return 0;
        case 0xa54u: /* tst r8,#0x80 */
            temporary=r[8]&128u;logic_flags(state,temporary,0,0);r[15]=0xa58u;return 0;
        case 0xa58u: /* beq 0x00000abc */
            if(state->cpsr&0x40000000u){r[15]=2748u;}else{r[15]=0xa5cu;}return 0;
        case 0xa5cu: /* ldrb r8,[r6,#0x4] */
            LOAD(r[8],r[6]+4u,1);r[15]=0xa60u;return 0;
        case 0xa60u: /* cmp r8,r3 */
            (void)subtract(state,r[8],r[3]);r[15]=0xa64u;return 0;
        case 0xa64u: /* bne 0x00000abc */
            if(!(state->cpsr&0x40000000u)){r[15]=2748u;}else{r[15]=0xa68u;}return 0;
        case 0xa68u: /* ldrb r8,[r6,#0x5] */
            LOAD(r[8],r[6]+5u,1);r[15]=0xa6cu;return 0;
        case 0xa6cu: /* cmp r8,r2 */
            (void)subtract(state,r[8],r[2]);r[15]=0xa70u;return 0;
        case 0xa70u: /* bne 0x00000abc */
            if(!(state->cpsr&0x40000000u)){r[15]=2748u;}else{r[15]=0xa74u;}return 0;
        case 0xa74u: /* ldrb r8,[r6,#0x6] */
            LOAD(r[8],r[6]+6u,1);r[15]=0xa78u;return 0;
        case 0xa78u: /* cmp r8,r0 */
            (void)subtract(state,r[8],r[0]);r[15]=0xa7cu;return 0;
        case 0xa7cu: /* bne 0x00000abc */
            if(!(state->cpsr&0x40000000u)){r[15]=2748u;}else{r[15]=0xa80u;}return 0;
        case 0xa80u: /* ldrb r10,[r6,#0x0] */
            LOAD(r[10],r[6]+0u,1);r[15]=0xa84u;return 0;
        case 0xa84u: /* bic r10,r10,#0x80 */
            r[10]=r[10]&~128u;r[15]=0xa88u;return 0;
        case 0xa88u: /* strb r10,[r6,#0x0] */
            STORE(r[6]+0u,r[10],1);r[15]=0xa8cu;return 0;
        case 0xa8cu: /* tst r10,#0x40 */
            temporary=r[10]&64u;logic_flags(state,temporary,0,0);r[15]=0xa90u;return 0;
        case 0xa90u: /* bne 0x00000abc */
            if(!(state->cpsr&0x40000000u)){r[15]=2748u;}else{r[15]=0xa94u;}return 0;
        case 0xa94u: /* mov r10,#0x0 */
            r[10]=0u;r[15]=0xa98u;return 0;
        case 0xa98u: /* strb r10,[r6,#0x3f] */
            STORE(r[6]+63u,r[10],1);r[15]=0xa9cu;return 0;
        case 0xa9cu: /* str r10,[r6,#0x38] */
            STORE(r[6]+56u,r[10],4);r[15]=0xaa0u;return 0;
        case 0xaa0u: /* mov r10,#0xff */
            r[10]=255u;r[15]=0xaa4u;return 0;
        case 0xaa4u: /* strb r10,[r6,#0x3] */
            STORE(r[6]+3u,r[10],1);r[15]=0xaa8u;return 0;
        case 0xaa8u: /* ldr r10,[r9,#0x0] */
            LOAD(r[10],r[9]+0u,4);r[15]=0xaacu;return 0;
        case 0xaacu: /* ldr r1,[0xad0] */
            LOAD(r[1],2768u,4);r[15]=0xab0u;return 0;
        case 0xab0u: /* and r10,r10,r1 */
            r[10]=r[10]&r[1];r[15]=0xab4u;return 0;
        case 0xab4u: /* orr r10,r10,#0x8000 */
            r[10]=r[10]|32768u;r[15]=0xab8u;return 0;
        case 0xab8u: /* str r10,[r9,#0x0] */
            STORE(r[9]+0u,r[10],4);r[15]=0xabcu;return 0;
        case 0xabcu: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0xac0u;return 0;
        case 0xac0u: /* add r9,r9,#0x80 */
            r[9]=r[9]+128u;r[15]=0xac4u;return 0;
        case 0xac4u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0xac8u;return 0;
        case 0xac8u: /* bne 0x00000a50 */
            if(!(state->cpsr&0x40000000u)){r[15]=2640u;}else{r[15]=0xaccu;}return 0;
        case 0xaccu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x135cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x13e0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x13e4u: /* add r5,r5,r6 */
            r[5]=r[5]+r[6];r[15]=0x13e8u;return 0;
        case 0x13e8u: /* mov r0,r1, lsl #0x7 */
            r[0]=(r[1]<<7);r[15]=0x13ecu;return 0;
        case 0x13ecu: /* and r0,r0,#0x3f80 */
            r[0]=r[0]&16256u;r[15]=0x13f0u;return 0;
        case 0x13f0u: /* mov r1,r1, lsr #0x8 */
            r[1]=(r[1]>>8);r[15]=0x13f4u;return 0;
        case 0x13f4u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x13f8u;return 0;
        case 0x13f8u: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0x13fcu;return 0;
        case 0x13fcu: /* subs r1,r0,#0x2000 */
            r[1]=subtract(state,r[0],8192u);r[15]=0x1400u;return 0;
        case 0x1400u: /* str r1,[r5,#0x4] */
            STORE(r[5]+4u,r[1],4);r[15]=0x1404u;return 0;
        case 0x1404u: /* str r1,[0x14a4] */
            STORE(5284u,r[1],4);r[15]=0x1408u;return 0;
        case 0x1408u: /* mov r5,#0xe800 */
            r[5]=59392u;r[15]=0x140cu;return 0;
        case 0x140cu: /* mov r10,#0x30 */
            r[10]=48u;r[15]=0x1410u;return 0;
        case 0x1410u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x1414u;return 0;
        case 0x1414u: /* ldr r1,[r5,#0x0] */
            LOAD(r[1],r[5]+0u,4);r[15]=0x1418u;return 0;
        case 0x1418u: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x141cu;return 0;
        case 0x141cu: /* beq 0x00001488 */
            if(state->cpsr&0x40000000u){r[15]=5256u;}else{r[15]=0x1420u;}return 0;
        case 0x1420u: /* ldrb r1,[r5,#0x4] */
            LOAD(r[1],r[5]+4u,1);r[15]=0x1424u;return 0;
        case 0x1424u: /* cmp r1,r3 */
            (void)subtract(state,r[1],r[3]);r[15]=0x1428u;return 0;
        case 0x1428u: /* bne 0x00001488 */
            if(!(state->cpsr&0x40000000u)){r[15]=5256u;}else{r[15]=0x142cu;}return 0;
        case 0x142cu: /* ldrb r1,[r5,#0x5] */
            LOAD(r[1],r[5]+5u,1);r[15]=0x1430u;return 0;
        case 0x1430u: /* cmp r1,r2 */
            (void)subtract(state,r[1],r[2]);r[15]=0x1434u;return 0;
        case 0x1434u: /* bne 0x00001488 */
            if(!(state->cpsr&0x40000000u)){r[15]=5256u;}else{r[15]=0x1438u;}return 0;
        case 0x1438u: /* ldr r0,[0x14a4] */
            LOAD(r[0],5284u,4);r[15]=0x143cu;return 0;
        case 0x143cu: /* ldr r1,[0x14a8] */
            LOAD(r[1],5288u,4);r[15]=0x1440u;return 0;
        case 0x1440u: /* tst r0,r1 */
            temporary=r[0]&r[1];logic_flags(state,temporary,0,0);r[15]=0x1444u;return 0;
        case 0x1444u: /* ldrbne r7,[r5,#0xb] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[7],r[5]+11u,1);}r[15]=0x1448u;return 0;
        case 0x1448u: /* ldrbeq r7,[r5,#0xa] */
            if(state->cpsr&0x40000000u){LOAD(r[7],r[5]+10u,1);}r[15]=0x144cu;return 0;
        case 0x144cu: /* mul r0,r7,r0 */
            r[0]=r[7]*r[0];r[15]=0x1450u;return 0;
        case 0x1450u: /* mov r0,r0, lsr #0x5 */
            r[0]=(r[0]>>5);r[15]=0x1454u;return 0;
        case 0x1454u: /* ldr r7,[0x14a0] */
            LOAD(r[7],5280u,4);r[15]=0x1458u;return 0;
        case 0x1458u: /* and r7,r0,r7 */
            r[7]=r[0]&r[7];r[15]=0x145cu;return 0;
        case 0x145cu: /* str r7,[r5,#0xc] */
            STORE(r[5]+12u,r[7],4);r[15]=0x1460u;return 0;
        case 0x1460u: /* ldr r6,[r5,#0x14] */
            LOAD(r[6],r[5]+20u,4);r[15]=0x1464u;return 0;
        case 0x1464u: /* add r7,r6,r7 */
            r[7]=r[6]+r[7];r[15]=0x1468u;return 0;
        case 0x1468u: /* ldr r6,[r5,#0x18] */
            LOAD(r[6],r[5]+24u,4);r[15]=0x146cu;return 0;
        case 0x146cu: /* ldr r0,[r6,#0x10] */
            LOAD(r[0],r[6]+16u,4);r[15]=0x1470u;return 0;
        case 0x1470u: /* sub r0,r0,#0x8000 */
            r[0]=r[0]-32768u;r[15]=0x1474u;return 0;
        case 0x1474u: /* add r7,r7,r0 */
            r[7]=r[7]+r[0];r[15]=0x1478u;return 0;
        case 0x1478u: /* stmdb sp!,{r2,r3,lr} */
            STORE(r[13]-12u,r[2],4);STORE(r[13]-8u,r[3],4);STORE(r[13]-4u,r[14],4);r[13]-=12u;r[15]=0x147cu;return 0;
        case 0x147cu: /* ldr r6,[r5,#0x10] */
            LOAD(r[6],r[5]+16u,4);r[15]=0x1480u;return 0;
        case 0x1480u: /* bl 0x000014b0 */
            r[14]=0x1484u;r[15]=5296u;return 0;
        case 0x1484u: /* ldmia sp!,{r2,r3,lr} */
            LOAD(r[2],r[13]+0u,4);LOAD(r[3],r[13]+4u,4);LOAD(r[14],r[13]+8u,4);r[13]+=12u;r[15]=0x1488u;return 0;
        case 0x1488u: /* add r5,r5,#0x40 */
            r[5]=r[5]+64u;r[15]=0x148cu;return 0;
        case 0x148cu: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x1490u;return 0;
        case 0x1490u: /* subs r10,r10,#0x1 */
            r[10]=subtract(state,r[10],1u);r[15]=0x1494u;return 0;
        case 0x1494u: /* bne 0x00001414 */
            if(!(state->cpsr&0x40000000u)){r[15]=5140u;}else{r[15]=0x1498u;}return 0;
        case 0x1498u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x14acu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x14b0u: /* ldrb r1,[r6,#0x27] */
            LOAD(r[1],r[6]+39u,1);r[15]=0x14b4u;return 0;
        case 0x14b4u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,0);r[15]=0x14b8u;return 0;
        case 0x14b8u: /* ldrne r2,[0x1554] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[2],5460u,4);}r[15]=0x14bcu;return 0;
        case 0x14bcu: /* orrne r1,r1,r2 */
            if(!(state->cpsr&0x40000000u)){r[1]=r[1]|r[2];}r[15]=0x14c0u;return 0;
        case 0x14c0u: /* add r7,r7,r1 */
            r[7]=r[7]+r[1];r[15]=0x14c4u;return 0;
        case 0x14c4u: /* mov r3,r7, lsr #0x8 */
            r[3]=(r[7]>>8);r[15]=0x14c8u;return 0;
        case 0x14c8u: /* ldrb r2,[r5,#0x6] */
            LOAD(r[2],r[5]+6u,1);r[15]=0x14ccu;return 0;
        case 0x14ccu: /* add r3,r3,r2 */
            r[3]=r[3]+r[2];r[15]=0x14d0u;return 0;
        case 0x14d0u: /* add r3,r3,#0x60 */
            r[3]=r[3]+96u;r[15]=0x14d4u;return 0;
        case 0x14d4u: /* ldrb r2,[r6,#0x26] */
            LOAD(r[2],r[6]+38u,1);r[15]=0x14d8u;return 0;
        case 0x14d8u: /* and r2,r2,#0x7f */
            r[2]=r[2]&127u;r[15]=0x14dcu;return 0;
        case 0x14dcu: /* sub r3,r3,r2 */
            r[3]=r[3]-r[2];r[15]=0x14e0u;return 0;
        case 0x14e0u: /* and r3,r3,#0xff */
            r[3]=r[3]&255u;r[15]=0x14e4u;return 0;
        case 0x14e4u: /* ldr r4,[0x1808] */
            LOAD(r[4],6152u,4);r[15]=0x14e8u;return 0;
        case 0x14e8u: /* ldrb r3,[r4,r3] */
            LOAD(r[3],r[4]+r[3],1);r[15]=0x14ecu;return 0;
        case 0x14ecu: /* mov r3,r3, lsl #0x7 */
            r[3]=(r[3]<<7);r[15]=0x14f0u;return 0;
        case 0x14f0u: /* and r2,r3,#0x7800 */
            r[2]=r[3]&30720u;r[15]=0x14f4u;return 0;
        case 0x14f4u: /* mov r3,r3, lsl #0x1 */
            r[3]=(r[3]<<1);r[15]=0x14f8u;return 0;
        case 0x14f8u: /* and r3,r3,#0xf00 */
            r[3]=r[3]&3840u;r[15]=0x14fcu;return 0;
        case 0x14fcu: /* and r7,r7,#0xfe */
            r[7]=r[7]&254u;r[15]=0x1500u;return 0;
        case 0x1500u: /* orr r3,r3,r7 */
            r[3]=r[3]|r[7];r[15]=0x1504u;return 0;
        case 0x1504u: /* mov r3,r3, lsr #0x1 */
            r[3]=(r[3]>>1);r[15]=0x1508u;return 0;
        case 0x1508u: /* ldr r4,[0x180c] */
            LOAD(r[4],6156u,4);r[15]=0x150cu;return 0;
        case 0x150cu: /* ldrb r7,[r4,r3] */
            LOAD(r[7],r[4]+r[3],1);r[15]=0x1510u;return 0;
        case 0x1510u: /* ldr r4,[0x1548] */
            LOAD(r[4],5448u,4);r[15]=0x1514u;return 0;
        case 0x1514u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x1518u;return 0;
        case 0x1518u: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x151cu;return 0;
        case 0x151cu: /* ldr r4,[0x154c] */
            LOAD(r[4],5452u,4);r[15]=0x1520u;return 0;
        case 0x1520u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x1524u;return 0;
        case 0x1524u: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x1528u;return 0;
        case 0x1528u: /* ldr r4,[0x1550] */
            LOAD(r[4],5456u,4);r[15]=0x152cu;return 0;
        case 0x152cu: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x1530u;return 0;
        case 0x1530u: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x1534u;return 0;
        case 0x1534u: /* orr r2,r2,r7 */
            r[2]=r[2]|r[7];r[15]=0x1538u;return 0;
        case 0x1538u: /* ldr r7,[r8,#0x18] */
            LOAD(r[7],r[8]+24u,4);r[15]=0x153cu;return 0;
        case 0x153cu: /* cmp r2,r7 */
            (void)subtract(state,r[2],r[7]);r[15]=0x1540u;return 0;
        case 0x1540u: /* strne r2,[r8,#0x18] */
            if(!(state->cpsr&0x40000000u)){STORE(r[8]+24u,r[2],4);}r[15]=0x1544u;return 0;
        case 0x1544u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1558u: /* mov r0,#0xb000 */
            r[0]=45056u;r[15]=0x155cu;return 0;
        case 0x155cu: /* mov r3,#0x13000 */
            r[3]=77824u;r[15]=0x1560u;return 0;
        case 0x1560u: /* ldr r1,[r3,#0x0] */
            LOAD(r[1],r[3]+0u,4);r[15]=0x1564u;return 0;
        case 0x1564u: /* ldr r3,[0x1810] */
            LOAD(r[3],6160u,4);r[15]=0x1568u;return 0;
        case 0x1568u: /* ldr r2,[r3,#0x0] */
            LOAD(r[2],r[3]+0u,4);r[15]=0x156cu;return 0;
        case 0x156cu: /* cmp r1,r2 */
            (void)subtract(state,r[1],r[2]);r[15]=0x1570u;return 0;
        case 0x1570u: /* moveq r1,#0x0 */
            if(state->cpsr&0x40000000u){r[1]=0u;}r[15]=0x1574u;return 0;
        case 0x1574u: /* beq 0x00001590 */
            if(state->cpsr&0x40000000u){r[15]=5520u;}else{r[15]=0x1578u;}return 0;
        case 0x1578u: /* add r0,r0,r2 */
            r[0]=r[0]+r[2];r[15]=0x157cu;return 0;
        case 0x157cu: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x1580u;return 0;
        case 0x1580u: /* add r2,r2,#0x4 */
            r[2]=r[2]+4u;r[15]=0x1584u;return 0;
        case 0x1584u: /* ldr r4,[0x1594] */
            LOAD(r[4],5524u,4);r[15]=0x1588u;return 0;
        case 0x1588u: /* and r2,r2,r4 */
            r[2]=r[2]&r[4];r[15]=0x158cu;return 0;
        case 0x158cu: /* str r2,[r3,#0x0] */
            STORE(r[3]+0u,r[2],4);r[15]=0x1590u;return 0;
        case 0x1590u: /* mov pc,lr */
            r[15]=r[14];return 0;
        default:*handled=0;return 0;
 }}else{switch(r[15]){
        case 0x9a4u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x9a8u;return 0;
        case 0x9a8u: /* bl 0x00001564 */
            r[14]=0x9acu;r[15]=5476u;return 0;
        case 0x9acu: /* bl 0x000009d0 */
            r[14]=0x9b0u;r[15]=2512u;return 0;
        case 0x9b0u: /* bl 0x00001564 */
            r[14]=0x9b4u;r[15]=5476u;return 0;
        case 0x9b4u: /* bl 0x000009d0 */
            r[14]=0x9b8u;r[15]=2512u;return 0;
        case 0x9b8u: /* bl 0x00001564 */
            r[14]=0x9bcu;r[15]=5476u;return 0;
        case 0x9bcu: /* bl 0x000009d0 */
            r[14]=0x9c0u;r[15]=2512u;return 0;
        case 0x9c0u: /* bl 0x00001564 */
            r[14]=0x9c4u;r[15]=5476u;return 0;
        case 0x9c4u: /* bl 0x000009d0 */
            r[14]=0x9c8u;r[15]=2512u;return 0;
        case 0x9c8u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x9ccu;return 0;
        case 0x9ccu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x9d0u: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x9d4u;return 0;
        case 0x9d4u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x9d8u;}return 0;
        case 0x9d8u: /* mov r0,r1, lsr #0x16 */
            r[0]=(r[1]>>22);r[15]=0x9dcu;return 0;
        case 0x9dcu: /* and r0,r0,#0x1c */
            r[0]=r[0]&28u;r[15]=0x9e0u;return 0;
        case 0x9e0u: /* mov r2,r1, lsr #0x10 */
            r[2]=(r[1]>>16);r[15]=0x9e4u;return 0;
        case 0x9e4u: /* and r2,r2,#0xf */
            r[2]=r[2]&15u;r[15]=0x9e8u;return 0;
        case 0x9e8u: /* mov r4,#0x48 */
            r[4]=72u;r[15]=0x9ecu;return 0;
        case 0x9ecu: /* mul r5,r2,r4 */
            r[5]=r[2]*r[4];r[15]=0x9f0u;return 0;
        case 0x9f0u: /* mov r3,r1, lsr #0x14 */
            r[3]=(r[1]>>20);r[15]=0x9f4u;return 0;
        case 0x9f4u: /* and r3,r3,#0x7 */
            r[3]=r[3]&7u;r[15]=0x9f8u;return 0;
        case 0x9f8u: /* mov r4,#0x480 */
            r[4]=1152u;r[15]=0x9fcu;return 0;
        case 0x9fcu: /* mov r6,#0xc400 */
            r[6]=50176u;r[15]=0xa00u;return 0;
        case 0xa00u: /* mla r6,r3,r4,r6 */
            r[6]=r[3]*r[4]+r[6];r[15]=0xa04u;return 0;
        case 0xa04u: /* mov r4,r1, lsr #0x1b */
            r[4]=(r[1]>>27);r[15]=0xa08u;return 0;
        case 0xa08u: /* and r4,r4,#0x1f */
            r[4]=r[4]&31u;r[15]=0xa0cu;return 0;
        case 0xa0cu: /* adr r7,0xa18 */
            r[7]=2584u;r[15]=0xa10u;return 0;
        case 0xa10u: /* add r7,r7,r0 */
            r[7]=r[7]+r[0];r[15]=0xa14u;return 0;
        case 0xa14u: /* mov pc,r7 */
            r[15]=r[7];return 0;
        case 0xa18u: /* b 0x00000a38 */
            r[15]=2616u;return 0;
        case 0xa1cu: /* b 0x00000ad4 */
            r[15]=2772u;return 0;
        case 0xa20u: /* b 0x00001364 */
            r[15]=4964u;return 0;
        case 0xa24u: /* b 0x00001820 */
            r[15]=6176u;return 0;
        case 0xa28u: /* b 0x00001368 */
            r[15]=4968u;return 0;
        case 0xa2cu: /* b 0x000013ec */
            r[15]=5100u;return 0;
        case 0xa30u: /* b 0x000013f0 */
            r[15]=5104u;return 0;
        case 0xa34u: /* b 0x000014b8 */
            r[15]=5304u;return 0;
        case 0xa38u: /* mov r0,r1, lsr #0x8 */
            r[0]=(r[1]>>8);r[15]=0xa3cu;return 0;
        case 0xa3cu: /* and r0,r0,#0x7f */
            r[0]=r[0]&127u;r[15]=0xa40u;return 0;
        case 0xa40u: /* add r5,r5,r6 */
            r[5]=r[5]+r[6];r[15]=0xa44u;return 0;
        case 0xa44u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0xa48u;return 0;
        case 0xa48u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0xa4cu;return 0;
        case 0xa4cu: /* mov r9,#0x800000 */
            r[9]=8388608u;r[15]=0xa50u;return 0;
        case 0xa50u: /* ldrb r8,[r6,#0x0] */
            LOAD(r[8],r[6]+0u,1);r[15]=0xa54u;return 0;
        case 0xa54u: /* tst r8,#0x80 */
            temporary=r[8]&128u;logic_flags(state,temporary,0,0);r[15]=0xa58u;return 0;
        case 0xa58u: /* beq 0x00000abc */
            if(state->cpsr&0x40000000u){r[15]=2748u;}else{r[15]=0xa5cu;}return 0;
        case 0xa5cu: /* ldrb r8,[r6,#0x4] */
            LOAD(r[8],r[6]+4u,1);r[15]=0xa60u;return 0;
        case 0xa60u: /* cmp r8,r3 */
            (void)subtract(state,r[8],r[3]);r[15]=0xa64u;return 0;
        case 0xa64u: /* bne 0x00000abc */
            if(!(state->cpsr&0x40000000u)){r[15]=2748u;}else{r[15]=0xa68u;}return 0;
        case 0xa68u: /* ldrb r8,[r6,#0x5] */
            LOAD(r[8],r[6]+5u,1);r[15]=0xa6cu;return 0;
        case 0xa6cu: /* cmp r8,r2 */
            (void)subtract(state,r[8],r[2]);r[15]=0xa70u;return 0;
        case 0xa70u: /* bne 0x00000abc */
            if(!(state->cpsr&0x40000000u)){r[15]=2748u;}else{r[15]=0xa74u;}return 0;
        case 0xa74u: /* ldrb r8,[r6,#0x6] */
            LOAD(r[8],r[6]+6u,1);r[15]=0xa78u;return 0;
        case 0xa78u: /* cmp r8,r0 */
            (void)subtract(state,r[8],r[0]);r[15]=0xa7cu;return 0;
        case 0xa7cu: /* bne 0x00000abc */
            if(!(state->cpsr&0x40000000u)){r[15]=2748u;}else{r[15]=0xa80u;}return 0;
        case 0xa80u: /* ldrb r10,[r6,#0x0] */
            LOAD(r[10],r[6]+0u,1);r[15]=0xa84u;return 0;
        case 0xa84u: /* bic r10,r10,#0x80 */
            r[10]=r[10]&~128u;r[15]=0xa88u;return 0;
        case 0xa88u: /* strb r10,[r6,#0x0] */
            STORE(r[6]+0u,r[10],1);r[15]=0xa8cu;return 0;
        case 0xa8cu: /* tst r10,#0x40 */
            temporary=r[10]&64u;logic_flags(state,temporary,0,0);r[15]=0xa90u;return 0;
        case 0xa90u: /* bne 0x00000abc */
            if(!(state->cpsr&0x40000000u)){r[15]=2748u;}else{r[15]=0xa94u;}return 0;
        case 0xa94u: /* mov r10,#0x0 */
            r[10]=0u;r[15]=0xa98u;return 0;
        case 0xa98u: /* strb r10,[r6,#0x3f] */
            STORE(r[6]+63u,r[10],1);r[15]=0xa9cu;return 0;
        case 0xa9cu: /* str r10,[r6,#0x38] */
            STORE(r[6]+56u,r[10],4);r[15]=0xaa0u;return 0;
        case 0xaa0u: /* mov r10,#0xff */
            r[10]=255u;r[15]=0xaa4u;return 0;
        case 0xaa4u: /* strb r10,[r6,#0x3] */
            STORE(r[6]+3u,r[10],1);r[15]=0xaa8u;return 0;
        case 0xaa8u: /* ldr r10,[r9,#0x0] */
            LOAD(r[10],r[9]+0u,4);r[15]=0xaacu;return 0;
        case 0xaacu: /* ldr r1,[0xad0] */
            LOAD(r[1],2768u,4);r[15]=0xab0u;return 0;
        case 0xab0u: /* and r10,r10,r1 */
            r[10]=r[10]&r[1];r[15]=0xab4u;return 0;
        case 0xab4u: /* orr r10,r10,#0x8000 */
            r[10]=r[10]|32768u;r[15]=0xab8u;return 0;
        case 0xab8u: /* str r10,[r9,#0x0] */
            STORE(r[9]+0u,r[10],4);r[15]=0xabcu;return 0;
        case 0xabcu: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0xac0u;return 0;
        case 0xac0u: /* add r9,r9,#0x80 */
            r[9]=r[9]+128u;r[15]=0xac4u;return 0;
        case 0xac4u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0xac8u;return 0;
        case 0xac8u: /* bne 0x00000a50 */
            if(!(state->cpsr&0x40000000u)){r[15]=2640u;}else{r[15]=0xaccu;}return 0;
        case 0xaccu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1364u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x13ecu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x13f0u: /* add r5,r5,r6 */
            r[5]=r[5]+r[6];r[15]=0x13f4u;return 0;
        case 0x13f4u: /* mov r0,r1, lsl #0x7 */
            r[0]=(r[1]<<7);r[15]=0x13f8u;return 0;
        case 0x13f8u: /* and r0,r0,#0x3f80 */
            r[0]=r[0]&16256u;r[15]=0x13fcu;return 0;
        case 0x13fcu: /* mov r1,r1, lsr #0x8 */
            r[1]=(r[1]>>8);r[15]=0x1400u;return 0;
        case 0x1400u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1404u;return 0;
        case 0x1404u: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0x1408u;return 0;
        case 0x1408u: /* subs r1,r0,#0x2000 */
            r[1]=subtract(state,r[0],8192u);r[15]=0x140cu;return 0;
        case 0x140cu: /* str r1,[r5,#0x4] */
            STORE(r[5]+4u,r[1],4);r[15]=0x1410u;return 0;
        case 0x1410u: /* str r1,[0x14b0] */
            STORE(5296u,r[1],4);r[15]=0x1414u;return 0;
        case 0x1414u: /* mov r5,#0xe800 */
            r[5]=59392u;r[15]=0x1418u;return 0;
        case 0x1418u: /* mov r10,#0x30 */
            r[10]=48u;r[15]=0x141cu;return 0;
        case 0x141cu: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x1420u;return 0;
        case 0x1420u: /* ldr r1,[r5,#0x0] */
            LOAD(r[1],r[5]+0u,4);r[15]=0x1424u;return 0;
        case 0x1424u: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x1428u;return 0;
        case 0x1428u: /* beq 0x00001494 */
            if(state->cpsr&0x40000000u){r[15]=5268u;}else{r[15]=0x142cu;}return 0;
        case 0x142cu: /* ldrb r1,[r5,#0x4] */
            LOAD(r[1],r[5]+4u,1);r[15]=0x1430u;return 0;
        case 0x1430u: /* cmp r1,r3 */
            (void)subtract(state,r[1],r[3]);r[15]=0x1434u;return 0;
        case 0x1434u: /* bne 0x00001494 */
            if(!(state->cpsr&0x40000000u)){r[15]=5268u;}else{r[15]=0x1438u;}return 0;
        case 0x1438u: /* ldrb r1,[r5,#0x5] */
            LOAD(r[1],r[5]+5u,1);r[15]=0x143cu;return 0;
        case 0x143cu: /* cmp r1,r2 */
            (void)subtract(state,r[1],r[2]);r[15]=0x1440u;return 0;
        case 0x1440u: /* bne 0x00001494 */
            if(!(state->cpsr&0x40000000u)){r[15]=5268u;}else{r[15]=0x1444u;}return 0;
        case 0x1444u: /* ldr r0,[0x14b0] */
            LOAD(r[0],5296u,4);r[15]=0x1448u;return 0;
        case 0x1448u: /* ldr r1,[0x14b4] */
            LOAD(r[1],5300u,4);r[15]=0x144cu;return 0;
        case 0x144cu: /* tst r0,r1 */
            temporary=r[0]&r[1];logic_flags(state,temporary,0,0);r[15]=0x1450u;return 0;
        case 0x1450u: /* ldrbne r7,[r5,#0xb] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[7],r[5]+11u,1);}r[15]=0x1454u;return 0;
        case 0x1454u: /* ldrbeq r7,[r5,#0xa] */
            if(state->cpsr&0x40000000u){LOAD(r[7],r[5]+10u,1);}r[15]=0x1458u;return 0;
        case 0x1458u: /* mul r0,r7,r0 */
            r[0]=r[7]*r[0];r[15]=0x145cu;return 0;
        case 0x145cu: /* mov r0,r0, lsr #0x5 */
            r[0]=(r[0]>>5);r[15]=0x1460u;return 0;
        case 0x1460u: /* ldr r7,[0x14ac] */
            LOAD(r[7],5292u,4);r[15]=0x1464u;return 0;
        case 0x1464u: /* and r7,r0,r7 */
            r[7]=r[0]&r[7];r[15]=0x1468u;return 0;
        case 0x1468u: /* str r7,[r5,#0xc] */
            STORE(r[5]+12u,r[7],4);r[15]=0x146cu;return 0;
        case 0x146cu: /* ldr r6,[r5,#0x14] */
            LOAD(r[6],r[5]+20u,4);r[15]=0x1470u;return 0;
        case 0x1470u: /* add r7,r6,r7 */
            r[7]=r[6]+r[7];r[15]=0x1474u;return 0;
        case 0x1474u: /* ldr r6,[r5,#0x18] */
            LOAD(r[6],r[5]+24u,4);r[15]=0x1478u;return 0;
        case 0x1478u: /* ldr r0,[r6,#0x10] */
            LOAD(r[0],r[6]+16u,4);r[15]=0x147cu;return 0;
        case 0x147cu: /* sub r0,r0,#0x8000 */
            r[0]=r[0]-32768u;r[15]=0x1480u;return 0;
        case 0x1480u: /* add r7,r7,r0 */
            r[7]=r[7]+r[0];r[15]=0x1484u;return 0;
        case 0x1484u: /* stmdb sp!,{r2,r3,lr} */
            STORE(r[13]-12u,r[2],4);STORE(r[13]-8u,r[3],4);STORE(r[13]-4u,r[14],4);r[13]-=12u;r[15]=0x1488u;return 0;
        case 0x1488u: /* ldr r6,[r5,#0x10] */
            LOAD(r[6],r[5]+16u,4);r[15]=0x148cu;return 0;
        case 0x148cu: /* bl 0x000014bc */
            r[14]=0x1490u;r[15]=5308u;return 0;
        case 0x1490u: /* ldmia sp!,{r2,r3,lr} */
            LOAD(r[2],r[13]+0u,4);LOAD(r[3],r[13]+4u,4);LOAD(r[14],r[13]+8u,4);r[13]+=12u;r[15]=0x1494u;return 0;
        case 0x1494u: /* add r5,r5,#0x40 */
            r[5]=r[5]+64u;r[15]=0x1498u;return 0;
        case 0x1498u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x149cu;return 0;
        case 0x149cu: /* subs r10,r10,#0x1 */
            r[10]=subtract(state,r[10],1u);r[15]=0x14a0u;return 0;
        case 0x14a0u: /* bne 0x00001420 */
            if(!(state->cpsr&0x40000000u)){r[15]=5152u;}else{r[15]=0x14a4u;}return 0;
        case 0x14a4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x14b8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x14bcu: /* ldrb r1,[r6,#0x27] */
            LOAD(r[1],r[6]+39u,1);r[15]=0x14c0u;return 0;
        case 0x14c0u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,0);r[15]=0x14c4u;return 0;
        case 0x14c4u: /* ldrne r2,[0x1560] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[2],5472u,4);}r[15]=0x14c8u;return 0;
        case 0x14c8u: /* orrne r1,r1,r2 */
            if(!(state->cpsr&0x40000000u)){r[1]=r[1]|r[2];}r[15]=0x14ccu;return 0;
        case 0x14ccu: /* add r7,r7,r1 */
            r[7]=r[7]+r[1];r[15]=0x14d0u;return 0;
        case 0x14d0u: /* mov r3,r7, lsr #0x8 */
            r[3]=(r[7]>>8);r[15]=0x14d4u;return 0;
        case 0x14d4u: /* ldrb r2,[r5,#0x6] */
            LOAD(r[2],r[5]+6u,1);r[15]=0x14d8u;return 0;
        case 0x14d8u: /* add r3,r3,r2 */
            r[3]=r[3]+r[2];r[15]=0x14dcu;return 0;
        case 0x14dcu: /* add r3,r3,#0x60 */
            r[3]=r[3]+96u;r[15]=0x14e0u;return 0;
        case 0x14e0u: /* ldrb r2,[r6,#0x26] */
            LOAD(r[2],r[6]+38u,1);r[15]=0x14e4u;return 0;
        case 0x14e4u: /* and r2,r2,#0x7f */
            r[2]=r[2]&127u;r[15]=0x14e8u;return 0;
        case 0x14e8u: /* sub r3,r3,r2 */
            r[3]=r[3]-r[2];r[15]=0x14ecu;return 0;
        case 0x14ecu: /* and r3,r3,#0xff */
            r[3]=r[3]&255u;r[15]=0x14f0u;return 0;
        case 0x14f0u: /* ldr r4,[0x1810] */
            LOAD(r[4],6160u,4);r[15]=0x14f4u;return 0;
        case 0x14f4u: /* ldrb r3,[r4,r3] */
            LOAD(r[3],r[4]+r[3],1);r[15]=0x14f8u;return 0;
        case 0x14f8u: /* mov r3,r3, lsl #0x7 */
            r[3]=(r[3]<<7);r[15]=0x14fcu;return 0;
        case 0x14fcu: /* and r2,r3,#0x7800 */
            r[2]=r[3]&30720u;r[15]=0x1500u;return 0;
        case 0x1500u: /* mov r3,r3, lsl #0x1 */
            r[3]=(r[3]<<1);r[15]=0x1504u;return 0;
        case 0x1504u: /* and r3,r3,#0xf00 */
            r[3]=r[3]&3840u;r[15]=0x1508u;return 0;
        case 0x1508u: /* and r7,r7,#0xfe */
            r[7]=r[7]&254u;r[15]=0x150cu;return 0;
        case 0x150cu: /* orr r3,r3,r7 */
            r[3]=r[3]|r[7];r[15]=0x1510u;return 0;
        case 0x1510u: /* mov r3,r3, lsr #0x1 */
            r[3]=(r[3]>>1);r[15]=0x1514u;return 0;
        case 0x1514u: /* ldr r4,[0x1814] */
            LOAD(r[4],6164u,4);r[15]=0x1518u;return 0;
        case 0x1518u: /* ldrb r7,[r4,r3] */
            LOAD(r[7],r[4]+r[3],1);r[15]=0x151cu;return 0;
        case 0x151cu: /* ldr r4,[0x1554] */
            LOAD(r[4],5460u,4);r[15]=0x1520u;return 0;
        case 0x1520u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x1524u;return 0;
        case 0x1524u: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x1528u;return 0;
        case 0x1528u: /* ldr r4,[0x1558] */
            LOAD(r[4],5464u,4);r[15]=0x152cu;return 0;
        case 0x152cu: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x1530u;return 0;
        case 0x1530u: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x1534u;return 0;
        case 0x1534u: /* ldr r4,[0x155c] */
            LOAD(r[4],5468u,4);r[15]=0x1538u;return 0;
        case 0x1538u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x153cu;return 0;
        case 0x153cu: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x1540u;return 0;
        case 0x1540u: /* orr r2,r2,r7 */
            r[2]=r[2]|r[7];r[15]=0x1544u;return 0;
        case 0x1544u: /* ldr r7,[r8,#0x18] */
            LOAD(r[7],r[8]+24u,4);r[15]=0x1548u;return 0;
        case 0x1548u: /* cmp r2,r7 */
            (void)subtract(state,r[2],r[7]);r[15]=0x154cu;return 0;
        case 0x154cu: /* strne r2,[r8,#0x18] */
            if(!(state->cpsr&0x40000000u)){STORE(r[8]+24u,r[2],4);}r[15]=0x1550u;return 0;
        case 0x1550u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1564u: /* mov r0,#0xb000 */
            r[0]=45056u;r[15]=0x1568u;return 0;
        case 0x1568u: /* mov r3,#0x13000 */
            r[3]=77824u;r[15]=0x156cu;return 0;
        case 0x156cu: /* ldr r1,[r3,#0x0] */
            LOAD(r[1],r[3]+0u,4);r[15]=0x1570u;return 0;
        case 0x1570u: /* ldr r3,[0x1818] */
            LOAD(r[3],6168u,4);r[15]=0x1574u;return 0;
        case 0x1574u: /* ldr r2,[r3,#0x0] */
            LOAD(r[2],r[3]+0u,4);r[15]=0x1578u;return 0;
        case 0x1578u: /* cmp r1,r2 */
            (void)subtract(state,r[1],r[2]);r[15]=0x157cu;return 0;
        case 0x157cu: /* moveq r1,#0x0 */
            if(state->cpsr&0x40000000u){r[1]=0u;}r[15]=0x1580u;return 0;
        case 0x1580u: /* beq 0x0000159c */
            if(state->cpsr&0x40000000u){r[15]=5532u;}else{r[15]=0x1584u;}return 0;
        case 0x1584u: /* add r0,r0,r2 */
            r[0]=r[0]+r[2];r[15]=0x1588u;return 0;
        case 0x1588u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x158cu;return 0;
        case 0x158cu: /* add r2,r2,#0x4 */
            r[2]=r[2]+4u;r[15]=0x1590u;return 0;
        case 0x1590u: /* ldr r4,[0x15a0] */
            LOAD(r[4],5536u,4);r[15]=0x1594u;return 0;
        case 0x1594u: /* and r2,r2,r4 */
            r[2]=r[2]&r[4];r[15]=0x1598u;return 0;
        case 0x1598u: /* str r2,[r3,#0x0] */
            STORE(r[3]+0u,r[2],4);r[15]=0x159cu;return 0;
        case 0x159cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        default:*handled=0;return 0;
 }}
}

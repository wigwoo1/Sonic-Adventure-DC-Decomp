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
static uint32_t addition(struct sa_arm_state *s,uint32_t a,uint32_t b){
 uint32_t v=a+b,flags=v&0x80000000u;
 if(!v)flags|=0x40000000u;if((uint64_t)a+b>0xffffffffu)flags|=0x20000000u;
 flags|=(~(a^b)&(a^v)&0x80000000u)>>3;
 s->cpsr=(s->cpsr&0x0fffffffu)|flags;return v;
}
int sa_arm_sound_ramp_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
        case 0x6620u: /* ldr r0,[0x663c] */
            LOAD(r[0],26172u,4);r[15]=0x6624u;return 0;
        case 0x6624u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x6628u;return 0;
        case 0x6628u: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x662cu;return 0;
        case 0x662cu: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x6630u;return 0;
        case 0x6630u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6640u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x6644u;return 0;
        case 0x6644u: /* mov r1,#0x8 */
            r[1]=8u;r[15]=0x6648u;return 0;
        case 0x6648u: /* ldrb r5,[r0,#0x28] */
            LOAD(r[5],r[0]+40u,1);r[15]=0x664cu;return 0;
        case 0x664cu: /* ldrb r2,[r0,#0x0] */
            LOAD(r[2],r[0]+0u,1);r[15]=0x6650u;return 0;
        case 0x6650u: /* ldrb r3,[r0,#0x1] */
            LOAD(r[3],r[0]+1u,1);r[15]=0x6654u;return 0;
        case 0x6654u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x6658u;return 0;
        case 0x6658u: /* biceq r5,r5,#0x4 */
            if(state->cpsr&0x40000000u){r[5]=r[5]&~4u;}r[15]=0x665cu;return 0;
        case 0x665cu: /* beq 0x000066cc */
            if(state->cpsr&0x40000000u){r[15]=26316u;}else{r[15]=0x6660u;}return 0;
        case 0x6660u: /* ldrb r4,[r0,#0x3] */
            LOAD(r[4],r[0]+3u,1);r[15]=0x6664u;return 0;
        case 0x6664u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6668u;return 0;
        case 0x6668u: /* beq 0x00006678 */
            if(state->cpsr&0x40000000u){r[15]=26232u;}else{r[15]=0x666cu;}return 0;
        case 0x666cu: /* sub r4,r4,#0x1 */
            r[4]=r[4]-1u;r[15]=0x6670u;return 0;
        case 0x6670u: /* strb r4,[r0,#0x3] */
            STORE(r[0]+3u,r[4],1);r[15]=0x6674u;return 0;
        case 0x6674u: /* b 0x000066cc */
            r[15]=26316u;return 0;
        case 0x6678u: /* ldrb r4,[r0,#0x2] */
            LOAD(r[4],r[0]+2u,1);r[15]=0x667cu;return 0;
        case 0x667cu: /* strb r4,[r0,#0x3] */
            STORE(r[0]+3u,r[4],1);r[15]=0x6680u;return 0;
        case 0x6680u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6684u;return 0;
        case 0x6684u: /* strbeq r2,[r0,#0x1] */
            if(state->cpsr&0x40000000u){STORE(r[0]+1u,r[2],1);}r[15]=0x6688u;return 0;
        case 0x6688u: /* beq 0x000066c0 */
            if(state->cpsr&0x40000000u){r[15]=26304u;}else{r[15]=0x668cu;}return 0;
        case 0x668cu: /* ldrb r4,[r0,#0x4] */
            LOAD(r[4],r[0]+4u,1);r[15]=0x6690u;return 0;
        case 0x6690u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x6694u;return 0;
        case 0x6694u: /* bcs 0x000066ac */
            if(state->cpsr&0x20000000u){r[15]=26284u;}else{r[15]=0x6698u;}return 0;
        case 0x6698u: /* add r3,r3,r4 */
            r[3]=r[3]+r[4];r[15]=0x669cu;return 0;
        case 0x669cu: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x66a0u;return 0;
        case 0x66a0u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x66a4u;return 0;
        case 0x66a4u: /* strb r3,[r0,#0x1] */
            STORE(r[0]+1u,r[3],1);r[15]=0x66a8u;return 0;
        case 0x66a8u: /* b 0x000066c0 */
            r[15]=26304u;return 0;
        case 0x66acu: /* subs r3,r3,r4 */
            r[3]=subtract(state,r[3],r[4]);r[15]=0x66b0u;return 0;
        case 0x66b0u: /* movcc r3,#0x0 */
            if(!(state->cpsr&0x20000000u))r[3]=0;r[15]=0x66b4u;return 0;
        case 0x66b4u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x66b8u;return 0;
        case 0x66b8u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x66bcu;return 0;
        case 0x66bcu: /* strb r3,[r0,#0x1] */
            STORE(r[0]+1u,r[3],1);r[15]=0x66c0u;return 0;
        case 0x66c0u: /* mov r3,#0x1 */
            r[3]=1u;r[15]=0x66c4u;return 0;
        case 0x66c4u: /* strb r3,[r0,#0x5] */
            STORE(r[0]+5u,r[3],1);r[15]=0x66c8u;return 0;
        case 0x66c8u: /* orr r5,r5,#0x4 */
            r[5]=r[5]|4u;r[15]=0x66ccu;return 0;
        case 0x66ccu: /* strb r5,[r0,#0x28] */
            STORE(r[0]+40u,r[5],1);r[15]=0x66d0u;return 0;
        case 0x66d0u: /* add r0,r0,#0x80 */
            r[0]=r[0]+128u;r[15]=0x66d4u;return 0;
        case 0x66d4u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x66d8u;return 0;
        case 0x66d8u: /* bne 0x00006648 */
            if(!(state->cpsr&0x40000000u)){r[15]=26184u;}else{r[15]=0x66dcu;}return 0;
        case 0x66dcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x66e0u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x66e4u;return 0;
        case 0x66e4u: /* mov r1,#0x8 */
            r[1]=8u;r[15]=0x66e8u;return 0;
        case 0x66e8u: /* ldrb r5,[r0,#0x28] */
            LOAD(r[5],r[0]+40u,1);r[15]=0x66ecu;return 0;
        case 0x66ecu: /* ldrb r2,[r0,#0x6] */
            LOAD(r[2],r[0]+6u,1);r[15]=0x66f0u;return 0;
        case 0x66f0u: /* ldrb r3,[r0,#0x7] */
            LOAD(r[3],r[0]+7u,1);r[15]=0x66f4u;return 0;
        case 0x66f4u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x66f8u;return 0;
        case 0x66f8u: /* biceq r5,r5,#0x10 */
            if(state->cpsr&0x40000000u){r[5]=r[5]&~16u;}r[15]=0x66fcu;return 0;
        case 0x66fcu: /* beq 0x0000676c */
            if(state->cpsr&0x40000000u){r[15]=26476u;}else{r[15]=0x6700u;}return 0;
        case 0x6700u: /* ldrb r4,[r0,#0x9] */
            LOAD(r[4],r[0]+9u,1);r[15]=0x6704u;return 0;
        case 0x6704u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6708u;return 0;
        case 0x6708u: /* beq 0x00006718 */
            if(state->cpsr&0x40000000u){r[15]=26392u;}else{r[15]=0x670cu;}return 0;
        case 0x670cu: /* sub r4,r4,#0x1 */
            r[4]=r[4]-1u;r[15]=0x6710u;return 0;
        case 0x6710u: /* strb r4,[r0,#0x9] */
            STORE(r[0]+9u,r[4],1);r[15]=0x6714u;return 0;
        case 0x6714u: /* b 0x0000676c */
            r[15]=26476u;return 0;
        case 0x6718u: /* ldrb r4,[r0,#0x8] */
            LOAD(r[4],r[0]+8u,1);r[15]=0x671cu;return 0;
        case 0x671cu: /* strb r4,[r0,#0x9] */
            STORE(r[0]+9u,r[4],1);r[15]=0x6720u;return 0;
        case 0x6720u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6724u;return 0;
        case 0x6724u: /* strbeq r2,[r0,#0x7] */
            if(state->cpsr&0x40000000u){STORE(r[0]+7u,r[2],1);}r[15]=0x6728u;return 0;
        case 0x6728u: /* beq 0x00006760 */
            if(state->cpsr&0x40000000u){r[15]=26464u;}else{r[15]=0x672cu;}return 0;
        case 0x672cu: /* ldrb r4,[r0,#0xa] */
            LOAD(r[4],r[0]+10u,1);r[15]=0x6730u;return 0;
        case 0x6730u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x6734u;return 0;
        case 0x6734u: /* bcs 0x0000674c */
            if(state->cpsr&0x20000000u){r[15]=26444u;}else{r[15]=0x6738u;}return 0;
        case 0x6738u: /* add r3,r3,r4 */
            r[3]=r[3]+r[4];r[15]=0x673cu;return 0;
        case 0x673cu: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x6740u;return 0;
        case 0x6740u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x6744u;return 0;
        case 0x6744u: /* strb r3,[r0,#0x7] */
            STORE(r[0]+7u,r[3],1);r[15]=0x6748u;return 0;
        case 0x6748u: /* b 0x00006760 */
            r[15]=26464u;return 0;
        case 0x674cu: /* subs r3,r3,r4 */
            r[3]=subtract(state,r[3],r[4]);r[15]=0x6750u;return 0;
        case 0x6750u: /* movcc r3,#0x0 */
            if(!(state->cpsr&0x20000000u))r[3]=0;r[15]=0x6754u;return 0;
        case 0x6754u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x6758u;return 0;
        case 0x6758u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x675cu;return 0;
        case 0x675cu: /* strb r3,[r0,#0x7] */
            STORE(r[0]+7u,r[3],1);r[15]=0x6760u;return 0;
        case 0x6760u: /* mov r3,#0x1 */
            r[3]=1u;r[15]=0x6764u;return 0;
        case 0x6764u: /* strb r3,[r0,#0xb] */
            STORE(r[0]+11u,r[3],1);r[15]=0x6768u;return 0;
        case 0x6768u: /* orr r5,r5,#0x4 */
            r[5]=r[5]|4u;r[15]=0x676cu;return 0;
        case 0x676cu: /* strb r5,[r0,#0x28] */
            STORE(r[0]+40u,r[5],1);r[15]=0x6770u;return 0;
        case 0x6770u: /* add r0,r0,#0x80 */
            r[0]=r[0]+128u;r[15]=0x6774u;return 0;
        case 0x6774u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x6778u;return 0;
        case 0x6778u: /* bne 0x000066e8 */
            if(!(state->cpsr&0x40000000u)){r[15]=26344u;}else{r[15]=0x677cu;}return 0;
        case 0x677cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6780u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x6784u;return 0;
        case 0x6784u: /* mov r1,#0x8 */
            r[1]=8u;r[15]=0x6788u;return 0;
        case 0x6788u: /* ldr r11,[0x6844] */
            LOAD(r[11],26692u,4);r[15]=0x678cu;return 0;
        case 0x678cu: /* ldrb r5,[r0,#0x28] */
            LOAD(r[5],r[0]+40u,1);r[15]=0x6790u;return 0;
        case 0x6790u: /* ldr r2,[r0,#0x18] */
            LOAD(r[2],r[0]+24u,4);r[15]=0x6794u;return 0;
        case 0x6794u: /* ldr r3,[r0,#0x1c] */
            LOAD(r[3],r[0]+28u,4);r[15]=0x6798u;return 0;
        case 0x6798u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x679cu;return 0;
        case 0x679cu: /* biceq r5,r5,#0x8 */
            if(state->cpsr&0x40000000u){r[5]=r[5]&~8u;}r[15]=0x67a0u;return 0;
        case 0x67a0u: /* beq 0x0000682c */
            if(state->cpsr&0x40000000u){r[15]=26668u;}else{r[15]=0x67a4u;}return 0;
        case 0x67a4u: /* ldrb r4,[r0,#0x21] */
            LOAD(r[4],r[0]+33u,1);r[15]=0x67a8u;return 0;
        case 0x67a8u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x67acu;return 0;
        case 0x67acu: /* beq 0x000067bc */
            if(state->cpsr&0x40000000u){r[15]=26556u;}else{r[15]=0x67b0u;}return 0;
        case 0x67b0u: /* sub r4,r4,#0x1 */
            r[4]=r[4]-1u;r[15]=0x67b4u;return 0;
        case 0x67b4u: /* strb r4,[r0,#0x21] */
            STORE(r[0]+33u,r[4],1);r[15]=0x67b8u;return 0;
        case 0x67b8u: /* b 0x0000682c */
            r[15]=26668u;return 0;
        case 0x67bcu: /* ldrb r4,[r0,#0x20] */
            LOAD(r[4],r[0]+32u,1);r[15]=0x67c0u;return 0;
        case 0x67c0u: /* strb r4,[r0,#0x21] */
            STORE(r[0]+33u,r[4],1);r[15]=0x67c4u;return 0;
        case 0x67c4u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x67c8u;return 0;
        case 0x67c8u: /* moveq r3,r2 */
            if(state->cpsr&0x40000000u){r[3]=r[2];}r[15]=0x67ccu;return 0;
        case 0x67ccu: /* streq r2,[r0,#0x1c] */
            if(state->cpsr&0x40000000u){STORE(r[0]+28u,r[2],4);}r[15]=0x67d0u;return 0;
        case 0x67d0u: /* beq 0x0000680c */
            if(state->cpsr&0x40000000u){r[15]=26636u;}else{r[15]=0x67d4u;}return 0;
        case 0x67d4u: /* ldrb r4,[r0,#0x22] */
            LOAD(r[4],r[0]+34u,1);r[15]=0x67d8u;return 0;
        case 0x67d8u: /* mov r4,r4, lsl #0x4 */
            r[4]=(r[4]<<4);r[15]=0x67dcu;return 0;
        case 0x67dcu: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x67e0u;return 0;
        case 0x67e0u: /* bcs 0x000067f8 */
            if(state->cpsr&0x20000000u){r[15]=26616u;}else{r[15]=0x67e4u;}return 0;
        case 0x67e4u: /* add r3,r3,r4 */
            r[3]=r[3]+r[4];r[15]=0x67e8u;return 0;
        case 0x67e8u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x67ecu;return 0;
        case 0x67ecu: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x67f0u;return 0;
        case 0x67f0u: /* str r3,[r0,#0x1c] */
            STORE(r[0]+28u,r[3],4);r[15]=0x67f4u;return 0;
        case 0x67f4u: /* b 0x0000680c */
            r[15]=26636u;return 0;
        case 0x67f8u: /* subs r3,r3,r4 */
            r[3]=subtract(state,r[3],r[4]);r[15]=0x67fcu;return 0;
        case 0x67fcu: /* movcc r3,#0x0 */
            if(!(state->cpsr&0x20000000u))r[3]=0;r[15]=0x6800u;return 0;
        case 0x6800u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x6804u;return 0;
        case 0x6804u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x6808u;return 0;
        case 0x6808u: /* str r3,[r0,#0x1c] */
            STORE(r[0]+28u,r[3],4);r[15]=0x680cu;return 0;
        case 0x680cu: /* ldr r2,[0x6848] */
            LOAD(r[2],26696u,4);r[15]=0x6810u;return 0;
        case 0x6810u: /* and r3,r3,r2 */
            r[3]=r[3]&r[2];r[15]=0x6814u;return 0;
        case 0x6814u: /* sub r3,r3,#0x8000 */
            r[3]=r[3]-32768u;r[15]=0x6818u;return 0;
        case 0x6818u: /* mov r2,#0x4000 */
            r[2]=16384u;r[15]=0x681cu;return 0;
        case 0x681cu: /* adds r3,r3,r2 */
            r[3]=addition(state,r[3],r[2]);r[15]=0x6820u;return 0;
        case 0x6820u: /* movmi r3,#0x0 */
            if(state->cpsr&0x80000000u){r[3]=0u;}r[15]=0x6824u;return 0;
        case 0x6824u: /* str r3,[r11,#0x3c] */
            STORE(r[11]+60u,r[3],4);r[15]=0x6828u;return 0;
        case 0x6828u: /* orr r5,r5,#0x8 */
            r[5]=r[5]|8u;r[15]=0x682cu;return 0;
        case 0x682cu: /* strb r5,[r0,#0x28] */
            STORE(r[0]+40u,r[5],1);r[15]=0x6830u;return 0;
        case 0x6830u: /* add r0,r0,#0x80 */
            r[0]=r[0]+128u;r[15]=0x6834u;return 0;
        case 0x6834u: /* add r11,r11,#0x40 */
            r[11]=r[11]+64u;r[15]=0x6838u;return 0;
        case 0x6838u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x683cu;return 0;
        case 0x683cu: /* bne 0x0000678c */
            if(!(state->cpsr&0x40000000u)){r[15]=26508u;}else{r[15]=0x6840u;}return 0;
        case 0x6840u: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}else{switch(r[15]){
        case 0x6628u: /* ldr r0,[0x6644] */
            LOAD(r[0],26180u,4);r[15]=0x662cu;return 0;
        case 0x662cu: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x6630u;return 0;
        case 0x6630u: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x6634u;return 0;
        case 0x6634u: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x6638u;return 0;
        case 0x6638u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6648u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x664cu;return 0;
        case 0x664cu: /* mov r1,#0x8 */
            r[1]=8u;r[15]=0x6650u;return 0;
        case 0x6650u: /* ldrb r5,[r0,#0x28] */
            LOAD(r[5],r[0]+40u,1);r[15]=0x6654u;return 0;
        case 0x6654u: /* ldrb r2,[r0,#0x0] */
            LOAD(r[2],r[0]+0u,1);r[15]=0x6658u;return 0;
        case 0x6658u: /* ldrb r3,[r0,#0x1] */
            LOAD(r[3],r[0]+1u,1);r[15]=0x665cu;return 0;
        case 0x665cu: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x6660u;return 0;
        case 0x6660u: /* biceq r5,r5,#0x4 */
            if(state->cpsr&0x40000000u){r[5]=r[5]&~4u;}r[15]=0x6664u;return 0;
        case 0x6664u: /* beq 0x000066d4 */
            if(state->cpsr&0x40000000u){r[15]=26324u;}else{r[15]=0x6668u;}return 0;
        case 0x6668u: /* ldrb r4,[r0,#0x3] */
            LOAD(r[4],r[0]+3u,1);r[15]=0x666cu;return 0;
        case 0x666cu: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6670u;return 0;
        case 0x6670u: /* beq 0x00006680 */
            if(state->cpsr&0x40000000u){r[15]=26240u;}else{r[15]=0x6674u;}return 0;
        case 0x6674u: /* sub r4,r4,#0x1 */
            r[4]=r[4]-1u;r[15]=0x6678u;return 0;
        case 0x6678u: /* strb r4,[r0,#0x3] */
            STORE(r[0]+3u,r[4],1);r[15]=0x667cu;return 0;
        case 0x667cu: /* b 0x000066d4 */
            r[15]=26324u;return 0;
        case 0x6680u: /* ldrb r4,[r0,#0x2] */
            LOAD(r[4],r[0]+2u,1);r[15]=0x6684u;return 0;
        case 0x6684u: /* strb r4,[r0,#0x3] */
            STORE(r[0]+3u,r[4],1);r[15]=0x6688u;return 0;
        case 0x6688u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x668cu;return 0;
        case 0x668cu: /* strbeq r2,[r0,#0x1] */
            if(state->cpsr&0x40000000u){STORE(r[0]+1u,r[2],1);}r[15]=0x6690u;return 0;
        case 0x6690u: /* beq 0x000066c8 */
            if(state->cpsr&0x40000000u){r[15]=26312u;}else{r[15]=0x6694u;}return 0;
        case 0x6694u: /* ldrb r4,[r0,#0x4] */
            LOAD(r[4],r[0]+4u,1);r[15]=0x6698u;return 0;
        case 0x6698u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x669cu;return 0;
        case 0x669cu: /* bcs 0x000066b4 */
            if(state->cpsr&0x20000000u){r[15]=26292u;}else{r[15]=0x66a0u;}return 0;
        case 0x66a0u: /* add r3,r3,r4 */
            r[3]=r[3]+r[4];r[15]=0x66a4u;return 0;
        case 0x66a4u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x66a8u;return 0;
        case 0x66a8u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x66acu;return 0;
        case 0x66acu: /* strb r3,[r0,#0x1] */
            STORE(r[0]+1u,r[3],1);r[15]=0x66b0u;return 0;
        case 0x66b0u: /* b 0x000066c8 */
            r[15]=26312u;return 0;
        case 0x66b4u: /* subs r3,r3,r4 */
            r[3]=subtract(state,r[3],r[4]);r[15]=0x66b8u;return 0;
        case 0x66b8u: /* movcc r3,#0x0 */
            if(!(state->cpsr&0x20000000u))r[3]=0;r[15]=0x66bcu;return 0;
        case 0x66bcu: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x66c0u;return 0;
        case 0x66c0u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x66c4u;return 0;
        case 0x66c4u: /* strb r3,[r0,#0x1] */
            STORE(r[0]+1u,r[3],1);r[15]=0x66c8u;return 0;
        case 0x66c8u: /* mov r3,#0x1 */
            r[3]=1u;r[15]=0x66ccu;return 0;
        case 0x66ccu: /* strb r3,[r0,#0x5] */
            STORE(r[0]+5u,r[3],1);r[15]=0x66d0u;return 0;
        case 0x66d0u: /* orr r5,r5,#0x4 */
            r[5]=r[5]|4u;r[15]=0x66d4u;return 0;
        case 0x66d4u: /* strb r5,[r0,#0x28] */
            STORE(r[0]+40u,r[5],1);r[15]=0x66d8u;return 0;
        case 0x66d8u: /* add r0,r0,#0x80 */
            r[0]=r[0]+128u;r[15]=0x66dcu;return 0;
        case 0x66dcu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x66e0u;return 0;
        case 0x66e0u: /* bne 0x00006650 */
            if(!(state->cpsr&0x40000000u)){r[15]=26192u;}else{r[15]=0x66e4u;}return 0;
        case 0x66e4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x66e8u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x66ecu;return 0;
        case 0x66ecu: /* mov r1,#0x8 */
            r[1]=8u;r[15]=0x66f0u;return 0;
        case 0x66f0u: /* ldrb r5,[r0,#0x28] */
            LOAD(r[5],r[0]+40u,1);r[15]=0x66f4u;return 0;
        case 0x66f4u: /* ldrb r2,[r0,#0x6] */
            LOAD(r[2],r[0]+6u,1);r[15]=0x66f8u;return 0;
        case 0x66f8u: /* ldrb r3,[r0,#0x7] */
            LOAD(r[3],r[0]+7u,1);r[15]=0x66fcu;return 0;
        case 0x66fcu: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x6700u;return 0;
        case 0x6700u: /* biceq r5,r5,#0x10 */
            if(state->cpsr&0x40000000u){r[5]=r[5]&~16u;}r[15]=0x6704u;return 0;
        case 0x6704u: /* beq 0x00006774 */
            if(state->cpsr&0x40000000u){r[15]=26484u;}else{r[15]=0x6708u;}return 0;
        case 0x6708u: /* ldrb r4,[r0,#0x9] */
            LOAD(r[4],r[0]+9u,1);r[15]=0x670cu;return 0;
        case 0x670cu: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6710u;return 0;
        case 0x6710u: /* beq 0x00006720 */
            if(state->cpsr&0x40000000u){r[15]=26400u;}else{r[15]=0x6714u;}return 0;
        case 0x6714u: /* sub r4,r4,#0x1 */
            r[4]=r[4]-1u;r[15]=0x6718u;return 0;
        case 0x6718u: /* strb r4,[r0,#0x9] */
            STORE(r[0]+9u,r[4],1);r[15]=0x671cu;return 0;
        case 0x671cu: /* b 0x00006774 */
            r[15]=26484u;return 0;
        case 0x6720u: /* ldrb r4,[r0,#0x8] */
            LOAD(r[4],r[0]+8u,1);r[15]=0x6724u;return 0;
        case 0x6724u: /* strb r4,[r0,#0x9] */
            STORE(r[0]+9u,r[4],1);r[15]=0x6728u;return 0;
        case 0x6728u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x672cu;return 0;
        case 0x672cu: /* strbeq r2,[r0,#0x7] */
            if(state->cpsr&0x40000000u){STORE(r[0]+7u,r[2],1);}r[15]=0x6730u;return 0;
        case 0x6730u: /* beq 0x00006768 */
            if(state->cpsr&0x40000000u){r[15]=26472u;}else{r[15]=0x6734u;}return 0;
        case 0x6734u: /* ldrb r4,[r0,#0xa] */
            LOAD(r[4],r[0]+10u,1);r[15]=0x6738u;return 0;
        case 0x6738u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x673cu;return 0;
        case 0x673cu: /* bcs 0x00006754 */
            if(state->cpsr&0x20000000u){r[15]=26452u;}else{r[15]=0x6740u;}return 0;
        case 0x6740u: /* add r3,r3,r4 */
            r[3]=r[3]+r[4];r[15]=0x6744u;return 0;
        case 0x6744u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x6748u;return 0;
        case 0x6748u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x674cu;return 0;
        case 0x674cu: /* strb r3,[r0,#0x7] */
            STORE(r[0]+7u,r[3],1);r[15]=0x6750u;return 0;
        case 0x6750u: /* b 0x00006768 */
            r[15]=26472u;return 0;
        case 0x6754u: /* subs r3,r3,r4 */
            r[3]=subtract(state,r[3],r[4]);r[15]=0x6758u;return 0;
        case 0x6758u: /* movcc r3,#0x0 */
            if(!(state->cpsr&0x20000000u))r[3]=0;r[15]=0x675cu;return 0;
        case 0x675cu: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x6760u;return 0;
        case 0x6760u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x6764u;return 0;
        case 0x6764u: /* strb r3,[r0,#0x7] */
            STORE(r[0]+7u,r[3],1);r[15]=0x6768u;return 0;
        case 0x6768u: /* mov r3,#0x1 */
            r[3]=1u;r[15]=0x676cu;return 0;
        case 0x676cu: /* strb r3,[r0,#0xb] */
            STORE(r[0]+11u,r[3],1);r[15]=0x6770u;return 0;
        case 0x6770u: /* orr r5,r5,#0x4 */
            r[5]=r[5]|4u;r[15]=0x6774u;return 0;
        case 0x6774u: /* strb r5,[r0,#0x28] */
            STORE(r[0]+40u,r[5],1);r[15]=0x6778u;return 0;
        case 0x6778u: /* add r0,r0,#0x80 */
            r[0]=r[0]+128u;r[15]=0x677cu;return 0;
        case 0x677cu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x6780u;return 0;
        case 0x6780u: /* bne 0x000066f0 */
            if(!(state->cpsr&0x40000000u)){r[15]=26352u;}else{r[15]=0x6784u;}return 0;
        case 0x6784u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6788u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x678cu;return 0;
        case 0x678cu: /* mov r1,#0x8 */
            r[1]=8u;r[15]=0x6790u;return 0;
        case 0x6790u: /* ldr r11,[0x684c] */
            LOAD(r[11],26700u,4);r[15]=0x6794u;return 0;
        case 0x6794u: /* ldrb r5,[r0,#0x28] */
            LOAD(r[5],r[0]+40u,1);r[15]=0x6798u;return 0;
        case 0x6798u: /* ldr r2,[r0,#0x18] */
            LOAD(r[2],r[0]+24u,4);r[15]=0x679cu;return 0;
        case 0x679cu: /* ldr r3,[r0,#0x1c] */
            LOAD(r[3],r[0]+28u,4);r[15]=0x67a0u;return 0;
        case 0x67a0u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x67a4u;return 0;
        case 0x67a4u: /* biceq r5,r5,#0x8 */
            if(state->cpsr&0x40000000u){r[5]=r[5]&~8u;}r[15]=0x67a8u;return 0;
        case 0x67a8u: /* beq 0x00006834 */
            if(state->cpsr&0x40000000u){r[15]=26676u;}else{r[15]=0x67acu;}return 0;
        case 0x67acu: /* ldrb r4,[r0,#0x21] */
            LOAD(r[4],r[0]+33u,1);r[15]=0x67b0u;return 0;
        case 0x67b0u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x67b4u;return 0;
        case 0x67b4u: /* beq 0x000067c4 */
            if(state->cpsr&0x40000000u){r[15]=26564u;}else{r[15]=0x67b8u;}return 0;
        case 0x67b8u: /* sub r4,r4,#0x1 */
            r[4]=r[4]-1u;r[15]=0x67bcu;return 0;
        case 0x67bcu: /* strb r4,[r0,#0x21] */
            STORE(r[0]+33u,r[4],1);r[15]=0x67c0u;return 0;
        case 0x67c0u: /* b 0x00006834 */
            r[15]=26676u;return 0;
        case 0x67c4u: /* ldrb r4,[r0,#0x20] */
            LOAD(r[4],r[0]+32u,1);r[15]=0x67c8u;return 0;
        case 0x67c8u: /* strb r4,[r0,#0x21] */
            STORE(r[0]+33u,r[4],1);r[15]=0x67ccu;return 0;
        case 0x67ccu: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x67d0u;return 0;
        case 0x67d0u: /* moveq r3,r2 */
            if(state->cpsr&0x40000000u){r[3]=r[2];}r[15]=0x67d4u;return 0;
        case 0x67d4u: /* streq r2,[r0,#0x1c] */
            if(state->cpsr&0x40000000u){STORE(r[0]+28u,r[2],4);}r[15]=0x67d8u;return 0;
        case 0x67d8u: /* beq 0x00006814 */
            if(state->cpsr&0x40000000u){r[15]=26644u;}else{r[15]=0x67dcu;}return 0;
        case 0x67dcu: /* ldrb r4,[r0,#0x22] */
            LOAD(r[4],r[0]+34u,1);r[15]=0x67e0u;return 0;
        case 0x67e0u: /* mov r4,r4, lsl #0x4 */
            r[4]=(r[4]<<4);r[15]=0x67e4u;return 0;
        case 0x67e4u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x67e8u;return 0;
        case 0x67e8u: /* bcs 0x00006800 */
            if(state->cpsr&0x20000000u){r[15]=26624u;}else{r[15]=0x67ecu;}return 0;
        case 0x67ecu: /* add r3,r3,r4 */
            r[3]=r[3]+r[4];r[15]=0x67f0u;return 0;
        case 0x67f0u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x67f4u;return 0;
        case 0x67f4u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x67f8u;return 0;
        case 0x67f8u: /* str r3,[r0,#0x1c] */
            STORE(r[0]+28u,r[3],4);r[15]=0x67fcu;return 0;
        case 0x67fcu: /* b 0x00006814 */
            r[15]=26644u;return 0;
        case 0x6800u: /* subs r3,r3,r4 */
            r[3]=subtract(state,r[3],r[4]);r[15]=0x6804u;return 0;
        case 0x6804u: /* movcc r3,#0x0 */
            if(!(state->cpsr&0x20000000u))r[3]=0;r[15]=0x6808u;return 0;
        case 0x6808u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x680cu;return 0;
        case 0x680cu: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x6810u;return 0;
        case 0x6810u: /* str r3,[r0,#0x1c] */
            STORE(r[0]+28u,r[3],4);r[15]=0x6814u;return 0;
        case 0x6814u: /* ldr r2,[0x6850] */
            LOAD(r[2],26704u,4);r[15]=0x6818u;return 0;
        case 0x6818u: /* and r3,r3,r2 */
            r[3]=r[3]&r[2];r[15]=0x681cu;return 0;
        case 0x681cu: /* sub r3,r3,#0x8000 */
            r[3]=r[3]-32768u;r[15]=0x6820u;return 0;
        case 0x6820u: /* mov r2,#0x4000 */
            r[2]=16384u;r[15]=0x6824u;return 0;
        case 0x6824u: /* adds r3,r3,r2 */
            r[3]=addition(state,r[3],r[2]);r[15]=0x6828u;return 0;
        case 0x6828u: /* movmi r3,#0x0 */
            if(state->cpsr&0x80000000u){r[3]=0u;}r[15]=0x682cu;return 0;
        case 0x682cu: /* str r3,[r11,#0x3c] */
            STORE(r[11]+60u,r[3],4);r[15]=0x6830u;return 0;
        case 0x6830u: /* orr r5,r5,#0x8 */
            r[5]=r[5]|8u;r[15]=0x6834u;return 0;
        case 0x6834u: /* strb r5,[r0,#0x28] */
            STORE(r[0]+40u,r[5],1);r[15]=0x6838u;return 0;
        case 0x6838u: /* add r0,r0,#0x80 */
            r[0]=r[0]+128u;r[15]=0x683cu;return 0;
        case 0x683cu: /* add r11,r11,#0x40 */
            r[11]=r[11]+64u;r[15]=0x6840u;return 0;
        case 0x6840u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x6844u;return 0;
        case 0x6844u: /* bne 0x00006794 */
            if(!(state->cpsr&0x40000000u)){r[15]=26516u;}else{r[15]=0x6848u;}return 0;
        case 0x6848u: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}
}

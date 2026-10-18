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
int sa_arm_sound_effect_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
        case 0x684cu: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x6850u;return 0;
        case 0x6850u: /* mov r1,#0x8 */
            r[1]=8u;r[15]=0x6854u;return 0;
        case 0x6854u: /* ldrb r5,[r0,#0x28] */
            LOAD(r[5],r[0]+40u,1);r[15]=0x6858u;return 0;
        case 0x6858u: /* ldr r2,[r0,#0xc] */
            LOAD(r[2],r[0]+12u,4);r[15]=0x685cu;return 0;
        case 0x685cu: /* ldr r3,[r0,#0x10] */
            LOAD(r[3],r[0]+16u,4);r[15]=0x6860u;return 0;
        case 0x6860u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x6864u;return 0;
        case 0x6864u: /* biceq r5,r5,#0x20 */
            if(state->cpsr&0x40000000u){r[5]=r[5]&~32u;}r[15]=0x6868u;return 0;
        case 0x6868u: /* beq 0x000068e0 */
            if(state->cpsr&0x40000000u){r[15]=26848u;}else{r[15]=0x686cu;}return 0;
        case 0x686cu: /* ldrb r4,[r0,#0x15] */
            LOAD(r[4],r[0]+21u,1);r[15]=0x6870u;return 0;
        case 0x6870u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6874u;return 0;
        case 0x6874u: /* beq 0x00006884 */
            if(state->cpsr&0x40000000u){r[15]=26756u;}else{r[15]=0x6878u;}return 0;
        case 0x6878u: /* sub r4,r4,#0x1 */
            r[4]=r[4]-1u;r[15]=0x687cu;return 0;
        case 0x687cu: /* strb r4,[r0,#0x15] */
            STORE(r[0]+21u,r[4],1);r[15]=0x6880u;return 0;
        case 0x6880u: /* b 0x000068e0 */
            r[15]=26848u;return 0;
        case 0x6884u: /* ldrb r4,[r0,#0x14] */
            LOAD(r[4],r[0]+20u,1);r[15]=0x6888u;return 0;
        case 0x6888u: /* strb r4,[r0,#0x15] */
            STORE(r[0]+21u,r[4],1);r[15]=0x688cu;return 0;
        case 0x688cu: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6890u;return 0;
        case 0x6890u: /* moveq r3,r2 */
            if(state->cpsr&0x40000000u){r[3]=r[2];}r[15]=0x6894u;return 0;
        case 0x6894u: /* streq r2,[r0,#0x10] */
            if(state->cpsr&0x40000000u){STORE(r[0]+16u,r[2],4);}r[15]=0x6898u;return 0;
        case 0x6898u: /* beq 0x000068d4 */
            if(state->cpsr&0x40000000u){r[15]=26836u;}else{r[15]=0x689cu;}return 0;
        case 0x689cu: /* ldrb r4,[r0,#0x16] */
            LOAD(r[4],r[0]+22u,1);r[15]=0x68a0u;return 0;
        case 0x68a0u: /* mov r4,r4, lsl #0x1 */
            r[4]=(r[4]<<1);r[15]=0x68a4u;return 0;
        case 0x68a4u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x68a8u;return 0;
        case 0x68a8u: /* bcs 0x000068c0 */
            if(state->cpsr&0x20000000u){r[15]=26816u;}else{r[15]=0x68acu;}return 0;
        case 0x68acu: /* add r3,r3,r4 */
            r[3]=r[3]+r[4];r[15]=0x68b0u;return 0;
        case 0x68b0u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x68b4u;return 0;
        case 0x68b4u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x68b8u;return 0;
        case 0x68b8u: /* str r3,[r0,#0x10] */
            STORE(r[0]+16u,r[3],4);r[15]=0x68bcu;return 0;
        case 0x68bcu: /* b 0x000068d4 */
            r[15]=26836u;return 0;
        case 0x68c0u: /* subs r3,r3,r4 */
            r[3]=subtract(state,r[3],r[4]);r[15]=0x68c4u;return 0;
        case 0x68c4u: /* movcs r3,#0x0 */
            if(state->cpsr&0x20000000u){r[3]=0u;}r[15]=0x68c8u;return 0;
        case 0x68c8u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x68ccu;return 0;
        case 0x68ccu: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x68d0u;return 0;
        case 0x68d0u: /* str r3,[r0,#0x10] */
            STORE(r[0]+16u,r[3],4);r[15]=0x68d4u;return 0;
        case 0x68d4u: /* mov r3,#0x1 */
            r[3]=1u;r[15]=0x68d8u;return 0;
        case 0x68d8u: /* strb r3,[r0,#0x17] */
            STORE(r[0]+23u,r[3],1);r[15]=0x68dcu;return 0;
        case 0x68dcu: /* orr r5,r5,#0x20 */
            r[5]=r[5]|32u;r[15]=0x68e0u;return 0;
        case 0x68e0u: /* strb r5,[r0,#0x28] */
            STORE(r[0]+40u,r[5],1);r[15]=0x68e4u;return 0;
        case 0x68e4u: /* add r0,r0,#0x80 */
            r[0]=r[0]+128u;r[15]=0x68e8u;return 0;
        case 0x68e8u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x68ecu;return 0;
        case 0x68ecu: /* bne 0x00006854 */
            if(!(state->cpsr&0x40000000u)){r[15]=26708u;}else{r[15]=0x68f0u;}return 0;
        case 0x68f0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x68f8u: /* mov r12,#0x11000 */
            r[12]=69632u;r[15]=0x68fcu;return 0;
        case 0x68fcu: /* mov r11,#0x18 */
            r[11]=24u;r[15]=0x6900u;return 0;
        case 0x6900u: /* ldrb r5,[r12,#0x0] */
            LOAD(r[5],r[12]+0u,1);r[15]=0x6904u;return 0;
        case 0x6904u: /* ldr r2,[r12,#0x40] */
            LOAD(r[2],r[12]+64u,4);r[15]=0x6908u;return 0;
        case 0x6908u: /* ldr r3,[r12,#0x44] */
            LOAD(r[3],r[12]+68u,4);r[15]=0x690cu;return 0;
        case 0x690cu: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x6910u;return 0;
        case 0x6910u: /* biceq r5,r5,#0x8 */
            if(state->cpsr&0x40000000u){r[5]=r[5]&~8u;}r[15]=0x6914u;return 0;
        case 0x6914u: /* beq 0x00006988 */
            if(state->cpsr&0x40000000u){r[15]=27016u;}else{r[15]=0x6918u;}return 0;
        case 0x6918u: /* ldrb r4,[r12,#0x49] */
            LOAD(r[4],r[12]+73u,1);r[15]=0x691cu;return 0;
        case 0x691cu: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6920u;return 0;
        case 0x6920u: /* beq 0x00006930 */
            if(state->cpsr&0x40000000u){r[15]=26928u;}else{r[15]=0x6924u;}return 0;
        case 0x6924u: /* sub r4,r4,#0x1 */
            r[4]=r[4]-1u;r[15]=0x6928u;return 0;
        case 0x6928u: /* strb r4,[r12,#0x49] */
            STORE(r[12]+73u,r[4],1);r[15]=0x692cu;return 0;
        case 0x692cu: /* b 0x00006988 */
            r[15]=27016u;return 0;
        case 0x6930u: /* ldrb r4,[r12,#0x48] */
            LOAD(r[4],r[12]+72u,1);r[15]=0x6934u;return 0;
        case 0x6934u: /* strb r4,[r12,#0x49] */
            STORE(r[12]+73u,r[4],1);r[15]=0x6938u;return 0;
        case 0x6938u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x693cu;return 0;
        case 0x693cu: /* streq r2,[r12,#0x44] */
            if(state->cpsr&0x40000000u){STORE(r[12]+68u,r[2],4);}r[15]=0x6940u;return 0;
        case 0x6940u: /* beq 0x0000697c */
            if(state->cpsr&0x40000000u){r[15]=27004u;}else{r[15]=0x6944u;}return 0;
        case 0x6944u: /* ldrb r4,[r12,#0x4a] */
            LOAD(r[4],r[12]+74u,1);r[15]=0x6948u;return 0;
        case 0x6948u: /* mov r4,r4, lsl #0x1 */
            r[4]=(r[4]<<1);r[15]=0x694cu;return 0;
        case 0x694cu: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x6950u;return 0;
        case 0x6950u: /* bcs 0x00006968 */
            if(state->cpsr&0x20000000u){r[15]=26984u;}else{r[15]=0x6954u;}return 0;
        case 0x6954u: /* add r3,r3,r4 */
            r[3]=r[3]+r[4];r[15]=0x6958u;return 0;
        case 0x6958u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x695cu;return 0;
        case 0x695cu: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x6960u;return 0;
        case 0x6960u: /* str r3,[r12,#0x44] */
            STORE(r[12]+68u,r[3],4);r[15]=0x6964u;return 0;
        case 0x6964u: /* b 0x0000697c */
            r[15]=27004u;return 0;
        case 0x6968u: /* subs r3,r3,r4 */
            r[3]=subtract(state,r[3],r[4]);r[15]=0x696cu;return 0;
        case 0x696cu: /* movcs r3,#0x0 */
            if(state->cpsr&0x20000000u){r[3]=0u;}r[15]=0x6970u;return 0;
        case 0x6970u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x6974u;return 0;
        case 0x6974u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x6978u;return 0;
        case 0x6978u: /* str r3,[r12,#0x44] */
            STORE(r[12]+68u,r[3],4);r[15]=0x697cu;return 0;
        case 0x697cu: /* mov r3,#0xff */
            r[3]=255u;r[15]=0x6980u;return 0;
        case 0x6980u: /* strb r3,[r12,#0x4b] */
            STORE(r[12]+75u,r[3],1);r[15]=0x6984u;return 0;
        case 0x6984u: /* orr r5,r5,#0x8 */
            r[5]=r[5]|8u;r[15]=0x6988u;return 0;
        case 0x6988u: /* strb r5,[r12,#0x0] */
            STORE(r[12]+0u,r[5],1);r[15]=0x698cu;return 0;
        case 0x698cu: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x6990u;return 0;
        case 0x6990u: /* subs r11,r11,#0x1 */
            r[11]=subtract(state,r[11],1u);r[15]=0x6994u;return 0;
        case 0x6994u: /* bne 0x00006900 */
            if(!(state->cpsr&0x40000000u)){r[15]=26880u;}else{r[15]=0x6998u;}return 0;
        case 0x6998u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x69a8u: /* mov r12,#0x11000 */
            r[12]=69632u;r[15]=0x69acu;return 0;
        case 0x69acu: /* mov r11,#0x18 */
            r[11]=24u;r[15]=0x69b0u;return 0;
        case 0x69b0u: /* ldrb r5,[r12,#0x0] */
            LOAD(r[5],r[12]+0u,1);r[15]=0x69b4u;return 0;
        case 0x69b4u: /* ldr r2,[r12,#0x4c] */
            LOAD(r[2],r[12]+76u,4);r[15]=0x69b8u;return 0;
        case 0x69b8u: /* ldr r3,[r12,#0x50] */
            LOAD(r[3],r[12]+80u,4);r[15]=0x69bcu;return 0;
        case 0x69bcu: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x69c0u;return 0;
        case 0x69c0u: /* biceq r5,r5,#0x20 */
            if(state->cpsr&0x40000000u){r[5]=r[5]&~32u;}r[15]=0x69c4u;return 0;
        case 0x69c4u: /* beq 0x00006a38 */
            if(state->cpsr&0x40000000u){r[15]=27192u;}else{r[15]=0x69c8u;}return 0;
        case 0x69c8u: /* ldrb r4,[r12,#0x55] */
            LOAD(r[4],r[12]+85u,1);r[15]=0x69ccu;return 0;
        case 0x69ccu: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x69d0u;return 0;
        case 0x69d0u: /* beq 0x000069e0 */
            if(state->cpsr&0x40000000u){r[15]=27104u;}else{r[15]=0x69d4u;}return 0;
        case 0x69d4u: /* sub r4,r4,#0x1 */
            r[4]=r[4]-1u;r[15]=0x69d8u;return 0;
        case 0x69d8u: /* strb r4,[r12,#0x55] */
            STORE(r[12]+85u,r[4],1);r[15]=0x69dcu;return 0;
        case 0x69dcu: /* b 0x00006a38 */
            r[15]=27192u;return 0;
        case 0x69e0u: /* ldrb r4,[r12,#0x54] */
            LOAD(r[4],r[12]+84u,1);r[15]=0x69e4u;return 0;
        case 0x69e4u: /* strb r4,[r12,#0x55] */
            STORE(r[12]+85u,r[4],1);r[15]=0x69e8u;return 0;
        case 0x69e8u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x69ecu;return 0;
        case 0x69ecu: /* streq r2,[r12,#0x50] */
            if(state->cpsr&0x40000000u){STORE(r[12]+80u,r[2],4);}r[15]=0x69f0u;return 0;
        case 0x69f0u: /* beq 0x00006a2c */
            if(state->cpsr&0x40000000u){r[15]=27180u;}else{r[15]=0x69f4u;}return 0;
        case 0x69f4u: /* ldrb r4,[r12,#0x56] */
            LOAD(r[4],r[12]+86u,1);r[15]=0x69f8u;return 0;
        case 0x69f8u: /* mov r4,r4, lsl #0x1 */
            r[4]=(r[4]<<1);r[15]=0x69fcu;return 0;
        case 0x69fcu: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x6a00u;return 0;
        case 0x6a00u: /* bcs 0x00006a18 */
            if(state->cpsr&0x20000000u){r[15]=27160u;}else{r[15]=0x6a04u;}return 0;
        case 0x6a04u: /* add r3,r3,r4 */
            r[3]=r[3]+r[4];r[15]=0x6a08u;return 0;
        case 0x6a08u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x6a0cu;return 0;
        case 0x6a0cu: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x6a10u;return 0;
        case 0x6a10u: /* str r3,[r12,#0x50] */
            STORE(r[12]+80u,r[3],4);r[15]=0x6a14u;return 0;
        case 0x6a14u: /* b 0x00006a2c */
            r[15]=27180u;return 0;
        case 0x6a18u: /* subs r3,r3,r4 */
            r[3]=subtract(state,r[3],r[4]);r[15]=0x6a1cu;return 0;
        case 0x6a1cu: /* movcc r3,#0x0 */
            if(!(state->cpsr&0x20000000u))r[3]=0;r[15]=0x6a20u;return 0;
        case 0x6a20u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x6a24u;return 0;
        case 0x6a24u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x6a28u;return 0;
        case 0x6a28u: /* str r3,[r12,#0x50] */
            STORE(r[12]+80u,r[3],4);r[15]=0x6a2cu;return 0;
        case 0x6a2cu: /* mov r3,#0xff */
            r[3]=255u;r[15]=0x6a30u;return 0;
        case 0x6a30u: /* strb r3,[r12,#0x4b] */
            STORE(r[12]+75u,r[3],1);r[15]=0x6a34u;return 0;
        case 0x6a34u: /* orr r5,r5,#0x20 */
            r[5]=r[5]|32u;r[15]=0x6a38u;return 0;
        case 0x6a38u: /* strb r5,[r12,#0x0] */
            STORE(r[12]+0u,r[5],1);r[15]=0x6a3cu;return 0;
        case 0x6a3cu: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x6a40u;return 0;
        case 0x6a40u: /* subs r11,r11,#0x1 */
            r[11]=subtract(state,r[11],1u);r[15]=0x6a44u;return 0;
        case 0x6a44u: /* bne 0x000069b0 */
            if(!(state->cpsr&0x40000000u)){r[15]=27056u;}else{r[15]=0x6a48u;}return 0;
        case 0x6a48u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6a5cu: /* mov r12,#0x11000 */
            r[12]=69632u;r[15]=0x6a60u;return 0;
        case 0x6a60u: /* mov r11,#0x18 */
            r[11]=24u;r[15]=0x6a64u;return 0;
        case 0x6a64u: /* ldrb r3,[r12,#0x4b] */
            LOAD(r[3],r[12]+75u,1);r[15]=0x6a68u;return 0;
        case 0x6a68u: /* tst r3,#0xff */
            temporary=r[3]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6a6cu;return 0;
        case 0x6a6cu: /* beq 0x00006ba8 */
            if(state->cpsr&0x40000000u){r[15]=27560u;}else{r[15]=0x6a70u;}return 0;
        case 0x6a70u: /* mov r3,#0x0 */
            r[3]=0u;r[15]=0x6a74u;return 0;
        case 0x6a74u: /* strb r3,[r12,#0x4b] */
            STORE(r[12]+75u,r[3],1);r[15]=0x6a78u;return 0;
        case 0x6a78u: /* ldr r4,[0x6bcc] */
            LOAD(r[4],27596u,4);r[15]=0x6a7cu;return 0;
        case 0x6a7cu: /* ldr r3,[r12,#0x44] */
            LOAD(r[3],r[12]+68u,4);r[15]=0x6a80u;return 0;
        case 0x6a80u: /* subs r3,r3,#0x8000 */
            r[3]=subtract(state,r[3],32768u);r[15]=0x6a84u;return 0;
        case 0x6a84u: /* eormi r3,r3,r4 */
            if(state->cpsr&0x80000000u){r[3]=r[3]^r[4];}r[15]=0x6a88u;return 0;
        case 0x6a88u: /* orrmi r3,r3,#0x8000 */
            if(state->cpsr&0x80000000u){r[3]=r[3]|32768u;}r[15]=0x6a8cu;return 0;
        case 0x6a8cu: /* ldr r4,[0x6d7c] */
            LOAD(r[4],28028u,4);r[15]=0x6a90u;return 0;
        case 0x6a90u: /* mov r0,r3, lsr #0x8 */
            r[0]=(r[3]>>8);r[15]=0x6a94u;return 0;
        case 0x6a94u: /* and r2,r0,#0x7f */
            r[2]=r[0]&127u;r[15]=0x6a98u;return 0;
        case 0x6a98u: /* add r2,r2,#0x60 */
            r[2]=r[2]+96u;r[15]=0x6a9cu;return 0;
        case 0x6a9cu: /* ldrb r2,[r4,r2] */
            LOAD(r[2],r[4]+r[2],1);r[15]=0x6aa0u;return 0;
        case 0x6aa0u: /* cmp r2,#0x7f */
            (void)subtract(state,r[2],127u);r[15]=0x6aa4u;return 0;
        case 0x6aa4u: /* ldreq r2,[0x6bd0] */
            if(state->cpsr&0x40000000u){LOAD(r[2],27600u,4);}r[15]=0x6aa8u;return 0;
        case 0x6aa8u: /* beq 0x00006af8 */
            if(state->cpsr&0x40000000u){r[15]=27384u;}else{r[15]=0x6aacu;}return 0;
        case 0x6aacu: /* and r5,r2,#0xf */
            r[5]=r[2]&15u;r[15]=0x6ab0u;return 0;
        case 0x6ab0u: /* mov r5,r5, lsl #0x7 */
            r[5]=(r[5]<<7);r[15]=0x6ab4u;return 0;
        case 0x6ab4u: /* mov r3,r3, lsr #0x1 */
            r[3]=(r[3]>>1);r[15]=0x6ab8u;return 0;
        case 0x6ab8u: /* and r3,r3,#0x7f */
            r[3]=r[3]&127u;r[15]=0x6abcu;return 0;
        case 0x6abcu: /* orr r3,r3,r5 */
            r[3]=r[3]|r[5];r[15]=0x6ac0u;return 0;
        case 0x6ac0u: /* ldr r4,[0x6d80] */
            LOAD(r[4],28032u,4);r[15]=0x6ac4u;return 0;
        case 0x6ac4u: /* ldrb r7,[r4,r3] */
            LOAD(r[7],r[4]+r[3],1);r[15]=0x6ac8u;return 0;
        case 0x6ac8u: /* ldr r4,[0x6bbc] */
            LOAD(r[4],27580u,4);r[15]=0x6accu;return 0;
        case 0x6accu: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x6ad0u;return 0;
        case 0x6ad0u: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x6ad4u;return 0;
        case 0x6ad4u: /* ldr r4,[0x6bc0] */
            LOAD(r[4],27584u,4);r[15]=0x6ad8u;return 0;
        case 0x6ad8u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x6adcu;return 0;
        case 0x6adcu: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x6ae0u;return 0;
        case 0x6ae0u: /* ldr r4,[0x6bc4] */
            LOAD(r[4],27588u,4);r[15]=0x6ae4u;return 0;
        case 0x6ae4u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x6ae8u;return 0;
        case 0x6ae8u: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x6aecu;return 0;
        case 0x6aecu: /* and r2,r2,#0xf0 */
            r[2]=r[2]&240u;r[15]=0x6af0u;return 0;
        case 0x6af0u: /* mov r2,r2, lsl #0x6 */
            r[2]=(r[2]<<6);r[15]=0x6af4u;return 0;
        case 0x6af4u: /* orr r2,r2,r7 */
            r[2]=r[2]|r[7];r[15]=0x6af8u;return 0;
        case 0x6af8u: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x6afcu;return 0;
        case 0x6afcu: /* ldr r3,[0x6bcc] */
            LOAD(r[3],27596u,4);r[15]=0x6b00u;return 0;
        case 0x6b00u: /* eorne r2,r2,r3 */
            if(!(state->cpsr&0x40000000u)){r[2]=r[2]^r[3];}r[15]=0x6b04u;return 0;
        case 0x6b04u: /* addne r2,r2,#0x1 */
            if(!(state->cpsr&0x40000000u)){r[2]=r[2]+1u;}r[15]=0x6b08u;return 0;
        case 0x6b08u: /* str r2,[0x6bc8] */
            STORE(27592u,r[2],4);r[15]=0x6b0cu;return 0;
        case 0x6b0cu: /* ldr r4,[0x6bcc] */
            LOAD(r[4],27596u,4);r[15]=0x6b10u;return 0;
        case 0x6b10u: /* ldr r3,[r12,#0x50] */
            LOAD(r[3],r[12]+80u,4);r[15]=0x6b14u;return 0;
        case 0x6b14u: /* subs r3,r3,#0x8000 */
            r[3]=subtract(state,r[3],32768u);r[15]=0x6b18u;return 0;
        case 0x6b18u: /* eormi r3,r3,r4 */
            if(state->cpsr&0x80000000u){r[3]=r[3]^r[4];}r[15]=0x6b1cu;return 0;
        case 0x6b1cu: /* orrmi r3,r3,#0x8000 */
            if(state->cpsr&0x80000000u){r[3]=r[3]|32768u;}r[15]=0x6b20u;return 0;
        case 0x6b20u: /* ldr r4,[0x6d7c] */
            LOAD(r[4],28028u,4);r[15]=0x6b24u;return 0;
        case 0x6b24u: /* mov r0,r3, lsr #0x8 */
            r[0]=(r[3]>>8);r[15]=0x6b28u;return 0;
        case 0x6b28u: /* and r2,r0,#0x7f */
            r[2]=r[0]&127u;r[15]=0x6b2cu;return 0;
        case 0x6b2cu: /* add r2,r2,#0x60 */
            r[2]=r[2]+96u;r[15]=0x6b30u;return 0;
        case 0x6b30u: /* ldrb r2,[r4,r2] */
            LOAD(r[2],r[4]+r[2],1);r[15]=0x6b34u;return 0;
        case 0x6b34u: /* cmp r2,#0x7f */
            (void)subtract(state,r[2],127u);r[15]=0x6b38u;return 0;
        case 0x6b38u: /* ldreq r2,[0x6bd0] */
            if(state->cpsr&0x40000000u){LOAD(r[2],27600u,4);}r[15]=0x6b3cu;return 0;
        case 0x6b3cu: /* beq 0x00006b8c */
            if(state->cpsr&0x40000000u){r[15]=27532u;}else{r[15]=0x6b40u;}return 0;
        case 0x6b40u: /* and r5,r2,#0xf */
            r[5]=r[2]&15u;r[15]=0x6b44u;return 0;
        case 0x6b44u: /* mov r5,r5, lsl #0x7 */
            r[5]=(r[5]<<7);r[15]=0x6b48u;return 0;
        case 0x6b48u: /* mov r3,r3, lsr #0x1 */
            r[3]=(r[3]>>1);r[15]=0x6b4cu;return 0;
        case 0x6b4cu: /* and r3,r3,#0x7f */
            r[3]=r[3]&127u;r[15]=0x6b50u;return 0;
        case 0x6b50u: /* orr r3,r3,r5 */
            r[3]=r[3]|r[5];r[15]=0x6b54u;return 0;
        case 0x6b54u: /* ldr r4,[0x6d80] */
            LOAD(r[4],28032u,4);r[15]=0x6b58u;return 0;
        case 0x6b58u: /* ldrb r7,[r4,r3] */
            LOAD(r[7],r[4]+r[3],1);r[15]=0x6b5cu;return 0;
        case 0x6b5cu: /* ldr r4,[0x6bbc] */
            LOAD(r[4],27580u,4);r[15]=0x6b60u;return 0;
        case 0x6b60u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x6b64u;return 0;
        case 0x6b64u: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x6b68u;return 0;
        case 0x6b68u: /* ldr r4,[0x6bc0] */
            LOAD(r[4],27584u,4);r[15]=0x6b6cu;return 0;
        case 0x6b6cu: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x6b70u;return 0;
        case 0x6b70u: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x6b74u;return 0;
        case 0x6b74u: /* ldr r4,[0x6bc4] */
            LOAD(r[4],27588u,4);r[15]=0x6b78u;return 0;
        case 0x6b78u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x6b7cu;return 0;
        case 0x6b7cu: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x6b80u;return 0;
        case 0x6b80u: /* and r2,r2,#0xf0 */
            r[2]=r[2]&240u;r[15]=0x6b84u;return 0;
        case 0x6b84u: /* mov r2,r2, lsl #0x6 */
            r[2]=(r[2]<<6);r[15]=0x6b88u;return 0;
        case 0x6b88u: /* orr r2,r2,r7 */
            r[2]=r[2]|r[7];r[15]=0x6b8cu;return 0;
        case 0x6b8cu: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x6b90u;return 0;
        case 0x6b90u: /* ldr r3,[0x6bcc] */
            LOAD(r[3],27596u,4);r[15]=0x6b94u;return 0;
        case 0x6b94u: /* eorne r2,r2,r3 */
            if(!(state->cpsr&0x40000000u)){r[2]=r[2]^r[3];}r[15]=0x6b98u;return 0;
        case 0x6b98u: /* addne r2,r2,#0x1 */
            if(!(state->cpsr&0x40000000u)){r[2]=r[2]+1u;}r[15]=0x6b9cu;return 0;
        case 0x6b9cu: /* ldr r3,[0x6bc8] */
            LOAD(r[3],27592u,4);r[15]=0x6ba0u;return 0;
        case 0x6ba0u: /* add r2,r2,r3 */
            r[2]=r[2]+r[3];r[15]=0x6ba4u;return 0;
        case 0x6ba4u: /* str r2,[r12,#0x10] */
            STORE(r[12]+16u,r[2],4);r[15]=0x6ba8u;return 0;
        case 0x6ba8u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x6bacu;return 0;
        case 0x6bacu: /* subs r11,r11,#0x1 */
            r[11]=subtract(state,r[11],1u);r[15]=0x6bb0u;return 0;
        case 0x6bb0u: /* bne 0x00006a64 */
            if(!(state->cpsr&0x40000000u)){r[15]=27236u;}else{r[15]=0x6bb4u;}return 0;
        case 0x6bb4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6bd4u: /* adr r12,0x6d3c */
            r[12]=27964u;r[15]=0x6bd8u;return 0;
        case 0x6bd8u: /* mov r5,#0xe800 */
            r[5]=59392u;r[15]=0x6bdcu;return 0;
        case 0x6bdcu: /* mov r10,#0x30 */
            r[10]=48u;r[15]=0x6be0u;return 0;
        case 0x6be0u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x6be4u;return 0;
        case 0x6be4u: /* ldr r2,[r5,#0x0] */
            LOAD(r[2],r[5]+0u,4);r[15]=0x6be8u;return 0;
        case 0x6be8u: /* movs r2,r2 */
            r[2]=r[2];logic_flags(state,r[2],0,0);r[15]=0x6becu;return 0;
        case 0x6becu: /* beq 0x00006d00 */
            if(state->cpsr&0x40000000u){r[15]=27904u;}else{r[15]=0x6bf0u;}return 0;
        case 0x6bf0u: /* ldrb r0,[r5,#0x0] */
            LOAD(r[0],r[5]+0u,1);r[15]=0x6bf4u;return 0;
        case 0x6bf4u: /* tst r0,#0xc0 */
            temporary=r[0]&192u;logic_flags(state,temporary,0,192u);r[15]=0x6bf8u;return 0;
        case 0x6bf8u: /* beq 0x00006c08 */
            if(state->cpsr&0x40000000u){r[15]=27656u;}else{r[15]=0x6bfcu;}return 0;
        case 0x6bfcu: /* ldrb r0,[r5,#0x3f] */
            LOAD(r[0],r[5]+63u,1);r[15]=0x6c00u;return 0;
        case 0x6c00u: /* add r0,r0,#0x1 */
            r[0]=r[0]+1u;r[15]=0x6c04u;return 0;
        case 0x6c04u: /* strb r0,[r5,#0x3f] */
            STORE(r[5]+63u,r[0],1);r[15]=0x6c08u;return 0;
        case 0x6c08u: /* ldrb r0,[r5,#0x4] */
            LOAD(r[0],r[5]+4u,1);r[15]=0x6c0cu;return 0;
        case 0x6c0cu: /* add r1,r12,r0, lsl #0x2 */
            r[1]=r[12]+(r[0]<<2);r[15]=0x6c10u;return 0;
        case 0x6c10u: /* ldr r0,[r1,#0x0] */
            LOAD(r[0],r[1]+0u,4);r[15]=0x6c14u;return 0;
        case 0x6c14u: /* ldrb r1,[r0,#0x5] */
            LOAD(r[1],r[0]+5u,1);r[15]=0x6c18u;return 0;
        case 0x6c18u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6c1cu;return 0;
        case 0x6c1cu: /* beq 0x00006c58 */
            if(state->cpsr&0x40000000u){r[15]=27736u;}else{r[15]=0x6c20u;}return 0;
        case 0x6c20u: /* ldrb r1,[r5,#0x8] */
            LOAD(r[1],r[5]+8u,1);r[15]=0x6c24u;return 0;
        case 0x6c24u: /* ldrb r2,[r0,#0x1] */
            LOAD(r[2],r[0]+1u,1);r[15]=0x6c28u;return 0;
        case 0x6c28u: /* sub r2,r2,#0x80 */
            r[2]=r[2]-128u;r[15]=0x6c2cu;return 0;
        case 0x6c2cu: /* adds r1,r1,r2 */
            r[1]=addition(state,r[1],r[2]);r[15]=0x6c30u;return 0;
        case 0x6c30u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x6c34u;return 0;
        case 0x6c34u: /* bmi 0x00006c40 */
            if(state->cpsr&0x80000000u){r[15]=27712u;}else{r[15]=0x6c38u;}return 0;
        case 0x6c38u: /* tst r1,#0x100 */
            temporary=r[1]&256u;logic_flags(state,temporary,1,256u);r[15]=0x6c3cu;return 0;
        case 0x6c3cu: /* movne r1,#0xff */
            if(!(state->cpsr&0x40000000u)){r[1]=255u;}r[15]=0x6c40u;return 0;
        case 0x6c40u: /* eor r1,r1,#0xff */
            r[1]=r[1]^255u;r[15]=0x6c44u;return 0;
        case 0x6c44u: /* and r1,r1,#0xff */
            r[1]=r[1]&255u;r[15]=0x6c48u;return 0;
        case 0x6c48u: /* ldr r2,[r8,#0x28] */
            LOAD(r[2],r[8]+40u,4);r[15]=0x6c4cu;return 0;
        case 0x6c4cu: /* and r2,r2,#0x1f */
            r[2]=r[2]&31u;r[15]=0x6c50u;return 0;
        case 0x6c50u: /* orr r2,r2,r1, lsl #0x8 */
            r[2]=r[2]|(r[1]<<8);r[15]=0x6c54u;return 0;
        case 0x6c54u: /* str r2,[r8,#0x28] */
            STORE(r[8]+40u,r[2],4);r[15]=0x6c58u;return 0;
        case 0x6c58u: /* ldrb r1,[r0,#0xb] */
            LOAD(r[1],r[0]+11u,1);r[15]=0x6c5cu;return 0;
        case 0x6c5cu: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6c60u;return 0;
        case 0x6c60u: /* beq 0x00006ca8 */
            if(state->cpsr&0x40000000u){r[15]=27816u;}else{r[15]=0x6c64u;}return 0;
        case 0x6c64u: /* ldrb r1,[r5,#0x9] */
            LOAD(r[1],r[5]+9u,1);r[15]=0x6c68u;return 0;
        case 0x6c68u: /* ldrb r2,[r0,#0x7] */
            LOAD(r[2],r[0]+7u,1);r[15]=0x6c6cu;return 0;
        case 0x6c6cu: /* sub r2,r2,#0x80 */
            r[2]=r[2]-128u;r[15]=0x6c70u;return 0;
        case 0x6c70u: /* adds r1,r1,r2 */
            r[1]=addition(state,r[1],r[2]);r[15]=0x6c74u;return 0;
        case 0x6c74u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x6c78u;return 0;
        case 0x6c78u: /* bmi 0x00006c84 */
            if(state->cpsr&0x80000000u){r[15]=27780u;}else{r[15]=0x6c7cu;}return 0;
        case 0x6c7cu: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x6c80u;return 0;
        case 0x6c80u: /* movne r1,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[1]=127u;}r[15]=0x6c84u;return 0;
        case 0x6c84u: /* mov r1,r1, lsr #0x2 */
            r[1]=(r[1]>>2);r[15]=0x6c88u;return 0;
        case 0x6c88u: /* subs r1,r1,#0x10 */
            r[1]=subtract(state,r[1],16u);r[15]=0x6c8cu;return 0;
        case 0x6c8cu: /* eormi r1,r1,#0xff */
            if(state->cpsr&0x80000000u){r[1]=r[1]^255u;}r[15]=0x6c90u;return 0;
        case 0x6c90u: /* addmi r1,r1,#0x10 */
            if(state->cpsr&0x80000000u){r[1]=r[1]+16u;}r[15]=0x6c94u;return 0;
        case 0x6c94u: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0x6c98u;return 0;
        case 0x6c98u: /* ldr r2,[r8,#0x24] */
            LOAD(r[2],r[8]+36u,4);r[15]=0x6c9cu;return 0;
        case 0x6c9cu: /* and r2,r2,#0xff00 */
            r[2]=r[2]&65280u;r[15]=0x6ca0u;return 0;
        case 0x6ca0u: /* orr r2,r2,r1 */
            r[2]=r[2]|r[1];r[15]=0x6ca4u;return 0;
        case 0x6ca4u: /* str r2,[r8,#0x24] */
            STORE(r[8]+36u,r[2],4);r[15]=0x6ca8u;return 0;
        case 0x6ca8u: /* mov r2,#0x0 */
            r[2]=0u;r[15]=0x6cacu;return 0;
        case 0x6cacu: /* ldrb r1,[r0,#0x17] */
            LOAD(r[1],r[0]+23u,1);r[15]=0x6cb0u;return 0;
        case 0x6cb0u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6cb4u;return 0;
        case 0x6cb4u: /* beq 0x00006ce0 */
            if(state->cpsr&0x40000000u){r[15]=27872u;}else{r[15]=0x6cb8u;}return 0;
        case 0x6cb8u: /* ldr r2,[r0,#0x10] */
            LOAD(r[2],r[0]+16u,4);r[15]=0x6cbcu;return 0;
        case 0x6cbcu: /* sub r2,r2,#0x8000 */
            r[2]=r[2]-32768u;r[15]=0x6cc0u;return 0;
        case 0x6cc0u: /* ldr r1,[r5,#0x14] */
            LOAD(r[1],r[5]+20u,4);r[15]=0x6cc4u;return 0;
        case 0x6cc4u: /* add r2,r2,r1 */
            r[2]=r[2]+r[1];r[15]=0x6cc8u;return 0;
        case 0x6cc8u: /* ldr r1,[r5,#0xc] */
            LOAD(r[1],r[5]+12u,4);r[15]=0x6cccu;return 0;
        case 0x6cccu: /* add r7,r2,r1 */
            r[7]=r[2]+r[1];r[15]=0x6cd0u;return 0;
        case 0x6cd0u: /* ldr r6,[r5,#0x10] */
            LOAD(r[6],r[5]+16u,4);r[15]=0x6cd4u;return 0;
        case 0x6cd4u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6cd8u;return 0;
        case 0x6cd8u: /* bl 0x000014b0 */
            r[14]=0x6cdcu;r[15]=5296u;return 0;
        case 0x6cdcu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x6ce0u;return 0;
        case 0x6ce0u: /* ldr r1,[r5,#0x38] */
            LOAD(r[1],r[5]+56u,4);r[15]=0x6ce4u;return 0;
        case 0x6ce4u: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x6ce8u;return 0;
        case 0x6ce8u: /* beq 0x00006d00 */
            if(state->cpsr&0x40000000u){r[15]=27904u;}else{r[15]=0x6cecu;}return 0;
        case 0x6cecu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x6cf0u;return 0;
        case 0x6cf0u: /* str r1,[r5,#0x38] */
            STORE(r[5]+56u,r[1],4);r[15]=0x6cf4u;return 0;
        case 0x6cf4u: /* ldreq r1,[r8,#0x0] */
            if(state->cpsr&0x40000000u){LOAD(r[1],r[8]+0u,4);}r[15]=0x6cf8u;return 0;
        case 0x6cf8u: /* orreq r1,r1,#0xc000 */
            if(state->cpsr&0x40000000u){r[1]=r[1]|49152u;}r[15]=0x6cfcu;return 0;
        case 0x6cfcu: /* streq r1,[r8,#0x0] */
            if(state->cpsr&0x40000000u){STORE(r[8]+0u,r[1],4);}r[15]=0x6d00u;return 0;
        case 0x6d00u: /* add r5,r5,#0x40 */
            r[5]=r[5]+64u;r[15]=0x6d04u;return 0;
        case 0x6d04u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x6d08u;return 0;
        case 0x6d08u: /* subs r10,r10,#0x1 */
            r[10]=subtract(state,r[10],1u);r[15]=0x6d0cu;return 0;
        case 0x6d0cu: /* bne 0x00006be4 */
            if(!(state->cpsr&0x40000000u)){r[15]=27620u;}else{r[15]=0x6d10u;}return 0;
        case 0x6d10u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x6d14u;return 0;
        case 0x6d14u: /* ldr r0,[0x6d3c] */
            LOAD(r[0],27964u,4);r[15]=0x6d18u;return 0;
        case 0x6d18u: /* mov r2,#0x8 */
            r[2]=8u;r[15]=0x6d1cu;return 0;
        case 0x6d1cu: /* strb r1,[r0,#0x5] */
            STORE(r[0]+5u,r[1],1);r[15]=0x6d20u;return 0;
        case 0x6d20u: /* strb r1,[r0,#0xb] */
            STORE(r[0]+11u,r[1],1);r[15]=0x6d24u;return 0;
        case 0x6d24u: /* strb r1,[r0,#0x17] */
            STORE(r[0]+23u,r[1],1);r[15]=0x6d28u;return 0;
        case 0x6d28u: /* add r0,r0,#0x80 */
            r[0]=r[0]+128u;r[15]=0x6d2cu;return 0;
        case 0x6d2cu: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x6d30u;return 0;
        case 0x6d30u: /* bne 0x00006d1c */
            if(!(state->cpsr&0x40000000u)){r[15]=27932u;}else{r[15]=0x6d34u;}return 0;
        case 0x6d34u: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}else{switch(r[15]){
        case 0x6854u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x6858u;return 0;
        case 0x6858u: /* mov r1,#0x8 */
            r[1]=8u;r[15]=0x685cu;return 0;
        case 0x685cu: /* ldrb r5,[r0,#0x28] */
            LOAD(r[5],r[0]+40u,1);r[15]=0x6860u;return 0;
        case 0x6860u: /* ldr r2,[r0,#0xc] */
            LOAD(r[2],r[0]+12u,4);r[15]=0x6864u;return 0;
        case 0x6864u: /* ldr r3,[r0,#0x10] */
            LOAD(r[3],r[0]+16u,4);r[15]=0x6868u;return 0;
        case 0x6868u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x686cu;return 0;
        case 0x686cu: /* biceq r5,r5,#0x20 */
            if(state->cpsr&0x40000000u){r[5]=r[5]&~32u;}r[15]=0x6870u;return 0;
        case 0x6870u: /* beq 0x000068e8 */
            if(state->cpsr&0x40000000u){r[15]=26856u;}else{r[15]=0x6874u;}return 0;
        case 0x6874u: /* ldrb r4,[r0,#0x15] */
            LOAD(r[4],r[0]+21u,1);r[15]=0x6878u;return 0;
        case 0x6878u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x687cu;return 0;
        case 0x687cu: /* beq 0x0000688c */
            if(state->cpsr&0x40000000u){r[15]=26764u;}else{r[15]=0x6880u;}return 0;
        case 0x6880u: /* sub r4,r4,#0x1 */
            r[4]=r[4]-1u;r[15]=0x6884u;return 0;
        case 0x6884u: /* strb r4,[r0,#0x15] */
            STORE(r[0]+21u,r[4],1);r[15]=0x6888u;return 0;
        case 0x6888u: /* b 0x000068e8 */
            r[15]=26856u;return 0;
        case 0x688cu: /* ldrb r4,[r0,#0x14] */
            LOAD(r[4],r[0]+20u,1);r[15]=0x6890u;return 0;
        case 0x6890u: /* strb r4,[r0,#0x15] */
            STORE(r[0]+21u,r[4],1);r[15]=0x6894u;return 0;
        case 0x6894u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6898u;return 0;
        case 0x6898u: /* moveq r3,r2 */
            if(state->cpsr&0x40000000u){r[3]=r[2];}r[15]=0x689cu;return 0;
        case 0x689cu: /* streq r2,[r0,#0x10] */
            if(state->cpsr&0x40000000u){STORE(r[0]+16u,r[2],4);}r[15]=0x68a0u;return 0;
        case 0x68a0u: /* beq 0x000068dc */
            if(state->cpsr&0x40000000u){r[15]=26844u;}else{r[15]=0x68a4u;}return 0;
        case 0x68a4u: /* ldrb r4,[r0,#0x16] */
            LOAD(r[4],r[0]+22u,1);r[15]=0x68a8u;return 0;
        case 0x68a8u: /* mov r4,r4, lsl #0x1 */
            r[4]=(r[4]<<1);r[15]=0x68acu;return 0;
        case 0x68acu: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x68b0u;return 0;
        case 0x68b0u: /* bcs 0x000068c8 */
            if(state->cpsr&0x20000000u){r[15]=26824u;}else{r[15]=0x68b4u;}return 0;
        case 0x68b4u: /* add r3,r3,r4 */
            r[3]=r[3]+r[4];r[15]=0x68b8u;return 0;
        case 0x68b8u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x68bcu;return 0;
        case 0x68bcu: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x68c0u;return 0;
        case 0x68c0u: /* str r3,[r0,#0x10] */
            STORE(r[0]+16u,r[3],4);r[15]=0x68c4u;return 0;
        case 0x68c4u: /* b 0x000068dc */
            r[15]=26844u;return 0;
        case 0x68c8u: /* subs r3,r3,r4 */
            r[3]=subtract(state,r[3],r[4]);r[15]=0x68ccu;return 0;
        case 0x68ccu: /* movcs r3,#0x0 */
            if(state->cpsr&0x20000000u){r[3]=0u;}r[15]=0x68d0u;return 0;
        case 0x68d0u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x68d4u;return 0;
        case 0x68d4u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x68d8u;return 0;
        case 0x68d8u: /* str r3,[r0,#0x10] */
            STORE(r[0]+16u,r[3],4);r[15]=0x68dcu;return 0;
        case 0x68dcu: /* mov r3,#0x1 */
            r[3]=1u;r[15]=0x68e0u;return 0;
        case 0x68e0u: /* strb r3,[r0,#0x17] */
            STORE(r[0]+23u,r[3],1);r[15]=0x68e4u;return 0;
        case 0x68e4u: /* orr r5,r5,#0x20 */
            r[5]=r[5]|32u;r[15]=0x68e8u;return 0;
        case 0x68e8u: /* strb r5,[r0,#0x28] */
            STORE(r[0]+40u,r[5],1);r[15]=0x68ecu;return 0;
        case 0x68ecu: /* add r0,r0,#0x80 */
            r[0]=r[0]+128u;r[15]=0x68f0u;return 0;
        case 0x68f0u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x68f4u;return 0;
        case 0x68f4u: /* bne 0x0000685c */
            if(!(state->cpsr&0x40000000u)){r[15]=26716u;}else{r[15]=0x68f8u;}return 0;
        case 0x68f8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6900u: /* mov r12,#0x11000 */
            r[12]=69632u;r[15]=0x6904u;return 0;
        case 0x6904u: /* mov r11,#0x18 */
            r[11]=24u;r[15]=0x6908u;return 0;
        case 0x6908u: /* ldrb r5,[r12,#0x0] */
            LOAD(r[5],r[12]+0u,1);r[15]=0x690cu;return 0;
        case 0x690cu: /* ldr r2,[r12,#0x40] */
            LOAD(r[2],r[12]+64u,4);r[15]=0x6910u;return 0;
        case 0x6910u: /* ldr r3,[r12,#0x44] */
            LOAD(r[3],r[12]+68u,4);r[15]=0x6914u;return 0;
        case 0x6914u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x6918u;return 0;
        case 0x6918u: /* biceq r5,r5,#0x8 */
            if(state->cpsr&0x40000000u){r[5]=r[5]&~8u;}r[15]=0x691cu;return 0;
        case 0x691cu: /* beq 0x00006990 */
            if(state->cpsr&0x40000000u){r[15]=27024u;}else{r[15]=0x6920u;}return 0;
        case 0x6920u: /* ldrb r4,[r12,#0x49] */
            LOAD(r[4],r[12]+73u,1);r[15]=0x6924u;return 0;
        case 0x6924u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6928u;return 0;
        case 0x6928u: /* beq 0x00006938 */
            if(state->cpsr&0x40000000u){r[15]=26936u;}else{r[15]=0x692cu;}return 0;
        case 0x692cu: /* sub r4,r4,#0x1 */
            r[4]=r[4]-1u;r[15]=0x6930u;return 0;
        case 0x6930u: /* strb r4,[r12,#0x49] */
            STORE(r[12]+73u,r[4],1);r[15]=0x6934u;return 0;
        case 0x6934u: /* b 0x00006990 */
            r[15]=27024u;return 0;
        case 0x6938u: /* ldrb r4,[r12,#0x48] */
            LOAD(r[4],r[12]+72u,1);r[15]=0x693cu;return 0;
        case 0x693cu: /* strb r4,[r12,#0x49] */
            STORE(r[12]+73u,r[4],1);r[15]=0x6940u;return 0;
        case 0x6940u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6944u;return 0;
        case 0x6944u: /* streq r2,[r12,#0x44] */
            if(state->cpsr&0x40000000u){STORE(r[12]+68u,r[2],4);}r[15]=0x6948u;return 0;
        case 0x6948u: /* beq 0x00006984 */
            if(state->cpsr&0x40000000u){r[15]=27012u;}else{r[15]=0x694cu;}return 0;
        case 0x694cu: /* ldrb r4,[r12,#0x4a] */
            LOAD(r[4],r[12]+74u,1);r[15]=0x6950u;return 0;
        case 0x6950u: /* mov r4,r4, lsl #0x1 */
            r[4]=(r[4]<<1);r[15]=0x6954u;return 0;
        case 0x6954u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x6958u;return 0;
        case 0x6958u: /* bcs 0x00006970 */
            if(state->cpsr&0x20000000u){r[15]=26992u;}else{r[15]=0x695cu;}return 0;
        case 0x695cu: /* add r3,r3,r4 */
            r[3]=r[3]+r[4];r[15]=0x6960u;return 0;
        case 0x6960u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x6964u;return 0;
        case 0x6964u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x6968u;return 0;
        case 0x6968u: /* str r3,[r12,#0x44] */
            STORE(r[12]+68u,r[3],4);r[15]=0x696cu;return 0;
        case 0x696cu: /* b 0x00006984 */
            r[15]=27012u;return 0;
        case 0x6970u: /* subs r3,r3,r4 */
            r[3]=subtract(state,r[3],r[4]);r[15]=0x6974u;return 0;
        case 0x6974u: /* movcs r3,#0x0 */
            if(state->cpsr&0x20000000u){r[3]=0u;}r[15]=0x6978u;return 0;
        case 0x6978u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x697cu;return 0;
        case 0x697cu: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x6980u;return 0;
        case 0x6980u: /* str r3,[r12,#0x44] */
            STORE(r[12]+68u,r[3],4);r[15]=0x6984u;return 0;
        case 0x6984u: /* mov r3,#0xff */
            r[3]=255u;r[15]=0x6988u;return 0;
        case 0x6988u: /* strb r3,[r12,#0x4b] */
            STORE(r[12]+75u,r[3],1);r[15]=0x698cu;return 0;
        case 0x698cu: /* orr r5,r5,#0x8 */
            r[5]=r[5]|8u;r[15]=0x6990u;return 0;
        case 0x6990u: /* strb r5,[r12,#0x0] */
            STORE(r[12]+0u,r[5],1);r[15]=0x6994u;return 0;
        case 0x6994u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x6998u;return 0;
        case 0x6998u: /* subs r11,r11,#0x1 */
            r[11]=subtract(state,r[11],1u);r[15]=0x699cu;return 0;
        case 0x699cu: /* bne 0x00006908 */
            if(!(state->cpsr&0x40000000u)){r[15]=26888u;}else{r[15]=0x69a0u;}return 0;
        case 0x69a0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x69b0u: /* mov r12,#0x11000 */
            r[12]=69632u;r[15]=0x69b4u;return 0;
        case 0x69b4u: /* mov r11,#0x18 */
            r[11]=24u;r[15]=0x69b8u;return 0;
        case 0x69b8u: /* ldrb r5,[r12,#0x0] */
            LOAD(r[5],r[12]+0u,1);r[15]=0x69bcu;return 0;
        case 0x69bcu: /* ldr r2,[r12,#0x4c] */
            LOAD(r[2],r[12]+76u,4);r[15]=0x69c0u;return 0;
        case 0x69c0u: /* ldr r3,[r12,#0x50] */
            LOAD(r[3],r[12]+80u,4);r[15]=0x69c4u;return 0;
        case 0x69c4u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x69c8u;return 0;
        case 0x69c8u: /* biceq r5,r5,#0x20 */
            if(state->cpsr&0x40000000u){r[5]=r[5]&~32u;}r[15]=0x69ccu;return 0;
        case 0x69ccu: /* beq 0x00006a40 */
            if(state->cpsr&0x40000000u){r[15]=27200u;}else{r[15]=0x69d0u;}return 0;
        case 0x69d0u: /* ldrb r4,[r12,#0x55] */
            LOAD(r[4],r[12]+85u,1);r[15]=0x69d4u;return 0;
        case 0x69d4u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x69d8u;return 0;
        case 0x69d8u: /* beq 0x000069e8 */
            if(state->cpsr&0x40000000u){r[15]=27112u;}else{r[15]=0x69dcu;}return 0;
        case 0x69dcu: /* sub r4,r4,#0x1 */
            r[4]=r[4]-1u;r[15]=0x69e0u;return 0;
        case 0x69e0u: /* strb r4,[r12,#0x55] */
            STORE(r[12]+85u,r[4],1);r[15]=0x69e4u;return 0;
        case 0x69e4u: /* b 0x00006a40 */
            r[15]=27200u;return 0;
        case 0x69e8u: /* ldrb r4,[r12,#0x54] */
            LOAD(r[4],r[12]+84u,1);r[15]=0x69ecu;return 0;
        case 0x69ecu: /* strb r4,[r12,#0x55] */
            STORE(r[12]+85u,r[4],1);r[15]=0x69f0u;return 0;
        case 0x69f0u: /* tst r4,#0xff */
            temporary=r[4]&255u;logic_flags(state,temporary,0,255u);r[15]=0x69f4u;return 0;
        case 0x69f4u: /* streq r2,[r12,#0x50] */
            if(state->cpsr&0x40000000u){STORE(r[12]+80u,r[2],4);}r[15]=0x69f8u;return 0;
        case 0x69f8u: /* beq 0x00006a34 */
            if(state->cpsr&0x40000000u){r[15]=27188u;}else{r[15]=0x69fcu;}return 0;
        case 0x69fcu: /* ldrb r4,[r12,#0x56] */
            LOAD(r[4],r[12]+86u,1);r[15]=0x6a00u;return 0;
        case 0x6a00u: /* mov r4,r4, lsl #0x1 */
            r[4]=(r[4]<<1);r[15]=0x6a04u;return 0;
        case 0x6a04u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x6a08u;return 0;
        case 0x6a08u: /* bcs 0x00006a20 */
            if(state->cpsr&0x20000000u){r[15]=27168u;}else{r[15]=0x6a0cu;}return 0;
        case 0x6a0cu: /* add r3,r3,r4 */
            r[3]=r[3]+r[4];r[15]=0x6a10u;return 0;
        case 0x6a10u: /* cmp r3,r2 */
            (void)subtract(state,r[3],r[2]);r[15]=0x6a14u;return 0;
        case 0x6a14u: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x6a18u;return 0;
        case 0x6a18u: /* str r3,[r12,#0x50] */
            STORE(r[12]+80u,r[3],4);r[15]=0x6a1cu;return 0;
        case 0x6a1cu: /* b 0x00006a34 */
            r[15]=27188u;return 0;
        case 0x6a20u: /* subs r3,r3,r4 */
            r[3]=subtract(state,r[3],r[4]);r[15]=0x6a24u;return 0;
        case 0x6a24u: /* movcc r3,#0x0 */
            if(!(state->cpsr&0x20000000u))r[3]=0;r[15]=0x6a28u;return 0;
        case 0x6a28u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x6a2cu;return 0;
        case 0x6a2cu: /* movcs r3,r2 */
            if(state->cpsr&0x20000000u){r[3]=r[2];}r[15]=0x6a30u;return 0;
        case 0x6a30u: /* str r3,[r12,#0x50] */
            STORE(r[12]+80u,r[3],4);r[15]=0x6a34u;return 0;
        case 0x6a34u: /* mov r3,#0xff */
            r[3]=255u;r[15]=0x6a38u;return 0;
        case 0x6a38u: /* strb r3,[r12,#0x4b] */
            STORE(r[12]+75u,r[3],1);r[15]=0x6a3cu;return 0;
        case 0x6a3cu: /* orr r5,r5,#0x20 */
            r[5]=r[5]|32u;r[15]=0x6a40u;return 0;
        case 0x6a40u: /* strb r5,[r12,#0x0] */
            STORE(r[12]+0u,r[5],1);r[15]=0x6a44u;return 0;
        case 0x6a44u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x6a48u;return 0;
        case 0x6a48u: /* subs r11,r11,#0x1 */
            r[11]=subtract(state,r[11],1u);r[15]=0x6a4cu;return 0;
        case 0x6a4cu: /* bne 0x000069b8 */
            if(!(state->cpsr&0x40000000u)){r[15]=27064u;}else{r[15]=0x6a50u;}return 0;
        case 0x6a50u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6a64u: /* mov r12,#0x11000 */
            r[12]=69632u;r[15]=0x6a68u;return 0;
        case 0x6a68u: /* mov r11,#0x18 */
            r[11]=24u;r[15]=0x6a6cu;return 0;
        case 0x6a6cu: /* ldrb r3,[r12,#0x4b] */
            LOAD(r[3],r[12]+75u,1);r[15]=0x6a70u;return 0;
        case 0x6a70u: /* tst r3,#0xff */
            temporary=r[3]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6a74u;return 0;
        case 0x6a74u: /* beq 0x00006bb0 */
            if(state->cpsr&0x40000000u){r[15]=27568u;}else{r[15]=0x6a78u;}return 0;
        case 0x6a78u: /* mov r3,#0x0 */
            r[3]=0u;r[15]=0x6a7cu;return 0;
        case 0x6a7cu: /* strb r3,[r12,#0x4b] */
            STORE(r[12]+75u,r[3],1);r[15]=0x6a80u;return 0;
        case 0x6a80u: /* ldr r4,[0x6bd4] */
            LOAD(r[4],27604u,4);r[15]=0x6a84u;return 0;
        case 0x6a84u: /* ldr r3,[r12,#0x44] */
            LOAD(r[3],r[12]+68u,4);r[15]=0x6a88u;return 0;
        case 0x6a88u: /* subs r3,r3,#0x8000 */
            r[3]=subtract(state,r[3],32768u);r[15]=0x6a8cu;return 0;
        case 0x6a8cu: /* eormi r3,r3,r4 */
            if(state->cpsr&0x80000000u){r[3]=r[3]^r[4];}r[15]=0x6a90u;return 0;
        case 0x6a90u: /* orrmi r3,r3,#0x8000 */
            if(state->cpsr&0x80000000u){r[3]=r[3]|32768u;}r[15]=0x6a94u;return 0;
        case 0x6a94u: /* ldr r4,[0x6d84] */
            LOAD(r[4],28036u,4);r[15]=0x6a98u;return 0;
        case 0x6a98u: /* mov r0,r3, lsr #0x8 */
            r[0]=(r[3]>>8);r[15]=0x6a9cu;return 0;
        case 0x6a9cu: /* and r2,r0,#0x7f */
            r[2]=r[0]&127u;r[15]=0x6aa0u;return 0;
        case 0x6aa0u: /* add r2,r2,#0x60 */
            r[2]=r[2]+96u;r[15]=0x6aa4u;return 0;
        case 0x6aa4u: /* ldrb r2,[r4,r2] */
            LOAD(r[2],r[4]+r[2],1);r[15]=0x6aa8u;return 0;
        case 0x6aa8u: /* cmp r2,#0x7f */
            (void)subtract(state,r[2],127u);r[15]=0x6aacu;return 0;
        case 0x6aacu: /* ldreq r2,[0x6bd8] */
            if(state->cpsr&0x40000000u){LOAD(r[2],27608u,4);}r[15]=0x6ab0u;return 0;
        case 0x6ab0u: /* beq 0x00006b00 */
            if(state->cpsr&0x40000000u){r[15]=27392u;}else{r[15]=0x6ab4u;}return 0;
        case 0x6ab4u: /* and r5,r2,#0xf */
            r[5]=r[2]&15u;r[15]=0x6ab8u;return 0;
        case 0x6ab8u: /* mov r5,r5, lsl #0x7 */
            r[5]=(r[5]<<7);r[15]=0x6abcu;return 0;
        case 0x6abcu: /* mov r3,r3, lsr #0x1 */
            r[3]=(r[3]>>1);r[15]=0x6ac0u;return 0;
        case 0x6ac0u: /* and r3,r3,#0x7f */
            r[3]=r[3]&127u;r[15]=0x6ac4u;return 0;
        case 0x6ac4u: /* orr r3,r3,r5 */
            r[3]=r[3]|r[5];r[15]=0x6ac8u;return 0;
        case 0x6ac8u: /* ldr r4,[0x6d88] */
            LOAD(r[4],28040u,4);r[15]=0x6accu;return 0;
        case 0x6accu: /* ldrb r7,[r4,r3] */
            LOAD(r[7],r[4]+r[3],1);r[15]=0x6ad0u;return 0;
        case 0x6ad0u: /* ldr r4,[0x6bc4] */
            LOAD(r[4],27588u,4);r[15]=0x6ad4u;return 0;
        case 0x6ad4u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x6ad8u;return 0;
        case 0x6ad8u: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x6adcu;return 0;
        case 0x6adcu: /* ldr r4,[0x6bc8] */
            LOAD(r[4],27592u,4);r[15]=0x6ae0u;return 0;
        case 0x6ae0u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x6ae4u;return 0;
        case 0x6ae4u: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x6ae8u;return 0;
        case 0x6ae8u: /* ldr r4,[0x6bcc] */
            LOAD(r[4],27596u,4);r[15]=0x6aecu;return 0;
        case 0x6aecu: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x6af0u;return 0;
        case 0x6af0u: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x6af4u;return 0;
        case 0x6af4u: /* and r2,r2,#0xf0 */
            r[2]=r[2]&240u;r[15]=0x6af8u;return 0;
        case 0x6af8u: /* mov r2,r2, lsl #0x6 */
            r[2]=(r[2]<<6);r[15]=0x6afcu;return 0;
        case 0x6afcu: /* orr r2,r2,r7 */
            r[2]=r[2]|r[7];r[15]=0x6b00u;return 0;
        case 0x6b00u: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x6b04u;return 0;
        case 0x6b04u: /* ldr r3,[0x6bd4] */
            LOAD(r[3],27604u,4);r[15]=0x6b08u;return 0;
        case 0x6b08u: /* eorne r2,r2,r3 */
            if(!(state->cpsr&0x40000000u)){r[2]=r[2]^r[3];}r[15]=0x6b0cu;return 0;
        case 0x6b0cu: /* addne r2,r2,#0x1 */
            if(!(state->cpsr&0x40000000u)){r[2]=r[2]+1u;}r[15]=0x6b10u;return 0;
        case 0x6b10u: /* str r2,[0x6bd0] */
            STORE(27600u,r[2],4);r[15]=0x6b14u;return 0;
        case 0x6b14u: /* ldr r4,[0x6bd4] */
            LOAD(r[4],27604u,4);r[15]=0x6b18u;return 0;
        case 0x6b18u: /* ldr r3,[r12,#0x50] */
            LOAD(r[3],r[12]+80u,4);r[15]=0x6b1cu;return 0;
        case 0x6b1cu: /* subs r3,r3,#0x8000 */
            r[3]=subtract(state,r[3],32768u);r[15]=0x6b20u;return 0;
        case 0x6b20u: /* eormi r3,r3,r4 */
            if(state->cpsr&0x80000000u){r[3]=r[3]^r[4];}r[15]=0x6b24u;return 0;
        case 0x6b24u: /* orrmi r3,r3,#0x8000 */
            if(state->cpsr&0x80000000u){r[3]=r[3]|32768u;}r[15]=0x6b28u;return 0;
        case 0x6b28u: /* ldr r4,[0x6d84] */
            LOAD(r[4],28036u,4);r[15]=0x6b2cu;return 0;
        case 0x6b2cu: /* mov r0,r3, lsr #0x8 */
            r[0]=(r[3]>>8);r[15]=0x6b30u;return 0;
        case 0x6b30u: /* and r2,r0,#0x7f */
            r[2]=r[0]&127u;r[15]=0x6b34u;return 0;
        case 0x6b34u: /* add r2,r2,#0x60 */
            r[2]=r[2]+96u;r[15]=0x6b38u;return 0;
        case 0x6b38u: /* ldrb r2,[r4,r2] */
            LOAD(r[2],r[4]+r[2],1);r[15]=0x6b3cu;return 0;
        case 0x6b3cu: /* cmp r2,#0x7f */
            (void)subtract(state,r[2],127u);r[15]=0x6b40u;return 0;
        case 0x6b40u: /* ldreq r2,[0x6bd8] */
            if(state->cpsr&0x40000000u){LOAD(r[2],27608u,4);}r[15]=0x6b44u;return 0;
        case 0x6b44u: /* beq 0x00006b94 */
            if(state->cpsr&0x40000000u){r[15]=27540u;}else{r[15]=0x6b48u;}return 0;
        case 0x6b48u: /* and r5,r2,#0xf */
            r[5]=r[2]&15u;r[15]=0x6b4cu;return 0;
        case 0x6b4cu: /* mov r5,r5, lsl #0x7 */
            r[5]=(r[5]<<7);r[15]=0x6b50u;return 0;
        case 0x6b50u: /* mov r3,r3, lsr #0x1 */
            r[3]=(r[3]>>1);r[15]=0x6b54u;return 0;
        case 0x6b54u: /* and r3,r3,#0x7f */
            r[3]=r[3]&127u;r[15]=0x6b58u;return 0;
        case 0x6b58u: /* orr r3,r3,r5 */
            r[3]=r[3]|r[5];r[15]=0x6b5cu;return 0;
        case 0x6b5cu: /* ldr r4,[0x6d88] */
            LOAD(r[4],28040u,4);r[15]=0x6b60u;return 0;
        case 0x6b60u: /* ldrb r7,[r4,r3] */
            LOAD(r[7],r[4]+r[3],1);r[15]=0x6b64u;return 0;
        case 0x6b64u: /* ldr r4,[0x6bc4] */
            LOAD(r[4],27588u,4);r[15]=0x6b68u;return 0;
        case 0x6b68u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x6b6cu;return 0;
        case 0x6b6cu: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x6b70u;return 0;
        case 0x6b70u: /* ldr r4,[0x6bc8] */
            LOAD(r[4],27592u,4);r[15]=0x6b74u;return 0;
        case 0x6b74u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x6b78u;return 0;
        case 0x6b78u: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x6b7cu;return 0;
        case 0x6b7cu: /* ldr r4,[0x6bcc] */
            LOAD(r[4],27596u,4);r[15]=0x6b80u;return 0;
        case 0x6b80u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x6b84u;return 0;
        case 0x6b84u: /* addpl r7,r7,#0x100 */
            if(!(state->cpsr&0x80000000u)){r[7]=r[7]+256u;}r[15]=0x6b88u;return 0;
        case 0x6b88u: /* and r2,r2,#0xf0 */
            r[2]=r[2]&240u;r[15]=0x6b8cu;return 0;
        case 0x6b8cu: /* mov r2,r2, lsl #0x6 */
            r[2]=(r[2]<<6);r[15]=0x6b90u;return 0;
        case 0x6b90u: /* orr r2,r2,r7 */
            r[2]=r[2]|r[7];r[15]=0x6b94u;return 0;
        case 0x6b94u: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x6b98u;return 0;
        case 0x6b98u: /* ldr r3,[0x6bd4] */
            LOAD(r[3],27604u,4);r[15]=0x6b9cu;return 0;
        case 0x6b9cu: /* eorne r2,r2,r3 */
            if(!(state->cpsr&0x40000000u)){r[2]=r[2]^r[3];}r[15]=0x6ba0u;return 0;
        case 0x6ba0u: /* addne r2,r2,#0x1 */
            if(!(state->cpsr&0x40000000u)){r[2]=r[2]+1u;}r[15]=0x6ba4u;return 0;
        case 0x6ba4u: /* ldr r3,[0x6bd0] */
            LOAD(r[3],27600u,4);r[15]=0x6ba8u;return 0;
        case 0x6ba8u: /* add r2,r2,r3 */
            r[2]=r[2]+r[3];r[15]=0x6bacu;return 0;
        case 0x6bacu: /* str r2,[r12,#0x10] */
            STORE(r[12]+16u,r[2],4);r[15]=0x6bb0u;return 0;
        case 0x6bb0u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x6bb4u;return 0;
        case 0x6bb4u: /* subs r11,r11,#0x1 */
            r[11]=subtract(state,r[11],1u);r[15]=0x6bb8u;return 0;
        case 0x6bb8u: /* bne 0x00006a6c */
            if(!(state->cpsr&0x40000000u)){r[15]=27244u;}else{r[15]=0x6bbcu;}return 0;
        case 0x6bbcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6bdcu: /* adr r12,0x6d44 */
            r[12]=27972u;r[15]=0x6be0u;return 0;
        case 0x6be0u: /* mov r5,#0xe800 */
            r[5]=59392u;r[15]=0x6be4u;return 0;
        case 0x6be4u: /* mov r10,#0x30 */
            r[10]=48u;r[15]=0x6be8u;return 0;
        case 0x6be8u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x6becu;return 0;
        case 0x6becu: /* ldr r2,[r5,#0x0] */
            LOAD(r[2],r[5]+0u,4);r[15]=0x6bf0u;return 0;
        case 0x6bf0u: /* movs r2,r2 */
            r[2]=r[2];logic_flags(state,r[2],0,0);r[15]=0x6bf4u;return 0;
        case 0x6bf4u: /* beq 0x00006d08 */
            if(state->cpsr&0x40000000u){r[15]=27912u;}else{r[15]=0x6bf8u;}return 0;
        case 0x6bf8u: /* ldrb r0,[r5,#0x0] */
            LOAD(r[0],r[5]+0u,1);r[15]=0x6bfcu;return 0;
        case 0x6bfcu: /* tst r0,#0xc0 */
            temporary=r[0]&192u;logic_flags(state,temporary,0,192u);r[15]=0x6c00u;return 0;
        case 0x6c00u: /* beq 0x00006c10 */
            if(state->cpsr&0x40000000u){r[15]=27664u;}else{r[15]=0x6c04u;}return 0;
        case 0x6c04u: /* ldrb r0,[r5,#0x3f] */
            LOAD(r[0],r[5]+63u,1);r[15]=0x6c08u;return 0;
        case 0x6c08u: /* add r0,r0,#0x1 */
            r[0]=r[0]+1u;r[15]=0x6c0cu;return 0;
        case 0x6c0cu: /* strb r0,[r5,#0x3f] */
            STORE(r[5]+63u,r[0],1);r[15]=0x6c10u;return 0;
        case 0x6c10u: /* ldrb r0,[r5,#0x4] */
            LOAD(r[0],r[5]+4u,1);r[15]=0x6c14u;return 0;
        case 0x6c14u: /* add r1,r12,r0, lsl #0x2 */
            r[1]=r[12]+(r[0]<<2);r[15]=0x6c18u;return 0;
        case 0x6c18u: /* ldr r0,[r1,#0x0] */
            LOAD(r[0],r[1]+0u,4);r[15]=0x6c1cu;return 0;
        case 0x6c1cu: /* ldrb r1,[r0,#0x5] */
            LOAD(r[1],r[0]+5u,1);r[15]=0x6c20u;return 0;
        case 0x6c20u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6c24u;return 0;
        case 0x6c24u: /* beq 0x00006c60 */
            if(state->cpsr&0x40000000u){r[15]=27744u;}else{r[15]=0x6c28u;}return 0;
        case 0x6c28u: /* ldrb r1,[r5,#0x8] */
            LOAD(r[1],r[5]+8u,1);r[15]=0x6c2cu;return 0;
        case 0x6c2cu: /* ldrb r2,[r0,#0x1] */
            LOAD(r[2],r[0]+1u,1);r[15]=0x6c30u;return 0;
        case 0x6c30u: /* sub r2,r2,#0x80 */
            r[2]=r[2]-128u;r[15]=0x6c34u;return 0;
        case 0x6c34u: /* adds r1,r1,r2 */
            r[1]=addition(state,r[1],r[2]);r[15]=0x6c38u;return 0;
        case 0x6c38u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x6c3cu;return 0;
        case 0x6c3cu: /* bmi 0x00006c48 */
            if(state->cpsr&0x80000000u){r[15]=27720u;}else{r[15]=0x6c40u;}return 0;
        case 0x6c40u: /* tst r1,#0x100 */
            temporary=r[1]&256u;logic_flags(state,temporary,1,256u);r[15]=0x6c44u;return 0;
        case 0x6c44u: /* movne r1,#0xff */
            if(!(state->cpsr&0x40000000u)){r[1]=255u;}r[15]=0x6c48u;return 0;
        case 0x6c48u: /* eor r1,r1,#0xff */
            r[1]=r[1]^255u;r[15]=0x6c4cu;return 0;
        case 0x6c4cu: /* and r1,r1,#0xff */
            r[1]=r[1]&255u;r[15]=0x6c50u;return 0;
        case 0x6c50u: /* ldr r2,[r8,#0x28] */
            LOAD(r[2],r[8]+40u,4);r[15]=0x6c54u;return 0;
        case 0x6c54u: /* and r2,r2,#0x1f */
            r[2]=r[2]&31u;r[15]=0x6c58u;return 0;
        case 0x6c58u: /* orr r2,r2,r1, lsl #0x8 */
            r[2]=r[2]|(r[1]<<8);r[15]=0x6c5cu;return 0;
        case 0x6c5cu: /* str r2,[r8,#0x28] */
            STORE(r[8]+40u,r[2],4);r[15]=0x6c60u;return 0;
        case 0x6c60u: /* ldrb r1,[r0,#0xb] */
            LOAD(r[1],r[0]+11u,1);r[15]=0x6c64u;return 0;
        case 0x6c64u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6c68u;return 0;
        case 0x6c68u: /* beq 0x00006cb0 */
            if(state->cpsr&0x40000000u){r[15]=27824u;}else{r[15]=0x6c6cu;}return 0;
        case 0x6c6cu: /* ldrb r1,[r5,#0x9] */
            LOAD(r[1],r[5]+9u,1);r[15]=0x6c70u;return 0;
        case 0x6c70u: /* ldrb r2,[r0,#0x7] */
            LOAD(r[2],r[0]+7u,1);r[15]=0x6c74u;return 0;
        case 0x6c74u: /* sub r2,r2,#0x80 */
            r[2]=r[2]-128u;r[15]=0x6c78u;return 0;
        case 0x6c78u: /* adds r1,r1,r2 */
            r[1]=addition(state,r[1],r[2]);r[15]=0x6c7cu;return 0;
        case 0x6c7cu: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x6c80u;return 0;
        case 0x6c80u: /* bmi 0x00006c8c */
            if(state->cpsr&0x80000000u){r[15]=27788u;}else{r[15]=0x6c84u;}return 0;
        case 0x6c84u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x6c88u;return 0;
        case 0x6c88u: /* movne r1,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[1]=127u;}r[15]=0x6c8cu;return 0;
        case 0x6c8cu: /* mov r1,r1, lsr #0x2 */
            r[1]=(r[1]>>2);r[15]=0x6c90u;return 0;
        case 0x6c90u: /* subs r1,r1,#0x10 */
            r[1]=subtract(state,r[1],16u);r[15]=0x6c94u;return 0;
        case 0x6c94u: /* eormi r1,r1,#0xff */
            if(state->cpsr&0x80000000u){r[1]=r[1]^255u;}r[15]=0x6c98u;return 0;
        case 0x6c98u: /* addmi r1,r1,#0x10 */
            if(state->cpsr&0x80000000u){r[1]=r[1]+16u;}r[15]=0x6c9cu;return 0;
        case 0x6c9cu: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0x6ca0u;return 0;
        case 0x6ca0u: /* ldr r2,[r8,#0x24] */
            LOAD(r[2],r[8]+36u,4);r[15]=0x6ca4u;return 0;
        case 0x6ca4u: /* and r2,r2,#0xff00 */
            r[2]=r[2]&65280u;r[15]=0x6ca8u;return 0;
        case 0x6ca8u: /* orr r2,r2,r1 */
            r[2]=r[2]|r[1];r[15]=0x6cacu;return 0;
        case 0x6cacu: /* str r2,[r8,#0x24] */
            STORE(r[8]+36u,r[2],4);r[15]=0x6cb0u;return 0;
        case 0x6cb0u: /* mov r2,#0x0 */
            r[2]=0u;r[15]=0x6cb4u;return 0;
        case 0x6cb4u: /* ldrb r1,[r0,#0x17] */
            LOAD(r[1],r[0]+23u,1);r[15]=0x6cb8u;return 0;
        case 0x6cb8u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6cbcu;return 0;
        case 0x6cbcu: /* beq 0x00006ce8 */
            if(state->cpsr&0x40000000u){r[15]=27880u;}else{r[15]=0x6cc0u;}return 0;
        case 0x6cc0u: /* ldr r2,[r0,#0x10] */
            LOAD(r[2],r[0]+16u,4);r[15]=0x6cc4u;return 0;
        case 0x6cc4u: /* sub r2,r2,#0x8000 */
            r[2]=r[2]-32768u;r[15]=0x6cc8u;return 0;
        case 0x6cc8u: /* ldr r1,[r5,#0x14] */
            LOAD(r[1],r[5]+20u,4);r[15]=0x6cccu;return 0;
        case 0x6cccu: /* add r2,r2,r1 */
            r[2]=r[2]+r[1];r[15]=0x6cd0u;return 0;
        case 0x6cd0u: /* ldr r1,[r5,#0xc] */
            LOAD(r[1],r[5]+12u,4);r[15]=0x6cd4u;return 0;
        case 0x6cd4u: /* add r7,r2,r1 */
            r[7]=r[2]+r[1];r[15]=0x6cd8u;return 0;
        case 0x6cd8u: /* ldr r6,[r5,#0x10] */
            LOAD(r[6],r[5]+16u,4);r[15]=0x6cdcu;return 0;
        case 0x6cdcu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6ce0u;return 0;
        case 0x6ce0u: /* bl 0x000014bc */
            r[14]=0x6ce4u;r[15]=5308u;return 0;
        case 0x6ce4u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x6ce8u;return 0;
        case 0x6ce8u: /* ldr r1,[r5,#0x38] */
            LOAD(r[1],r[5]+56u,4);r[15]=0x6cecu;return 0;
        case 0x6cecu: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x6cf0u;return 0;
        case 0x6cf0u: /* beq 0x00006d08 */
            if(state->cpsr&0x40000000u){r[15]=27912u;}else{r[15]=0x6cf4u;}return 0;
        case 0x6cf4u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x6cf8u;return 0;
        case 0x6cf8u: /* str r1,[r5,#0x38] */
            STORE(r[5]+56u,r[1],4);r[15]=0x6cfcu;return 0;
        case 0x6cfcu: /* ldreq r1,[r8,#0x0] */
            if(state->cpsr&0x40000000u){LOAD(r[1],r[8]+0u,4);}r[15]=0x6d00u;return 0;
        case 0x6d00u: /* orreq r1,r1,#0xc000 */
            if(state->cpsr&0x40000000u){r[1]=r[1]|49152u;}r[15]=0x6d04u;return 0;
        case 0x6d04u: /* streq r1,[r8,#0x0] */
            if(state->cpsr&0x40000000u){STORE(r[8]+0u,r[1],4);}r[15]=0x6d08u;return 0;
        case 0x6d08u: /* add r5,r5,#0x40 */
            r[5]=r[5]+64u;r[15]=0x6d0cu;return 0;
        case 0x6d0cu: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x6d10u;return 0;
        case 0x6d10u: /* subs r10,r10,#0x1 */
            r[10]=subtract(state,r[10],1u);r[15]=0x6d14u;return 0;
        case 0x6d14u: /* bne 0x00006bec */
            if(!(state->cpsr&0x40000000u)){r[15]=27628u;}else{r[15]=0x6d18u;}return 0;
        case 0x6d18u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x6d1cu;return 0;
        case 0x6d1cu: /* ldr r0,[0x6d44] */
            LOAD(r[0],27972u,4);r[15]=0x6d20u;return 0;
        case 0x6d20u: /* mov r2,#0x8 */
            r[2]=8u;r[15]=0x6d24u;return 0;
        case 0x6d24u: /* strb r1,[r0,#0x5] */
            STORE(r[0]+5u,r[1],1);r[15]=0x6d28u;return 0;
        case 0x6d28u: /* strb r1,[r0,#0xb] */
            STORE(r[0]+11u,r[1],1);r[15]=0x6d2cu;return 0;
        case 0x6d2cu: /* strb r1,[r0,#0x17] */
            STORE(r[0]+23u,r[1],1);r[15]=0x6d30u;return 0;
        case 0x6d30u: /* add r0,r0,#0x80 */
            r[0]=r[0]+128u;r[15]=0x6d34u;return 0;
        case 0x6d34u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x6d38u;return 0;
        case 0x6d38u: /* bne 0x00006d24 */
            if(!(state->cpsr&0x40000000u)){r[15]=27940u;}else{r[15]=0x6d3cu;}return 0;
        case 0x6d3cu: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}
}

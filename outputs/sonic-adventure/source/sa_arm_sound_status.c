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
int sa_arm_sound_status_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
        case 0x6d0u: /* ldr r0,[0x974] */
            LOAD(r[0],2420u,4);r[15]=0x6d4u;return 0;
        case 0x6d4u: /* ldrb r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,1);r[15]=0x6d8u;return 0;
        case 0x6d8u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6dcu;return 0;
        case 0x6dcu: /* beq 0x00000700 */
            if(state->cpsr&0x40000000u){r[15]=1792u;}else{r[15]=0x6e0u;}return 0;
        case 0x6e0u: /* ldr r0,[0x96c] */
            LOAD(r[0],2412u,4);r[15]=0x6e4u;return 0;
        case 0x6e4u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x6e8u;return 0;
        case 0x6e8u: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x6ecu;return 0;
        case 0x6ecu: /* beq 0x00000700 */
            if(state->cpsr&0x40000000u){r[15]=1792u;}else{r[15]=0x6f0u;}return 0;
        case 0x6f0u: /* ldr r0,[0x978] */
            LOAD(r[0],2424u,4);r[15]=0x6f4u;return 0;
        case 0x6f4u: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x6f8u;return 0;
        case 0x6f8u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x6fcu;return 0;
        case 0x6fcu: /* b 0x0000070c */
            r[15]=1804u;return 0;
        case 0x700u: /* ldr r0,[0x978] */
            LOAD(r[0],2424u,4);r[15]=0x704u;return 0;
        case 0x704u: /* movne r1,#0x0 */
            if(!(state->cpsr&0x40000000u)){r[1]=0u;}r[15]=0x708u;return 0;
        case 0x708u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x70cu;return 0;
        case 0x70cu: /* ldr r0,[0x97c] */
            LOAD(r[0],2428u,4);r[15]=0x710u;return 0;
        case 0x710u: /* ldrb r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,1);r[15]=0x714u;return 0;
        case 0x714u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x718u;return 0;
        case 0x718u: /* beq 0x0000072c */
            if(state->cpsr&0x40000000u){r[15]=1836u;}else{r[15]=0x71cu;}return 0;
        case 0x71cu: /* ldr r0,[0x980] */
            LOAD(r[0],2432u,4);r[15]=0x720u;return 0;
        case 0x720u: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x724u;return 0;
        case 0x724u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x728u;return 0;
        case 0x728u: /* b 0x00000738 */
            r[15]=1848u;return 0;
        case 0x72cu: /* ldr r0,[0x980] */
            LOAD(r[0],2432u,4);r[15]=0x730u;return 0;
        case 0x730u: /* movne r1,#0x0 */
            if(!(state->cpsr&0x40000000u)){r[1]=0u;}r[15]=0x734u;return 0;
        case 0x734u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x738u;return 0;
        case 0x738u: /* ldr r0,[0x984] */
            LOAD(r[0],2436u,4);r[15]=0x73cu;return 0;
        case 0x73cu: /* ldrb r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,1);r[15]=0x740u;return 0;
        case 0x740u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x744u;return 0;
        case 0x744u: /* beq 0x00000758 */
            if(state->cpsr&0x40000000u){r[15]=1880u;}else{r[15]=0x748u;}return 0;
        case 0x748u: /* ldr r0,[0x988] */
            LOAD(r[0],2440u,4);r[15]=0x74cu;return 0;
        case 0x74cu: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x750u;return 0;
        case 0x750u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x754u;return 0;
        case 0x754u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x758u: /* ldr r0,[0x988] */
            LOAD(r[0],2440u,4);r[15]=0x75cu;return 0;
        case 0x75cu: /* movne r1,#0x0 */
            if(!(state->cpsr&0x40000000u)){r[1]=0u;}r[15]=0x760u;return 0;
        case 0x760u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x764u;return 0;
        case 0x764u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x768u: /* ldr r1,[0x978] */
            LOAD(r[1],2424u,4);r[15]=0x76cu;return 0;
        case 0x76cu: /* ldrb r2,[r1,#0x0] */
            LOAD(r[2],r[1]+0u,1);r[15]=0x770u;return 0;
        case 0x770u: /* tst r2,#0xff */
            temporary=r[2]&255u;logic_flags(state,temporary,0,255u);r[15]=0x774u;return 0;
        case 0x774u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x778u;}return 0;
        case 0x778u: /* ldr r1,[0x96c] */
            LOAD(r[1],2412u,4);r[15]=0x77cu;return 0;
        case 0x77cu: /* ldr r2,[r1,#0x0] */
            LOAD(r[2],r[1]+0u,4);r[15]=0x780u;return 0;
        case 0x780u: /* mov r3,#0x18000 */
            r[3]=98304u;r[15]=0x784u;return 0;
        case 0x784u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x788u;return 0;
        case 0x788u: /* movmi pc,lr */
            if(state->cpsr&0x80000000u){r[15]=r[14];}else{r[15]=0x78cu;}return 0;
        case 0x78cu: /* mov r2,r2, lsr #0xb */
            r[2]=(r[2]>>11);r[15]=0x790u;return 0;
        case 0x790u: /* ldr r3,[0x850] */
            LOAD(r[3],2128u,4);r[15]=0x794u;return 0;
        case 0x794u: /* and r2,r2,r3 */
            r[2]=r[2]&r[3];r[15]=0x798u;return 0;
        case 0x798u: /* ldr r1,[0x970] */
            LOAD(r[1],2416u,4);r[15]=0x79cu;return 0;
        case 0x79cu: /* ldr r4,[r1,#0x0] */
            LOAD(r[4],r[1]+0u,4);r[15]=0x7a0u;return 0;
        case 0x7a0u: /* and r4,r4,r3 */
            r[4]=r[4]&r[3];r[15]=0x7a4u;return 0;
        case 0x7a4u: /* cmp r4,r2 */
            (void)subtract(state,r[4],r[2]);r[15]=0x7a8u;return 0;
        case 0x7a8u: /* beq 0x000007c0 */
            if(state->cpsr&0x40000000u){r[15]=1984u;}else{r[15]=0x7acu;}return 0;
        case 0x7acu: /* mov r3,#0xff */
            r[3]=255u;r[15]=0x7b0u;return 0;
        case 0x7b0u: /* str r3,[0x854] */
            STORE(2132u,r[3],4);r[15]=0x7b4u;return 0;
        case 0x7b4u: /* and r4,r4,#0x6000 */
            r[4]=r[4]&24576u;r[15]=0x7b8u;return 0;
        case 0x7b8u: /* orr r2,r4,r2 */
            r[2]=r[4]|r[2];r[15]=0x7bcu;return 0;
        case 0x7bcu: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x7c0u;return 0;
        case 0x7c0u: /* ldr r3,[0x98c] */
            LOAD(r[3],2444u,4);r[15]=0x7c4u;return 0;
        case 0x7c4u: /* ldr r2,[r3,#0x0] */
            LOAD(r[2],r[3]+0u,4);r[15]=0x7c8u;return 0;
        case 0x7c8u: /* mov r2,r2, lsr #0xe */
            r[2]=(r[2]>>14);r[15]=0x7ccu;return 0;
        case 0x7ccu: /* tst r2,#0xf8 */
            temporary=r[2]&248u;logic_flags(state,temporary,0,248u);r[15]=0x7d0u;return 0;
        case 0x7d0u: /* movne r3,#0x6000 */
            if(!(state->cpsr&0x40000000u)){r[3]=24576u;}r[15]=0x7d4u;return 0;
        case 0x7d4u: /* bne 0x000007f0 */
            if(!(state->cpsr&0x40000000u)){r[15]=2032u;}else{r[15]=0x7d8u;}return 0;
        case 0x7d8u: /* tst r2,#0x4 */
            temporary=r[2]&4u;logic_flags(state,temporary,0,4u);r[15]=0x7dcu;return 0;
        case 0x7dcu: /* movne r3,#0x4000 */
            if(!(state->cpsr&0x40000000u)){r[3]=16384u;}r[15]=0x7e0u;return 0;
        case 0x7e0u: /* bne 0x000007f0 */
            if(!(state->cpsr&0x40000000u)){r[15]=2032u;}else{r[15]=0x7e4u;}return 0;
        case 0x7e4u: /* tst r2,#0x2 */
            temporary=r[2]&2u;logic_flags(state,temporary,0,2u);r[15]=0x7e8u;return 0;
        case 0x7e8u: /* movne r3,#0x2000 */
            if(!(state->cpsr&0x40000000u)){r[3]=8192u;}r[15]=0x7ecu;return 0;
        case 0x7ecu: /* moveq r3,#0x0 */
            if(state->cpsr&0x40000000u){r[3]=0u;}r[15]=0x7f0u;return 0;
        case 0x7f0u: /* ldr r4,[r1,#0x0] */
            LOAD(r[4],r[1]+0u,4);r[15]=0x7f4u;return 0;
        case 0x7f4u: /* and r2,r4,#0x6000 */
            r[2]=r[4]&24576u;r[15]=0x7f8u;return 0;
        case 0x7f8u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x7fcu;return 0;
        case 0x7fcu: /* beq 0x00000814 */
            if(state->cpsr&0x40000000u){r[15]=2068u;}else{r[15]=0x800u;}return 0;
        case 0x800u: /* ldr r2,[0x850] */
            LOAD(r[2],2128u,4);r[15]=0x804u;return 0;
        case 0x804u: /* and r2,r4,r2 */
            r[2]=r[4]&r[2];r[15]=0x808u;return 0;
        case 0x808u: /* orr r2,r2,r3 */
            r[2]=r[2]|r[3];r[15]=0x80cu;return 0;
        case 0x80cu: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x810u;return 0;
        case 0x810u: /* b 0x00000820 */
            r[15]=2080u;return 0;
        case 0x814u: /* ldr r1,[0x854] */
            LOAD(r[1],2132u,4);r[15]=0x818u;return 0;
        case 0x818u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x81cu;return 0;
        case 0x81cu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x820u;}return 0;
        case 0x820u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x824u;return 0;
        case 0x824u: /* str r1,[0x854] */
            STORE(2132u,r[1],4);r[15]=0x828u;return 0;
        case 0x828u: /* ldr r1,[0x970] */
            LOAD(r[1],2416u,4);r[15]=0x82cu;return 0;
        case 0x82cu: /* ldr r2,[r1,#0x0] */
            LOAD(r[2],r[1]+0u,4);r[15]=0x830u;return 0;
        case 0x830u: /* ldr r1,[0x990] */
            LOAD(r[1],2448u,4);r[15]=0x834u;return 0;
        case 0x834u: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x838u;return 0;
        case 0x838u: /* ldr r1,[0x98c] */
            LOAD(r[1],2444u,4);r[15]=0x83cu;return 0;
        case 0x83cu: /* ldr r2,[r1,#0x0] */
            LOAD(r[2],r[1]+0u,4);r[15]=0x840u;return 0;
        case 0x840u: /* ldr r1,[0x994] */
            LOAD(r[1],2452u,4);r[15]=0x844u;return 0;
        case 0x844u: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x848u;return 0;
        case 0x848u: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}else{switch(r[15]){
        case 0x6d0u: /* ldr r0,[0x974] */
            LOAD(r[0],2420u,4);r[15]=0x6d4u;return 0;
        case 0x6d4u: /* ldrb r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,1);r[15]=0x6d8u;return 0;
        case 0x6d8u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6dcu;return 0;
        case 0x6dcu: /* beq 0x00000700 */
            if(state->cpsr&0x40000000u){r[15]=1792u;}else{r[15]=0x6e0u;}return 0;
        case 0x6e0u: /* ldr r0,[0x96c] */
            LOAD(r[0],2412u,4);r[15]=0x6e4u;return 0;
        case 0x6e4u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x6e8u;return 0;
        case 0x6e8u: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x6ecu;return 0;
        case 0x6ecu: /* beq 0x00000700 */
            if(state->cpsr&0x40000000u){r[15]=1792u;}else{r[15]=0x6f0u;}return 0;
        case 0x6f0u: /* ldr r0,[0x978] */
            LOAD(r[0],2424u,4);r[15]=0x6f4u;return 0;
        case 0x6f4u: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x6f8u;return 0;
        case 0x6f8u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x6fcu;return 0;
        case 0x6fcu: /* b 0x0000070c */
            r[15]=1804u;return 0;
        case 0x700u: /* ldr r0,[0x978] */
            LOAD(r[0],2424u,4);r[15]=0x704u;return 0;
        case 0x704u: /* movne r1,#0x0 */
            if(!(state->cpsr&0x40000000u)){r[1]=0u;}r[15]=0x708u;return 0;
        case 0x708u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x70cu;return 0;
        case 0x70cu: /* ldr r0,[0x97c] */
            LOAD(r[0],2428u,4);r[15]=0x710u;return 0;
        case 0x710u: /* ldrb r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,1);r[15]=0x714u;return 0;
        case 0x714u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x718u;return 0;
        case 0x718u: /* beq 0x0000072c */
            if(state->cpsr&0x40000000u){r[15]=1836u;}else{r[15]=0x71cu;}return 0;
        case 0x71cu: /* ldr r0,[0x980] */
            LOAD(r[0],2432u,4);r[15]=0x720u;return 0;
        case 0x720u: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x724u;return 0;
        case 0x724u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x728u;return 0;
        case 0x728u: /* b 0x00000738 */
            r[15]=1848u;return 0;
        case 0x72cu: /* ldr r0,[0x980] */
            LOAD(r[0],2432u,4);r[15]=0x730u;return 0;
        case 0x730u: /* movne r1,#0x0 */
            if(!(state->cpsr&0x40000000u)){r[1]=0u;}r[15]=0x734u;return 0;
        case 0x734u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x738u;return 0;
        case 0x738u: /* ldr r0,[0x984] */
            LOAD(r[0],2436u,4);r[15]=0x73cu;return 0;
        case 0x73cu: /* ldrb r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,1);r[15]=0x740u;return 0;
        case 0x740u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x744u;return 0;
        case 0x744u: /* beq 0x00000758 */
            if(state->cpsr&0x40000000u){r[15]=1880u;}else{r[15]=0x748u;}return 0;
        case 0x748u: /* ldr r0,[0x988] */
            LOAD(r[0],2440u,4);r[15]=0x74cu;return 0;
        case 0x74cu: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x750u;return 0;
        case 0x750u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x754u;return 0;
        case 0x754u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x758u: /* ldr r0,[0x988] */
            LOAD(r[0],2440u,4);r[15]=0x75cu;return 0;
        case 0x75cu: /* movne r1,#0x0 */
            if(!(state->cpsr&0x40000000u)){r[1]=0u;}r[15]=0x760u;return 0;
        case 0x760u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x764u;return 0;
        case 0x764u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x768u: /* ldr r1,[0x978] */
            LOAD(r[1],2424u,4);r[15]=0x76cu;return 0;
        case 0x76cu: /* ldrb r2,[r1,#0x0] */
            LOAD(r[2],r[1]+0u,1);r[15]=0x770u;return 0;
        case 0x770u: /* tst r2,#0xff */
            temporary=r[2]&255u;logic_flags(state,temporary,0,255u);r[15]=0x774u;return 0;
        case 0x774u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x778u;}return 0;
        case 0x778u: /* ldr r1,[0x96c] */
            LOAD(r[1],2412u,4);r[15]=0x77cu;return 0;
        case 0x77cu: /* ldr r2,[r1,#0x0] */
            LOAD(r[2],r[1]+0u,4);r[15]=0x780u;return 0;
        case 0x780u: /* mov r3,#0x18000 */
            r[3]=98304u;r[15]=0x784u;return 0;
        case 0x784u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x788u;return 0;
        case 0x788u: /* movmi pc,lr */
            if(state->cpsr&0x80000000u){r[15]=r[14];}else{r[15]=0x78cu;}return 0;
        case 0x78cu: /* mov r2,r2, lsr #0xb */
            r[2]=(r[2]>>11);r[15]=0x790u;return 0;
        case 0x790u: /* ldr r3,[0x850] */
            LOAD(r[3],2128u,4);r[15]=0x794u;return 0;
        case 0x794u: /* and r2,r2,r3 */
            r[2]=r[2]&r[3];r[15]=0x798u;return 0;
        case 0x798u: /* ldr r1,[0x970] */
            LOAD(r[1],2416u,4);r[15]=0x79cu;return 0;
        case 0x79cu: /* ldr r4,[r1,#0x0] */
            LOAD(r[4],r[1]+0u,4);r[15]=0x7a0u;return 0;
        case 0x7a0u: /* and r4,r4,r3 */
            r[4]=r[4]&r[3];r[15]=0x7a4u;return 0;
        case 0x7a4u: /* cmp r4,r2 */
            (void)subtract(state,r[4],r[2]);r[15]=0x7a8u;return 0;
        case 0x7a8u: /* beq 0x000007c0 */
            if(state->cpsr&0x40000000u){r[15]=1984u;}else{r[15]=0x7acu;}return 0;
        case 0x7acu: /* mov r3,#0xff */
            r[3]=255u;r[15]=0x7b0u;return 0;
        case 0x7b0u: /* str r3,[0x854] */
            STORE(2132u,r[3],4);r[15]=0x7b4u;return 0;
        case 0x7b4u: /* and r4,r4,#0x6000 */
            r[4]=r[4]&24576u;r[15]=0x7b8u;return 0;
        case 0x7b8u: /* orr r2,r4,r2 */
            r[2]=r[4]|r[2];r[15]=0x7bcu;return 0;
        case 0x7bcu: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x7c0u;return 0;
        case 0x7c0u: /* ldr r3,[0x98c] */
            LOAD(r[3],2444u,4);r[15]=0x7c4u;return 0;
        case 0x7c4u: /* ldr r2,[r3,#0x0] */
            LOAD(r[2],r[3]+0u,4);r[15]=0x7c8u;return 0;
        case 0x7c8u: /* mov r2,r2, lsr #0xe */
            r[2]=(r[2]>>14);r[15]=0x7ccu;return 0;
        case 0x7ccu: /* tst r2,#0xf8 */
            temporary=r[2]&248u;logic_flags(state,temporary,0,248u);r[15]=0x7d0u;return 0;
        case 0x7d0u: /* movne r3,#0x6000 */
            if(!(state->cpsr&0x40000000u)){r[3]=24576u;}r[15]=0x7d4u;return 0;
        case 0x7d4u: /* bne 0x000007f0 */
            if(!(state->cpsr&0x40000000u)){r[15]=2032u;}else{r[15]=0x7d8u;}return 0;
        case 0x7d8u: /* tst r2,#0x4 */
            temporary=r[2]&4u;logic_flags(state,temporary,0,4u);r[15]=0x7dcu;return 0;
        case 0x7dcu: /* movne r3,#0x4000 */
            if(!(state->cpsr&0x40000000u)){r[3]=16384u;}r[15]=0x7e0u;return 0;
        case 0x7e0u: /* bne 0x000007f0 */
            if(!(state->cpsr&0x40000000u)){r[15]=2032u;}else{r[15]=0x7e4u;}return 0;
        case 0x7e4u: /* tst r2,#0x2 */
            temporary=r[2]&2u;logic_flags(state,temporary,0,2u);r[15]=0x7e8u;return 0;
        case 0x7e8u: /* movne r3,#0x2000 */
            if(!(state->cpsr&0x40000000u)){r[3]=8192u;}r[15]=0x7ecu;return 0;
        case 0x7ecu: /* moveq r3,#0x0 */
            if(state->cpsr&0x40000000u){r[3]=0u;}r[15]=0x7f0u;return 0;
        case 0x7f0u: /* ldr r4,[r1,#0x0] */
            LOAD(r[4],r[1]+0u,4);r[15]=0x7f4u;return 0;
        case 0x7f4u: /* and r2,r4,#0x6000 */
            r[2]=r[4]&24576u;r[15]=0x7f8u;return 0;
        case 0x7f8u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x7fcu;return 0;
        case 0x7fcu: /* beq 0x00000814 */
            if(state->cpsr&0x40000000u){r[15]=2068u;}else{r[15]=0x800u;}return 0;
        case 0x800u: /* ldr r2,[0x850] */
            LOAD(r[2],2128u,4);r[15]=0x804u;return 0;
        case 0x804u: /* and r2,r4,r2 */
            r[2]=r[4]&r[2];r[15]=0x808u;return 0;
        case 0x808u: /* orr r2,r2,r3 */
            r[2]=r[2]|r[3];r[15]=0x80cu;return 0;
        case 0x80cu: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x810u;return 0;
        case 0x810u: /* b 0x00000820 */
            r[15]=2080u;return 0;
        case 0x814u: /* ldr r1,[0x854] */
            LOAD(r[1],2132u,4);r[15]=0x818u;return 0;
        case 0x818u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x81cu;return 0;
        case 0x81cu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x820u;}return 0;
        case 0x820u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x824u;return 0;
        case 0x824u: /* str r1,[0x854] */
            STORE(2132u,r[1],4);r[15]=0x828u;return 0;
        case 0x828u: /* ldr r1,[0x970] */
            LOAD(r[1],2416u,4);r[15]=0x82cu;return 0;
        case 0x82cu: /* ldr r2,[r1,#0x0] */
            LOAD(r[2],r[1]+0u,4);r[15]=0x830u;return 0;
        case 0x830u: /* ldr r1,[0x990] */
            LOAD(r[1],2448u,4);r[15]=0x834u;return 0;
        case 0x834u: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x838u;return 0;
        case 0x838u: /* ldr r1,[0x98c] */
            LOAD(r[1],2444u,4);r[15]=0x83cu;return 0;
        case 0x83cu: /* ldr r2,[r1,#0x0] */
            LOAD(r[2],r[1]+0u,4);r[15]=0x840u;return 0;
        case 0x840u: /* ldr r1,[0x994] */
            LOAD(r[1],2452u,4);r[15]=0x844u;return 0;
        case 0x844u: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x848u;return 0;
        case 0x848u: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}
}

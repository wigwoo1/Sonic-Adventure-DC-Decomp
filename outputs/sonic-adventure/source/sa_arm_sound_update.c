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
int sa_arm_sound_update_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
        case 0x615cu: /* ldr r0,[0x63bc] */
            LOAD(r[0],25532u,4);r[15]=0x6160u;return 0;
        case 0x6160u: /* ldrb r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,1);r[15]=0x6164u;return 0;
        case 0x6164u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6168u;return 0;
        case 0x6168u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x616cu;}return 0;
        case 0x616cu: /* ldr r0,[0x612c] */
            LOAD(r[0],24876u,4);r[15]=0x6170u;return 0;
        case 0x6170u: /* ldrb r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,1);r[15]=0x6174u;return 0;
        case 0x6174u: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x6178u;return 0;
        case 0x6178u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x617cu;}return 0;
        case 0x617cu: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x6180u;return 0;
        case 0x6180u: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x6184u;return 0;
        case 0x6184u: /* stmdb sp!,{r0,r1,lr} */
            STORE(r[13]-12u,r[0],4);STORE(r[13]-8u,r[1],4);STORE(r[13]-4u,r[14],4);r[13]-=12u;r[15]=0x6188u;return 0;
        case 0x6188u: /* bl 0x000062b0 */
            r[14]=0x618cu;r[15]=25264u;return 0;
        case 0x618cu: /* bl 0x00005d78 */
            r[14]=0x6190u;r[15]=23928u;return 0;
        case 0x6190u: /* bl 0x00006274 */
            r[14]=0x6194u;r[15]=25204u;return 0;
        case 0x6194u: /* ldmia sp!,{r0,r1,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[1],r[13]+4u,4);LOAD(r[14],r[13]+8u,4);r[13]+=12u;r[15]=0x6198u;return 0;
        case 0x6198u: /* ldr r0,[0x6124] */
            LOAD(r[0],24868u,4);r[15]=0x619cu;return 0;
        case 0x619cu: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x61a0u;return 0;
        case 0x61a0u: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x61a4u;return 0;
        case 0x61a4u: /* beq 0x000061b0 */
            if(state->cpsr&0x40000000u){r[15]=25008u;}else{r[15]=0x61a8u;}return 0;
        case 0x61a8u: /* ldr r0,[0x63c0] */
            LOAD(r[0],25536u,4);r[15]=0x61acu;return 0;
        case 0x61acu: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x61b0u;return 0;
        case 0x61b0u: /* ldr r1,[0x63c4] */
            LOAD(r[1],25540u,4);r[15]=0x61b4u;return 0;
        case 0x61b4u: /* mov r0,#0xff */
            r[0]=255u;r[15]=0x61b8u;return 0;
        case 0x61b8u: /* strb r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],1);r[15]=0x61bcu;return 0;
        case 0x61bcu: /* ldr r2,[0x6134] */
            LOAD(r[2],24884u,4);r[15]=0x61c0u;return 0;
        case 0x61c0u: /* ldr r1,[0x63c8] */
            LOAD(r[1],25544u,4);r[15]=0x61c4u;return 0;
        case 0x61c4u: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x61c8u;return 0;
        case 0x61c8u: /* mov r2,#0xc00 */
            r[2]=3072u;r[15]=0x61ccu;return 0;
        case 0x61ccu: /* ldr r1,[0x63cc] */
            LOAD(r[1],25548u,4);r[15]=0x61d0u;return 0;
        case 0x61d0u: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x61d4u;return 0;
        case 0x61d4u: /* ldr r1,[0x6124] */
            LOAD(r[1],24868u,4);r[15]=0x61d8u;return 0;
        case 0x61d8u: /* ldr r2,[r1,#0x0] */
            LOAD(r[2],r[1]+0u,4);r[15]=0x61dcu;return 0;
        case 0x61dcu: /* ldr r1,[0x63d0] */
            LOAD(r[1],25552u,4);r[15]=0x61e0u;return 0;
        case 0x61e0u: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x61e4u;return 0;
        case 0x61e4u: /* ldr r0,[0x6128] */
            LOAD(r[0],24872u,4);r[15]=0x61e8u;return 0;
        case 0x61e8u: /* ldr r0,[r0,#0x0] */
            LOAD(r[0],r[0]+0u,4);r[15]=0x61ecu;return 0;
        case 0x61ecu: /* ldrb r2,[r0,#0x24] */
            LOAD(r[2],r[0]+36u,1);r[15]=0x61f0u;return 0;
        case 0x61f0u: /* ldr r1,[0x6154] */
            LOAD(r[1],24916u,4);r[15]=0x61f4u;return 0;
        case 0x61f4u: /* strb r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],1);r[15]=0x61f8u;return 0;
        case 0x61f8u: /* ldrb r2,[r0,#0x22] */
            LOAD(r[2],r[0]+34u,1);r[15]=0x61fcu;return 0;
        case 0x61fcu: /* ldr r1,[0x63d4] */
            LOAD(r[1],25556u,4);r[15]=0x6200u;return 0;
        case 0x6200u: /* strb r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],1);r[15]=0x6204u;return 0;
        case 0x6204u: /* ldrb r2,[r0,#0x23] */
            LOAD(r[2],r[0]+35u,1);r[15]=0x6208u;return 0;
        case 0x6208u: /* ldr r1,[0x6150] */
            LOAD(r[1],24912u,4);r[15]=0x620cu;return 0;
        case 0x620cu: /* strb r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],1);r[15]=0x6210u;return 0;
        case 0x6210u: /* add r0,r0,#0x40 */
            r[0]=r[0]+64u;r[15]=0x6214u;return 0;
        case 0x6214u: /* ldr r1,[0x6134] */
            LOAD(r[1],24884u,4);r[15]=0x6218u;return 0;
        case 0x6218u: /* ldr r2,[0x6268] */
            LOAD(r[2],25192u,4);r[15]=0x621cu;return 0;
        case 0x621cu: /* ldr r3,[r0],#0x4 */
            LOAD(r[3],r[0],4);r[0]+=4u;r[15]=0x6220u;return 0;
        case 0x6220u: /* str r3,[r1],#0x4 */
            STORE(r[1],r[3],4);r[1]+=4u;r[15]=0x6224u;return 0;
        case 0x6224u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x6228u;return 0;
        case 0x6228u: /* bne 0x0000621c */
            if(!(state->cpsr&0x40000000u)){r[15]=25116u;}else{r[15]=0x622cu;}return 0;
        case 0x622cu: /* stmdb sp!,{r0,r1,lr} */
            STORE(r[13]-12u,r[0],4);STORE(r[13]-8u,r[1],4);STORE(r[13]-4u,r[14],4);r[13]-=12u;r[15]=0x6230u;return 0;
        case 0x6230u: /* bl 0x00006274 */
            r[14]=0x6234u;r[15]=25204u;return 0;
        case 0x6234u: /* ldmia sp!,{r0,r1,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[1],r[13]+4u,4);LOAD(r[14],r[13]+8u,4);r[13]+=12u;r[15]=0x6238u;return 0;
        case 0x6238u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x623cu;return 0;
        case 0x623cu: /* bl 0x00006310 */
            r[14]=0x6240u;r[15]=25360u;return 0;
        case 0x6240u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x6244u;return 0;
        case 0x6244u: /* ldr r8,[0x63d8] */
            LOAD(r[8],25560u,4);r[15]=0x6248u;return 0;
        case 0x6248u: /* ldrb r9,[r8,#0x0] */
            LOAD(r[9],r[8]+0u,1);r[15]=0x624cu;return 0;
        case 0x624cu: /* ldr r8,[0x6140] */
            LOAD(r[8],24896u,4);r[15]=0x6250u;return 0;
        case 0x6250u: /* strb r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],1);r[15]=0x6254u;return 0;
        case 0x6254u: /* ldr r8,[0x63c4] */
            LOAD(r[8],25540u,4);r[15]=0x6258u;return 0;
        case 0x6258u: /* mov r9,#0x0 */
            r[9]=0u;r[15]=0x625cu;return 0;
        case 0x625cu: /* strb r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],1);r[15]=0x6260u;return 0;
        case 0x6260u: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}else{switch(r[15]){
        case 0x6164u: /* ldr r0,[0x63c4] */
            LOAD(r[0],25540u,4);r[15]=0x6168u;return 0;
        case 0x6168u: /* ldrb r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,1);r[15]=0x616cu;return 0;
        case 0x616cu: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x6170u;return 0;
        case 0x6170u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x6174u;}return 0;
        case 0x6174u: /* ldr r0,[0x6134] */
            LOAD(r[0],24884u,4);r[15]=0x6178u;return 0;
        case 0x6178u: /* ldrb r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,1);r[15]=0x617cu;return 0;
        case 0x617cu: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x6180u;return 0;
        case 0x6180u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x6184u;}return 0;
        case 0x6184u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x6188u;return 0;
        case 0x6188u: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x618cu;return 0;
        case 0x618cu: /* stmdb sp!,{r0,r1,lr} */
            STORE(r[13]-12u,r[0],4);STORE(r[13]-8u,r[1],4);STORE(r[13]-4u,r[14],4);r[13]-=12u;r[15]=0x6190u;return 0;
        case 0x6190u: /* bl 0x000062b8 */
            r[14]=0x6194u;r[15]=25272u;return 0;
        case 0x6194u: /* bl 0x00005d80 */
            r[14]=0x6198u;r[15]=23936u;return 0;
        case 0x6198u: /* bl 0x0000627c */
            r[14]=0x619cu;r[15]=25212u;return 0;
        case 0x619cu: /* ldmia sp!,{r0,r1,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[1],r[13]+4u,4);LOAD(r[14],r[13]+8u,4);r[13]+=12u;r[15]=0x61a0u;return 0;
        case 0x61a0u: /* ldr r0,[0x612c] */
            LOAD(r[0],24876u,4);r[15]=0x61a4u;return 0;
        case 0x61a4u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x61a8u;return 0;
        case 0x61a8u: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x61acu;return 0;
        case 0x61acu: /* beq 0x000061b8 */
            if(state->cpsr&0x40000000u){r[15]=25016u;}else{r[15]=0x61b0u;}return 0;
        case 0x61b0u: /* ldr r0,[0x63c8] */
            LOAD(r[0],25544u,4);r[15]=0x61b4u;return 0;
        case 0x61b4u: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x61b8u;return 0;
        case 0x61b8u: /* ldr r1,[0x63cc] */
            LOAD(r[1],25548u,4);r[15]=0x61bcu;return 0;
        case 0x61bcu: /* mov r0,#0xff */
            r[0]=255u;r[15]=0x61c0u;return 0;
        case 0x61c0u: /* strb r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],1);r[15]=0x61c4u;return 0;
        case 0x61c4u: /* ldr r2,[0x613c] */
            LOAD(r[2],24892u,4);r[15]=0x61c8u;return 0;
        case 0x61c8u: /* ldr r1,[0x63d0] */
            LOAD(r[1],25552u,4);r[15]=0x61ccu;return 0;
        case 0x61ccu: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x61d0u;return 0;
        case 0x61d0u: /* mov r2,#0xc00 */
            r[2]=3072u;r[15]=0x61d4u;return 0;
        case 0x61d4u: /* ldr r1,[0x63d4] */
            LOAD(r[1],25556u,4);r[15]=0x61d8u;return 0;
        case 0x61d8u: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x61dcu;return 0;
        case 0x61dcu: /* ldr r1,[0x612c] */
            LOAD(r[1],24876u,4);r[15]=0x61e0u;return 0;
        case 0x61e0u: /* ldr r2,[r1,#0x0] */
            LOAD(r[2],r[1]+0u,4);r[15]=0x61e4u;return 0;
        case 0x61e4u: /* ldr r1,[0x63d8] */
            LOAD(r[1],25560u,4);r[15]=0x61e8u;return 0;
        case 0x61e8u: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x61ecu;return 0;
        case 0x61ecu: /* ldr r0,[0x6130] */
            LOAD(r[0],24880u,4);r[15]=0x61f0u;return 0;
        case 0x61f0u: /* ldr r0,[r0,#0x0] */
            LOAD(r[0],r[0]+0u,4);r[15]=0x61f4u;return 0;
        case 0x61f4u: /* ldrb r2,[r0,#0x24] */
            LOAD(r[2],r[0]+36u,1);r[15]=0x61f8u;return 0;
        case 0x61f8u: /* ldr r1,[0x615c] */
            LOAD(r[1],24924u,4);r[15]=0x61fcu;return 0;
        case 0x61fcu: /* strb r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],1);r[15]=0x6200u;return 0;
        case 0x6200u: /* ldrb r2,[r0,#0x22] */
            LOAD(r[2],r[0]+34u,1);r[15]=0x6204u;return 0;
        case 0x6204u: /* ldr r1,[0x63dc] */
            LOAD(r[1],25564u,4);r[15]=0x6208u;return 0;
        case 0x6208u: /* strb r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],1);r[15]=0x620cu;return 0;
        case 0x620cu: /* ldrb r2,[r0,#0x23] */
            LOAD(r[2],r[0]+35u,1);r[15]=0x6210u;return 0;
        case 0x6210u: /* ldr r1,[0x6158] */
            LOAD(r[1],24920u,4);r[15]=0x6214u;return 0;
        case 0x6214u: /* strb r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],1);r[15]=0x6218u;return 0;
        case 0x6218u: /* add r0,r0,#0x40 */
            r[0]=r[0]+64u;r[15]=0x621cu;return 0;
        case 0x621cu: /* ldr r1,[0x613c] */
            LOAD(r[1],24892u,4);r[15]=0x6220u;return 0;
        case 0x6220u: /* ldr r2,[0x6270] */
            LOAD(r[2],25200u,4);r[15]=0x6224u;return 0;
        case 0x6224u: /* ldr r3,[r0],#0x4 */
            LOAD(r[3],r[0],4);r[0]+=4u;r[15]=0x6228u;return 0;
        case 0x6228u: /* str r3,[r1],#0x4 */
            STORE(r[1],r[3],4);r[1]+=4u;r[15]=0x622cu;return 0;
        case 0x622cu: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x6230u;return 0;
        case 0x6230u: /* bne 0x00006224 */
            if(!(state->cpsr&0x40000000u)){r[15]=25124u;}else{r[15]=0x6234u;}return 0;
        case 0x6234u: /* stmdb sp!,{r0,r1,lr} */
            STORE(r[13]-12u,r[0],4);STORE(r[13]-8u,r[1],4);STORE(r[13]-4u,r[14],4);r[13]-=12u;r[15]=0x6238u;return 0;
        case 0x6238u: /* bl 0x0000627c */
            r[14]=0x623cu;r[15]=25212u;return 0;
        case 0x623cu: /* ldmia sp!,{r0,r1,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[1],r[13]+4u,4);LOAD(r[14],r[13]+8u,4);r[13]+=12u;r[15]=0x6240u;return 0;
        case 0x6240u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6244u;return 0;
        case 0x6244u: /* bl 0x00006318 */
            r[14]=0x6248u;r[15]=25368u;return 0;
        case 0x6248u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x624cu;return 0;
        case 0x624cu: /* ldr r8,[0x63e0] */
            LOAD(r[8],25568u,4);r[15]=0x6250u;return 0;
        case 0x6250u: /* ldrb r9,[r8,#0x0] */
            LOAD(r[9],r[8]+0u,1);r[15]=0x6254u;return 0;
        case 0x6254u: /* ldr r8,[0x6148] */
            LOAD(r[8],24904u,4);r[15]=0x6258u;return 0;
        case 0x6258u: /* strb r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],1);r[15]=0x625cu;return 0;
        case 0x625cu: /* ldr r8,[0x63cc] */
            LOAD(r[8],25548u,4);r[15]=0x6260u;return 0;
        case 0x6260u: /* mov r9,#0x0 */
            r[9]=0u;r[15]=0x6264u;return 0;
        case 0x6264u: /* strb r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],1);r[15]=0x6268u;return 0;
        case 0x6268u: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}
}

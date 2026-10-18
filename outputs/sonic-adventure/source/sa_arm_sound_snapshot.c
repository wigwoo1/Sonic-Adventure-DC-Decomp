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
int sa_arm_sound_snapshot_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
        case 0x63f8u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x63fcu;return 0;
        case 0x63fcu: /* ldr r1,[0x6638] */
            LOAD(r[1],26168u,4);r[15]=0x6400u;return 0;
        case 0x6400u: /* mov r3,#0x10000 */
            r[3]=65536u;r[15]=0x6404u;return 0;
        case 0x6404u: /* mov r4,#0x8000 */
            r[4]=32768u;r[15]=0x6408u;return 0;
        case 0x6408u: /* mov r12,#0x8 */
            r[12]=8u;r[15]=0x640cu;return 0;
        case 0x640cu: /* ldrb r2,[r0,#0x1] */
            LOAD(r[2],r[0]+1u,1);r[15]=0x6410u;return 0;
        case 0x6410u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x6414u;return 0;
        case 0x6414u: /* strb r2,[r1,#0x1] */
            STORE(r[1]+1u,r[2],1);r[15]=0x6418u;return 0;
        case 0x6418u: /* ldrb r2,[r0,#0x7] */
            LOAD(r[2],r[0]+7u,1);r[15]=0x641cu;return 0;
        case 0x641cu: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x6420u;return 0;
        case 0x6420u: /* strb r2,[r1,#0x2] */
            STORE(r[1]+2u,r[2],1);r[15]=0x6424u;return 0;
        case 0x6424u: /* ldr r2,[r0,#0x10] */
            LOAD(r[2],r[0]+16u,4);r[15]=0x6428u;return 0;
        case 0x6428u: /* sub r2,r2,r4 */
            r[2]=r[2]-r[4];r[15]=0x642cu;return 0;
        case 0x642cu: /* str r2,[r1,#0x8] */
            STORE(r[1]+8u,r[2],4);r[15]=0x6430u;return 0;
        case 0x6430u: /* ldr r2,[r0,#0x1c] */
            LOAD(r[2],r[0]+28u,4);r[15]=0x6434u;return 0;
        case 0x6434u: /* sub r2,r2,r4 */
            r[2]=r[2]-r[4];r[15]=0x6438u;return 0;
        case 0x6438u: /* str r2,[r1,#0x4] */
            STORE(r[1]+4u,r[2],4);r[15]=0x643cu;return 0;
        case 0x643cu: /* ldrb r2,[r0,#0x24] */
            LOAD(r[2],r[0]+36u,1);r[15]=0x6440u;return 0;
        case 0x6440u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x6444u;return 0;
        case 0x6444u: /* strb r2,[r1,#0x11] */
            STORE(r[1]+17u,r[2],1);r[15]=0x6448u;return 0;
        case 0x6448u: /* ldrb r2,[r0,#0x26] */
            LOAD(r[2],r[0]+38u,1);r[15]=0x644cu;return 0;
        case 0x644cu: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x6450u;return 0;
        case 0x6450u: /* strb r2,[r1,#0x10] */
            STORE(r[1]+16u,r[2],1);r[15]=0x6454u;return 0;
        case 0x6454u: /* ldr r2,[r3,#0x10] */
            LOAD(r[2],r[3]+16u,4);r[15]=0x6458u;return 0;
        case 0x6458u: /* ldr r5,[r3,#0x14] */
            LOAD(r[5],r[3]+20u,4);r[15]=0x645cu;return 0;
        case 0x645cu: /* sub r2,r2,r5 */
            r[2]=r[2]-r[5];r[15]=0x6460u;return 0;
        case 0x6460u: /* str r2,[r1,#0x18] */
            STORE(r[1]+24u,r[2],4);r[15]=0x6464u;return 0;
        case 0x6464u: /* ldr r2,[r3,#0x30] */
            LOAD(r[2],r[3]+48u,4);r[15]=0x6468u;return 0;
        case 0x6468u: /* str r2,[r1,#0x14] */
            STORE(r[1]+20u,r[2],4);r[15]=0x646cu;return 0;
        case 0x646cu: /* ldr r2,[r3,#0x18] */
            LOAD(r[2],r[3]+24u,4);r[15]=0x6470u;return 0;
        case 0x6470u: /* str r2,[r1,#0xc] */
            STORE(r[1]+12u,r[2],4);r[15]=0x6474u;return 0;
        case 0x6474u: /* ldrb r2,[r0,#0x29] */
            LOAD(r[2],r[0]+41u,1);r[15]=0x6478u;return 0;
        case 0x6478u: /* strb r2,[r1,#0x1c] */
            STORE(r[1]+28u,r[2],1);r[15]=0x647cu;return 0;
        case 0x647cu: /* ldrb r5,[r0,#0x2a] */
            LOAD(r[5],r[0]+42u,1);r[15]=0x6480u;return 0;
        case 0x6480u: /* strb r5,[r1,#0x1f] */
            STORE(r[1]+31u,r[5],1);r[15]=0x6484u;return 0;
        case 0x6484u: /* orrs r5,r2,r5 */
            r[5]=r[2]|r[5];logic_flags(state,r[5],0,0);r[15]=0x6488u;return 0;
        case 0x6488u: /* ldrb r2,[r0,#0x28] */
            LOAD(r[2],r[0]+40u,1);r[15]=0x648cu;return 0;
        case 0x648cu: /* orrne r2,r2,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[2]=r[2]|128u;}r[15]=0x6490u;return 0;
        case 0x6490u: /* strb r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],1);r[15]=0x6494u;return 0;
        case 0x6494u: /* add r0,r0,#0x80 */
            r[0]=r[0]+128u;r[15]=0x6498u;return 0;
        case 0x6498u: /* add r1,r1,#0x20 */
            r[1]=r[1]+32u;r[15]=0x649cu;return 0;
        case 0x649cu: /* add r3,r3,#0x40 */
            r[3]=r[3]+64u;r[15]=0x64a0u;return 0;
        case 0x64a0u: /* subs r12,r12,#0x1 */
            r[12]=subtract(state,r[12],1u);r[15]=0x64a4u;return 0;
        case 0x64a4u: /* bne 0x0000640c */
            if(!(state->cpsr&0x40000000u)){r[15]=25612u;}else{r[15]=0x64a8u;}return 0;
        case 0x64a8u: /* mov r0,#0x11000 */
            r[0]=69632u;r[15]=0x64acu;return 0;
        case 0x64acu: /* mov r1,#0x13800 */
            r[1]=79872u;r[15]=0x64b0u;return 0;
        case 0x64b0u: /* mov r12,#0x8 */
            r[12]=8u;r[15]=0x64b4u;return 0;
        case 0x64b4u: /* ldrb r2,[r0,#0x1e] */
            LOAD(r[2],r[0]+30u,1);r[15]=0x64b8u;return 0;
        case 0x64b8u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x64bcu;return 0;
        case 0x64bcu: /* strb r2,[r1,#0x1] */
            STORE(r[1]+1u,r[2],1);r[15]=0x64c0u;return 0;
        case 0x64c0u: /* ldrb r2,[r0,#0x1f] */
            LOAD(r[2],r[0]+31u,1);r[15]=0x64c4u;return 0;
        case 0x64c4u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x64c8u;return 0;
        case 0x64c8u: /* strb r2,[r1,#0x2] */
            STORE(r[1]+2u,r[2],1);r[15]=0x64ccu;return 0;
        case 0x64ccu: /* ldr r2,[r0,#0x44] */
            LOAD(r[2],r[0]+68u,4);r[15]=0x64d0u;return 0;
        case 0x64d0u: /* sub r2,r2,r4 */
            r[2]=r[2]-r[4];r[15]=0x64d4u;return 0;
        case 0x64d4u: /* str r2,[r1,#0x4] */
            STORE(r[1]+4u,r[2],4);r[15]=0x64d8u;return 0;
        case 0x64d8u: /* ldr r2,[r0,#0x50] */
            LOAD(r[2],r[0]+80u,4);r[15]=0x64dcu;return 0;
        case 0x64dcu: /* sub r2,r2,r4 */
            r[2]=r[2]-r[4];r[15]=0x64e0u;return 0;
        case 0x64e0u: /* str r2,[r1,#0x8] */
            STORE(r[1]+8u,r[2],4);r[15]=0x64e4u;return 0;
        case 0x64e4u: /* ldrb r2,[r0,#0x1b] */
            LOAD(r[2],r[0]+27u,1);r[15]=0x64e8u;return 0;
        case 0x64e8u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x64ecu;return 0;
        case 0x64ecu: /* strb r2,[r1,#0x11] */
            STORE(r[1]+17u,r[2],1);r[15]=0x64f0u;return 0;
        case 0x64f0u: /* ldrb r2,[r0,#0x1a] */
            LOAD(r[2],r[0]+26u,1);r[15]=0x64f4u;return 0;
        case 0x64f4u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x64f8u;return 0;
        case 0x64f8u: /* strb r2,[r1,#0x10] */
            STORE(r[1]+16u,r[2],1);r[15]=0x64fcu;return 0;
        case 0x64fcu: /* ldrb r2,[r0,#0x0] */
            LOAD(r[2],r[0]+0u,1);r[15]=0x6500u;return 0;
        case 0x6500u: /* strb r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],1);r[15]=0x6504u;return 0;
        case 0x6504u: /* tst r2,#0x1 */
            temporary=r[2]&1u;logic_flags(state,temporary,0,1u);r[15]=0x6508u;return 0;
        case 0x6508u: /* beq 0x00006550 */
            if(state->cpsr&0x40000000u){r[15]=25936u;}else{r[15]=0x650cu;}return 0;
        case 0x650cu: /* ldr r2,[r0,#0x4] */
            LOAD(r[2],r[0]+4u,4);r[15]=0x6510u;return 0;
        case 0x6510u: /* str r2,[r1,#0x18] */
            STORE(r[1]+24u,r[2],4);r[15]=0x6514u;return 0;
        case 0x6514u: /* ldr r2,[r0,#0x38] */
            LOAD(r[2],r[0]+56u,4);r[15]=0x6518u;return 0;
        case 0x6518u: /* str r2,[r1,#0xc] */
            STORE(r[1]+12u,r[2],4);r[15]=0x651cu;return 0;
        case 0x651cu: /* ldrb r2,[r0,#0x3] */
            LOAD(r[2],r[0]+3u,1);r[15]=0x6520u;return 0;
        case 0x6520u: /* add r2,r2,#0x30 */
            r[2]=r[2]+48u;r[15]=0x6524u;return 0;
        case 0x6524u: /* mov r2,r2, lsl #0x7 */
            r[2]=(r[2]<<7);r[15]=0x6528u;return 0;
        case 0x6528u: /* mov r5,#0x800000 */
            r[5]=8388608u;r[15]=0x652cu;return 0;
        case 0x652cu: /* add r5,r5,r2 */
            r[5]=r[5]+r[2];r[15]=0x6530u;return 0;
        case 0x6530u: /* ldr r2,[r5,#0xc] */
            LOAD(r[2],r[5]+12u,4);r[15]=0x6534u;return 0;
        case 0x6534u: /* ldr r3,[r5,#0x8] */
            LOAD(r[3],r[5]+8u,4);r[15]=0x6538u;return 0;
        case 0x6538u: /* sub r3,r2,r3 */
            r[3]=r[2]-r[3];r[15]=0x653cu;return 0;
        case 0x653cu: /* ldrb r5,[r0,#0x2c] */
            LOAD(r[5],r[0]+44u,1);r[15]=0x6540u;return 0;
        case 0x6540u: /* mul r2,r3,r5 */
            r[2]=r[3]*r[5];r[15]=0x6544u;return 0;
        case 0x6544u: /* ldr r3,[r0,#0x4] */
            LOAD(r[3],r[0]+4u,4);r[15]=0x6548u;return 0;
        case 0x6548u: /* add r2,r2,r3 */
            r[2]=r[2]+r[3];r[15]=0x654cu;return 0;
        case 0x654cu: /* str r2,[r1,#0x14] */
            STORE(r[1]+20u,r[2],4);r[15]=0x6550u;return 0;
        case 0x6550u: /* add r0,r0,#0x58 */
            r[0]=r[0]+88u;r[15]=0x6554u;return 0;
        case 0x6554u: /* add r1,r1,#0x20 */
            r[1]=r[1]+32u;r[15]=0x6558u;return 0;
        case 0x6558u: /* subs r12,r12,#0x1 */
            r[12]=subtract(state,r[12],1u);r[15]=0x655cu;return 0;
        case 0x655cu: /* bne 0x000064b4 */
            if(!(state->cpsr&0x40000000u)){r[15]=25780u;}else{r[15]=0x6560u;}return 0;
        case 0x6560u: /* ldr r0,[0x5858] */
            LOAD(r[0],22616u,4);r[15]=0x6564u;return 0;
        case 0x6564u: /* ldr r1,[0x5854] */
            LOAD(r[1],22612u,4);r[15]=0x6568u;return 0;
        case 0x6568u: /* mov r4,#0x8000 */
            r[4]=32768u;r[15]=0x656cu;return 0;
        case 0x656cu: /* mov r12,#0x10 */
            r[12]=16u;r[15]=0x6570u;return 0;
        case 0x6570u: /* ldrb r2,[r0,#0x1e] */
            LOAD(r[2],r[0]+30u,1);r[15]=0x6574u;return 0;
        case 0x6574u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x6578u;return 0;
        case 0x6578u: /* strb r2,[r1,#0x1] */
            STORE(r[1]+1u,r[2],1);r[15]=0x657cu;return 0;
        case 0x657cu: /* ldrb r2,[r0,#0x1f] */
            LOAD(r[2],r[0]+31u,1);r[15]=0x6580u;return 0;
        case 0x6580u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x6584u;return 0;
        case 0x6584u: /* strb r2,[r1,#0x2] */
            STORE(r[1]+2u,r[2],1);r[15]=0x6588u;return 0;
        case 0x6588u: /* ldr r2,[r0,#0x44] */
            LOAD(r[2],r[0]+68u,4);r[15]=0x658cu;return 0;
        case 0x658cu: /* sub r2,r2,r4 */
            r[2]=r[2]-r[4];r[15]=0x6590u;return 0;
        case 0x6590u: /* str r2,[r1,#0x4] */
            STORE(r[1]+4u,r[2],4);r[15]=0x6594u;return 0;
        case 0x6594u: /* ldr r2,[r0,#0x50] */
            LOAD(r[2],r[0]+80u,4);r[15]=0x6598u;return 0;
        case 0x6598u: /* sub r2,r2,r4 */
            r[2]=r[2]-r[4];r[15]=0x659cu;return 0;
        case 0x659cu: /* str r2,[r1,#0x8] */
            STORE(r[1]+8u,r[2],4);r[15]=0x65a0u;return 0;
        case 0x65a0u: /* ldrb r2,[r0,#0x1b] */
            LOAD(r[2],r[0]+27u,1);r[15]=0x65a4u;return 0;
        case 0x65a4u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x65a8u;return 0;
        case 0x65a8u: /* strb r2,[r1,#0x11] */
            STORE(r[1]+17u,r[2],1);r[15]=0x65acu;return 0;
        case 0x65acu: /* ldrb r2,[r0,#0x1a] */
            LOAD(r[2],r[0]+26u,1);r[15]=0x65b0u;return 0;
        case 0x65b0u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x65b4u;return 0;
        case 0x65b4u: /* strb r2,[r1,#0x10] */
            STORE(r[1]+16u,r[2],1);r[15]=0x65b8u;return 0;
        case 0x65b8u: /* ldrb r2,[r0,#0x0] */
            LOAD(r[2],r[0]+0u,1);r[15]=0x65bcu;return 0;
        case 0x65bcu: /* strb r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],1);r[15]=0x65c0u;return 0;
        case 0x65c0u: /* tst r2,#0x1 */
            temporary=r[2]&1u;logic_flags(state,temporary,0,1u);r[15]=0x65c4u;return 0;
        case 0x65c4u: /* beq 0x00006608 */
            if(state->cpsr&0x40000000u){r[15]=26120u;}else{r[15]=0x65c8u;}return 0;
        case 0x65c8u: /* ldr r2,[r0,#0x4] */
            LOAD(r[2],r[0]+4u,4);r[15]=0x65ccu;return 0;
        case 0x65ccu: /* str r2,[r1,#0x18] */
            STORE(r[1]+24u,r[2],4);r[15]=0x65d0u;return 0;
        case 0x65d0u: /* ldr r2,[r0,#0x2c] */
            LOAD(r[2],r[0]+44u,4);r[15]=0x65d4u;return 0;
        case 0x65d4u: /* str r2,[r1,#0xc] */
            STORE(r[1]+12u,r[2],4);r[15]=0x65d8u;return 0;
        case 0x65d8u: /* ldr r2,[r0,#0x34] */
            LOAD(r[2],r[0]+52u,4);r[15]=0x65dcu;return 0;
        case 0x65dcu: /* ldrb r5,[r0,#0x27] */
            LOAD(r[5],r[0]+39u,1);r[15]=0x65e0u;return 0;
        case 0x65e0u: /* tst r5,#0x3 */
            temporary=r[5]&3u;logic_flags(state,temporary,0,3u);r[15]=0x65e4u;return 0;
        case 0x65e4u: /* moveq r2,r2, lsr #0x1 */
            if(state->cpsr&0x40000000u){r[2]=(r[2]>>1);}r[15]=0x65e8u;return 0;
        case 0x65e8u: /* beq 0x000065f8 */
            if(state->cpsr&0x40000000u){r[15]=26104u;}else{r[15]=0x65ecu;}return 0;
        case 0x65ecu: /* tst r5,#0x2 */
            temporary=r[5]&2u;logic_flags(state,temporary,0,2u);r[15]=0x65f0u;return 0;
        case 0x65f0u: /* beq 0x000065f8 */
            if(state->cpsr&0x40000000u){r[15]=26104u;}else{r[15]=0x65f4u;}return 0;
        case 0x65f4u: /* mov r2,r2, lsl #0x1 */
            r[2]=(r[2]<<1);r[15]=0x65f8u;return 0;
        case 0x65f8u: /* ldr r3,[r0,#0x2c] */
            LOAD(r[3],r[0]+44u,4);r[15]=0x65fcu;return 0;
        case 0x65fcu: /* ldr r5,[r0,#0x4] */
            LOAD(r[5],r[0]+4u,4);r[15]=0x6600u;return 0;
        case 0x6600u: /* mla r2,r3,r2,r5 */
            r[2]=r[3]*r[2]+r[5];r[15]=0x6604u;return 0;
        case 0x6604u: /* str r2,[r1,#0x14] */
            STORE(r[1]+20u,r[2],4);r[15]=0x6608u;return 0;
        case 0x6608u: /* add r0,r0,#0x58 */
            r[0]=r[0]+88u;r[15]=0x660cu;return 0;
        case 0x660cu: /* add r1,r1,#0x20 */
            r[1]=r[1]+32u;r[15]=0x6610u;return 0;
        case 0x6610u: /* subs r12,r12,#0x1 */
            r[12]=subtract(state,r[12],1u);r[15]=0x6614u;return 0;
        case 0x6614u: /* bne 0x00006570 */
            if(!(state->cpsr&0x40000000u)){r[15]=25968u;}else{r[15]=0x6618u;}return 0;
        case 0x6618u: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}else{switch(r[15]){
        case 0x6400u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x6404u;return 0;
        case 0x6404u: /* ldr r1,[0x6640] */
            LOAD(r[1],26176u,4);r[15]=0x6408u;return 0;
        case 0x6408u: /* mov r3,#0x10000 */
            r[3]=65536u;r[15]=0x640cu;return 0;
        case 0x640cu: /* mov r4,#0x8000 */
            r[4]=32768u;r[15]=0x6410u;return 0;
        case 0x6410u: /* mov r12,#0x8 */
            r[12]=8u;r[15]=0x6414u;return 0;
        case 0x6414u: /* ldrb r2,[r0,#0x1] */
            LOAD(r[2],r[0]+1u,1);r[15]=0x6418u;return 0;
        case 0x6418u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x641cu;return 0;
        case 0x641cu: /* strb r2,[r1,#0x1] */
            STORE(r[1]+1u,r[2],1);r[15]=0x6420u;return 0;
        case 0x6420u: /* ldrb r2,[r0,#0x7] */
            LOAD(r[2],r[0]+7u,1);r[15]=0x6424u;return 0;
        case 0x6424u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x6428u;return 0;
        case 0x6428u: /* strb r2,[r1,#0x2] */
            STORE(r[1]+2u,r[2],1);r[15]=0x642cu;return 0;
        case 0x642cu: /* ldr r2,[r0,#0x10] */
            LOAD(r[2],r[0]+16u,4);r[15]=0x6430u;return 0;
        case 0x6430u: /* sub r2,r2,r4 */
            r[2]=r[2]-r[4];r[15]=0x6434u;return 0;
        case 0x6434u: /* str r2,[r1,#0x8] */
            STORE(r[1]+8u,r[2],4);r[15]=0x6438u;return 0;
        case 0x6438u: /* ldr r2,[r0,#0x1c] */
            LOAD(r[2],r[0]+28u,4);r[15]=0x643cu;return 0;
        case 0x643cu: /* sub r2,r2,r4 */
            r[2]=r[2]-r[4];r[15]=0x6440u;return 0;
        case 0x6440u: /* str r2,[r1,#0x4] */
            STORE(r[1]+4u,r[2],4);r[15]=0x6444u;return 0;
        case 0x6444u: /* ldrb r2,[r0,#0x24] */
            LOAD(r[2],r[0]+36u,1);r[15]=0x6448u;return 0;
        case 0x6448u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x644cu;return 0;
        case 0x644cu: /* strb r2,[r1,#0x11] */
            STORE(r[1]+17u,r[2],1);r[15]=0x6450u;return 0;
        case 0x6450u: /* ldrb r2,[r0,#0x26] */
            LOAD(r[2],r[0]+38u,1);r[15]=0x6454u;return 0;
        case 0x6454u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x6458u;return 0;
        case 0x6458u: /* strb r2,[r1,#0x10] */
            STORE(r[1]+16u,r[2],1);r[15]=0x645cu;return 0;
        case 0x645cu: /* ldr r2,[r3,#0x10] */
            LOAD(r[2],r[3]+16u,4);r[15]=0x6460u;return 0;
        case 0x6460u: /* ldr r5,[r3,#0x14] */
            LOAD(r[5],r[3]+20u,4);r[15]=0x6464u;return 0;
        case 0x6464u: /* sub r2,r2,r5 */
            r[2]=r[2]-r[5];r[15]=0x6468u;return 0;
        case 0x6468u: /* str r2,[r1,#0x18] */
            STORE(r[1]+24u,r[2],4);r[15]=0x646cu;return 0;
        case 0x646cu: /* ldr r2,[r3,#0x30] */
            LOAD(r[2],r[3]+48u,4);r[15]=0x6470u;return 0;
        case 0x6470u: /* str r2,[r1,#0x14] */
            STORE(r[1]+20u,r[2],4);r[15]=0x6474u;return 0;
        case 0x6474u: /* ldr r2,[r3,#0x18] */
            LOAD(r[2],r[3]+24u,4);r[15]=0x6478u;return 0;
        case 0x6478u: /* str r2,[r1,#0xc] */
            STORE(r[1]+12u,r[2],4);r[15]=0x647cu;return 0;
        case 0x647cu: /* ldrb r2,[r0,#0x29] */
            LOAD(r[2],r[0]+41u,1);r[15]=0x6480u;return 0;
        case 0x6480u: /* strb r2,[r1,#0x1c] */
            STORE(r[1]+28u,r[2],1);r[15]=0x6484u;return 0;
        case 0x6484u: /* ldrb r5,[r0,#0x2a] */
            LOAD(r[5],r[0]+42u,1);r[15]=0x6488u;return 0;
        case 0x6488u: /* strb r5,[r1,#0x1f] */
            STORE(r[1]+31u,r[5],1);r[15]=0x648cu;return 0;
        case 0x648cu: /* orrs r5,r2,r5 */
            r[5]=r[2]|r[5];logic_flags(state,r[5],0,0);r[15]=0x6490u;return 0;
        case 0x6490u: /* ldrb r2,[r0,#0x28] */
            LOAD(r[2],r[0]+40u,1);r[15]=0x6494u;return 0;
        case 0x6494u: /* orrne r2,r2,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[2]=r[2]|128u;}r[15]=0x6498u;return 0;
        case 0x6498u: /* strb r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],1);r[15]=0x649cu;return 0;
        case 0x649cu: /* add r0,r0,#0x80 */
            r[0]=r[0]+128u;r[15]=0x64a0u;return 0;
        case 0x64a0u: /* add r1,r1,#0x20 */
            r[1]=r[1]+32u;r[15]=0x64a4u;return 0;
        case 0x64a4u: /* add r3,r3,#0x40 */
            r[3]=r[3]+64u;r[15]=0x64a8u;return 0;
        case 0x64a8u: /* subs r12,r12,#0x1 */
            r[12]=subtract(state,r[12],1u);r[15]=0x64acu;return 0;
        case 0x64acu: /* bne 0x00006414 */
            if(!(state->cpsr&0x40000000u)){r[15]=25620u;}else{r[15]=0x64b0u;}return 0;
        case 0x64b0u: /* mov r0,#0x11000 */
            r[0]=69632u;r[15]=0x64b4u;return 0;
        case 0x64b4u: /* mov r1,#0x13800 */
            r[1]=79872u;r[15]=0x64b8u;return 0;
        case 0x64b8u: /* mov r12,#0x8 */
            r[12]=8u;r[15]=0x64bcu;return 0;
        case 0x64bcu: /* ldrb r2,[r0,#0x1e] */
            LOAD(r[2],r[0]+30u,1);r[15]=0x64c0u;return 0;
        case 0x64c0u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x64c4u;return 0;
        case 0x64c4u: /* strb r2,[r1,#0x1] */
            STORE(r[1]+1u,r[2],1);r[15]=0x64c8u;return 0;
        case 0x64c8u: /* ldrb r2,[r0,#0x1f] */
            LOAD(r[2],r[0]+31u,1);r[15]=0x64ccu;return 0;
        case 0x64ccu: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x64d0u;return 0;
        case 0x64d0u: /* strb r2,[r1,#0x2] */
            STORE(r[1]+2u,r[2],1);r[15]=0x64d4u;return 0;
        case 0x64d4u: /* ldr r2,[r0,#0x44] */
            LOAD(r[2],r[0]+68u,4);r[15]=0x64d8u;return 0;
        case 0x64d8u: /* sub r2,r2,r4 */
            r[2]=r[2]-r[4];r[15]=0x64dcu;return 0;
        case 0x64dcu: /* str r2,[r1,#0x4] */
            STORE(r[1]+4u,r[2],4);r[15]=0x64e0u;return 0;
        case 0x64e0u: /* ldr r2,[r0,#0x50] */
            LOAD(r[2],r[0]+80u,4);r[15]=0x64e4u;return 0;
        case 0x64e4u: /* sub r2,r2,r4 */
            r[2]=r[2]-r[4];r[15]=0x64e8u;return 0;
        case 0x64e8u: /* str r2,[r1,#0x8] */
            STORE(r[1]+8u,r[2],4);r[15]=0x64ecu;return 0;
        case 0x64ecu: /* ldrb r2,[r0,#0x1b] */
            LOAD(r[2],r[0]+27u,1);r[15]=0x64f0u;return 0;
        case 0x64f0u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x64f4u;return 0;
        case 0x64f4u: /* strb r2,[r1,#0x11] */
            STORE(r[1]+17u,r[2],1);r[15]=0x64f8u;return 0;
        case 0x64f8u: /* ldrb r2,[r0,#0x1a] */
            LOAD(r[2],r[0]+26u,1);r[15]=0x64fcu;return 0;
        case 0x64fcu: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x6500u;return 0;
        case 0x6500u: /* strb r2,[r1,#0x10] */
            STORE(r[1]+16u,r[2],1);r[15]=0x6504u;return 0;
        case 0x6504u: /* ldrb r2,[r0,#0x0] */
            LOAD(r[2],r[0]+0u,1);r[15]=0x6508u;return 0;
        case 0x6508u: /* strb r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],1);r[15]=0x650cu;return 0;
        case 0x650cu: /* tst r2,#0x1 */
            temporary=r[2]&1u;logic_flags(state,temporary,0,1u);r[15]=0x6510u;return 0;
        case 0x6510u: /* beq 0x00006558 */
            if(state->cpsr&0x40000000u){r[15]=25944u;}else{r[15]=0x6514u;}return 0;
        case 0x6514u: /* ldr r2,[r0,#0x4] */
            LOAD(r[2],r[0]+4u,4);r[15]=0x6518u;return 0;
        case 0x6518u: /* str r2,[r1,#0x18] */
            STORE(r[1]+24u,r[2],4);r[15]=0x651cu;return 0;
        case 0x651cu: /* ldr r2,[r0,#0x38] */
            LOAD(r[2],r[0]+56u,4);r[15]=0x6520u;return 0;
        case 0x6520u: /* str r2,[r1,#0xc] */
            STORE(r[1]+12u,r[2],4);r[15]=0x6524u;return 0;
        case 0x6524u: /* ldrb r2,[r0,#0x3] */
            LOAD(r[2],r[0]+3u,1);r[15]=0x6528u;return 0;
        case 0x6528u: /* add r2,r2,#0x30 */
            r[2]=r[2]+48u;r[15]=0x652cu;return 0;
        case 0x652cu: /* mov r2,r2, lsl #0x7 */
            r[2]=(r[2]<<7);r[15]=0x6530u;return 0;
        case 0x6530u: /* mov r5,#0x800000 */
            r[5]=8388608u;r[15]=0x6534u;return 0;
        case 0x6534u: /* add r5,r5,r2 */
            r[5]=r[5]+r[2];r[15]=0x6538u;return 0;
        case 0x6538u: /* ldr r2,[r5,#0xc] */
            LOAD(r[2],r[5]+12u,4);r[15]=0x653cu;return 0;
        case 0x653cu: /* ldr r3,[r5,#0x8] */
            LOAD(r[3],r[5]+8u,4);r[15]=0x6540u;return 0;
        case 0x6540u: /* sub r3,r2,r3 */
            r[3]=r[2]-r[3];r[15]=0x6544u;return 0;
        case 0x6544u: /* ldrb r5,[r0,#0x2c] */
            LOAD(r[5],r[0]+44u,1);r[15]=0x6548u;return 0;
        case 0x6548u: /* mul r2,r3,r5 */
            r[2]=r[3]*r[5];r[15]=0x654cu;return 0;
        case 0x654cu: /* ldr r3,[r0,#0x4] */
            LOAD(r[3],r[0]+4u,4);r[15]=0x6550u;return 0;
        case 0x6550u: /* add r2,r2,r3 */
            r[2]=r[2]+r[3];r[15]=0x6554u;return 0;
        case 0x6554u: /* str r2,[r1,#0x14] */
            STORE(r[1]+20u,r[2],4);r[15]=0x6558u;return 0;
        case 0x6558u: /* add r0,r0,#0x58 */
            r[0]=r[0]+88u;r[15]=0x655cu;return 0;
        case 0x655cu: /* add r1,r1,#0x20 */
            r[1]=r[1]+32u;r[15]=0x6560u;return 0;
        case 0x6560u: /* subs r12,r12,#0x1 */
            r[12]=subtract(state,r[12],1u);r[15]=0x6564u;return 0;
        case 0x6564u: /* bne 0x000064bc */
            if(!(state->cpsr&0x40000000u)){r[15]=25788u;}else{r[15]=0x6568u;}return 0;
        case 0x6568u: /* ldr r0,[0x5860] */
            LOAD(r[0],22624u,4);r[15]=0x656cu;return 0;
        case 0x656cu: /* ldr r1,[0x585c] */
            LOAD(r[1],22620u,4);r[15]=0x6570u;return 0;
        case 0x6570u: /* mov r4,#0x8000 */
            r[4]=32768u;r[15]=0x6574u;return 0;
        case 0x6574u: /* mov r12,#0x10 */
            r[12]=16u;r[15]=0x6578u;return 0;
        case 0x6578u: /* ldrb r2,[r0,#0x1e] */
            LOAD(r[2],r[0]+30u,1);r[15]=0x657cu;return 0;
        case 0x657cu: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x6580u;return 0;
        case 0x6580u: /* strb r2,[r1,#0x1] */
            STORE(r[1]+1u,r[2],1);r[15]=0x6584u;return 0;
        case 0x6584u: /* ldrb r2,[r0,#0x1f] */
            LOAD(r[2],r[0]+31u,1);r[15]=0x6588u;return 0;
        case 0x6588u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x658cu;return 0;
        case 0x658cu: /* strb r2,[r1,#0x2] */
            STORE(r[1]+2u,r[2],1);r[15]=0x6590u;return 0;
        case 0x6590u: /* ldr r2,[r0,#0x44] */
            LOAD(r[2],r[0]+68u,4);r[15]=0x6594u;return 0;
        case 0x6594u: /* sub r2,r2,r4 */
            r[2]=r[2]-r[4];r[15]=0x6598u;return 0;
        case 0x6598u: /* str r2,[r1,#0x4] */
            STORE(r[1]+4u,r[2],4);r[15]=0x659cu;return 0;
        case 0x659cu: /* ldr r2,[r0,#0x50] */
            LOAD(r[2],r[0]+80u,4);r[15]=0x65a0u;return 0;
        case 0x65a0u: /* sub r2,r2,r4 */
            r[2]=r[2]-r[4];r[15]=0x65a4u;return 0;
        case 0x65a4u: /* str r2,[r1,#0x8] */
            STORE(r[1]+8u,r[2],4);r[15]=0x65a8u;return 0;
        case 0x65a8u: /* ldrb r2,[r0,#0x1b] */
            LOAD(r[2],r[0]+27u,1);r[15]=0x65acu;return 0;
        case 0x65acu: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x65b0u;return 0;
        case 0x65b0u: /* strb r2,[r1,#0x11] */
            STORE(r[1]+17u,r[2],1);r[15]=0x65b4u;return 0;
        case 0x65b4u: /* ldrb r2,[r0,#0x1a] */
            LOAD(r[2],r[0]+26u,1);r[15]=0x65b8u;return 0;
        case 0x65b8u: /* sub r2,r2,r4, lsr #0x8 */
            r[2]=r[2]-(r[4]>>8);r[15]=0x65bcu;return 0;
        case 0x65bcu: /* strb r2,[r1,#0x10] */
            STORE(r[1]+16u,r[2],1);r[15]=0x65c0u;return 0;
        case 0x65c0u: /* ldrb r2,[r0,#0x0] */
            LOAD(r[2],r[0]+0u,1);r[15]=0x65c4u;return 0;
        case 0x65c4u: /* strb r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],1);r[15]=0x65c8u;return 0;
        case 0x65c8u: /* tst r2,#0x1 */
            temporary=r[2]&1u;logic_flags(state,temporary,0,1u);r[15]=0x65ccu;return 0;
        case 0x65ccu: /* beq 0x00006610 */
            if(state->cpsr&0x40000000u){r[15]=26128u;}else{r[15]=0x65d0u;}return 0;
        case 0x65d0u: /* ldr r2,[r0,#0x4] */
            LOAD(r[2],r[0]+4u,4);r[15]=0x65d4u;return 0;
        case 0x65d4u: /* str r2,[r1,#0x18] */
            STORE(r[1]+24u,r[2],4);r[15]=0x65d8u;return 0;
        case 0x65d8u: /* ldr r2,[r0,#0x2c] */
            LOAD(r[2],r[0]+44u,4);r[15]=0x65dcu;return 0;
        case 0x65dcu: /* str r2,[r1,#0xc] */
            STORE(r[1]+12u,r[2],4);r[15]=0x65e0u;return 0;
        case 0x65e0u: /* ldr r2,[r0,#0x34] */
            LOAD(r[2],r[0]+52u,4);r[15]=0x65e4u;return 0;
        case 0x65e4u: /* ldrb r5,[r0,#0x27] */
            LOAD(r[5],r[0]+39u,1);r[15]=0x65e8u;return 0;
        case 0x65e8u: /* tst r5,#0x3 */
            temporary=r[5]&3u;logic_flags(state,temporary,0,3u);r[15]=0x65ecu;return 0;
        case 0x65ecu: /* moveq r2,r2, lsr #0x1 */
            if(state->cpsr&0x40000000u){r[2]=(r[2]>>1);}r[15]=0x65f0u;return 0;
        case 0x65f0u: /* beq 0x00006600 */
            if(state->cpsr&0x40000000u){r[15]=26112u;}else{r[15]=0x65f4u;}return 0;
        case 0x65f4u: /* tst r5,#0x2 */
            temporary=r[5]&2u;logic_flags(state,temporary,0,2u);r[15]=0x65f8u;return 0;
        case 0x65f8u: /* beq 0x00006600 */
            if(state->cpsr&0x40000000u){r[15]=26112u;}else{r[15]=0x65fcu;}return 0;
        case 0x65fcu: /* mov r2,r2, lsl #0x1 */
            r[2]=(r[2]<<1);r[15]=0x6600u;return 0;
        case 0x6600u: /* ldr r3,[r0,#0x2c] */
            LOAD(r[3],r[0]+44u,4);r[15]=0x6604u;return 0;
        case 0x6604u: /* ldr r5,[r0,#0x4] */
            LOAD(r[5],r[0]+4u,4);r[15]=0x6608u;return 0;
        case 0x6608u: /* mla r2,r3,r2,r5 */
            r[2]=r[3]*r[2]+r[5];r[15]=0x660cu;return 0;
        case 0x660cu: /* str r2,[r1,#0x14] */
            STORE(r[1]+20u,r[2],4);r[15]=0x6610u;return 0;
        case 0x6610u: /* add r0,r0,#0x58 */
            r[0]=r[0]+88u;r[15]=0x6614u;return 0;
        case 0x6614u: /* add r1,r1,#0x20 */
            r[1]=r[1]+32u;r[15]=0x6618u;return 0;
        case 0x6618u: /* subs r12,r12,#0x1 */
            r[12]=subtract(state,r[12],1u);r[15]=0x661cu;return 0;
        case 0x661cu: /* bne 0x00006578 */
            if(!(state->cpsr&0x40000000u)){r[15]=25976u;}else{r[15]=0x6620u;}return 0;
        case 0x6620u: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}
}

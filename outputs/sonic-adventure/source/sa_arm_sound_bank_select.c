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
int sa_arm_sound_command_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
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
int sa_arm_sound_bank_select_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r;*handled=1;
 if(driver==0){switch(r[15]){
        case 0x1360u: /* add r5,r5,r6 */
            r[5]=r[5]+r[6];r[15]=0x1364u;return 0;
        case 0x1364u: /* ldr r8,[r5,#0x10] */
            LOAD(r[8],r[5]+16u,4);r[15]=0x1368u;return 0;
        case 0x1368u: /* str r8,[0x13dc] */
            STORE(5084u,r[8],4);r[15]=0x136cu;return 0;
        case 0x136cu: /* ldrb r0,[r5,#0x1] */
            LOAD(r[0],r[5]+1u,1);r[15]=0x1370u;return 0;
        case 0x1370u: /* ldr r7,[0x1800] */
            LOAD(r[7],6144u,4);r[15]=0x1374u;return 0;
        case 0x1374u: /* add r7,r7,r0, lsl #0x3 */
            r[7]=r[7]+(r[0]<<3);r[15]=0x1378u;return 0;
        case 0x1378u: /* ldr r8,[r7,#0x0] */
            LOAD(r[8],r[7]+0u,4);r[15]=0x137cu;return 0;
        case 0x137cu: /* movs r8,r8 */
            r[8]=r[8];logic_flags(state,r[8],0,0);r[15]=0x1380u;return 0;
        case 0x1380u: /* ldreq r8,[0x1804] */
            if(state->cpsr&0x40000000u){LOAD(r[8],6148u,4);}r[15]=0x1384u;return 0;
        case 0x1384u: /* str r8,[r5,#0x10] */
            STORE(r[5]+16u,r[8],4);r[15]=0x1388u;return 0;
        case 0x1388u: /* mov r0,r1, lsr #0x8 */
            r[0]=(r[1]>>8);r[15]=0x138cu;return 0;
        case 0x138cu: /* and r0,r0,#0x7f */
            r[0]=r[0]&127u;r[15]=0x1390u;return 0;
        case 0x1390u: /* ldr r7,[r8,#0x14] */
            LOAD(r[7],r[8]+20u,4);r[15]=0x1394u;return 0;
        case 0x1394u: /* cmp r0,r7 */
            (void)subtract(state,r[0],r[7]);r[15]=0x1398u;return 0;
        case 0x1398u: /* beq 0x000013d0 */
            if(state->cpsr&0x40000000u){r[15]=5072u;}else{r[15]=0x139cu;}return 0;
        case 0x139cu: /* bcs 0x000013d0 */
            if(state->cpsr&0x20000000u){r[15]=5072u;}else{r[15]=0x13a0u;}return 0;
        case 0x13a0u: /* strb r0,[r5,#0x0] */
            STORE(r[5]+0u,r[0],1);r[15]=0x13a4u;return 0;
        case 0x13a4u: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x13a8u;return 0;
        case 0x13a8u: /* ldr r7,[r8,#0x10] */
            LOAD(r[7],r[8]+16u,4);r[15]=0x13acu;return 0;
        case 0x13acu: /* add r7,r7,r8 */
            r[7]=r[7]+r[8];r[15]=0x13b0u;return 0;
        case 0x13b0u: /* add r0,r0,r7 */
            r[0]=r[0]+r[7];r[15]=0x13b4u;return 0;
        case 0x13b4u: /* ldr r7,[r0,#0x0] */
            LOAD(r[7],r[0]+0u,4);r[15]=0x13b8u;return 0;
        case 0x13b8u: /* add r7,r7,r8 */
            r[7]=r[7]+r[8];r[15]=0x13bcu;return 0;
        case 0x13bcu: /* str r7,[r5,#0x14] */
            STORE(r[5]+20u,r[7],4);r[15]=0x13c0u;return 0;
        case 0x13c0u: /* ldr r7,[r8,#0x18] */
            LOAD(r[7],r[8]+24u,4);r[15]=0x13c4u;return 0;
        case 0x13c4u: /* add r7,r7,r8 */
            r[7]=r[7]+r[8];r[15]=0x13c8u;return 0;
        case 0x13c8u: /* str r7,[r5,#0x18] */
            STORE(r[5]+24u,r[7],4);r[15]=0x13ccu;return 0;
        case 0x13ccu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x13d0u: /* ldr r8,[0x13dc] */
            LOAD(r[8],5084u,4);r[15]=0x13d4u;return 0;
        case 0x13d4u: /* str r8,[r5,#0x10] */
            STORE(r[5]+16u,r[8],4);r[15]=0x13d8u;return 0;
        case 0x13d8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        default:*handled=0;return 0;
 }}else{switch(r[15]){
        case 0x1368u: /* add r5,r5,r6 */
            r[5]=r[5]+r[6];r[15]=0x136cu;return 0;
        case 0x136cu: /* ldr r8,[r5,#0x10] */
            LOAD(r[8],r[5]+16u,4);r[15]=0x1370u;return 0;
        case 0x1370u: /* str r8,[0x13e8] */
            STORE(5096u,r[8],4);r[15]=0x1374u;return 0;
        case 0x1374u: /* ldrb r0,[r5,#0x1] */
            LOAD(r[0],r[5]+1u,1);r[15]=0x1378u;return 0;
        case 0x1378u: /* ldr r7,[0x180c] */
            LOAD(r[7],6156u,4);r[15]=0x137cu;return 0;
        case 0x137cu: /* add r7,r7,r0, lsl #0x3 */
            r[7]=r[7]+(r[0]<<3);r[15]=0x1380u;return 0;
        case 0x1380u: /* ldr r8,[r7,#0x0] */
            LOAD(r[8],r[7]+0u,4);r[15]=0x1384u;return 0;
        case 0x1384u: /* movs r8,r8 */
            r[8]=r[8];logic_flags(state,r[8],0,0);r[15]=0x1388u;return 0;
        case 0x1388u: /* mvneq r8,#0x0 */
            if(state->cpsr&0x40000000u){r[8]=~0u;}r[15]=0x138cu;return 0;
        case 0x138cu: /* str r8,[r5,#0x10] */
            STORE(r[5]+16u,r[8],4);r[15]=0x1390u;return 0;
        case 0x1390u: /* mov r0,r1, lsr #0x8 */
            r[0]=(r[1]>>8);r[15]=0x1394u;return 0;
        case 0x1394u: /* and r0,r0,#0x7f */
            r[0]=r[0]&127u;r[15]=0x1398u;return 0;
        case 0x1398u: /* ldr r7,[r8,#0x14] */
            LOAD(r[7],r[8]+20u,4);r[15]=0x139cu;return 0;
        case 0x139cu: /* cmp r0,r7 */
            (void)subtract(state,r[0],r[7]);r[15]=0x13a0u;return 0;
        case 0x13a0u: /* beq 0x000013d8 */
            if(state->cpsr&0x40000000u){r[15]=5080u;}else{r[15]=0x13a4u;}return 0;
        case 0x13a4u: /* bcs 0x000013d8 */
            if(state->cpsr&0x20000000u){r[15]=5080u;}else{r[15]=0x13a8u;}return 0;
        case 0x13a8u: /* strb r0,[r5,#0x0] */
            STORE(r[5]+0u,r[0],1);r[15]=0x13acu;return 0;
        case 0x13acu: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x13b0u;return 0;
        case 0x13b0u: /* ldr r7,[r8,#0x10] */
            LOAD(r[7],r[8]+16u,4);r[15]=0x13b4u;return 0;
        case 0x13b4u: /* add r7,r7,r8 */
            r[7]=r[7]+r[8];r[15]=0x13b8u;return 0;
        case 0x13b8u: /* add r0,r0,r7 */
            r[0]=r[0]+r[7];r[15]=0x13bcu;return 0;
        case 0x13bcu: /* ldr r7,[r0,#0x0] */
            LOAD(r[7],r[0]+0u,4);r[15]=0x13c0u;return 0;
        case 0x13c0u: /* add r7,r7,r8 */
            r[7]=r[7]+r[8];r[15]=0x13c4u;return 0;
        case 0x13c4u: /* str r7,[r5,#0x14] */
            STORE(r[5]+20u,r[7],4);r[15]=0x13c8u;return 0;
        case 0x13c8u: /* ldr r7,[r8,#0x18] */
            LOAD(r[7],r[8]+24u,4);r[15]=0x13ccu;return 0;
        case 0x13ccu: /* add r7,r7,r8 */
            r[7]=r[7]+r[8];r[15]=0x13d0u;return 0;
        case 0x13d0u: /* str r7,[r5,#0x18] */
            STORE(r[5]+24u,r[7],4);r[15]=0x13d4u;return 0;
        case 0x13d4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x13d8u: /* mvn r0,#0x0 */
            r[0]=~0u;r[15]=0x13dcu;return 0;
        case 0x13dcu: /* strb r0,[r5,#0x0] */
            STORE(r[5]+0u,r[0],1);r[15]=0x13e0u;return 0;
        case 0x13e0u: /* str r0,[r5,#0x10] */
            STORE(r[5]+16u,r[0],4);r[15]=0x13e4u;return 0;
        case 0x13e4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        default:*handled=0;return 0;
 }}
}

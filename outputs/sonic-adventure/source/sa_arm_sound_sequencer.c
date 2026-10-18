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
int sa_arm_sound_sequence_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
        case 0x4210u: /* ldr r9,[0x48c4] */
            LOAD(r[9],18628u,4);r[15]=0x4214u;return 0;
        case 0x4214u: /* ldrb r0,[r9,#0x1] */
            LOAD(r[0],r[9]+1u,1);r[15]=0x4218u;return 0;
        case 0x4218u: /* cmp r0,#0x1 */
            (void)subtract(state,r[0],1u);r[15]=0x421cu;return 0;
        case 0x421cu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x4220u;}return 0;
        case 0x4220u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x4224u;return 0;
        case 0x4224u: /* ldr r0,[0x48cc] */
            LOAD(r[0],18636u,4);r[15]=0x4228u;return 0;
        case 0x4228u: /* mov r1,#0x30 */
            r[1]=48u;r[15]=0x422cu;return 0;
        case 0x422cu: /* mov r8,#0x13000 */
            r[8]=77824u;r[15]=0x4230u;return 0;
        case 0x4230u: /* mov r7,#0xb000 */
            r[7]=45056u;r[15]=0x4234u;return 0;
        case 0x4234u: /* ldr r6,[r8,#0x0] */
            LOAD(r[6],r[8]+0u,4);r[15]=0x4238u;return 0;
        case 0x4238u: /* ldr r4,[0x48d4] */
            LOAD(r[4],18644u,4);r[15]=0x423cu;return 0;
        case 0x423cu: /* ldr r4,[r4,#0x0] */
            LOAD(r[4],r[4]+0u,4);r[15]=0x4240u;return 0;
        case 0x4240u: /* sub r4,r4,#0x4 */
            r[4]=r[4]-4u;r[15]=0x4244u;return 0;
        case 0x4244u: /* ldr r5,[r0,#0x0] */
            LOAD(r[5],r[0]+0u,4);r[15]=0x4248u;return 0;
        case 0x4248u: /* cmp r5,#0x0 */
            (void)subtract(state,r[5],0u);r[15]=0x424cu;return 0;
        case 0x424cu: /* beq 0x000042ac */
            if(state->cpsr&0x40000000u){r[15]=17068u;}else{r[15]=0x4250u;}return 0;
        case 0x4250u: /* and r2,r5,#0xf00000 */
            r[2]=r[5]&15728640u;r[15]=0x4254u;return 0;
        case 0x4254u: /* mov r2,r2, lsr #0x14 */
            r[2]=(r[2]>>20);r[15]=0x4258u;return 0;
        case 0x4258u: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x425cu;return 0;
        case 0x425cu: /* mov r3,#0x40 */
            r[3]=64u;r[15]=0x4260u;return 0;
        case 0x4260u: /* mla r11,r2,r3,r11 */
            r[11]=r[2]*r[3]+r[11];r[15]=0x4264u;return 0;
        case 0x4264u: /* ldrb r2,[r11,#0x1] */
            LOAD(r[2],r[11]+1u,1);r[15]=0x4268u;return 0;
        case 0x4268u: /* tst r2,#0x2 */
            temporary=r[2]&2u;logic_flags(state,temporary,0,2u);r[15]=0x426cu;return 0;
        case 0x426cu: /* bne 0x000042ac */
            if(!(state->cpsr&0x40000000u)){r[15]=17068u;}else{r[15]=0x4270u;}return 0;
        case 0x4270u: /* ldr r3,[r11,#0x3c] */
            LOAD(r[3],r[11]+60u,4);r[15]=0x4274u;return 0;
        case 0x4274u: /* ldr r2,[r0,#0x8] */
            LOAD(r[2],r[0]+8u,4);r[15]=0x4278u;return 0;
        case 0x4278u: /* subs r2,r2,r3 */
            r[2]=subtract(state,r[2],r[3]);r[15]=0x427cu;return 0;
        case 0x427cu: /* ble 0x000042bc */
            if((state->cpsr&0x40000000u)||(((state->cpsr>>31)^(state->cpsr>>28))&1u)!=0){r[15]=17084u;}else{r[15]=0x4280u;}return 0;
        case 0x4280u: /* tst r5,#0x1000000 */
            temporary=r[5]&16777216u;logic_flags(state,temporary,1,16777216u);r[15]=0x4284u;return 0;
        case 0x4284u: /* beq 0x0000429c */
            if(state->cpsr&0x40000000u){r[15]=17052u;}else{r[15]=0x4288u;}return 0;
        case 0x4288u: /* bl 0x000048a0 */
            r[14]=0x428cu;r[15]=18592u;return 0;
        case 0x428cu: /* b 0x000042e0 */
            r[15]=17120u;return 0;
        case 0x4290u: /* ldr r5,[r0,#0x0] */
            LOAD(r[5],r[0]+0u,4);r[15]=0x4294u;return 0;
        case 0x4294u: /* bic r5,r5,#0x1000000 */
            r[5]=r[5]&~16777216u;r[15]=0x4298u;return 0;
        case 0x4298u: /* str r5,[r0,#0x0] */
            STORE(r[0]+0u,r[5],4);r[15]=0x429cu;return 0;
        case 0x429cu: /* str r2,[r0,#0x8] */
            STORE(r[0]+8u,r[2],4);r[15]=0x42a0u;return 0;
        case 0x42a0u: /* ldr r2,[r0,#0x4] */
            LOAD(r[2],r[0]+4u,4);r[15]=0x42a4u;return 0;
        case 0x42a4u: /* add r2,r2,#0x1 */
            r[2]=r[2]+1u;r[15]=0x42a8u;return 0;
        case 0x42a8u: /* str r2,[r0,#0x4] */
            STORE(r[0]+4u,r[2],4);r[15]=0x42acu;return 0;
        case 0x42acu: /* add r0,r0,#0xc */
            r[0]=r[0]+12u;r[15]=0x42b0u;return 0;
        case 0x42b0u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x42b4u;return 0;
        case 0x42b4u: /* bne 0x00004244 */
            if(!(state->cpsr&0x40000000u)){r[15]=16964u;}else{r[15]=0x42b8u;}return 0;
        case 0x42b8u: /* b 0x00004310 */
            r[15]=17168u;return 0;
        case 0x42bcu: /* tst r5,#0x1000000 */
            temporary=r[5]&16777216u;logic_flags(state,temporary,1,16777216u);r[15]=0x42c0u;return 0;
        case 0x42c0u: /* bne 0x000042cc */
            if(!(state->cpsr&0x40000000u)){r[15]=17100u;}else{r[15]=0x42c4u;}return 0;
        case 0x42c4u: /* bl 0x000048a0 */
            r[14]=0x42c8u;r[15]=18592u;return 0;
        case 0x42c8u: /* b 0x000042f0 */
            r[15]=17136u;return 0;
        case 0x42ccu: /* mov r5,#0x0 */
            r[5]=0u;r[15]=0x42d0u;return 0;
        case 0x42d0u: /* str r5,[r0,#0x0] */
            STORE(r[0]+0u,r[5],4);r[15]=0x42d4u;return 0;
        case 0x42d4u: /* str r5,[r0,#0x8] */
            STORE(r[0]+8u,r[5],4);r[15]=0x42d8u;return 0;
        case 0x42d8u: /* str r5,[r0,#0x4] */
            STORE(r[0]+4u,r[5],4);r[15]=0x42dcu;return 0;
        case 0x42dcu: /* b 0x000042ac */
            r[15]=17068u;return 0;
        case 0x42e0u: /* mov r5,#0x0 */
            r[5]=0u;r[15]=0x42e4u;return 0;
        case 0x42e4u: /* str r5,[r0,#0x0] */
            STORE(r[0]+0u,r[5],4);r[15]=0x42e8u;return 0;
        case 0x42e8u: /* str r5,[r0,#0x8] */
            STORE(r[0]+8u,r[5],4);r[15]=0x42ecu;return 0;
        case 0x42ecu: /* str r5,[r0,#0x4] */
            STORE(r[0]+4u,r[5],4);r[15]=0x42f0u;return 0;
        case 0x42f0u: /* add r0,r0,#0xc */
            r[0]=r[0]+12u;r[15]=0x42f4u;return 0;
        case 0x42f4u: /* cmp r0,r1 */
            (void)subtract(state,r[0],r[1]);r[15]=0x42f8u;return 0;
        case 0x42f8u: /* movcs r0,#0x20 */
            if(state->cpsr&0x20000000u){r[0]=32u;}r[15]=0x42fcu;return 0;
        case 0x42fcu: /* bcs 0x000043d8 */
            if(state->cpsr&0x20000000u){r[15]=17368u;}else{r[15]=0x4300u;}return 0;
        case 0x4300u: /* ldr r5,[r0,#0x0] */
            LOAD(r[5],r[0]+0u,4);r[15]=0x4304u;return 0;
        case 0x4304u: /* tst r5,#0x1000000 */
            temporary=r[5]&16777216u;logic_flags(state,temporary,1,16777216u);r[15]=0x4308u;return 0;
        case 0x4308u: /* beq 0x000042f0 */
            if(state->cpsr&0x40000000u){r[15]=17136u;}else{r[15]=0x430cu;}return 0;
        case 0x430cu: /* b 0x000042e0 */
            r[15]=17120u;return 0;
        case 0x4310u: /* ldr r11,[r9,#0x4] */
            LOAD(r[11],r[9]+4u,4);r[15]=0x4314u;return 0;
        case 0x4314u: /* ldrb r10,[r9,#0x2] */
            LOAD(r[10],r[9]+2u,1);r[15]=0x4318u;return 0;
        case 0x4318u: /* ldrb r0,[r11,#0x1] */
            LOAD(r[0],r[11]+1u,1);r[15]=0x431cu;return 0;
        case 0x431cu: /* cmp r0,#0x1 */
            (void)subtract(state,r[0],1u);r[15]=0x4320u;return 0;
        case 0x4320u: /* bne 0x000043ac */
            if(!(state->cpsr&0x40000000u)){r[15]=17324u;}else{r[15]=0x4324u;}return 0;
        case 0x4324u: /* ldr r12,[r11,#0x10] */
            LOAD(r[12],r[11]+16u,4);r[15]=0x4328u;return 0;
        case 0x4328u: /* ldr r1,[r11,#0x3c] */
            LOAD(r[1],r[11]+60u,4);r[15]=0x432cu;return 0;
        case 0x432cu: /* ldr r0,[r11,#0x38] */
            LOAD(r[0],r[11]+56u,4);r[15]=0x4330u;return 0;
        case 0x4330u: /* subs r0,r0,r1, lsr #0xc */
            r[0]=subtract(state,r[0],(r[1]>>12));r[15]=0x4334u;return 0;
        case 0x4334u: /* bhi 0x00004350 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=17232u;}else{r[15]=0x4338u;}return 0;
        case 0x4338u: /* ldr r2,[r11,#0x30] */
            LOAD(r[2],r[11]+48u,4);r[15]=0x433cu;return 0;
        case 0x433cu: /* ldr r3,[r11,#0x34] */
            LOAD(r[3],r[11]+52u,4);r[15]=0x4340u;return 0;
        case 0x4340u: /* add r2,r2,#0x1 */
            r[2]=r[2]+1u;r[15]=0x4344u;return 0;
        case 0x4344u: /* adds r0,r0,r3 */
            r[0]=addition(state,r[0],r[3]);r[15]=0x4348u;return 0;
        case 0x4348u: /* bmi 0x00004340 */
            if(state->cpsr&0x80000000u){r[15]=17216u;}else{r[15]=0x434cu;}return 0;
        case 0x434cu: /* str r2,[r11,#0x30] */
            STORE(r[11]+48u,r[2],4);r[15]=0x4350u;return 0;
        case 0x4350u: /* str r0,[r11,#0x38] */
            STORE(r[11]+56u,r[0],4);r[15]=0x4354u;return 0;
        case 0x4354u: /* ldr r0,[r11,#0x24] */
            LOAD(r[0],r[11]+36u,4);r[15]=0x4358u;return 0;
        case 0x4358u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x435cu;return 0;
        case 0x435cu: /* str r0,[r11,#0x24] */
            STORE(r[11]+36u,r[0],4);r[15]=0x4360u;return 0;
        case 0x4360u: /* bhi 0x000043ac */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=17324u;}else{r[15]=0x4364u;}return 0;
        case 0x4364u: /* ldrb r0,[r11,#0x2] */
            LOAD(r[0],r[11]+2u,1);r[15]=0x4368u;return 0;
        case 0x4368u: /* cmp r0,#0x0 */
            (void)subtract(state,r[0],0u);r[15]=0x436cu;return 0;
        case 0x436cu: /* beq 0x00004380 */
            if(state->cpsr&0x40000000u){r[15]=17280u;}else{r[15]=0x4370u;}return 0;
        case 0x4370u: /* cmp r0,#0x1 */
            (void)subtract(state,r[0],1u);r[15]=0x4374u;return 0;
        case 0x4374u: /* ldreq r12,[r11,#0x1c] */
            if(state->cpsr&0x40000000u){LOAD(r[12],r[11]+28u,4);}r[15]=0x4378u;return 0;
        case 0x4378u: /* sub r0,r0,#0x1 */
            r[0]=r[0]-1u;r[15]=0x437cu;return 0;
        case 0x437cu: /* strb r0,[r11,#0x2] */
            STORE(r[11]+2u,r[0],1);r[15]=0x4380u;return 0;
        case 0x4380u: /* ldr r0,[r11,#0x4] */
            LOAD(r[0],r[11]+4u,4);r[15]=0x4384u;return 0;
        case 0x4384u: /* cmp r12,r0 */
            (void)subtract(state,r[12],r[0]);r[15]=0x4388u;return 0;
        case 0x4388u: /* bcs 0x00004764 */
            if(state->cpsr&0x20000000u){r[15]=18276u;}else{r[15]=0x438cu;}return 0;
        case 0x438cu: /* str r12,[r11,#0x10] */
            STORE(r[11]+16u,r[12],4);r[15]=0x4390u;return 0;
        case 0x4390u: /* ldrb r0,[r12],#0x1 */
            LOAD(r[0],r[12],1);r[12]+=1u;r[15]=0x4394u;return 0;
        case 0x4394u: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x4398u;return 0;
        case 0x4398u: /* beq 0x000043ec */
            if(state->cpsr&0x40000000u){r[15]=17388u;}else{r[15]=0x439cu;}return 0;
        case 0x439cu: /* subs r1,r0,#0x90 */
            r[1]=subtract(state,r[0],144u);r[15]=0x43a0u;return 0;
        case 0x43a0u: /* bmi 0x000045d8 */
            if(state->cpsr&0x80000000u){r[15]=17880u;}else{r[15]=0x43a4u;}return 0;
        case 0x43a4u: /* mov r1,r1, lsr #0x4 */
            r[1]=(r[1]>>4);r[15]=0x43a8u;return 0;
        case 0x43a8u: /* b 0x00004548 */
            r[15]=17736u;return 0;
        case 0x43acu: /* cmp r10,#0x7 */
            (void)subtract(state,r[10],7u);r[15]=0x43b0u;return 0;
        case 0x43b0u: /* beq 0x000043c0 */
            if(state->cpsr&0x40000000u){r[15]=17344u;}else{r[15]=0x43b4u;}return 0;
        case 0x43b4u: /* add r10,r10,#0x1 */
            r[10]=r[10]+1u;r[15]=0x43b8u;return 0;
        case 0x43b8u: /* add r11,r11,#0x40 */
            r[11]=r[11]+64u;r[15]=0x43bcu;return 0;
        case 0x43bcu: /* b 0x00004318 */
            r[15]=17176u;return 0;
        case 0x43c0u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x43c4u;return 0;
        case 0x43c4u: /* strb r0,[r9,#0x2] */
            STORE(r[9]+2u,r[0],1);r[15]=0x43c8u;return 0;
        case 0x43c8u: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x43ccu;return 0;
        case 0x43ccu: /* str r11,[r9,#0x4] */
            STORE(r[9]+4u,r[11],4);r[15]=0x43d0u;return 0;
        case 0x43d0u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x43d4u;return 0;
        case 0x43d4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x43d8u: /* ldr r1,[r9,#0x0] */
            LOAD(r[1],r[9]+0u,4);r[15]=0x43dcu;return 0;
        case 0x43dcu: /* orr r0,r1,r0 */
            r[0]=r[1]|r[0];r[15]=0x43e0u;return 0;
        case 0x43e0u: /* strb r0,[r9,#0x0] */
            STORE(r[9]+0u,r[0],1);r[15]=0x43e4u;return 0;
        case 0x43e4u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x43e8u;return 0;
        case 0x43e8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x43ecu: /* ldrb r2,[r11,#0x0] */
            LOAD(r[2],r[11]+0u,1);r[15]=0x43f0u;return 0;
        case 0x43f0u: /* mov r1,#0x1 */
            r[1]=1u;r[15]=0x43f4u;return 0;
        case 0x43f4u: /* orr r2,r1,r2 */
            r[2]=r[1]|r[2];r[15]=0x43f8u;return 0;
        case 0x43f8u: /* orr r2,r10,r2, lsl #0x4 */
            r[2]=r[10]|(r[2]<<4);r[15]=0x43fcu;return 0;
        case 0x43fcu: /* bic r1,r0,#0xf0 */
            r[1]=r[0]&~240u;r[15]=0x4400u;return 0;
        case 0x4400u: /* orr r2,r1,r2, lsl #0x4 */
            r[2]=r[1]|(r[2]<<4);r[15]=0x4404u;return 0;
        case 0x4404u: /* ldrb r1,[r12],#0x1 */
            LOAD(r[1],r[12],1);r[12]+=1u;r[15]=0x4408u;return 0;
        case 0x4408u: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x440cu;return 0;
        case 0x440cu: /* orr r3,r3,r1, lsl #0x8 */
            r[3]=r[3]|(r[1]<<8);r[15]=0x4410u;return 0;
        case 0x4410u: /* orr r2,r3,r2, lsl #0x10 */
            r[2]=r[3]|(r[2]<<16);r[15]=0x4414u;return 0;
        case 0x4414u: /* mov r1,r0, lsr #0x5 */
            r[1]=(r[0]>>5);r[15]=0x4418u;return 0;
        case 0x4418u: /* mov r4,#0x0 */
            r[4]=0u;r[15]=0x441cu;return 0;
        case 0x441cu: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x4420u;return 0;
        case 0x4420u: /* orr r4,r4,r3 */
            r[4]=r[4]|r[3];r[15]=0x4424u;return 0;
        case 0x4424u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x4428u;return 0;
        case 0x4428u: /* movpl r4,r4, lsl #0x8 */
            if(!(state->cpsr&0x80000000u)){r[4]=(r[4]<<8);}r[15]=0x442cu;return 0;
        case 0x442cu: /* bpl 0x0000441c */
            if(!(state->cpsr&0x80000000u)){r[15]=17436u;}else{r[15]=0x4430u;}return 0;
        case 0x4430u: /* ldr r1,[r11,#0xc] */
            LOAD(r[1],r[11]+12u,4);r[15]=0x4434u;return 0;
        case 0x4434u: /* ldr r3,[r11,#0x2c] */
            LOAD(r[3],r[11]+44u,4);r[15]=0x4438u;return 0;
        case 0x4438u: /* mlas r4,r1,r4,r3; C follows TI925T reference, not ARMv4 hardware proof */
            r[4]=r[1]*r[4]+r[3];logic_flags(state,r[4],0,0);r[15]=0x443cu;return 0;
        case 0x443cu: /* mvnvs r4,#0x0 */
            if(state->cpsr&0x10000000u)r[4]=~0u;r[15]=0x4440u;return 0;
        case 0x4440u: /* ldr r1,[r11,#0x24] */
            LOAD(r[1],r[11]+36u,4);r[15]=0x4444u;return 0;
        case 0x4444u: /* add r4,r4,r1 */
            r[4]=r[4]+r[1];r[15]=0x4448u;return 0;
        case 0x4448u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x444cu;return 0;
        case 0x444cu: /* str r1,[r11,#0x2c] */
            STORE(r[11]+44u,r[1],4);r[15]=0x4450u;return 0;
        case 0x4450u: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x4454u;return 0;
        case 0x4454u: /* tst r0,#0x10 */
            temporary=r[0]&16u;logic_flags(state,temporary,0,16u);r[15]=0x4458u;return 0;
        case 0x4458u: /* ldrbne r1,[r12],#0x1 */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],r[12],1);r[12]+=1u;}r[15]=0x445cu;return 0;
        case 0x445cu: /* orrne r3,r1,r3, lsl #0x8 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[1]|(r[3]<<8);}r[15]=0x4460u;return 0;
        case 0x4460u: /* ldr r0,[0x48cc] */
            LOAD(r[0],18636u,4);r[15]=0x4464u;return 0;
        case 0x4464u: /* add r5,r0,#0x240 */
            r[5]=r[0]+576u;r[15]=0x4468u;return 0;
        case 0x4468u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x446cu;return 0;
        case 0x446cu: /* cmp r1,#0x0 */
            (void)subtract(state,r[1],0u);r[15]=0x4470u;return 0;
        case 0x4470u: /* beq 0x00004484 */
            if(state->cpsr&0x40000000u){r[15]=17540u;}else{r[15]=0x4474u;}return 0;
        case 0x4474u: /* add r0,r0,#0xc */
            r[0]=r[0]+12u;r[15]=0x4478u;return 0;
        case 0x4478u: /* cmp r5,r0 */
            (void)subtract(state,r[5],r[0]);r[15]=0x447cu;return 0;
        case 0x447cu: /* beq 0x0000449c */
            if(state->cpsr&0x40000000u){r[15]=17564u;}else{r[15]=0x4480u;}return 0;
        case 0x4480u: /* b 0x00004468 */
            r[15]=17512u;return 0;
        case 0x4484u: /* bic r1,r2,#0x1000000 */
            r[1]=r[2]&~16777216u;r[15]=0x4488u;return 0;
        case 0x4488u: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x448cu;return 0;
        case 0x448cu: /* str r4,[r0,#0x8] */
            STORE(r[0]+8u,r[4],4);r[15]=0x4490u;return 0;
        case 0x4490u: /* mov r6,#0x1 */
            r[6]=1u;r[15]=0x4494u;return 0;
        case 0x4494u: /* str r6,[r0,#0x4] */
            STORE(r[0]+4u,r[6],4);r[15]=0x4498u;return 0;
        case 0x4498u: /* b 0x000047b4 */
            r[15]=18356u;return 0;
        case 0x449cu: /* ldr r0,[r9,#0x0] */
            LOAD(r[0],r[9]+0u,4);r[15]=0x44a0u;return 0;
        case 0x44a0u: /* orr r0,r0,#0x80 */
            r[0]=r[0]|128u;r[15]=0x44a4u;return 0;
        case 0x44a4u: /* strb r0,[r9,#0x0] */
            STORE(r[9]+0u,r[0],1);r[15]=0x44a8u;return 0;
        case 0x44a8u: /* stmdb sp!,{r10} */
            STORE(r[13]-4u,r[10],4);r[13]-=4u;r[15]=0x44acu;return 0;
        case 0x44acu: /* ldr r0,[0x48cc] */
            LOAD(r[0],18636u,4);r[15]=0x44b0u;return 0;
        case 0x44b0u: /* ldr r10,[0x48cc] */
            LOAD(r[10],18636u,4);r[15]=0x44b4u;return 0;
        case 0x44b4u: /* mov r7,#0x1f */
            r[7]=31u;r[15]=0x44b8u;return 0;
        case 0x44b8u: /* mov r8,#0x0 */
            r[8]=0u;r[15]=0x44bcu;return 0;
        case 0x44bcu: /* ldr r6,[r0,#0x0] */
            LOAD(r[6],r[0]+0u,4);r[15]=0x44c0u;return 0;
        case 0x44c0u: /* mov r6,r6, lsr #0x1b */
            r[6]=(r[6]>>27);r[15]=0x44c4u;return 0;
        case 0x44c4u: /* cmp r6,#0x0 */
            (void)subtract(state,r[6],0u);r[15]=0x44c8u;return 0;
        case 0x44c8u: /* beq 0x000044e8 */
            if(state->cpsr&0x40000000u){r[15]=17640u;}else{r[15]=0x44ccu;}return 0;
        case 0x44ccu: /* cmp r6,r7 */
            (void)subtract(state,r[6],r[7]);r[15]=0x44d0u;return 0;
        case 0x44d0u: /* beq 0x000044fc */
            if(state->cpsr&0x40000000u){r[15]=17660u;}else{r[15]=0x44d4u;}return 0;
        case 0x44d4u: /* bcc 0x000044e8 */
            if(!(state->cpsr&0x20000000u)){r[15]=17640u;}else{r[15]=0x44d8u;}return 0;
        case 0x44d8u: /* add r0,r0,#0xc */
            r[0]=r[0]+12u;r[15]=0x44dcu;return 0;
        case 0x44dcu: /* cmp r0,r5 */
            (void)subtract(state,r[0],r[5]);r[15]=0x44e0u;return 0;
        case 0x44e0u: /* bcs 0x00004510 */
            if(state->cpsr&0x20000000u){r[15]=17680u;}else{r[15]=0x44e4u;}return 0;
        case 0x44e4u: /* b 0x000044bc */
            r[15]=17596u;return 0;
        case 0x44e8u: /* mov r10,r0 */
            r[10]=r[0];r[15]=0x44ecu;return 0;
        case 0x44ecu: /* beq 0x00004510 */
            if(state->cpsr&0x40000000u){r[15]=17680u;}else{r[15]=0x44f0u;}return 0;
        case 0x44f0u: /* mov r7,r6 */
            r[7]=r[6];r[15]=0x44f4u;return 0;
        case 0x44f4u: /* ldr r8,[r0,#0x4] */
            LOAD(r[8],r[0]+4u,4);r[15]=0x44f8u;return 0;
        case 0x44f8u: /* b 0x000044d8 */
            r[15]=17624u;return 0;
        case 0x44fcu: /* ldr r6,[r0,#0x4] */
            LOAD(r[6],r[0]+4u,4);r[15]=0x4500u;return 0;
        case 0x4500u: /* cmp r6,r8 */
            (void)subtract(state,r[6],r[8]);r[15]=0x4504u;return 0;
        case 0x4504u: /* movhi r8,r6 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[8]=r[6];}r[15]=0x4508u;return 0;
        case 0x4508u: /* mov r10,r0 */
            r[10]=r[0];r[15]=0x450cu;return 0;
        case 0x450cu: /* b 0x000044d8 */
            r[15]=17624u;return 0;
        case 0x4510u: /* ldr r5,[0x48cc] */
            LOAD(r[5],18636u,4);r[15]=0x4514u;return 0;
        case 0x4514u: /* ldr r1,[r9,#0x8] */
            LOAD(r[1],r[9]+8u,4);r[15]=0x4518u;return 0;
        case 0x4518u: /* cmp r1,r5 */
            (void)subtract(state,r[1],r[5]);r[15]=0x451cu;return 0;
        case 0x451cu: /* ldrcs r1,[0x48c8] */
            if(state->cpsr&0x20000000u){LOAD(r[1],18632u,4);}r[15]=0x4520u;return 0;
        case 0x4520u: /* ldr r8,[r1,#0x0] */
            LOAD(r[8],r[1]+0u,4);r[15]=0x4524u;return 0;
        case 0x4524u: /* cmp r8,#0x0 */
            (void)subtract(state,r[8],0u);r[15]=0x4528u;return 0;
        case 0x4528u: /* bne 0x00004864 */
            if(!(state->cpsr&0x40000000u)){r[15]=18532u;}else{r[15]=0x452cu;}return 0;
        case 0x452cu: /* mov r0,r10 */
            r[0]=r[10];r[15]=0x4530u;return 0;
        case 0x4530u: /* ldr r6,[r0,#0x0] */
            LOAD(r[6],r[0]+0u,4);r[15]=0x4534u;return 0;
        case 0x4534u: /* bic r6,r6,#0x1000000 */
            r[6]=r[6]&~16777216u;r[15]=0x4538u;return 0;
        case 0x4538u: /* str r6,[r1],#0x4 */
            STORE(r[1],r[6],4);r[1]+=4u;r[15]=0x453cu;return 0;
        case 0x453cu: /* str r1,[r9,#0x8] */
            STORE(r[9]+8u,r[1],4);r[15]=0x4540u;return 0;
        case 0x4540u: /* ldmia sp!,{r10} */
            LOAD(r[10],r[13]+0u,4);r[13]+=4u;r[15]=0x4544u;return 0;
        case 0x4544u: /* b 0x00004484 */
            r[15]=17540u;return 0;
        case 0x4548u: /* and r5,r0,#0x70 */
            r[5]=r[0]&112u;r[15]=0x454cu;return 0;
        case 0x454cu: /* and r0,r0,#0xf */
            r[0]=r[0]&15u;r[15]=0x4550u;return 0;
        case 0x4550u: /* ldrb r2,[r11,#0x0] */
            LOAD(r[2],r[11]+0u,1);r[15]=0x4554u;return 0;
        case 0x4554u: /* orr r2,r2,r5, lsr #0x4 */
            r[2]=r[2]|(r[5]>>4);r[15]=0x4558u;return 0;
        case 0x4558u: /* orr r2,r10,r2, lsl #0x4 */
            r[2]=r[10]|(r[2]<<4);r[15]=0x455cu;return 0;
        case 0x455cu: /* orr r2,r0,r2, lsl #0x4 */
            r[2]=r[0]|(r[2]<<4);r[15]=0x4560u;return 0;
        case 0x4560u: /* ldr r3,[0x48d8] */
            LOAD(r[3],18648u,4);r[15]=0x4564u;return 0;
        case 0x4564u: /* ldr r3,[r3,r1,lsl #0x2] */
            LOAD(r[3],r[3]+(r[1]<<2),4);r[15]=0x4568u;return 0;
        case 0x4568u: /* mov pc,r3 */
            r[15]=r[3];return 0;
        case 0x4588u: /* ldrb r4,[r12],#0x1 */
            LOAD(r[4],r[12],1);r[12]+=1u;r[15]=0x458cu;return 0;
        case 0x458cu: /* bic r1,r4,#0x80 */
            r[1]=r[4]&~128u;r[15]=0x4590u;return 0;
        case 0x4590u: /* orr r2,r1,r2, lsl #0x8 */
            r[2]=r[1]|(r[2]<<8);r[15]=0x4594u;return 0;
        case 0x4594u: /* ldrb r1,[r12],#0x1 */
            LOAD(r[1],r[12],1);r[12]+=1u;r[15]=0x4598u;return 0;
        case 0x4598u: /* orr r2,r1,r2, lsl #0x8 */
            r[2]=r[1]|(r[2]<<8);r[15]=0x459cu;return 0;
        case 0x459cu: /* b 0x000045b0 */
            r[15]=17840u;return 0;
        case 0x45a0u: /* ldrb r4,[r12],#0x1 */
            LOAD(r[4],r[12],1);r[12]+=1u;r[15]=0x45a4u;return 0;
        case 0x45a4u: /* bic r1,r4,#0x80 */
            r[1]=r[4]&~128u;r[15]=0x45a8u;return 0;
        case 0x45a8u: /* orr r2,r1,r2, lsl #0x8 */
            r[2]=r[1]|(r[2]<<8);r[15]=0x45acu;return 0;
        case 0x45acu: /* mov r2,r2, lsl #0x8 */
            r[2]=(r[2]<<8);r[15]=0x45b0u;return 0;
        case 0x45b0u: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x45b4u;return 0;
        case 0x45b4u: /* tst r4,#0x80 */
            temporary=r[4]&128u;logic_flags(state,temporary,0,128u);r[15]=0x45b8u;return 0;
        case 0x45b8u: /* ldrbne r4,[r12],#0x1 */
            if(!(state->cpsr&0x40000000u)){LOAD(r[4],r[12],1);r[12]+=1u;}r[15]=0x45bcu;return 0;
        case 0x45bcu: /* orrne r3,r4,r3, lsl #0x8 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[4]|(r[3]<<8);}r[15]=0x45c0u;return 0;
        case 0x45c0u: /* b 0x000047b4 */
            r[15]=18356u;return 0;
        case 0x45c4u: /* mov r2,r2, lsl #0x8 */
            r[2]=(r[2]<<8);r[15]=0x45c8u;return 0;
        case 0x45c8u: /* ldrb r1,[r12],#0x1 */
            LOAD(r[1],r[12],1);r[12]+=1u;r[15]=0x45ccu;return 0;
        case 0x45ccu: /* orr r2,r1,r2, lsl #0x8 */
            r[2]=r[1]|(r[2]<<8);r[15]=0x45d0u;return 0;
        case 0x45d0u: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x45d4u;return 0;
        case 0x45d4u: /* b 0x000047b4 */
            r[15]=18356u;return 0;
        case 0x45d8u: /* ldr r2,[0x48dc] */
            LOAD(r[2],18652u,4);r[15]=0x45dcu;return 0;
        case 0x45dcu: /* sub r1,r0,#0x80 */
            r[1]=r[0]-128u;r[15]=0x45e0u;return 0;
        case 0x45e0u: /* mov r3,#0x4 */
            r[3]=4u;r[15]=0x45e4u;return 0;
        case 0x45e4u: /* mul r3,r1,r3 */
            r[3]=r[1]*r[3];r[15]=0x45e8u;return 0;
        case 0x45e8u: /* ldr r3,[r2,r3] */
            LOAD(r[3],r[2]+r[3],4);r[15]=0x45ecu;return 0;
        case 0x45ecu: /* mov pc,r3 */
            r[15]=r[3];return 0;
        case 0x4630u: /* ldrb r1,[r12],#0x1 */
            LOAD(r[1],r[12],1);r[12]+=1u;r[15]=0x4634u;return 0;
        case 0x4634u: /* ldrb r2,[r12],#0x1 */
            LOAD(r[2],r[12],1);r[12]+=1u;r[15]=0x4638u;return 0;
        case 0x4638u: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x463cu;return 0;
        case 0x463cu: /* str r12,[r11,#0x1c] */
            STORE(r[11]+28u,r[12],4);r[15]=0x4640u;return 0;
        case 0x4640u: /* strb r3,[r11,#0x2] */
            STORE(r[11]+2u,r[3],1);r[15]=0x4644u;return 0;
        case 0x4644u: /* orr r1,r2,r1, lsl #0x8 */
            r[1]=r[2]|(r[1]<<8);r[15]=0x4648u;return 0;
        case 0x4648u: /* ldr r4,[r11,#0x14] */
            LOAD(r[4],r[11]+20u,4);r[15]=0x464cu;return 0;
        case 0x464cu: /* add r12,r4,r1 */
            r[12]=r[4]+r[1];r[15]=0x4650u;return 0;
        case 0x4650u: /* b 0x00004380 */
            r[15]=17280u;return 0;
        case 0x4654u: /* ldr r0,[r11,#0x20] */
            LOAD(r[0],r[11]+32u,4);r[15]=0x4658u;return 0;
        case 0x4658u: /* cmp r0,#0x0 */
            (void)subtract(state,r[0],0u);r[15]=0x465cu;return 0;
        case 0x465cu: /* bne 0x0000468c */
            if(!(state->cpsr&0x40000000u)){r[15]=18060u;}else{r[15]=0x4660u;}return 0;
        case 0x4660u: /* str r12,[r11,#0x20] */
            STORE(r[11]+32u,r[12],4);r[15]=0x4664u;return 0;
        case 0x4664u: /* str r0,[r11,#0x18] */
            STORE(r[11]+24u,r[0],4);r[15]=0x4668u;return 0;
        case 0x4668u: /* ldrb r0,[r12],#0x1 */
            LOAD(r[0],r[12],1);r[12]+=1u;r[15]=0x466cu;return 0;
        case 0x466cu: /* ands r1,r0,#0x7f */
            temporary=r[0]&127u;r[1]=temporary;logic_flags(state,temporary,0,127u);r[15]=0x4670u;return 0;
        case 0x4670u: /* moveq r1,#0x80 */
            if(state->cpsr&0x40000000u){r[1]=128u;}r[15]=0x4674u;return 0;
        case 0x4674u: /* strb r1,[r11,#0x3] */
            STORE(r[11]+3u,r[1],1);r[15]=0x4678u;return 0;
        case 0x4678u: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x467cu;return 0;
        case 0x467cu: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x4680u;return 0;
        case 0x4680u: /* ldrbne r1,[r12],#0x1 */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],r[12],1);r[12]+=1u;}r[15]=0x4684u;return 0;
        case 0x4684u: /* orrne r3,r1,r3, lsl #0x8 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[1]|(r[3]<<8);}r[15]=0x4688u;return 0;
        case 0x4688u: /* b 0x00004814 */
            r[15]=18452u;return 0;
        case 0x468cu: /* ldrb r1,[r11,#0x3] */
            LOAD(r[1],r[11]+3u,1);r[15]=0x4690u;return 0;
        case 0x4690u: /* cmp r1,#0x0 */
            (void)subtract(state,r[1],0u);r[15]=0x4694u;return 0;
        case 0x4694u: /* beq 0x000046c0 */
            if(state->cpsr&0x40000000u){r[15]=18112u;}else{r[15]=0x4698u;}return 0;
        case 0x4698u: /* ldr r12,[r11,#0x20] */
            LOAD(r[12],r[11]+32u,4);r[15]=0x469cu;return 0;
        case 0x469cu: /* ldrb r0,[r12],#0x1 */
            LOAD(r[0],r[12],1);r[12]+=1u;r[15]=0x46a0u;return 0;
        case 0x46a0u: /* ldr r2,[r11,#0x18] */
            LOAD(r[2],r[11]+24u,4);r[15]=0x46a4u;return 0;
        case 0x46a4u: /* add r2,r2,#0x1 */
            r[2]=r[2]+1u;r[15]=0x46a8u;return 0;
        case 0x46a8u: /* str r2,[r11,#0x18] */
            STORE(r[11]+24u,r[2],4);r[15]=0x46acu;return 0;
        case 0x46acu: /* cmp r1,#0x80 */
            (void)subtract(state,r[1],128u);r[15]=0x46b0u;return 0;
        case 0x46b0u: /* beq 0x00004678 */
            if(state->cpsr&0x40000000u){r[15]=18040u;}else{r[15]=0x46b4u;}return 0;
        case 0x46b4u: /* sub r1,r1,#0x1 */
            r[1]=r[1]-1u;r[15]=0x46b8u;return 0;
        case 0x46b8u: /* strb r1,[r11,#0x3] */
            STORE(r[11]+3u,r[1],1);r[15]=0x46bcu;return 0;
        case 0x46bcu: /* b 0x00004678 */
            r[15]=18040u;return 0;
        case 0x46c0u: /* str r1,[r11,#0x20] */
            STORE(r[11]+32u,r[1],4);r[15]=0x46c4u;return 0;
        case 0x46c4u: /* ldrb r0,[r12],#0x1 */
            LOAD(r[0],r[12],1);r[12]+=1u;r[15]=0x46c8u;return 0;
        case 0x46c8u: /* b 0x00004678 */
            r[15]=18040u;return 0;
        case 0x46ccu: /* ldrb r0,[r12],#0x1 */
            LOAD(r[0],r[12],1);r[12]+=1u;r[15]=0x46d0u;return 0;
        case 0x46d0u: /* ldrb r1,[r12],#0x1 */
            LOAD(r[1],r[12],1);r[12]+=1u;r[15]=0x46d4u;return 0;
        case 0x46d4u: /* orr r0,r1,r0, lsl #0x8 */
            r[0]=r[1]|(r[0]<<8);r[15]=0x46d8u;return 0;
        case 0x46d8u: /* str r0,[r11,#0x34] */
            STORE(r[11]+52u,r[0],4);r[15]=0x46dcu;return 0;
        case 0x46dcu: /* ldr r1,[r11,#0x8] */
            LOAD(r[1],r[11]+8u,4);r[15]=0x46e0u;return 0;
        case 0x46e0u: /* mul r0,r1,r0 */
            r[0]=r[1]*r[0];r[15]=0x46e4u;return 0;
        case 0x46e4u: /* mov r0,r0, lsr #0x4 */
            r[0]=(r[0]>>4);r[15]=0x46e8u;return 0;
        case 0x46e8u: /* str r0,[r11,#0xc] */
            STORE(r[11]+12u,r[0],4);r[15]=0x46ecu;return 0;
        case 0x46ecu: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x46f0u;return 0;
        case 0x46f0u: /* b 0x00004814 */
            r[15]=18452u;return 0;
        case 0x46f4u: /* and r0,r0,#0x3 */
            r[0]=r[0]&3u;r[15]=0x46f8u;return 0;
        case 0x46f8u: /* ldr r1,[0x48e0] */
            LOAD(r[1],18656u,4);r[15]=0x46fcu;return 0;
        case 0x46fcu: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x4700u;return 0;
        case 0x4700u: /* ldr r1,[r1,r0] */
            LOAD(r[1],r[1]+r[0],4);r[15]=0x4704u;return 0;
        case 0x4704u: /* ldr r0,[r11,#0xc] */
            LOAD(r[0],r[11]+12u,4);r[15]=0x4708u;return 0;
        case 0x4708u: /* ldr r2,[r11,#0x2c] */
            LOAD(r[2],r[11]+44u,4);r[15]=0x470cu;return 0;
        case 0x470cu: /* mlas r1,r0,r1,r2; C follows TI925T reference, not ARMv4 hardware proof */
            r[1]=r[0]*r[1]+r[2];logic_flags(state,r[1],0,0);r[15]=0x4710u;return 0;
        case 0x4710u: /* mvnvs r1,#0x0 */
            if(state->cpsr&0x10000000u)r[1]=~0u;r[15]=0x4714u;return 0;
        case 0x4714u: /* str r1,[r11,#0x2c] */
            STORE(r[11]+44u,r[1],4);r[15]=0x4718u;return 0;
        case 0x4718u: /* b 0x00004380 */
            r[15]=17280u;return 0;
        case 0x471cu: /* and r0,r0,#0x3 */
            r[0]=r[0]&3u;r[15]=0x4720u;return 0;
        case 0x4720u: /* ldr r1,[0x48e4] */
            LOAD(r[1],18660u,4);r[15]=0x4724u;return 0;
        case 0x4724u: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x4728u;return 0;
        case 0x4728u: /* ldr r1,[r1,r0] */
            LOAD(r[1],r[1]+r[0],4);r[15]=0x472cu;return 0;
        case 0x472cu: /* ldr r0,[r11,#0xc] */
            LOAD(r[0],r[11]+12u,4);r[15]=0x4730u;return 0;
        case 0x4730u: /* ldr r2,[r11,#0x28] */
            LOAD(r[2],r[11]+40u,4);r[15]=0x4734u;return 0;
        case 0x4734u: /* mlas r1,r0,r1,r2; C follows TI925T reference, not ARMv4 hardware proof */
            r[1]=r[0]*r[1]+r[2];logic_flags(state,r[1],0,0);r[15]=0x4738u;return 0;
        case 0x4738u: /* mvnvs r1,#0x0 */
            if(state->cpsr&0x10000000u)r[1]=~0u;r[15]=0x473cu;return 0;
        case 0x473cu: /* str r1,[r11,#0x28] */
            STORE(r[11]+40u,r[1],4);r[15]=0x4740u;return 0;
        case 0x4740u: /* b 0x00004380 */
            r[15]=17280u;return 0;
        case 0x4764u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x4768u;return 0;
        case 0x4768u: /* str r0,[r11,#0x0] */
            STORE(r[11]+0u,r[0],4);r[15]=0x476cu;return 0;
        case 0x476cu: /* str r0,[r11,#0x4] */
            STORE(r[11]+4u,r[0],4);r[15]=0x4770u;return 0;
        case 0x4770u: /* str r0,[r11,#0x8] */
            STORE(r[11]+8u,r[0],4);r[15]=0x4774u;return 0;
        case 0x4774u: /* str r0,[r11,#0xc] */
            STORE(r[11]+12u,r[0],4);r[15]=0x4778u;return 0;
        case 0x4778u: /* strb r0,[r11,#0x3] */
            STORE(r[11]+3u,r[0],1);r[15]=0x477cu;return 0;
        case 0x477cu: /* str r0,[r11,#0x18] */
            STORE(r[11]+24u,r[0],4);r[15]=0x4780u;return 0;
        case 0x4780u: /* str r0,[r11,#0x1c] */
            STORE(r[11]+28u,r[0],4);r[15]=0x4784u;return 0;
        case 0x4784u: /* str r0,[r11,#0x20] */
            STORE(r[11]+32u,r[0],4);r[15]=0x4788u;return 0;
        case 0x4788u: /* str r0,[r11,#0x24] */
            STORE(r[11]+36u,r[0],4);r[15]=0x478cu;return 0;
        case 0x478cu: /* str r0,[r11,#0x28] */
            STORE(r[11]+40u,r[0],4);r[15]=0x4790u;return 0;
        case 0x4790u: /* str r0,[r11,#0x2c] */
            STORE(r[11]+44u,r[0],4);r[15]=0x4794u;return 0;
        case 0x4794u: /* str r0,[r11,#0x30] */
            STORE(r[11]+48u,r[0],4);r[15]=0x4798u;return 0;
        case 0x4798u: /* str r0,[r11,#0x34] */
            STORE(r[11]+52u,r[0],4);r[15]=0x479cu;return 0;
        case 0x479cu: /* str r0,[r11,#0x38] */
            STORE(r[11]+56u,r[0],4);r[15]=0x47a0u;return 0;
        case 0x47a0u: /* mov r0,r10 */
            r[0]=r[10];r[15]=0x47a4u;return 0;
        case 0x47a4u: /* stmdb sp!,{r1,r2,lr} */
            STORE(r[13]-12u,r[1],4);STORE(r[13]-8u,r[2],4);STORE(r[13]-4u,r[14],4);r[13]-=12u;r[15]=0x47a8u;return 0;
        case 0x47a8u: /* bl 0x000063dc */
            r[14]=0x47acu;r[15]=25564u;return 0;
        case 0x47acu: /* ldmia sp!,{r1,r2,lr} */
            LOAD(r[1],r[13]+0u,4);LOAD(r[2],r[13]+4u,4);LOAD(r[14],r[13]+8u,4);r[13]+=12u;r[15]=0x47b0u;return 0;
        case 0x47b0u: /* b 0x000043ac */
            r[15]=17324u;return 0;
        case 0x47b4u: /* ldr r0,[r9,#0xc] */
            LOAD(r[0],r[9]+12u,4);r[15]=0x47b8u;return 0;
        case 0x47b8u: /* ldr r1,[0x48cc] */
            LOAD(r[1],18636u,4);r[15]=0x47bcu;return 0;
        case 0x47bcu: /* mov r8,#0x13000 */
            r[8]=77824u;r[15]=0x47c0u;return 0;
        case 0x47c0u: /* mov r7,#0xb000 */
            r[7]=45056u;r[15]=0x47c4u;return 0;
        case 0x47c4u: /* ldr r4,[0x48d4] */
            LOAD(r[4],18644u,4);r[15]=0x47c8u;return 0;
        case 0x47c8u: /* ldr r4,[r4,#0x0] */
            LOAD(r[4],r[4]+0u,4);r[15]=0x47ccu;return 0;
        case 0x47ccu: /* sub r4,r4,#0x4 */
            r[4]=r[4]-4u;r[15]=0x47d0u;return 0;
        case 0x47d0u: /* cmp r0,r1 */
            (void)subtract(state,r[0],r[1]);r[15]=0x47d4u;return 0;
        case 0x47d4u: /* ldrhi r0,[0x48c8] */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){LOAD(r[0],18632u,4);}r[15]=0x47d8u;return 0;
        case 0x47d8u: /* ldr r5,[r0,#0x0] */
            LOAD(r[5],r[0]+0u,4);r[15]=0x47dcu;return 0;
        case 0x47dcu: /* cmp r5,#0x0 */
            (void)subtract(state,r[5],0u);r[15]=0x47e0u;return 0;
        case 0x47e0u: /* bne 0x000047f0 */
            if(!(state->cpsr&0x40000000u)){r[15]=18416u;}else{r[15]=0x47e4u;}return 0;
        case 0x47e4u: /* str r0,[r9,#0xc] */
            STORE(r[9]+12u,r[0],4);r[15]=0x47e8u;return 0;
        case 0x47e8u: /* mov r5,r2 */
            r[5]=r[2];r[15]=0x47ecu;return 0;
        case 0x47ecu: /* mov r2,#0x0 */
            r[2]=0u;r[15]=0x47f0u;return 0;
        case 0x47f0u: /* ldr r6,[r8,#0x0] */
            LOAD(r[6],r[8]+0u,4);r[15]=0x47f4u;return 0;
        case 0x47f4u: /* bl 0x000048a0 */
            r[14]=0x47f8u;r[15]=18592u;return 0;
        case 0x47f8u: /* b 0x0000482c */
            r[15]=18476u;return 0;
        case 0x47fcu: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x4800u;return 0;
        case 0x4800u: /* beq 0x00004814 */
            if(state->cpsr&0x40000000u){r[15]=18452u;}else{r[15]=0x4804u;}return 0;
        case 0x4804u: /* mov r5,#0x0 */
            r[5]=0u;r[15]=0x4808u;return 0;
        case 0x4808u: /* str r5,[r0,#0x0] */
            STORE(r[0]+0u,r[5],4);r[15]=0x480cu;return 0;
        case 0x480cu: /* add r0,r0,#0x4 */
            r[0]=r[0]+4u;r[15]=0x4810u;return 0;
        case 0x4810u: /* b 0x000047d0 */
            r[15]=18384u;return 0;
        case 0x4814u: /* cmp r3,#0x0 */
            (void)subtract(state,r[3],0u);r[15]=0x4818u;return 0;
        case 0x4818u: /* ble 0x00004364 */
            if((state->cpsr&0x40000000u)||(((state->cpsr>>31)^(state->cpsr>>28))&1u)!=0){r[15]=17252u;}else{r[15]=0x481cu;}return 0;
        case 0x481cu: /* bl 0x00004874 */
            r[14]=0x4820u;r[15]=18548u;return 0;
        case 0x4820u: /* cmp r3,#0x0 */
            (void)subtract(state,r[3],0u);r[15]=0x4824u;return 0;
        case 0x4824u: /* ble 0x00004364 */
            if((state->cpsr&0x40000000u)||(((state->cpsr>>31)^(state->cpsr>>28))&1u)!=0){r[15]=17252u;}else{r[15]=0x4828u;}return 0;
        case 0x4828u: /* b 0x000043ac */
            r[15]=17324u;return 0;
        case 0x482cu: /* ldr r0,[r9,#0x8] */
            LOAD(r[0],r[9]+8u,4);r[15]=0x4830u;return 0;
        case 0x4830u: /* cmp r0,r1 */
            (void)subtract(state,r[0],r[1]);r[15]=0x4834u;return 0;
        case 0x4834u: /* ldrhi r0,[0x48c8] */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){LOAD(r[0],18632u,4);}r[15]=0x4838u;return 0;
        case 0x4838u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x483cu;return 0;
        case 0x483cu: /* cmp r1,#0x0 */
            (void)subtract(state,r[1],0u);r[15]=0x4840u;return 0;
        case 0x4840u: /* bne 0x00004864 */
            if(!(state->cpsr&0x40000000u)){r[15]=18532u;}else{r[15]=0x4844u;}return 0;
        case 0x4844u: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x4848u;return 0;
        case 0x4848u: /* strne r2,[r0],#0x4 */
            if(!(state->cpsr&0x40000000u)){STORE(r[0],r[2],4);r[0]+=4u;}r[15]=0x484cu;return 0;
        case 0x484cu: /* str r0,[r9,#0x8] */
            STORE(r[9]+8u,r[0],4);r[15]=0x4850u;return 0;
        case 0x4850u: /* bl 0x00004874 */
            r[14]=0x4854u;r[15]=18548u;return 0;
        case 0x4854u: /* strb r10,[r9,#0x2] */
            STORE(r[9]+2u,r[10],1);r[15]=0x4858u;return 0;
        case 0x4858u: /* str r11,[r9,#0x4] */
            STORE(r[9]+4u,r[11],4);r[15]=0x485cu;return 0;
        case 0x485cu: /* mov r0,#0x20 */
            r[0]=32u;r[15]=0x4860u;return 0;
        case 0x4860u: /* b 0x000043d8 */
            r[15]=17368u;return 0;
        case 0x4864u: /* mov r0,#0x1 */
            r[0]=1u;r[15]=0x4868u;return 0;
        case 0x4868u: /* strb r0,[r9,#0x1] */
            STORE(r[9]+1u,r[0],1);r[15]=0x486cu;return 0;
        case 0x486cu: /* mov r0,#0x60 */
            r[0]=96u;r[15]=0x4870u;return 0;
        case 0x4870u: /* b 0x000043d8 */
            r[15]=17368u;return 0;
        case 0x4874u: /* ldr r0,[r11,#0xc] */
            LOAD(r[0],r[11]+12u,4);r[15]=0x4878u;return 0;
        case 0x4878u: /* ldr r4,[r11,#0x28] */
            LOAD(r[4],r[11]+40u,4);r[15]=0x487cu;return 0;
        case 0x487cu: /* mlas r3,r0,r3,r4; C follows TI925T reference, not ARMv4 hardware proof */
            r[3]=r[0]*r[3]+r[4];logic_flags(state,r[3],0,0);r[15]=0x4880u;return 0;
        case 0x4880u: /* mvnvs r3,#0x0 */
            if(state->cpsr&0x10000000u)r[3]=~0u;r[15]=0x4884u;return 0;
        case 0x4884u: /* mov r4,#0x0 */
            r[4]=0u;r[15]=0x4888u;return 0;
        case 0x4888u: /* str r4,[r11,#0x28] */
            STORE(r[11]+40u,r[4],4);r[15]=0x488cu;return 0;
        case 0x488cu: /* ldr r0,[r11,#0x24] */
            LOAD(r[0],r[11]+36u,4);r[15]=0x4890u;return 0;
        case 0x4890u: /* add r3,r3,r0 */
            r[3]=r[3]+r[0];r[15]=0x4894u;return 0;
        case 0x4894u: /* str r3,[r11,#0x24] */
            STORE(r[11]+36u,r[3],4);r[15]=0x4898u;return 0;
        case 0x4898u: /* str r12,[r11,#0x10] */
            STORE(r[11]+16u,r[12],4);r[15]=0x489cu;return 0;
        case 0x489cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x48a0u: /* cmp r4,r6 */
            (void)subtract(state,r[4],r[6]);r[15]=0x48a4u;return 0;
        case 0x48a4u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x48a8u;}return 0;
        case 0x48a8u: /* str r5,[r7,r6] */
            STORE(r[7]+r[6],r[5],4);r[15]=0x48acu;return 0;
        case 0x48acu: /* ldr r5,[0x48d0] */
            LOAD(r[5],18640u,4);r[15]=0x48b0u;return 0;
        case 0x48b0u: /* add r6,r6,#0x4 */
            r[6]=r[6]+4u;r[15]=0x48b4u;return 0;
        case 0x48b4u: /* and r6,r6,r5 */
            r[6]=r[6]&r[5];r[15]=0x48b8u;return 0;
        case 0x48b8u: /* str r6,[r8,#0x0] */
            STORE(r[8]+0u,r[6],4);r[15]=0x48bcu;return 0;
        case 0x48bcu: /* add lr,lr,#0x4 */
            r[14]=r[14]+4u;r[15]=0x48c0u;return 0;
        case 0x48c0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x63dcu: /* mov r1,#0xc000 */
            r[1]=49152u;r[15]=0x63e0u;return 0;
        case 0x63e0u: /* mov r2,#0x80 */
            r[2]=128u;r[15]=0x63e4u;return 0;
        case 0x63e4u: /* mla r1,r2,r0,r1 */
            r[1]=r[2]*r[0]+r[1];r[15]=0x63e8u;return 0;
        case 0x63e8u: /* ldrb r0,[r1,#0x28] */
            LOAD(r[0],r[1]+40u,1);r[15]=0x63ecu;return 0;
        case 0x63ecu: /* bic r0,r0,#0x1 */
            r[0]=r[0]&~1u;r[15]=0x63f0u;return 0;
        case 0x63f0u: /* strb r0,[r1,#0x28] */
            STORE(r[1]+40u,r[0],1);r[15]=0x63f4u;return 0;
        case 0x63f4u: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}else{switch(r[15]){
        case 0x4218u: /* ldr r9,[0x48cc] */
            LOAD(r[9],18636u,4);r[15]=0x421cu;return 0;
        case 0x421cu: /* ldrb r0,[r9,#0x1] */
            LOAD(r[0],r[9]+1u,1);r[15]=0x4220u;return 0;
        case 0x4220u: /* cmp r0,#0x1 */
            (void)subtract(state,r[0],1u);r[15]=0x4224u;return 0;
        case 0x4224u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x4228u;}return 0;
        case 0x4228u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x422cu;return 0;
        case 0x422cu: /* ldr r0,[0x48d4] */
            LOAD(r[0],18644u,4);r[15]=0x4230u;return 0;
        case 0x4230u: /* mov r1,#0x30 */
            r[1]=48u;r[15]=0x4234u;return 0;
        case 0x4234u: /* mov r8,#0x13000 */
            r[8]=77824u;r[15]=0x4238u;return 0;
        case 0x4238u: /* mov r7,#0xb000 */
            r[7]=45056u;r[15]=0x423cu;return 0;
        case 0x423cu: /* ldr r6,[r8,#0x0] */
            LOAD(r[6],r[8]+0u,4);r[15]=0x4240u;return 0;
        case 0x4240u: /* ldr r4,[0x48dc] */
            LOAD(r[4],18652u,4);r[15]=0x4244u;return 0;
        case 0x4244u: /* ldr r4,[r4,#0x0] */
            LOAD(r[4],r[4]+0u,4);r[15]=0x4248u;return 0;
        case 0x4248u: /* sub r4,r4,#0x4 */
            r[4]=r[4]-4u;r[15]=0x424cu;return 0;
        case 0x424cu: /* ldr r5,[r0,#0x0] */
            LOAD(r[5],r[0]+0u,4);r[15]=0x4250u;return 0;
        case 0x4250u: /* cmp r5,#0x0 */
            (void)subtract(state,r[5],0u);r[15]=0x4254u;return 0;
        case 0x4254u: /* beq 0x000042b4 */
            if(state->cpsr&0x40000000u){r[15]=17076u;}else{r[15]=0x4258u;}return 0;
        case 0x4258u: /* and r2,r5,#0xf00000 */
            r[2]=r[5]&15728640u;r[15]=0x425cu;return 0;
        case 0x425cu: /* mov r2,r2, lsr #0x14 */
            r[2]=(r[2]>>20);r[15]=0x4260u;return 0;
        case 0x4260u: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x4264u;return 0;
        case 0x4264u: /* mov r3,#0x40 */
            r[3]=64u;r[15]=0x4268u;return 0;
        case 0x4268u: /* mla r11,r2,r3,r11 */
            r[11]=r[2]*r[3]+r[11];r[15]=0x426cu;return 0;
        case 0x426cu: /* ldrb r2,[r11,#0x1] */
            LOAD(r[2],r[11]+1u,1);r[15]=0x4270u;return 0;
        case 0x4270u: /* tst r2,#0x2 */
            temporary=r[2]&2u;logic_flags(state,temporary,0,2u);r[15]=0x4274u;return 0;
        case 0x4274u: /* bne 0x000042b4 */
            if(!(state->cpsr&0x40000000u)){r[15]=17076u;}else{r[15]=0x4278u;}return 0;
        case 0x4278u: /* ldr r3,[r11,#0x3c] */
            LOAD(r[3],r[11]+60u,4);r[15]=0x427cu;return 0;
        case 0x427cu: /* ldr r2,[r0,#0x8] */
            LOAD(r[2],r[0]+8u,4);r[15]=0x4280u;return 0;
        case 0x4280u: /* subs r2,r2,r3 */
            r[2]=subtract(state,r[2],r[3]);r[15]=0x4284u;return 0;
        case 0x4284u: /* ble 0x000042c4 */
            if((state->cpsr&0x40000000u)||(((state->cpsr>>31)^(state->cpsr>>28))&1u)!=0){r[15]=17092u;}else{r[15]=0x4288u;}return 0;
        case 0x4288u: /* tst r5,#0x1000000 */
            temporary=r[5]&16777216u;logic_flags(state,temporary,1,16777216u);r[15]=0x428cu;return 0;
        case 0x428cu: /* beq 0x000042a4 */
            if(state->cpsr&0x40000000u){r[15]=17060u;}else{r[15]=0x4290u;}return 0;
        case 0x4290u: /* bl 0x000048a8 */
            r[14]=0x4294u;r[15]=18600u;return 0;
        case 0x4294u: /* b 0x000042e8 */
            r[15]=17128u;return 0;
        case 0x4298u: /* ldr r5,[r0,#0x0] */
            LOAD(r[5],r[0]+0u,4);r[15]=0x429cu;return 0;
        case 0x429cu: /* bic r5,r5,#0x1000000 */
            r[5]=r[5]&~16777216u;r[15]=0x42a0u;return 0;
        case 0x42a0u: /* str r5,[r0,#0x0] */
            STORE(r[0]+0u,r[5],4);r[15]=0x42a4u;return 0;
        case 0x42a4u: /* str r2,[r0,#0x8] */
            STORE(r[0]+8u,r[2],4);r[15]=0x42a8u;return 0;
        case 0x42a8u: /* ldr r2,[r0,#0x4] */
            LOAD(r[2],r[0]+4u,4);r[15]=0x42acu;return 0;
        case 0x42acu: /* add r2,r2,#0x1 */
            r[2]=r[2]+1u;r[15]=0x42b0u;return 0;
        case 0x42b0u: /* str r2,[r0,#0x4] */
            STORE(r[0]+4u,r[2],4);r[15]=0x42b4u;return 0;
        case 0x42b4u: /* add r0,r0,#0xc */
            r[0]=r[0]+12u;r[15]=0x42b8u;return 0;
        case 0x42b8u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x42bcu;return 0;
        case 0x42bcu: /* bne 0x0000424c */
            if(!(state->cpsr&0x40000000u)){r[15]=16972u;}else{r[15]=0x42c0u;}return 0;
        case 0x42c0u: /* b 0x00004318 */
            r[15]=17176u;return 0;
        case 0x42c4u: /* tst r5,#0x1000000 */
            temporary=r[5]&16777216u;logic_flags(state,temporary,1,16777216u);r[15]=0x42c8u;return 0;
        case 0x42c8u: /* bne 0x000042d4 */
            if(!(state->cpsr&0x40000000u)){r[15]=17108u;}else{r[15]=0x42ccu;}return 0;
        case 0x42ccu: /* bl 0x000048a8 */
            r[14]=0x42d0u;r[15]=18600u;return 0;
        case 0x42d0u: /* b 0x000042f8 */
            r[15]=17144u;return 0;
        case 0x42d4u: /* mov r5,#0x0 */
            r[5]=0u;r[15]=0x42d8u;return 0;
        case 0x42d8u: /* str r5,[r0,#0x0] */
            STORE(r[0]+0u,r[5],4);r[15]=0x42dcu;return 0;
        case 0x42dcu: /* str r5,[r0,#0x8] */
            STORE(r[0]+8u,r[5],4);r[15]=0x42e0u;return 0;
        case 0x42e0u: /* str r5,[r0,#0x4] */
            STORE(r[0]+4u,r[5],4);r[15]=0x42e4u;return 0;
        case 0x42e4u: /* b 0x000042b4 */
            r[15]=17076u;return 0;
        case 0x42e8u: /* mov r5,#0x0 */
            r[5]=0u;r[15]=0x42ecu;return 0;
        case 0x42ecu: /* str r5,[r0,#0x0] */
            STORE(r[0]+0u,r[5],4);r[15]=0x42f0u;return 0;
        case 0x42f0u: /* str r5,[r0,#0x8] */
            STORE(r[0]+8u,r[5],4);r[15]=0x42f4u;return 0;
        case 0x42f4u: /* str r5,[r0,#0x4] */
            STORE(r[0]+4u,r[5],4);r[15]=0x42f8u;return 0;
        case 0x42f8u: /* add r0,r0,#0xc */
            r[0]=r[0]+12u;r[15]=0x42fcu;return 0;
        case 0x42fcu: /* cmp r0,r1 */
            (void)subtract(state,r[0],r[1]);r[15]=0x4300u;return 0;
        case 0x4300u: /* movcs r0,#0x20 */
            if(state->cpsr&0x20000000u){r[0]=32u;}r[15]=0x4304u;return 0;
        case 0x4304u: /* bcs 0x000043e0 */
            if(state->cpsr&0x20000000u){r[15]=17376u;}else{r[15]=0x4308u;}return 0;
        case 0x4308u: /* ldr r5,[r0,#0x0] */
            LOAD(r[5],r[0]+0u,4);r[15]=0x430cu;return 0;
        case 0x430cu: /* tst r5,#0x1000000 */
            temporary=r[5]&16777216u;logic_flags(state,temporary,1,16777216u);r[15]=0x4310u;return 0;
        case 0x4310u: /* beq 0x000042f8 */
            if(state->cpsr&0x40000000u){r[15]=17144u;}else{r[15]=0x4314u;}return 0;
        case 0x4314u: /* b 0x000042e8 */
            r[15]=17128u;return 0;
        case 0x4318u: /* ldr r11,[r9,#0x4] */
            LOAD(r[11],r[9]+4u,4);r[15]=0x431cu;return 0;
        case 0x431cu: /* ldrb r10,[r9,#0x2] */
            LOAD(r[10],r[9]+2u,1);r[15]=0x4320u;return 0;
        case 0x4320u: /* ldrb r0,[r11,#0x1] */
            LOAD(r[0],r[11]+1u,1);r[15]=0x4324u;return 0;
        case 0x4324u: /* cmp r0,#0x1 */
            (void)subtract(state,r[0],1u);r[15]=0x4328u;return 0;
        case 0x4328u: /* bne 0x000043b4 */
            if(!(state->cpsr&0x40000000u)){r[15]=17332u;}else{r[15]=0x432cu;}return 0;
        case 0x432cu: /* ldr r12,[r11,#0x10] */
            LOAD(r[12],r[11]+16u,4);r[15]=0x4330u;return 0;
        case 0x4330u: /* ldr r1,[r11,#0x3c] */
            LOAD(r[1],r[11]+60u,4);r[15]=0x4334u;return 0;
        case 0x4334u: /* ldr r0,[r11,#0x38] */
            LOAD(r[0],r[11]+56u,4);r[15]=0x4338u;return 0;
        case 0x4338u: /* subs r0,r0,r1, lsr #0xc */
            r[0]=subtract(state,r[0],(r[1]>>12));r[15]=0x433cu;return 0;
        case 0x433cu: /* bhi 0x00004358 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=17240u;}else{r[15]=0x4340u;}return 0;
        case 0x4340u: /* ldr r2,[r11,#0x30] */
            LOAD(r[2],r[11]+48u,4);r[15]=0x4344u;return 0;
        case 0x4344u: /* ldr r3,[r11,#0x34] */
            LOAD(r[3],r[11]+52u,4);r[15]=0x4348u;return 0;
        case 0x4348u: /* add r2,r2,#0x1 */
            r[2]=r[2]+1u;r[15]=0x434cu;return 0;
        case 0x434cu: /* adds r0,r0,r3 */
            r[0]=addition(state,r[0],r[3]);r[15]=0x4350u;return 0;
        case 0x4350u: /* bmi 0x00004348 */
            if(state->cpsr&0x80000000u){r[15]=17224u;}else{r[15]=0x4354u;}return 0;
        case 0x4354u: /* str r2,[r11,#0x30] */
            STORE(r[11]+48u,r[2],4);r[15]=0x4358u;return 0;
        case 0x4358u: /* str r0,[r11,#0x38] */
            STORE(r[11]+56u,r[0],4);r[15]=0x435cu;return 0;
        case 0x435cu: /* ldr r0,[r11,#0x24] */
            LOAD(r[0],r[11]+36u,4);r[15]=0x4360u;return 0;
        case 0x4360u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x4364u;return 0;
        case 0x4364u: /* str r0,[r11,#0x24] */
            STORE(r[11]+36u,r[0],4);r[15]=0x4368u;return 0;
        case 0x4368u: /* bhi 0x000043b4 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=17332u;}else{r[15]=0x436cu;}return 0;
        case 0x436cu: /* ldrb r0,[r11,#0x2] */
            LOAD(r[0],r[11]+2u,1);r[15]=0x4370u;return 0;
        case 0x4370u: /* cmp r0,#0x0 */
            (void)subtract(state,r[0],0u);r[15]=0x4374u;return 0;
        case 0x4374u: /* beq 0x00004388 */
            if(state->cpsr&0x40000000u){r[15]=17288u;}else{r[15]=0x4378u;}return 0;
        case 0x4378u: /* cmp r0,#0x1 */
            (void)subtract(state,r[0],1u);r[15]=0x437cu;return 0;
        case 0x437cu: /* ldreq r12,[r11,#0x1c] */
            if(state->cpsr&0x40000000u){LOAD(r[12],r[11]+28u,4);}r[15]=0x4380u;return 0;
        case 0x4380u: /* sub r0,r0,#0x1 */
            r[0]=r[0]-1u;r[15]=0x4384u;return 0;
        case 0x4384u: /* strb r0,[r11,#0x2] */
            STORE(r[11]+2u,r[0],1);r[15]=0x4388u;return 0;
        case 0x4388u: /* ldr r0,[r11,#0x4] */
            LOAD(r[0],r[11]+4u,4);r[15]=0x438cu;return 0;
        case 0x438cu: /* cmp r12,r0 */
            (void)subtract(state,r[12],r[0]);r[15]=0x4390u;return 0;
        case 0x4390u: /* bcs 0x0000476c */
            if(state->cpsr&0x20000000u){r[15]=18284u;}else{r[15]=0x4394u;}return 0;
        case 0x4394u: /* str r12,[r11,#0x10] */
            STORE(r[11]+16u,r[12],4);r[15]=0x4398u;return 0;
        case 0x4398u: /* ldrb r0,[r12],#0x1 */
            LOAD(r[0],r[12],1);r[12]+=1u;r[15]=0x439cu;return 0;
        case 0x439cu: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x43a0u;return 0;
        case 0x43a0u: /* beq 0x000043f4 */
            if(state->cpsr&0x40000000u){r[15]=17396u;}else{r[15]=0x43a4u;}return 0;
        case 0x43a4u: /* subs r1,r0,#0x90 */
            r[1]=subtract(state,r[0],144u);r[15]=0x43a8u;return 0;
        case 0x43a8u: /* bmi 0x000045e0 */
            if(state->cpsr&0x80000000u){r[15]=17888u;}else{r[15]=0x43acu;}return 0;
        case 0x43acu: /* mov r1,r1, lsr #0x4 */
            r[1]=(r[1]>>4);r[15]=0x43b0u;return 0;
        case 0x43b0u: /* b 0x00004550 */
            r[15]=17744u;return 0;
        case 0x43b4u: /* cmp r10,#0x7 */
            (void)subtract(state,r[10],7u);r[15]=0x43b8u;return 0;
        case 0x43b8u: /* beq 0x000043c8 */
            if(state->cpsr&0x40000000u){r[15]=17352u;}else{r[15]=0x43bcu;}return 0;
        case 0x43bcu: /* add r10,r10,#0x1 */
            r[10]=r[10]+1u;r[15]=0x43c0u;return 0;
        case 0x43c0u: /* add r11,r11,#0x40 */
            r[11]=r[11]+64u;r[15]=0x43c4u;return 0;
        case 0x43c4u: /* b 0x00004320 */
            r[15]=17184u;return 0;
        case 0x43c8u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x43ccu;return 0;
        case 0x43ccu: /* strb r0,[r9,#0x2] */
            STORE(r[9]+2u,r[0],1);r[15]=0x43d0u;return 0;
        case 0x43d0u: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x43d4u;return 0;
        case 0x43d4u: /* str r11,[r9,#0x4] */
            STORE(r[9]+4u,r[11],4);r[15]=0x43d8u;return 0;
        case 0x43d8u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x43dcu;return 0;
        case 0x43dcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x43e0u: /* ldr r1,[r9,#0x0] */
            LOAD(r[1],r[9]+0u,4);r[15]=0x43e4u;return 0;
        case 0x43e4u: /* orr r0,r1,r0 */
            r[0]=r[1]|r[0];r[15]=0x43e8u;return 0;
        case 0x43e8u: /* strb r0,[r9,#0x0] */
            STORE(r[9]+0u,r[0],1);r[15]=0x43ecu;return 0;
        case 0x43ecu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x43f0u;return 0;
        case 0x43f0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x43f4u: /* ldrb r2,[r11,#0x0] */
            LOAD(r[2],r[11]+0u,1);r[15]=0x43f8u;return 0;
        case 0x43f8u: /* mov r1,#0x1 */
            r[1]=1u;r[15]=0x43fcu;return 0;
        case 0x43fcu: /* orr r2,r1,r2 */
            r[2]=r[1]|r[2];r[15]=0x4400u;return 0;
        case 0x4400u: /* orr r2,r10,r2, lsl #0x4 */
            r[2]=r[10]|(r[2]<<4);r[15]=0x4404u;return 0;
        case 0x4404u: /* bic r1,r0,#0xf0 */
            r[1]=r[0]&~240u;r[15]=0x4408u;return 0;
        case 0x4408u: /* orr r2,r1,r2, lsl #0x4 */
            r[2]=r[1]|(r[2]<<4);r[15]=0x440cu;return 0;
        case 0x440cu: /* ldrb r1,[r12],#0x1 */
            LOAD(r[1],r[12],1);r[12]+=1u;r[15]=0x4410u;return 0;
        case 0x4410u: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x4414u;return 0;
        case 0x4414u: /* orr r3,r3,r1, lsl #0x8 */
            r[3]=r[3]|(r[1]<<8);r[15]=0x4418u;return 0;
        case 0x4418u: /* orr r2,r3,r2, lsl #0x10 */
            r[2]=r[3]|(r[2]<<16);r[15]=0x441cu;return 0;
        case 0x441cu: /* mov r1,r0, lsr #0x5 */
            r[1]=(r[0]>>5);r[15]=0x4420u;return 0;
        case 0x4420u: /* mov r4,#0x0 */
            r[4]=0u;r[15]=0x4424u;return 0;
        case 0x4424u: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x4428u;return 0;
        case 0x4428u: /* orr r4,r4,r3 */
            r[4]=r[4]|r[3];r[15]=0x442cu;return 0;
        case 0x442cu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x4430u;return 0;
        case 0x4430u: /* movpl r4,r4, lsl #0x8 */
            if(!(state->cpsr&0x80000000u)){r[4]=(r[4]<<8);}r[15]=0x4434u;return 0;
        case 0x4434u: /* bpl 0x00004424 */
            if(!(state->cpsr&0x80000000u)){r[15]=17444u;}else{r[15]=0x4438u;}return 0;
        case 0x4438u: /* ldr r1,[r11,#0xc] */
            LOAD(r[1],r[11]+12u,4);r[15]=0x443cu;return 0;
        case 0x443cu: /* ldr r3,[r11,#0x2c] */
            LOAD(r[3],r[11]+44u,4);r[15]=0x4440u;return 0;
        case 0x4440u: /* mlas r4,r1,r4,r3; C follows TI925T reference, not ARMv4 hardware proof */
            r[4]=r[1]*r[4]+r[3];logic_flags(state,r[4],0,0);r[15]=0x4444u;return 0;
        case 0x4444u: /* mvnvs r4,#0x0 */
            if(state->cpsr&0x10000000u)r[4]=~0u;r[15]=0x4448u;return 0;
        case 0x4448u: /* ldr r1,[r11,#0x24] */
            LOAD(r[1],r[11]+36u,4);r[15]=0x444cu;return 0;
        case 0x444cu: /* add r4,r4,r1 */
            r[4]=r[4]+r[1];r[15]=0x4450u;return 0;
        case 0x4450u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x4454u;return 0;
        case 0x4454u: /* str r1,[r11,#0x2c] */
            STORE(r[11]+44u,r[1],4);r[15]=0x4458u;return 0;
        case 0x4458u: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x445cu;return 0;
        case 0x445cu: /* tst r0,#0x10 */
            temporary=r[0]&16u;logic_flags(state,temporary,0,16u);r[15]=0x4460u;return 0;
        case 0x4460u: /* ldrbne r1,[r12],#0x1 */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],r[12],1);r[12]+=1u;}r[15]=0x4464u;return 0;
        case 0x4464u: /* orrne r3,r1,r3, lsl #0x8 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[1]|(r[3]<<8);}r[15]=0x4468u;return 0;
        case 0x4468u: /* ldr r0,[0x48d4] */
            LOAD(r[0],18644u,4);r[15]=0x446cu;return 0;
        case 0x446cu: /* add r5,r0,#0x240 */
            r[5]=r[0]+576u;r[15]=0x4470u;return 0;
        case 0x4470u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x4474u;return 0;
        case 0x4474u: /* cmp r1,#0x0 */
            (void)subtract(state,r[1],0u);r[15]=0x4478u;return 0;
        case 0x4478u: /* beq 0x0000448c */
            if(state->cpsr&0x40000000u){r[15]=17548u;}else{r[15]=0x447cu;}return 0;
        case 0x447cu: /* add r0,r0,#0xc */
            r[0]=r[0]+12u;r[15]=0x4480u;return 0;
        case 0x4480u: /* cmp r5,r0 */
            (void)subtract(state,r[5],r[0]);r[15]=0x4484u;return 0;
        case 0x4484u: /* beq 0x000044a4 */
            if(state->cpsr&0x40000000u){r[15]=17572u;}else{r[15]=0x4488u;}return 0;
        case 0x4488u: /* b 0x00004470 */
            r[15]=17520u;return 0;
        case 0x448cu: /* bic r1,r2,#0x1000000 */
            r[1]=r[2]&~16777216u;r[15]=0x4490u;return 0;
        case 0x4490u: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x4494u;return 0;
        case 0x4494u: /* str r4,[r0,#0x8] */
            STORE(r[0]+8u,r[4],4);r[15]=0x4498u;return 0;
        case 0x4498u: /* mov r6,#0x1 */
            r[6]=1u;r[15]=0x449cu;return 0;
        case 0x449cu: /* str r6,[r0,#0x4] */
            STORE(r[0]+4u,r[6],4);r[15]=0x44a0u;return 0;
        case 0x44a0u: /* b 0x000047bc */
            r[15]=18364u;return 0;
        case 0x44a4u: /* ldr r0,[r9,#0x0] */
            LOAD(r[0],r[9]+0u,4);r[15]=0x44a8u;return 0;
        case 0x44a8u: /* orr r0,r0,#0x80 */
            r[0]=r[0]|128u;r[15]=0x44acu;return 0;
        case 0x44acu: /* strb r0,[r9,#0x0] */
            STORE(r[9]+0u,r[0],1);r[15]=0x44b0u;return 0;
        case 0x44b0u: /* stmdb sp!,{r10} */
            STORE(r[13]-4u,r[10],4);r[13]-=4u;r[15]=0x44b4u;return 0;
        case 0x44b4u: /* ldr r0,[0x48d4] */
            LOAD(r[0],18644u,4);r[15]=0x44b8u;return 0;
        case 0x44b8u: /* ldr r10,[0x48d4] */
            LOAD(r[10],18644u,4);r[15]=0x44bcu;return 0;
        case 0x44bcu: /* mov r7,#0x1f */
            r[7]=31u;r[15]=0x44c0u;return 0;
        case 0x44c0u: /* mov r8,#0x0 */
            r[8]=0u;r[15]=0x44c4u;return 0;
        case 0x44c4u: /* ldr r6,[r0,#0x0] */
            LOAD(r[6],r[0]+0u,4);r[15]=0x44c8u;return 0;
        case 0x44c8u: /* mov r6,r6, lsr #0x1b */
            r[6]=(r[6]>>27);r[15]=0x44ccu;return 0;
        case 0x44ccu: /* cmp r6,#0x0 */
            (void)subtract(state,r[6],0u);r[15]=0x44d0u;return 0;
        case 0x44d0u: /* beq 0x000044f0 */
            if(state->cpsr&0x40000000u){r[15]=17648u;}else{r[15]=0x44d4u;}return 0;
        case 0x44d4u: /* cmp r6,r7 */
            (void)subtract(state,r[6],r[7]);r[15]=0x44d8u;return 0;
        case 0x44d8u: /* beq 0x00004504 */
            if(state->cpsr&0x40000000u){r[15]=17668u;}else{r[15]=0x44dcu;}return 0;
        case 0x44dcu: /* bcc 0x000044f0 */
            if(!(state->cpsr&0x20000000u)){r[15]=17648u;}else{r[15]=0x44e0u;}return 0;
        case 0x44e0u: /* add r0,r0,#0xc */
            r[0]=r[0]+12u;r[15]=0x44e4u;return 0;
        case 0x44e4u: /* cmp r0,r5 */
            (void)subtract(state,r[0],r[5]);r[15]=0x44e8u;return 0;
        case 0x44e8u: /* bcs 0x00004518 */
            if(state->cpsr&0x20000000u){r[15]=17688u;}else{r[15]=0x44ecu;}return 0;
        case 0x44ecu: /* b 0x000044c4 */
            r[15]=17604u;return 0;
        case 0x44f0u: /* mov r10,r0 */
            r[10]=r[0];r[15]=0x44f4u;return 0;
        case 0x44f4u: /* beq 0x00004518 */
            if(state->cpsr&0x40000000u){r[15]=17688u;}else{r[15]=0x44f8u;}return 0;
        case 0x44f8u: /* mov r7,r6 */
            r[7]=r[6];r[15]=0x44fcu;return 0;
        case 0x44fcu: /* ldr r8,[r0,#0x4] */
            LOAD(r[8],r[0]+4u,4);r[15]=0x4500u;return 0;
        case 0x4500u: /* b 0x000044e0 */
            r[15]=17632u;return 0;
        case 0x4504u: /* ldr r6,[r0,#0x4] */
            LOAD(r[6],r[0]+4u,4);r[15]=0x4508u;return 0;
        case 0x4508u: /* cmp r6,r8 */
            (void)subtract(state,r[6],r[8]);r[15]=0x450cu;return 0;
        case 0x450cu: /* movhi r8,r6 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[8]=r[6];}r[15]=0x4510u;return 0;
        case 0x4510u: /* mov r10,r0 */
            r[10]=r[0];r[15]=0x4514u;return 0;
        case 0x4514u: /* b 0x000044e0 */
            r[15]=17632u;return 0;
        case 0x4518u: /* ldr r5,[0x48d4] */
            LOAD(r[5],18644u,4);r[15]=0x451cu;return 0;
        case 0x451cu: /* ldr r1,[r9,#0x8] */
            LOAD(r[1],r[9]+8u,4);r[15]=0x4520u;return 0;
        case 0x4520u: /* cmp r1,r5 */
            (void)subtract(state,r[1],r[5]);r[15]=0x4524u;return 0;
        case 0x4524u: /* ldrcs r1,[0x48d0] */
            if(state->cpsr&0x20000000u){LOAD(r[1],18640u,4);}r[15]=0x4528u;return 0;
        case 0x4528u: /* ldr r8,[r1,#0x0] */
            LOAD(r[8],r[1]+0u,4);r[15]=0x452cu;return 0;
        case 0x452cu: /* cmp r8,#0x0 */
            (void)subtract(state,r[8],0u);r[15]=0x4530u;return 0;
        case 0x4530u: /* bne 0x0000486c */
            if(!(state->cpsr&0x40000000u)){r[15]=18540u;}else{r[15]=0x4534u;}return 0;
        case 0x4534u: /* mov r0,r10 */
            r[0]=r[10];r[15]=0x4538u;return 0;
        case 0x4538u: /* ldr r6,[r0,#0x0] */
            LOAD(r[6],r[0]+0u,4);r[15]=0x453cu;return 0;
        case 0x453cu: /* bic r6,r6,#0x1000000 */
            r[6]=r[6]&~16777216u;r[15]=0x4540u;return 0;
        case 0x4540u: /* str r6,[r1],#0x4 */
            STORE(r[1],r[6],4);r[1]+=4u;r[15]=0x4544u;return 0;
        case 0x4544u: /* str r1,[r9,#0x8] */
            STORE(r[9]+8u,r[1],4);r[15]=0x4548u;return 0;
        case 0x4548u: /* ldmia sp!,{r10} */
            LOAD(r[10],r[13]+0u,4);r[13]+=4u;r[15]=0x454cu;return 0;
        case 0x454cu: /* b 0x0000448c */
            r[15]=17548u;return 0;
        case 0x4550u: /* and r5,r0,#0x70 */
            r[5]=r[0]&112u;r[15]=0x4554u;return 0;
        case 0x4554u: /* and r0,r0,#0xf */
            r[0]=r[0]&15u;r[15]=0x4558u;return 0;
        case 0x4558u: /* ldrb r2,[r11,#0x0] */
            LOAD(r[2],r[11]+0u,1);r[15]=0x455cu;return 0;
        case 0x455cu: /* orr r2,r2,r5, lsr #0x4 */
            r[2]=r[2]|(r[5]>>4);r[15]=0x4560u;return 0;
        case 0x4560u: /* orr r2,r10,r2, lsl #0x4 */
            r[2]=r[10]|(r[2]<<4);r[15]=0x4564u;return 0;
        case 0x4564u: /* orr r2,r0,r2, lsl #0x4 */
            r[2]=r[0]|(r[2]<<4);r[15]=0x4568u;return 0;
        case 0x4568u: /* ldr r3,[0x48e0] */
            LOAD(r[3],18656u,4);r[15]=0x456cu;return 0;
        case 0x456cu: /* ldr r3,[r3,r1,lsl #0x2] */
            LOAD(r[3],r[3]+(r[1]<<2),4);r[15]=0x4570u;return 0;
        case 0x4570u: /* mov pc,r3 */
            r[15]=r[3];return 0;
        case 0x4590u: /* ldrb r4,[r12],#0x1 */
            LOAD(r[4],r[12],1);r[12]+=1u;r[15]=0x4594u;return 0;
        case 0x4594u: /* bic r1,r4,#0x80 */
            r[1]=r[4]&~128u;r[15]=0x4598u;return 0;
        case 0x4598u: /* orr r2,r1,r2, lsl #0x8 */
            r[2]=r[1]|(r[2]<<8);r[15]=0x459cu;return 0;
        case 0x459cu: /* ldrb r1,[r12],#0x1 */
            LOAD(r[1],r[12],1);r[12]+=1u;r[15]=0x45a0u;return 0;
        case 0x45a0u: /* orr r2,r1,r2, lsl #0x8 */
            r[2]=r[1]|(r[2]<<8);r[15]=0x45a4u;return 0;
        case 0x45a4u: /* b 0x000045b8 */
            r[15]=17848u;return 0;
        case 0x45a8u: /* ldrb r4,[r12],#0x1 */
            LOAD(r[4],r[12],1);r[12]+=1u;r[15]=0x45acu;return 0;
        case 0x45acu: /* bic r1,r4,#0x80 */
            r[1]=r[4]&~128u;r[15]=0x45b0u;return 0;
        case 0x45b0u: /* orr r2,r1,r2, lsl #0x8 */
            r[2]=r[1]|(r[2]<<8);r[15]=0x45b4u;return 0;
        case 0x45b4u: /* mov r2,r2, lsl #0x8 */
            r[2]=(r[2]<<8);r[15]=0x45b8u;return 0;
        case 0x45b8u: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x45bcu;return 0;
        case 0x45bcu: /* tst r4,#0x80 */
            temporary=r[4]&128u;logic_flags(state,temporary,0,128u);r[15]=0x45c0u;return 0;
        case 0x45c0u: /* ldrbne r4,[r12],#0x1 */
            if(!(state->cpsr&0x40000000u)){LOAD(r[4],r[12],1);r[12]+=1u;}r[15]=0x45c4u;return 0;
        case 0x45c4u: /* orrne r3,r4,r3, lsl #0x8 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[4]|(r[3]<<8);}r[15]=0x45c8u;return 0;
        case 0x45c8u: /* b 0x000047bc */
            r[15]=18364u;return 0;
        case 0x45ccu: /* mov r2,r2, lsl #0x8 */
            r[2]=(r[2]<<8);r[15]=0x45d0u;return 0;
        case 0x45d0u: /* ldrb r1,[r12],#0x1 */
            LOAD(r[1],r[12],1);r[12]+=1u;r[15]=0x45d4u;return 0;
        case 0x45d4u: /* orr r2,r1,r2, lsl #0x8 */
            r[2]=r[1]|(r[2]<<8);r[15]=0x45d8u;return 0;
        case 0x45d8u: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x45dcu;return 0;
        case 0x45dcu: /* b 0x000047bc */
            r[15]=18364u;return 0;
        case 0x45e0u: /* ldr r2,[0x48e4] */
            LOAD(r[2],18660u,4);r[15]=0x45e4u;return 0;
        case 0x45e4u: /* sub r1,r0,#0x80 */
            r[1]=r[0]-128u;r[15]=0x45e8u;return 0;
        case 0x45e8u: /* mov r3,#0x4 */
            r[3]=4u;r[15]=0x45ecu;return 0;
        case 0x45ecu: /* mul r3,r1,r3 */
            r[3]=r[1]*r[3];r[15]=0x45f0u;return 0;
        case 0x45f0u: /* ldr r3,[r2,r3] */
            LOAD(r[3],r[2]+r[3],4);r[15]=0x45f4u;return 0;
        case 0x45f4u: /* mov pc,r3 */
            r[15]=r[3];return 0;
        case 0x4638u: /* ldrb r1,[r12],#0x1 */
            LOAD(r[1],r[12],1);r[12]+=1u;r[15]=0x463cu;return 0;
        case 0x463cu: /* ldrb r2,[r12],#0x1 */
            LOAD(r[2],r[12],1);r[12]+=1u;r[15]=0x4640u;return 0;
        case 0x4640u: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x4644u;return 0;
        case 0x4644u: /* str r12,[r11,#0x1c] */
            STORE(r[11]+28u,r[12],4);r[15]=0x4648u;return 0;
        case 0x4648u: /* strb r3,[r11,#0x2] */
            STORE(r[11]+2u,r[3],1);r[15]=0x464cu;return 0;
        case 0x464cu: /* orr r1,r2,r1, lsl #0x8 */
            r[1]=r[2]|(r[1]<<8);r[15]=0x4650u;return 0;
        case 0x4650u: /* ldr r4,[r11,#0x14] */
            LOAD(r[4],r[11]+20u,4);r[15]=0x4654u;return 0;
        case 0x4654u: /* add r12,r4,r1 */
            r[12]=r[4]+r[1];r[15]=0x4658u;return 0;
        case 0x4658u: /* b 0x00004388 */
            r[15]=17288u;return 0;
        case 0x465cu: /* ldr r0,[r11,#0x20] */
            LOAD(r[0],r[11]+32u,4);r[15]=0x4660u;return 0;
        case 0x4660u: /* cmp r0,#0x0 */
            (void)subtract(state,r[0],0u);r[15]=0x4664u;return 0;
        case 0x4664u: /* bne 0x00004694 */
            if(!(state->cpsr&0x40000000u)){r[15]=18068u;}else{r[15]=0x4668u;}return 0;
        case 0x4668u: /* str r12,[r11,#0x20] */
            STORE(r[11]+32u,r[12],4);r[15]=0x466cu;return 0;
        case 0x466cu: /* str r0,[r11,#0x18] */
            STORE(r[11]+24u,r[0],4);r[15]=0x4670u;return 0;
        case 0x4670u: /* ldrb r0,[r12],#0x1 */
            LOAD(r[0],r[12],1);r[12]+=1u;r[15]=0x4674u;return 0;
        case 0x4674u: /* ands r1,r0,#0x7f */
            temporary=r[0]&127u;r[1]=temporary;logic_flags(state,temporary,0,127u);r[15]=0x4678u;return 0;
        case 0x4678u: /* moveq r1,#0x80 */
            if(state->cpsr&0x40000000u){r[1]=128u;}r[15]=0x467cu;return 0;
        case 0x467cu: /* strb r1,[r11,#0x3] */
            STORE(r[11]+3u,r[1],1);r[15]=0x4680u;return 0;
        case 0x4680u: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x4684u;return 0;
        case 0x4684u: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x4688u;return 0;
        case 0x4688u: /* ldrbne r1,[r12],#0x1 */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],r[12],1);r[12]+=1u;}r[15]=0x468cu;return 0;
        case 0x468cu: /* orrne r3,r1,r3, lsl #0x8 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[1]|(r[3]<<8);}r[15]=0x4690u;return 0;
        case 0x4690u: /* b 0x0000481c */
            r[15]=18460u;return 0;
        case 0x4694u: /* ldrb r1,[r11,#0x3] */
            LOAD(r[1],r[11]+3u,1);r[15]=0x4698u;return 0;
        case 0x4698u: /* cmp r1,#0x0 */
            (void)subtract(state,r[1],0u);r[15]=0x469cu;return 0;
        case 0x469cu: /* beq 0x000046c8 */
            if(state->cpsr&0x40000000u){r[15]=18120u;}else{r[15]=0x46a0u;}return 0;
        case 0x46a0u: /* ldr r12,[r11,#0x20] */
            LOAD(r[12],r[11]+32u,4);r[15]=0x46a4u;return 0;
        case 0x46a4u: /* ldrb r0,[r12],#0x1 */
            LOAD(r[0],r[12],1);r[12]+=1u;r[15]=0x46a8u;return 0;
        case 0x46a8u: /* ldr r2,[r11,#0x18] */
            LOAD(r[2],r[11]+24u,4);r[15]=0x46acu;return 0;
        case 0x46acu: /* add r2,r2,#0x1 */
            r[2]=r[2]+1u;r[15]=0x46b0u;return 0;
        case 0x46b0u: /* str r2,[r11,#0x18] */
            STORE(r[11]+24u,r[2],4);r[15]=0x46b4u;return 0;
        case 0x46b4u: /* cmp r1,#0x80 */
            (void)subtract(state,r[1],128u);r[15]=0x46b8u;return 0;
        case 0x46b8u: /* beq 0x00004680 */
            if(state->cpsr&0x40000000u){r[15]=18048u;}else{r[15]=0x46bcu;}return 0;
        case 0x46bcu: /* sub r1,r1,#0x1 */
            r[1]=r[1]-1u;r[15]=0x46c0u;return 0;
        case 0x46c0u: /* strb r1,[r11,#0x3] */
            STORE(r[11]+3u,r[1],1);r[15]=0x46c4u;return 0;
        case 0x46c4u: /* b 0x00004680 */
            r[15]=18048u;return 0;
        case 0x46c8u: /* str r1,[r11,#0x20] */
            STORE(r[11]+32u,r[1],4);r[15]=0x46ccu;return 0;
        case 0x46ccu: /* ldrb r0,[r12],#0x1 */
            LOAD(r[0],r[12],1);r[12]+=1u;r[15]=0x46d0u;return 0;
        case 0x46d0u: /* b 0x00004680 */
            r[15]=18048u;return 0;
        case 0x46d4u: /* ldrb r0,[r12],#0x1 */
            LOAD(r[0],r[12],1);r[12]+=1u;r[15]=0x46d8u;return 0;
        case 0x46d8u: /* ldrb r1,[r12],#0x1 */
            LOAD(r[1],r[12],1);r[12]+=1u;r[15]=0x46dcu;return 0;
        case 0x46dcu: /* orr r0,r1,r0, lsl #0x8 */
            r[0]=r[1]|(r[0]<<8);r[15]=0x46e0u;return 0;
        case 0x46e0u: /* str r0,[r11,#0x34] */
            STORE(r[11]+52u,r[0],4);r[15]=0x46e4u;return 0;
        case 0x46e4u: /* ldr r1,[r11,#0x8] */
            LOAD(r[1],r[11]+8u,4);r[15]=0x46e8u;return 0;
        case 0x46e8u: /* mul r0,r1,r0 */
            r[0]=r[1]*r[0];r[15]=0x46ecu;return 0;
        case 0x46ecu: /* mov r0,r0, lsr #0x4 */
            r[0]=(r[0]>>4);r[15]=0x46f0u;return 0;
        case 0x46f0u: /* str r0,[r11,#0xc] */
            STORE(r[11]+12u,r[0],4);r[15]=0x46f4u;return 0;
        case 0x46f4u: /* ldrb r3,[r12],#0x1 */
            LOAD(r[3],r[12],1);r[12]+=1u;r[15]=0x46f8u;return 0;
        case 0x46f8u: /* b 0x0000481c */
            r[15]=18460u;return 0;
        case 0x46fcu: /* and r0,r0,#0x3 */
            r[0]=r[0]&3u;r[15]=0x4700u;return 0;
        case 0x4700u: /* ldr r1,[0x48e8] */
            LOAD(r[1],18664u,4);r[15]=0x4704u;return 0;
        case 0x4704u: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x4708u;return 0;
        case 0x4708u: /* ldr r1,[r1,r0] */
            LOAD(r[1],r[1]+r[0],4);r[15]=0x470cu;return 0;
        case 0x470cu: /* ldr r0,[r11,#0xc] */
            LOAD(r[0],r[11]+12u,4);r[15]=0x4710u;return 0;
        case 0x4710u: /* ldr r2,[r11,#0x2c] */
            LOAD(r[2],r[11]+44u,4);r[15]=0x4714u;return 0;
        case 0x4714u: /* mlas r1,r0,r1,r2; C follows TI925T reference, not ARMv4 hardware proof */
            r[1]=r[0]*r[1]+r[2];logic_flags(state,r[1],0,0);r[15]=0x4718u;return 0;
        case 0x4718u: /* mvnvs r1,#0x0 */
            if(state->cpsr&0x10000000u)r[1]=~0u;r[15]=0x471cu;return 0;
        case 0x471cu: /* str r1,[r11,#0x2c] */
            STORE(r[11]+44u,r[1],4);r[15]=0x4720u;return 0;
        case 0x4720u: /* b 0x00004388 */
            r[15]=17288u;return 0;
        case 0x4724u: /* and r0,r0,#0x3 */
            r[0]=r[0]&3u;r[15]=0x4728u;return 0;
        case 0x4728u: /* ldr r1,[0x48ec] */
            LOAD(r[1],18668u,4);r[15]=0x472cu;return 0;
        case 0x472cu: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x4730u;return 0;
        case 0x4730u: /* ldr r1,[r1,r0] */
            LOAD(r[1],r[1]+r[0],4);r[15]=0x4734u;return 0;
        case 0x4734u: /* ldr r0,[r11,#0xc] */
            LOAD(r[0],r[11]+12u,4);r[15]=0x4738u;return 0;
        case 0x4738u: /* ldr r2,[r11,#0x28] */
            LOAD(r[2],r[11]+40u,4);r[15]=0x473cu;return 0;
        case 0x473cu: /* mlas r1,r0,r1,r2; C follows TI925T reference, not ARMv4 hardware proof */
            r[1]=r[0]*r[1]+r[2];logic_flags(state,r[1],0,0);r[15]=0x4740u;return 0;
        case 0x4740u: /* mvnvs r1,#0x0 */
            if(state->cpsr&0x10000000u)r[1]=~0u;r[15]=0x4744u;return 0;
        case 0x4744u: /* str r1,[r11,#0x28] */
            STORE(r[11]+40u,r[1],4);r[15]=0x4748u;return 0;
        case 0x4748u: /* b 0x00004388 */
            r[15]=17288u;return 0;
        case 0x476cu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x4770u;return 0;
        case 0x4770u: /* str r0,[r11,#0x0] */
            STORE(r[11]+0u,r[0],4);r[15]=0x4774u;return 0;
        case 0x4774u: /* str r0,[r11,#0x4] */
            STORE(r[11]+4u,r[0],4);r[15]=0x4778u;return 0;
        case 0x4778u: /* str r0,[r11,#0x8] */
            STORE(r[11]+8u,r[0],4);r[15]=0x477cu;return 0;
        case 0x477cu: /* str r0,[r11,#0xc] */
            STORE(r[11]+12u,r[0],4);r[15]=0x4780u;return 0;
        case 0x4780u: /* strb r0,[r11,#0x3] */
            STORE(r[11]+3u,r[0],1);r[15]=0x4784u;return 0;
        case 0x4784u: /* str r0,[r11,#0x18] */
            STORE(r[11]+24u,r[0],4);r[15]=0x4788u;return 0;
        case 0x4788u: /* str r0,[r11,#0x1c] */
            STORE(r[11]+28u,r[0],4);r[15]=0x478cu;return 0;
        case 0x478cu: /* str r0,[r11,#0x20] */
            STORE(r[11]+32u,r[0],4);r[15]=0x4790u;return 0;
        case 0x4790u: /* str r0,[r11,#0x24] */
            STORE(r[11]+36u,r[0],4);r[15]=0x4794u;return 0;
        case 0x4794u: /* str r0,[r11,#0x28] */
            STORE(r[11]+40u,r[0],4);r[15]=0x4798u;return 0;
        case 0x4798u: /* str r0,[r11,#0x2c] */
            STORE(r[11]+44u,r[0],4);r[15]=0x479cu;return 0;
        case 0x479cu: /* str r0,[r11,#0x30] */
            STORE(r[11]+48u,r[0],4);r[15]=0x47a0u;return 0;
        case 0x47a0u: /* str r0,[r11,#0x34] */
            STORE(r[11]+52u,r[0],4);r[15]=0x47a4u;return 0;
        case 0x47a4u: /* str r0,[r11,#0x38] */
            STORE(r[11]+56u,r[0],4);r[15]=0x47a8u;return 0;
        case 0x47a8u: /* mov r0,r10 */
            r[0]=r[10];r[15]=0x47acu;return 0;
        case 0x47acu: /* stmdb sp!,{r1,r2,lr} */
            STORE(r[13]-12u,r[1],4);STORE(r[13]-8u,r[2],4);STORE(r[13]-4u,r[14],4);r[13]-=12u;r[15]=0x47b0u;return 0;
        case 0x47b0u: /* bl 0x000063e4 */
            r[14]=0x47b4u;r[15]=25572u;return 0;
        case 0x47b4u: /* ldmia sp!,{r1,r2,lr} */
            LOAD(r[1],r[13]+0u,4);LOAD(r[2],r[13]+4u,4);LOAD(r[14],r[13]+8u,4);r[13]+=12u;r[15]=0x47b8u;return 0;
        case 0x47b8u: /* b 0x000043b4 */
            r[15]=17332u;return 0;
        case 0x47bcu: /* ldr r0,[r9,#0xc] */
            LOAD(r[0],r[9]+12u,4);r[15]=0x47c0u;return 0;
        case 0x47c0u: /* ldr r1,[0x48d4] */
            LOAD(r[1],18644u,4);r[15]=0x47c4u;return 0;
        case 0x47c4u: /* mov r8,#0x13000 */
            r[8]=77824u;r[15]=0x47c8u;return 0;
        case 0x47c8u: /* mov r7,#0xb000 */
            r[7]=45056u;r[15]=0x47ccu;return 0;
        case 0x47ccu: /* ldr r4,[0x48dc] */
            LOAD(r[4],18652u,4);r[15]=0x47d0u;return 0;
        case 0x47d0u: /* ldr r4,[r4,#0x0] */
            LOAD(r[4],r[4]+0u,4);r[15]=0x47d4u;return 0;
        case 0x47d4u: /* sub r4,r4,#0x4 */
            r[4]=r[4]-4u;r[15]=0x47d8u;return 0;
        case 0x47d8u: /* cmp r0,r1 */
            (void)subtract(state,r[0],r[1]);r[15]=0x47dcu;return 0;
        case 0x47dcu: /* ldrhi r0,[0x48d0] */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){LOAD(r[0],18640u,4);}r[15]=0x47e0u;return 0;
        case 0x47e0u: /* ldr r5,[r0,#0x0] */
            LOAD(r[5],r[0]+0u,4);r[15]=0x47e4u;return 0;
        case 0x47e4u: /* cmp r5,#0x0 */
            (void)subtract(state,r[5],0u);r[15]=0x47e8u;return 0;
        case 0x47e8u: /* bne 0x000047f8 */
            if(!(state->cpsr&0x40000000u)){r[15]=18424u;}else{r[15]=0x47ecu;}return 0;
        case 0x47ecu: /* str r0,[r9,#0xc] */
            STORE(r[9]+12u,r[0],4);r[15]=0x47f0u;return 0;
        case 0x47f0u: /* mov r5,r2 */
            r[5]=r[2];r[15]=0x47f4u;return 0;
        case 0x47f4u: /* mov r2,#0x0 */
            r[2]=0u;r[15]=0x47f8u;return 0;
        case 0x47f8u: /* ldr r6,[r8,#0x0] */
            LOAD(r[6],r[8]+0u,4);r[15]=0x47fcu;return 0;
        case 0x47fcu: /* bl 0x000048a8 */
            r[14]=0x4800u;r[15]=18600u;return 0;
        case 0x4800u: /* b 0x00004834 */
            r[15]=18484u;return 0;
        case 0x4804u: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x4808u;return 0;
        case 0x4808u: /* beq 0x0000481c */
            if(state->cpsr&0x40000000u){r[15]=18460u;}else{r[15]=0x480cu;}return 0;
        case 0x480cu: /* mov r5,#0x0 */
            r[5]=0u;r[15]=0x4810u;return 0;
        case 0x4810u: /* str r5,[r0,#0x0] */
            STORE(r[0]+0u,r[5],4);r[15]=0x4814u;return 0;
        case 0x4814u: /* add r0,r0,#0x4 */
            r[0]=r[0]+4u;r[15]=0x4818u;return 0;
        case 0x4818u: /* b 0x000047d8 */
            r[15]=18392u;return 0;
        case 0x481cu: /* cmp r3,#0x0 */
            (void)subtract(state,r[3],0u);r[15]=0x4820u;return 0;
        case 0x4820u: /* ble 0x0000436c */
            if((state->cpsr&0x40000000u)||(((state->cpsr>>31)^(state->cpsr>>28))&1u)!=0){r[15]=17260u;}else{r[15]=0x4824u;}return 0;
        case 0x4824u: /* bl 0x0000487c */
            r[14]=0x4828u;r[15]=18556u;return 0;
        case 0x4828u: /* cmp r3,#0x0 */
            (void)subtract(state,r[3],0u);r[15]=0x482cu;return 0;
        case 0x482cu: /* ble 0x0000436c */
            if((state->cpsr&0x40000000u)||(((state->cpsr>>31)^(state->cpsr>>28))&1u)!=0){r[15]=17260u;}else{r[15]=0x4830u;}return 0;
        case 0x4830u: /* b 0x000043b4 */
            r[15]=17332u;return 0;
        case 0x4834u: /* ldr r0,[r9,#0x8] */
            LOAD(r[0],r[9]+8u,4);r[15]=0x4838u;return 0;
        case 0x4838u: /* cmp r0,r1 */
            (void)subtract(state,r[0],r[1]);r[15]=0x483cu;return 0;
        case 0x483cu: /* ldrhi r0,[0x48d0] */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){LOAD(r[0],18640u,4);}r[15]=0x4840u;return 0;
        case 0x4840u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x4844u;return 0;
        case 0x4844u: /* cmp r1,#0x0 */
            (void)subtract(state,r[1],0u);r[15]=0x4848u;return 0;
        case 0x4848u: /* bne 0x0000486c */
            if(!(state->cpsr&0x40000000u)){r[15]=18540u;}else{r[15]=0x484cu;}return 0;
        case 0x484cu: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x4850u;return 0;
        case 0x4850u: /* strne r2,[r0],#0x4 */
            if(!(state->cpsr&0x40000000u)){STORE(r[0],r[2],4);r[0]+=4u;}r[15]=0x4854u;return 0;
        case 0x4854u: /* str r0,[r9,#0x8] */
            STORE(r[9]+8u,r[0],4);r[15]=0x4858u;return 0;
        case 0x4858u: /* bl 0x0000487c */
            r[14]=0x485cu;r[15]=18556u;return 0;
        case 0x485cu: /* strb r10,[r9,#0x2] */
            STORE(r[9]+2u,r[10],1);r[15]=0x4860u;return 0;
        case 0x4860u: /* str r11,[r9,#0x4] */
            STORE(r[9]+4u,r[11],4);r[15]=0x4864u;return 0;
        case 0x4864u: /* mov r0,#0x20 */
            r[0]=32u;r[15]=0x4868u;return 0;
        case 0x4868u: /* b 0x000043e0 */
            r[15]=17376u;return 0;
        case 0x486cu: /* mov r0,#0x1 */
            r[0]=1u;r[15]=0x4870u;return 0;
        case 0x4870u: /* strb r0,[r9,#0x1] */
            STORE(r[9]+1u,r[0],1);r[15]=0x4874u;return 0;
        case 0x4874u: /* mov r0,#0x60 */
            r[0]=96u;r[15]=0x4878u;return 0;
        case 0x4878u: /* b 0x000043e0 */
            r[15]=17376u;return 0;
        case 0x487cu: /* ldr r0,[r11,#0xc] */
            LOAD(r[0],r[11]+12u,4);r[15]=0x4880u;return 0;
        case 0x4880u: /* ldr r4,[r11,#0x28] */
            LOAD(r[4],r[11]+40u,4);r[15]=0x4884u;return 0;
        case 0x4884u: /* mlas r3,r0,r3,r4; C follows TI925T reference, not ARMv4 hardware proof */
            r[3]=r[0]*r[3]+r[4];logic_flags(state,r[3],0,0);r[15]=0x4888u;return 0;
        case 0x4888u: /* mvnvs r3,#0x0 */
            if(state->cpsr&0x10000000u)r[3]=~0u;r[15]=0x488cu;return 0;
        case 0x488cu: /* mov r4,#0x0 */
            r[4]=0u;r[15]=0x4890u;return 0;
        case 0x4890u: /* str r4,[r11,#0x28] */
            STORE(r[11]+40u,r[4],4);r[15]=0x4894u;return 0;
        case 0x4894u: /* ldr r0,[r11,#0x24] */
            LOAD(r[0],r[11]+36u,4);r[15]=0x4898u;return 0;
        case 0x4898u: /* add r3,r3,r0 */
            r[3]=r[3]+r[0];r[15]=0x489cu;return 0;
        case 0x489cu: /* str r3,[r11,#0x24] */
            STORE(r[11]+36u,r[3],4);r[15]=0x48a0u;return 0;
        case 0x48a0u: /* str r12,[r11,#0x10] */
            STORE(r[11]+16u,r[12],4);r[15]=0x48a4u;return 0;
        case 0x48a4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x48a8u: /* cmp r4,r6 */
            (void)subtract(state,r[4],r[6]);r[15]=0x48acu;return 0;
        case 0x48acu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x48b0u;}return 0;
        case 0x48b0u: /* str r5,[r7,r6] */
            STORE(r[7]+r[6],r[5],4);r[15]=0x48b4u;return 0;
        case 0x48b4u: /* ldr r5,[0x48d8] */
            LOAD(r[5],18648u,4);r[15]=0x48b8u;return 0;
        case 0x48b8u: /* add r6,r6,#0x4 */
            r[6]=r[6]+4u;r[15]=0x48bcu;return 0;
        case 0x48bcu: /* and r6,r6,r5 */
            r[6]=r[6]&r[5];r[15]=0x48c0u;return 0;
        case 0x48c0u: /* str r6,[r8,#0x0] */
            STORE(r[8]+0u,r[6],4);r[15]=0x48c4u;return 0;
        case 0x48c4u: /* add lr,lr,#0x4 */
            r[14]=r[14]+4u;r[15]=0x48c8u;return 0;
        case 0x48c8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x63e4u: /* mov r1,#0xc000 */
            r[1]=49152u;r[15]=0x63e8u;return 0;
        case 0x63e8u: /* mov r2,#0x80 */
            r[2]=128u;r[15]=0x63ecu;return 0;
        case 0x63ecu: /* mla r1,r2,r0,r1 */
            r[1]=r[2]*r[0]+r[1];r[15]=0x63f0u;return 0;
        case 0x63f0u: /* ldrb r0,[r1,#0x28] */
            LOAD(r[0],r[1]+40u,1);r[15]=0x63f4u;return 0;
        case 0x63f4u: /* bic r0,r0,#0x1 */
            r[0]=r[0]&~1u;r[15]=0x63f8u;return 0;
        case 0x63f8u: /* strb r0,[r1,#0x28] */
            STORE(r[1]+40u,r[0],1);r[15]=0x63fcu;return 0;
        case 0x63fcu: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}
}

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
int sa_arm_sound_stream_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
        case 0x70a4u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x70a8u;return 0;
        case 0x70a8u: /* ldr r12,[0x70fc] */
            LOAD(r[12],28924u,4);r[15]=0x70acu;return 0;
        case 0x70acu: /* ldr r9,[0x73a8] */
            LOAD(r[9],29608u,4);r[15]=0x70b0u;return 0;
        case 0x70b0u: /* mov r5,#0x8 */
            r[5]=8u;r[15]=0x70b4u;return 0;
        case 0x70b4u: /* bl 0x00007100 */
            r[14]=0x70b8u;r[15]=28928u;return 0;
        case 0x70b8u: /* tst r0,#0x1 */
            temporary=r[0]&1u;logic_flags(state,temporary,0,1u);r[15]=0x70bcu;return 0;
        case 0x70bcu: /* blne 0x000072d8 */
            if(!(state->cpsr&0x40000000u)){r[14]=0x70c0u;r[15]=29400u;}else{r[15]=0x70c0u;}return 0;
        case 0x70c0u: /* strb r0,[r12,#0x0] */
            STORE(r[12]+0u,r[0],1);r[15]=0x70c4u;return 0;
        case 0x70c4u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x70c8u;return 0;
        case 0x70c8u: /* subs r5,r5,#0x1 */
            r[5]=subtract(state,r[5],1u);r[15]=0x70ccu;return 0;
        case 0x70ccu: /* bne 0x000070b4 */
            if(!(state->cpsr&0x40000000u)){r[15]=28852u;}else{r[15]=0x70d0u;}return 0;
        case 0x70d0u: /* mov r5,#0x10 */
            r[5]=16u;r[15]=0x70d4u;return 0;
        case 0x70d4u: /* bl 0x00007100 */
            r[14]=0x70d8u;r[15]=28928u;return 0;
        case 0x70d8u: /* strb r0,[r12,#0x0] */
            STORE(r[12]+0u,r[0],1);r[15]=0x70dcu;return 0;
        case 0x70dcu: /* tst r0,#0x1 */
            temporary=r[0]&1u;logic_flags(state,temporary,0,1u);r[15]=0x70e0u;return 0;
        case 0x70e0u: /* blne 0x00007350 */
            if(!(state->cpsr&0x40000000u)){r[14]=0x70e4u;r[15]=29520u;}else{r[15]=0x70e4u;}return 0;
        case 0x70e4u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x70e8u;return 0;
        case 0x70e8u: /* subs r5,r5,#0x1 */
            r[5]=subtract(state,r[5],1u);r[15]=0x70ecu;return 0;
        case 0x70ecu: /* bne 0x000070d4 */
            if(!(state->cpsr&0x40000000u)){r[15]=28884u;}else{r[15]=0x70f0u;}return 0;
        case 0x70f0u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x70f4u;return 0;
        case 0x70f4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7100u: /* ldrb r0,[r12,#0x0] */
            LOAD(r[0],r[12]+0u,1);r[15]=0x7104u;return 0;
        case 0x7104u: /* tst r0,#0x1 */
            temporary=r[0]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7108u;return 0;
        case 0x7108u: /* beq 0x00007128 */
            if(state->cpsr&0x40000000u){r[15]=28968u;}else{r[15]=0x710cu;}return 0;
        case 0x710cu: /* ldrb r1,[r12,#0x3] */
            LOAD(r[1],r[12]+3u,1);r[15]=0x7110u;return 0;
        case 0x7110u: /* mov r2,#0xc */
            r[2]=12u;r[15]=0x7114u;return 0;
        case 0x7114u: /* ldr r7,[0x70f8] */
            LOAD(r[7],28920u,4);r[15]=0x7118u;return 0;
        case 0x7118u: /* mla r10,r1,r2,r7 */
            r[10]=r[1]*r[2]+r[7];r[15]=0x711cu;return 0;
        case 0x711cu: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x7120u;return 0;
        case 0x7120u: /* add r1,r1,#0x30 */
            r[1]=r[1]+48u;r[15]=0x7124u;return 0;
        case 0x7124u: /* strb r1,[r9,#0xd] */
            STORE(r[9]+13u,r[1],1);r[15]=0x7128u;return 0;
        case 0x7128u: /* tst r0,#0x4 */
            temporary=r[0]&4u;logic_flags(state,temporary,0,4u);r[15]=0x712cu;return 0;
        case 0x712cu: /* beq 0x000071d0 */
            if(state->cpsr&0x40000000u){r[15]=29136u;}else{r[15]=0x7130u;}return 0;
        case 0x7130u: /* ldrb r2,[r12,#0x1c] */
            LOAD(r[2],r[12]+28u,1);r[15]=0x7134u;return 0;
        case 0x7134u: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x7138u;return 0;
        case 0x7138u: /* biceq r0,r0,#0x4 */
            if(state->cpsr&0x40000000u){r[0]=r[0]&~4u;}r[15]=0x713cu;return 0;
        case 0x713cu: /* ldrbeq r2,[r12,#0x18] */
            if(state->cpsr&0x40000000u){LOAD(r[2],r[12]+24u,1);}r[15]=0x7140u;return 0;
        case 0x7140u: /* beq 0x0000719c */
            if(state->cpsr&0x40000000u){r[15]=29084u;}else{r[15]=0x7144u;}return 0;
        case 0x7144u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x7148u;return 0;
        case 0x7148u: /* strb r2,[r12,#0x1c] */
            STORE(r[12]+28u,r[2],1);r[15]=0x714cu;return 0;
        case 0x714cu: /* bne 0x000071d0 */
            if(!(state->cpsr&0x40000000u)){r[15]=29136u;}else{r[15]=0x7150u;}return 0;
        case 0x7150u: /* ldrb r2,[r12,#0x1e] */
            LOAD(r[2],r[12]+30u,1);r[15]=0x7154u;return 0;
        case 0x7154u: /* ldrb r3,[r12,#0x15] */
            LOAD(r[3],r[12]+21u,1);r[15]=0x7158u;return 0;
        case 0x7158u: /* ldrb r4,[r12,#0x18] */
            LOAD(r[4],r[12]+24u,1);r[15]=0x715cu;return 0;
        case 0x715cu: /* cmp r2,r4 */
            (void)subtract(state,r[2],r[4]);r[15]=0x7160u;return 0;
        case 0x7160u: /* biceq r0,r0,#0x4 */
            if(state->cpsr&0x40000000u){r[0]=r[0]&~4u;}r[15]=0x7164u;return 0;
        case 0x7164u: /* beq 0x000071d0 */
            if(state->cpsr&0x40000000u){r[15]=29136u;}else{r[15]=0x7168u;}return 0;
        case 0x7168u: /* bcc 0x00007180 */
            if(!(state->cpsr&0x20000000u)){r[15]=29056u;}else{r[15]=0x716cu;}return 0;
        case 0x716cu: /* sub r2,r2,r3 */
            r[2]=r[2]-r[3];r[15]=0x7170u;return 0;
        case 0x7170u: /* cmp r2,r4 */
            (void)subtract(state,r[2],r[4]);r[15]=0x7174u;return 0;
        case 0x7174u: /* bhi 0x00007194 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=29076u;}else{r[15]=0x7178u;}return 0;
        case 0x7178u: /* movcc r2,r4 */
            if(!(state->cpsr&0x20000000u)){r[2]=r[4];r[15]=0x717cu;}else{r[15]=0x717cu;}return 0;
        case 0x717cu: /* b 0x0000719c */
            r[15]=29084u;return 0;
        case 0x7180u: /* add r2,r2,r3 */
            r[2]=r[2]+r[3];r[15]=0x7184u;return 0;
        case 0x7184u: /* cmp r2,r4 */
            (void)subtract(state,r[2],r[4]);r[15]=0x7188u;return 0;
        case 0x7188u: /* bcc 0x00007194 */
            if(!(state->cpsr&0x20000000u)){r[15]=29076u;}else{r[15]=0x718cu;}return 0;
        case 0x718cu: /* movhi r2,r4 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[2]=r[4];}r[15]=0x7190u;return 0;
        case 0x7190u: /* b 0x0000719c */
            r[15]=29084u;return 0;
        case 0x7194u: /* ldrb r4,[r12,#0x14] */
            LOAD(r[4],r[12]+20u,1);r[15]=0x7198u;return 0;
        case 0x7198u: /* strb r4,[r12,#0x1c] */
            STORE(r[12]+28u,r[4],1);r[15]=0x719cu;return 0;
        case 0x719cu: /* strb r2,[r12,#0x1e] */
            STORE(r[12]+30u,r[2],1);r[15]=0x71a0u;return 0;
        case 0x71a0u: /* tst r0,#0x1 */
            temporary=r[0]&1u;logic_flags(state,temporary,0,1u);r[15]=0x71a4u;return 0;
        case 0x71a4u: /* beq 0x000071d0 */
            if(state->cpsr&0x40000000u){r[15]=29136u;}else{r[15]=0x71a8u;}return 0;
        case 0x71a8u: /* mov r2,r2, lsl #0x1 */
            r[2]=(r[2]<<1);r[15]=0x71acu;return 0;
        case 0x71acu: /* rsb r2,r2,#0x100 */
            r[2]=256u-r[2];r[15]=0x71b0u;return 0;
        case 0x71b0u: /* ldrb r3,[r12,#0x24] */
            LOAD(r[3],r[12]+36u,1);r[15]=0x71b4u;return 0;
        case 0x71b4u: /* add r2,r2,r3 */
            r[2]=r[2]+r[3];r[15]=0x71b8u;return 0;
        case 0x71b8u: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x71bcu;return 0;
        case 0x71bcu: /* movmi r2,#0x0 */
            if(state->cpsr&0x80000000u){r[2]=0u;}r[15]=0x71c0u;return 0;
        case 0x71c0u: /* cmp r2,#0xff */
            (void)subtract(state,r[2],255u);r[15]=0x71c4u;return 0;
        case 0x71c4u: /* movgt r2,#0xff */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[2]=255u;r[15]=0x71c8u;}else{r[15]=0x71c8u;}return 0;
        case 0x71c8u: /* strb r2,[r11,#0x29] */
            STORE(r[11]+41u,r[2],1);r[15]=0x71ccu;return 0;
        case 0x71ccu: /* strb r2,[r12,#0x20] */
            STORE(r[12]+32u,r[2],1);r[15]=0x71d0u;return 0;
        case 0x71d0u: /* tst r0,#0x10 */
            temporary=r[0]&16u;logic_flags(state,temporary,0,16u);r[15]=0x71d4u;return 0;
        case 0x71d4u: /* beq 0x00007288 */
            if(state->cpsr&0x40000000u){r[15]=29320u;}else{r[15]=0x71d8u;}return 0;
        case 0x71d8u: /* ldrb r2,[r12,#0x1d] */
            LOAD(r[2],r[12]+29u,1);r[15]=0x71dcu;return 0;
        case 0x71dcu: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x71e0u;return 0;
        case 0x71e0u: /* biceq r0,r0,#0x10 */
            if(state->cpsr&0x40000000u){r[0]=r[0]&~16u;}r[15]=0x71e4u;return 0;
        case 0x71e4u: /* ldrbeq r2,[r12,#0x19] */
            if(state->cpsr&0x40000000u){LOAD(r[2],r[12]+25u,1);}r[15]=0x71e8u;return 0;
        case 0x71e8u: /* beq 0x00007244 */
            if(state->cpsr&0x40000000u){r[15]=29252u;}else{r[15]=0x71ecu;}return 0;
        case 0x71ecu: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x71f0u;return 0;
        case 0x71f0u: /* strb r2,[r12,#0x1d] */
            STORE(r[12]+29u,r[2],1);r[15]=0x71f4u;return 0;
        case 0x71f4u: /* bne 0x00007288 */
            if(!(state->cpsr&0x40000000u)){r[15]=29320u;}else{r[15]=0x71f8u;}return 0;
        case 0x71f8u: /* ldrb r2,[r12,#0x1f] */
            LOAD(r[2],r[12]+31u,1);r[15]=0x71fcu;return 0;
        case 0x71fcu: /* ldrb r3,[r12,#0x17] */
            LOAD(r[3],r[12]+23u,1);r[15]=0x7200u;return 0;
        case 0x7200u: /* ldrb r4,[r12,#0x19] */
            LOAD(r[4],r[12]+25u,1);r[15]=0x7204u;return 0;
        case 0x7204u: /* cmp r2,r4 */
            (void)subtract(state,r[2],r[4]);r[15]=0x7208u;return 0;
        case 0x7208u: /* biceq r0,r0,#0x10 */
            if(state->cpsr&0x40000000u){r[0]=r[0]&~16u;}r[15]=0x720cu;return 0;
        case 0x720cu: /* beq 0x00007288 */
            if(state->cpsr&0x40000000u){r[15]=29320u;}else{r[15]=0x7210u;}return 0;
        case 0x7210u: /* bcc 0x00007228 */
            if(!(state->cpsr&0x20000000u)){r[15]=29224u;}else{r[15]=0x7214u;}return 0;
        case 0x7214u: /* sub r2,r2,r3 */
            r[2]=r[2]-r[3];r[15]=0x7218u;return 0;
        case 0x7218u: /* cmp r2,r4 */
            (void)subtract(state,r[2],r[4]);r[15]=0x721cu;return 0;
        case 0x721cu: /* bhi 0x0000723c */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=29244u;}else{r[15]=0x7220u;}return 0;
        case 0x7220u: /* movcc r2,r4 */
            if(!(state->cpsr&0x20000000u)){r[2]=r[4];r[15]=0x7224u;}else{r[15]=0x7224u;}return 0;
        case 0x7224u: /* b 0x00007244 */
            r[15]=29252u;return 0;
        case 0x7228u: /* add r2,r2,r3 */
            r[2]=r[2]+r[3];r[15]=0x722cu;return 0;
        case 0x722cu: /* cmp r2,r4 */
            (void)subtract(state,r[2],r[4]);r[15]=0x7230u;return 0;
        case 0x7230u: /* bcc 0x0000723c */
            if(!(state->cpsr&0x20000000u)){r[15]=29244u;}else{r[15]=0x7234u;}return 0;
        case 0x7234u: /* movhi r2,r4 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[2]=r[4];}r[15]=0x7238u;return 0;
        case 0x7238u: /* b 0x00007244 */
            r[15]=29252u;return 0;
        case 0x723cu: /* ldrb r4,[r12,#0x16] */
            LOAD(r[4],r[12]+22u,1);r[15]=0x7240u;return 0;
        case 0x7240u: /* strb r4,[r12,#0x1d] */
            STORE(r[12]+29u,r[4],1);r[15]=0x7244u;return 0;
        case 0x7244u: /* strb r2,[r12,#0x1f] */
            STORE(r[12]+31u,r[2],1);r[15]=0x7248u;return 0;
        case 0x7248u: /* tst r0,#0x1 */
            temporary=r[0]&1u;logic_flags(state,temporary,0,1u);r[15]=0x724cu;return 0;
        case 0x724cu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7250u;}return 0;
        case 0x7250u: /* mov r2,r2, lsr #0x2 */
            r[2]=(r[2]>>2);r[15]=0x7254u;return 0;
        case 0x7254u: /* sub r2,r2,#0x20 */
            r[2]=r[2]-32u;r[15]=0x7258u;return 0;
        case 0x7258u: /* ldrb r3,[r12,#0x28] */
            LOAD(r[3],r[12]+40u,1);r[15]=0x725cu;return 0;
        case 0x725cu: /* ldr r4,[0x73ac] */
            LOAD(r[4],29612u,4);r[15]=0x7260u;return 0;
        case 0x7260u: /* ldrb r3,[r4,r3] */
            LOAD(r[3],r[4]+r[3],1);r[15]=0x7264u;return 0;
        case 0x7264u: /* add r3,r3,r2 */
            r[3]=r[3]+r[2];r[15]=0x7268u;return 0;
        case 0x7268u: /* cmp r3,#0x0 */
            (void)subtract(state,r[3],0u);r[15]=0x726cu;return 0;
        case 0x726cu: /* movmi r3,#0x0 */
            if(state->cpsr&0x80000000u){r[3]=0u;}r[15]=0x7270u;return 0;
        case 0x7270u: /* cmp r3,#0x1f */
            (void)subtract(state,r[3],31u);r[15]=0x7274u;return 0;
        case 0x7274u: /* movgt r3,#0x1f */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[3]=31u;r[15]=0x7278u;}else{r[15]=0x7278u;}return 0;
        case 0x7278u: /* ldr r4,[0x73b0] */
            LOAD(r[4],29616u,4);r[15]=0x727cu;return 0;
        case 0x727cu: /* ldrb r2,[r4,r3] */
            LOAD(r[2],r[4]+r[3],1);r[15]=0x7280u;return 0;
        case 0x7280u: /* strb r2,[r11,#0x24] */
            STORE(r[11]+36u,r[2],1);r[15]=0x7284u;return 0;
        case 0x7284u: /* strb r2,[r12,#0x21] */
            STORE(r[12]+33u,r[2],1);r[15]=0x7288u;return 0;
        case 0x7288u: /* tst r0,#0x1 */
            temporary=r[0]&1u;logic_flags(state,temporary,0,1u);r[15]=0x728cu;return 0;
        case 0x728cu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7290u;}return 0;
        case 0x7290u: /* ldr r4,[r12,#0xc] */
            LOAD(r[4],r[12]+12u,4);r[15]=0x7294u;return 0;
        case 0x7294u: /* b 0x00007d64 */
            r[15]=32100u;return 0;
        case 0x72d8u: /* mov r3,#0x1e */
            r[3]=30u;r[15]=0x72dcu;return 0;
        case 0x72dcu: /* ldr r2,[r12,#0x30] */
            LOAD(r[2],r[12]+48u,4);r[15]=0x72e0u;return 0;
        case 0x72e0u: /* subs r3,r3,#0x1 */
            r[3]=subtract(state,r[3],1u);r[15]=0x72e4u;return 0;
        case 0x72e4u: /* bne 0x000072dc */
            if(!(state->cpsr&0x40000000u)){r[15]=29404u;}else{r[15]=0x72e8u;}return 0;
        case 0x72e8u: /* ldr r1,[r9,#0x10] */
            LOAD(r[1],r[9]+16u,4);r[15]=0x72ecu;return 0;
        case 0x72ecu: /* tst r1,#0x8000 */
            temporary=r[1]&32768u;logic_flags(state,temporary,1,32768u);r[15]=0x72f0u;return 0;
        case 0x72f0u: /* beq 0x0000737c */
            if(state->cpsr&0x40000000u){r[15]=29564u;}else{r[15]=0x72f4u;}return 0;
        case 0x72f4u: /* ldr r1,[r11,#0x0] */
            LOAD(r[1],r[11]+0u,4);r[15]=0x72f8u;return 0;
        case 0x72f8u: /* tst r1,#0x200 */
            temporary=r[1]&512u;logic_flags(state,temporary,1,512u);r[15]=0x72fcu;return 0;
        case 0x72fcu: /* beq 0x00007318 */
            if(state->cpsr&0x40000000u){r[15]=29464u;}else{r[15]=0x7300u;}return 0;
        case 0x7300u: /* ldr r1,[r12,#0x38] */
            LOAD(r[1],r[12]+56u,4);r[15]=0x7304u;return 0;
        case 0x7304u: /* cmp r1,#0x0 */
            (void)subtract(state,r[1],0u);r[15]=0x7308u;return 0;
        case 0x7308u: /* beq 0x00007370 */
            if(state->cpsr&0x40000000u){r[15]=29552u;}else{r[15]=0x730cu;}return 0;
        case 0x730cu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x7310u;return 0;
        case 0x7310u: /* str r1,[r12,#0x38] */
            STORE(r[12]+56u,r[1],4);r[15]=0x7314u;return 0;
        case 0x7314u: /* bne 0x00007370 */
            if(!(state->cpsr&0x40000000u)){r[15]=29552u;}else{r[15]=0x7318u;}return 0;
        case 0x7318u: /* ldr r1,[r11,#0x0] */
            LOAD(r[1],r[11]+0u,4);r[15]=0x731cu;return 0;
        case 0x731cu: /* orr r1,r1,#0x8000 */
            r[1]=r[1]|32768u;r[15]=0x7320u;return 0;
        case 0x7320u: /* bic r1,r1,#0x4000 */
            r[1]=r[1]&~16384u;r[15]=0x7324u;return 0;
        case 0x7324u: /* str r1,[r11,#0x0] */
            STORE(r[11]+0u,r[1],4);r[15]=0x7328u;return 0;
        case 0x7328u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x732cu;return 0;
        case 0x732cu: /* ldrb r1,[r12,#0x23] */
            LOAD(r[1],r[12]+35u,1);r[15]=0x7330u;return 0;
        case 0x7330u: /* ldrb r7,[r12,#0x29] */
            LOAD(r[7],r[12]+41u,1);r[15]=0x7334u;return 0;
        case 0x7334u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7338u;return 0;
        case 0x7338u: /* bl 0x0000817c */
            r[14]=0x733cu;r[15]=33148u;return 0;
        case 0x733cu: /* strb r1,[r12,#0x23] */
            STORE(r[12]+35u,r[1],1);r[15]=0x7340u;return 0;
        case 0x7340u: /* strb r7,[r12,#0x29] */
            STORE(r[12]+41u,r[7],1);r[15]=0x7344u;return 0;
        case 0x7344u: /* bic r0,r0,#0x1 */
            r[0]=r[0]&~1u;r[15]=0x7348u;return 0;
        case 0x7348u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x734cu;return 0;
        case 0x734cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7350u: /* mov r3,#0x1e */
            r[3]=30u;r[15]=0x7354u;return 0;
        case 0x7354u: /* ldr r2,[r12,#0x30] */
            LOAD(r[2],r[12]+48u,4);r[15]=0x7358u;return 0;
        case 0x7358u: /* subs r3,r3,#0x1 */
            r[3]=subtract(state,r[3],1u);r[15]=0x735cu;return 0;
        case 0x735cu: /* bne 0x00007354 */
            if(!(state->cpsr&0x40000000u)){r[15]=29524u;}else{r[15]=0x7360u;}return 0;
        case 0x7360u: /* ldr r1,[r9,#0x10] */
            LOAD(r[1],r[9]+16u,4);r[15]=0x7364u;return 0;
        case 0x7364u: /* movs r1,r1, lsr #0xf */
            temporary=r[1];r[1]=temporary>>15;logic_flags(state,r[1],0,0);state->cpsr=(state->cpsr&~0x20000000u)|(((temporary>>14)&1u)<<29);r[15]=0x7368u;return 0;
        case 0x7368u: /* strb r1,[r12,#0x23] */
            STORE(r[12]+35u,r[1],1);r[15]=0x736cu;return 0;
        case 0x736cu: /* beq 0x0000737c */
            if(state->cpsr&0x40000000u){r[15]=29564u;}else{r[15]=0x7370u;}return 0;
        case 0x7370u: /* ldr r1,[r12,#0x2c] */
            LOAD(r[1],r[12]+44u,4);r[15]=0x7374u;return 0;
        case 0x7374u: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x7378u;return 0;
        case 0x7378u: /* str r1,[r12,#0x2c] */
            STORE(r[12]+44u,r[1],4);r[15]=0x737cu;return 0;
        case 0x737cu: /* ldr r3,[r9,#0x14] */
            LOAD(r[3],r[9]+20u,4);r[15]=0x7380u;return 0;
        case 0x7380u: /* cmp r3,#0x0 */
            (void)subtract(state,r[3],0u);r[15]=0x7384u;return 0;
        case 0x7384u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7388u;}return 0;
        case 0x7388u: /* ldrb r1,[r12,#0x27] */
            LOAD(r[1],r[12]+39u,1);r[15]=0x738cu;return 0;
        case 0x738cu: /* cmp r1,#0x1 */
            (void)subtract(state,r[1],1u);r[15]=0x7390u;return 0;
        case 0x7390u: /* movgt r1,r3, lsl #0x1 */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[1]=(r[3]<<1);r[15]=0x7394u;}else{r[15]=0x7394u;}return 0;
        case 0x7394u: /* movlt r1,r3, lsr #0x1 */
            if((((state->cpsr>>31)^(state->cpsr>>28))&1u)!=0){r[1]=(r[3]>>1);r[15]=0x7398u;}else{r[15]=0x7398u;}return 0;
        case 0x7398u: /* add r2,r1,r2 */
            r[2]=r[1]+r[2];r[15]=0x739cu;return 0;
        case 0x739cu: /* str r2,[r12,#0x8] */
            STORE(r[12]+8u,r[2],4);r[15]=0x73a0u;return 0;
        case 0x73a0u: /* str r3,[r12,#0x4] */
            STORE(r[12]+4u,r[3],4);r[15]=0x73a4u;return 0;
        case 0x73a4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7d64u: /* mov r6,r4, lsr #0xb */
            r[6]=(r[4]>>11);r[15]=0x7d68u;return 0;
        case 0x7d68u: /* ldr r7,[0x8254] */
            LOAD(r[7],33364u,4);r[15]=0x7d6cu;return 0;
        case 0x7d6cu: /* ldrb r6,[r7,r6] */
            LOAD(r[6],r[7]+r[6],1);r[15]=0x7d70u;return 0;
        case 0x7d70u: /* ldr r8,[0x77ec] */
            LOAD(r[8],30700u,4);r[15]=0x7d74u;return 0;
        case 0x7d74u: /* and r8,r4,r8 */
            r[8]=r[4]&r[8];r[15]=0x7d78u;return 0;
        case 0x7d78u: /* orr r8,r8,r6, lsl #0xa */
            r[8]=r[8]|(r[6]<<10);r[15]=0x7d7cu;return 0;
        case 0x7d7cu: /* ldr r6,[r12,#0x10] */
            LOAD(r[6],r[12]+16u,4);r[15]=0x7d80u;return 0;
        case 0x7d80u: /* add r6,r6,r8 */
            r[6]=r[6]+r[8];r[15]=0x7d84u;return 0;
        case 0x7d84u: /* ldr r8,[0x77ec] */
            LOAD(r[8],30700u,4);r[15]=0x7d88u;return 0;
        case 0x7d88u: /* and r8,r6,r8 */
            r[8]=r[6]&r[8];r[15]=0x7d8cu;return 0;
        case 0x7d8cu: /* mov r6,r6, lsr #0xa */
            r[6]=(r[6]>>10);r[15]=0x7d90u;return 0;
        case 0x7d90u: /* cmp r6,#0x0 */
            (void)subtract(state,r[6],0u);r[15]=0x7d94u;return 0;
        case 0x7d94u: /* movlt r6,#0x0 */
            if((((state->cpsr>>31)^(state->cpsr>>28))&1u)!=0){r[6]=0u;r[15]=0x7d98u;}else{r[15]=0x7d98u;}return 0;
        case 0x7d98u: /* cmp r6,#0xf */
            (void)subtract(state,r[6],15u);r[15]=0x7d9cu;return 0;
        case 0x7d9cu: /* movgt r6,#0xf */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[6]=15u;r[15]=0x7da0u;}else{r[15]=0x7da0u;}return 0;
        case 0x7da0u: /* ldrb r6,[r7,r6] */
            LOAD(r[6],r[7]+r[6],1);r[15]=0x7da4u;return 0;
        case 0x7da4u: /* orr r6,r8,r6, lsl #0xb */
            r[6]=r[8]|(r[6]<<11);r[15]=0x7da8u;return 0;
        case 0x7da8u: /* str r6,[r11,#0x18] */
            STORE(r[11]+24u,r[6],4);r[15]=0x7dacu;return 0;
        case 0x7dacu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x817cu: /* str r6,[r12,#0x3c] */
            STORE(r[12]+60u,r[6],4);r[15]=0x8180u;return 0;
        case 0x8180u: /* str r6,[r12,#0x38] */
            STORE(r[12]+56u,r[6],4);r[15]=0x8184u;return 0;
        case 0x8184u: /* str r6,[r12,#0x30] */
            STORE(r[12]+48u,r[6],4);r[15]=0x8188u;return 0;
        case 0x8188u: /* str r6,[r10,#0x4] */
            STORE(r[10]+4u,r[6],4);r[15]=0x818cu;return 0;
        case 0x818cu: /* str r6,[r10,#0x0] */
            STORE(r[10]+0u,r[6],4);r[15]=0x8190u;return 0;
        case 0x8190u: /* str r6,[r12,#0x28] */
            STORE(r[12]+40u,r[6],4);r[15]=0x8194u;return 0;
        case 0x8194u: /* strb r6,[r12,#0x3] */
            STORE(r[12]+3u,r[6],1);r[15]=0x8198u;return 0;
        case 0x8198u: /* strb r6,[r12,#0x2] */
            STORE(r[12]+2u,r[6],1);r[15]=0x819cu;return 0;
        case 0x819cu: /* str r6,[r12,#0xc] */
            STORE(r[12]+12u,r[6],4);r[15]=0x81a0u;return 0;
        case 0x81a0u: /* strb r6,[r12,#0x20] */
            STORE(r[12]+32u,r[6],1);r[15]=0x81a4u;return 0;
        case 0x81a4u: /* strb r6,[r12,#0x21] */
            STORE(r[12]+33u,r[6],1);r[15]=0x81a8u;return 0;
        case 0x81a8u: /* strb r6,[r12,#0x1d] */
            STORE(r[12]+29u,r[6],1);r[15]=0x81acu;return 0;
        case 0x81acu: /* strb r6,[r12,#0x1c] */
            STORE(r[12]+28u,r[6],1);r[15]=0x81b0u;return 0;
        case 0x81b0u: /* str r6,[r12,#0x24] */
            STORE(r[12]+36u,r[6],4);r[15]=0x81b4u;return 0;
        case 0x81b4u: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}else{switch(r[15]){
        case 0x70acu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x70b0u;return 0;
        case 0x70b0u: /* ldr r12,[0x7104] */
            LOAD(r[12],28932u,4);r[15]=0x70b4u;return 0;
        case 0x70b4u: /* ldr r9,[0x73b0] */
            LOAD(r[9],29616u,4);r[15]=0x70b8u;return 0;
        case 0x70b8u: /* mov r5,#0x8 */
            r[5]=8u;r[15]=0x70bcu;return 0;
        case 0x70bcu: /* bl 0x00007108 */
            r[14]=0x70c0u;r[15]=28936u;return 0;
        case 0x70c0u: /* tst r0,#0x1 */
            temporary=r[0]&1u;logic_flags(state,temporary,0,1u);r[15]=0x70c4u;return 0;
        case 0x70c4u: /* blne 0x000072e0 */
            if(!(state->cpsr&0x40000000u)){r[14]=0x70c8u;r[15]=29408u;}else{r[15]=0x70c8u;}return 0;
        case 0x70c8u: /* strb r0,[r12,#0x0] */
            STORE(r[12]+0u,r[0],1);r[15]=0x70ccu;return 0;
        case 0x70ccu: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x70d0u;return 0;
        case 0x70d0u: /* subs r5,r5,#0x1 */
            r[5]=subtract(state,r[5],1u);r[15]=0x70d4u;return 0;
        case 0x70d4u: /* bne 0x000070bc */
            if(!(state->cpsr&0x40000000u)){r[15]=28860u;}else{r[15]=0x70d8u;}return 0;
        case 0x70d8u: /* mov r5,#0x10 */
            r[5]=16u;r[15]=0x70dcu;return 0;
        case 0x70dcu: /* bl 0x00007108 */
            r[14]=0x70e0u;r[15]=28936u;return 0;
        case 0x70e0u: /* strb r0,[r12,#0x0] */
            STORE(r[12]+0u,r[0],1);r[15]=0x70e4u;return 0;
        case 0x70e4u: /* tst r0,#0x1 */
            temporary=r[0]&1u;logic_flags(state,temporary,0,1u);r[15]=0x70e8u;return 0;
        case 0x70e8u: /* blne 0x00007358 */
            if(!(state->cpsr&0x40000000u)){r[14]=0x70ecu;r[15]=29528u;}else{r[15]=0x70ecu;}return 0;
        case 0x70ecu: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x70f0u;return 0;
        case 0x70f0u: /* subs r5,r5,#0x1 */
            r[5]=subtract(state,r[5],1u);r[15]=0x70f4u;return 0;
        case 0x70f4u: /* bne 0x000070dc */
            if(!(state->cpsr&0x40000000u)){r[15]=28892u;}else{r[15]=0x70f8u;}return 0;
        case 0x70f8u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x70fcu;return 0;
        case 0x70fcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7108u: /* ldrb r0,[r12,#0x0] */
            LOAD(r[0],r[12]+0u,1);r[15]=0x710cu;return 0;
        case 0x710cu: /* tst r0,#0x1 */
            temporary=r[0]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7110u;return 0;
        case 0x7110u: /* beq 0x00007130 */
            if(state->cpsr&0x40000000u){r[15]=28976u;}else{r[15]=0x7114u;}return 0;
        case 0x7114u: /* ldrb r1,[r12,#0x3] */
            LOAD(r[1],r[12]+3u,1);r[15]=0x7118u;return 0;
        case 0x7118u: /* mov r2,#0xc */
            r[2]=12u;r[15]=0x711cu;return 0;
        case 0x711cu: /* ldr r7,[0x7100] */
            LOAD(r[7],28928u,4);r[15]=0x7120u;return 0;
        case 0x7120u: /* mla r10,r1,r2,r7 */
            r[10]=r[1]*r[2]+r[7];r[15]=0x7124u;return 0;
        case 0x7124u: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x7128u;return 0;
        case 0x7128u: /* add r1,r1,#0x30 */
            r[1]=r[1]+48u;r[15]=0x712cu;return 0;
        case 0x712cu: /* strb r1,[r9,#0xd] */
            STORE(r[9]+13u,r[1],1);r[15]=0x7130u;return 0;
        case 0x7130u: /* tst r0,#0x4 */
            temporary=r[0]&4u;logic_flags(state,temporary,0,4u);r[15]=0x7134u;return 0;
        case 0x7134u: /* beq 0x000071d8 */
            if(state->cpsr&0x40000000u){r[15]=29144u;}else{r[15]=0x7138u;}return 0;
        case 0x7138u: /* ldrb r2,[r12,#0x1c] */
            LOAD(r[2],r[12]+28u,1);r[15]=0x713cu;return 0;
        case 0x713cu: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x7140u;return 0;
        case 0x7140u: /* biceq r0,r0,#0x4 */
            if(state->cpsr&0x40000000u){r[0]=r[0]&~4u;}r[15]=0x7144u;return 0;
        case 0x7144u: /* ldrbeq r2,[r12,#0x18] */
            if(state->cpsr&0x40000000u){LOAD(r[2],r[12]+24u,1);}r[15]=0x7148u;return 0;
        case 0x7148u: /* beq 0x000071a4 */
            if(state->cpsr&0x40000000u){r[15]=29092u;}else{r[15]=0x714cu;}return 0;
        case 0x714cu: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x7150u;return 0;
        case 0x7150u: /* strb r2,[r12,#0x1c] */
            STORE(r[12]+28u,r[2],1);r[15]=0x7154u;return 0;
        case 0x7154u: /* bne 0x000071d8 */
            if(!(state->cpsr&0x40000000u)){r[15]=29144u;}else{r[15]=0x7158u;}return 0;
        case 0x7158u: /* ldrb r2,[r12,#0x1e] */
            LOAD(r[2],r[12]+30u,1);r[15]=0x715cu;return 0;
        case 0x715cu: /* ldrb r3,[r12,#0x15] */
            LOAD(r[3],r[12]+21u,1);r[15]=0x7160u;return 0;
        case 0x7160u: /* ldrb r4,[r12,#0x18] */
            LOAD(r[4],r[12]+24u,1);r[15]=0x7164u;return 0;
        case 0x7164u: /* cmp r2,r4 */
            (void)subtract(state,r[2],r[4]);r[15]=0x7168u;return 0;
        case 0x7168u: /* biceq r0,r0,#0x4 */
            if(state->cpsr&0x40000000u){r[0]=r[0]&~4u;}r[15]=0x716cu;return 0;
        case 0x716cu: /* beq 0x000071d8 */
            if(state->cpsr&0x40000000u){r[15]=29144u;}else{r[15]=0x7170u;}return 0;
        case 0x7170u: /* bcc 0x00007188 */
            if(!(state->cpsr&0x20000000u)){r[15]=29064u;}else{r[15]=0x7174u;}return 0;
        case 0x7174u: /* sub r2,r2,r3 */
            r[2]=r[2]-r[3];r[15]=0x7178u;return 0;
        case 0x7178u: /* cmp r2,r4 */
            (void)subtract(state,r[2],r[4]);r[15]=0x717cu;return 0;
        case 0x717cu: /* bhi 0x0000719c */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=29084u;}else{r[15]=0x7180u;}return 0;
        case 0x7180u: /* movcc r2,r4 */
            if(!(state->cpsr&0x20000000u)){r[2]=r[4];r[15]=0x7184u;}else{r[15]=0x7184u;}return 0;
        case 0x7184u: /* b 0x000071a4 */
            r[15]=29092u;return 0;
        case 0x7188u: /* add r2,r2,r3 */
            r[2]=r[2]+r[3];r[15]=0x718cu;return 0;
        case 0x718cu: /* cmp r2,r4 */
            (void)subtract(state,r[2],r[4]);r[15]=0x7190u;return 0;
        case 0x7190u: /* bcc 0x0000719c */
            if(!(state->cpsr&0x20000000u)){r[15]=29084u;}else{r[15]=0x7194u;}return 0;
        case 0x7194u: /* movhi r2,r4 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[2]=r[4];}r[15]=0x7198u;return 0;
        case 0x7198u: /* b 0x000071a4 */
            r[15]=29092u;return 0;
        case 0x719cu: /* ldrb r4,[r12,#0x14] */
            LOAD(r[4],r[12]+20u,1);r[15]=0x71a0u;return 0;
        case 0x71a0u: /* strb r4,[r12,#0x1c] */
            STORE(r[12]+28u,r[4],1);r[15]=0x71a4u;return 0;
        case 0x71a4u: /* strb r2,[r12,#0x1e] */
            STORE(r[12]+30u,r[2],1);r[15]=0x71a8u;return 0;
        case 0x71a8u: /* tst r0,#0x1 */
            temporary=r[0]&1u;logic_flags(state,temporary,0,1u);r[15]=0x71acu;return 0;
        case 0x71acu: /* beq 0x000071d8 */
            if(state->cpsr&0x40000000u){r[15]=29144u;}else{r[15]=0x71b0u;}return 0;
        case 0x71b0u: /* mov r2,r2, lsl #0x1 */
            r[2]=(r[2]<<1);r[15]=0x71b4u;return 0;
        case 0x71b4u: /* rsb r2,r2,#0x100 */
            r[2]=256u-r[2];r[15]=0x71b8u;return 0;
        case 0x71b8u: /* ldrb r3,[r12,#0x24] */
            LOAD(r[3],r[12]+36u,1);r[15]=0x71bcu;return 0;
        case 0x71bcu: /* add r2,r2,r3 */
            r[2]=r[2]+r[3];r[15]=0x71c0u;return 0;
        case 0x71c0u: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x71c4u;return 0;
        case 0x71c4u: /* movmi r2,#0x0 */
            if(state->cpsr&0x80000000u){r[2]=0u;}r[15]=0x71c8u;return 0;
        case 0x71c8u: /* cmp r2,#0xff */
            (void)subtract(state,r[2],255u);r[15]=0x71ccu;return 0;
        case 0x71ccu: /* movgt r2,#0xff */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[2]=255u;r[15]=0x71d0u;}else{r[15]=0x71d0u;}return 0;
        case 0x71d0u: /* strb r2,[r11,#0x29] */
            STORE(r[11]+41u,r[2],1);r[15]=0x71d4u;return 0;
        case 0x71d4u: /* strb r2,[r12,#0x20] */
            STORE(r[12]+32u,r[2],1);r[15]=0x71d8u;return 0;
        case 0x71d8u: /* tst r0,#0x10 */
            temporary=r[0]&16u;logic_flags(state,temporary,0,16u);r[15]=0x71dcu;return 0;
        case 0x71dcu: /* beq 0x00007290 */
            if(state->cpsr&0x40000000u){r[15]=29328u;}else{r[15]=0x71e0u;}return 0;
        case 0x71e0u: /* ldrb r2,[r12,#0x1d] */
            LOAD(r[2],r[12]+29u,1);r[15]=0x71e4u;return 0;
        case 0x71e4u: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x71e8u;return 0;
        case 0x71e8u: /* biceq r0,r0,#0x10 */
            if(state->cpsr&0x40000000u){r[0]=r[0]&~16u;}r[15]=0x71ecu;return 0;
        case 0x71ecu: /* ldrbeq r2,[r12,#0x19] */
            if(state->cpsr&0x40000000u){LOAD(r[2],r[12]+25u,1);}r[15]=0x71f0u;return 0;
        case 0x71f0u: /* beq 0x0000724c */
            if(state->cpsr&0x40000000u){r[15]=29260u;}else{r[15]=0x71f4u;}return 0;
        case 0x71f4u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x71f8u;return 0;
        case 0x71f8u: /* strb r2,[r12,#0x1d] */
            STORE(r[12]+29u,r[2],1);r[15]=0x71fcu;return 0;
        case 0x71fcu: /* bne 0x00007290 */
            if(!(state->cpsr&0x40000000u)){r[15]=29328u;}else{r[15]=0x7200u;}return 0;
        case 0x7200u: /* ldrb r2,[r12,#0x1f] */
            LOAD(r[2],r[12]+31u,1);r[15]=0x7204u;return 0;
        case 0x7204u: /* ldrb r3,[r12,#0x17] */
            LOAD(r[3],r[12]+23u,1);r[15]=0x7208u;return 0;
        case 0x7208u: /* ldrb r4,[r12,#0x19] */
            LOAD(r[4],r[12]+25u,1);r[15]=0x720cu;return 0;
        case 0x720cu: /* cmp r2,r4 */
            (void)subtract(state,r[2],r[4]);r[15]=0x7210u;return 0;
        case 0x7210u: /* biceq r0,r0,#0x10 */
            if(state->cpsr&0x40000000u){r[0]=r[0]&~16u;}r[15]=0x7214u;return 0;
        case 0x7214u: /* beq 0x00007290 */
            if(state->cpsr&0x40000000u){r[15]=29328u;}else{r[15]=0x7218u;}return 0;
        case 0x7218u: /* bcc 0x00007230 */
            if(!(state->cpsr&0x20000000u)){r[15]=29232u;}else{r[15]=0x721cu;}return 0;
        case 0x721cu: /* sub r2,r2,r3 */
            r[2]=r[2]-r[3];r[15]=0x7220u;return 0;
        case 0x7220u: /* cmp r2,r4 */
            (void)subtract(state,r[2],r[4]);r[15]=0x7224u;return 0;
        case 0x7224u: /* bhi 0x00007244 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=29252u;}else{r[15]=0x7228u;}return 0;
        case 0x7228u: /* movcc r2,r4 */
            if(!(state->cpsr&0x20000000u)){r[2]=r[4];r[15]=0x722cu;}else{r[15]=0x722cu;}return 0;
        case 0x722cu: /* b 0x0000724c */
            r[15]=29260u;return 0;
        case 0x7230u: /* add r2,r2,r3 */
            r[2]=r[2]+r[3];r[15]=0x7234u;return 0;
        case 0x7234u: /* cmp r2,r4 */
            (void)subtract(state,r[2],r[4]);r[15]=0x7238u;return 0;
        case 0x7238u: /* bcc 0x00007244 */
            if(!(state->cpsr&0x20000000u)){r[15]=29252u;}else{r[15]=0x723cu;}return 0;
        case 0x723cu: /* movhi r2,r4 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[2]=r[4];}r[15]=0x7240u;return 0;
        case 0x7240u: /* b 0x0000724c */
            r[15]=29260u;return 0;
        case 0x7244u: /* ldrb r4,[r12,#0x16] */
            LOAD(r[4],r[12]+22u,1);r[15]=0x7248u;return 0;
        case 0x7248u: /* strb r4,[r12,#0x1d] */
            STORE(r[12]+29u,r[4],1);r[15]=0x724cu;return 0;
        case 0x724cu: /* strb r2,[r12,#0x1f] */
            STORE(r[12]+31u,r[2],1);r[15]=0x7250u;return 0;
        case 0x7250u: /* tst r0,#0x1 */
            temporary=r[0]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7254u;return 0;
        case 0x7254u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7258u;}return 0;
        case 0x7258u: /* mov r2,r2, lsr #0x2 */
            r[2]=(r[2]>>2);r[15]=0x725cu;return 0;
        case 0x725cu: /* sub r2,r2,#0x20 */
            r[2]=r[2]-32u;r[15]=0x7260u;return 0;
        case 0x7260u: /* ldrb r3,[r12,#0x28] */
            LOAD(r[3],r[12]+40u,1);r[15]=0x7264u;return 0;
        case 0x7264u: /* ldr r4,[0x73b4] */
            LOAD(r[4],29620u,4);r[15]=0x7268u;return 0;
        case 0x7268u: /* ldrb r3,[r4,r3] */
            LOAD(r[3],r[4]+r[3],1);r[15]=0x726cu;return 0;
        case 0x726cu: /* add r3,r3,r2 */
            r[3]=r[3]+r[2];r[15]=0x7270u;return 0;
        case 0x7270u: /* cmp r3,#0x0 */
            (void)subtract(state,r[3],0u);r[15]=0x7274u;return 0;
        case 0x7274u: /* movmi r3,#0x0 */
            if(state->cpsr&0x80000000u){r[3]=0u;}r[15]=0x7278u;return 0;
        case 0x7278u: /* cmp r3,#0x1f */
            (void)subtract(state,r[3],31u);r[15]=0x727cu;return 0;
        case 0x727cu: /* movgt r3,#0x1f */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[3]=31u;r[15]=0x7280u;}else{r[15]=0x7280u;}return 0;
        case 0x7280u: /* ldr r4,[0x73b8] */
            LOAD(r[4],29624u,4);r[15]=0x7284u;return 0;
        case 0x7284u: /* ldrb r2,[r4,r3] */
            LOAD(r[2],r[4]+r[3],1);r[15]=0x7288u;return 0;
        case 0x7288u: /* strb r2,[r11,#0x24] */
            STORE(r[11]+36u,r[2],1);r[15]=0x728cu;return 0;
        case 0x728cu: /* strb r2,[r12,#0x21] */
            STORE(r[12]+33u,r[2],1);r[15]=0x7290u;return 0;
        case 0x7290u: /* tst r0,#0x1 */
            temporary=r[0]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7294u;return 0;
        case 0x7294u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7298u;}return 0;
        case 0x7298u: /* ldr r4,[r12,#0xc] */
            LOAD(r[4],r[12]+12u,4);r[15]=0x729cu;return 0;
        case 0x729cu: /* b 0x00007d68 */
            r[15]=32104u;return 0;
        case 0x72e0u: /* mov r3,#0x1e */
            r[3]=30u;r[15]=0x72e4u;return 0;
        case 0x72e4u: /* ldr r2,[r12,#0x30] */
            LOAD(r[2],r[12]+48u,4);r[15]=0x72e8u;return 0;
        case 0x72e8u: /* subs r3,r3,#0x1 */
            r[3]=subtract(state,r[3],1u);r[15]=0x72ecu;return 0;
        case 0x72ecu: /* bne 0x000072e4 */
            if(!(state->cpsr&0x40000000u)){r[15]=29412u;}else{r[15]=0x72f0u;}return 0;
        case 0x72f0u: /* ldr r1,[r9,#0x10] */
            LOAD(r[1],r[9]+16u,4);r[15]=0x72f4u;return 0;
        case 0x72f4u: /* tst r1,#0x8000 */
            temporary=r[1]&32768u;logic_flags(state,temporary,1,32768u);r[15]=0x72f8u;return 0;
        case 0x72f8u: /* beq 0x00007384 */
            if(state->cpsr&0x40000000u){r[15]=29572u;}else{r[15]=0x72fcu;}return 0;
        case 0x72fcu: /* ldr r1,[r11,#0x0] */
            LOAD(r[1],r[11]+0u,4);r[15]=0x7300u;return 0;
        case 0x7300u: /* tst r1,#0x200 */
            temporary=r[1]&512u;logic_flags(state,temporary,1,512u);r[15]=0x7304u;return 0;
        case 0x7304u: /* beq 0x00007320 */
            if(state->cpsr&0x40000000u){r[15]=29472u;}else{r[15]=0x7308u;}return 0;
        case 0x7308u: /* ldr r1,[r12,#0x38] */
            LOAD(r[1],r[12]+56u,4);r[15]=0x730cu;return 0;
        case 0x730cu: /* cmp r1,#0x0 */
            (void)subtract(state,r[1],0u);r[15]=0x7310u;return 0;
        case 0x7310u: /* beq 0x00007378 */
            if(state->cpsr&0x40000000u){r[15]=29560u;}else{r[15]=0x7314u;}return 0;
        case 0x7314u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x7318u;return 0;
        case 0x7318u: /* str r1,[r12,#0x38] */
            STORE(r[12]+56u,r[1],4);r[15]=0x731cu;return 0;
        case 0x731cu: /* bne 0x00007378 */
            if(!(state->cpsr&0x40000000u)){r[15]=29560u;}else{r[15]=0x7320u;}return 0;
        case 0x7320u: /* ldr r1,[r11,#0x0] */
            LOAD(r[1],r[11]+0u,4);r[15]=0x7324u;return 0;
        case 0x7324u: /* orr r1,r1,#0x8000 */
            r[1]=r[1]|32768u;r[15]=0x7328u;return 0;
        case 0x7328u: /* bic r1,r1,#0x4000 */
            r[1]=r[1]&~16384u;r[15]=0x732cu;return 0;
        case 0x732cu: /* str r1,[r11,#0x0] */
            STORE(r[11]+0u,r[1],4);r[15]=0x7330u;return 0;
        case 0x7330u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7334u;return 0;
        case 0x7334u: /* ldrb r1,[r12,#0x23] */
            LOAD(r[1],r[12]+35u,1);r[15]=0x7338u;return 0;
        case 0x7338u: /* ldrb r7,[r12,#0x29] */
            LOAD(r[7],r[12]+41u,1);r[15]=0x733cu;return 0;
        case 0x733cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7340u;return 0;
        case 0x7340u: /* bl 0x00008180 */
            r[14]=0x7344u;r[15]=33152u;return 0;
        case 0x7344u: /* strb r1,[r12,#0x23] */
            STORE(r[12]+35u,r[1],1);r[15]=0x7348u;return 0;
        case 0x7348u: /* strb r7,[r12,#0x29] */
            STORE(r[12]+41u,r[7],1);r[15]=0x734cu;return 0;
        case 0x734cu: /* bic r0,r0,#0x1 */
            r[0]=r[0]&~1u;r[15]=0x7350u;return 0;
        case 0x7350u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x7354u;return 0;
        case 0x7354u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7358u: /* mov r3,#0x1e */
            r[3]=30u;r[15]=0x735cu;return 0;
        case 0x735cu: /* ldr r2,[r12,#0x30] */
            LOAD(r[2],r[12]+48u,4);r[15]=0x7360u;return 0;
        case 0x7360u: /* subs r3,r3,#0x1 */
            r[3]=subtract(state,r[3],1u);r[15]=0x7364u;return 0;
        case 0x7364u: /* bne 0x0000735c */
            if(!(state->cpsr&0x40000000u)){r[15]=29532u;}else{r[15]=0x7368u;}return 0;
        case 0x7368u: /* ldr r1,[r9,#0x10] */
            LOAD(r[1],r[9]+16u,4);r[15]=0x736cu;return 0;
        case 0x736cu: /* movs r1,r1, lsr #0xf */
            temporary=r[1];r[1]=temporary>>15;logic_flags(state,r[1],0,0);state->cpsr=(state->cpsr&~0x20000000u)|(((temporary>>14)&1u)<<29);r[15]=0x7370u;return 0;
        case 0x7370u: /* strb r1,[r12,#0x23] */
            STORE(r[12]+35u,r[1],1);r[15]=0x7374u;return 0;
        case 0x7374u: /* beq 0x00007384 */
            if(state->cpsr&0x40000000u){r[15]=29572u;}else{r[15]=0x7378u;}return 0;
        case 0x7378u: /* ldr r1,[r12,#0x2c] */
            LOAD(r[1],r[12]+44u,4);r[15]=0x737cu;return 0;
        case 0x737cu: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x7380u;return 0;
        case 0x7380u: /* str r1,[r12,#0x2c] */
            STORE(r[12]+44u,r[1],4);r[15]=0x7384u;return 0;
        case 0x7384u: /* ldr r3,[r9,#0x14] */
            LOAD(r[3],r[9]+20u,4);r[15]=0x7388u;return 0;
        case 0x7388u: /* cmp r3,#0x0 */
            (void)subtract(state,r[3],0u);r[15]=0x738cu;return 0;
        case 0x738cu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7390u;}return 0;
        case 0x7390u: /* ldrb r1,[r12,#0x27] */
            LOAD(r[1],r[12]+39u,1);r[15]=0x7394u;return 0;
        case 0x7394u: /* cmp r1,#0x1 */
            (void)subtract(state,r[1],1u);r[15]=0x7398u;return 0;
        case 0x7398u: /* movgt r1,r3, lsl #0x1 */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[1]=(r[3]<<1);r[15]=0x739cu;}else{r[15]=0x739cu;}return 0;
        case 0x739cu: /* movlt r1,r3, lsr #0x1 */
            if((((state->cpsr>>31)^(state->cpsr>>28))&1u)!=0){r[1]=(r[3]>>1);r[15]=0x73a0u;}else{r[15]=0x73a0u;}return 0;
        case 0x73a0u: /* add r2,r1,r2 */
            r[2]=r[1]+r[2];r[15]=0x73a4u;return 0;
        case 0x73a4u: /* str r2,[r12,#0x8] */
            STORE(r[12]+8u,r[2],4);r[15]=0x73a8u;return 0;
        case 0x73a8u: /* str r3,[r12,#0x4] */
            STORE(r[12]+4u,r[3],4);r[15]=0x73acu;return 0;
        case 0x73acu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7d68u: /* mov r6,r4, lsr #0xb */
            r[6]=(r[4]>>11);r[15]=0x7d6cu;return 0;
        case 0x7d6cu: /* ldr r7,[0x8258] */
            LOAD(r[7],33368u,4);r[15]=0x7d70u;return 0;
        case 0x7d70u: /* ldrb r6,[r7,r6] */
            LOAD(r[6],r[7]+r[6],1);r[15]=0x7d74u;return 0;
        case 0x7d74u: /* ldr r8,[0x77f4] */
            LOAD(r[8],30708u,4);r[15]=0x7d78u;return 0;
        case 0x7d78u: /* and r8,r4,r8 */
            r[8]=r[4]&r[8];r[15]=0x7d7cu;return 0;
        case 0x7d7cu: /* orr r8,r8,r6, lsl #0xa */
            r[8]=r[8]|(r[6]<<10);r[15]=0x7d80u;return 0;
        case 0x7d80u: /* ldr r6,[r12,#0x10] */
            LOAD(r[6],r[12]+16u,4);r[15]=0x7d84u;return 0;
        case 0x7d84u: /* add r6,r6,r8 */
            r[6]=r[6]+r[8];r[15]=0x7d88u;return 0;
        case 0x7d88u: /* ldr r8,[0x77f4] */
            LOAD(r[8],30708u,4);r[15]=0x7d8cu;return 0;
        case 0x7d8cu: /* and r8,r6,r8 */
            r[8]=r[6]&r[8];r[15]=0x7d90u;return 0;
        case 0x7d90u: /* mov r6,r6, lsr #0xa */
            r[6]=(r[6]>>10);r[15]=0x7d94u;return 0;
        case 0x7d94u: /* cmp r6,#0x0 */
            (void)subtract(state,r[6],0u);r[15]=0x7d98u;return 0;
        case 0x7d98u: /* movlt r6,#0x0 */
            if((((state->cpsr>>31)^(state->cpsr>>28))&1u)!=0){r[6]=0u;r[15]=0x7d9cu;}else{r[15]=0x7d9cu;}return 0;
        case 0x7d9cu: /* cmp r6,#0xf */
            (void)subtract(state,r[6],15u);r[15]=0x7da0u;return 0;
        case 0x7da0u: /* movgt r6,#0xf */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[6]=15u;r[15]=0x7da4u;}else{r[15]=0x7da4u;}return 0;
        case 0x7da4u: /* ldrb r6,[r7,r6] */
            LOAD(r[6],r[7]+r[6],1);r[15]=0x7da8u;return 0;
        case 0x7da8u: /* orr r6,r8,r6, lsl #0xb */
            r[6]=r[8]|(r[6]<<11);r[15]=0x7dacu;return 0;
        case 0x7dacu: /* str r6,[r11,#0x18] */
            STORE(r[11]+24u,r[6],4);r[15]=0x7db0u;return 0;
        case 0x7db0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x8180u: /* str r6,[r12,#0x3c] */
            STORE(r[12]+60u,r[6],4);r[15]=0x8184u;return 0;
        case 0x8184u: /* str r6,[r12,#0x38] */
            STORE(r[12]+56u,r[6],4);r[15]=0x8188u;return 0;
        case 0x8188u: /* str r6,[r12,#0x30] */
            STORE(r[12]+48u,r[6],4);r[15]=0x818cu;return 0;
        case 0x818cu: /* str r6,[r10,#0x4] */
            STORE(r[10]+4u,r[6],4);r[15]=0x8190u;return 0;
        case 0x8190u: /* str r6,[r10,#0x0] */
            STORE(r[10]+0u,r[6],4);r[15]=0x8194u;return 0;
        case 0x8194u: /* str r6,[r12,#0x28] */
            STORE(r[12]+40u,r[6],4);r[15]=0x8198u;return 0;
        case 0x8198u: /* strb r6,[r12,#0x3] */
            STORE(r[12]+3u,r[6],1);r[15]=0x819cu;return 0;
        case 0x819cu: /* strb r6,[r12,#0x2] */
            STORE(r[12]+2u,r[6],1);r[15]=0x81a0u;return 0;
        case 0x81a0u: /* str r6,[r12,#0xc] */
            STORE(r[12]+12u,r[6],4);r[15]=0x81a4u;return 0;
        case 0x81a4u: /* strb r6,[r12,#0x20] */
            STORE(r[12]+32u,r[6],1);r[15]=0x81a8u;return 0;
        case 0x81a8u: /* strb r6,[r12,#0x21] */
            STORE(r[12]+33u,r[6],1);r[15]=0x81acu;return 0;
        case 0x81acu: /* strb r6,[r12,#0x1d] */
            STORE(r[12]+29u,r[6],1);r[15]=0x81b0u;return 0;
        case 0x81b0u: /* strb r6,[r12,#0x1c] */
            STORE(r[12]+28u,r[6],1);r[15]=0x81b4u;return 0;
        case 0x81b4u: /* str r6,[r12,#0x24] */
            STORE(r[12]+36u,r[6],4);r[15]=0x81b8u;return 0;
        case 0x81b8u: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}
}

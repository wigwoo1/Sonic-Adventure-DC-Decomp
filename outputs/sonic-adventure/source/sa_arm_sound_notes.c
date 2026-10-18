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
int sa_arm_sound_note_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
        case 0xad4u: /* mov r0,r1, lsr #0x8 */
            r[0]=(r[1]>>8);r[15]=0xad8u;return 0;
        case 0xad8u: /* and r0,r0,#0x7f */
            r[0]=r[0]&127u;r[15]=0xadcu;return 0;
        case 0xadcu: /* ands r1,r1,#0x7f */
            temporary=r[1]&127u;r[1]=temporary;logic_flags(state,temporary,0,127u);r[15]=0xae0u;return 0;
        case 0xae0u: /* beq 0x00000a40 */
            if(state->cpsr&0x40000000u){r[15]=2624u;}else{r[15]=0xae4u;}return 0;
        case 0xae4u: /* add r5,r5,r6 */
            r[5]=r[5]+r[6];r[15]=0xae8u;return 0;
        case 0xae8u: /* and r3,r3,#0xf */
            r[3]=r[3]&15u;r[15]=0xaecu;return 0;
        case 0xaecu: /* strb r3,[0x1204] */
            STORE(4612u,r[3],1);r[15]=0xaf0u;return 0;
        case 0xaf0u: /* strb r4,[0x1205] */
            STORE(4613u,r[4],1);r[15]=0xaf4u;return 0;
        case 0xaf4u: /* strb r2,[0x1206] */
            STORE(4614u,r[2],1);r[15]=0xaf8u;return 0;
        case 0xaf8u: /* strb r0,[0x1207] */
            STORE(4615u,r[0],1);r[15]=0xafcu;return 0;
        case 0xafcu: /* strb r1,[0x1208] */
            STORE(4616u,r[1],1);r[15]=0xb00u;return 0;
        case 0xb00u: /* mov r4,#0xc000 */
            r[4]=49152u;r[15]=0xb04u;return 0;
        case 0xb04u: /* mov r0,#0x80 */
            r[0]=128u;r[15]=0xb08u;return 0;
        case 0xb08u: /* mla r0,r3,r0,r4 */
            r[0]=r[3]*r[0]+r[4];r[15]=0xb0cu;return 0;
        case 0xb0cu: /* str r0,[0x1214] */
            STORE(4628u,r[0],4);r[15]=0xb10u;return 0;
        case 0xb10u: /* ldr r0,[r5,#0x14] */
            LOAD(r[0],r[5]+20u,4);r[15]=0xb14u;return 0;
        case 0xb14u: /* str r0,[0x1218] */
            STORE(4632u,r[0],4);r[15]=0xb18u;return 0;
        case 0xb18u: /* mov r12,#0x4 */
            r[12]=4u;r[15]=0xb1cu;return 0;
        case 0xb1cu: /* ldr r1,[0x1218] */
            LOAD(r[1],4632u,4);r[15]=0xb20u;return 0;
        case 0xb20u: /* ldr r0,[r1,#0x0] */
            LOAD(r[0],r[1]+0u,4);r[15]=0xb24u;return 0;
        case 0xb24u: /* movs r0,r0 */
            r[0]=r[0];logic_flags(state,r[0],0,0);r[15]=0xb28u;return 0;
        case 0xb28u: /* beq 0x000011ec */
            if(state->cpsr&0x40000000u){r[15]=4588u;}else{r[15]=0xb2cu;}return 0;
        case 0xb2cu: /* ldr r1,[r5,#0x10] */
            LOAD(r[1],r[5]+16u,4);r[15]=0xb30u;return 0;
        case 0xb30u: /* add r0,r0,r1 */
            r[0]=r[0]+r[1];r[15]=0xb34u;return 0;
        case 0xb34u: /* ldr r1,[r0,#0x8] */
            LOAD(r[1],r[0]+8u,4);r[15]=0xb38u;return 0;
        case 0xb38u: /* str r1,[0x120c] */
            STORE(4620u,r[1],4);r[15]=0xb3cu;return 0;
        case 0xb3cu: /* ldrb r1,[r0,#0xc] */
            LOAD(r[1],r[0]+12u,1);r[15]=0xb40u;return 0;
        case 0xb40u: /* strb r1,[0x1210] */
            STORE(4624u,r[1],1);r[15]=0xb44u;return 0;
        case 0xb44u: /* ldrb r1,[r0,#0xd] */
            LOAD(r[1],r[0]+13u,1);r[15]=0xb48u;return 0;
        case 0xb48u: /* strb r1,[0x1211] */
            STORE(4625u,r[1],1);r[15]=0xb4cu;return 0;
        case 0xb4cu: /* ldrb r1,[r0,#0xe] */
            LOAD(r[1],r[0]+14u,1);r[15]=0xb50u;return 0;
        case 0xb50u: /* strb r1,[0x1212] */
            STORE(4626u,r[1],1);r[15]=0xb54u;return 0;
        case 0xb54u: /* ldrb r1,[r0,#0xf] */
            LOAD(r[1],r[0]+15u,1);r[15]=0xb58u;return 0;
        case 0xb58u: /* strb r1,[0x1213] */
            STORE(4627u,r[1],1);r[15]=0xb5cu;return 0;
        case 0xb5cu: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0xb60u;return 0;
        case 0xb60u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0xb64u;return 0;
        case 0xb64u: /* bne 0x000011ec */
            if(!(state->cpsr&0x40000000u)){r[15]=4588u;}else{r[15]=0xb68u;}return 0;
        case 0xb68u: /* and r11,r1,#0x7f */
            r[11]=r[1]&127u;r[15]=0xb6cu;return 0;
        case 0xb6cu: /* ldr r2,[r0,#0x4] */
            LOAD(r[2],r[0]+4u,4);r[15]=0xb70u;return 0;
        case 0xb70u: /* ldr r3,[r5,#0x10] */
            LOAD(r[3],r[5]+16u,4);r[15]=0xb74u;return 0;
        case 0xb74u: /* add r2,r2,r3 */
            r[2]=r[2]+r[3];r[15]=0xb78u;return 0;
        case 0xb78u: /* ldrb r1,[r2,#0x24] */
            LOAD(r[1],r[2]+36u,1);r[15]=0xb7cu;return 0;
        case 0xb7cu: /* ldrb r3,[0x1207] */
            LOAD(r[3],4615u,1);r[15]=0xb80u;return 0;
        case 0xb80u: /* cmp r3,r1 */
            (void)subtract(state,r[3],r[1]);r[15]=0xb84u;return 0;
        case 0xb84u: /* bmi 0x000011e0 */
            if(state->cpsr&0x80000000u){r[15]=4576u;}else{r[15]=0xb88u;}return 0;
        case 0xb88u: /* ldrb r1,[r2,#0x25] */
            LOAD(r[1],r[2]+37u,1);r[15]=0xb8cu;return 0;
        case 0xb8cu: /* cmp r1,r3 */
            (void)subtract(state,r[1],r[3]);r[15]=0xb90u;return 0;
        case 0xb90u: /* bmi 0x000011e0 */
            if(state->cpsr&0x80000000u){r[15]=4576u;}else{r[15]=0xb94u;}return 0;
        case 0xb94u: /* ldrb r1,[r2,#0x2b] */
            LOAD(r[1],r[2]+43u,1);r[15]=0xb98u;return 0;
        case 0xb98u: /* ldrb r3,[0x1208] */
            LOAD(r[3],4616u,1);r[15]=0xb9cu;return 0;
        case 0xb9cu: /* cmp r3,r1 */
            (void)subtract(state,r[3],r[1]);r[15]=0xba0u;return 0;
        case 0xba0u: /* bmi 0x000011e0 */
            if(state->cpsr&0x80000000u){r[15]=4576u;}else{r[15]=0xba4u;}return 0;
        case 0xba4u: /* ldrb r1,[r2,#0x2c] */
            LOAD(r[1],r[2]+44u,1);r[15]=0xba8u;return 0;
        case 0xba8u: /* cmp r1,r3 */
            (void)subtract(state,r[1],r[3]);r[15]=0xbacu;return 0;
        case 0xbacu: /* bmi 0x000011e0 */
            if(state->cpsr&0x80000000u){r[15]=4576u;}else{r[15]=0xbb0u;}return 0;
        case 0xbb0u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0xbb4u;return 0;
        case 0xbb4u: /* bl 0x00001244 */
            r[14]=0xbb8u;r[15]=4676u;return 0;
        case 0xbb8u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0xbbcu;return 0;
        case 0xbbcu: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0xbc0u;return 0;
        case 0xbc0u: /* bne 0x000011e0 */
            if(!(state->cpsr&0x40000000u)){r[15]=4576u;}else{r[15]=0xbc4u;}return 0;
        case 0xbc4u: /* mov r0,#0x8000 */
            r[0]=32768u;r[15]=0xbc8u;return 0;
        case 0xbc8u: /* str r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],4);r[15]=0xbccu;return 0;
        case 0xbccu: /* ldr r0,[0x120c] */
            LOAD(r[0],4620u,4);r[15]=0xbd0u;return 0;
        case 0xbd0u: /* str r0,[r6,#0x38] */
            STORE(r[6]+56u,r[0],4);r[15]=0xbd4u;return 0;
        case 0xbd4u: /* str r2,[r6,#0x10] */
            STORE(r[6]+16u,r[2],4);r[15]=0xbd8u;return 0;
        case 0xbd8u: /* ldrb r0,[r5,#0x3] */
            LOAD(r[0],r[5]+3u,1);r[15]=0xbdcu;return 0;
        case 0xbdcu: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0xbe0u;return 0;
        case 0xbe0u: /* andne r0,r0,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[0]=r[0]&127u;}r[15]=0xbe4u;return 0;
        case 0xbe4u: /* movne r1,r0 */
            if(!(state->cpsr&0x40000000u)){r[1]=r[0];}r[15]=0xbe8u;return 0;
        case 0xbe8u: /* ldrbeq r0,[0x1210] */
            if(state->cpsr&0x40000000u){LOAD(r[0],4624u,1);}r[15]=0xbecu;return 0;
        case 0xbecu: /* ldrbeq r1,[0x1211] */
            if(state->cpsr&0x40000000u){LOAD(r[1],4625u,1);}r[15]=0xbf0u;return 0;
        case 0xbf0u: /* strb r0,[r6,#0xa] */
            STORE(r[6]+10u,r[0],1);r[15]=0xbf4u;return 0;
        case 0xbf4u: /* strb r1,[r6,#0xb] */
            STORE(r[6]+11u,r[1],1);r[15]=0xbf8u;return 0;
        case 0xbf8u: /* ldrb r0,[0x1207] */
            LOAD(r[0],4615u,1);r[15]=0xbfcu;return 0;
        case 0xbfcu: /* strb r0,[r6,#0x6] */
            STORE(r[6]+6u,r[0],1);r[15]=0xc00u;return 0;
        case 0xc00u: /* ldr r0,[r2,#0x0] */
            LOAD(r[0],r[2]+0u,4);r[15]=0xc04u;return 0;
        case 0xc04u: /* ldr r1,[0x121c] */
            LOAD(r[1],4636u,4);r[15]=0xc08u;return 0;
        case 0xc08u: /* mov r3,r0, ror #0x10 */
            r[3]=((r[0]>>16)|(r[0]<<16));r[15]=0xc0cu;return 0;
        case 0xc0cu: /* and r3,r3,r1 */
            r[3]=r[3]&r[1];r[15]=0xc10u;return 0;
        case 0xc10u: /* ldr r1,[r5,#0x10] */
            LOAD(r[1],r[5]+16u,4);r[15]=0xc14u;return 0;
        case 0xc14u: /* add r3,r3,r1 */
            r[3]=r[3]+r[1];r[15]=0xc18u;return 0;
        case 0xc18u: /* ldr r1,[0x122c] */
            LOAD(r[1],4652u,4);r[15]=0xc1cu;return 0;
        case 0xc1cu: /* and r1,r3,r1 */
            r[1]=r[3]&r[1];r[15]=0xc20u;return 0;
        case 0xc20u: /* str r1,[r8,#0x4] */
            STORE(r[8]+4u,r[1],4);r[15]=0xc24u;return 0;
        case 0xc24u: /* mov r1,r3, ror #0x10 */
            r[1]=((r[3]>>16)|(r[3]<<16));r[15]=0xc28u;return 0;
        case 0xc28u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0xc2cu;return 0;
        case 0xc2cu: /* and r0,r0,#0x780 */
            r[0]=r[0]&1920u;r[15]=0xc30u;return 0;
        case 0xc30u: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0xc34u;return 0;
        case 0xc34u: /* str r0,[0x1224] */
            STORE(4644u,r[0],4);r[15]=0xc38u;return 0;
        case 0xc38u: /* ldr r0,[r2,#0x4] */
            LOAD(r[0],r[2]+4u,4);r[15]=0xc3cu;return 0;
        case 0xc3cu: /* ldr r1,[0x122c] */
            LOAD(r[1],4652u,4);r[15]=0xc40u;return 0;
        case 0xc40u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0xc44u;return 0;
        case 0xc44u: /* str r1,[r8,#0x8] */
            STORE(r[8]+8u,r[1],4);r[15]=0xc48u;return 0;
        case 0xc48u: /* mov r0,r0, ror #0x10 */
            r[0]=((r[0]>>16)|(r[0]<<16));r[15]=0xc4cu;return 0;
        case 0xc4cu: /* ldr r1,[0x122c] */
            LOAD(r[1],4652u,4);r[15]=0xc50u;return 0;
        case 0xc50u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0xc54u;return 0;
        case 0xc54u: /* str r1,[r8,#0xc] */
            STORE(r[8]+12u,r[1],4);r[15]=0xc58u;return 0;
        case 0xc58u: /* ldr r0,[r2,#0x8] */
            LOAD(r[0],r[2]+8u,4);r[15]=0xc5cu;return 0;
        case 0xc5cu: /* and r1,r0,#0x1f */
            r[1]=r[0]&31u;r[15]=0xc60u;return 0;
        case 0xc60u: /* ldrb r3,[r5,#0x37] */
            LOAD(r[3],r[5]+55u,1);r[15]=0xc64u;return 0;
        case 0xc64u: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xc68u;return 0;
        case 0xc68u: /* adds r1,r1,r3 */
            r[1]=addition(state,r[1],r[3]);r[15]=0xc6cu;return 0;
        case 0xc6cu: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xc70u;return 0;
        case 0xc70u: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0xc74u;return 0;
        case 0xc74u: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0xc78u;return 0;
        case 0xc78u: /* str r1,[0x1240] */
            STORE(4672u,r[1],4);r[15]=0xc7cu;return 0;
        case 0xc7cu: /* mov r1,r0, lsr #0x6 */
            r[1]=(r[0]>>6);r[15]=0xc80u;return 0;
        case 0xc80u: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0xc84u;return 0;
        case 0xc84u: /* ldrb r3,[r5,#0x38] */
            LOAD(r[3],r[5]+56u,1);r[15]=0xc88u;return 0;
        case 0xc88u: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xc8cu;return 0;
        case 0xc8cu: /* adds r1,r1,r3 */
            r[1]=addition(state,r[1],r[3]);r[15]=0xc90u;return 0;
        case 0xc90u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xc94u;return 0;
        case 0xc94u: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0xc98u;return 0;
        case 0xc98u: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0xc9cu;return 0;
        case 0xc9cu: /* mov r0,r0, lsr #0xb */
            r[0]=(r[0]>>11);r[15]=0xca0u;return 0;
        case 0xca0u: /* and r0,r0,#0x1f */
            r[0]=r[0]&31u;r[15]=0xca4u;return 0;
        case 0xca4u: /* ldrb r3,[r5,#0x38] */
            LOAD(r[3],r[5]+56u,1);r[15]=0xca8u;return 0;
        case 0xca8u: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xcacu;return 0;
        case 0xcacu: /* adds r0,r0,r3 */
            r[0]=addition(state,r[0],r[3]);r[15]=0xcb0u;return 0;
        case 0xcb0u: /* movmi r0,#0x0 */
            if(state->cpsr&0x80000000u){r[0]=0u;}r[15]=0xcb4u;return 0;
        case 0xcb4u: /* tst r0,#0xe0 */
            temporary=r[0]&224u;logic_flags(state,temporary,0,224u);r[15]=0xcb8u;return 0;
        case 0xcb8u: /* movne r0,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[0]=31u;}r[15]=0xcbcu;return 0;
        case 0xcbcu: /* orr r0,r1,r0, lsl #0x5 */
            r[0]=r[1]|(r[0]<<5);r[15]=0xcc0u;return 0;
        case 0xcc0u: /* ldr r1,[0x1240] */
            LOAD(r[1],4672u,4);r[15]=0xcc4u;return 0;
        case 0xcc4u: /* orr r1,r1,r0, lsl #0x6 */
            r[1]=r[1]|(r[0]<<6);r[15]=0xcc8u;return 0;
        case 0xcc8u: /* str r1,[r8,#0x10] */
            STORE(r[8]+16u,r[1],4);r[15]=0xcccu;return 0;
        case 0xcccu: /* ldr r0,[r2,#0x8] */
            LOAD(r[0],r[2]+8u,4);r[15]=0xcd0u;return 0;
        case 0xcd0u: /* mov r0,r0, ror #0x10 */
            r[0]=((r[0]>>16)|(r[0]<<16));r[15]=0xcd4u;return 0;
        case 0xcd4u: /* and r1,r0,#0x1f */
            r[1]=r[0]&31u;r[15]=0xcd8u;return 0;
        case 0xcd8u: /* ldrb r3,[r5,#0x3b] */
            LOAD(r[3],r[5]+59u,1);r[15]=0xcdcu;return 0;
        case 0xcdcu: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xce0u;return 0;
        case 0xce0u: /* adds r1,r1,r3 */
            r[1]=addition(state,r[1],r[3]);r[15]=0xce4u;return 0;
        case 0xce4u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xce8u;return 0;
        case 0xce8u: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0xcecu;return 0;
        case 0xcecu: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0xcf0u;return 0;
        case 0xcf0u: /* str r1,[0x1240] */
            STORE(4672u,r[1],4);r[15]=0xcf4u;return 0;
        case 0xcf4u: /* mov r1,r0, lsr #0x5 */
            r[1]=(r[0]>>5);r[15]=0xcf8u;return 0;
        case 0xcf8u: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0xcfcu;return 0;
        case 0xcfcu: /* ldrb r3,[r5,#0x39] */
            LOAD(r[3],r[5]+57u,1);r[15]=0xd00u;return 0;
        case 0xd00u: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xd04u;return 0;
        case 0xd04u: /* adds r1,r1,r3 */
            r[1]=addition(state,r[1],r[3]);r[15]=0xd08u;return 0;
        case 0xd08u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xd0cu;return 0;
        case 0xd0cu: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0xd10u;return 0;
        case 0xd10u: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0xd14u;return 0;
        case 0xd14u: /* and r0,r0,#0x7c00 */
            r[0]=r[0]&31744u;r[15]=0xd18u;return 0;
        case 0xd18u: /* orr r1,r0,r1, lsl #0x5 */
            r[1]=r[0]|(r[1]<<5);r[15]=0xd1cu;return 0;
        case 0xd1cu: /* ldr r0,[0x1240] */
            LOAD(r[0],4672u,4);r[15]=0xd20u;return 0;
        case 0xd20u: /* orr r1,r0,r1 */
            r[1]=r[0]|r[1];r[15]=0xd24u;return 0;
        case 0xd24u: /* str r1,[r8,#0x14] */
            STORE(r[8]+20u,r[1],4);r[15]=0xd28u;return 0;
        case 0xd28u: /* ldr r0,[r2,#0xc] */
            LOAD(r[0],r[2]+12u,4);r[15]=0xd2cu;return 0;
        case 0xd2cu: /* mov r0,r0, ror #0x10 */
            r[0]=((r[0]>>16)|(r[0]<<16));r[15]=0xd30u;return 0;
        case 0xd30u: /* ldrb r1,[r5,#0x2] */
            LOAD(r[1],r[5]+2u,1);r[15]=0xd34u;return 0;
        case 0xd34u: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0xd38u;return 0;
        case 0xd38u: /* mov r3,r0, lsr #0x5 */
            r[3]=(r[0]>>5);r[15]=0xd3cu;return 0;
        case 0xd3cu: /* and r3,r3,#0x7 */
            r[3]=r[3]&7u;r[15]=0xd40u;return 0;
        case 0xd40u: /* add r3,r3,#0x1 */
            r[3]=r[3]+1u;r[15]=0xd44u;return 0;
        case 0xd44u: /* mul r1,r3,r1 */
            r[1]=r[3]*r[1];r[15]=0xd48u;return 0;
        case 0xd48u: /* sub r1,r1,#0x1 */
            r[1]=r[1]-1u;r[15]=0xd4cu;return 0;
        case 0xd4cu: /* mov r1,r1, lsr #0x2 */
            r[1]=(r[1]>>2);r[15]=0xd50u;return 0;
        case 0xd50u: /* and r1,r1,#0xe0 */
            r[1]=r[1]&224u;r[15]=0xd54u;return 0;
        case 0xd54u: /* ldr r3,[0x1238] */
            LOAD(r[3],4664u,4);r[15]=0xd58u;return 0;
        case 0xd58u: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0xd5cu;return 0;
        case 0xd5cu: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0xd60u;return 0;
        case 0xd60u: /* ldrb r1,[r5,#0x2] */
            LOAD(r[1],r[5]+2u,1);r[15]=0xd64u;return 0;
        case 0xd64u: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0xd68u;return 0;
        case 0xd68u: /* and r3,r0,#0x7 */
            r[3]=r[0]&7u;r[15]=0xd6cu;return 0;
        case 0xd6cu: /* add r3,r3,#0x1 */
            r[3]=r[3]+1u;r[15]=0xd70u;return 0;
        case 0xd70u: /* mul r1,r3,r1 */
            r[1]=r[3]*r[1];r[15]=0xd74u;return 0;
        case 0xd74u: /* sub r1,r1,#0x1 */
            r[1]=r[1]-1u;r[15]=0xd78u;return 0;
        case 0xd78u: /* mov r1,r1, lsr #0x7 */
            r[1]=(r[1]>>7);r[15]=0xd7cu;return 0;
        case 0xd7cu: /* and r1,r1,#0x7 */
            r[1]=r[1]&7u;r[15]=0xd80u;return 0;
        case 0xd80u: /* ldr r3,[0x123c] */
            LOAD(r[3],4668u,4);r[15]=0xd84u;return 0;
        case 0xd84u: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0xd88u;return 0;
        case 0xd88u: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0xd8cu;return 0;
        case 0xd8cu: /* str r0,[r8,#0x1c] */
            STORE(r[8]+28u,r[0],4);r[15]=0xd90u;return 0;
        case 0xd90u: /* ldr r0,[r2,#0x10] */
            LOAD(r[0],r[2]+16u,4);r[15]=0xd94u;return 0;
        case 0xd94u: /* and r1,r0,#0xf */
            r[1]=r[0]&15u;r[15]=0xd98u;return 0;
        case 0xd98u: /* ldr r3,[0x1214] */
            LOAD(r[3],4628u,4);r[15]=0xd9cu;return 0;
        case 0xd9cu: /* ldrb r3,[r3,#0x26] */
            LOAD(r[3],r[3]+38u,1);r[15]=0xda0u;return 0;
        case 0xda0u: /* mov r3,r3, lsr #0x3 */
            r[3]=(r[3]>>3);r[15]=0xda4u;return 0;
        case 0xda4u: /* and r3,r3,#0x1f */
            r[3]=r[3]&31u;r[15]=0xda8u;return 0;
        case 0xda8u: /* sub r3,r3,#0x10 */
            r[3]=r[3]-16u;r[15]=0xdacu;return 0;
        case 0xdacu: /* ldrb r1,[r5,#0x36] */
            LOAD(r[1],r[5]+54u,1);r[15]=0xdb0u;return 0;
        case 0xdb0u: /* movs r0,r1 */
            r[0]=r[1];logic_flags(state,r[0],0,0);r[15]=0xdb4u;return 0;
        case 0xdb4u: /* ldreq r0,[r2,#0x10] */
            if(state->cpsr&0x40000000u){LOAD(r[0],r[2]+16u,4);}r[15]=0xdb8u;return 0;
        case 0xdb8u: /* ldr r1,[r2,#0x10] */
            LOAD(r[1],r[2]+16u,4);r[15]=0xdbcu;return 0;
        case 0xdbcu: /* and r1,r1,#0xf */
            r[1]=r[1]&15u;r[15]=0xdc0u;return 0;
        case 0xdc0u: /* and r0,r0,#0xf0 */
            r[0]=r[0]&240u;r[15]=0xdc4u;return 0;
        case 0xdc4u: /* mov r0,r0, lsr #0x4 */
            r[0]=(r[0]>>4);r[15]=0xdc8u;return 0;
        case 0xdc8u: /* adds r0,r0,r3 */
            r[0]=addition(state,r[0],r[3]);r[15]=0xdccu;return 0;
        case 0xdccu: /* movmi r0,#0x0 */
            if(state->cpsr&0x80000000u){r[0]=0u;}r[15]=0xdd0u;return 0;
        case 0xdd0u: /* tst r0,#0xf0 */
            temporary=r[0]&240u;logic_flags(state,temporary,0,240u);r[15]=0xdd4u;return 0;
        case 0xdd4u: /* movne r0,#0xf */
            if(!(state->cpsr&0x40000000u)){r[0]=15u;}r[15]=0xdd8u;return 0;
        case 0xdd8u: /* and r0,r0,#0xf */
            r[0]=r[0]&15u;r[15]=0xddcu;return 0;
        case 0xddcu: /* orr r1,r1,r0, lsl #0x4 */
            r[1]=r[1]|(r[0]<<4);r[15]=0xde0u;return 0;
        case 0xde0u: /* str r1,[r8,#0x20] */
            STORE(r[8]+32u,r[1],4);r[15]=0xde4u;return 0;
        case 0xde4u: /* ldr r0,[r2,#0x14] */
            LOAD(r[0],r[2]+20u,4);r[15]=0xde8u;return 0;
        case 0xde8u: /* and r1,r0,#0x1f */
            r[1]=r[0]&31u;r[15]=0xdecu;return 0;
        case 0xdecu: /* ldrb r3,[r5,#0x27] */
            LOAD(r[3],r[5]+39u,1);r[15]=0xdf0u;return 0;
        case 0xdf0u: /* sub r3,r3,#0x40 */
            r[3]=r[3]-64u;r[15]=0xdf4u;return 0;
        case 0xdf4u: /* adds r1,r1,r3 */
            r[1]=addition(state,r[1],r[3]);r[15]=0xdf8u;return 0;
        case 0xdf8u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xdfcu;return 0;
        case 0xdfcu: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0xe00u;return 0;
        case 0xe00u: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0xe04u;return 0;
        case 0xe04u: /* ldrb r3,[r5,#0x3d] */
            LOAD(r[3],r[5]+61u,1);r[15]=0xe08u;return 0;
        case 0xe08u: /* tst r3,#0xff */
            temporary=r[3]&255u;logic_flags(state,temporary,0,255u);r[15]=0xe0cu;return 0;
        case 0xe0cu: /* andeq r3,r0,#0x20 */
            if(state->cpsr&0x40000000u){r[3]=r[0]&32u;}r[15]=0xe10u;return 0;
        case 0xe10u: /* beq 0x00000e20 */
            if(state->cpsr&0x40000000u){r[15]=3616u;}else{r[15]=0xe14u;}return 0;
        case 0xe14u: /* tst r3,#0x40 */
            temporary=r[3]&64u;logic_flags(state,temporary,0,64u);r[15]=0xe18u;return 0;
        case 0xe18u: /* movne r3,#0x0 */
            if(!(state->cpsr&0x40000000u)){r[3]=0u;}r[15]=0xe1cu;return 0;
        case 0xe1cu: /* moveq r3,#0x20 */
            if(state->cpsr&0x40000000u){r[3]=32u;}r[15]=0xe20u;return 0;
        case 0xe20u: /* orr r1,r1,r3 */
            r[1]=r[1]|r[3];r[15]=0xe24u;return 0;
        case 0xe24u: /* str r1,[0x1228] */
            STORE(4648u,r[1],4);r[15]=0xe28u;return 0;
        case 0xe28u: /* ldr r4,[r5,#0x40] */
            LOAD(r[4],r[5]+64u,4);r[15]=0xe2cu;return 0;
        case 0xe2cu: /* mov r0,r0, ror #0x10 */
            r[0]=((r[0]>>16)|(r[0]<<16));r[15]=0xe30u;return 0;
        case 0xe30u: /* ldr r3,[r5,#0x28] */
            LOAD(r[3],r[5]+40u,4);r[15]=0xe34u;return 0;
        case 0xe34u: /* ldr r1,[0x122c] */
            LOAD(r[1],4652u,4);r[15]=0xe38u;return 0;
        case 0xe38u: /* and r3,r3,r1 */
            r[3]=r[3]&r[1];r[15]=0xe3cu;return 0;
        case 0xe3cu: /* sub r3,r3,#0x2000 */
            r[3]=r[3]-8192u;r[15]=0xe40u;return 0;
        case 0xe40u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0xe44u;return 0;
        case 0xe44u: /* add r1,r1,r3 */
            r[1]=r[1]+r[3];r[15]=0xe48u;return 0;
        case 0xe48u: /* adds r1,r1,r4 */
            r[1]=addition(state,r[1],r[4]);r[15]=0xe4cu;return 0;
        case 0xe4cu: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0xe50u;return 0;
        case 0xe50u: /* subs r3,r1,#0x8 */
            r[3]=subtract(state,r[1],8u);r[15]=0xe54u;return 0;
        case 0xe54u: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0xe58u;return 0;
        case 0xe58u: /* add r3,r1,#0x8 */
            r[3]=r[1]+8u;r[15]=0xe5cu;return 0;
        case 0xe5cu: /* tst r3,#0xe000 */
            temporary=r[3]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0xe60u;return 0;
        case 0xe60u: /* ldrne r1,[0x1230] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],4656u,4);}r[15]=0xe64u;return 0;
        case 0xe64u: /* str r1,[r8,#0x2c] */
            STORE(r[8]+44u,r[1],4);r[15]=0xe68u;return 0;
        case 0xe68u: /* ldr r0,[r2,#0x18] */
            LOAD(r[0],r[2]+24u,4);r[15]=0xe6cu;return 0;
        case 0xe6cu: /* ldr r3,[r5,#0x28] */
            LOAD(r[3],r[5]+40u,4);r[15]=0xe70u;return 0;
        case 0xe70u: /* ldr r1,[0x122c] */
            LOAD(r[1],4652u,4);r[15]=0xe74u;return 0;
        case 0xe74u: /* mov r3,r3, ror #0x10 */
            r[3]=((r[3]>>16)|(r[3]<<16));r[15]=0xe78u;return 0;
        case 0xe78u: /* and r3,r3,r1 */
            r[3]=r[3]&r[1];r[15]=0xe7cu;return 0;
        case 0xe7cu: /* sub r3,r3,#0x2000 */
            r[3]=r[3]-8192u;r[15]=0xe80u;return 0;
        case 0xe80u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0xe84u;return 0;
        case 0xe84u: /* add r1,r1,r3 */
            r[1]=r[1]+r[3];r[15]=0xe88u;return 0;
        case 0xe88u: /* adds r1,r1,r4 */
            r[1]=addition(state,r[1],r[4]);r[15]=0xe8cu;return 0;
        case 0xe8cu: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0xe90u;return 0;
        case 0xe90u: /* subs r3,r1,#0x8 */
            r[3]=subtract(state,r[1],8u);r[15]=0xe94u;return 0;
        case 0xe94u: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0xe98u;return 0;
        case 0xe98u: /* add r3,r1,#0x8 */
            r[3]=r[1]+8u;r[15]=0xe9cu;return 0;
        case 0xe9cu: /* tst r3,#0xe000 */
            temporary=r[3]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0xea0u;return 0;
        case 0xea0u: /* ldrne r1,[0x1230] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],4656u,4);}r[15]=0xea4u;return 0;
        case 0xea4u: /* str r1,[r8,#0x30] */
            STORE(r[8]+48u,r[1],4);r[15]=0xea8u;return 0;
        case 0xea8u: /* mov r0,r0, ror #0x10 */
            r[0]=((r[0]>>16)|(r[0]<<16));r[15]=0xeacu;return 0;
        case 0xeacu: /* ldr r3,[r5,#0x2c] */
            LOAD(r[3],r[5]+44u,4);r[15]=0xeb0u;return 0;
        case 0xeb0u: /* ldr r1,[0x122c] */
            LOAD(r[1],4652u,4);r[15]=0xeb4u;return 0;
        case 0xeb4u: /* and r3,r3,r1 */
            r[3]=r[3]&r[1];r[15]=0xeb8u;return 0;
        case 0xeb8u: /* sub r3,r3,#0x2000 */
            r[3]=r[3]-8192u;r[15]=0xebcu;return 0;
        case 0xebcu: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0xec0u;return 0;
        case 0xec0u: /* add r1,r1,r3 */
            r[1]=r[1]+r[3];r[15]=0xec4u;return 0;
        case 0xec4u: /* adds r1,r1,r4 */
            r[1]=addition(state,r[1],r[4]);r[15]=0xec8u;return 0;
        case 0xec8u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xeccu;return 0;
        case 0xeccu: /* subs r3,r1,#0x8 */
            r[3]=subtract(state,r[1],8u);r[15]=0xed0u;return 0;
        case 0xed0u: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0xed4u;return 0;
        case 0xed4u: /* add r3,r1,#0x8 */
            r[3]=r[1]+8u;r[15]=0xed8u;return 0;
        case 0xed8u: /* tst r3,#0xe000 */
            temporary=r[3]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0xedcu;return 0;
        case 0xedcu: /* ldrne r1,[0x1230] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],4656u,4);}r[15]=0xee0u;return 0;
        case 0xee0u: /* str r1,[r8,#0x34] */
            STORE(r[8]+52u,r[1],4);r[15]=0xee4u;return 0;
        case 0xee4u: /* ldr r0,[r2,#0x1c] */
            LOAD(r[0],r[2]+28u,4);r[15]=0xee8u;return 0;
        case 0xee8u: /* ldr r3,[r5,#0x2c] */
            LOAD(r[3],r[5]+44u,4);r[15]=0xeecu;return 0;
        case 0xeecu: /* mov r3,r3, ror #0x10 */
            r[3]=((r[3]>>16)|(r[3]<<16));r[15]=0xef0u;return 0;
        case 0xef0u: /* ldr r1,[0x122c] */
            LOAD(r[1],4652u,4);r[15]=0xef4u;return 0;
        case 0xef4u: /* and r3,r3,r1 */
            r[3]=r[3]&r[1];r[15]=0xef8u;return 0;
        case 0xef8u: /* sub r3,r3,#0x2000 */
            r[3]=r[3]-8192u;r[15]=0xefcu;return 0;
        case 0xefcu: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0xf00u;return 0;
        case 0xf00u: /* add r1,r1,r3 */
            r[1]=r[1]+r[3];r[15]=0xf04u;return 0;
        case 0xf04u: /* adds r1,r1,r4 */
            r[1]=addition(state,r[1],r[4]);r[15]=0xf08u;return 0;
        case 0xf08u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xf0cu;return 0;
        case 0xf0cu: /* subs r3,r1,#0x8 */
            r[3]=subtract(state,r[1],8u);r[15]=0xf10u;return 0;
        case 0xf10u: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0xf14u;return 0;
        case 0xf14u: /* add r3,r1,#0x8 */
            r[3]=r[1]+8u;r[15]=0xf18u;return 0;
        case 0xf18u: /* tst r3,#0xe000 */
            temporary=r[3]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0xf1cu;return 0;
        case 0xf1cu: /* ldrne r1,[0x1230] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],4656u,4);}r[15]=0xf20u;return 0;
        case 0xf20u: /* str r1,[r8,#0x38] */
            STORE(r[8]+56u,r[1],4);r[15]=0xf24u;return 0;
        case 0xf24u: /* mov r0,r0, ror #0x10 */
            r[0]=((r[0]>>16)|(r[0]<<16));r[15]=0xf28u;return 0;
        case 0xf28u: /* ldr r3,[r5,#0x30] */
            LOAD(r[3],r[5]+48u,4);r[15]=0xf2cu;return 0;
        case 0xf2cu: /* ldr r1,[0x122c] */
            LOAD(r[1],4652u,4);r[15]=0xf30u;return 0;
        case 0xf30u: /* and r3,r3,r1 */
            r[3]=r[3]&r[1];r[15]=0xf34u;return 0;
        case 0xf34u: /* sub r3,r3,#0x2000 */
            r[3]=r[3]-8192u;r[15]=0xf38u;return 0;
        case 0xf38u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0xf3cu;return 0;
        case 0xf3cu: /* add r1,r1,r3 */
            r[1]=r[1]+r[3];r[15]=0xf40u;return 0;
        case 0xf40u: /* adds r1,r1,r4 */
            r[1]=addition(state,r[1],r[4]);r[15]=0xf44u;return 0;
        case 0xf44u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xf48u;return 0;
        case 0xf48u: /* subs r3,r1,#0x8 */
            r[3]=subtract(state,r[1],8u);r[15]=0xf4cu;return 0;
        case 0xf4cu: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0xf50u;return 0;
        case 0xf50u: /* add r3,r1,#0x8 */
            r[3]=r[1]+8u;r[15]=0xf54u;return 0;
        case 0xf54u: /* tst r3,#0xe000 */
            temporary=r[3]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0xf58u;return 0;
        case 0xf58u: /* ldrne r1,[0x1230] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],4656u,4);}r[15]=0xf5cu;return 0;
        case 0xf5cu: /* str r1,[r8,#0x3c] */
            STORE(r[8]+60u,r[1],4);r[15]=0xf60u;return 0;
        case 0xf60u: /* ldr r0,[r2,#0x20] */
            LOAD(r[0],r[2]+32u,4);r[15]=0xf64u;return 0;
        case 0xf64u: /* ldrb r3,[r5,#0x32] */
            LOAD(r[3],r[5]+50u,1);r[15]=0xf68u;return 0;
        case 0xf68u: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xf6cu;return 0;
        case 0xf6cu: /* mov r1,r0, lsr #0x8 */
            r[1]=(r[0]>>8);r[15]=0xf70u;return 0;
        case 0xf70u: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0xf74u;return 0;
        case 0xf74u: /* adds r1,r1,r3 */
            r[1]=addition(state,r[1],r[3]);r[15]=0xf78u;return 0;
        case 0xf78u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xf7cu;return 0;
        case 0xf7cu: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0xf80u;return 0;
        case 0xf80u: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0xf84u;return 0;
        case 0xf84u: /* and r0,r0,#0x1f */
            r[0]=r[0]&31u;r[15]=0xf88u;return 0;
        case 0xf88u: /* ldrb r3,[r5,#0x33] */
            LOAD(r[3],r[5]+51u,1);r[15]=0xf8cu;return 0;
        case 0xf8cu: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xf90u;return 0;
        case 0xf90u: /* adds r0,r0,r3 */
            r[0]=addition(state,r[0],r[3]);r[15]=0xf94u;return 0;
        case 0xf94u: /* movmi r0,#0x0 */
            if(state->cpsr&0x80000000u){r[0]=0u;}r[15]=0xf98u;return 0;
        case 0xf98u: /* tst r0,#0xe0 */
            temporary=r[0]&224u;logic_flags(state,temporary,0,224u);r[15]=0xf9cu;return 0;
        case 0xf9cu: /* movne r0,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[0]=31u;}r[15]=0xfa0u;return 0;
        case 0xfa0u: /* orr r1,r0,r1, lsl #0x8 */
            r[1]=r[0]|(r[1]<<8);r[15]=0xfa4u;return 0;
        case 0xfa4u: /* str r1,[r8,#0x40] */
            STORE(r[8]+64u,r[1],4);r[15]=0xfa8u;return 0;
        case 0xfa8u: /* ldr r0,[r2,#0x20] */
            LOAD(r[0],r[2]+32u,4);r[15]=0xfacu;return 0;
        case 0xfacu: /* mov r0,r0, ror #0x10 */
            r[0]=((r[0]>>16)|(r[0]<<16));r[15]=0xfb0u;return 0;
        case 0xfb0u: /* ldrb r3,[r5,#0x34] */
            LOAD(r[3],r[5]+52u,1);r[15]=0xfb4u;return 0;
        case 0xfb4u: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xfb8u;return 0;
        case 0xfb8u: /* mov r1,r0, lsr #0x8 */
            r[1]=(r[0]>>8);r[15]=0xfbcu;return 0;
        case 0xfbcu: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0xfc0u;return 0;
        case 0xfc0u: /* adds r1,r1,r3 */
            r[1]=addition(state,r[1],r[3]);r[15]=0xfc4u;return 0;
        case 0xfc4u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xfc8u;return 0;
        case 0xfc8u: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0xfccu;return 0;
        case 0xfccu: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0xfd0u;return 0;
        case 0xfd0u: /* and r0,r0,#0x1f */
            r[0]=r[0]&31u;r[15]=0xfd4u;return 0;
        case 0xfd4u: /* ldrb r3,[r5,#0x35] */
            LOAD(r[3],r[5]+53u,1);r[15]=0xfd8u;return 0;
        case 0xfd8u: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xfdcu;return 0;
        case 0xfdcu: /* adds r0,r0,r3 */
            r[0]=addition(state,r[0],r[3]);r[15]=0xfe0u;return 0;
        case 0xfe0u: /* movmi r0,#0x0 */
            if(state->cpsr&0x80000000u){r[0]=0u;}r[15]=0xfe4u;return 0;
        case 0xfe4u: /* tst r0,#0xe0 */
            temporary=r[0]&224u;logic_flags(state,temporary,0,224u);r[15]=0xfe8u;return 0;
        case 0xfe8u: /* movne r0,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[0]=31u;}r[15]=0xfecu;return 0;
        case 0xfecu: /* orr r1,r0,r1, lsl #0x8 */
            r[1]=r[0]|(r[1]<<8);r[15]=0xff0u;return 0;
        case 0xff0u: /* str r1,[r8,#0x44] */
            STORE(r[8]+68u,r[1],4);r[15]=0xff4u;return 0;
        case 0xff4u: /* ldrb r1,[r2,#0x2a] */
            LOAD(r[1],r[2]+42u,1);r[15]=0xff8u;return 0;
        case 0xff8u: /* strb r1,[r6,#0x1c] */
            STORE(r[6]+28u,r[1],1);r[15]=0xffcu;return 0;
        case 0xffcu: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x1000u;return 0;
        case 0x1000u: /* ldr r0,[r5,#0x18] */
            LOAD(r[0],r[5]+24u,4);r[15]=0x1004u;return 0;
        case 0x1004u: /* add r1,r1,r0 */
            r[1]=r[1]+r[0];r[15]=0x1008u;return 0;
        case 0x1008u: /* ldrb r0,[0x1208] */
            LOAD(r[0],4616u,1);r[15]=0x100cu;return 0;
        case 0x100cu: /* strb r0,[r6,#0x7] */
            STORE(r[6]+7u,r[0],1);r[15]=0x1010u;return 0;
        case 0x1010u: /* ldrb r0,[r1,r0] */
            LOAD(r[0],r[1]+r[0],1);r[15]=0x1014u;return 0;
        case 0x1014u: /* add r0,r0,#0x1 */
            r[0]=r[0]+1u;r[15]=0x1018u;return 0;
        case 0x1018u: /* ldrb r1,[r5,#0xa] */
            LOAD(r[1],r[5]+10u,1);r[15]=0x101cu;return 0;
        case 0x101cu: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x1020u;return 0;
        case 0x1020u: /* mul r0,r1,r0 */
            r[0]=r[1]*r[0];r[15]=0x1024u;return 0;
        case 0x1024u: /* ldrb r1,[r2,#0x15] */
            LOAD(r[1],r[2]+21u,1);r[15]=0x1028u;return 0;
        case 0x1028u: /* eor r1,r1,#0xff */
            r[1]=r[1]^255u;r[15]=0x102cu;return 0;
        case 0x102cu: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x1030u;return 0;
        case 0x1030u: /* mul r0,r1,r0 */
            r[0]=r[1]*r[0];r[15]=0x1034u;return 0;
        case 0x1034u: /* sub r0,r0,#0x1 */
            r[0]=r[0]-1u;r[15]=0x1038u;return 0;
        case 0x1038u: /* mov r0,r0, lsr #0xe */
            r[0]=(r[0]>>14);r[15]=0x103cu;return 0;
        case 0x103cu: /* and r0,r0,#0xff */
            r[0]=r[0]&255u;r[15]=0x1040u;return 0;
        case 0x1040u: /* strb r0,[r6,#0x8] */
            STORE(r[6]+8u,r[0],1);r[15]=0x1044u;return 0;
        case 0x1044u: /* ldr r3,[0x1214] */
            LOAD(r[3],4628u,4);r[15]=0x1048u;return 0;
        case 0x1048u: /* str r3,[r6,#0x18] */
            STORE(r[6]+24u,r[3],4);r[15]=0x104cu;return 0;
        case 0x104cu: /* ldrb r1,[r3,#0x1] */
            LOAD(r[1],r[3]+1u,1);r[15]=0x1050u;return 0;
        case 0x1050u: /* sub r1,r1,#0x80 */
            r[1]=r[1]-128u;r[15]=0x1054u;return 0;
        case 0x1054u: /* adds r1,r1,r0 */
            r[1]=addition(state,r[1],r[0]);r[15]=0x1058u;return 0;
        case 0x1058u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x105cu;return 0;
        case 0x105cu: /* bmi 0x00001068 */
            if(state->cpsr&0x80000000u){r[15]=4200u;}else{r[15]=0x1060u;}return 0;
        case 0x1060u: /* tst r1,#0x100 */
            temporary=r[1]&256u;logic_flags(state,temporary,1,256u);r[15]=0x1064u;return 0;
        case 0x1064u: /* movne r1,#0xff */
            if(!(state->cpsr&0x40000000u)){r[1]=255u;}r[15]=0x1068u;return 0;
        case 0x1068u: /* eor r1,r1,#0xff */
            r[1]=r[1]^255u;r[15]=0x106cu;return 0;
        case 0x106cu: /* and r1,r1,#0xff */
            r[1]=r[1]&255u;r[15]=0x1070u;return 0;
        case 0x1070u: /* ldr r0,[0x1228] */
            LOAD(r[0],4648u,4);r[15]=0x1074u;return 0;
        case 0x1074u: /* and r0,r0,#0x3f */
            r[0]=r[0]&63u;r[15]=0x1078u;return 0;
        case 0x1078u: /* orr r0,r0,r1, lsl #0x8 */
            r[0]=r[0]|(r[1]<<8);r[15]=0x107cu;return 0;
        case 0x107cu: /* str r0,[r8,#0x28] */
            STORE(r[8]+40u,r[0],4);r[15]=0x1080u;return 0;
        case 0x1080u: /* ldr r0,[r2,#0x12] */
            LOAD(r[0],r[2]+18u,4);r[15]=0x1084u;return 0;
        case 0x1084u: /* and r0,r0,#0x1f */
            r[0]=r[0]&31u;r[15]=0x1088u;return 0;
        case 0x1088u: /* cmp r0,#0x10 */
            (void)subtract(state,r[0],16u);r[15]=0x108cu;return 0;
        case 0x108cu: /* moveq r0,#0x0 */
            if(state->cpsr&0x40000000u){r[0]=0u;}r[15]=0x1090u;return 0;
        case 0x1090u: /* tst r0,#0x10 */
            temporary=r[0]&16u;logic_flags(state,temporary,0,16u);r[15]=0x1094u;return 0;
        case 0x1094u: /* subne r0,r0,#0x10 */
            if(!(state->cpsr&0x40000000u)){r[0]=r[0]-16u;}r[15]=0x1098u;return 0;
        case 0x1098u: /* eorne r0,r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=r[0]^255u;}r[15]=0x109cu;return 0;
        case 0x109cu: /* add r0,r0,#0x10 */
            r[0]=r[0]+16u;r[15]=0x10a0u;return 0;
        case 0x10a0u: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x10a4u;return 0;
        case 0x10a4u: /* ldrb r1,[r5,#0xb] */
            LOAD(r[1],r[5]+11u,1);r[15]=0x10a8u;return 0;
        case 0x10a8u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x10acu;return 0;
        case 0x10acu: /* moveq r0,r1 */
            if(state->cpsr&0x40000000u){r[0]=r[1];}r[15]=0x10b0u;return 0;
        case 0x10b0u: /* and r0,r0,#0x7f */
            r[0]=r[0]&127u;r[15]=0x10b4u;return 0;
        case 0x10b4u: /* ldr r3,[0x1214] */
            LOAD(r[3],4628u,4);r[15]=0x10b8u;return 0;
        case 0x10b8u: /* ldrb r1,[r3,#0x7] */
            LOAD(r[1],r[3]+7u,1);r[15]=0x10bcu;return 0;
        case 0x10bcu: /* and r1,r1,#0xff */
            r[1]=r[1]&255u;r[15]=0x10c0u;return 0;
        case 0x10c0u: /* sub r1,r1,#0x80 */
            r[1]=r[1]-128u;r[15]=0x10c4u;return 0;
        case 0x10c4u: /* adds r1,r1,r0 */
            r[1]=addition(state,r[1],r[0]);r[15]=0x10c8u;return 0;
        case 0x10c8u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x10ccu;return 0;
        case 0x10ccu: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x10d0u;return 0;
        case 0x10d0u: /* movne r1,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[1]=127u;}r[15]=0x10d4u;return 0;
        case 0x10d4u: /* mov r1,r1, lsr #0x2 */
            r[1]=(r[1]>>2);r[15]=0x10d8u;return 0;
        case 0x10d8u: /* subs r1,r1,#0x10 */
            r[1]=subtract(state,r[1],16u);r[15]=0x10dcu;return 0;
        case 0x10dcu: /* eormi r1,r1,#0xff */
            if(state->cpsr&0x80000000u){r[1]=r[1]^255u;}r[15]=0x10e0u;return 0;
        case 0x10e0u: /* addmi r1,r1,#0x10 */
            if(state->cpsr&0x80000000u){r[1]=r[1]+16u;}r[15]=0x10e4u;return 0;
        case 0x10e4u: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0x10e8u;return 0;
        case 0x10e8u: /* ldrb r3,[r3,#0x24] */
            LOAD(r[3],r[3]+36u,1);r[15]=0x10ecu;return 0;
        case 0x10ecu: /* mov r3,r3, lsr #0x3 */
            r[3]=(r[3]>>3);r[15]=0x10f0u;return 0;
        case 0x10f0u: /* and r3,r3,#0x1f */
            r[3]=r[3]&31u;r[15]=0x10f4u;return 0;
        case 0x10f4u: /* sub r3,r3,#0x10 */
            r[3]=r[3]-16u;r[15]=0x10f8u;return 0;
        case 0x10f8u: /* ldr r0,[r2,#0x12] */
            LOAD(r[0],r[2]+18u,4);r[15]=0x10fcu;return 0;
        case 0x10fcu: /* mov r0,r0, lsr #0x8 */
            r[0]=(r[0]>>8);r[15]=0x1100u;return 0;
        case 0x1100u: /* and r0,r0,#0xf */
            r[0]=r[0]&15u;r[15]=0x1104u;return 0;
        case 0x1104u: /* adds r0,r0,r3 */
            r[0]=addition(state,r[0],r[3]);r[15]=0x1108u;return 0;
        case 0x1108u: /* movmi r0,#0x0 */
            if(state->cpsr&0x80000000u){r[0]=0u;}r[15]=0x110cu;return 0;
        case 0x110cu: /* tst r0,#0x10 */
            temporary=r[0]&16u;logic_flags(state,temporary,0,16u);r[15]=0x1110u;return 0;
        case 0x1110u: /* movne r0,#0xf */
            if(!(state->cpsr&0x40000000u)){r[0]=15u;}r[15]=0x1114u;return 0;
        case 0x1114u: /* and r0,r0,#0xf */
            r[0]=r[0]&15u;r[15]=0x1118u;return 0;
        case 0x1118u: /* orr r1,r1,r0, lsl #0x8 */
            r[1]=r[1]|(r[0]<<8);r[15]=0x111cu;return 0;
        case 0x111cu: /* str r1,[r8,#0x24] */
            STORE(r[8]+36u,r[1],4);r[15]=0x1120u;return 0;
        case 0x1120u: /* ldrb r0,[0x1204] */
            LOAD(r[0],4612u,1);r[15]=0x1124u;return 0;
        case 0x1124u: /* strb r0,[r6,#0x4] */
            STORE(r[6]+4u,r[0],1);r[15]=0x1128u;return 0;
        case 0x1128u: /* ldrb r0,[0x1205] */
            LOAD(r[0],4613u,1);r[15]=0x112cu;return 0;
        case 0x112cu: /* strb r0,[r6,#0x3e] */
            STORE(r[6]+62u,r[0],1);r[15]=0x1130u;return 0;
        case 0x1130u: /* ldrb r0,[0x1206] */
            LOAD(r[0],4614u,1);r[15]=0x1134u;return 0;
        case 0x1134u: /* strb r0,[r6,#0x5] */
            STORE(r[6]+5u,r[0],1);r[15]=0x1138u;return 0;
        case 0x1138u: /* ldr r3,[0x1214] */
            LOAD(r[3],4628u,4);r[15]=0x113cu;return 0;
        case 0x113cu: /* ldr r0,[r3,#0x10] */
            LOAD(r[0],r[3]+16u,4);r[15]=0x1140u;return 0;
        case 0x1140u: /* sub r0,r0,#0x8000 */
            r[0]=r[0]-32768u;r[15]=0x1144u;return 0;
        case 0x1144u: /* ldr r7,[r6,#0x14] */
            LOAD(r[7],r[6]+20u,4);r[15]=0x1148u;return 0;
        case 0x1148u: /* add r7,r7,r0 */
            r[7]=r[7]+r[0];r[15]=0x114cu;return 0;
        case 0x114cu: /* ldr r0,[r5,#0x4] */
            LOAD(r[0],r[5]+4u,4);r[15]=0x1150u;return 0;
        case 0x1150u: /* ldr r1,[0x1234] */
            LOAD(r[1],4660u,4);r[15]=0x1154u;return 0;
        case 0x1154u: /* tst r0,r1 */
            temporary=r[0]&r[1];logic_flags(state,temporary,0,r[1]);r[15]=0x1158u;return 0;
        case 0x1158u: /* ldrbne r1,[r6,#0xb] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],r[6]+11u,1);}r[15]=0x115cu;return 0;
        case 0x115cu: /* ldrbeq r1,[r6,#0xa] */
            if(state->cpsr&0x40000000u){LOAD(r[1],r[6]+10u,1);}r[15]=0x1160u;return 0;
        case 0x1160u: /* mul r0,r1,r0 */
            r[0]=r[1]*r[0];r[15]=0x1164u;return 0;
        case 0x1164u: /* mov r0,r0, lsr #0x5 */
            r[0]=(r[0]>>5);r[15]=0x1168u;return 0;
        case 0x1168u: /* ldr r1,[0x122c] */
            LOAD(r[1],4652u,4);r[15]=0x116cu;return 0;
        case 0x116cu: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0x1170u;return 0;
        case 0x1170u: /* str r1,[r6,#0xc] */
            STORE(r[6]+12u,r[1],4);r[15]=0x1174u;return 0;
        case 0x1174u: /* add r7,r7,r1 */
            r[7]=r[7]+r[1];r[15]=0x1178u;return 0;
        case 0x1178u: /* stmdb sp!,{r2,r5,r6,lr} */
            STORE(r[13]-16u,r[2],4);STORE(r[13]-12u,r[5],4);STORE(r[13]-8u,r[6],4);STORE(r[13]-4u,r[14],4);r[13]-=16u;r[15]=0x117cu;return 0;
        case 0x117cu: /* mov r5,r6 */
            r[5]=r[6];r[15]=0x1180u;return 0;
        case 0x1180u: /* mov r6,r2 */
            r[6]=r[2];r[15]=0x1184u;return 0;
        case 0x1184u: /* bl 0x000014b0 */
            r[14]=0x1188u;r[15]=5296u;return 0;
        case 0x1188u: /* ldmia sp!,{r2,r5,r6,lr} */
            LOAD(r[2],r[13]+0u,4);LOAD(r[5],r[13]+4u,4);LOAD(r[6],r[13]+8u,4);LOAD(r[14],r[13]+12u,4);r[13]+=16u;r[15]=0x118cu;return 0;
        case 0x118cu: /* ldrb r0,[r6,#0x0] */
            LOAD(r[0],r[6]+0u,1);r[15]=0x1190u;return 0;
        case 0x1190u: /* orr r0,r0,#0x80 */
            r[0]=r[0]|128u;r[15]=0x1194u;return 0;
        case 0x1194u: /* ldrb r1,[r5,#0x26] */
            LOAD(r[1],r[5]+38u,1);r[15]=0x1198u;return 0;
        case 0x1198u: /* tst r1,#0x40 */
            temporary=r[1]&64u;logic_flags(state,temporary,0,64u);r[15]=0x119cu;return 0;
        case 0x119cu: /* orrne r0,r0,#0x40 */
            if(!(state->cpsr&0x40000000u)){r[0]=r[0]|64u;}r[15]=0x11a0u;return 0;
        case 0x11a0u: /* strb r0,[r6,#0x0] */
            STORE(r[6]+0u,r[0],1);r[15]=0x11a4u;return 0;
        case 0x11a4u: /* mov r0,#0x1 */
            r[0]=1u;r[15]=0x11a8u;return 0;
        case 0x11a8u: /* strb r0,[r6,#0x3f] */
            STORE(r[6]+63u,r[0],1);r[15]=0x11acu;return 0;
        case 0x11acu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x11b0u;return 0;
        case 0x11b0u: /* strb r0,[r6,#0x3] */
            STORE(r[6]+3u,r[0],1);r[15]=0x11b4u;return 0;
        case 0x11b4u: /* ldr r0,[0x1224] */
            LOAD(r[0],4644u,4);r[15]=0x11b8u;return 0;
        case 0x11b8u: /* ldr r1,[0x1220] */
            LOAD(r[1],4640u,4);r[15]=0x11bcu;return 0;
        case 0x11bcu: /* and r0,r0,r1 */
            r[0]=r[0]&r[1];r[15]=0x11c0u;return 0;
        case 0x11c0u: /* ldr r1,[r6,#0x38] */
            LOAD(r[1],r[6]+56u,4);r[15]=0x11c4u;return 0;
        case 0x11c4u: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x11c8u;return 0;
        case 0x11c8u: /* orreq r0,r0,#0xc000 */
            if(state->cpsr&0x40000000u){r[0]=r[0]|49152u;}r[15]=0x11ccu;return 0;
        case 0x11ccu: /* ldr r1,[r8,#0x1c] */
            LOAD(r[1],r[8]+28u,4);r[15]=0x11d0u;return 0;
        case 0x11d0u: /* bic r1,r1,#0x8000 */
            r[1]=r[1]&~32768u;r[15]=0x11d4u;return 0;
        case 0x11d4u: /* str r1,[r8,#0x1c] */
            STORE(r[8]+28u,r[1],4);r[15]=0x11d8u;return 0;
        case 0x11d8u: /* str r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],4);r[15]=0x11dcu;return 0;
        case 0x11dcu: /* b 0x000011ec */
            r[15]=4588u;return 0;
        case 0x11e0u: /* add r2,r2,#0x30 */
            r[2]=r[2]+48u;r[15]=0x11e4u;return 0;
        case 0x11e4u: /* subs r11,r11,#0x1 */
            r[11]=subtract(state,r[11],1u);r[15]=0x11e8u;return 0;
        case 0x11e8u: /* bne 0x00000b78 */
            if(!(state->cpsr&0x40000000u)){r[15]=2936u;}else{r[15]=0x11ecu;}return 0;
        case 0x11ecu: /* ldr r1,[0x1218] */
            LOAD(r[1],4632u,4);r[15]=0x11f0u;return 0;
        case 0x11f0u: /* add r1,r1,#0x4 */
            r[1]=r[1]+4u;r[15]=0x11f4u;return 0;
        case 0x11f4u: /* str r1,[0x1218] */
            STORE(4632u,r[1],4);r[15]=0x11f8u;return 0;
        case 0x11f8u: /* subs r12,r12,#0x1 */
            r[12]=subtract(state,r[12],1u);r[15]=0x11fcu;return 0;
        case 0x11fcu: /* bne 0x00000b1c */
            if(!(state->cpsr&0x40000000u)){r[15]=2844u;}else{r[15]=0x1200u;}return 0;
        case 0x1200u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1244u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x1248u;return 0;
        case 0x1248u: /* mov r10,#0x30 */
            r[10]=48u;r[15]=0x124cu;return 0;
        case 0x124cu: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x1250u;return 0;
        case 0x1250u: /* adr r1,0x1350 */
            r[1]=4944u;r[15]=0x1254u;return 0;
        case 0x1254u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x1258u;return 0;
        case 0x1258u: /* str r0,[r1],#0x4 */
            STORE(r[1],r[0],4);r[1]+=4u;r[15]=0x125cu;return 0;
        case 0x125cu: /* str r0,[r1],#0x4 */
            STORE(r[1],r[0],4);r[1]+=4u;r[15]=0x1260u;return 0;
        case 0x1260u: /* str r0,[r1],#0x4 */
            STORE(r[1],r[0],4);r[1]+=4u;r[15]=0x1264u;return 0;
        case 0x1264u: /* ldr r0,[r6,#0x0] */
            LOAD(r[0],r[6]+0u,4);r[15]=0x1268u;return 0;
        case 0x1268u: /* movs r0,r0 */
            r[0]=r[0];logic_flags(state,r[0],0,0);r[15]=0x126cu;return 0;
        case 0x126cu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x1270u;}return 0;
        case 0x1270u: /* ldrb r0,[r6,#0x3e] */
            LOAD(r[0],r[6]+62u,1);r[15]=0x1274u;return 0;
        case 0x1274u: /* ldrb r3,[0x1205] */
            LOAD(r[3],4613u,1);r[15]=0x1278u;return 0;
        case 0x1278u: /* cmp r3,r0 */
            (void)subtract(state,r[3],r[0]);r[15]=0x127cu;return 0;
        case 0x127cu: /* bmi 0x00001328 */
            if(state->cpsr&0x80000000u){r[15]=4904u;}else{r[15]=0x1280u;}return 0;
        case 0x1280u: /* adr r1,0x1350 */
            r[1]=4944u;r[15]=0x1284u;return 0;
        case 0x1284u: /* ldr r3,[r1,#0x0] */
            LOAD(r[3],r[1]+0u,4);r[15]=0x1288u;return 0;
        case 0x1288u: /* movs r3,r3 */
            r[3]=r[3];logic_flags(state,r[3],0,0);r[15]=0x128cu;return 0;
        case 0x128cu: /* beq 0x00001300 */
            if(state->cpsr&0x40000000u){r[15]=4864u;}else{r[15]=0x1290u;}return 0;
        case 0x1290u: /* ldrb r0,[r1,#0x8] */
            LOAD(r[0],r[1]+8u,1);r[15]=0x1294u;return 0;
        case 0x1294u: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x1298u;return 0;
        case 0x1298u: /* bne 0x000012cc */
            if(!(state->cpsr&0x40000000u)){r[15]=4812u;}else{r[15]=0x129cu;}return 0;
        case 0x129cu: /* ldrb r3,[r6,#0x0] */
            LOAD(r[3],r[6]+0u,1);r[15]=0x12a0u;return 0;
        case 0x12a0u: /* tst r3,#0xff */
            temporary=r[3]&255u;logic_flags(state,temporary,0,255u);r[15]=0x12a4u;return 0;
        case 0x12a4u: /* beq 0x00001300 */
            if(state->cpsr&0x40000000u){r[15]=4864u;}else{r[15]=0x12a8u;}return 0;
        case 0x12a8u: /* ldrb r3,[r6,#0x3e] */
            LOAD(r[3],r[6]+62u,1);r[15]=0x12acu;return 0;
        case 0x12acu: /* ldrb r0,[r1,#0x9] */
            LOAD(r[0],r[1]+9u,1);r[15]=0x12b0u;return 0;
        case 0x12b0u: /* cmp r0,r3 */
            (void)subtract(state,r[0],r[3]);r[15]=0x12b4u;return 0;
        case 0x12b4u: /* bmi 0x00001328 */
            if(state->cpsr&0x80000000u){r[15]=4904u;}else{r[15]=0x12b8u;}return 0;
        case 0x12b8u: /* ldrb r3,[r6,#0x3f] */
            LOAD(r[3],r[6]+63u,1);r[15]=0x12bcu;return 0;
        case 0x12bcu: /* ldrb r0,[r1,#0xa] */
            LOAD(r[0],r[1]+10u,1);r[15]=0x12c0u;return 0;
        case 0x12c0u: /* cmp r3,r0 */
            (void)subtract(state,r[3],r[0]);r[15]=0x12c4u;return 0;
        case 0x12c4u: /* bmi 0x00001328 */
            if(state->cpsr&0x80000000u){r[15]=4904u;}else{r[15]=0x12c8u;}return 0;
        case 0x12c8u: /* b 0x00001300 */
            r[15]=4864u;return 0;
        case 0x12ccu: /* ldrb r3,[r6,#0x0] */
            LOAD(r[3],r[6]+0u,1);r[15]=0x12d0u;return 0;
        case 0x12d0u: /* tst r3,#0xff */
            temporary=r[3]&255u;logic_flags(state,temporary,0,255u);r[15]=0x12d4u;return 0;
        case 0x12d4u: /* bne 0x00001328 */
            if(!(state->cpsr&0x40000000u)){r[15]=4904u;}else{r[15]=0x12d8u;}return 0;
        case 0x12d8u: /* ldrb r3,[r6,#0x3] */
            LOAD(r[3],r[6]+3u,1);r[15]=0x12dcu;return 0;
        case 0x12dcu: /* ldrb r0,[r1,#0x8] */
            LOAD(r[0],r[1]+8u,1);r[15]=0x12e0u;return 0;
        case 0x12e0u: /* cmp r0,r3 */
            (void)subtract(state,r[0],r[3]);r[15]=0x12e4u;return 0;
        case 0x12e4u: /* beq 0x000012f0 */
            if(state->cpsr&0x40000000u){r[15]=4848u;}else{r[15]=0x12e8u;}return 0;
        case 0x12e8u: /* bmi 0x00001328 */
            if(state->cpsr&0x80000000u){r[15]=4904u;}else{r[15]=0x12ecu;}return 0;
        case 0x12ecu: /* b 0x00001300 */
            r[15]=4864u;return 0;
        case 0x12f0u: /* ldrb r3,[r6,#0x3e] */
            LOAD(r[3],r[6]+62u,1);r[15]=0x12f4u;return 0;
        case 0x12f4u: /* ldrb r0,[r1,#0x9] */
            LOAD(r[0],r[1]+9u,1);r[15]=0x12f8u;return 0;
        case 0x12f8u: /* cmp r3,r0 */
            (void)subtract(state,r[3],r[0]);r[15]=0x12fcu;return 0;
        case 0x12fcu: /* bmi 0x00001328 */
            if(state->cpsr&0x80000000u){r[15]=4904u;}else{r[15]=0x1300u;}return 0;
        case 0x1300u: /* str r6,[r1,#0x0] */
            STORE(r[1]+0u,r[6],4);r[15]=0x1304u;return 0;
        case 0x1304u: /* str r8,[r1,#0x4] */
            STORE(r[1]+4u,r[8],4);r[15]=0x1308u;return 0;
        case 0x1308u: /* ldrb r3,[r6,#0x3] */
            LOAD(r[3],r[6]+3u,1);r[15]=0x130cu;return 0;
        case 0x130cu: /* strb r3,[r1,#0x8] */
            STORE(r[1]+8u,r[3],1);r[15]=0x1310u;return 0;
        case 0x1310u: /* ldrb r3,[r6,#0x3e] */
            LOAD(r[3],r[6]+62u,1);r[15]=0x1314u;return 0;
        case 0x1314u: /* strb r3,[r1,#0x9] */
            STORE(r[1]+9u,r[3],1);r[15]=0x1318u;return 0;
        case 0x1318u: /* ldrb r3,[r6,#0x3f] */
            LOAD(r[3],r[6]+63u,1);r[15]=0x131cu;return 0;
        case 0x131cu: /* strb r3,[r1,#0xa] */
            STORE(r[1]+10u,r[3],1);r[15]=0x1320u;return 0;
        case 0x1320u: /* ldrb r3,[r6,#0x4] */
            LOAD(r[3],r[6]+4u,1);r[15]=0x1324u;return 0;
        case 0x1324u: /* strb r3,[r1,#0xb] */
            STORE(r[1]+11u,r[3],1);r[15]=0x1328u;return 0;
        case 0x1328u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x132cu;return 0;
        case 0x132cu: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x1330u;return 0;
        case 0x1330u: /* subs r10,r10,#0x1 */
            r[10]=subtract(state,r[10],1u);r[15]=0x1334u;return 0;
        case 0x1334u: /* bne 0x00001264 */
            if(!(state->cpsr&0x40000000u)){r[15]=4708u;}else{r[15]=0x1338u;}return 0;
        case 0x1338u: /* ldr r6,[r1,#0x0] */
            LOAD(r[6],r[1]+0u,4);r[15]=0x133cu;return 0;
        case 0x133cu: /* ldr r8,[r1,#0x4] */
            LOAD(r[8],r[1]+4u,4);r[15]=0x1340u;return 0;
        case 0x1340u: /* movs r6,r6 */
            r[6]=r[6];logic_flags(state,r[6],0,0);r[15]=0x1344u;return 0;
        case 0x1344u: /* movne r0,#0x0 */
            if(!(state->cpsr&0x40000000u)){r[0]=0u;}r[15]=0x1348u;return 0;
        case 0x1348u: /* moveq r0,#0xff */
            if(state->cpsr&0x40000000u){r[0]=255u;}r[15]=0x134cu;return 0;
        case 0x134cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        default:*handled=0;return 0;
 }}else{switch(r[15]){
        case 0xad4u: /* mov r0,r1, lsr #0x8 */
            r[0]=(r[1]>>8);r[15]=0xad8u;return 0;
        case 0xad8u: /* and r0,r0,#0x7f */
            r[0]=r[0]&127u;r[15]=0xadcu;return 0;
        case 0xadcu: /* ands r1,r1,#0x7f */
            temporary=r[1]&127u;r[1]=temporary;logic_flags(state,temporary,0,127u);r[15]=0xae0u;return 0;
        case 0xae0u: /* beq 0x00000a40 */
            if(state->cpsr&0x40000000u){r[15]=2624u;}else{r[15]=0xae4u;}return 0;
        case 0xae4u: /* add r5,r5,r6 */
            r[5]=r[5]+r[6];r[15]=0xae8u;return 0;
        case 0xae8u: /* and r3,r3,#0xf */
            r[3]=r[3]&15u;r[15]=0xaecu;return 0;
        case 0xaecu: /* strb r3,[0x120c] */
            STORE(4620u,r[3],1);r[15]=0xaf0u;return 0;
        case 0xaf0u: /* strb r4,[0x120d] */
            STORE(4621u,r[4],1);r[15]=0xaf4u;return 0;
        case 0xaf4u: /* strb r2,[0x120e] */
            STORE(4622u,r[2],1);r[15]=0xaf8u;return 0;
        case 0xaf8u: /* strb r0,[0x120f] */
            STORE(4623u,r[0],1);r[15]=0xafcu;return 0;
        case 0xafcu: /* strb r1,[0x1210] */
            STORE(4624u,r[1],1);r[15]=0xb00u;return 0;
        case 0xb00u: /* mov r4,#0xc000 */
            r[4]=49152u;r[15]=0xb04u;return 0;
        case 0xb04u: /* mov r0,#0x80 */
            r[0]=128u;r[15]=0xb08u;return 0;
        case 0xb08u: /* mla r0,r3,r0,r4 */
            r[0]=r[3]*r[0]+r[4];r[15]=0xb0cu;return 0;
        case 0xb0cu: /* str r0,[0x121c] */
            STORE(4636u,r[0],4);r[15]=0xb10u;return 0;
        case 0xb10u: /* ldr r0,[r5,#0x14] */
            LOAD(r[0],r[5]+20u,4);r[15]=0xb14u;return 0;
        case 0xb14u: /* cmn r0,#0x1 */
            (void)addition(state,r[0],1u);r[15]=0xb18u;return 0;
        case 0xb18u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0xb1cu;}return 0;
        case 0xb1cu: /* str r0,[0x1220] */
            STORE(4640u,r[0],4);r[15]=0xb20u;return 0;
        case 0xb20u: /* mov r12,#0x4 */
            r[12]=4u;r[15]=0xb24u;return 0;
        case 0xb24u: /* ldr r1,[0x1220] */
            LOAD(r[1],4640u,4);r[15]=0xb28u;return 0;
        case 0xb28u: /* ldr r0,[r1,#0x0] */
            LOAD(r[0],r[1]+0u,4);r[15]=0xb2cu;return 0;
        case 0xb2cu: /* movs r0,r0 */
            r[0]=r[0];logic_flags(state,r[0],0,0);r[15]=0xb30u;return 0;
        case 0xb30u: /* beq 0x000011f4 */
            if(state->cpsr&0x40000000u){r[15]=4596u;}else{r[15]=0xb34u;}return 0;
        case 0xb34u: /* ldr r1,[r5,#0x10] */
            LOAD(r[1],r[5]+16u,4);r[15]=0xb38u;return 0;
        case 0xb38u: /* add r0,r0,r1 */
            r[0]=r[0]+r[1];r[15]=0xb3cu;return 0;
        case 0xb3cu: /* ldr r1,[r0,#0x8] */
            LOAD(r[1],r[0]+8u,4);r[15]=0xb40u;return 0;
        case 0xb40u: /* str r1,[0x1214] */
            STORE(4628u,r[1],4);r[15]=0xb44u;return 0;
        case 0xb44u: /* ldrb r1,[r0,#0xc] */
            LOAD(r[1],r[0]+12u,1);r[15]=0xb48u;return 0;
        case 0xb48u: /* strb r1,[0x1218] */
            STORE(4632u,r[1],1);r[15]=0xb4cu;return 0;
        case 0xb4cu: /* ldrb r1,[r0,#0xd] */
            LOAD(r[1],r[0]+13u,1);r[15]=0xb50u;return 0;
        case 0xb50u: /* strb r1,[0x1219] */
            STORE(4633u,r[1],1);r[15]=0xb54u;return 0;
        case 0xb54u: /* ldrb r1,[r0,#0xe] */
            LOAD(r[1],r[0]+14u,1);r[15]=0xb58u;return 0;
        case 0xb58u: /* strb r1,[0x121a] */
            STORE(4634u,r[1],1);r[15]=0xb5cu;return 0;
        case 0xb5cu: /* ldrb r1,[r0,#0xf] */
            LOAD(r[1],r[0]+15u,1);r[15]=0xb60u;return 0;
        case 0xb60u: /* strb r1,[0x121b] */
            STORE(4635u,r[1],1);r[15]=0xb64u;return 0;
        case 0xb64u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0xb68u;return 0;
        case 0xb68u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0xb6cu;return 0;
        case 0xb6cu: /* bne 0x000011f4 */
            if(!(state->cpsr&0x40000000u)){r[15]=4596u;}else{r[15]=0xb70u;}return 0;
        case 0xb70u: /* and r11,r1,#0x7f */
            r[11]=r[1]&127u;r[15]=0xb74u;return 0;
        case 0xb74u: /* ldr r2,[r0,#0x4] */
            LOAD(r[2],r[0]+4u,4);r[15]=0xb78u;return 0;
        case 0xb78u: /* ldr r3,[r5,#0x10] */
            LOAD(r[3],r[5]+16u,4);r[15]=0xb7cu;return 0;
        case 0xb7cu: /* add r2,r2,r3 */
            r[2]=r[2]+r[3];r[15]=0xb80u;return 0;
        case 0xb80u: /* ldrb r1,[r2,#0x24] */
            LOAD(r[1],r[2]+36u,1);r[15]=0xb84u;return 0;
        case 0xb84u: /* ldrb r3,[0x120f] */
            LOAD(r[3],4623u,1);r[15]=0xb88u;return 0;
        case 0xb88u: /* cmp r3,r1 */
            (void)subtract(state,r[3],r[1]);r[15]=0xb8cu;return 0;
        case 0xb8cu: /* bmi 0x000011e8 */
            if(state->cpsr&0x80000000u){r[15]=4584u;}else{r[15]=0xb90u;}return 0;
        case 0xb90u: /* ldrb r1,[r2,#0x25] */
            LOAD(r[1],r[2]+37u,1);r[15]=0xb94u;return 0;
        case 0xb94u: /* cmp r1,r3 */
            (void)subtract(state,r[1],r[3]);r[15]=0xb98u;return 0;
        case 0xb98u: /* bmi 0x000011e8 */
            if(state->cpsr&0x80000000u){r[15]=4584u;}else{r[15]=0xb9cu;}return 0;
        case 0xb9cu: /* ldrb r1,[r2,#0x2b] */
            LOAD(r[1],r[2]+43u,1);r[15]=0xba0u;return 0;
        case 0xba0u: /* ldrb r3,[0x1210] */
            LOAD(r[3],4624u,1);r[15]=0xba4u;return 0;
        case 0xba4u: /* cmp r3,r1 */
            (void)subtract(state,r[3],r[1]);r[15]=0xba8u;return 0;
        case 0xba8u: /* bmi 0x000011e8 */
            if(state->cpsr&0x80000000u){r[15]=4584u;}else{r[15]=0xbacu;}return 0;
        case 0xbacu: /* ldrb r1,[r2,#0x2c] */
            LOAD(r[1],r[2]+44u,1);r[15]=0xbb0u;return 0;
        case 0xbb0u: /* cmp r1,r3 */
            (void)subtract(state,r[1],r[3]);r[15]=0xbb4u;return 0;
        case 0xbb4u: /* bmi 0x000011e8 */
            if(state->cpsr&0x80000000u){r[15]=4584u;}else{r[15]=0xbb8u;}return 0;
        case 0xbb8u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0xbbcu;return 0;
        case 0xbbcu: /* bl 0x0000124c */
            r[14]=0xbc0u;r[15]=4684u;return 0;
        case 0xbc0u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0xbc4u;return 0;
        case 0xbc4u: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0xbc8u;return 0;
        case 0xbc8u: /* bne 0x000011e8 */
            if(!(state->cpsr&0x40000000u)){r[15]=4584u;}else{r[15]=0xbccu;}return 0;
        case 0xbccu: /* mov r0,#0x8000 */
            r[0]=32768u;r[15]=0xbd0u;return 0;
        case 0xbd0u: /* str r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],4);r[15]=0xbd4u;return 0;
        case 0xbd4u: /* ldr r0,[0x1214] */
            LOAD(r[0],4628u,4);r[15]=0xbd8u;return 0;
        case 0xbd8u: /* str r0,[r6,#0x38] */
            STORE(r[6]+56u,r[0],4);r[15]=0xbdcu;return 0;
        case 0xbdcu: /* str r2,[r6,#0x10] */
            STORE(r[6]+16u,r[2],4);r[15]=0xbe0u;return 0;
        case 0xbe0u: /* ldrb r0,[r5,#0x3] */
            LOAD(r[0],r[5]+3u,1);r[15]=0xbe4u;return 0;
        case 0xbe4u: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0xbe8u;return 0;
        case 0xbe8u: /* andne r0,r0,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[0]=r[0]&127u;}r[15]=0xbecu;return 0;
        case 0xbecu: /* movne r1,r0 */
            if(!(state->cpsr&0x40000000u)){r[1]=r[0];}r[15]=0xbf0u;return 0;
        case 0xbf0u: /* ldrbeq r0,[0x1218] */
            if(state->cpsr&0x40000000u){LOAD(r[0],4632u,1);}r[15]=0xbf4u;return 0;
        case 0xbf4u: /* ldrbeq r1,[0x1219] */
            if(state->cpsr&0x40000000u){LOAD(r[1],4633u,1);}r[15]=0xbf8u;return 0;
        case 0xbf8u: /* strb r0,[r6,#0xa] */
            STORE(r[6]+10u,r[0],1);r[15]=0xbfcu;return 0;
        case 0xbfcu: /* strb r1,[r6,#0xb] */
            STORE(r[6]+11u,r[1],1);r[15]=0xc00u;return 0;
        case 0xc00u: /* ldrb r0,[0x120f] */
            LOAD(r[0],4623u,1);r[15]=0xc04u;return 0;
        case 0xc04u: /* strb r0,[r6,#0x6] */
            STORE(r[6]+6u,r[0],1);r[15]=0xc08u;return 0;
        case 0xc08u: /* ldr r0,[r2,#0x0] */
            LOAD(r[0],r[2]+0u,4);r[15]=0xc0cu;return 0;
        case 0xc0cu: /* ldr r1,[0x1224] */
            LOAD(r[1],4644u,4);r[15]=0xc10u;return 0;
        case 0xc10u: /* mov r3,r0, ror #0x10 */
            r[3]=((r[0]>>16)|(r[0]<<16));r[15]=0xc14u;return 0;
        case 0xc14u: /* and r3,r3,r1 */
            r[3]=r[3]&r[1];r[15]=0xc18u;return 0;
        case 0xc18u: /* ldr r1,[r5,#0x10] */
            LOAD(r[1],r[5]+16u,4);r[15]=0xc1cu;return 0;
        case 0xc1cu: /* add r3,r3,r1 */
            r[3]=r[3]+r[1];r[15]=0xc20u;return 0;
        case 0xc20u: /* ldr r1,[0x1234] */
            LOAD(r[1],4660u,4);r[15]=0xc24u;return 0;
        case 0xc24u: /* and r1,r3,r1 */
            r[1]=r[3]&r[1];r[15]=0xc28u;return 0;
        case 0xc28u: /* str r1,[r8,#0x4] */
            STORE(r[8]+4u,r[1],4);r[15]=0xc2cu;return 0;
        case 0xc2cu: /* mov r1,r3, ror #0x10 */
            r[1]=((r[3]>>16)|(r[3]<<16));r[15]=0xc30u;return 0;
        case 0xc30u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0xc34u;return 0;
        case 0xc34u: /* and r0,r0,#0x780 */
            r[0]=r[0]&1920u;r[15]=0xc38u;return 0;
        case 0xc38u: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0xc3cu;return 0;
        case 0xc3cu: /* str r0,[0x122c] */
            STORE(4652u,r[0],4);r[15]=0xc40u;return 0;
        case 0xc40u: /* ldr r0,[r2,#0x4] */
            LOAD(r[0],r[2]+4u,4);r[15]=0xc44u;return 0;
        case 0xc44u: /* ldr r1,[0x1234] */
            LOAD(r[1],4660u,4);r[15]=0xc48u;return 0;
        case 0xc48u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0xc4cu;return 0;
        case 0xc4cu: /* str r1,[r8,#0x8] */
            STORE(r[8]+8u,r[1],4);r[15]=0xc50u;return 0;
        case 0xc50u: /* mov r0,r0, ror #0x10 */
            r[0]=((r[0]>>16)|(r[0]<<16));r[15]=0xc54u;return 0;
        case 0xc54u: /* ldr r1,[0x1234] */
            LOAD(r[1],4660u,4);r[15]=0xc58u;return 0;
        case 0xc58u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0xc5cu;return 0;
        case 0xc5cu: /* str r1,[r8,#0xc] */
            STORE(r[8]+12u,r[1],4);r[15]=0xc60u;return 0;
        case 0xc60u: /* ldr r0,[r2,#0x8] */
            LOAD(r[0],r[2]+8u,4);r[15]=0xc64u;return 0;
        case 0xc64u: /* and r1,r0,#0x1f */
            r[1]=r[0]&31u;r[15]=0xc68u;return 0;
        case 0xc68u: /* ldrb r3,[r5,#0x37] */
            LOAD(r[3],r[5]+55u,1);r[15]=0xc6cu;return 0;
        case 0xc6cu: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xc70u;return 0;
        case 0xc70u: /* adds r1,r1,r3 */
            r[1]=addition(state,r[1],r[3]);r[15]=0xc74u;return 0;
        case 0xc74u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xc78u;return 0;
        case 0xc78u: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0xc7cu;return 0;
        case 0xc7cu: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0xc80u;return 0;
        case 0xc80u: /* str r1,[0x1248] */
            STORE(4680u,r[1],4);r[15]=0xc84u;return 0;
        case 0xc84u: /* mov r1,r0, lsr #0x6 */
            r[1]=(r[0]>>6);r[15]=0xc88u;return 0;
        case 0xc88u: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0xc8cu;return 0;
        case 0xc8cu: /* ldrb r3,[r5,#0x38] */
            LOAD(r[3],r[5]+56u,1);r[15]=0xc90u;return 0;
        case 0xc90u: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xc94u;return 0;
        case 0xc94u: /* adds r1,r1,r3 */
            r[1]=addition(state,r[1],r[3]);r[15]=0xc98u;return 0;
        case 0xc98u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xc9cu;return 0;
        case 0xc9cu: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0xca0u;return 0;
        case 0xca0u: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0xca4u;return 0;
        case 0xca4u: /* mov r0,r0, lsr #0xb */
            r[0]=(r[0]>>11);r[15]=0xca8u;return 0;
        case 0xca8u: /* and r0,r0,#0x1f */
            r[0]=r[0]&31u;r[15]=0xcacu;return 0;
        case 0xcacu: /* ldrb r3,[r5,#0x38] */
            LOAD(r[3],r[5]+56u,1);r[15]=0xcb0u;return 0;
        case 0xcb0u: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xcb4u;return 0;
        case 0xcb4u: /* adds r0,r0,r3 */
            r[0]=addition(state,r[0],r[3]);r[15]=0xcb8u;return 0;
        case 0xcb8u: /* movmi r0,#0x0 */
            if(state->cpsr&0x80000000u){r[0]=0u;}r[15]=0xcbcu;return 0;
        case 0xcbcu: /* tst r0,#0xe0 */
            temporary=r[0]&224u;logic_flags(state,temporary,0,224u);r[15]=0xcc0u;return 0;
        case 0xcc0u: /* movne r0,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[0]=31u;}r[15]=0xcc4u;return 0;
        case 0xcc4u: /* orr r0,r1,r0, lsl #0x5 */
            r[0]=r[1]|(r[0]<<5);r[15]=0xcc8u;return 0;
        case 0xcc8u: /* ldr r1,[0x1248] */
            LOAD(r[1],4680u,4);r[15]=0xcccu;return 0;
        case 0xcccu: /* orr r1,r1,r0, lsl #0x6 */
            r[1]=r[1]|(r[0]<<6);r[15]=0xcd0u;return 0;
        case 0xcd0u: /* str r1,[r8,#0x10] */
            STORE(r[8]+16u,r[1],4);r[15]=0xcd4u;return 0;
        case 0xcd4u: /* ldr r0,[r2,#0x8] */
            LOAD(r[0],r[2]+8u,4);r[15]=0xcd8u;return 0;
        case 0xcd8u: /* mov r0,r0, ror #0x10 */
            r[0]=((r[0]>>16)|(r[0]<<16));r[15]=0xcdcu;return 0;
        case 0xcdcu: /* and r1,r0,#0x1f */
            r[1]=r[0]&31u;r[15]=0xce0u;return 0;
        case 0xce0u: /* ldrb r3,[r5,#0x3b] */
            LOAD(r[3],r[5]+59u,1);r[15]=0xce4u;return 0;
        case 0xce4u: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xce8u;return 0;
        case 0xce8u: /* adds r1,r1,r3 */
            r[1]=addition(state,r[1],r[3]);r[15]=0xcecu;return 0;
        case 0xcecu: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xcf0u;return 0;
        case 0xcf0u: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0xcf4u;return 0;
        case 0xcf4u: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0xcf8u;return 0;
        case 0xcf8u: /* str r1,[0x1248] */
            STORE(4680u,r[1],4);r[15]=0xcfcu;return 0;
        case 0xcfcu: /* mov r1,r0, lsr #0x5 */
            r[1]=(r[0]>>5);r[15]=0xd00u;return 0;
        case 0xd00u: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0xd04u;return 0;
        case 0xd04u: /* ldrb r3,[r5,#0x39] */
            LOAD(r[3],r[5]+57u,1);r[15]=0xd08u;return 0;
        case 0xd08u: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xd0cu;return 0;
        case 0xd0cu: /* adds r1,r1,r3 */
            r[1]=addition(state,r[1],r[3]);r[15]=0xd10u;return 0;
        case 0xd10u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xd14u;return 0;
        case 0xd14u: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0xd18u;return 0;
        case 0xd18u: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0xd1cu;return 0;
        case 0xd1cu: /* and r0,r0,#0x7c00 */
            r[0]=r[0]&31744u;r[15]=0xd20u;return 0;
        case 0xd20u: /* orr r1,r0,r1, lsl #0x5 */
            r[1]=r[0]|(r[1]<<5);r[15]=0xd24u;return 0;
        case 0xd24u: /* ldr r0,[0x1248] */
            LOAD(r[0],4680u,4);r[15]=0xd28u;return 0;
        case 0xd28u: /* orr r1,r0,r1 */
            r[1]=r[0]|r[1];r[15]=0xd2cu;return 0;
        case 0xd2cu: /* str r1,[r8,#0x14] */
            STORE(r[8]+20u,r[1],4);r[15]=0xd30u;return 0;
        case 0xd30u: /* ldr r0,[r2,#0xc] */
            LOAD(r[0],r[2]+12u,4);r[15]=0xd34u;return 0;
        case 0xd34u: /* mov r0,r0, ror #0x10 */
            r[0]=((r[0]>>16)|(r[0]<<16));r[15]=0xd38u;return 0;
        case 0xd38u: /* ldrb r1,[r5,#0x2] */
            LOAD(r[1],r[5]+2u,1);r[15]=0xd3cu;return 0;
        case 0xd3cu: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0xd40u;return 0;
        case 0xd40u: /* mov r3,r0, lsr #0x5 */
            r[3]=(r[0]>>5);r[15]=0xd44u;return 0;
        case 0xd44u: /* and r3,r3,#0x7 */
            r[3]=r[3]&7u;r[15]=0xd48u;return 0;
        case 0xd48u: /* add r3,r3,#0x1 */
            r[3]=r[3]+1u;r[15]=0xd4cu;return 0;
        case 0xd4cu: /* mul r1,r3,r1 */
            r[1]=r[3]*r[1];r[15]=0xd50u;return 0;
        case 0xd50u: /* sub r1,r1,#0x1 */
            r[1]=r[1]-1u;r[15]=0xd54u;return 0;
        case 0xd54u: /* mov r1,r1, lsr #0x2 */
            r[1]=(r[1]>>2);r[15]=0xd58u;return 0;
        case 0xd58u: /* and r1,r1,#0xe0 */
            r[1]=r[1]&224u;r[15]=0xd5cu;return 0;
        case 0xd5cu: /* ldr r3,[0x1240] */
            LOAD(r[3],4672u,4);r[15]=0xd60u;return 0;
        case 0xd60u: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0xd64u;return 0;
        case 0xd64u: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0xd68u;return 0;
        case 0xd68u: /* ldrb r1,[r5,#0x2] */
            LOAD(r[1],r[5]+2u,1);r[15]=0xd6cu;return 0;
        case 0xd6cu: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0xd70u;return 0;
        case 0xd70u: /* and r3,r0,#0x7 */
            r[3]=r[0]&7u;r[15]=0xd74u;return 0;
        case 0xd74u: /* add r3,r3,#0x1 */
            r[3]=r[3]+1u;r[15]=0xd78u;return 0;
        case 0xd78u: /* mul r1,r3,r1 */
            r[1]=r[3]*r[1];r[15]=0xd7cu;return 0;
        case 0xd7cu: /* sub r1,r1,#0x1 */
            r[1]=r[1]-1u;r[15]=0xd80u;return 0;
        case 0xd80u: /* mov r1,r1, lsr #0x7 */
            r[1]=(r[1]>>7);r[15]=0xd84u;return 0;
        case 0xd84u: /* and r1,r1,#0x7 */
            r[1]=r[1]&7u;r[15]=0xd88u;return 0;
        case 0xd88u: /* ldr r3,[0x1244] */
            LOAD(r[3],4676u,4);r[15]=0xd8cu;return 0;
        case 0xd8cu: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0xd90u;return 0;
        case 0xd90u: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0xd94u;return 0;
        case 0xd94u: /* str r0,[r8,#0x1c] */
            STORE(r[8]+28u,r[0],4);r[15]=0xd98u;return 0;
        case 0xd98u: /* ldr r0,[r2,#0x10] */
            LOAD(r[0],r[2]+16u,4);r[15]=0xd9cu;return 0;
        case 0xd9cu: /* and r1,r0,#0xf */
            r[1]=r[0]&15u;r[15]=0xda0u;return 0;
        case 0xda0u: /* ldr r3,[0x121c] */
            LOAD(r[3],4636u,4);r[15]=0xda4u;return 0;
        case 0xda4u: /* ldrb r3,[r3,#0x26] */
            LOAD(r[3],r[3]+38u,1);r[15]=0xda8u;return 0;
        case 0xda8u: /* mov r3,r3, lsr #0x3 */
            r[3]=(r[3]>>3);r[15]=0xdacu;return 0;
        case 0xdacu: /* and r3,r3,#0x1f */
            r[3]=r[3]&31u;r[15]=0xdb0u;return 0;
        case 0xdb0u: /* sub r3,r3,#0x10 */
            r[3]=r[3]-16u;r[15]=0xdb4u;return 0;
        case 0xdb4u: /* ldrb r1,[r5,#0x36] */
            LOAD(r[1],r[5]+54u,1);r[15]=0xdb8u;return 0;
        case 0xdb8u: /* movs r0,r1 */
            r[0]=r[1];logic_flags(state,r[0],0,0);r[15]=0xdbcu;return 0;
        case 0xdbcu: /* ldreq r0,[r2,#0x10] */
            if(state->cpsr&0x40000000u){LOAD(r[0],r[2]+16u,4);}r[15]=0xdc0u;return 0;
        case 0xdc0u: /* ldr r1,[r2,#0x10] */
            LOAD(r[1],r[2]+16u,4);r[15]=0xdc4u;return 0;
        case 0xdc4u: /* and r1,r1,#0xf */
            r[1]=r[1]&15u;r[15]=0xdc8u;return 0;
        case 0xdc8u: /* and r0,r0,#0xf0 */
            r[0]=r[0]&240u;r[15]=0xdccu;return 0;
        case 0xdccu: /* mov r0,r0, lsr #0x4 */
            r[0]=(r[0]>>4);r[15]=0xdd0u;return 0;
        case 0xdd0u: /* adds r0,r0,r3 */
            r[0]=addition(state,r[0],r[3]);r[15]=0xdd4u;return 0;
        case 0xdd4u: /* movmi r0,#0x0 */
            if(state->cpsr&0x80000000u){r[0]=0u;}r[15]=0xdd8u;return 0;
        case 0xdd8u: /* tst r0,#0xf0 */
            temporary=r[0]&240u;logic_flags(state,temporary,0,240u);r[15]=0xddcu;return 0;
        case 0xddcu: /* movne r0,#0xf */
            if(!(state->cpsr&0x40000000u)){r[0]=15u;}r[15]=0xde0u;return 0;
        case 0xde0u: /* and r0,r0,#0xf */
            r[0]=r[0]&15u;r[15]=0xde4u;return 0;
        case 0xde4u: /* orr r1,r1,r0, lsl #0x4 */
            r[1]=r[1]|(r[0]<<4);r[15]=0xde8u;return 0;
        case 0xde8u: /* str r1,[r8,#0x20] */
            STORE(r[8]+32u,r[1],4);r[15]=0xdecu;return 0;
        case 0xdecu: /* ldr r0,[r2,#0x14] */
            LOAD(r[0],r[2]+20u,4);r[15]=0xdf0u;return 0;
        case 0xdf0u: /* and r1,r0,#0x1f */
            r[1]=r[0]&31u;r[15]=0xdf4u;return 0;
        case 0xdf4u: /* ldrb r3,[r5,#0x27] */
            LOAD(r[3],r[5]+39u,1);r[15]=0xdf8u;return 0;
        case 0xdf8u: /* sub r3,r3,#0x40 */
            r[3]=r[3]-64u;r[15]=0xdfcu;return 0;
        case 0xdfcu: /* adds r1,r1,r3 */
            r[1]=addition(state,r[1],r[3]);r[15]=0xe00u;return 0;
        case 0xe00u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xe04u;return 0;
        case 0xe04u: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0xe08u;return 0;
        case 0xe08u: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0xe0cu;return 0;
        case 0xe0cu: /* ldrb r3,[r5,#0x3d] */
            LOAD(r[3],r[5]+61u,1);r[15]=0xe10u;return 0;
        case 0xe10u: /* tst r3,#0xff */
            temporary=r[3]&255u;logic_flags(state,temporary,0,255u);r[15]=0xe14u;return 0;
        case 0xe14u: /* andeq r3,r0,#0x20 */
            if(state->cpsr&0x40000000u){r[3]=r[0]&32u;}r[15]=0xe18u;return 0;
        case 0xe18u: /* beq 0x00000e28 */
            if(state->cpsr&0x40000000u){r[15]=3624u;}else{r[15]=0xe1cu;}return 0;
        case 0xe1cu: /* tst r3,#0x40 */
            temporary=r[3]&64u;logic_flags(state,temporary,0,64u);r[15]=0xe20u;return 0;
        case 0xe20u: /* movne r3,#0x0 */
            if(!(state->cpsr&0x40000000u)){r[3]=0u;}r[15]=0xe24u;return 0;
        case 0xe24u: /* moveq r3,#0x20 */
            if(state->cpsr&0x40000000u){r[3]=32u;}r[15]=0xe28u;return 0;
        case 0xe28u: /* orr r1,r1,r3 */
            r[1]=r[1]|r[3];r[15]=0xe2cu;return 0;
        case 0xe2cu: /* str r1,[0x1230] */
            STORE(4656u,r[1],4);r[15]=0xe30u;return 0;
        case 0xe30u: /* ldr r4,[r5,#0x40] */
            LOAD(r[4],r[5]+64u,4);r[15]=0xe34u;return 0;
        case 0xe34u: /* mov r0,r0, ror #0x10 */
            r[0]=((r[0]>>16)|(r[0]<<16));r[15]=0xe38u;return 0;
        case 0xe38u: /* ldr r3,[r5,#0x28] */
            LOAD(r[3],r[5]+40u,4);r[15]=0xe3cu;return 0;
        case 0xe3cu: /* ldr r1,[0x1234] */
            LOAD(r[1],4660u,4);r[15]=0xe40u;return 0;
        case 0xe40u: /* and r3,r3,r1 */
            r[3]=r[3]&r[1];r[15]=0xe44u;return 0;
        case 0xe44u: /* sub r3,r3,#0x2000 */
            r[3]=r[3]-8192u;r[15]=0xe48u;return 0;
        case 0xe48u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0xe4cu;return 0;
        case 0xe4cu: /* add r1,r1,r3 */
            r[1]=r[1]+r[3];r[15]=0xe50u;return 0;
        case 0xe50u: /* adds r1,r1,r4 */
            r[1]=addition(state,r[1],r[4]);r[15]=0xe54u;return 0;
        case 0xe54u: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0xe58u;return 0;
        case 0xe58u: /* subs r3,r1,#0x8 */
            r[3]=subtract(state,r[1],8u);r[15]=0xe5cu;return 0;
        case 0xe5cu: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0xe60u;return 0;
        case 0xe60u: /* add r3,r1,#0x8 */
            r[3]=r[1]+8u;r[15]=0xe64u;return 0;
        case 0xe64u: /* tst r3,#0xe000 */
            temporary=r[3]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0xe68u;return 0;
        case 0xe68u: /* ldrne r1,[0x1238] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],4664u,4);}r[15]=0xe6cu;return 0;
        case 0xe6cu: /* str r1,[r8,#0x2c] */
            STORE(r[8]+44u,r[1],4);r[15]=0xe70u;return 0;
        case 0xe70u: /* ldr r0,[r2,#0x18] */
            LOAD(r[0],r[2]+24u,4);r[15]=0xe74u;return 0;
        case 0xe74u: /* ldr r3,[r5,#0x28] */
            LOAD(r[3],r[5]+40u,4);r[15]=0xe78u;return 0;
        case 0xe78u: /* ldr r1,[0x1234] */
            LOAD(r[1],4660u,4);r[15]=0xe7cu;return 0;
        case 0xe7cu: /* mov r3,r3, ror #0x10 */
            r[3]=((r[3]>>16)|(r[3]<<16));r[15]=0xe80u;return 0;
        case 0xe80u: /* and r3,r3,r1 */
            r[3]=r[3]&r[1];r[15]=0xe84u;return 0;
        case 0xe84u: /* sub r3,r3,#0x2000 */
            r[3]=r[3]-8192u;r[15]=0xe88u;return 0;
        case 0xe88u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0xe8cu;return 0;
        case 0xe8cu: /* add r1,r1,r3 */
            r[1]=r[1]+r[3];r[15]=0xe90u;return 0;
        case 0xe90u: /* adds r1,r1,r4 */
            r[1]=addition(state,r[1],r[4]);r[15]=0xe94u;return 0;
        case 0xe94u: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0xe98u;return 0;
        case 0xe98u: /* subs r3,r1,#0x8 */
            r[3]=subtract(state,r[1],8u);r[15]=0xe9cu;return 0;
        case 0xe9cu: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0xea0u;return 0;
        case 0xea0u: /* add r3,r1,#0x8 */
            r[3]=r[1]+8u;r[15]=0xea4u;return 0;
        case 0xea4u: /* tst r3,#0xe000 */
            temporary=r[3]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0xea8u;return 0;
        case 0xea8u: /* ldrne r1,[0x1238] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],4664u,4);}r[15]=0xeacu;return 0;
        case 0xeacu: /* str r1,[r8,#0x30] */
            STORE(r[8]+48u,r[1],4);r[15]=0xeb0u;return 0;
        case 0xeb0u: /* mov r0,r0, ror #0x10 */
            r[0]=((r[0]>>16)|(r[0]<<16));r[15]=0xeb4u;return 0;
        case 0xeb4u: /* ldr r3,[r5,#0x2c] */
            LOAD(r[3],r[5]+44u,4);r[15]=0xeb8u;return 0;
        case 0xeb8u: /* ldr r1,[0x1234] */
            LOAD(r[1],4660u,4);r[15]=0xebcu;return 0;
        case 0xebcu: /* and r3,r3,r1 */
            r[3]=r[3]&r[1];r[15]=0xec0u;return 0;
        case 0xec0u: /* sub r3,r3,#0x2000 */
            r[3]=r[3]-8192u;r[15]=0xec4u;return 0;
        case 0xec4u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0xec8u;return 0;
        case 0xec8u: /* add r1,r1,r3 */
            r[1]=r[1]+r[3];r[15]=0xeccu;return 0;
        case 0xeccu: /* adds r1,r1,r4 */
            r[1]=addition(state,r[1],r[4]);r[15]=0xed0u;return 0;
        case 0xed0u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xed4u;return 0;
        case 0xed4u: /* subs r3,r1,#0x8 */
            r[3]=subtract(state,r[1],8u);r[15]=0xed8u;return 0;
        case 0xed8u: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0xedcu;return 0;
        case 0xedcu: /* add r3,r1,#0x8 */
            r[3]=r[1]+8u;r[15]=0xee0u;return 0;
        case 0xee0u: /* tst r3,#0xe000 */
            temporary=r[3]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0xee4u;return 0;
        case 0xee4u: /* ldrne r1,[0x1238] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],4664u,4);}r[15]=0xee8u;return 0;
        case 0xee8u: /* str r1,[r8,#0x34] */
            STORE(r[8]+52u,r[1],4);r[15]=0xeecu;return 0;
        case 0xeecu: /* ldr r0,[r2,#0x1c] */
            LOAD(r[0],r[2]+28u,4);r[15]=0xef0u;return 0;
        case 0xef0u: /* ldr r3,[r5,#0x2c] */
            LOAD(r[3],r[5]+44u,4);r[15]=0xef4u;return 0;
        case 0xef4u: /* mov r3,r3, ror #0x10 */
            r[3]=((r[3]>>16)|(r[3]<<16));r[15]=0xef8u;return 0;
        case 0xef8u: /* ldr r1,[0x1234] */
            LOAD(r[1],4660u,4);r[15]=0xefcu;return 0;
        case 0xefcu: /* and r3,r3,r1 */
            r[3]=r[3]&r[1];r[15]=0xf00u;return 0;
        case 0xf00u: /* sub r3,r3,#0x2000 */
            r[3]=r[3]-8192u;r[15]=0xf04u;return 0;
        case 0xf04u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0xf08u;return 0;
        case 0xf08u: /* add r1,r1,r3 */
            r[1]=r[1]+r[3];r[15]=0xf0cu;return 0;
        case 0xf0cu: /* adds r1,r1,r4 */
            r[1]=addition(state,r[1],r[4]);r[15]=0xf10u;return 0;
        case 0xf10u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xf14u;return 0;
        case 0xf14u: /* subs r3,r1,#0x8 */
            r[3]=subtract(state,r[1],8u);r[15]=0xf18u;return 0;
        case 0xf18u: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0xf1cu;return 0;
        case 0xf1cu: /* add r3,r1,#0x8 */
            r[3]=r[1]+8u;r[15]=0xf20u;return 0;
        case 0xf20u: /* tst r3,#0xe000 */
            temporary=r[3]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0xf24u;return 0;
        case 0xf24u: /* ldrne r1,[0x1238] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],4664u,4);}r[15]=0xf28u;return 0;
        case 0xf28u: /* str r1,[r8,#0x38] */
            STORE(r[8]+56u,r[1],4);r[15]=0xf2cu;return 0;
        case 0xf2cu: /* mov r0,r0, ror #0x10 */
            r[0]=((r[0]>>16)|(r[0]<<16));r[15]=0xf30u;return 0;
        case 0xf30u: /* ldr r3,[r5,#0x30] */
            LOAD(r[3],r[5]+48u,4);r[15]=0xf34u;return 0;
        case 0xf34u: /* ldr r1,[0x1234] */
            LOAD(r[1],4660u,4);r[15]=0xf38u;return 0;
        case 0xf38u: /* and r3,r3,r1 */
            r[3]=r[3]&r[1];r[15]=0xf3cu;return 0;
        case 0xf3cu: /* sub r3,r3,#0x2000 */
            r[3]=r[3]-8192u;r[15]=0xf40u;return 0;
        case 0xf40u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0xf44u;return 0;
        case 0xf44u: /* add r1,r1,r3 */
            r[1]=r[1]+r[3];r[15]=0xf48u;return 0;
        case 0xf48u: /* adds r1,r1,r4 */
            r[1]=addition(state,r[1],r[4]);r[15]=0xf4cu;return 0;
        case 0xf4cu: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xf50u;return 0;
        case 0xf50u: /* subs r3,r1,#0x8 */
            r[3]=subtract(state,r[1],8u);r[15]=0xf54u;return 0;
        case 0xf54u: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0xf58u;return 0;
        case 0xf58u: /* add r3,r1,#0x8 */
            r[3]=r[1]+8u;r[15]=0xf5cu;return 0;
        case 0xf5cu: /* tst r3,#0xe000 */
            temporary=r[3]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0xf60u;return 0;
        case 0xf60u: /* ldrne r1,[0x1238] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],4664u,4);}r[15]=0xf64u;return 0;
        case 0xf64u: /* str r1,[r8,#0x3c] */
            STORE(r[8]+60u,r[1],4);r[15]=0xf68u;return 0;
        case 0xf68u: /* ldr r0,[r2,#0x20] */
            LOAD(r[0],r[2]+32u,4);r[15]=0xf6cu;return 0;
        case 0xf6cu: /* ldrb r3,[r5,#0x32] */
            LOAD(r[3],r[5]+50u,1);r[15]=0xf70u;return 0;
        case 0xf70u: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xf74u;return 0;
        case 0xf74u: /* mov r1,r0, lsr #0x8 */
            r[1]=(r[0]>>8);r[15]=0xf78u;return 0;
        case 0xf78u: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0xf7cu;return 0;
        case 0xf7cu: /* adds r1,r1,r3 */
            r[1]=addition(state,r[1],r[3]);r[15]=0xf80u;return 0;
        case 0xf80u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xf84u;return 0;
        case 0xf84u: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0xf88u;return 0;
        case 0xf88u: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0xf8cu;return 0;
        case 0xf8cu: /* and r0,r0,#0x1f */
            r[0]=r[0]&31u;r[15]=0xf90u;return 0;
        case 0xf90u: /* ldrb r3,[r5,#0x33] */
            LOAD(r[3],r[5]+51u,1);r[15]=0xf94u;return 0;
        case 0xf94u: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xf98u;return 0;
        case 0xf98u: /* adds r0,r0,r3 */
            r[0]=addition(state,r[0],r[3]);r[15]=0xf9cu;return 0;
        case 0xf9cu: /* movmi r0,#0x0 */
            if(state->cpsr&0x80000000u){r[0]=0u;}r[15]=0xfa0u;return 0;
        case 0xfa0u: /* tst r0,#0xe0 */
            temporary=r[0]&224u;logic_flags(state,temporary,0,224u);r[15]=0xfa4u;return 0;
        case 0xfa4u: /* movne r0,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[0]=31u;}r[15]=0xfa8u;return 0;
        case 0xfa8u: /* orr r1,r0,r1, lsl #0x8 */
            r[1]=r[0]|(r[1]<<8);r[15]=0xfacu;return 0;
        case 0xfacu: /* str r1,[r8,#0x40] */
            STORE(r[8]+64u,r[1],4);r[15]=0xfb0u;return 0;
        case 0xfb0u: /* ldr r0,[r2,#0x20] */
            LOAD(r[0],r[2]+32u,4);r[15]=0xfb4u;return 0;
        case 0xfb4u: /* mov r0,r0, ror #0x10 */
            r[0]=((r[0]>>16)|(r[0]<<16));r[15]=0xfb8u;return 0;
        case 0xfb8u: /* ldrb r3,[r5,#0x34] */
            LOAD(r[3],r[5]+52u,1);r[15]=0xfbcu;return 0;
        case 0xfbcu: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xfc0u;return 0;
        case 0xfc0u: /* mov r1,r0, lsr #0x8 */
            r[1]=(r[0]>>8);r[15]=0xfc4u;return 0;
        case 0xfc4u: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0xfc8u;return 0;
        case 0xfc8u: /* adds r1,r1,r3 */
            r[1]=addition(state,r[1],r[3]);r[15]=0xfccu;return 0;
        case 0xfccu: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0xfd0u;return 0;
        case 0xfd0u: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0xfd4u;return 0;
        case 0xfd4u: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0xfd8u;return 0;
        case 0xfd8u: /* and r0,r0,#0x1f */
            r[0]=r[0]&31u;r[15]=0xfdcu;return 0;
        case 0xfdcu: /* ldrb r3,[r5,#0x35] */
            LOAD(r[3],r[5]+53u,1);r[15]=0xfe0u;return 0;
        case 0xfe0u: /* sub r3,r3,#0x20 */
            r[3]=r[3]-32u;r[15]=0xfe4u;return 0;
        case 0xfe4u: /* adds r0,r0,r3 */
            r[0]=addition(state,r[0],r[3]);r[15]=0xfe8u;return 0;
        case 0xfe8u: /* movmi r0,#0x0 */
            if(state->cpsr&0x80000000u){r[0]=0u;}r[15]=0xfecu;return 0;
        case 0xfecu: /* tst r0,#0xe0 */
            temporary=r[0]&224u;logic_flags(state,temporary,0,224u);r[15]=0xff0u;return 0;
        case 0xff0u: /* movne r0,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[0]=31u;}r[15]=0xff4u;return 0;
        case 0xff4u: /* orr r1,r0,r1, lsl #0x8 */
            r[1]=r[0]|(r[1]<<8);r[15]=0xff8u;return 0;
        case 0xff8u: /* str r1,[r8,#0x44] */
            STORE(r[8]+68u,r[1],4);r[15]=0xffcu;return 0;
        case 0xffcu: /* ldrb r1,[r2,#0x2a] */
            LOAD(r[1],r[2]+42u,1);r[15]=0x1000u;return 0;
        case 0x1000u: /* strb r1,[r6,#0x1c] */
            STORE(r[6]+28u,r[1],1);r[15]=0x1004u;return 0;
        case 0x1004u: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x1008u;return 0;
        case 0x1008u: /* ldr r0,[r5,#0x18] */
            LOAD(r[0],r[5]+24u,4);r[15]=0x100cu;return 0;
        case 0x100cu: /* add r1,r1,r0 */
            r[1]=r[1]+r[0];r[15]=0x1010u;return 0;
        case 0x1010u: /* ldrb r0,[0x1210] */
            LOAD(r[0],4624u,1);r[15]=0x1014u;return 0;
        case 0x1014u: /* strb r0,[r6,#0x7] */
            STORE(r[6]+7u,r[0],1);r[15]=0x1018u;return 0;
        case 0x1018u: /* ldrb r0,[r1,r0] */
            LOAD(r[0],r[1]+r[0],1);r[15]=0x101cu;return 0;
        case 0x101cu: /* add r0,r0,#0x1 */
            r[0]=r[0]+1u;r[15]=0x1020u;return 0;
        case 0x1020u: /* ldrb r1,[r5,#0xa] */
            LOAD(r[1],r[5]+10u,1);r[15]=0x1024u;return 0;
        case 0x1024u: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x1028u;return 0;
        case 0x1028u: /* mul r0,r1,r0 */
            r[0]=r[1]*r[0];r[15]=0x102cu;return 0;
        case 0x102cu: /* ldrb r1,[r2,#0x15] */
            LOAD(r[1],r[2]+21u,1);r[15]=0x1030u;return 0;
        case 0x1030u: /* eor r1,r1,#0xff */
            r[1]=r[1]^255u;r[15]=0x1034u;return 0;
        case 0x1034u: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x1038u;return 0;
        case 0x1038u: /* mul r0,r1,r0 */
            r[0]=r[1]*r[0];r[15]=0x103cu;return 0;
        case 0x103cu: /* sub r0,r0,#0x1 */
            r[0]=r[0]-1u;r[15]=0x1040u;return 0;
        case 0x1040u: /* mov r0,r0, lsr #0xe */
            r[0]=(r[0]>>14);r[15]=0x1044u;return 0;
        case 0x1044u: /* and r0,r0,#0xff */
            r[0]=r[0]&255u;r[15]=0x1048u;return 0;
        case 0x1048u: /* strb r0,[r6,#0x8] */
            STORE(r[6]+8u,r[0],1);r[15]=0x104cu;return 0;
        case 0x104cu: /* ldr r3,[0x121c] */
            LOAD(r[3],4636u,4);r[15]=0x1050u;return 0;
        case 0x1050u: /* str r3,[r6,#0x18] */
            STORE(r[6]+24u,r[3],4);r[15]=0x1054u;return 0;
        case 0x1054u: /* ldrb r1,[r3,#0x1] */
            LOAD(r[1],r[3]+1u,1);r[15]=0x1058u;return 0;
        case 0x1058u: /* sub r1,r1,#0x80 */
            r[1]=r[1]-128u;r[15]=0x105cu;return 0;
        case 0x105cu: /* adds r1,r1,r0 */
            r[1]=addition(state,r[1],r[0]);r[15]=0x1060u;return 0;
        case 0x1060u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x1064u;return 0;
        case 0x1064u: /* bmi 0x00001070 */
            if(state->cpsr&0x80000000u){r[15]=4208u;}else{r[15]=0x1068u;}return 0;
        case 0x1068u: /* tst r1,#0x100 */
            temporary=r[1]&256u;logic_flags(state,temporary,1,256u);r[15]=0x106cu;return 0;
        case 0x106cu: /* movne r1,#0xff */
            if(!(state->cpsr&0x40000000u)){r[1]=255u;}r[15]=0x1070u;return 0;
        case 0x1070u: /* eor r1,r1,#0xff */
            r[1]=r[1]^255u;r[15]=0x1074u;return 0;
        case 0x1074u: /* and r1,r1,#0xff */
            r[1]=r[1]&255u;r[15]=0x1078u;return 0;
        case 0x1078u: /* ldr r0,[0x1230] */
            LOAD(r[0],4656u,4);r[15]=0x107cu;return 0;
        case 0x107cu: /* and r0,r0,#0x3f */
            r[0]=r[0]&63u;r[15]=0x1080u;return 0;
        case 0x1080u: /* orr r0,r0,r1, lsl #0x8 */
            r[0]=r[0]|(r[1]<<8);r[15]=0x1084u;return 0;
        case 0x1084u: /* str r0,[r8,#0x28] */
            STORE(r[8]+40u,r[0],4);r[15]=0x1088u;return 0;
        case 0x1088u: /* ldr r0,[r2,#0x12] */
            LOAD(r[0],r[2]+18u,4);r[15]=0x108cu;return 0;
        case 0x108cu: /* and r0,r0,#0x1f */
            r[0]=r[0]&31u;r[15]=0x1090u;return 0;
        case 0x1090u: /* cmp r0,#0x10 */
            (void)subtract(state,r[0],16u);r[15]=0x1094u;return 0;
        case 0x1094u: /* moveq r0,#0x0 */
            if(state->cpsr&0x40000000u){r[0]=0u;}r[15]=0x1098u;return 0;
        case 0x1098u: /* tst r0,#0x10 */
            temporary=r[0]&16u;logic_flags(state,temporary,0,16u);r[15]=0x109cu;return 0;
        case 0x109cu: /* subne r0,r0,#0x10 */
            if(!(state->cpsr&0x40000000u)){r[0]=r[0]-16u;}r[15]=0x10a0u;return 0;
        case 0x10a0u: /* eorne r0,r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=r[0]^255u;}r[15]=0x10a4u;return 0;
        case 0x10a4u: /* add r0,r0,#0x10 */
            r[0]=r[0]+16u;r[15]=0x10a8u;return 0;
        case 0x10a8u: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x10acu;return 0;
        case 0x10acu: /* ldrb r1,[r5,#0xb] */
            LOAD(r[1],r[5]+11u,1);r[15]=0x10b0u;return 0;
        case 0x10b0u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x10b4u;return 0;
        case 0x10b4u: /* moveq r0,r1 */
            if(state->cpsr&0x40000000u){r[0]=r[1];}r[15]=0x10b8u;return 0;
        case 0x10b8u: /* and r0,r0,#0x7f */
            r[0]=r[0]&127u;r[15]=0x10bcu;return 0;
        case 0x10bcu: /* ldr r3,[0x121c] */
            LOAD(r[3],4636u,4);r[15]=0x10c0u;return 0;
        case 0x10c0u: /* ldrb r1,[r3,#0x7] */
            LOAD(r[1],r[3]+7u,1);r[15]=0x10c4u;return 0;
        case 0x10c4u: /* and r1,r1,#0xff */
            r[1]=r[1]&255u;r[15]=0x10c8u;return 0;
        case 0x10c8u: /* sub r1,r1,#0x80 */
            r[1]=r[1]-128u;r[15]=0x10ccu;return 0;
        case 0x10ccu: /* adds r1,r1,r0 */
            r[1]=addition(state,r[1],r[0]);r[15]=0x10d0u;return 0;
        case 0x10d0u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x10d4u;return 0;
        case 0x10d4u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x10d8u;return 0;
        case 0x10d8u: /* movne r1,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[1]=127u;}r[15]=0x10dcu;return 0;
        case 0x10dcu: /* mov r1,r1, lsr #0x2 */
            r[1]=(r[1]>>2);r[15]=0x10e0u;return 0;
        case 0x10e0u: /* subs r1,r1,#0x10 */
            r[1]=subtract(state,r[1],16u);r[15]=0x10e4u;return 0;
        case 0x10e4u: /* eormi r1,r1,#0xff */
            if(state->cpsr&0x80000000u){r[1]=r[1]^255u;}r[15]=0x10e8u;return 0;
        case 0x10e8u: /* addmi r1,r1,#0x10 */
            if(state->cpsr&0x80000000u){r[1]=r[1]+16u;}r[15]=0x10ecu;return 0;
        case 0x10ecu: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0x10f0u;return 0;
        case 0x10f0u: /* ldrb r3,[r3,#0x24] */
            LOAD(r[3],r[3]+36u,1);r[15]=0x10f4u;return 0;
        case 0x10f4u: /* mov r3,r3, lsr #0x3 */
            r[3]=(r[3]>>3);r[15]=0x10f8u;return 0;
        case 0x10f8u: /* and r3,r3,#0x1f */
            r[3]=r[3]&31u;r[15]=0x10fcu;return 0;
        case 0x10fcu: /* sub r3,r3,#0x10 */
            r[3]=r[3]-16u;r[15]=0x1100u;return 0;
        case 0x1100u: /* ldr r0,[r2,#0x12] */
            LOAD(r[0],r[2]+18u,4);r[15]=0x1104u;return 0;
        case 0x1104u: /* mov r0,r0, lsr #0x8 */
            r[0]=(r[0]>>8);r[15]=0x1108u;return 0;
        case 0x1108u: /* and r0,r0,#0xf */
            r[0]=r[0]&15u;r[15]=0x110cu;return 0;
        case 0x110cu: /* adds r0,r0,r3 */
            r[0]=addition(state,r[0],r[3]);r[15]=0x1110u;return 0;
        case 0x1110u: /* movmi r0,#0x0 */
            if(state->cpsr&0x80000000u){r[0]=0u;}r[15]=0x1114u;return 0;
        case 0x1114u: /* tst r0,#0x10 */
            temporary=r[0]&16u;logic_flags(state,temporary,0,16u);r[15]=0x1118u;return 0;
        case 0x1118u: /* movne r0,#0xf */
            if(!(state->cpsr&0x40000000u)){r[0]=15u;}r[15]=0x111cu;return 0;
        case 0x111cu: /* and r0,r0,#0xf */
            r[0]=r[0]&15u;r[15]=0x1120u;return 0;
        case 0x1120u: /* orr r1,r1,r0, lsl #0x8 */
            r[1]=r[1]|(r[0]<<8);r[15]=0x1124u;return 0;
        case 0x1124u: /* str r1,[r8,#0x24] */
            STORE(r[8]+36u,r[1],4);r[15]=0x1128u;return 0;
        case 0x1128u: /* ldrb r0,[0x120c] */
            LOAD(r[0],4620u,1);r[15]=0x112cu;return 0;
        case 0x112cu: /* strb r0,[r6,#0x4] */
            STORE(r[6]+4u,r[0],1);r[15]=0x1130u;return 0;
        case 0x1130u: /* ldrb r0,[0x120d] */
            LOAD(r[0],4621u,1);r[15]=0x1134u;return 0;
        case 0x1134u: /* strb r0,[r6,#0x3e] */
            STORE(r[6]+62u,r[0],1);r[15]=0x1138u;return 0;
        case 0x1138u: /* ldrb r0,[0x120e] */
            LOAD(r[0],4622u,1);r[15]=0x113cu;return 0;
        case 0x113cu: /* strb r0,[r6,#0x5] */
            STORE(r[6]+5u,r[0],1);r[15]=0x1140u;return 0;
        case 0x1140u: /* ldr r3,[0x121c] */
            LOAD(r[3],4636u,4);r[15]=0x1144u;return 0;
        case 0x1144u: /* ldr r0,[r3,#0x10] */
            LOAD(r[0],r[3]+16u,4);r[15]=0x1148u;return 0;
        case 0x1148u: /* sub r0,r0,#0x8000 */
            r[0]=r[0]-32768u;r[15]=0x114cu;return 0;
        case 0x114cu: /* ldr r7,[r6,#0x14] */
            LOAD(r[7],r[6]+20u,4);r[15]=0x1150u;return 0;
        case 0x1150u: /* add r7,r7,r0 */
            r[7]=r[7]+r[0];r[15]=0x1154u;return 0;
        case 0x1154u: /* ldr r0,[r5,#0x4] */
            LOAD(r[0],r[5]+4u,4);r[15]=0x1158u;return 0;
        case 0x1158u: /* ldr r1,[0x123c] */
            LOAD(r[1],4668u,4);r[15]=0x115cu;return 0;
        case 0x115cu: /* tst r0,r1 */
            temporary=r[0]&r[1];logic_flags(state,temporary,0,r[1]);r[15]=0x1160u;return 0;
        case 0x1160u: /* ldrbne r1,[r6,#0xb] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[1],r[6]+11u,1);}r[15]=0x1164u;return 0;
        case 0x1164u: /* ldrbeq r1,[r6,#0xa] */
            if(state->cpsr&0x40000000u){LOAD(r[1],r[6]+10u,1);}r[15]=0x1168u;return 0;
        case 0x1168u: /* mul r0,r1,r0 */
            r[0]=r[1]*r[0];r[15]=0x116cu;return 0;
        case 0x116cu: /* mov r0,r0, lsr #0x5 */
            r[0]=(r[0]>>5);r[15]=0x1170u;return 0;
        case 0x1170u: /* ldr r1,[0x1234] */
            LOAD(r[1],4660u,4);r[15]=0x1174u;return 0;
        case 0x1174u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0x1178u;return 0;
        case 0x1178u: /* str r1,[r6,#0xc] */
            STORE(r[6]+12u,r[1],4);r[15]=0x117cu;return 0;
        case 0x117cu: /* add r7,r7,r1 */
            r[7]=r[7]+r[1];r[15]=0x1180u;return 0;
        case 0x1180u: /* stmdb sp!,{r2,r5,r6,lr} */
            STORE(r[13]-16u,r[2],4);STORE(r[13]-12u,r[5],4);STORE(r[13]-8u,r[6],4);STORE(r[13]-4u,r[14],4);r[13]-=16u;r[15]=0x1184u;return 0;
        case 0x1184u: /* mov r5,r6 */
            r[5]=r[6];r[15]=0x1188u;return 0;
        case 0x1188u: /* mov r6,r2 */
            r[6]=r[2];r[15]=0x118cu;return 0;
        case 0x118cu: /* bl 0x000014bc */
            r[14]=0x1190u;r[15]=5308u;return 0;
        case 0x1190u: /* ldmia sp!,{r2,r5,r6,lr} */
            LOAD(r[2],r[13]+0u,4);LOAD(r[5],r[13]+4u,4);LOAD(r[6],r[13]+8u,4);LOAD(r[14],r[13]+12u,4);r[13]+=16u;r[15]=0x1194u;return 0;
        case 0x1194u: /* ldrb r0,[r6,#0x0] */
            LOAD(r[0],r[6]+0u,1);r[15]=0x1198u;return 0;
        case 0x1198u: /* orr r0,r0,#0x80 */
            r[0]=r[0]|128u;r[15]=0x119cu;return 0;
        case 0x119cu: /* ldrb r1,[r5,#0x26] */
            LOAD(r[1],r[5]+38u,1);r[15]=0x11a0u;return 0;
        case 0x11a0u: /* tst r1,#0x40 */
            temporary=r[1]&64u;logic_flags(state,temporary,0,64u);r[15]=0x11a4u;return 0;
        case 0x11a4u: /* orrne r0,r0,#0x40 */
            if(!(state->cpsr&0x40000000u)){r[0]=r[0]|64u;}r[15]=0x11a8u;return 0;
        case 0x11a8u: /* strb r0,[r6,#0x0] */
            STORE(r[6]+0u,r[0],1);r[15]=0x11acu;return 0;
        case 0x11acu: /* mov r0,#0x1 */
            r[0]=1u;r[15]=0x11b0u;return 0;
        case 0x11b0u: /* strb r0,[r6,#0x3f] */
            STORE(r[6]+63u,r[0],1);r[15]=0x11b4u;return 0;
        case 0x11b4u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x11b8u;return 0;
        case 0x11b8u: /* strb r0,[r6,#0x3] */
            STORE(r[6]+3u,r[0],1);r[15]=0x11bcu;return 0;
        case 0x11bcu: /* ldr r0,[0x122c] */
            LOAD(r[0],4652u,4);r[15]=0x11c0u;return 0;
        case 0x11c0u: /* ldr r1,[0x1228] */
            LOAD(r[1],4648u,4);r[15]=0x11c4u;return 0;
        case 0x11c4u: /* and r0,r0,r1 */
            r[0]=r[0]&r[1];r[15]=0x11c8u;return 0;
        case 0x11c8u: /* ldr r1,[r6,#0x38] */
            LOAD(r[1],r[6]+56u,4);r[15]=0x11ccu;return 0;
        case 0x11ccu: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x11d0u;return 0;
        case 0x11d0u: /* orreq r0,r0,#0xc000 */
            if(state->cpsr&0x40000000u){r[0]=r[0]|49152u;}r[15]=0x11d4u;return 0;
        case 0x11d4u: /* ldr r1,[r8,#0x1c] */
            LOAD(r[1],r[8]+28u,4);r[15]=0x11d8u;return 0;
        case 0x11d8u: /* bic r1,r1,#0x8000 */
            r[1]=r[1]&~32768u;r[15]=0x11dcu;return 0;
        case 0x11dcu: /* str r1,[r8,#0x1c] */
            STORE(r[8]+28u,r[1],4);r[15]=0x11e0u;return 0;
        case 0x11e0u: /* str r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],4);r[15]=0x11e4u;return 0;
        case 0x11e4u: /* b 0x000011f4 */
            r[15]=4596u;return 0;
        case 0x11e8u: /* add r2,r2,#0x30 */
            r[2]=r[2]+48u;r[15]=0x11ecu;return 0;
        case 0x11ecu: /* subs r11,r11,#0x1 */
            r[11]=subtract(state,r[11],1u);r[15]=0x11f0u;return 0;
        case 0x11f0u: /* bne 0x00000b80 */
            if(!(state->cpsr&0x40000000u)){r[15]=2944u;}else{r[15]=0x11f4u;}return 0;
        case 0x11f4u: /* ldr r1,[0x1220] */
            LOAD(r[1],4640u,4);r[15]=0x11f8u;return 0;
        case 0x11f8u: /* add r1,r1,#0x4 */
            r[1]=r[1]+4u;r[15]=0x11fcu;return 0;
        case 0x11fcu: /* str r1,[0x1220] */
            STORE(4640u,r[1],4);r[15]=0x1200u;return 0;
        case 0x1200u: /* subs r12,r12,#0x1 */
            r[12]=subtract(state,r[12],1u);r[15]=0x1204u;return 0;
        case 0x1204u: /* bne 0x00000b24 */
            if(!(state->cpsr&0x40000000u)){r[15]=2852u;}else{r[15]=0x1208u;}return 0;
        case 0x1208u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x124cu: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x1250u;return 0;
        case 0x1250u: /* mov r10,#0x30 */
            r[10]=48u;r[15]=0x1254u;return 0;
        case 0x1254u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x1258u;return 0;
        case 0x1258u: /* adr r1,0x1358 */
            r[1]=4952u;r[15]=0x125cu;return 0;
        case 0x125cu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x1260u;return 0;
        case 0x1260u: /* str r0,[r1],#0x4 */
            STORE(r[1],r[0],4);r[1]+=4u;r[15]=0x1264u;return 0;
        case 0x1264u: /* str r0,[r1],#0x4 */
            STORE(r[1],r[0],4);r[1]+=4u;r[15]=0x1268u;return 0;
        case 0x1268u: /* str r0,[r1],#0x4 */
            STORE(r[1],r[0],4);r[1]+=4u;r[15]=0x126cu;return 0;
        case 0x126cu: /* ldr r0,[r6,#0x0] */
            LOAD(r[0],r[6]+0u,4);r[15]=0x1270u;return 0;
        case 0x1270u: /* movs r0,r0 */
            r[0]=r[0];logic_flags(state,r[0],0,0);r[15]=0x1274u;return 0;
        case 0x1274u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x1278u;}return 0;
        case 0x1278u: /* ldrb r0,[r6,#0x3e] */
            LOAD(r[0],r[6]+62u,1);r[15]=0x127cu;return 0;
        case 0x127cu: /* ldrb r3,[0x120d] */
            LOAD(r[3],4621u,1);r[15]=0x1280u;return 0;
        case 0x1280u: /* cmp r3,r0 */
            (void)subtract(state,r[3],r[0]);r[15]=0x1284u;return 0;
        case 0x1284u: /* bmi 0x00001330 */
            if(state->cpsr&0x80000000u){r[15]=4912u;}else{r[15]=0x1288u;}return 0;
        case 0x1288u: /* adr r1,0x1358 */
            r[1]=4952u;r[15]=0x128cu;return 0;
        case 0x128cu: /* ldr r3,[r1,#0x0] */
            LOAD(r[3],r[1]+0u,4);r[15]=0x1290u;return 0;
        case 0x1290u: /* movs r3,r3 */
            r[3]=r[3];logic_flags(state,r[3],0,0);r[15]=0x1294u;return 0;
        case 0x1294u: /* beq 0x00001308 */
            if(state->cpsr&0x40000000u){r[15]=4872u;}else{r[15]=0x1298u;}return 0;
        case 0x1298u: /* ldrb r0,[r1,#0x8] */
            LOAD(r[0],r[1]+8u,1);r[15]=0x129cu;return 0;
        case 0x129cu: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x12a0u;return 0;
        case 0x12a0u: /* bne 0x000012d4 */
            if(!(state->cpsr&0x40000000u)){r[15]=4820u;}else{r[15]=0x12a4u;}return 0;
        case 0x12a4u: /* ldrb r3,[r6,#0x0] */
            LOAD(r[3],r[6]+0u,1);r[15]=0x12a8u;return 0;
        case 0x12a8u: /* tst r3,#0xff */
            temporary=r[3]&255u;logic_flags(state,temporary,0,255u);r[15]=0x12acu;return 0;
        case 0x12acu: /* beq 0x00001308 */
            if(state->cpsr&0x40000000u){r[15]=4872u;}else{r[15]=0x12b0u;}return 0;
        case 0x12b0u: /* ldrb r3,[r6,#0x3e] */
            LOAD(r[3],r[6]+62u,1);r[15]=0x12b4u;return 0;
        case 0x12b4u: /* ldrb r0,[r1,#0x9] */
            LOAD(r[0],r[1]+9u,1);r[15]=0x12b8u;return 0;
        case 0x12b8u: /* cmp r0,r3 */
            (void)subtract(state,r[0],r[3]);r[15]=0x12bcu;return 0;
        case 0x12bcu: /* bmi 0x00001330 */
            if(state->cpsr&0x80000000u){r[15]=4912u;}else{r[15]=0x12c0u;}return 0;
        case 0x12c0u: /* ldrb r3,[r6,#0x3f] */
            LOAD(r[3],r[6]+63u,1);r[15]=0x12c4u;return 0;
        case 0x12c4u: /* ldrb r0,[r1,#0xa] */
            LOAD(r[0],r[1]+10u,1);r[15]=0x12c8u;return 0;
        case 0x12c8u: /* cmp r3,r0 */
            (void)subtract(state,r[3],r[0]);r[15]=0x12ccu;return 0;
        case 0x12ccu: /* bmi 0x00001330 */
            if(state->cpsr&0x80000000u){r[15]=4912u;}else{r[15]=0x12d0u;}return 0;
        case 0x12d0u: /* b 0x00001308 */
            r[15]=4872u;return 0;
        case 0x12d4u: /* ldrb r3,[r6,#0x0] */
            LOAD(r[3],r[6]+0u,1);r[15]=0x12d8u;return 0;
        case 0x12d8u: /* tst r3,#0xff */
            temporary=r[3]&255u;logic_flags(state,temporary,0,255u);r[15]=0x12dcu;return 0;
        case 0x12dcu: /* bne 0x00001330 */
            if(!(state->cpsr&0x40000000u)){r[15]=4912u;}else{r[15]=0x12e0u;}return 0;
        case 0x12e0u: /* ldrb r3,[r6,#0x3] */
            LOAD(r[3],r[6]+3u,1);r[15]=0x12e4u;return 0;
        case 0x12e4u: /* ldrb r0,[r1,#0x8] */
            LOAD(r[0],r[1]+8u,1);r[15]=0x12e8u;return 0;
        case 0x12e8u: /* cmp r0,r3 */
            (void)subtract(state,r[0],r[3]);r[15]=0x12ecu;return 0;
        case 0x12ecu: /* beq 0x000012f8 */
            if(state->cpsr&0x40000000u){r[15]=4856u;}else{r[15]=0x12f0u;}return 0;
        case 0x12f0u: /* bmi 0x00001330 */
            if(state->cpsr&0x80000000u){r[15]=4912u;}else{r[15]=0x12f4u;}return 0;
        case 0x12f4u: /* b 0x00001308 */
            r[15]=4872u;return 0;
        case 0x12f8u: /* ldrb r3,[r6,#0x3e] */
            LOAD(r[3],r[6]+62u,1);r[15]=0x12fcu;return 0;
        case 0x12fcu: /* ldrb r0,[r1,#0x9] */
            LOAD(r[0],r[1]+9u,1);r[15]=0x1300u;return 0;
        case 0x1300u: /* cmp r3,r0 */
            (void)subtract(state,r[3],r[0]);r[15]=0x1304u;return 0;
        case 0x1304u: /* bmi 0x00001330 */
            if(state->cpsr&0x80000000u){r[15]=4912u;}else{r[15]=0x1308u;}return 0;
        case 0x1308u: /* str r6,[r1,#0x0] */
            STORE(r[1]+0u,r[6],4);r[15]=0x130cu;return 0;
        case 0x130cu: /* str r8,[r1,#0x4] */
            STORE(r[1]+4u,r[8],4);r[15]=0x1310u;return 0;
        case 0x1310u: /* ldrb r3,[r6,#0x3] */
            LOAD(r[3],r[6]+3u,1);r[15]=0x1314u;return 0;
        case 0x1314u: /* strb r3,[r1,#0x8] */
            STORE(r[1]+8u,r[3],1);r[15]=0x1318u;return 0;
        case 0x1318u: /* ldrb r3,[r6,#0x3e] */
            LOAD(r[3],r[6]+62u,1);r[15]=0x131cu;return 0;
        case 0x131cu: /* strb r3,[r1,#0x9] */
            STORE(r[1]+9u,r[3],1);r[15]=0x1320u;return 0;
        case 0x1320u: /* ldrb r3,[r6,#0x3f] */
            LOAD(r[3],r[6]+63u,1);r[15]=0x1324u;return 0;
        case 0x1324u: /* strb r3,[r1,#0xa] */
            STORE(r[1]+10u,r[3],1);r[15]=0x1328u;return 0;
        case 0x1328u: /* ldrb r3,[r6,#0x4] */
            LOAD(r[3],r[6]+4u,1);r[15]=0x132cu;return 0;
        case 0x132cu: /* strb r3,[r1,#0xb] */
            STORE(r[1]+11u,r[3],1);r[15]=0x1330u;return 0;
        case 0x1330u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x1334u;return 0;
        case 0x1334u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x1338u;return 0;
        case 0x1338u: /* subs r10,r10,#0x1 */
            r[10]=subtract(state,r[10],1u);r[15]=0x133cu;return 0;
        case 0x133cu: /* bne 0x0000126c */
            if(!(state->cpsr&0x40000000u)){r[15]=4716u;}else{r[15]=0x1340u;}return 0;
        case 0x1340u: /* ldr r6,[r1,#0x0] */
            LOAD(r[6],r[1]+0u,4);r[15]=0x1344u;return 0;
        case 0x1344u: /* ldr r8,[r1,#0x4] */
            LOAD(r[8],r[1]+4u,4);r[15]=0x1348u;return 0;
        case 0x1348u: /* movs r6,r6 */
            r[6]=r[6];logic_flags(state,r[6],0,0);r[15]=0x134cu;return 0;
        case 0x134cu: /* movne r0,#0x0 */
            if(!(state->cpsr&0x40000000u)){r[0]=0u;}r[15]=0x1350u;return 0;
        case 0x1350u: /* moveq r0,#0xff */
            if(state->cpsr&0x40000000u){r[0]=255u;}r[15]=0x1354u;return 0;
        case 0x1354u: /* mov pc,lr */
            r[15]=r[14];return 0;
        default:*handled=0;return 0;
 }}
}

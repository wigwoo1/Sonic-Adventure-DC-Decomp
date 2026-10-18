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
int sa_arm_sound_dispatch_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
        case 0x17b0u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x17b4u;return 0;
        case 0x17b4u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x17b8u;return 0;
        case 0x17b8u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x17bcu;return 0;
        case 0x17bcu: /* ldrb r1,[r6,#0x4] */
            LOAD(r[1],r[6]+4u,1);r[15]=0x17c0u;return 0;
        case 0x17c0u: /* cmp r1,r0 */
            (void)subtract(state,r[1],r[0]);r[15]=0x17c4u;return 0;
        case 0x17c4u: /* bne 0x000017e8 */
            if(!(state->cpsr&0x40000000u)){r[15]=6120u;}else{r[15]=0x17c8u;}return 0;
        case 0x17c8u: /* ldrb r1,[r6,#0x0] */
            LOAD(r[1],r[6]+0u,1);r[15]=0x17ccu;return 0;
        case 0x17ccu: /* bic r1,r1,#0x80 */
            r[1]=r[1]&~128u;r[15]=0x17d0u;return 0;
        case 0x17d0u: /* strb r1,[r6,#0x0] */
            STORE(r[6]+0u,r[1],1);r[15]=0x17d4u;return 0;
        case 0x17d4u: /* ldr r1,[r8,#0x0] */
            LOAD(r[1],r[8]+0u,4);r[15]=0x17d8u;return 0;
        case 0x17d8u: /* ldr r2,[0x17fc] */
            LOAD(r[2],6140u,4);r[15]=0x17dcu;return 0;
        case 0x17dcu: /* and r1,r1,r2 */
            r[1]=r[1]&r[2];r[15]=0x17e0u;return 0;
        case 0x17e0u: /* orr r1,r1,#0x8000 */
            r[1]=r[1]|32768u;r[15]=0x17e4u;return 0;
        case 0x17e4u: /* str r1,[r8,#0x0] */
            STORE(r[8]+0u,r[1],4);r[15]=0x17e8u;return 0;
        case 0x17e8u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x17ecu;return 0;
        case 0x17ecu: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x17f0u;return 0;
        case 0x17f0u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x17f4u;return 0;
        case 0x17f4u: /* bne 0x000017bc */
            if(!(state->cpsr&0x40000000u)){r[15]=6076u;}else{r[15]=0x17f8u;}return 0;
        case 0x17f8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x48e8u: /* ldr r10,[0x4b2c] */
            LOAD(r[10],19244u,4);r[15]=0x48ecu;return 0;
        case 0x48ecu: /* mov r11,#0x20 */
            r[11]=32u;r[15]=0x48f0u;return 0;
        case 0x48f0u: /* ldrb r0,[r10,#0x0] */
            LOAD(r[0],r[10]+0u,1);r[15]=0x48f4u;return 0;
        case 0x48f4u: /* cmp r0,#0x0 */
            (void)subtract(state,r[0],0u);r[15]=0x48f8u;return 0;
        case 0x48f8u: /* stmdb sp!,{r10,r11,lr} */
            STORE(r[13]-12u,r[10],4);STORE(r[13]-8u,r[11],4);STORE(r[13]-4u,r[14],4);r[13]-=12u;r[15]=0x48fcu;return 0;
        case 0x48fcu: /* blne 0x00004938 */
            if(!(state->cpsr&0x40000000u)){r[14]=0x4900u;r[15]=18744u;}else{r[15]=0x4900u;}return 0;
        case 0x4900u: /* ldmia sp!,{r10,r11,lr} */
            LOAD(r[10],r[13]+0u,4);LOAD(r[11],r[13]+4u,4);LOAD(r[14],r[13]+8u,4);r[13]+=12u;r[15]=0x4904u;return 0;
        case 0x4904u: /* ldr r1,[0x4934] */
            LOAD(r[1],18740u,4);r[15]=0x4908u;return 0;
        case 0x4908u: /* strb r0,[r1,#0x1] */
            STORE(r[1]+1u,r[0],1);r[15]=0x490cu;return 0;
        case 0x490cu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x4910u;return 0;
        case 0x4910u: /* strb r0,[r10,#0x0] */
            STORE(r[10]+0u,r[0],1);r[15]=0x4914u;return 0;
        case 0x4914u: /* add r10,r10,#0x10 */
            r[10]=r[10]+16u;r[15]=0x4918u;return 0;
        case 0x4918u: /* subs r11,r11,#0x1 */
            r[11]=subtract(state,r[11],1u);r[15]=0x491cu;return 0;
        case 0x491cu: /* bne 0x000048f0 */
            if(!(state->cpsr&0x40000000u)){r[15]=18672u;}else{r[15]=0x4920u;}return 0;
        case 0x4920u: /* mov r0,#0x13400 */
            r[0]=78848u;r[15]=0x4924u;return 0;
        case 0x4924u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x4928u;return 0;
        case 0x4928u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x492cu;return 0;
        case 0x492cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4938u: /* mov r3,#0x14800 */
            r[3]=83968u;r[15]=0x493cu;return 0;
        case 0x493cu: /* ldr r4,[0x4b30] */
            LOAD(r[4],19248u,4);r[15]=0x4940u;return 0;
        case 0x4940u: /* ldr r7,[r4,#0x0] */
            LOAD(r[7],r[4]+0u,4);r[15]=0x4944u;return 0;
        case 0x4944u: /* add r3,r3,r7 */
            r[3]=r[3]+r[7];r[15]=0x4948u;return 0;
        case 0x4948u: /* str r3,[0x4934] */
            STORE(18740u,r[3],4);r[15]=0x494cu;return 0;
        case 0x494cu: /* ldr r5,[r10,#0x0] */
            LOAD(r[5],r[10]+0u,4);r[15]=0x4950u;return 0;
        case 0x4950u: /* str r5,[r3,#0x0] */
            STORE(r[3]+0u,r[5],4);r[15]=0x4954u;return 0;
        case 0x4954u: /* ldr r5,[r10,#0x4] */
            LOAD(r[5],r[10]+4u,4);r[15]=0x4958u;return 0;
        case 0x4958u: /* str r5,[r3,#0x4] */
            STORE(r[3]+4u,r[5],4);r[15]=0x495cu;return 0;
        case 0x495cu: /* ldr r5,[r10,#0x8] */
            LOAD(r[5],r[10]+8u,4);r[15]=0x4960u;return 0;
        case 0x4960u: /* str r5,[r3,#0x8] */
            STORE(r[3]+8u,r[5],4);r[15]=0x4964u;return 0;
        case 0x4964u: /* ldr r6,[0x4b34] */
            LOAD(r[6],19252u,4);r[15]=0x4968u;return 0;
        case 0x4968u: /* ldr r5,[r6,#0x0] */
            LOAD(r[5],r[6]+0u,4);r[15]=0x496cu;return 0;
        case 0x496cu: /* str r5,[r3,#0xc] */
            STORE(r[3]+12u,r[5],4);r[15]=0x4970u;return 0;
        case 0x4970u: /* mov r5,#0x0 */
            r[5]=0u;r[15]=0x4974u;return 0;
        case 0x4974u: /* str r5,[r6,#0x0] */
            STORE(r[6]+0u,r[5],4);r[15]=0x4978u;return 0;
        case 0x4978u: /* add r7,r7,#0x10 */
            r[7]=r[7]+16u;r[15]=0x497cu;return 0;
        case 0x497cu: /* ldr r5,[0x4b24] */
            LOAD(r[5],19236u,4);r[15]=0x4980u;return 0;
        case 0x4980u: /* and r7,r7,r5 */
            r[7]=r[7]&r[5];r[15]=0x4984u;return 0;
        case 0x4984u: /* str r7,[r4,#0x0] */
            STORE(r[4]+0u,r[7],4);r[15]=0x4988u;return 0;
        case 0x4988u: /* mov r3,#0x14800 */
            r[3]=83968u;r[15]=0x498cu;return 0;
        case 0x498cu: /* add r3,r3,r7 */
            r[3]=r[3]+r[7];r[15]=0x4990u;return 0;
        case 0x4990u: /* mvn r5,#0x0 */
            r[5]=~0u;r[15]=0x4994u;return 0;
        case 0x4994u: /* str r5,[r3,#0x0] */
            STORE(r[3]+0u,r[5],4);r[15]=0x4998u;return 0;
        case 0x4998u: /* str r5,[r3,#0x4] */
            STORE(r[3]+4u,r[5],4);r[15]=0x499cu;return 0;
        case 0x499cu: /* str r5,[r3,#0x8] */
            STORE(r[3]+8u,r[5],4);r[15]=0x49a0u;return 0;
        case 0x49a0u: /* str r5,[r3,#0xc] */
            STORE(r[3]+12u,r[5],4);r[15]=0x49a4u;return 0;
        case 0x49a4u: /* ands r0,r0,#0xff */
            temporary=r[0]&255u;r[0]=temporary;logic_flags(state,temporary,0,255u);r[15]=0x49a8u;return 0;
        case 0x49a8u: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x49acu;return 0;
        case 0x49acu: /* cmp r0,#0x200 */
            (void)subtract(state,r[0],512u);r[15]=0x49b0u;return 0;
        case 0x49b0u: /* bcs 0x000049c8 */
            if(state->cpsr&0x20000000u){r[15]=18888u;}else{r[15]=0x49b4u;}return 0;
        case 0x49b4u: /* adr r3,0x49e4 */
            r[3]=18916u;r[15]=0x49b8u;return 0;
        case 0x49b8u: /* cmp r0,#0x100 */
            (void)subtract(state,r[0],256u);r[15]=0x49bcu;return 0;
        case 0x49bcu: /* movcs pc,lr */
            if(state->cpsr&0x20000000u){r[15]=r[14];}else{r[15]=0x49c0u;}return 0;
        case 0x49c0u: /* add r3,r3,r0 */
            r[3]=r[3]+r[0];r[15]=0x49c4u;return 0;
        case 0x49c4u: /* mov pc,r3 */
            r[15]=r[3];return 0;
        case 0x49c8u: /* ldr r3,[0x4b28] */
            LOAD(r[3],19240u,4);r[15]=0x49ccu;return 0;
        case 0x49ccu: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0x49d0u;return 0;
        case 0x49d0u: /* adr r3,0x4ae4 */
            r[3]=19172u;r[15]=0x49d4u;return 0;
        case 0x49d4u: /* cmp r0,#0x40 */
            (void)subtract(state,r[0],64u);r[15]=0x49d8u;return 0;
        case 0x49d8u: /* movcs pc,lr */
            if(state->cpsr&0x20000000u){r[15]=r[14];}else{r[15]=0x49dcu;}return 0;
        case 0x49dcu: /* add r3,r3,r0 */
            r[3]=r[3]+r[0];r[15]=0x49e0u;return 0;
        case 0x49e0u: /* mov pc,r3 */
            r[15]=r[3];return 0;
        case 0x49e4u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x49e8u: /* b 0x00004b3c */
            r[15]=19260u;return 0;
        case 0x49ecu: /* b 0x00004c20 */
            r[15]=19488u;return 0;
        case 0x49f0u: /* b 0x00004cb8 */
            r[15]=19640u;return 0;
        case 0x49f4u: /* b 0x00004d44 */
            r[15]=19780u;return 0;
        case 0x49f8u: /* b 0x00004d90 */
            r[15]=19856u;return 0;
        case 0x49fcu: /* b 0x00004e18 */
            r[15]=19992u;return 0;
        case 0x4a00u: /* b 0x00004ea0 */
            r[15]=20128u;return 0;
        case 0x4a04u: /* b 0x00004f40 */
            r[15]=20288u;return 0;
        case 0x4a08u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4a0cu: /* b 0x00004fd8 */
            r[15]=20440u;return 0;
        case 0x4a10u: /* b 0x00004ffc */
            r[15]=20476u;return 0;
        case 0x4a14u: /* b 0x00005020 */
            r[15]=20512u;return 0;
        case 0x4a18u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4a1cu: /* b 0x00005034 */
            r[15]=20532u;return 0;
        case 0x4a20u: /* b 0x000050e0 */
            r[15]=20704u;return 0;
        case 0x4a24u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4a28u: /* b 0x0000517c */
            r[15]=20860u;return 0;
        case 0x4a2cu: /* b 0x0000526c */
            r[15]=21100u;return 0;
        case 0x4a30u: /* b 0x00005280 */
            r[15]=21120u;return 0;
        case 0x4a34u: /* b 0x00005294 */
            r[15]=21140u;return 0;
        case 0x4a38u: /* b 0x000052a8 */
            r[15]=21160u;return 0;
        case 0x4a3cu: /* b 0x000052d0 */
            r[15]=21200u;return 0;
        case 0x4a40u: /* b 0x000052f8 */
            r[15]=21240u;return 0;
        case 0x4a44u: /* b 0x00005398 */
            r[15]=21400u;return 0;
        case 0x4a48u: /* b 0x00005438 */
            r[15]=21560u;return 0;
        case 0x4a4cu: /* b 0x00005454 */
            r[15]=21588u;return 0;
        case 0x4a50u: /* b 0x00005470 */
            r[15]=21616u;return 0;
        case 0x4a54u: /* b 0x0000548c */
            r[15]=21644u;return 0;
        case 0x4a58u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4a5cu: /* b 0x000054a8 */
            r[15]=21672u;return 0;
        case 0x4a60u: /* b 0x000054d4 */
            r[15]=21716u;return 0;
        case 0x4a64u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4a68u: /* b 0x000054e8 */
            r[15]=21736u;return 0;
        case 0x4a6cu: /* b 0x000055c0 */
            r[15]=21952u;return 0;
        case 0x4a70u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4a74u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4a78u: /* b 0x00005604 */
            r[15]=22020u;return 0;
        case 0x4a7cu: /* b 0x00005638 */
            r[15]=22072u;return 0;
        case 0x4a80u: /* b 0x0000566c */
            r[15]=22124u;return 0;
        case 0x4a84u: /* b 0x0000571c */
            r[15]=22300u;return 0;
        case 0x4a88u: /* b 0x000057cc */
            r[15]=22476u;return 0;
        case 0x4a8cu: /* b 0x000057e8 */
            r[15]=22504u;return 0;
        case 0x4a90u: /* b 0x00005800 */
            r[15]=22528u;return 0;
        case 0x4a94u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4a98u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4a9cu: /* b 0x00005818 */
            r[15]=22552u;return 0;
        case 0x4aa0u: /* b 0x00005844 */
            r[15]=22596u;return 0;
        case 0x4aa4u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4aa8u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4aacu: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4ab0u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4ab4u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4ab8u: /* b 0x0000585c */
            r[15]=22620u;return 0;
        case 0x4abcu: /* b 0x00005884 */
            r[15]=22660u;return 0;
        case 0x4ac0u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4ac4u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4ac8u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4accu: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4ad0u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4ad4u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4ad8u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4adcu: /* b 0x000058cc */
            r[15]=22732u;return 0;
        case 0x4ae0u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4ae4u: /* b 0x000058f0 */
            r[15]=22768u;return 0;
        case 0x4ae8u: /* b 0x000059d8 */
            r[15]=23000u;return 0;
        case 0x4aecu: /* b 0x00005a14 */
            r[15]=23060u;return 0;
        case 0x4af0u: /* b 0x00005b60 */
            r[15]=23392u;return 0;
        case 0x4af4u: /* b 0x00005c1c */
            r[15]=23580u;return 0;
        case 0x4af8u: /* b 0x00005d54 */
            r[15]=23892u;return 0;
        case 0x4afcu: /* b 0x00005df8 */
            r[15]=24056u;return 0;
        case 0x4b00u: /* b 0x00005e28 */
            r[15]=24104u;return 0;
        case 0x4b04u: /* b 0x00005edc */
            r[15]=24284u;return 0;
        case 0x4b08u: /* b 0x00005fc8 */
            r[15]=24520u;return 0;
        case 0x4b0cu: /* b 0x00005fcc */
            r[15]=24524u;return 0;
        case 0x4b10u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4b14u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4b18u: /* b 0x00004b38 */
            r[15]=19256u;return 0;
        case 0x4b1cu: /* b 0x0000601c */
            r[15]=24604u;return 0;
        case 0x4b20u: /* b 0x00006060 */
            r[15]=24672u;return 0;
        case 0x4b38u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4b3cu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4b40u;return 0;
        case 0x4b40u: /* ldrb r3,[r10,#0x3] */
            LOAD(r[3],r[10]+3u,1);r[15]=0x4b44u;return 0;
        case 0x4b44u: /* ldrb r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,1);r[15]=0x4b48u;return 0;
        case 0x4b48u: /* ldrb r2,[r10,#0x5] */
            LOAD(r[2],r[10]+5u,1);r[15]=0x4b4cu;return 0;
        case 0x4b4cu: /* mov r3,r3, lsl #0x3 */
            r[3]=(r[3]<<3);r[15]=0x4b50u;return 0;
        case 0x4b50u: /* mov r4,#0x14000 */
            r[4]=81920u;r[15]=0x4b54u;return 0;
        case 0x4b54u: /* add r4,r4,r3 */
            r[4]=r[4]+r[3];r[15]=0x4b58u;return 0;
        case 0x4b58u: /* ldr r12,[r4,#0x0] */
            LOAD(r[12],r[4]+0u,4);r[15]=0x4b5cu;return 0;
        case 0x4b5cu: /* ldr r5,[r12,#0x0] */
            LOAD(r[5],r[12]+0u,4);r[15]=0x4b60u;return 0;
        case 0x4b60u: /* ldr r4,[0x4c18] */
            LOAD(r[4],19480u,4);r[15]=0x4b64u;return 0;
        case 0x4b64u: /* cmp r5,r4 */
            (void)subtract(state,r[5],r[4]);r[15]=0x4b68u;return 0;
        case 0x4b68u: /* movne r1,#0x2 */
            if(!(state->cpsr&0x40000000u)){r[1]=2u;}r[15]=0x4b6cu;return 0;
        case 0x4b6cu: /* bne 0x00004bec */
            if(!(state->cpsr&0x40000000u)){r[15]=19436u;}else{r[15]=0x4b70u;}return 0;
        case 0x4b70u: /* ldrb r5,[r12,#0x4] */
            LOAD(r[5],r[12]+4u,1);r[15]=0x4b74u;return 0;
        case 0x4b74u: /* cmp r5,#0x1 */
            (void)subtract(state,r[5],1u);r[15]=0x4b78u;return 0;
        case 0x4b78u: /* movne r1,#0x8 */
            if(!(state->cpsr&0x40000000u)){r[1]=8u;}r[15]=0x4b7cu;return 0;
        case 0x4b7cu: /* bne 0x00004bec */
            if(!(state->cpsr&0x40000000u)){r[15]=19436u;}else{r[15]=0x4b80u;}return 0;
        case 0x4b80u: /* ldr r6,[r12,#0x8] */
            LOAD(r[6],r[12]+8u,4);r[15]=0x4b84u;return 0;
        case 0x4b84u: /* sub r6,r6,#0x4 */
            r[6]=r[6]-4u;r[15]=0x4b88u;return 0;
        case 0x4b88u: /* ldr r5,[r12,r6] */
            LOAD(r[5],r[12]+r[6],4);r[15]=0x4b8cu;return 0;
        case 0x4b8cu: /* ldr r6,[0x4c1c] */
            LOAD(r[6],19484u,4);r[15]=0x4b90u;return 0;
        case 0x4b90u: /* cmp r5,r6 */
            (void)subtract(state,r[5],r[6]);r[15]=0x4b94u;return 0;
        case 0x4b94u: /* movne r1,#0x4 */
            if(!(state->cpsr&0x40000000u)){r[1]=4u;}r[15]=0x4b98u;return 0;
        case 0x4b98u: /* bne 0x00004bec */
            if(!(state->cpsr&0x40000000u)){r[15]=19436u;}else{r[15]=0x4b9cu;}return 0;
        case 0x4b9cu: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x4ba0u;return 0;
        case 0x4ba0u: /* bl 0x00007404 */
            r[14]=0x4ba4u;r[15]=29700u;return 0;
        case 0x4ba4u: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x4ba8u;return 0;
        case 0x4ba8u: /* mov r2,#0xc000 */
            r[2]=49152u;r[15]=0x4bacu;return 0;
        case 0x4bacu: /* mov r0,r0, lsl #0x7 */
            r[0]=(r[0]<<7);r[15]=0x4bb0u;return 0;
        case 0x4bb0u: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x4bb4u;return 0;
        case 0x4bb4u: /* ldrb r3,[r2,#0x28] */
            LOAD(r[3],r[2]+40u,1);r[15]=0x4bb8u;return 0;
        case 0x4bb8u: /* and r3,r3,#0xfc */
            r[3]=r[3]&252u;r[15]=0x4bbcu;return 0;
        case 0x4bbcu: /* and r1,r1,#0x3 */
            r[1]=r[1]&3u;r[15]=0x4bc0u;return 0;
        case 0x4bc0u: /* orr r3,r3,r1 */
            r[3]=r[3]|r[1];r[15]=0x4bc4u;return 0;
        case 0x4bc4u: /* ldr r4,[0x5178] */
            LOAD(r[4],20856u,4);r[15]=0x4bc8u;return 0;
        case 0x4bc8u: /* ldrb r1,[r4,#0x0] */
            LOAD(r[1],r[4]+0u,1);r[15]=0x4bccu;return 0;
        case 0x4bccu: /* strb r1,[r2,#0x29] */
            STORE(r[2]+41u,r[1],1);r[15]=0x4bd0u;return 0;
        case 0x4bd0u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x4bd4u;return 0;
        case 0x4bd4u: /* orrne r3,r3,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[3]|128u;}r[15]=0x4bd8u;return 0;
        case 0x4bd8u: /* biceq r3,r3,#0x80 */
            if(state->cpsr&0x40000000u){r[3]=r[3]&~128u;}r[15]=0x4bdcu;return 0;
        case 0x4bdcu: /* strb r3,[r2,#0x28] */
            STORE(r[2]+40u,r[3],1);r[15]=0x4be0u;return 0;
        case 0x4be0u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x4be4u;return 0;
        case 0x4be4u: /* strb r1,[r4,#0x0] */
            STORE(r[4]+0u,r[1],1);r[15]=0x4be8u;return 0;
        case 0x4be8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4becu: /* mov r2,#0xc000 */
            r[2]=49152u;r[15]=0x4bf0u;return 0;
        case 0x4bf0u: /* mov r0,r0, lsl #0x7 */
            r[0]=(r[0]<<7);r[15]=0x4bf4u;return 0;
        case 0x4bf4u: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x4bf8u;return 0;
        case 0x4bf8u: /* ldrb r0,[r2,#0x28] */
            LOAD(r[0],r[2]+40u,1);r[15]=0x4bfcu;return 0;
        case 0x4bfcu: /* orr r0,r0,#0x80 */
            r[0]=r[0]|128u;r[15]=0x4c00u;return 0;
        case 0x4c00u: /* strb r0,[r2,#0x28] */
            STORE(r[2]+40u,r[0],1);r[15]=0x4c04u;return 0;
        case 0x4c04u: /* ldrb r0,[r2,#0x2a] */
            LOAD(r[0],r[2]+42u,1);r[15]=0x4c08u;return 0;
        case 0x4c08u: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0x4c0cu;return 0;
        case 0x4c0cu: /* strb r0,[r2,#0x2a] */
            STORE(r[2]+42u,r[0],1);r[15]=0x4c10u;return 0;
        case 0x4c10u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4c20u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4c24u;return 0;
        case 0x4c24u: /* mov r1,#0xe800 */
            r[1]=59392u;r[15]=0x4c28u;return 0;
        case 0x4c28u: /* mov r2,#0x30 */
            r[2]=48u;r[15]=0x4c2cu;return 0;
        case 0x4c2cu: /* mov r3,#0x800000 */
            r[3]=8388608u;r[15]=0x4c30u;return 0;
        case 0x4c30u: /* ldrb r4,[r1,#0x4] */
            LOAD(r[4],r[1]+4u,1);r[15]=0x4c34u;return 0;
        case 0x4c34u: /* cmp r4,r0 */
            (void)subtract(state,r[4],r[0]);r[15]=0x4c38u;return 0;
        case 0x4c38u: /* bne 0x00004c58 */
            if(!(state->cpsr&0x40000000u)){r[15]=19544u;}else{r[15]=0x4c3cu;}return 0;
        case 0x4c3cu: /* mov r4,#0x0 */
            r[4]=0u;r[15]=0x4c40u;return 0;
        case 0x4c40u: /* str r4,[r1,#0x0] */
            STORE(r[1]+0u,r[4],4);r[15]=0x4c44u;return 0;
        case 0x4c44u: /* ldr r5,[r3,#0x0] */
            LOAD(r[5],r[3]+0u,4);r[15]=0x4c48u;return 0;
        case 0x4c48u: /* ldr r4,[0x4cb4] */
            LOAD(r[4],19636u,4);r[15]=0x4c4cu;return 0;
        case 0x4c4cu: /* and r5,r5,r4 */
            r[5]=r[5]&r[4];r[15]=0x4c50u;return 0;
        case 0x4c50u: /* orr r5,r5,#0x8000 */
            r[5]=r[5]|32768u;r[15]=0x4c54u;return 0;
        case 0x4c54u: /* str r5,[r3,#0x0] */
            STORE(r[3]+0u,r[5],4);r[15]=0x4c58u;return 0;
        case 0x4c58u: /* add r1,r1,#0x40 */
            r[1]=r[1]+64u;r[15]=0x4c5cu;return 0;
        case 0x4c5cu: /* add r3,r3,#0x80 */
            r[3]=r[3]+128u;r[15]=0x4c60u;return 0;
        case 0x4c60u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x4c64u;return 0;
        case 0x4c64u: /* bne 0x00004c30 */
            if(!(state->cpsr&0x40000000u)){r[15]=19504u;}else{r[15]=0x4c68u;}return 0;
        case 0x4c68u: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x4c6cu;return 0;
        case 0x4c6cu: /* bl 0x000074ec */
            r[14]=0x4c70u;r[15]=29932u;return 0;
        case 0x4c70u: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x4c74u;return 0;
        case 0x4c74u: /* mov r2,#0xc000 */
            r[2]=49152u;r[15]=0x4c78u;return 0;
        case 0x4c78u: /* mov r0,r0, lsl #0x7 */
            r[0]=(r[0]<<7);r[15]=0x4c7cu;return 0;
        case 0x4c7cu: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x4c80u;return 0;
        case 0x4c80u: /* ldrb r3,[r2,#0x28] */
            LOAD(r[3],r[2]+40u,1);r[15]=0x4c84u;return 0;
        case 0x4c84u: /* and r3,r3,#0xfc */
            r[3]=r[3]&252u;r[15]=0x4c88u;return 0;
        case 0x4c88u: /* and r1,r1,#0x3 */
            r[1]=r[1]&3u;r[15]=0x4c8cu;return 0;
        case 0x4c8cu: /* orr r3,r3,r1 */
            r[3]=r[3]|r[1];r[15]=0x4c90u;return 0;
        case 0x4c90u: /* ldr r4,[0x5178] */
            LOAD(r[4],20856u,4);r[15]=0x4c94u;return 0;
        case 0x4c94u: /* ldrb r1,[r4,#0x0] */
            LOAD(r[1],r[4]+0u,1);r[15]=0x4c98u;return 0;
        case 0x4c98u: /* strb r1,[r2,#0x29] */
            STORE(r[2]+41u,r[1],1);r[15]=0x4c9cu;return 0;
        case 0x4c9cu: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x4ca0u;return 0;
        case 0x4ca0u: /* orrne r3,r3,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[3]|128u;}r[15]=0x4ca4u;return 0;
        case 0x4ca4u: /* biceq r3,r3,#0x80 */
            if(state->cpsr&0x40000000u){r[3]=r[3]&~128u;}r[15]=0x4ca8u;return 0;
        case 0x4ca8u: /* strb r3,[r2,#0x28] */
            STORE(r[2]+40u,r[3],1);r[15]=0x4cacu;return 0;
        case 0x4cacu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4cb8u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4cbcu;return 0;
        case 0x4cbcu: /* mov r1,#0xe800 */
            r[1]=59392u;r[15]=0x4cc0u;return 0;
        case 0x4cc0u: /* mov r2,#0x30 */
            r[2]=48u;r[15]=0x4cc4u;return 0;
        case 0x4cc4u: /* mov r3,#0x800000 */
            r[3]=8388608u;r[15]=0x4cc8u;return 0;
        case 0x4cc8u: /* ldrb r4,[r1,#0x4] */
            LOAD(r[4],r[1]+4u,1);r[15]=0x4cccu;return 0;
        case 0x4cccu: /* cmp r4,r0 */
            (void)subtract(state,r[4],r[0]);r[15]=0x4cd0u;return 0;
        case 0x4cd0u: /* bne 0x00004ce8 */
            if(!(state->cpsr&0x40000000u)){r[15]=19688u;}else{r[15]=0x4cd4u;}return 0;
        case 0x4cd4u: /* ldr r5,[r3,#0x0] */
            LOAD(r[5],r[3]+0u,4);r[15]=0x4cd8u;return 0;
        case 0x4cd8u: /* ldr r4,[0x4d40] */
            LOAD(r[4],19776u,4);r[15]=0x4cdcu;return 0;
        case 0x4cdcu: /* and r5,r5,r4 */
            r[5]=r[5]&r[4];r[15]=0x4ce0u;return 0;
        case 0x4ce0u: /* orr r5,r5,#0x8000 */
            r[5]=r[5]|32768u;r[15]=0x4ce4u;return 0;
        case 0x4ce4u: /* str r5,[r3,#0x0] */
            STORE(r[3]+0u,r[5],4);r[15]=0x4ce8u;return 0;
        case 0x4ce8u: /* add r1,r1,#0x40 */
            r[1]=r[1]+64u;r[15]=0x4cecu;return 0;
        case 0x4cecu: /* add r3,r3,#0x80 */
            r[3]=r[3]+128u;r[15]=0x4cf0u;return 0;
        case 0x4cf0u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x4cf4u;return 0;
        case 0x4cf4u: /* bne 0x00004cc8 */
            if(!(state->cpsr&0x40000000u)){r[15]=19656u;}else{r[15]=0x4cf8u;}return 0;
        case 0x4cf8u: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x4cfcu;return 0;
        case 0x4cfcu: /* bl 0x0000757c */
            r[14]=0x4d00u;r[15]=30076u;return 0;
        case 0x4d00u: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x4d04u;return 0;
        case 0x4d04u: /* mov r2,#0xc000 */
            r[2]=49152u;r[15]=0x4d08u;return 0;
        case 0x4d08u: /* mov r0,r0, lsl #0x7 */
            r[0]=(r[0]<<7);r[15]=0x4d0cu;return 0;
        case 0x4d0cu: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x4d10u;return 0;
        case 0x4d10u: /* ldrb r3,[r2,#0x28] */
            LOAD(r[3],r[2]+40u,1);r[15]=0x4d14u;return 0;
        case 0x4d14u: /* and r3,r3,#0xfc */
            r[3]=r[3]&252u;r[15]=0x4d18u;return 0;
        case 0x4d18u: /* and r1,r1,#0x3 */
            r[1]=r[1]&3u;r[15]=0x4d1cu;return 0;
        case 0x4d1cu: /* orr r3,r3,r1 */
            r[3]=r[3]|r[1];r[15]=0x4d20u;return 0;
        case 0x4d20u: /* ldr r4,[0x5178] */
            LOAD(r[4],20856u,4);r[15]=0x4d24u;return 0;
        case 0x4d24u: /* ldrb r1,[r4,#0x0] */
            LOAD(r[1],r[4]+0u,1);r[15]=0x4d28u;return 0;
        case 0x4d28u: /* strb r1,[r2,#0x29] */
            STORE(r[2]+41u,r[1],1);r[15]=0x4d2cu;return 0;
        case 0x4d2cu: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x4d30u;return 0;
        case 0x4d30u: /* orrne r3,r3,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[3]|128u;}r[15]=0x4d34u;return 0;
        case 0x4d34u: /* biceq r3,r3,#0x80 */
            if(state->cpsr&0x40000000u){r[3]=r[3]&~128u;}r[15]=0x4d38u;return 0;
        case 0x4d38u: /* strb r3,[r2,#0x28] */
            STORE(r[2]+40u,r[3],1);r[15]=0x4d3cu;return 0;
        case 0x4d3cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4d44u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4d48u;return 0;
        case 0x4d48u: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x4d4cu;return 0;
        case 0x4d4cu: /* bl 0x000075fc */
            r[14]=0x4d50u;r[15]=30204u;return 0;
        case 0x4d50u: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x4d54u;return 0;
        case 0x4d54u: /* mov r2,#0xc000 */
            r[2]=49152u;r[15]=0x4d58u;return 0;
        case 0x4d58u: /* mov r0,r0, lsl #0x7 */
            r[0]=(r[0]<<7);r[15]=0x4d5cu;return 0;
        case 0x4d5cu: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x4d60u;return 0;
        case 0x4d60u: /* ldrb r3,[r2,#0x28] */
            LOAD(r[3],r[2]+40u,1);r[15]=0x4d64u;return 0;
        case 0x4d64u: /* and r3,r3,#0xfc */
            r[3]=r[3]&252u;r[15]=0x4d68u;return 0;
        case 0x4d68u: /* and r1,r1,#0x3 */
            r[1]=r[1]&3u;r[15]=0x4d6cu;return 0;
        case 0x4d6cu: /* orr r3,r3,r1 */
            r[3]=r[3]|r[1];r[15]=0x4d70u;return 0;
        case 0x4d70u: /* ldr r4,[0x5178] */
            LOAD(r[4],20856u,4);r[15]=0x4d74u;return 0;
        case 0x4d74u: /* ldrb r1,[r4,#0x0] */
            LOAD(r[1],r[4]+0u,1);r[15]=0x4d78u;return 0;
        case 0x4d78u: /* strb r1,[r2,#0x29] */
            STORE(r[2]+41u,r[1],1);r[15]=0x4d7cu;return 0;
        case 0x4d7cu: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x4d80u;return 0;
        case 0x4d80u: /* orrne r3,r3,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[3]|128u;}r[15]=0x4d84u;return 0;
        case 0x4d84u: /* biceq r3,r3,#0x80 */
            if(state->cpsr&0x40000000u){r[3]=r[3]&~128u;}r[15]=0x4d88u;return 0;
        case 0x4d88u: /* strb r3,[r2,#0x28] */
            STORE(r[2]+40u,r[3],1);r[15]=0x4d8cu;return 0;
        case 0x4d8cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4d90u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4d94u;return 0;
        case 0x4d94u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x4d98u;return 0;
        case 0x4d98u: /* ldr r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,4);r[15]=0x4d9cu;return 0;
        case 0x4d9cu: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x4da0u;return 0;
        case 0x4da0u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x4da4u;return 0;
        case 0x4da4u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x4da8u;return 0;
        case 0x4da8u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x4dacu;return 0;
        case 0x4dacu: /* ldr r3,[0x4e0c] */
            LOAD(r[3],19980u,4);r[15]=0x4db0u;return 0;
        case 0x4db0u: /* ands r2,r2,r3 */
            temporary=r[2]&r[3];r[2]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x4db4u;return 0;
        case 0x4db4u: /* beq 0x00004df4 */
            if(state->cpsr&0x40000000u){r[15]=19956u;}else{r[15]=0x4db8u;}return 0;
        case 0x4db8u: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x4dbcu;return 0;
        case 0x4dbcu: /* ldrb r0,[r4,#0x1] */
            LOAD(r[0],r[4]+1u,1);r[15]=0x4dc0u;return 0;
        case 0x4dc0u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x4dc4u;return 0;
        case 0x4dc4u: /* ldr r3,[0x4e10] */
            LOAD(r[3],19984u,4);r[15]=0x4dc8u;return 0;
        case 0x4dc8u: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x4dccu;return 0;
        case 0x4dccu: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x4dd0u;return 0;
        case 0x4dd0u: /* and r0,r0,#0xff */
            r[0]=r[0]&255u;r[15]=0x4dd4u;return 0;
        case 0x4dd4u: /* ldr r3,[0x4e14] */
            LOAD(r[3],19988u,4);r[15]=0x4dd8u;return 0;
        case 0x4dd8u: /* tst r0,#0xfe */
            temporary=r[0]&254u;logic_flags(state,temporary,0,254u);r[15]=0x4ddcu;return 0;
        case 0x4ddcu: /* beq 0x00004df4 */
            if(state->cpsr&0x40000000u){r[15]=19956u;}else{r[15]=0x4de0u;}return 0;
        case 0x4de0u: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4de4u;return 0;
        case 0x4de4u: /* beq 0x00004df4 */
            if(state->cpsr&0x40000000u){r[15]=19956u;}else{r[15]=0x4de8u;}return 0;
        case 0x4de8u: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x4decu;return 0;
        case 0x4decu: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x4df0u;return 0;
        case 0x4df0u: /* b 0x00004dd8 */
            r[15]=19928u;return 0;
        case 0x4df4u: /* strb r1,[r4,#0x0] */
            STORE(r[4]+0u,r[1],1);r[15]=0x4df8u;return 0;
        case 0x4df8u: /* strb r2,[r4,#0x2] */
            STORE(r[4]+2u,r[2],1);r[15]=0x4dfcu;return 0;
        case 0x4dfcu: /* strb r2,[r4,#0x3] */
            STORE(r[4]+3u,r[2],1);r[15]=0x4e00u;return 0;
        case 0x4e00u: /* strb r0,[r4,#0x4] */
            STORE(r[4]+4u,r[0],1);r[15]=0x4e04u;return 0;
        case 0x4e04u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x4e08u;return 0;
        case 0x4e08u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4e18u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4e1cu;return 0;
        case 0x4e1cu: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x4e20u;return 0;
        case 0x4e20u: /* ldr r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,4);r[15]=0x4e24u;return 0;
        case 0x4e24u: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x4e28u;return 0;
        case 0x4e28u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x4e2cu;return 0;
        case 0x4e2cu: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x4e30u;return 0;
        case 0x4e30u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x4e34u;return 0;
        case 0x4e34u: /* ldr r3,[0x4e94] */
            LOAD(r[3],20116u,4);r[15]=0x4e38u;return 0;
        case 0x4e38u: /* ands r2,r2,r3 */
            temporary=r[2]&r[3];r[2]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x4e3cu;return 0;
        case 0x4e3cu: /* beq 0x00004e7c */
            if(state->cpsr&0x40000000u){r[15]=20092u;}else{r[15]=0x4e40u;}return 0;
        case 0x4e40u: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x4e44u;return 0;
        case 0x4e44u: /* ldrb r0,[r4,#0x7] */
            LOAD(r[0],r[4]+7u,1);r[15]=0x4e48u;return 0;
        case 0x4e48u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x4e4cu;return 0;
        case 0x4e4cu: /* ldr r3,[0x4e98] */
            LOAD(r[3],20120u,4);r[15]=0x4e50u;return 0;
        case 0x4e50u: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x4e54u;return 0;
        case 0x4e54u: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x4e58u;return 0;
        case 0x4e58u: /* and r0,r0,#0xff */
            r[0]=r[0]&255u;r[15]=0x4e5cu;return 0;
        case 0x4e5cu: /* ldr r3,[0x4e9c] */
            LOAD(r[3],20124u,4);r[15]=0x4e60u;return 0;
        case 0x4e60u: /* tst r0,#0xfe */
            temporary=r[0]&254u;logic_flags(state,temporary,0,254u);r[15]=0x4e64u;return 0;
        case 0x4e64u: /* beq 0x00004e7c */
            if(state->cpsr&0x40000000u){r[15]=20092u;}else{r[15]=0x4e68u;}return 0;
        case 0x4e68u: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4e6cu;return 0;
        case 0x4e6cu: /* beq 0x00004e7c */
            if(state->cpsr&0x40000000u){r[15]=20092u;}else{r[15]=0x4e70u;}return 0;
        case 0x4e70u: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x4e74u;return 0;
        case 0x4e74u: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x4e78u;return 0;
        case 0x4e78u: /* b 0x00004e60 */
            r[15]=20064u;return 0;
        case 0x4e7cu: /* strb r1,[r4,#0x6] */
            STORE(r[4]+6u,r[1],1);r[15]=0x4e80u;return 0;
        case 0x4e80u: /* strb r2,[r4,#0x8] */
            STORE(r[4]+8u,r[2],1);r[15]=0x4e84u;return 0;
        case 0x4e84u: /* strb r2,[r4,#0x9] */
            STORE(r[4]+9u,r[2],1);r[15]=0x4e88u;return 0;
        case 0x4e88u: /* strb r0,[r4,#0xa] */
            STORE(r[4]+10u,r[0],1);r[15]=0x4e8cu;return 0;
        case 0x4e8cu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x4e90u;return 0;
        case 0x4e90u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4ea0u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4ea4u;return 0;
        case 0x4ea4u: /* ldr r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,4);r[15]=0x4ea8u;return 0;
        case 0x4ea8u: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x4eacu;return 0;
        case 0x4eacu: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x4eb0u;return 0;
        case 0x4eb0u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x4eb4u;return 0;
        case 0x4eb4u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x4eb8u;return 0;
        case 0x4eb8u: /* mov r2,r1, lsr #0x10 */
            r[2]=(r[1]>>16);r[15]=0x4ebcu;return 0;
        case 0x4ebcu: /* ldr r3,[0x4f30] */
            LOAD(r[3],20272u,4);r[15]=0x4ec0u;return 0;
        case 0x4ec0u: /* and r1,r1,r3 */
            r[1]=r[1]&r[3];r[15]=0x4ec4u;return 0;
        case 0x4ec4u: /* ands r2,r2,r3 */
            temporary=r[2]&r[3];r[2]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x4ec8u;return 0;
        case 0x4ec8u: /* beq 0x00004f0c */
            if(state->cpsr&0x40000000u){r[15]=20236u;}else{r[15]=0x4eccu;}return 0;
        case 0x4eccu: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x4ed0u;return 0;
        case 0x4ed0u: /* ldr r0,[r4,#0x1c] */
            LOAD(r[0],r[4]+28u,4);r[15]=0x4ed4u;return 0;
        case 0x4ed4u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x4ed8u;return 0;
        case 0x4ed8u: /* ldr r3,[0x4f34] */
            LOAD(r[3],20276u,4);r[15]=0x4edcu;return 0;
        case 0x4edcu: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x4ee0u;return 0;
        case 0x4ee0u: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x4ee4u;return 0;
        case 0x4ee4u: /* ldr r3,[0x4f30] */
            LOAD(r[3],20272u,4);r[15]=0x4ee8u;return 0;
        case 0x4ee8u: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0x4eecu;return 0;
        case 0x4eecu: /* ldr r3,[0x4f38] */
            LOAD(r[3],20280u,4);r[15]=0x4ef0u;return 0;
        case 0x4ef0u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4ef4u;return 0;
        case 0x4ef4u: /* beq 0x00004f0c */
            if(state->cpsr&0x40000000u){r[15]=20236u;}else{r[15]=0x4ef8u;}return 0;
        case 0x4ef8u: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4efcu;return 0;
        case 0x4efcu: /* beq 0x00004f0c */
            if(state->cpsr&0x40000000u){r[15]=20236u;}else{r[15]=0x4f00u;}return 0;
        case 0x4f00u: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x4f04u;return 0;
        case 0x4f04u: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x4f08u;return 0;
        case 0x4f08u: /* b 0x00004ef0 */
            r[15]=20208u;return 0;
        case 0x4f0cu: /* ldr r3,[0x4f3c] */
            LOAD(r[3],20284u,4);r[15]=0x4f10u;return 0;
        case 0x4f10u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4f14u;return 0;
        case 0x4f14u: /* movne r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=255u;}r[15]=0x4f18u;return 0;
        case 0x4f18u: /* str r1,[r4,#0x18] */
            STORE(r[4]+24u,r[1],4);r[15]=0x4f1cu;return 0;
        case 0x4f1cu: /* strb r2,[r4,#0x20] */
            STORE(r[4]+32u,r[2],1);r[15]=0x4f20u;return 0;
        case 0x4f20u: /* strb r2,[r4,#0x21] */
            STORE(r[4]+33u,r[2],1);r[15]=0x4f24u;return 0;
        case 0x4f24u: /* strb r0,[r4,#0x22] */
            STORE(r[4]+34u,r[0],1);r[15]=0x4f28u;return 0;
        case 0x4f28u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x4f2cu;return 0;
        case 0x4f2cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4f40u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4f44u;return 0;
        case 0x4f44u: /* ldr r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,4);r[15]=0x4f48u;return 0;
        case 0x4f48u: /* mov r2,r1, lsr #0x10 */
            r[2]=(r[1]>>16);r[15]=0x4f4cu;return 0;
        case 0x4f4cu: /* ldr r3,[0x4fc8] */
            LOAD(r[3],20424u,4);r[15]=0x4f50u;return 0;
        case 0x4f50u: /* and r1,r1,r3 */
            r[1]=r[1]&r[3];r[15]=0x4f54u;return 0;
        case 0x4f54u: /* and r2,r2,r3 */
            r[2]=r[2]&r[3];r[15]=0x4f58u;return 0;
        case 0x4f58u: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x4f5cu;return 0;
        case 0x4f5cu: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x4f60u;return 0;
        case 0x4f60u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x4f64u;return 0;
        case 0x4f64u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x4f68u;return 0;
        case 0x4f68u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x4f6cu;return 0;
        case 0x4f6cu: /* ldr r0,[r4,#0x10] */
            LOAD(r[0],r[4]+16u,4);r[15]=0x4f70u;return 0;
        case 0x4f70u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x4f74u;return 0;
        case 0x4f74u: /* ldr r3,[0x4fcc] */
            LOAD(r[3],20428u,4);r[15]=0x4f78u;return 0;
        case 0x4f78u: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x4f7cu;return 0;
        case 0x4f7cu: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x4f80u;return 0;
        case 0x4f80u: /* ldr r3,[0x4fc8] */
            LOAD(r[3],20424u,4);r[15]=0x4f84u;return 0;
        case 0x4f84u: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0x4f88u;return 0;
        case 0x4f88u: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x4f8cu;return 0;
        case 0x4f8cu: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x4f90u;return 0;
        case 0x4f90u: /* ldr r3,[0x4fd0] */
            LOAD(r[3],20432u,4);r[15]=0x4f94u;return 0;
        case 0x4f94u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4f98u;return 0;
        case 0x4f98u: /* beq 0x00004fa4 */
            if(state->cpsr&0x40000000u){r[15]=20388u;}else{r[15]=0x4f9cu;}return 0;
        case 0x4f9cu: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4fa0u;return 0;
        case 0x4fa0u: /* bne 0x00004f88 */
            if(!(state->cpsr&0x40000000u)){r[15]=20360u;}else{r[15]=0x4fa4u;}return 0;
        case 0x4fa4u: /* ldr r3,[0x4fd4] */
            LOAD(r[3],20436u,4);r[15]=0x4fa8u;return 0;
        case 0x4fa8u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4facu;return 0;
        case 0x4facu: /* movne r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=255u;}r[15]=0x4fb0u;return 0;
        case 0x4fb0u: /* str r1,[r4,#0xc] */
            STORE(r[4]+12u,r[1],4);r[15]=0x4fb4u;return 0;
        case 0x4fb4u: /* strb r2,[r4,#0x14] */
            STORE(r[4]+20u,r[2],1);r[15]=0x4fb8u;return 0;
        case 0x4fb8u: /* strb r2,[r4,#0x15] */
            STORE(r[4]+21u,r[2],1);r[15]=0x4fbcu;return 0;
        case 0x4fbcu: /* strb r0,[r4,#0x16] */
            STORE(r[4]+22u,r[0],1);r[15]=0x4fc0u;return 0;
        case 0x4fc0u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x4fc4u;return 0;
        case 0x4fc4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4fd8u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4fdcu;return 0;
        case 0x4fdcu: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x4fe0u;return 0;
        case 0x4fe0u: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x4fe4u;return 0;
        case 0x4fe4u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x4fe8u;return 0;
        case 0x4fe8u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x4fecu;return 0;
        case 0x4fecu: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x4ff0u;return 0;
        case 0x4ff0u: /* strb r1,[r4,#0x26] */
            STORE(r[4]+38u,r[1],1);r[15]=0x4ff4u;return 0;
        case 0x4ff4u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x4ff8u;return 0;
        case 0x4ff8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4ffcu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5000u;return 0;
        case 0x5000u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x5004u;return 0;
        case 0x5004u: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x5008u;return 0;
        case 0x5008u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x500cu;return 0;
        case 0x500cu: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x5010u;return 0;
        case 0x5010u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x5014u;return 0;
        case 0x5014u: /* strb r1,[r4,#0x24] */
            STORE(r[4]+36u,r[1],1);r[15]=0x5018u;return 0;
        case 0x5018u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x501cu;return 0;
        case 0x501cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5020u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5024u;return 0;
        case 0x5024u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5028u;return 0;
        case 0x5028u: /* bl 0x000076cc */
            r[14]=0x502cu;r[15]=30412u;return 0;
        case 0x502cu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5030u;return 0;
        case 0x5030u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5034u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5038u;return 0;
        case 0x5038u: /* cmp r0,#0xff */
            (void)subtract(state,r[0],255u);r[15]=0x503cu;return 0;
        case 0x503cu: /* beq 0x000050bc */
            if(state->cpsr&0x40000000u){r[15]=20668u;}else{r[15]=0x5040u;}return 0;
        case 0x5040u: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x5044u;return 0;
        case 0x5044u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x5048u;return 0;
        case 0x5048u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x504cu;return 0;
        case 0x504cu: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x5050u;return 0;
        case 0x5050u: /* mov r1,#0x80 */
            r[1]=128u;r[15]=0x5054u;return 0;
        case 0x5054u: /* mov r2,r1, lsl #0x8 */
            r[2]=(r[1]<<8);r[15]=0x5058u;return 0;
        case 0x5058u: /* mov r3,#0x0 */
            r[3]=0u;r[15]=0x505cu;return 0;
        case 0x505cu: /* strb r1,[r4,#0x0] */
            STORE(r[4]+0u,r[1],1);r[15]=0x5060u;return 0;
        case 0x5060u: /* strb r1,[r4,#0x1] */
            STORE(r[4]+1u,r[1],1);r[15]=0x5064u;return 0;
        case 0x5064u: /* strb r3,[r4,#0x2] */
            STORE(r[4]+2u,r[3],1);r[15]=0x5068u;return 0;
        case 0x5068u: /* strb r3,[r4,#0x3] */
            STORE(r[4]+3u,r[3],1);r[15]=0x506cu;return 0;
        case 0x506cu: /* strb r3,[r4,#0x4] */
            STORE(r[4]+4u,r[3],1);r[15]=0x5070u;return 0;
        case 0x5070u: /* strb r1,[r4,#0x6] */
            STORE(r[4]+6u,r[1],1);r[15]=0x5074u;return 0;
        case 0x5074u: /* strb r1,[r4,#0x7] */
            STORE(r[4]+7u,r[1],1);r[15]=0x5078u;return 0;
        case 0x5078u: /* strb r3,[r4,#0x8] */
            STORE(r[4]+8u,r[3],1);r[15]=0x507cu;return 0;
        case 0x507cu: /* strb r3,[r4,#0x9] */
            STORE(r[4]+9u,r[3],1);r[15]=0x5080u;return 0;
        case 0x5080u: /* strb r3,[r4,#0xa] */
            STORE(r[4]+10u,r[3],1);r[15]=0x5084u;return 0;
        case 0x5084u: /* str r2,[r4,#0xc] */
            STORE(r[4]+12u,r[2],4);r[15]=0x5088u;return 0;
        case 0x5088u: /* str r2,[r4,#0x10] */
            STORE(r[4]+16u,r[2],4);r[15]=0x508cu;return 0;
        case 0x508cu: /* strb r3,[r4,#0x14] */
            STORE(r[4]+20u,r[3],1);r[15]=0x5090u;return 0;
        case 0x5090u: /* strb r3,[r4,#0x15] */
            STORE(r[4]+21u,r[3],1);r[15]=0x5094u;return 0;
        case 0x5094u: /* strb r3,[r4,#0x16] */
            STORE(r[4]+22u,r[3],1);r[15]=0x5098u;return 0;
        case 0x5098u: /* str r2,[r4,#0x18] */
            STORE(r[4]+24u,r[2],4);r[15]=0x509cu;return 0;
        case 0x509cu: /* str r2,[r4,#0x1c] */
            STORE(r[4]+28u,r[2],4);r[15]=0x50a0u;return 0;
        case 0x50a0u: /* strb r3,[r4,#0x20] */
            STORE(r[4]+32u,r[3],1);r[15]=0x50a4u;return 0;
        case 0x50a4u: /* strb r3,[r4,#0x21] */
            STORE(r[4]+33u,r[3],1);r[15]=0x50a8u;return 0;
        case 0x50a8u: /* strb r3,[r4,#0x22] */
            STORE(r[4]+34u,r[3],1);r[15]=0x50acu;return 0;
        case 0x50acu: /* strb r1,[r4,#0x24] */
            STORE(r[4]+36u,r[1],1);r[15]=0x50b0u;return 0;
        case 0x50b0u: /* strb r1,[r4,#0x26] */
            STORE(r[4]+38u,r[1],1);r[15]=0x50b4u;return 0;
        case 0x50b4u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x50b8u;return 0;
        case 0x50b8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x50bcu: /* mov r4,#0xc000 */
            r[4]=49152u;r[15]=0x50c0u;return 0;
        case 0x50c0u: /* mov r5,#0x8 */
            r[5]=8u;r[15]=0x50c4u;return 0;
        case 0x50c4u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x50c8u;return 0;
        case 0x50c8u: /* bl 0x00005050 */
            r[14]=0x50ccu;r[15]=20560u;return 0;
        case 0x50ccu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x50d0u;return 0;
        case 0x50d0u: /* add r4,r4,#0x80 */
            r[4]=r[4]+128u;r[15]=0x50d4u;return 0;
        case 0x50d4u: /* subs r5,r5,#0x1 */
            r[5]=subtract(state,r[5],1u);r[15]=0x50d8u;return 0;
        case 0x50d8u: /* bne 0x000050c4 */
            if(!(state->cpsr&0x40000000u)){r[15]=20676u;}else{r[15]=0x50dcu;}return 0;
        case 0x50dcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x50e0u: /* mov r1,#0xe800 */
            r[1]=59392u;r[15]=0x50e4u;return 0;
        case 0x50e4u: /* mov r2,#0x30 */
            r[2]=48u;r[15]=0x50e8u;return 0;
        case 0x50e8u: /* mov r3,#0x800000 */
            r[3]=8388608u;r[15]=0x50ecu;return 0;
        case 0x50ecu: /* mov r4,#0x0 */
            r[4]=0u;r[15]=0x50f0u;return 0;
        case 0x50f0u: /* str r4,[r1,#0x0] */
            STORE(r[1]+0u,r[4],4);r[15]=0x50f4u;return 0;
        case 0x50f4u: /* ldr r5,[r3,#0x0] */
            LOAD(r[5],r[3]+0u,4);r[15]=0x50f8u;return 0;
        case 0x50f8u: /* ldr r4,[0x5170] */
            LOAD(r[4],20848u,4);r[15]=0x50fcu;return 0;
        case 0x50fcu: /* and r5,r5,r4 */
            r[5]=r[5]&r[4];r[15]=0x5100u;return 0;
        case 0x5100u: /* orr r5,r5,#0x8000 */
            r[5]=r[5]|32768u;r[15]=0x5104u;return 0;
        case 0x5104u: /* str r5,[r3,#0x0] */
            STORE(r[3]+0u,r[5],4);r[15]=0x5108u;return 0;
        case 0x5108u: /* add r1,r1,#0x40 */
            r[1]=r[1]+64u;r[15]=0x510cu;return 0;
        case 0x510cu: /* add r3,r3,#0x80 */
            r[3]=r[3]+128u;r[15]=0x5110u;return 0;
        case 0x5110u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x5114u;return 0;
        case 0x5114u: /* bne 0x000050ec */
            if(!(state->cpsr&0x40000000u)){r[15]=20716u;}else{r[15]=0x5118u;}return 0;
        case 0x5118u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x511cu;return 0;
        case 0x511cu: /* mov r2,#0xc000 */
            r[2]=49152u;r[15]=0x5120u;return 0;
        case 0x5120u: /* stmdb sp!,{r0,r2,lr} */
            STORE(r[13]-12u,r[0],4);STORE(r[13]-8u,r[2],4);STORE(r[13]-4u,r[14],4);r[13]-=12u;r[15]=0x5124u;return 0;
        case 0x5124u: /* bl 0x000074ec */
            r[14]=0x5128u;r[15]=29932u;return 0;
        case 0x5128u: /* ldmia sp!,{r0,r2,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[2],r[13]+4u,4);LOAD(r[14],r[13]+8u,4);r[13]+=12u;r[15]=0x512cu;return 0;
        case 0x512cu: /* ldrb r3,[r2,#0x28] */
            LOAD(r[3],r[2]+40u,1);r[15]=0x5130u;return 0;
        case 0x5130u: /* and r3,r3,#0xfc */
            r[3]=r[3]&252u;r[15]=0x5134u;return 0;
        case 0x5134u: /* and r1,r1,#0x3 */
            r[1]=r[1]&3u;r[15]=0x5138u;return 0;
        case 0x5138u: /* orr r3,r3,r1 */
            r[3]=r[3]|r[1];r[15]=0x513cu;return 0;
        case 0x513cu: /* ldr r4,[0x5178] */
            LOAD(r[4],20856u,4);r[15]=0x5140u;return 0;
        case 0x5140u: /* ldrb r1,[r4,#0x0] */
            LOAD(r[1],r[4]+0u,1);r[15]=0x5144u;return 0;
        case 0x5144u: /* strb r1,[r2,#0x29] */
            STORE(r[2]+41u,r[1],1);r[15]=0x5148u;return 0;
        case 0x5148u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x514cu;return 0;
        case 0x514cu: /* orrne r3,r3,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[3]|128u;}r[15]=0x5150u;return 0;
        case 0x5150u: /* biceq r3,r3,#0x80 */
            if(state->cpsr&0x40000000u){r[3]=r[3]&~128u;}r[15]=0x5154u;return 0;
        case 0x5154u: /* strb r3,[r2,#0x28] */
            STORE(r[2]+40u,r[3],1);r[15]=0x5158u;return 0;
        case 0x5158u: /* add r2,r2,#0x80 */
            r[2]=r[2]+128u;r[15]=0x515cu;return 0;
        case 0x515cu: /* add r0,r0,#0x1 */
            r[0]=r[0]+1u;r[15]=0x5160u;return 0;
        case 0x5160u: /* cmp r0,#0x8 */
            (void)subtract(state,r[0],8u);r[15]=0x5164u;return 0;
        case 0x5164u: /* bne 0x00005120 */
            if(!(state->cpsr&0x40000000u)){r[15]=20768u;}else{r[15]=0x5168u;}return 0;
        case 0x5168u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x517cu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5180u;return 0;
        case 0x5180u: /* ldrb r3,[r10,#0x3] */
            LOAD(r[3],r[10]+3u,1);r[15]=0x5184u;return 0;
        case 0x5184u: /* mov r3,r3, lsl #0x3 */
            r[3]=(r[3]<<3);r[15]=0x5188u;return 0;
        case 0x5188u: /* ldr r4,[0x54e4] */
            LOAD(r[4],21732u,4);r[15]=0x518cu;return 0;
        case 0x518cu: /* add r4,r4,r3 */
            r[4]=r[4]+r[3];r[15]=0x5190u;return 0;
        case 0x5190u: /* ldr r3,[r4,#0x0] */
            LOAD(r[3],r[4]+0u,4);r[15]=0x5194u;return 0;
        case 0x5194u: /* ldr r5,[r3,#0x0] */
            LOAD(r[5],r[3]+0u,4);r[15]=0x5198u;return 0;
        case 0x5198u: /* ldr r4,[0x5264] */
            LOAD(r[4],21092u,4);r[15]=0x519cu;return 0;
        case 0x519cu: /* cmp r5,r4 */
            (void)subtract(state,r[5],r[4]);r[15]=0x51a0u;return 0;
        case 0x51a0u: /* movne r1,#0x2 */
            if(!(state->cpsr&0x40000000u)){r[1]=2u;}r[15]=0x51a4u;return 0;
        case 0x51a4u: /* bne 0x0000522c */
            if(!(state->cpsr&0x40000000u)){r[15]=21036u;}else{r[15]=0x51a8u;}return 0;
        case 0x51a8u: /* ldrb r5,[r3,#0x4] */
            LOAD(r[5],r[3]+4u,1);r[15]=0x51acu;return 0;
        case 0x51acu: /* cmp r5,#0x1 */
            (void)subtract(state,r[5],1u);r[15]=0x51b0u;return 0;
        case 0x51b0u: /* movne r1,#0x8 */
            if(!(state->cpsr&0x40000000u)){r[1]=8u;}r[15]=0x51b4u;return 0;
        case 0x51b4u: /* bne 0x0000522c */
            if(!(state->cpsr&0x40000000u)){r[15]=21036u;}else{r[15]=0x51b8u;}return 0;
        case 0x51b8u: /* ldr r6,[r3,#0x8] */
            LOAD(r[6],r[3]+8u,4);r[15]=0x51bcu;return 0;
        case 0x51bcu: /* sub r6,r6,#0x4 */
            r[6]=r[6]-4u;r[15]=0x51c0u;return 0;
        case 0x51c0u: /* ldr r5,[r3,r6] */
            LOAD(r[5],r[3]+r[6],4);r[15]=0x51c4u;return 0;
        case 0x51c4u: /* ldr r6,[0x5268] */
            LOAD(r[6],21096u,4);r[15]=0x51c8u;return 0;
        case 0x51c8u: /* cmp r5,r6 */
            (void)subtract(state,r[5],r[6]);r[15]=0x51ccu;return 0;
        case 0x51ccu: /* movne r1,#0x4 */
            if(!(state->cpsr&0x40000000u)){r[1]=4u;}r[15]=0x51d0u;return 0;
        case 0x51d0u: /* bne 0x0000522c */
            if(!(state->cpsr&0x40000000u)){r[15]=21036u;}else{r[15]=0x51d4u;}return 0;
        case 0x51d4u: /* ldrb r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,1);r[15]=0x51d8u;return 0;
        case 0x51d8u: /* ldrb r2,[r10,#0x5] */
            LOAD(r[2],r[10]+5u,1);r[15]=0x51dcu;return 0;
        case 0x51dcu: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x51e0u;return 0;
        case 0x51e0u: /* bl 0x0000780c */
            r[14]=0x51e4u;r[15]=30732u;return 0;
        case 0x51e4u: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x51e8u;return 0;
        case 0x51e8u: /* mov r4,#0x11000 */
            r[4]=69632u;r[15]=0x51ecu;return 0;
        case 0x51ecu: /* mov r1,#0x58 */
            r[1]=88u;r[15]=0x51f0u;return 0;
        case 0x51f0u: /* mla r4,r0,r1,r4 */
            r[4]=r[0]*r[1]+r[4];r[15]=0x51f4u;return 0;
        case 0x51f4u: /* ldrb r1,[r4,#0x1] */
            LOAD(r[1],r[4]+1u,1);r[15]=0x51f8u;return 0;
        case 0x51f8u: /* mov r2,#0x13800 */
            r[2]=79872u;r[15]=0x51fcu;return 0;
        case 0x51fcu: /* mov r0,r0, lsl #0x5 */
            r[0]=(r[0]<<5);r[15]=0x5200u;return 0;
        case 0x5200u: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x5204u;return 0;
        case 0x5204u: /* ldrb r0,[r4,#0x0] */
            LOAD(r[0],r[4]+0u,1);r[15]=0x5208u;return 0;
        case 0x5208u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x520cu;return 0;
        case 0x520cu: /* orrne r0,r0,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[0]=r[0]|128u;}r[15]=0x5210u;return 0;
        case 0x5210u: /* biceq r0,r0,#0x80 */
            if(state->cpsr&0x40000000u){r[0]=r[0]&~128u;}r[15]=0x5214u;return 0;
        case 0x5214u: /* strb r0,[r4,#0x0] */
            STORE(r[4]+0u,r[0],1);r[15]=0x5218u;return 0;
        case 0x5218u: /* strb r1,[r2,#0x1c] */
            STORE(r[2]+28u,r[1],1);r[15]=0x521cu;return 0;
        case 0x521cu: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x5220u;return 0;
        case 0x5220u: /* strb r1,[r4,#0x1] */
            STORE(r[4]+1u,r[1],1);r[15]=0x5224u;return 0;
        case 0x5224u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x522cu: /* mov r4,#0x11000 */
            r[4]=69632u;r[15]=0x5230u;return 0;
        case 0x5230u: /* mov r2,#0x58 */
            r[2]=88u;r[15]=0x5234u;return 0;
        case 0x5234u: /* mla r4,r0,r2,r4 */
            r[4]=r[0]*r[2]+r[4];r[15]=0x5238u;return 0;
        case 0x5238u: /* ldrb r2,[r4,#0x0] */
            LOAD(r[2],r[4]+0u,1);r[15]=0x523cu;return 0;
        case 0x523cu: /* orr r2,r2,#0x80 */
            r[2]=r[2]|128u;r[15]=0x5240u;return 0;
        case 0x5240u: /* strb r2,[r4,#0x0] */
            STORE(r[4]+0u,r[2],1);r[15]=0x5244u;return 0;
        case 0x5244u: /* mov r2,#0x13800 */
            r[2]=79872u;r[15]=0x5248u;return 0;
        case 0x5248u: /* mov r0,r0, lsl #0x5 */
            r[0]=(r[0]<<5);r[15]=0x524cu;return 0;
        case 0x524cu: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x5250u;return 0;
        case 0x5250u: /* ldrb r0,[r2,#0x1f] */
            LOAD(r[0],r[2]+31u,1);r[15]=0x5254u;return 0;
        case 0x5254u: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0x5258u;return 0;
        case 0x5258u: /* strb r0,[r2,#0x1f] */
            STORE(r[2]+31u,r[0],1);r[15]=0x525cu;return 0;
        case 0x525cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x526cu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5270u;return 0;
        case 0x5270u: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x5274u;return 0;
        case 0x5274u: /* bl 0x00007dc0 */
            r[14]=0x5278u;r[15]=32192u;return 0;
        case 0x5278u: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x527cu;return 0;
        case 0x527cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5280u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5284u;return 0;
        case 0x5284u: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x5288u;return 0;
        case 0x5288u: /* bl 0x00007f74 */
            r[14]=0x528cu;r[15]=32628u;return 0;
        case 0x528cu: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x5290u;return 0;
        case 0x5290u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5294u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5298u;return 0;
        case 0x5298u: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x529cu;return 0;
        case 0x529cu: /* bl 0x00007fd0 */
            r[14]=0x52a0u;r[15]=32720u;return 0;
        case 0x52a0u: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x52a4u;return 0;
        case 0x52a4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x52a8u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x52acu;return 0;
        case 0x52acu: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x52b0u;return 0;
        case 0x52b0u: /* ldr r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,4);r[15]=0x52b4u;return 0;
        case 0x52b4u: /* ldr r3,[0x52cc] */
            LOAD(r[3],21196u,4);r[15]=0x52b8u;return 0;
        case 0x52b8u: /* and r2,r2,r3 */
            r[2]=r[2]&r[3];r[15]=0x52bcu;return 0;
        case 0x52bcu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x52c0u;return 0;
        case 0x52c0u: /* bl 0x00006d84 */
            r[14]=0x52c4u;r[15]=28036u;return 0;
        case 0x52c4u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x52c8u;return 0;
        case 0x52c8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x52d0u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x52d4u;return 0;
        case 0x52d4u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x52d8u;return 0;
        case 0x52d8u: /* ldr r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,4);r[15]=0x52dcu;return 0;
        case 0x52dcu: /* ldr r3,[0x52f4] */
            LOAD(r[3],21236u,4);r[15]=0x52e0u;return 0;
        case 0x52e0u: /* and r2,r2,r3 */
            r[2]=r[2]&r[3];r[15]=0x52e4u;return 0;
        case 0x52e4u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x52e8u;return 0;
        case 0x52e8u: /* bl 0x00006e0c */
            r[14]=0x52ecu;r[15]=28172u;return 0;
        case 0x52ecu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x52f0u;return 0;
        case 0x52f0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x52f8u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x52fcu;return 0;
        case 0x52fcu: /* ldr r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,4);r[15]=0x5300u;return 0;
        case 0x5300u: /* mov r2,r1, lsr #0x10 */
            r[2]=(r[1]>>16);r[15]=0x5304u;return 0;
        case 0x5304u: /* ldr r3,[0x5388] */
            LOAD(r[3],21384u,4);r[15]=0x5308u;return 0;
        case 0x5308u: /* and r1,r1,r3 */
            r[1]=r[1]&r[3];r[15]=0x530cu;return 0;
        case 0x530cu: /* mov r4,#0x58 */
            r[4]=88u;r[15]=0x5310u;return 0;
        case 0x5310u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x5314u;return 0;
        case 0x5314u: /* mov r0,#0x11000 */
            r[0]=69632u;r[15]=0x5318u;return 0;
        case 0x5318u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x531cu;return 0;
        case 0x531cu: /* ands r2,r2,r3 */
            temporary=r[2]&r[3];r[2]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x5320u;return 0;
        case 0x5320u: /* beq 0x00005364 */
            if(state->cpsr&0x40000000u){r[15]=21348u;}else{r[15]=0x5324u;}return 0;
        case 0x5324u: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x5328u;return 0;
        case 0x5328u: /* ldr r0,[r4,#0x44] */
            LOAD(r[0],r[4]+68u,4);r[15]=0x532cu;return 0;
        case 0x532cu: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x5330u;return 0;
        case 0x5330u: /* ldr r3,[0x538c] */
            LOAD(r[3],21388u,4);r[15]=0x5334u;return 0;
        case 0x5334u: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x5338u;return 0;
        case 0x5338u: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x533cu;return 0;
        case 0x533cu: /* ldr r3,[0x5388] */
            LOAD(r[3],21384u,4);r[15]=0x5340u;return 0;
        case 0x5340u: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0x5344u;return 0;
        case 0x5344u: /* ldr r3,[0x5390] */
            LOAD(r[3],21392u,4);r[15]=0x5348u;return 0;
        case 0x5348u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x534cu;return 0;
        case 0x534cu: /* beq 0x00005364 */
            if(state->cpsr&0x40000000u){r[15]=21348u;}else{r[15]=0x5350u;}return 0;
        case 0x5350u: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x5354u;return 0;
        case 0x5354u: /* bne 0x00005364 */
            if(!(state->cpsr&0x40000000u)){r[15]=21348u;}else{r[15]=0x5358u;}return 0;
        case 0x5358u: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x535cu;return 0;
        case 0x535cu: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x5360u;return 0;
        case 0x5360u: /* b 0x00005348 */
            r[15]=21320u;return 0;
        case 0x5364u: /* ldr r3,[0x5394] */
            LOAD(r[3],21396u,4);r[15]=0x5368u;return 0;
        case 0x5368u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x536cu;return 0;
        case 0x536cu: /* movne r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=255u;}r[15]=0x5370u;return 0;
        case 0x5370u: /* str r1,[r4,#0x40] */
            STORE(r[4]+64u,r[1],4);r[15]=0x5374u;return 0;
        case 0x5374u: /* strb r2,[r4,#0x48] */
            STORE(r[4]+72u,r[2],1);r[15]=0x5378u;return 0;
        case 0x5378u: /* strb r2,[r4,#0x49] */
            STORE(r[4]+73u,r[2],1);r[15]=0x537cu;return 0;
        case 0x537cu: /* strb r0,[r4,#0x4a] */
            STORE(r[4]+74u,r[0],1);r[15]=0x5380u;return 0;
        case 0x5380u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5384u;return 0;
        case 0x5384u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5398u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x539cu;return 0;
        case 0x539cu: /* ldr r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,4);r[15]=0x53a0u;return 0;
        case 0x53a0u: /* mov r2,r1, lsr #0x10 */
            r[2]=(r[1]>>16);r[15]=0x53a4u;return 0;
        case 0x53a4u: /* ldr r3,[0x5428] */
            LOAD(r[3],21544u,4);r[15]=0x53a8u;return 0;
        case 0x53a8u: /* and r1,r1,r3 */
            r[1]=r[1]&r[3];r[15]=0x53acu;return 0;
        case 0x53acu: /* mov r4,#0x58 */
            r[4]=88u;r[15]=0x53b0u;return 0;
        case 0x53b0u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x53b4u;return 0;
        case 0x53b4u: /* mov r0,#0x11000 */
            r[0]=69632u;r[15]=0x53b8u;return 0;
        case 0x53b8u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x53bcu;return 0;
        case 0x53bcu: /* ands r2,r2,r3 */
            temporary=r[2]&r[3];r[2]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x53c0u;return 0;
        case 0x53c0u: /* beq 0x00005404 */
            if(state->cpsr&0x40000000u){r[15]=21508u;}else{r[15]=0x53c4u;}return 0;
        case 0x53c4u: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x53c8u;return 0;
        case 0x53c8u: /* ldr r0,[r4,#0x50] */
            LOAD(r[0],r[4]+80u,4);r[15]=0x53ccu;return 0;
        case 0x53ccu: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x53d0u;return 0;
        case 0x53d0u: /* ldr r3,[0x542c] */
            LOAD(r[3],21548u,4);r[15]=0x53d4u;return 0;
        case 0x53d4u: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x53d8u;return 0;
        case 0x53d8u: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x53dcu;return 0;
        case 0x53dcu: /* ldr r3,[0x5428] */
            LOAD(r[3],21544u,4);r[15]=0x53e0u;return 0;
        case 0x53e0u: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0x53e4u;return 0;
        case 0x53e4u: /* ldr r3,[0x5430] */
            LOAD(r[3],21552u,4);r[15]=0x53e8u;return 0;
        case 0x53e8u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x53ecu;return 0;
        case 0x53ecu: /* beq 0x00005404 */
            if(state->cpsr&0x40000000u){r[15]=21508u;}else{r[15]=0x53f0u;}return 0;
        case 0x53f0u: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x53f4u;return 0;
        case 0x53f4u: /* bne 0x00005404 */
            if(!(state->cpsr&0x40000000u)){r[15]=21508u;}else{r[15]=0x53f8u;}return 0;
        case 0x53f8u: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x53fcu;return 0;
        case 0x53fcu: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x5400u;return 0;
        case 0x5400u: /* b 0x000053e8 */
            r[15]=21480u;return 0;
        case 0x5404u: /* ldr r3,[0x5434] */
            LOAD(r[3],21556u,4);r[15]=0x5408u;return 0;
        case 0x5408u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x540cu;return 0;
        case 0x540cu: /* movne r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=255u;}r[15]=0x5410u;return 0;
        case 0x5410u: /* str r1,[r4,#0x4c] */
            STORE(r[4]+76u,r[1],4);r[15]=0x5414u;return 0;
        case 0x5414u: /* strb r2,[r4,#0x54] */
            STORE(r[4]+84u,r[2],1);r[15]=0x5418u;return 0;
        case 0x5418u: /* strb r2,[r4,#0x55] */
            STORE(r[4]+85u,r[2],1);r[15]=0x541cu;return 0;
        case 0x541cu: /* strb r0,[r4,#0x56] */
            STORE(r[4]+86u,r[0],1);r[15]=0x5420u;return 0;
        case 0x5420u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5424u;return 0;
        case 0x5424u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5438u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x543cu;return 0;
        case 0x543cu: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x5440u;return 0;
        case 0x5440u: /* ldrb r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,1);r[15]=0x5444u;return 0;
        case 0x5444u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5448u;return 0;
        case 0x5448u: /* bl 0x00006ed4 */
            r[14]=0x544cu;r[15]=28372u;return 0;
        case 0x544cu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5450u;return 0;
        case 0x5450u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5454u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5458u;return 0;
        case 0x5458u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x545cu;return 0;
        case 0x545cu: /* ldrb r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,1);r[15]=0x5460u;return 0;
        case 0x5460u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5464u;return 0;
        case 0x5464u: /* bl 0x00006f68 */
            r[14]=0x5468u;r[15]=28520u;return 0;
        case 0x5468u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x546cu;return 0;
        case 0x546cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5470u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5474u;return 0;
        case 0x5474u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x5478u;return 0;
        case 0x5478u: /* ldrb r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,1);r[15]=0x547cu;return 0;
        case 0x547cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5480u;return 0;
        case 0x5480u: /* bl 0x00006ff0 */
            r[14]=0x5484u;r[15]=28656u;return 0;
        case 0x5484u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5488u;return 0;
        case 0x5488u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x548cu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5490u;return 0;
        case 0x5490u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x5494u;return 0;
        case 0x5494u: /* ldr r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,4);r[15]=0x5498u;return 0;
        case 0x5498u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x549cu;return 0;
        case 0x549cu: /* bl 0x000081d0 */
            r[14]=0x54a0u;r[15]=33232u;return 0;
        case 0x54a0u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x54a4u;return 0;
        case 0x54a4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x54a8u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x54acu;return 0;
        case 0x54acu: /* cmp r0,#0xff */
            (void)subtract(state,r[0],255u);r[15]=0x54b0u;return 0;
        case 0x54b0u: /* beq 0x000054c4 */
            if(state->cpsr&0x40000000u){r[15]=21700u;}else{r[15]=0x54b4u;}return 0;
        case 0x54b4u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x54b8u;return 0;
        case 0x54b8u: /* bl 0x00008014 */
            r[14]=0x54bcu;r[15]=32788u;return 0;
        case 0x54bcu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x54c0u;return 0;
        case 0x54c0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x54c4u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x54c8u;return 0;
        case 0x54c8u: /* bl 0x00008008 */
            r[14]=0x54ccu;r[15]=32776u;return 0;
        case 0x54ccu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x54d0u;return 0;
        case 0x54d0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x54d4u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x54d8u;return 0;
        case 0x54d8u: /* bl 0x00007e90 */
            r[14]=0x54dcu;r[15]=32400u;return 0;
        case 0x54dcu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x54e0u;return 0;
        case 0x54e0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x54e8u: /* ldrb r2,[r10,#0x2] */
            LOAD(r[2],r[10]+2u,1);r[15]=0x54ecu;return 0;
        case 0x54ecu: /* mov r1,r2, lsr #0x4 */
            r[1]=(r[2]>>4);r[15]=0x54f0u;return 0;
        case 0x54f0u: /* and r1,r1,#0x3 */
            r[1]=r[1]&3u;r[15]=0x54f4u;return 0;
        case 0x54f4u: /* and r0,r2,#0xf */
            r[0]=r[2]&15u;r[15]=0x54f8u;return 0;
        case 0x54f8u: /* ldrb r3,[r10,#0x3] */
            LOAD(r[3],r[10]+3u,1);r[15]=0x54fcu;return 0;
        case 0x54fcu: /* ldr r4,[r10,#0x4] */
            LOAD(r[4],r[10]+4u,4);r[15]=0x5500u;return 0;
        case 0x5500u: /* ldr r5,[0x55bc] */
            LOAD(r[5],21948u,4);r[15]=0x5504u;return 0;
        case 0x5504u: /* and r4,r4,r5 */
            r[4]=r[4]&r[5];r[15]=0x5508u;return 0;
        case 0x5508u: /* ldrb r5,[r10,#0x6] */
            LOAD(r[5],r[10]+6u,1);r[15]=0x550cu;return 0;
        case 0x550cu: /* ldrb r6,[r10,#0x7] */
            LOAD(r[6],r[10]+7u,1);r[15]=0x5510u;return 0;
        case 0x5510u: /* ldrb r7,[r10,#0x8] */
            LOAD(r[7],r[10]+8u,1);r[15]=0x5514u;return 0;
        case 0x5514u: /* ands r2,r2,#0x80 */
            temporary=r[2]&128u;r[2]=temporary;logic_flags(state,temporary,0,128u);r[15]=0x5518u;return 0;
        case 0x5518u: /* moveq r7,#0x0 */
            if(state->cpsr&0x40000000u){r[7]=0u;}r[15]=0x551cu;return 0;
        case 0x551cu: /* orr r7,r6,r7, lsl #0x8 */
            r[7]=r[6]|(r[7]<<8);r[15]=0x5520u;return 0;
        case 0x5520u: /* orr r2,r7,r2, lsl #0x9 */
            r[2]=r[7]|(r[2]<<9);r[15]=0x5524u;return 0;
        case 0x5524u: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x5528u;return 0;
        case 0x5528u: /* bl 0x0000793c */
            r[14]=0x552cu;r[15]=31036u;return 0;
        case 0x552cu: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x5530u;return 0;
        case 0x5530u: /* ldr r5,[0x5854] */
            LOAD(r[5],22612u,4);r[15]=0x5534u;return 0;
        case 0x5534u: /* mov r3,r0, lsl #0x6 */
            r[3]=(r[0]<<6);r[15]=0x5538u;return 0;
        case 0x5538u: /* add r5,r5,r3 */
            r[5]=r[5]+r[3];r[15]=0x553cu;return 0;
        case 0x553cu: /* and r1,r1,#0x17 */
            r[1]=r[1]&23u;r[15]=0x5540u;return 0;
        case 0x5540u: /* and r2,r2,#0x17 */
            r[2]=r[2]&23u;r[15]=0x5544u;return 0;
        case 0x5544u: /* orr r3,r3,r1 */
            r[3]=r[3]|r[1];r[15]=0x5548u;return 0;
        case 0x5548u: /* ldrb r3,[r5,#0x0] */
            LOAD(r[3],r[5]+0u,1);r[15]=0x554cu;return 0;
        case 0x554cu: /* and r3,r3,#0xe8 */
            r[3]=r[3]&232u;r[15]=0x5550u;return 0;
        case 0x5550u: /* strb r3,[r5,#0x0] */
            STORE(r[5]+0u,r[3],1);r[15]=0x5554u;return 0;
        case 0x5554u: /* ldrb r3,[r5,#0x20] */
            LOAD(r[3],r[5]+32u,1);r[15]=0x5558u;return 0;
        case 0x5558u: /* and r3,r3,#0xe8 */
            r[3]=r[3]&232u;r[15]=0x555cu;return 0;
        case 0x555cu: /* strb r3,[r5,#0x20] */
            STORE(r[5]+32u,r[3],1);r[15]=0x5560u;return 0;
        case 0x5560u: /* ldr r4,[0x5858] */
            LOAD(r[4],22616u,4);r[15]=0x5564u;return 0;
        case 0x5564u: /* mov r1,#0xb0 */
            r[1]=176u;r[15]=0x5568u;return 0;
        case 0x5568u: /* mla r4,r0,r1,r4 */
            r[4]=r[0]*r[1]+r[4];r[15]=0x556cu;return 0;
        case 0x556cu: /* ldrb r1,[r4,#0x1] */
            LOAD(r[1],r[4]+1u,1);r[15]=0x5570u;return 0;
        case 0x5570u: /* ldrb r0,[r4,#0x0] */
            LOAD(r[0],r[4]+0u,1);r[15]=0x5574u;return 0;
        case 0x5574u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x5578u;return 0;
        case 0x5578u: /* orrne r0,r0,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[0]=r[0]|128u;}r[15]=0x557cu;return 0;
        case 0x557cu: /* biceq r0,r0,#0x80 */
            if(state->cpsr&0x40000000u){r[0]=r[0]&~128u;}r[15]=0x5580u;return 0;
        case 0x5580u: /* strb r0,[r4,#0x0] */
            STORE(r[4]+0u,r[0],1);r[15]=0x5584u;return 0;
        case 0x5584u: /* strb r1,[r5,#0x1c] */
            STORE(r[5]+28u,r[1],1);r[15]=0x5588u;return 0;
        case 0x5588u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x558cu;return 0;
        case 0x558cu: /* strb r0,[r4,#0x1] */
            STORE(r[4]+1u,r[0],1);r[15]=0x5590u;return 0;
        case 0x5590u: /* add r4,r4,#0x58 */
            r[4]=r[4]+88u;r[15]=0x5594u;return 0;
        case 0x5594u: /* ldrb r1,[r4,#0x1] */
            LOAD(r[1],r[4]+1u,1);r[15]=0x5598u;return 0;
        case 0x5598u: /* ldrb r0,[r4,#0x0] */
            LOAD(r[0],r[4]+0u,1);r[15]=0x559cu;return 0;
        case 0x559cu: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x55a0u;return 0;
        case 0x55a0u: /* orrne r0,r0,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[0]=r[0]|128u;}r[15]=0x55a4u;return 0;
        case 0x55a4u: /* biceq r0,r0,#0x80 */
            if(state->cpsr&0x40000000u){r[0]=r[0]&~128u;}r[15]=0x55a8u;return 0;
        case 0x55a8u: /* strb r0,[r4,#0x0] */
            STORE(r[4]+0u,r[0],1);r[15]=0x55acu;return 0;
        case 0x55acu: /* strb r1,[r5,#0x3c] */
            STORE(r[5]+60u,r[1],1);r[15]=0x55b0u;return 0;
        case 0x55b0u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x55b4u;return 0;
        case 0x55b4u: /* strb r0,[r4,#0x1] */
            STORE(r[4]+1u,r[0],1);r[15]=0x55b8u;return 0;
        case 0x55b8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x55c0u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x55c4u;return 0;
        case 0x55c4u: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x55c8u;return 0;
        case 0x55c8u: /* bl 0x00007df0 */
            r[14]=0x55ccu;r[15]=32240u;return 0;
        case 0x55ccu: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x55d0u;return 0;
        case 0x55d0u: /* ldr r5,[0x5854] */
            LOAD(r[5],22612u,4);r[15]=0x55d4u;return 0;
        case 0x55d4u: /* mov r3,r0, lsl #0x6 */
            r[3]=(r[0]<<6);r[15]=0x55d8u;return 0;
        case 0x55d8u: /* add r5,r5,r3 */
            r[5]=r[5]+r[3];r[15]=0x55dcu;return 0;
        case 0x55dcu: /* and r1,r1,#0x17 */
            r[1]=r[1]&23u;r[15]=0x55e0u;return 0;
        case 0x55e0u: /* and r2,r2,#0x17 */
            r[2]=r[2]&23u;r[15]=0x55e4u;return 0;
        case 0x55e4u: /* orr r3,r3,r1 */
            r[3]=r[3]|r[1];r[15]=0x55e8u;return 0;
        case 0x55e8u: /* ldrb r3,[r5,#0x0] */
            LOAD(r[3],r[5]+0u,1);r[15]=0x55ecu;return 0;
        case 0x55ecu: /* and r3,r3,#0xe8 */
            r[3]=r[3]&232u;r[15]=0x55f0u;return 0;
        case 0x55f0u: /* strb r3,[r5,#0x0] */
            STORE(r[5]+0u,r[3],1);r[15]=0x55f4u;return 0;
        case 0x55f4u: /* ldrb r3,[r5,#0x20] */
            LOAD(r[3],r[5]+32u,1);r[15]=0x55f8u;return 0;
        case 0x55f8u: /* and r3,r3,#0xe8 */
            r[3]=r[3]&232u;r[15]=0x55fcu;return 0;
        case 0x55fcu: /* strb r3,[r5,#0x20] */
            STORE(r[5]+32u,r[3],1);r[15]=0x5600u;return 0;
        case 0x5600u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5604u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5608u;return 0;
        case 0x5608u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x560cu;return 0;
        case 0x560cu: /* ldr r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,4);r[15]=0x5610u;return 0;
        case 0x5610u: /* ldrb r4,[r10,#0x7] */
            LOAD(r[4],r[10]+7u,1);r[15]=0x5614u;return 0;
        case 0x5614u: /* ldr r5,[r10,#0x8] */
            LOAD(r[5],r[10]+8u,4);r[15]=0x5618u;return 0;
        case 0x5618u: /* ldr r3,[0x5634] */
            LOAD(r[3],22068u,4);r[15]=0x561cu;return 0;
        case 0x561cu: /* and r2,r2,r3 */
            r[2]=r[2]&r[3];r[15]=0x5620u;return 0;
        case 0x5620u: /* and r5,r5,r3 */
            r[5]=r[5]&r[3];r[15]=0x5624u;return 0;
        case 0x5624u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5628u;return 0;
        case 0x5628u: /* bl 0x00006d9c */
            r[14]=0x562cu;r[15]=28060u;return 0;
        case 0x562cu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5630u;return 0;
        case 0x5630u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5638u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x563cu;return 0;
        case 0x563cu: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x5640u;return 0;
        case 0x5640u: /* ldr r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,4);r[15]=0x5644u;return 0;
        case 0x5644u: /* ldrb r4,[r10,#0x7] */
            LOAD(r[4],r[10]+7u,1);r[15]=0x5648u;return 0;
        case 0x5648u: /* ldr r5,[r10,#0x8] */
            LOAD(r[5],r[10]+8u,4);r[15]=0x564cu;return 0;
        case 0x564cu: /* ldr r3,[0x5668] */
            LOAD(r[3],22120u,4);r[15]=0x5650u;return 0;
        case 0x5650u: /* and r2,r2,r3 */
            r[2]=r[2]&r[3];r[15]=0x5654u;return 0;
        case 0x5654u: /* and r5,r5,r3 */
            r[5]=r[5]&r[3];r[15]=0x5658u;return 0;
        case 0x5658u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x565cu;return 0;
        case 0x565cu: /* bl 0x00006e24 */
            r[14]=0x5660u;r[15]=28196u;return 0;
        case 0x5660u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5664u;return 0;
        case 0x5664u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x566cu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5670u;return 0;
        case 0x5670u: /* ldr r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,4);r[15]=0x5674u;return 0;
        case 0x5674u: /* mov r2,r1, lsr #0x10 */
            r[2]=(r[1]>>16);r[15]=0x5678u;return 0;
        case 0x5678u: /* ldr r3,[0x570c] */
            LOAD(r[3],22284u,4);r[15]=0x567cu;return 0;
        case 0x567cu: /* and r1,r1,r3 */
            r[1]=r[1]&r[3];r[15]=0x5680u;return 0;
        case 0x5680u: /* mov r4,#0xb0 */
            r[4]=176u;r[15]=0x5684u;return 0;
        case 0x5684u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x5688u;return 0;
        case 0x5688u: /* ldr r0,[0x5858] */
            LOAD(r[0],22616u,4);r[15]=0x568cu;return 0;
        case 0x568cu: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x5690u;return 0;
        case 0x5690u: /* ands r2,r2,r3 */
            temporary=r[2]&r[3];r[2]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x5694u;return 0;
        case 0x5694u: /* beq 0x000056d8 */
            if(state->cpsr&0x40000000u){r[15]=22232u;}else{r[15]=0x5698u;}return 0;
        case 0x5698u: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x569cu;return 0;
        case 0x569cu: /* ldr r0,[r4,#0x44] */
            LOAD(r[0],r[4]+68u,4);r[15]=0x56a0u;return 0;
        case 0x56a0u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x56a4u;return 0;
        case 0x56a4u: /* ldr r3,[0x5710] */
            LOAD(r[3],22288u,4);r[15]=0x56a8u;return 0;
        case 0x56a8u: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x56acu;return 0;
        case 0x56acu: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x56b0u;return 0;
        case 0x56b0u: /* ldr r3,[0x570c] */
            LOAD(r[3],22284u,4);r[15]=0x56b4u;return 0;
        case 0x56b4u: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0x56b8u;return 0;
        case 0x56b8u: /* ldr r3,[0x5714] */
            LOAD(r[3],22292u,4);r[15]=0x56bcu;return 0;
        case 0x56bcu: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x56c0u;return 0;
        case 0x56c0u: /* beq 0x000056d8 */
            if(state->cpsr&0x40000000u){r[15]=22232u;}else{r[15]=0x56c4u;}return 0;
        case 0x56c4u: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x56c8u;return 0;
        case 0x56c8u: /* bne 0x000056d8 */
            if(!(state->cpsr&0x40000000u)){r[15]=22232u;}else{r[15]=0x56ccu;}return 0;
        case 0x56ccu: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x56d0u;return 0;
        case 0x56d0u: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x56d4u;return 0;
        case 0x56d4u: /* b 0x000056bc */
            r[15]=22204u;return 0;
        case 0x56d8u: /* ldr r3,[0x5718] */
            LOAD(r[3],22296u,4);r[15]=0x56dcu;return 0;
        case 0x56dcu: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x56e0u;return 0;
        case 0x56e0u: /* movne r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=255u;}r[15]=0x56e4u;return 0;
        case 0x56e4u: /* str r1,[r4,#0x40] */
            STORE(r[4]+64u,r[1],4);r[15]=0x56e8u;return 0;
        case 0x56e8u: /* strb r2,[r4,#0x48] */
            STORE(r[4]+72u,r[2],1);r[15]=0x56ecu;return 0;
        case 0x56ecu: /* strb r2,[r4,#0x49] */
            STORE(r[4]+73u,r[2],1);r[15]=0x56f0u;return 0;
        case 0x56f0u: /* strb r0,[r4,#0x4a] */
            STORE(r[4]+74u,r[0],1);r[15]=0x56f4u;return 0;
        case 0x56f4u: /* str r1,[r4,#0x98] */
            STORE(r[4]+152u,r[1],4);r[15]=0x56f8u;return 0;
        case 0x56f8u: /* strb r2,[r4,#0xa0] */
            STORE(r[4]+160u,r[2],1);r[15]=0x56fcu;return 0;
        case 0x56fcu: /* strb r2,[r4,#0xa1] */
            STORE(r[4]+161u,r[2],1);r[15]=0x5700u;return 0;
        case 0x5700u: /* strb r0,[r4,#0xa2] */
            STORE(r[4]+162u,r[0],1);r[15]=0x5704u;return 0;
        case 0x5704u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5708u;return 0;
        case 0x5708u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x571cu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5720u;return 0;
        case 0x5720u: /* ldr r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,4);r[15]=0x5724u;return 0;
        case 0x5724u: /* mov r2,r1, lsr #0x10 */
            r[2]=(r[1]>>16);r[15]=0x5728u;return 0;
        case 0x5728u: /* ldr r3,[0x57bc] */
            LOAD(r[3],22460u,4);r[15]=0x572cu;return 0;
        case 0x572cu: /* and r1,r1,r3 */
            r[1]=r[1]&r[3];r[15]=0x5730u;return 0;
        case 0x5730u: /* mov r4,#0xb0 */
            r[4]=176u;r[15]=0x5734u;return 0;
        case 0x5734u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x5738u;return 0;
        case 0x5738u: /* ldr r0,[0x5858] */
            LOAD(r[0],22616u,4);r[15]=0x573cu;return 0;
        case 0x573cu: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x5740u;return 0;
        case 0x5740u: /* ands r2,r2,r3 */
            temporary=r[2]&r[3];r[2]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x5744u;return 0;
        case 0x5744u: /* beq 0x00005788 */
            if(state->cpsr&0x40000000u){r[15]=22408u;}else{r[15]=0x5748u;}return 0;
        case 0x5748u: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x574cu;return 0;
        case 0x574cu: /* ldr r0,[r4,#0x50] */
            LOAD(r[0],r[4]+80u,4);r[15]=0x5750u;return 0;
        case 0x5750u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x5754u;return 0;
        case 0x5754u: /* ldr r3,[0x57c0] */
            LOAD(r[3],22464u,4);r[15]=0x5758u;return 0;
        case 0x5758u: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x575cu;return 0;
        case 0x575cu: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x5760u;return 0;
        case 0x5760u: /* ldr r3,[0x57bc] */
            LOAD(r[3],22460u,4);r[15]=0x5764u;return 0;
        case 0x5764u: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0x5768u;return 0;
        case 0x5768u: /* ldr r3,[0x57c4] */
            LOAD(r[3],22468u,4);r[15]=0x576cu;return 0;
        case 0x576cu: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x5770u;return 0;
        case 0x5770u: /* beq 0x00005788 */
            if(state->cpsr&0x40000000u){r[15]=22408u;}else{r[15]=0x5774u;}return 0;
        case 0x5774u: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x5778u;return 0;
        case 0x5778u: /* bne 0x00005788 */
            if(!(state->cpsr&0x40000000u)){r[15]=22408u;}else{r[15]=0x577cu;}return 0;
        case 0x577cu: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x5780u;return 0;
        case 0x5780u: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x5784u;return 0;
        case 0x5784u: /* b 0x0000576c */
            r[15]=22380u;return 0;
        case 0x5788u: /* ldr r3,[0x57c8] */
            LOAD(r[3],22472u,4);r[15]=0x578cu;return 0;
        case 0x578cu: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x5790u;return 0;
        case 0x5790u: /* movne r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=255u;}r[15]=0x5794u;return 0;
        case 0x5794u: /* str r1,[r4,#0x4c] */
            STORE(r[4]+76u,r[1],4);r[15]=0x5798u;return 0;
        case 0x5798u: /* strb r2,[r4,#0x54] */
            STORE(r[4]+84u,r[2],1);r[15]=0x579cu;return 0;
        case 0x579cu: /* strb r2,[r4,#0x55] */
            STORE(r[4]+85u,r[2],1);r[15]=0x57a0u;return 0;
        case 0x57a0u: /* strb r0,[r4,#0x56] */
            STORE(r[4]+86u,r[0],1);r[15]=0x57a4u;return 0;
        case 0x57a4u: /* str r1,[r4,#0xa4] */
            STORE(r[4]+164u,r[1],4);r[15]=0x57a8u;return 0;
        case 0x57a8u: /* strb r2,[r4,#0xac] */
            STORE(r[4]+172u,r[2],1);r[15]=0x57acu;return 0;
        case 0x57acu: /* strb r2,[r4,#0xad] */
            STORE(r[4]+173u,r[2],1);r[15]=0x57b0u;return 0;
        case 0x57b0u: /* strb r0,[r4,#0xae] */
            STORE(r[4]+174u,r[0],1);r[15]=0x57b4u;return 0;
        case 0x57b4u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x57b8u;return 0;
        case 0x57b8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x57ccu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x57d0u;return 0;
        case 0x57d0u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x57d4u;return 0;
        case 0x57d4u: /* ldrb r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,1);r[15]=0x57d8u;return 0;
        case 0x57d8u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x57dcu;return 0;
        case 0x57dcu: /* bl 0x00006eec */
            r[14]=0x57e0u;r[15]=28396u;return 0;
        case 0x57e0u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x57e4u;return 0;
        case 0x57e4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x57e8u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x57ecu;return 0;
        case 0x57ecu: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x57f0u;return 0;
        case 0x57f0u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x57f4u;return 0;
        case 0x57f4u: /* bl 0x00006f80 */
            r[14]=0x57f8u;r[15]=28544u;return 0;
        case 0x57f8u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x57fcu;return 0;
        case 0x57fcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5800u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5804u;return 0;
        case 0x5804u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x5808u;return 0;
        case 0x5808u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x580cu;return 0;
        case 0x580cu: /* bl 0x00007008 */
            r[14]=0x5810u;r[15]=28680u;return 0;
        case 0x5810u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5814u;return 0;
        case 0x5814u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5818u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x581cu;return 0;
        case 0x581cu: /* cmp r0,#0xff */
            (void)subtract(state,r[0],255u);r[15]=0x5820u;return 0;
        case 0x5820u: /* beq 0x00005834 */
            if(state->cpsr&0x40000000u){r[15]=22580u;}else{r[15]=0x5824u;}return 0;
        case 0x5824u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5828u;return 0;
        case 0x5828u: /* bl 0x00008064 */
            r[14]=0x582cu;r[15]=32868u;return 0;
        case 0x582cu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5830u;return 0;
        case 0x5830u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5834u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5838u;return 0;
        case 0x5838u: /* bl 0x00008058 */
            r[14]=0x583cu;r[15]=32856u;return 0;
        case 0x583cu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5840u;return 0;
        case 0x5840u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5844u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5848u;return 0;
        case 0x5848u: /* bl 0x00007ef8 */
            r[14]=0x584cu;r[15]=32504u;return 0;
        case 0x584cu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5850u;return 0;
        case 0x5850u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x585cu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5860u;return 0;
        case 0x5860u: /* ldr r8,[0x58e0] */
            LOAD(r[8],22752u,4);r[15]=0x5864u;return 0;
        case 0x5864u: /* mov r0,r0, lsr #0x4 */
            r[0]=(r[0]>>4);r[15]=0x5868u;return 0;
        case 0x5868u: /* strb r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],1);r[15]=0x586cu;return 0;
        case 0x586cu: /* ldrb r0,[r10,#0x3] */
            LOAD(r[0],r[10]+3u,1);r[15]=0x5870u;return 0;
        case 0x5870u: /* ldr r8,[0x58e4] */
            LOAD(r[8],22756u,4);r[15]=0x5874u;return 0;
        case 0x5874u: /* mov r0,r0, lsr #0x4 */
            r[0]=(r[0]>>4);r[15]=0x5878u;return 0;
        case 0x5878u: /* strb r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],1);r[15]=0x587cu;return 0;
        case 0x587cu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5880u;return 0;
        case 0x5880u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5884u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5888u;return 0;
        case 0x5888u: /* ldr r8,[0x58e8] */
            LOAD(r[8],22760u,4);r[15]=0x588cu;return 0;
        case 0x588cu: /* mov r0,r0, lsr #0x3 */
            r[0]=(r[0]>>3);r[15]=0x5890u;return 0;
        case 0x5890u: /* subs r0,r0,#0x10 */
            r[0]=subtract(state,r[0],16u);r[15]=0x5894u;return 0;
        case 0x5894u: /* eormi r0,r0,#0xff */
            if(state->cpsr&0x80000000u){r[0]=r[0]^255u;}r[15]=0x5898u;return 0;
        case 0x5898u: /* addmi r0,r0,#0x10 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+16u;}r[15]=0x589cu;return 0;
        case 0x589cu: /* and r0,r0,#0x1f */
            r[0]=r[0]&31u;r[15]=0x58a0u;return 0;
        case 0x58a0u: /* strb r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],1);r[15]=0x58a4u;return 0;
        case 0x58a4u: /* ldrb r0,[r10,#0x3] */
            LOAD(r[0],r[10]+3u,1);r[15]=0x58a8u;return 0;
        case 0x58a8u: /* ldr r8,[0x58ec] */
            LOAD(r[8],22764u,4);r[15]=0x58acu;return 0;
        case 0x58acu: /* mov r0,r0, lsr #0x3 */
            r[0]=(r[0]>>3);r[15]=0x58b0u;return 0;
        case 0x58b0u: /* subs r0,r0,#0x10 */
            r[0]=subtract(state,r[0],16u);r[15]=0x58b4u;return 0;
        case 0x58b4u: /* eormi r0,r0,#0xff */
            if(state->cpsr&0x80000000u){r[0]=r[0]^255u;}r[15]=0x58b8u;return 0;
        case 0x58b8u: /* addmi r0,r0,#0x10 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+16u;}r[15]=0x58bcu;return 0;
        case 0x58bcu: /* and r0,r0,#0x1f */
            r[0]=r[0]&31u;r[15]=0x58c0u;return 0;
        case 0x58c0u: /* strb r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],1);r[15]=0x58c4u;return 0;
        case 0x58c4u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x58c8u;return 0;
        case 0x58c8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x58ccu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x58d0u;return 0;
        case 0x58d0u: /* ldr r8,[0x58e8] */
            LOAD(r[8],22760u,4);r[15]=0x58d4u;return 0;
        case 0x58d4u: /* str r0,[r8],#0x4 */
            STORE(r[8],r[0],4);r[8]+=4u;r[15]=0x58d8u;return 0;
        case 0x58d8u: /* str r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],4);r[15]=0x58dcu;return 0;
        case 0x58dcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x58f0u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x58f4u;return 0;
        case 0x58f4u: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x58f8u;return 0;
        case 0x58f8u: /* beq 0x00005954 */
            if(state->cpsr&0x40000000u){r[15]=22868u;}else{r[15]=0x58fcu;}return 0;
        case 0x58fcu: /* mov r1,#0xe800 */
            r[1]=59392u;r[15]=0x5900u;return 0;
        case 0x5900u: /* mov r2,#0x30 */
            r[2]=48u;r[15]=0x5904u;return 0;
        case 0x5904u: /* mov r3,#0x800000 */
            r[3]=8388608u;r[15]=0x5908u;return 0;
        case 0x5908u: /* mov r4,#0x0 */
            r[4]=0u;r[15]=0x590cu;return 0;
        case 0x590cu: /* str r4,[r1,#0x0] */
            STORE(r[1]+0u,r[4],4);r[15]=0x5910u;return 0;
        case 0x5910u: /* ldr r5,[r3,#0x0] */
            LOAD(r[5],r[3]+0u,4);r[15]=0x5914u;return 0;
        case 0x5914u: /* mov r4,#0x700 */
            r[4]=1792u;r[15]=0x5918u;return 0;
        case 0x5918u: /* orr r4,r4,#0xff */
            r[4]=r[4]|255u;r[15]=0x591cu;return 0;
        case 0x591cu: /* and r5,r5,r4 */
            r[5]=r[5]&r[4];r[15]=0x5920u;return 0;
        case 0x5920u: /* orr r5,r5,#0x8000 */
            r[5]=r[5]|32768u;r[15]=0x5924u;return 0;
        case 0x5924u: /* str r5,[r3,#0x0] */
            STORE(r[3]+0u,r[5],4);r[15]=0x5928u;return 0;
        case 0x5928u: /* add r1,r1,#0x40 */
            r[1]=r[1]+64u;r[15]=0x592cu;return 0;
        case 0x592cu: /* add r3,r3,#0x80 */
            r[3]=r[3]+128u;r[15]=0x5930u;return 0;
        case 0x5930u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x5934u;return 0;
        case 0x5934u: /* bne 0x00005908 */
            if(!(state->cpsr&0x40000000u)){r[15]=22792u;}else{r[15]=0x5938u;}return 0;
        case 0x5938u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x593cu;return 0;
        case 0x593cu: /* stmdb sp!,{r0,r10,lr} */
            STORE(r[13]-12u,r[0],4);STORE(r[13]-8u,r[10],4);STORE(r[13]-4u,r[14],4);r[13]-=12u;r[15]=0x5940u;return 0;
        case 0x5940u: /* bl 0x000074ec */
            r[14]=0x5944u;r[15]=29932u;return 0;
        case 0x5944u: /* ldmia sp!,{r0,r10,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[10],r[13]+4u,4);LOAD(r[14],r[13]+8u,4);r[13]+=12u;r[15]=0x5948u;return 0;
        case 0x5948u: /* add r0,r0,#0x1 */
            r[0]=r[0]+1u;r[15]=0x594cu;return 0;
        case 0x594cu: /* cmp r0,#0x8 */
            (void)subtract(state,r[0],8u);r[15]=0x5950u;return 0;
        case 0x5950u: /* bne 0x0000593c */
            if(!(state->cpsr&0x40000000u)){r[15]=22844u;}else{r[15]=0x5954u;}return 0;
        case 0x5954u: /* ldrb r0,[r10,#0x3] */
            LOAD(r[0],r[10]+3u,1);r[15]=0x5958u;return 0;
        case 0x5958u: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x595cu;return 0;
        case 0x595cu: /* stmdb sp!,{r10,lr} */
            STORE(r[13]-8u,r[10],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x5960u;return 0;
        case 0x5960u: /* blne 0x00007e90 */
            if(!(state->cpsr&0x40000000u)){r[14]=0x5964u;r[15]=32400u;}else{r[15]=0x5964u;}return 0;
        case 0x5964u: /* ldmia sp!,{r10,lr} */
            LOAD(r[10],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x5968u;return 0;
        case 0x5968u: /* ldrb r0,[r10,#0x4] */
            LOAD(r[0],r[10]+4u,1);r[15]=0x596cu;return 0;
        case 0x596cu: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x5970u;return 0;
        case 0x5970u: /* stmdb sp!,{r10,lr} */
            STORE(r[13]-8u,r[10],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x5974u;return 0;
        case 0x5974u: /* blne 0x00007ef8 */
            if(!(state->cpsr&0x40000000u)){r[14]=0x5978u;r[15]=32504u;}else{r[15]=0x5978u;}return 0;
        case 0x5978u: /* ldmia sp!,{r10,lr} */
            LOAD(r[10],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x597cu;return 0;
        case 0x597cu: /* ldrb r0,[r10,#0x5] */
            LOAD(r[0],r[10]+5u,1);r[15]=0x5980u;return 0;
        case 0x5980u: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x5984u;return 0;
        case 0x5984u: /* beq 0x00005998 */
            if(state->cpsr&0x40000000u){r[15]=22936u;}else{r[15]=0x5988u;}return 0;
        case 0x5988u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x598cu;return 0;
        case 0x598cu: /* ldr r8,[0x58e8] */
            LOAD(r[8],22760u,4);r[15]=0x5990u;return 0;
        case 0x5990u: /* str r0,[r8],#0x4 */
            STORE(r[8],r[0],4);r[8]+=4u;r[15]=0x5994u;return 0;
        case 0x5994u: /* str r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],4);r[15]=0x5998u;return 0;
        case 0x5998u: /* ldrb r0,[r10,#0x6] */
            LOAD(r[0],r[10]+6u,1);r[15]=0x599cu;return 0;
        case 0x599cu: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x59a0u;return 0;
        case 0x59a0u: /* stmdb sp!,{r10,lr} */
            STORE(r[13]-8u,r[10],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x59a4u;return 0;
        case 0x59a4u: /* blne 0x00005d54 */
            if(!(state->cpsr&0x40000000u)){r[14]=0x59a8u;r[15]=23892u;}else{r[15]=0x59a8u;}return 0;
        case 0x59a8u: /* ldmia sp!,{r10,lr} */
            LOAD(r[10],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x59acu;return 0;
        case 0x59acu: /* ldrb r0,[r10,#0x7] */
            LOAD(r[0],r[10]+7u,1);r[15]=0x59b0u;return 0;
        case 0x59b0u: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x59b4u;return 0;
        case 0x59b4u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x59b8u;}return 0;
        case 0x59b8u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x59bcu;return 0;
        case 0x59bcu: /* mov r1,#0x10 */
            r[1]=16u;r[15]=0x59c0u;return 0;
        case 0x59c0u: /* ldr r8,[0x60f4] */
            LOAD(r[8],24820u,4);r[15]=0x59c4u;return 0;
        case 0x59c4u: /* str r0,[r8],#0x4 */
            STORE(r[8],r[0],4);r[8]+=4u;r[15]=0x59c8u;return 0;
        case 0x59c8u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x59ccu;return 0;
        case 0x59ccu: /* bne 0x000059c4 */
            if(!(state->cpsr&0x40000000u)){r[15]=22980u;}else{r[15]=0x59d0u;}return 0;
        case 0x59d0u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x59d4u;return 0;
        case 0x59d4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x59d8u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x59dcu;return 0;
        case 0x59dcu: /* mov r0,r0, lsr #0x4 */
            r[0]=(r[0]>>4);r[15]=0x59e0u;return 0;
        case 0x59e0u: /* and r0,r0,#0xf */
            r[0]=r[0]&15u;r[15]=0x59e4u;return 0;
        case 0x59e4u: /* ldr r1,[0x60f8] */
            LOAD(r[1],24824u,4);r[15]=0x59e8u;return 0;
        case 0x59e8u: /* strb r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],1);r[15]=0x59ecu;return 0;
        case 0x59ecu: /* ldr r1,[0x60fc] */
            LOAD(r[1],24828u,4);r[15]=0x59f0u;return 0;
        case 0x59f0u: /* ldr r1,[r1,#0x0] */
            LOAD(r[1],r[1]+0u,4);r[15]=0x59f4u;return 0;
        case 0x59f4u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x59f8u;return 0;
        case 0x59f8u: /* beq 0x00005a04 */
            if(state->cpsr&0x40000000u){r[15]=23044u;}else{r[15]=0x59fcu;}return 0;
        case 0x59fcu: /* cmp r0,#0xd */
            (void)subtract(state,r[0],13u);r[15]=0x5a00u;return 0;
        case 0x5a00u: /* movhi r0,#0xd */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[0]=13u;}r[15]=0x5a04u;return 0;
        case 0x5a04u: /* ldr r1,[0x6100] */
            LOAD(r[1],24832u,4);r[15]=0x5a08u;return 0;
        case 0x5a08u: /* strb r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],1);r[15]=0x5a0cu;return 0;
        case 0x5a0cu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5a10u;return 0;
        case 0x5a10u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5a14u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5a18u;return 0;
        case 0x5a18u: /* ldr r3,[0x6104] */
            LOAD(r[3],24836u,4);r[15]=0x5a1cu;return 0;
        case 0x5a1cu: /* ldr r1,[r3,#0x0] */
            LOAD(r[1],r[3]+0u,4);r[15]=0x5a20u;return 0;
        case 0x5a20u: /* ldr r3,[r1,#0x0] */
            LOAD(r[3],r[1]+0u,4);r[15]=0x5a24u;return 0;
        case 0x5a24u: /* ldr r4,[0x5b58] */
            LOAD(r[4],23384u,4);r[15]=0x5a28u;return 0;
        case 0x5a28u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x5a2cu;return 0;
        case 0x5a2cu: /* movne r1,#0x2 */
            if(!(state->cpsr&0x40000000u)){r[1]=2u;}r[15]=0x5a30u;return 0;
        case 0x5a30u: /* bne 0x00005b44 */
            if(!(state->cpsr&0x40000000u)){r[15]=23364u;}else{r[15]=0x5a34u;}return 0;
        case 0x5a34u: /* ldrb r3,[r1,#0x4] */
            LOAD(r[3],r[1]+4u,1);r[15]=0x5a38u;return 0;
        case 0x5a38u: /* cmp r3,#0x1 */
            (void)subtract(state,r[3],1u);r[15]=0x5a3cu;return 0;
        case 0x5a3cu: /* movne r1,#0x8 */
            if(!(state->cpsr&0x40000000u)){r[1]=8u;}r[15]=0x5a40u;return 0;
        case 0x5a40u: /* bne 0x00005b44 */
            if(!(state->cpsr&0x40000000u)){r[15]=23364u;}else{r[15]=0x5a44u;}return 0;
        case 0x5a44u: /* ldr r2,[r1,#0x8] */
            LOAD(r[2],r[1]+8u,4);r[15]=0x5a48u;return 0;
        case 0x5a48u: /* sub r2,r2,#0x4 */
            r[2]=r[2]-4u;r[15]=0x5a4cu;return 0;
        case 0x5a4cu: /* ldr r3,[r1,r2] */
            LOAD(r[3],r[1]+r[2],4);r[15]=0x5a50u;return 0;
        case 0x5a50u: /* ldr r4,[0x5b5c] */
            LOAD(r[4],23388u,4);r[15]=0x5a54u;return 0;
        case 0x5a54u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x5a58u;return 0;
        case 0x5a58u: /* movne r1,#0x4 */
            if(!(state->cpsr&0x40000000u)){r[1]=4u;}r[15]=0x5a5cu;return 0;
        case 0x5a5cu: /* bne 0x00005b44 */
            if(!(state->cpsr&0x40000000u)){r[15]=23364u;}else{r[15]=0x5a60u;}return 0;
        case 0x5a60u: /* ldrb r2,[r1,#0xc] */
            LOAD(r[2],r[1]+12u,1);r[15]=0x5a64u;return 0;
        case 0x5a64u: /* cmp r0,r2 */
            (void)subtract(state,r[0],r[2]);r[15]=0x5a68u;return 0;
        case 0x5a68u: /* movcs r1,#0x10 */
            if(state->cpsr&0x20000000u){r[1]=16u;}r[15]=0x5a6cu;return 0;
        case 0x5a6cu: /* bcs 0x00005b44 */
            if(state->cpsr&0x20000000u){r[15]=23364u;}else{r[15]=0x5a70u;}return 0;
        case 0x5a70u: /* ldr r2,[0x6108] */
            LOAD(r[2],24840u,4);r[15]=0x5a74u;return 0;
        case 0x5a74u: /* strb r0,[r2,#0x0] */
            STORE(r[2]+0u,r[0],1);r[15]=0x5a78u;return 0;
        case 0x5a78u: /* ldrb r2,[r1,#0x10] */
            LOAD(r[2],r[1]+16u,1);r[15]=0x5a7cu;return 0;
        case 0x5a7cu: /* add r2,r1,r2 */
            r[2]=r[1]+r[2];r[15]=0x5a80u;return 0;
        case 0x5a80u: /* ldr r1,[0x610c] */
            LOAD(r[1],24844u,4);r[15]=0x5a84u;return 0;
        case 0x5a84u: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x5a88u;return 0;
        case 0x5a88u: /* mov r3,#0x20 */
            r[3]=32u;r[15]=0x5a8cu;return 0;
        case 0x5a8cu: /* mla r0,r3,r0,r2 */
            r[0]=r[3]*r[0]+r[2];r[15]=0x5a90u;return 0;
        case 0x5a90u: /* ldr r2,[0x60f4] */
            LOAD(r[2],24820u,4);r[15]=0x5a94u;return 0;
        case 0x5a94u: /* mov r4,#0x10 */
            r[4]=16u;r[15]=0x5a98u;return 0;
        case 0x5a98u: /* ldr r5,[0x6110] */
            LOAD(r[5],24848u,4);r[15]=0x5a9cu;return 0;
        case 0x5a9cu: /* ldrb r3,[r0],#0x1 */
            LOAD(r[3],r[0],1);r[0]+=1u;r[15]=0x5aa0u;return 0;
        case 0x5aa0u: /* and r3,r3,#0xf */
            r[3]=r[3]&15u;r[15]=0x5aa4u;return 0;
        case 0x5aa4u: /* ldrb r6,[r5],#0x1 */
            LOAD(r[6],r[5],1);r[5]+=1u;r[15]=0x5aa8u;return 0;
        case 0x5aa8u: /* tst r6,#0x80 */
            temporary=r[6]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5aacu;return 0;
        case 0x5aacu: /* orrne r6,r6,#0xff00 */
            if(!(state->cpsr&0x40000000u)){r[6]=r[6]|65280u;}r[15]=0x5ab0u;return 0;
        case 0x5ab0u: /* orrne r6,r6,#0xff0000 */
            if(!(state->cpsr&0x40000000u)){r[6]=r[6]|16711680u;}r[15]=0x5ab4u;return 0;
        case 0x5ab4u: /* orrne r6,r6,#0xff000000 */
            if(!(state->cpsr&0x40000000u)){r[6]=r[6]|4278190080u;}r[15]=0x5ab8u;return 0;
        case 0x5ab8u: /* adds r6,r6,r3, lsl #0x3 */
            r[6]=addition(state,r[6],(r[3]<<3));r[15]=0x5abcu;return 0;
        case 0x5abcu: /* movmi r6,#0x0 */
            if(state->cpsr&0x80000000u){r[6]=0u;}r[15]=0x5ac0u;return 0;
        case 0x5ac0u: /* tst r6,#0x80 */
            temporary=r[6]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5ac4u;return 0;
        case 0x5ac4u: /* movne r6,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[6]=127u;}r[15]=0x5ac8u;return 0;
        case 0x5ac8u: /* mov r6,r6, lsl #0x5 */
            r[6]=(r[6]<<5);r[15]=0x5accu;return 0;
        case 0x5accu: /* and r6,r6,#0xf00 */
            r[6]=r[6]&3840u;r[15]=0x5ad0u;return 0;
        case 0x5ad0u: /* ldrb r3,[r0],#0x1 */
            LOAD(r[3],r[0],1);r[0]+=1u;r[15]=0x5ad4u;return 0;
        case 0x5ad4u: /* cmp r3,#0x10 */
            (void)subtract(state,r[3],16u);r[15]=0x5ad8u;return 0;
        case 0x5ad8u: /* moveq r3,#0x0 */
            if(state->cpsr&0x40000000u){r[3]=0u;}r[15]=0x5adcu;return 0;
        case 0x5adcu: /* tst r3,#0x10 */
            temporary=r[3]&16u;logic_flags(state,temporary,0,16u);r[15]=0x5ae0u;return 0;
        case 0x5ae0u: /* subne r3,r3,#0x10 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[3]-16u;}r[15]=0x5ae4u;return 0;
        case 0x5ae4u: /* eorne r3,r3,#0xff */
            if(!(state->cpsr&0x40000000u)){r[3]=r[3]^255u;}r[15]=0x5ae8u;return 0;
        case 0x5ae8u: /* add r3,r3,#0x10 */
            r[3]=r[3]+16u;r[15]=0x5aecu;return 0;
        case 0x5aecu: /* and r3,r3,#0x1f */
            r[3]=r[3]&31u;r[15]=0x5af0u;return 0;
        case 0x5af0u: /* ldrb r7,[r5],#0x1 */
            LOAD(r[7],r[5],1);r[5]+=1u;r[15]=0x5af4u;return 0;
        case 0x5af4u: /* tst r7,#0x80 */
            temporary=r[7]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5af8u;return 0;
        case 0x5af8u: /* orrne r7,r7,#0xff00 */
            if(!(state->cpsr&0x40000000u)){r[7]=r[7]|65280u;}r[15]=0x5afcu;return 0;
        case 0x5afcu: /* orrne r7,r7,#0xff0000 */
            if(!(state->cpsr&0x40000000u)){r[7]=r[7]|16711680u;}r[15]=0x5b00u;return 0;
        case 0x5b00u: /* orrne r7,r7,#0xff000000 */
            if(!(state->cpsr&0x40000000u)){r[7]=r[7]|4278190080u;}r[15]=0x5b04u;return 0;
        case 0x5b04u: /* adds r7,r7,r3, lsl #0x2 */
            r[7]=addition(state,r[7],(r[3]<<2));r[15]=0x5b08u;return 0;
        case 0x5b08u: /* movmi r7,#0x0 */
            if(state->cpsr&0x80000000u){r[7]=0u;}r[15]=0x5b0cu;return 0;
        case 0x5b0cu: /* tst r7,#0x80 */
            temporary=r[7]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5b10u;return 0;
        case 0x5b10u: /* movne r7,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[7]=127u;}r[15]=0x5b14u;return 0;
        case 0x5b14u: /* mov r7,r7, lsr #0x2 */
            r[7]=(r[7]>>2);r[15]=0x5b18u;return 0;
        case 0x5b18u: /* and r7,r7,#0x1f */
            r[7]=r[7]&31u;r[15]=0x5b1cu;return 0;
        case 0x5b1cu: /* subs r7,r7,#0x10 */
            r[7]=subtract(state,r[7],16u);r[15]=0x5b20u;return 0;
        case 0x5b20u: /* eormi r7,r7,#0xff */
            if(state->cpsr&0x80000000u){r[7]=r[7]^255u;}r[15]=0x5b24u;return 0;
        case 0x5b24u: /* addmi r7,r7,#0x10 */
            if(state->cpsr&0x80000000u){r[7]=r[7]+16u;}r[15]=0x5b28u;return 0;
        case 0x5b28u: /* and r7,r7,#0x1f */
            r[7]=r[7]&31u;r[15]=0x5b2cu;return 0;
        case 0x5b2cu: /* orr r3,r7,r6 */
            r[3]=r[7]|r[6];r[15]=0x5b30u;return 0;
        case 0x5b30u: /* str r3,[r2],#0x4 */
            STORE(r[2],r[3],4);r[2]+=4u;r[15]=0x5b34u;return 0;
        case 0x5b34u: /* subs r4,r4,#0x1 */
            r[4]=subtract(state,r[4],1u);r[15]=0x5b38u;return 0;
        case 0x5b38u: /* bne 0x00005a9c */
            if(!(state->cpsr&0x40000000u)){r[15]=23196u;}else{r[15]=0x5b3cu;}return 0;
        case 0x5b3cu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5b40u;return 0;
        case 0x5b40u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5b44u: /* ldr r2,[0x6114] */
            LOAD(r[2],24852u,4);r[15]=0x5b48u;return 0;
        case 0x5b48u: /* ldrb r0,[r2,#0x0] */
            LOAD(r[0],r[2]+0u,1);r[15]=0x5b4cu;return 0;
        case 0x5b4cu: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0x5b50u;return 0;
        case 0x5b50u: /* strb r0,[r2,#0x0] */
            STORE(r[2]+0u,r[0],1);r[15]=0x5b54u;return 0;
        case 0x5b54u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5b60u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5b64u;return 0;
        case 0x5b64u: /* and r0,r0,#0xf */
            r[0]=r[0]&15u;r[15]=0x5b68u;return 0;
        case 0x5b68u: /* ldr r2,[0x60f4] */
            LOAD(r[2],24820u,4);r[15]=0x5b6cu;return 0;
        case 0x5b6cu: /* add r2,r2,r0, lsl #0x2 */
            r[2]=r[2]+(r[0]<<2);r[15]=0x5b70u;return 0;
        case 0x5b70u: /* ldr r3,[0x6110] */
            LOAD(r[3],24848u,4);r[15]=0x5b74u;return 0;
        case 0x5b74u: /* add r3,r3,r0, lsl #0x1 */
            r[3]=r[3]+(r[0]<<1);r[15]=0x5b78u;return 0;
        case 0x5b78u: /* ldr r4,[0x610c] */
            LOAD(r[4],24844u,4);r[15]=0x5b7cu;return 0;
        case 0x5b7cu: /* ldr r4,[r4,#0x0] */
            LOAD(r[4],r[4]+0u,4);r[15]=0x5b80u;return 0;
        case 0x5b80u: /* movs r4,r4 */
            r[4]=r[4];logic_flags(state,r[4],0,0);r[15]=0x5b84u;return 0;
        case 0x5b84u: /* beq 0x00005c18 */
            if(state->cpsr&0x40000000u){r[15]=23576u;}else{r[15]=0x5b88u;}return 0;
        case 0x5b88u: /* add r4,r4,r0, lsl #0x1 */
            r[4]=r[4]+(r[0]<<1);r[15]=0x5b8cu;return 0;
        case 0x5b8cu: /* ldrb r0,[r10,#0x3] */
            LOAD(r[0],r[10]+3u,1);r[15]=0x5b90u;return 0;
        case 0x5b90u: /* sub r0,r0,#0x80 */
            r[0]=r[0]-128u;r[15]=0x5b94u;return 0;
        case 0x5b94u: /* strb r0,[r3],#0x1 */
            STORE(r[3],r[0],1);r[3]+=1u;r[15]=0x5b98u;return 0;
        case 0x5b98u: /* ldrb r5,[r4],#0x1 */
            LOAD(r[5],r[4],1);r[4]+=1u;r[15]=0x5b9cu;return 0;
        case 0x5b9cu: /* and r5,r5,#0xf */
            r[5]=r[5]&15u;r[15]=0x5ba0u;return 0;
        case 0x5ba0u: /* adds r0,r0,r5, lsl #0x3 */
            r[0]=addition(state,r[0],(r[5]<<3));r[15]=0x5ba4u;return 0;
        case 0x5ba4u: /* movmi r0,#0x0 */
            if(state->cpsr&0x80000000u){r[0]=0u;}r[15]=0x5ba8u;return 0;
        case 0x5ba8u: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5bacu;return 0;
        case 0x5bacu: /* movne r0,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[0]=127u;}r[15]=0x5bb0u;return 0;
        case 0x5bb0u: /* mov r0,r0, lsl #0x5 */
            r[0]=(r[0]<<5);r[15]=0x5bb4u;return 0;
        case 0x5bb4u: /* and r0,r0,#0xf00 */
            r[0]=r[0]&3840u;r[15]=0x5bb8u;return 0;
        case 0x5bb8u: /* ldrb r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,1);r[15]=0x5bbcu;return 0;
        case 0x5bbcu: /* sub r1,r1,#0x80 */
            r[1]=r[1]-128u;r[15]=0x5bc0u;return 0;
        case 0x5bc0u: /* strb r1,[r3,#0x0] */
            STORE(r[3]+0u,r[1],1);r[15]=0x5bc4u;return 0;
        case 0x5bc4u: /* ldrb r5,[r4,#0x0] */
            LOAD(r[5],r[4]+0u,1);r[15]=0x5bc8u;return 0;
        case 0x5bc8u: /* cmp r5,#0x10 */
            (void)subtract(state,r[5],16u);r[15]=0x5bccu;return 0;
        case 0x5bccu: /* moveq r5,#0x0 */
            if(state->cpsr&0x40000000u){r[5]=0u;}r[15]=0x5bd0u;return 0;
        case 0x5bd0u: /* tst r5,#0x10 */
            temporary=r[5]&16u;logic_flags(state,temporary,0,16u);r[15]=0x5bd4u;return 0;
        case 0x5bd4u: /* subne r5,r5,#0x10 */
            if(!(state->cpsr&0x40000000u)){r[5]=r[5]-16u;}r[15]=0x5bd8u;return 0;
        case 0x5bd8u: /* eorne r5,r5,#0xff */
            if(!(state->cpsr&0x40000000u)){r[5]=r[5]^255u;}r[15]=0x5bdcu;return 0;
        case 0x5bdcu: /* add r5,r5,#0x10 */
            r[5]=r[5]+16u;r[15]=0x5be0u;return 0;
        case 0x5be0u: /* and r5,r5,#0x1f */
            r[5]=r[5]&31u;r[15]=0x5be4u;return 0;
        case 0x5be4u: /* adds r1,r1,r5, lsl #0x2 */
            r[1]=addition(state,r[1],(r[5]<<2));r[15]=0x5be8u;return 0;
        case 0x5be8u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x5becu;return 0;
        case 0x5becu: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5bf0u;return 0;
        case 0x5bf0u: /* movne r1,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[1]=127u;}r[15]=0x5bf4u;return 0;
        case 0x5bf4u: /* mov r1,r1, lsr #0x2 */
            r[1]=(r[1]>>2);r[15]=0x5bf8u;return 0;
        case 0x5bf8u: /* subs r1,r1,#0x10 */
            r[1]=subtract(state,r[1],16u);r[15]=0x5bfcu;return 0;
        case 0x5bfcu: /* eormi r1,r1,#0xff */
            if(state->cpsr&0x80000000u){r[1]=r[1]^255u;}r[15]=0x5c00u;return 0;
        case 0x5c00u: /* addmi r1,r1,#0x10 */
            if(state->cpsr&0x80000000u){r[1]=r[1]+16u;}r[15]=0x5c04u;return 0;
        case 0x5c04u: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0x5c08u;return 0;
        case 0x5c08u: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0x5c0cu;return 0;
        case 0x5c0cu: /* str r0,[r2,#0x0] */
            STORE(r[2]+0u,r[0],4);r[15]=0x5c10u;return 0;
        case 0x5c10u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5c14u;return 0;
        case 0x5c14u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5c18u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5c1cu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5c20u;return 0;
        case 0x5df8u: /* ldr r0,[r10,#0x4] */
            LOAD(r[0],r[10]+4u,4);r[15]=0x5dfcu;return 0;
        case 0x5dfcu: /* mov r1,#0xb000 */
            r[1]=45056u;r[15]=0x5e00u;return 0;
        case 0x5e00u: /* mov r3,#0x13000 */
            r[3]=77824u;r[15]=0x5e04u;return 0;
        case 0x5e04u: /* ldr r2,[r3,#0x0] */
            LOAD(r[2],r[3]+0u,4);r[15]=0x5e08u;return 0;
        case 0x5e08u: /* str r0,[r1,r2] */
            STORE(r[1]+r[2],r[0],4);r[15]=0x5e0cu;return 0;
        case 0x5e0cu: /* add r2,r2,#0x4 */
            r[2]=r[2]+4u;r[15]=0x5e10u;return 0;
        case 0x5e10u: /* ldr r1,[0x5e24] */
            LOAD(r[1],24100u,4);r[15]=0x5e14u;return 0;
        case 0x5e14u: /* and r2,r2,r1 */
            r[2]=r[2]&r[1];r[15]=0x5e18u;return 0;
        case 0x5e18u: /* str r2,[r3,#0x0] */
            STORE(r[3]+0u,r[2],4);r[15]=0x5e1cu;return 0;
        case 0x5e1cu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5e20u;return 0;
        case 0x5e20u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5e28u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5e2cu;return 0;
        case 0x5e2cu: /* and r0,r0,#0x1 */
            r[0]=r[0]&1u;r[15]=0x5e30u;return 0;
        case 0x5e30u: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x5e34u;return 0;
        case 0x5e34u: /* adr r1,0x5e40 */
            r[1]=24128u;r[15]=0x5e38u;return 0;
        case 0x5e38u: /* add r0,r0,r1 */
            r[0]=r[0]+r[1];r[15]=0x5e3cu;return 0;
        case 0x5e3cu: /* mov pc,r0 */
            r[15]=r[0];return 0;
        case 0x5e40u: /* b 0x00005e48 */
            r[15]=24136u;return 0;
        case 0x5e44u: /* b 0x00005e68 */
            r[15]=24168u;return 0;
        case 0x5e48u: /* ldrb r0,[r10,#0x3] */
            LOAD(r[0],r[10]+3u,1);r[15]=0x5e4cu;return 0;
        case 0x5e4cu: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x5e50u;return 0;
        case 0x5e50u: /* mov r1,#0x480 */
            r[1]=1152u;r[15]=0x5e54u;return 0;
        case 0x5e54u: /* ldr r2,[0x6144] */
            LOAD(r[2],24900u,4);r[15]=0x5e58u;return 0;
        case 0x5e58u: /* mla r0,r1,r0,r2 */
            r[0]=r[1]*r[0]+r[2];r[15]=0x5e5cu;return 0;
        case 0x5e5cu: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x5e60u;return 0;
        case 0x5e60u: /* strb r1,[r0,#0x3c] */
            STORE(r[0]+60u,r[1],1);r[15]=0x5e64u;return 0;
        case 0x5e64u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5e68u: /* ldrb r0,[r10,#0x3] */
            LOAD(r[0],r[10]+3u,1);r[15]=0x5e6cu;return 0;
        case 0x5e6cu: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x5e70u;return 0;
        case 0x5e70u: /* mov r1,#0x480 */
            r[1]=1152u;r[15]=0x5e74u;return 0;
        case 0x5e74u: /* ldr r2,[0x6144] */
            LOAD(r[2],24900u,4);r[15]=0x5e78u;return 0;
        case 0x5e78u: /* mla r0,r1,r0,r2 */
            r[0]=r[1]*r[0]+r[2];r[15]=0x5e7cu;return 0;
        case 0x5e7cu: /* mov r1,#0x1 */
            r[1]=1u;r[15]=0x5e80u;return 0;
        case 0x5e80u: /* strb r1,[r0,#0x3c] */
            STORE(r[0]+60u,r[1],1);r[15]=0x5e84u;return 0;
        case 0x5e84u: /* ldr r1,[0x6148] */
            LOAD(r[1],24904u,4);r[15]=0x5e88u;return 0;
        case 0x5e88u: /* ldr r2,[r1,#0x0] */
            LOAD(r[2],r[1]+0u,4);r[15]=0x5e8cu;return 0;
        case 0x5e8cu: /* movs r2,r2 */
            r[2]=r[2];logic_flags(state,r[2],0,0);r[15]=0x5e90u;return 0;
        case 0x5e90u: /* beq 0x00005ed0 */
            if(state->cpsr&0x40000000u){r[15]=24272u;}else{r[15]=0x5e94u;}return 0;
        case 0x5e94u: /* ldr r3,[r1,#0x4] */
            LOAD(r[3],r[1]+4u,4);r[15]=0x5e98u;return 0;
        case 0x5e98u: /* movs r3,r3 */
            r[3]=r[3];logic_flags(state,r[3],0,0);r[15]=0x5e9cu;return 0;
        case 0x5e9cu: /* beq 0x00005ed0 */
            if(state->cpsr&0x40000000u){r[15]=24272u;}else{r[15]=0x5ea0u;}return 0;
        case 0x5ea0u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x5ea4u;return 0;
        case 0x5ea4u: /* strb r1,[r0,#0x1] */
            STORE(r[0]+1u,r[1],1);r[15]=0x5ea8u;return 0;
        case 0x5ea8u: /* str r2,[r0,#0x10] */
            STORE(r[0]+16u,r[2],4);r[15]=0x5eacu;return 0;
        case 0x5eacu: /* ldr r1,[r2,#0x10] */
            LOAD(r[1],r[2]+16u,4);r[15]=0x5eb0u;return 0;
        case 0x5eb0u: /* add r1,r1,r2 */
            r[1]=r[1]+r[2];r[15]=0x5eb4u;return 0;
        case 0x5eb4u: /* ldr r1,[r1,#0x0] */
            LOAD(r[1],r[1]+0u,4);r[15]=0x5eb8u;return 0;
        case 0x5eb8u: /* add r1,r1,r2 */
            r[1]=r[1]+r[2];r[15]=0x5ebcu;return 0;
        case 0x5ebcu: /* str r1,[r0,#0x14] */
            STORE(r[0]+20u,r[1],4);r[15]=0x5ec0u;return 0;
        case 0x5ec0u: /* ldr r1,[r2,#0x18] */
            LOAD(r[1],r[2]+24u,4);r[15]=0x5ec4u;return 0;
        case 0x5ec4u: /* add r1,r1,r2 */
            r[1]=r[1]+r[2];r[15]=0x5ec8u;return 0;
        case 0x5ec8u: /* str r1,[r0,#0x18] */
            STORE(r[0]+24u,r[1],4);r[15]=0x5eccu;return 0;
        case 0x5eccu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5ed0u: /* ldr r1,[0x614c] */
            LOAD(r[1],24908u,4);r[15]=0x5ed4u;return 0;
        case 0x5ed4u: /* str r1,[r0,#0x10] */
            STORE(r[0]+16u,r[1],4);r[15]=0x5ed8u;return 0;
        case 0x5ed8u: /* b 0x00005eac */
            r[15]=24236u;return 0;
        case 0x5edcu: /* ldr r0,[0x6150] */
            LOAD(r[0],24912u,4);r[15]=0x5ee0u;return 0;
        case 0x5ee0u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x5ee4u;return 0;
        case 0x5ee4u: /* tst r1,#0x30 */
            temporary=r[1]&48u;logic_flags(state,temporary,0,48u);r[15]=0x5ee8u;return 0;
        case 0x5ee8u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x5eecu;}return 0;
        case 0x5eecu: /* tst r1,#0x20 */
            temporary=r[1]&32u;logic_flags(state,temporary,0,32u);r[15]=0x5ef0u;return 0;
        case 0x5ef0u: /* bne 0x00005f5c */
            if(!(state->cpsr&0x40000000u)){r[15]=24412u;}else{r[15]=0x5ef4u;}return 0;
        case 0x5ef4u: /* mov r3,#0x8 */
            r[3]=8u;r[15]=0x5ef8u;return 0;
        case 0x5ef8u: /* add r9,r10,#0x2 */
            r[9]=r[10]+2u;r[15]=0x5efcu;return 0;
        case 0x5efcu: /* ldr r2,[0x6154] */
            LOAD(r[2],24916u,4);r[15]=0x5f00u;return 0;
        case 0x5f00u: /* ldrb r2,[r2,#0x0] */
            LOAD(r[2],r[2]+0u,1);r[15]=0x5f04u;return 0;
        case 0x5f04u: /* ldr r1,[0x6134] */
            LOAD(r[1],24884u,4);r[15]=0x5f08u;return 0;
        case 0x5f08u: /* add r1,r1,r2, lsl #0x2 */
            r[1]=r[1]+(r[2]<<2);r[15]=0x5f0cu;return 0;
        case 0x5f0cu: /* ldr r2,[0x5fc4] */
            LOAD(r[2],24516u,4);r[15]=0x5f10u;return 0;
        case 0x5f10u: /* ldrb r0,[r9],#0x1 */
            LOAD(r[0],r[9],1);r[9]+=1u;r[15]=0x5f14u;return 0;
        case 0x5f14u: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5f18u;return 0;
        case 0x5f18u: /* beq 0x00005f4c */
            if(state->cpsr&0x40000000u){r[15]=24396u;}else{r[15]=0x5f1cu;}return 0;
        case 0x5f1cu: /* and r0,r0,#0x7c */
            r[0]=r[0]&124u;r[15]=0x5f20u;return 0;
        case 0x5f20u: /* add r0,r2,r0, lsl #0x2 */
            r[0]=r[2]+(r[0]<<2);r[15]=0x5f24u;return 0;
        case 0x5f24u: /* mov r5,r1 */
            r[5]=r[1];r[15]=0x5f28u;return 0;
        case 0x5f28u: /* ldr r2,[r0,#0x0] */
            LOAD(r[2],r[0]+0u,4);r[15]=0x5f2cu;return 0;
        case 0x5f2cu: /* str r2,[r5,#0x0] */
            STORE(r[5]+0u,r[2],4);r[15]=0x5f30u;return 0;
        case 0x5f30u: /* ldr r2,[r0,#0x4] */
            LOAD(r[2],r[0]+4u,4);r[15]=0x5f34u;return 0;
        case 0x5f34u: /* str r2,[r5,#0x20] */
            STORE(r[5]+32u,r[2],4);r[15]=0x5f38u;return 0;
        case 0x5f38u: /* ldr r2,[r0,#0x8] */
            LOAD(r[2],r[0]+8u,4);r[15]=0x5f3cu;return 0;
        case 0x5f3cu: /* str r2,[r5,#0x40] */
            STORE(r[5]+64u,r[2],4);r[15]=0x5f40u;return 0;
        case 0x5f40u: /* ldr r2,[r0,#0xc] */
            LOAD(r[2],r[0]+12u,4);r[15]=0x5f44u;return 0;
        case 0x5f44u: /* str r2,[r5,#0x60] */
            STORE(r[5]+96u,r[2],4);r[15]=0x5f48u;return 0;
        case 0x5f48u: /* mov r1,r5 */
            r[1]=r[5];r[15]=0x5f4cu;return 0;
        case 0x5f4cu: /* add r1,r1,#0x4 */
            r[1]=r[1]+4u;r[15]=0x5f50u;return 0;
        case 0x5f50u: /* subs r3,r3,#0x1 */
            r[3]=subtract(state,r[3],1u);r[15]=0x5f54u;return 0;
        case 0x5f54u: /* bne 0x00005f0c */
            if(!(state->cpsr&0x40000000u)){r[15]=24332u;}else{r[15]=0x5f58u;}return 0;
        case 0x5f58u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5f5cu: /* mov r3,#0x4 */
            r[3]=4u;r[15]=0x5f60u;return 0;
        case 0x5f60u: /* add r9,r10,#0x2 */
            r[9]=r[10]+2u;r[15]=0x5f64u;return 0;
        case 0x5f64u: /* ldr r2,[0x6154] */
            LOAD(r[2],24916u,4);r[15]=0x5f68u;return 0;
        case 0x5f68u: /* ldrb r2,[r2,#0x0] */
            LOAD(r[2],r[2]+0u,1);r[15]=0x5f6cu;return 0;
        case 0x5f6cu: /* ldr r1,[0x6134] */
            LOAD(r[1],24884u,4);r[15]=0x5f70u;return 0;
        case 0x5f70u: /* add r1,r1,r2, lsl #0x2 */
            r[1]=r[1]+(r[2]<<2);r[15]=0x5f74u;return 0;
        case 0x5f74u: /* ldr r2,[0x5fc4] */
            LOAD(r[2],24516u,4);r[15]=0x5f78u;return 0;
        case 0x5f78u: /* ldrb r0,[r9],#0x1 */
            LOAD(r[0],r[9],1);r[9]+=1u;r[15]=0x5f7cu;return 0;
        case 0x5f7cu: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5f80u;return 0;
        case 0x5f80u: /* beq 0x00005fb4 */
            if(state->cpsr&0x40000000u){r[15]=24500u;}else{r[15]=0x5f84u;}return 0;
        case 0x5f84u: /* and r0,r0,#0x7c */
            r[0]=r[0]&124u;r[15]=0x5f88u;return 0;
        case 0x5f88u: /* add r0,r2,r0, lsl #0x2 */
            r[0]=r[2]+(r[0]<<2);r[15]=0x5f8cu;return 0;
        case 0x5f8cu: /* mov r5,r1 */
            r[5]=r[1];r[15]=0x5f90u;return 0;
        case 0x5f90u: /* ldr r2,[r0,#0x0] */
            LOAD(r[2],r[0]+0u,4);r[15]=0x5f94u;return 0;
        case 0x5f94u: /* str r2,[r5,#0x0] */
            STORE(r[5]+0u,r[2],4);r[15]=0x5f98u;return 0;
        case 0x5f98u: /* ldr r2,[r0,#0x4] */
            LOAD(r[2],r[0]+4u,4);r[15]=0x5f9cu;return 0;
        case 0x5f9cu: /* str r2,[r5,#0x10] */
            STORE(r[5]+16u,r[2],4);r[15]=0x5fa0u;return 0;
        case 0x5fa0u: /* ldr r2,[r0,#0x8] */
            LOAD(r[2],r[0]+8u,4);r[15]=0x5fa4u;return 0;
        case 0x5fa4u: /* str r2,[r5,#0x20] */
            STORE(r[5]+32u,r[2],4);r[15]=0x5fa8u;return 0;
        case 0x5fa8u: /* ldr r2,[r0,#0xc] */
            LOAD(r[2],r[0]+12u,4);r[15]=0x5facu;return 0;
        case 0x5facu: /* str r2,[r5,#0x30] */
            STORE(r[5]+48u,r[2],4);r[15]=0x5fb0u;return 0;
        case 0x5fb0u: /* mov r1,r5 */
            r[1]=r[5];r[15]=0x5fb4u;return 0;
        case 0x5fb4u: /* add r1,r1,#0x4 */
            r[1]=r[1]+4u;r[15]=0x5fb8u;return 0;
        case 0x5fb8u: /* subs r3,r3,#0x1 */
            r[3]=subtract(state,r[3],1u);r[15]=0x5fbcu;return 0;
        case 0x5fbcu: /* bne 0x00005f74 */
            if(!(state->cpsr&0x40000000u)){r[15]=24436u;}else{r[15]=0x5fc0u;}return 0;
        case 0x5fc0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5fc8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5fccu: /* ldr r1,[0x60f8] */
            LOAD(r[1],24824u,4);r[15]=0x5fd0u;return 0;
        case 0x5fd0u: /* ldrb r2,[r1,#0x0] */
            LOAD(r[2],r[1]+0u,1);r[15]=0x5fd4u;return 0;
        case 0x5fd4u: /* ldr r1,[0x60fc] */
            LOAD(r[1],24828u,4);r[15]=0x5fd8u;return 0;
        case 0x5fd8u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5fdcu;return 0;
        case 0x5fdcu: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x5fe0u;return 0;
        case 0x5fe0u: /* bne 0x00006000 */
            if(!(state->cpsr&0x40000000u)){r[15]=24576u;}else{r[15]=0x5fe4u;}return 0;
        case 0x5fe4u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5fe8u;return 0;
        case 0x5fe8u: /* str r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],4);r[15]=0x5fecu;return 0;
        case 0x5fecu: /* orr r0,r0,r2 */
            r[0]=r[0]|r[2];r[15]=0x5ff0u;return 0;
        case 0x5ff0u: /* ldr r8,[0x6100] */
            LOAD(r[8],24832u,4);r[15]=0x5ff4u;return 0;
        case 0x5ff4u: /* str r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],4);r[15]=0x5ff8u;return 0;
        case 0x5ff8u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5ffcu;return 0;
        case 0x5ffcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6000u: /* mvn r0,#0x0 */
            r[0]=~0u;r[15]=0x6004u;return 0;
        case 0x6004u: /* str r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],4);r[15]=0x6008u;return 0;
        case 0x6008u: /* cmp r2,#0xd */
            (void)subtract(state,r[2],13u);r[15]=0x600cu;return 0;
        case 0x600cu: /* movhi r2,#0xd */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[2]=13u;}r[15]=0x6010u;return 0;
        case 0x6010u: /* mov r0,#0x8000 */
            r[0]=32768u;r[15]=0x6014u;return 0;
        case 0x6014u: /* b 0x00005fec */
            r[15]=24556u;return 0;
        case 0x601cu: /* mrs r10,cpsr */
            r[10]=state->cpsr;r[15]=0x6020u;return 0;
        case 0x6020u: /* orr r10,r10,#0x40 */
            r[10]=r[10]|64u;r[15]=0x6024u;return 0;
        case 0x6024u: /* msr cpsr_cf,r10 */
            state->cpsr=(state->cpsr&0x00ffff00u)|(r[10]&0xff0000ffu);r[15]=0x6028u;return 0;
        case 0x6028u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x602cu;return 0;
        case 0x602cu: /* bl 0x00000650 */
            r[14]=0x6030u;r[15]=1616u;return 0;
        case 0x6030u: /* bl 0x0000043c */
            r[14]=0x6034u;r[15]=1084u;return 0;
        case 0x6034u: /* bl 0x00000464 */
            r[14]=0x6038u;r[15]=1124u;return 0;
        case 0x6038u: /* bl 0x000004e8 */
            r[14]=0x603cu;r[15]=1256u;return 0;
        case 0x603cu: /* bl 0x00000504 */
            r[14]=0x6040u;r[15]=1284u;return 0;
        case 0x6040u: /* bl 0x00000610 */
            r[14]=0x6044u;r[15]=1552u;return 0;
        case 0x6044u: /* bl 0x000073b4 */
            r[14]=0x6048u;r[15]=29620u;return 0;
        case 0x6048u: /* bl 0x00005d54 */
            r[14]=0x604cu;r[15]=23892u;return 0;
        case 0x604cu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x6050u;return 0;
        case 0x6050u: /* mrs r10,cpsr */
            r[10]=state->cpsr;r[15]=0x6054u;return 0;
        case 0x6054u: /* bic r10,r10,#0x40 */
            r[10]=r[10]&~64u;r[15]=0x6058u;return 0;
        case 0x6058u: /* msr cpsr_cf,r10 */
            state->cpsr=(state->cpsr&0x00ffff00u)|(r[10]&0xff0000ffu);r[15]=0x605cu;return 0;
        case 0x605cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6060u: /* ldrb r0,[r10,#0x4] */
            LOAD(r[0],r[10]+4u,1);r[15]=0x6064u;return 0;
        case 0x6064u: /* cmp r0,#0x53 */
            (void)subtract(state,r[0],83u);r[15]=0x6068u;return 0;
        case 0x6068u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x606cu;}return 0;
        case 0x606cu: /* ldrb r0,[r10,#0x5] */
            LOAD(r[0],r[10]+5u,1);r[15]=0x6070u;return 0;
        case 0x6070u: /* cmp r0,#0x45 */
            (void)subtract(state,r[0],69u);r[15]=0x6074u;return 0;
        case 0x6074u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x6078u;}return 0;
        case 0x6078u: /* ldrb r0,[r10,#0x6] */
            LOAD(r[0],r[10]+6u,1);r[15]=0x607cu;return 0;
        case 0x607cu: /* cmp r0,#0x47 */
            (void)subtract(state,r[0],71u);r[15]=0x6080u;return 0;
        case 0x6080u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x6084u;}return 0;
        case 0x6084u: /* ldrb r0,[r10,#0x7] */
            LOAD(r[0],r[10]+7u,1);r[15]=0x6088u;return 0;
        case 0x6088u: /* cmp r0,#0x41 */
            (void)subtract(state,r[0],65u);r[15]=0x608cu;return 0;
        case 0x608cu: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x6090u;}return 0;
        case 0x6090u: /* mrs r10,cpsr */
            r[10]=state->cpsr;r[15]=0x6094u;return 0;
        case 0x6094u: /* orr r10,r10,#0x40 */
            r[10]=r[10]|64u;r[15]=0x6098u;return 0;
        case 0x6098u: /* msr cpsr_cf,r10 */
            state->cpsr=(state->cpsr&0x00ffff00u)|(r[10]&0xff0000ffu);r[15]=0x609cu;return 0;
        case 0x609cu: /* ldr r0,[0x6158] */
            LOAD(r[0],24920u,4);r[15]=0x60a0u;return 0;
        case 0x60a0u: /* mov r1,#0xff000000 */
            r[1]=4278190080u;r[15]=0x60a4u;return 0;
        case 0x60a4u: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x60a8u;return 0;
        case 0x60a8u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x60acu;return 0;
        case 0x60acu: /* bl 0x00000650 */
            r[14]=0x60b0u;r[15]=1616u;return 0;
        case 0x60b0u: /* bl 0x0000043c */
            r[14]=0x60b4u;r[15]=1084u;return 0;
        case 0x60b4u: /* bl 0x00000464 */
            r[14]=0x60b8u;r[15]=1124u;return 0;
        case 0x60b8u: /* bl 0x000004e8 */
            r[14]=0x60bcu;r[15]=1256u;return 0;
        case 0x60bcu: /* bl 0x00000504 */
            r[14]=0x60c0u;r[15]=1284u;return 0;
        case 0x60c0u: /* bl 0x00000610 */
            r[14]=0x60c4u;r[15]=1552u;return 0;
        case 0x60c4u: /* bl 0x000073b4 */
            r[14]=0x60c8u;r[15]=29620u;return 0;
        case 0x60c8u: /* bl 0x00005d54 */
            r[14]=0x60ccu;r[15]=23892u;return 0;
        case 0x60ccu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x60d0u;return 0;
        case 0x60d0u: /* ldr r0,[0x6158] */
            LOAD(r[0],24920u,4);r[15]=0x60d4u;return 0;
        case 0x60d4u: /* mvn r1,#0x0 */
            r[1]=~0u;r[15]=0x60d8u;return 0;
        case 0x60d8u: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x60dcu;return 0;
        case 0x60dcu: /* mov r0,r0 */
            r[0]=r[0];r[15]=0x60e0u;return 0;
        case 0x60e0u: /* mov r0,r0 */
            r[0]=r[0];r[15]=0x60e4u;return 0;
        case 0x60e4u: /* mov r0,r0 */
            r[0]=r[0];r[15]=0x60e8u;return 0;
        case 0x60e8u: /* mov r0,r0 */
            r[0]=r[0];r[15]=0x60ecu;return 0;
        case 0x60ecu: /* b 0x000060dc */
            r[15]=24796u;return 0;
        case 0x6d84u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6d88u;return 0;
        case 0x6d88u: /* ldr r12,[0x70a0] */
            LOAD(r[12],28832u,4);r[15]=0x6d8cu;return 0;
        case 0x6d8cu: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x6d90u;return 0;
        case 0x6d90u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x6d94u;return 0;
        case 0x6d94u: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x6d98u;return 0;
        case 0x6d98u: /* b 0x00006db8 */
            r[15]=28088u;return 0;
        case 0x6d9cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6da0u;return 0;
        case 0x6da0u: /* ldr r12,[0x7098] */
            LOAD(r[12],28824u,4);r[15]=0x6da4u;return 0;
        case 0x6da4u: /* ldr r7,[0x709c] */
            LOAD(r[7],28828u,4);r[15]=0x6da8u;return 0;
        case 0x6da8u: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x6dacu;return 0;
        case 0x6dacu: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x6db0u;return 0;
        case 0x6db0u: /* tst r0,#0x20 */
            temporary=r[0]&32u;logic_flags(state,temporary,0,32u);r[15]=0x6db4u;return 0;
        case 0x6db4u: /* bleq 0x00006dec */
            if(state->cpsr&0x40000000u){r[14]=0x6db8u;r[15]=28140u;}else{r[15]=0x6db8u;}return 0;
        case 0x6db8u: /* ldrb r7,[r12,#0x0] */
            LOAD(r[7],r[12]+0u,1);r[15]=0x6dbcu;return 0;
        case 0x6dbcu: /* orr r8,r7,#0x4 */
            r[8]=r[7]|4u;r[15]=0x6dc0u;return 0;
        case 0x6dc0u: /* strb r8,[r12,#0x0] */
            STORE(r[12]+0u,r[8],1);r[15]=0x6dc4u;return 0;
        case 0x6dc4u: /* ldrb r7,[r12,#0x1e] */
            LOAD(r[7],r[12]+30u,1);r[15]=0x6dc8u;return 0;
        case 0x6dc8u: /* bl 0x00006e74 */
            r[14]=0x6dccu;r[15]=28276u;return 0;
        case 0x6dccu: /* strb r1,[r12,#0x18] */
            STORE(r[12]+24u,r[1],1);r[15]=0x6dd0u;return 0;
        case 0x6dd0u: /* strb r2,[r12,#0x14] */
            STORE(r[12]+20u,r[2],1);r[15]=0x6dd4u;return 0;
        case 0x6dd4u: /* strb r2,[r12,#0x1c] */
            STORE(r[12]+28u,r[2],1);r[15]=0x6dd8u;return 0;
        case 0x6dd8u: /* strb r3,[r12,#0x15] */
            STORE(r[12]+21u,r[3],1);r[15]=0x6ddcu;return 0;
        case 0x6ddcu: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x6de0u;return 0;
        case 0x6de0u: /* strbeq r1,[r12,#0x1e] */
            if(state->cpsr&0x40000000u){STORE(r[12]+30u,r[1],1);}r[15]=0x6de4u;return 0;
        case 0x6de4u: /* bl 0x00006dec */
            r[14]=0x6de8u;r[15]=28140u;return 0;
        case 0x6de8u: /* b 0x00006db8 */
            r[15]=28088u;return 0;
        case 0x6decu: /* tst r0,#0x10 */
            temporary=r[0]&16u;logic_flags(state,temporary,0,16u);r[15]=0x6df0u;return 0;
        case 0x6df0u: /* ldmiaeq sp!,{lr} */
            if(state->cpsr&0x40000000u){LOAD(r[14],r[13]+0u,4);r[13]+=4u;}r[15]=0x6df4u;return 0;
        case 0x6df4u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x6df8u;}return 0;
        case 0x6df8u: /* bic r0,r0,#0x10 */
            r[0]=r[0]&~16u;r[15]=0x6dfcu;return 0;
        case 0x6dfcu: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x6e00u;return 0;
        case 0x6e00u: /* mov r1,r4 */
            r[1]=r[4];r[15]=0x6e04u;return 0;
        case 0x6e04u: /* mov r2,r5 */
            r[2]=r[5];r[15]=0x6e08u;return 0;
        case 0x6e08u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6e0cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6e10u;return 0;
        case 0x6e10u: /* ldr r12,[0x70a0] */
            LOAD(r[12],28832u,4);r[15]=0x6e14u;return 0;
        case 0x6e14u: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x6e18u;return 0;
        case 0x6e18u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x6e1cu;return 0;
        case 0x6e1cu: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x6e20u;return 0;
        case 0x6e20u: /* b 0x00006e40 */
            r[15]=28224u;return 0;
        case 0x6e24u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6e28u;return 0;
        case 0x6e28u: /* ldr r12,[0x7098] */
            LOAD(r[12],28824u,4);r[15]=0x6e2cu;return 0;
        case 0x6e2cu: /* ldr r7,[0x709c] */
            LOAD(r[7],28828u,4);r[15]=0x6e30u;return 0;
        case 0x6e30u: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x6e34u;return 0;
        case 0x6e34u: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x6e38u;return 0;
        case 0x6e38u: /* tst r0,#0x20 */
            temporary=r[0]&32u;logic_flags(state,temporary,0,32u);r[15]=0x6e3cu;return 0;
        case 0x6e3cu: /* bleq 0x00006dec */
            if(state->cpsr&0x40000000u){r[14]=0x6e40u;r[15]=28140u;}else{r[15]=0x6e40u;}return 0;
        case 0x6e40u: /* ldrb r7,[r12,#0x0] */
            LOAD(r[7],r[12]+0u,1);r[15]=0x6e44u;return 0;
        case 0x6e44u: /* orr r7,r7,#0x10 */
            r[7]=r[7]|16u;r[15]=0x6e48u;return 0;
        case 0x6e48u: /* strb r7,[r12,#0x0] */
            STORE(r[12]+0u,r[7],1);r[15]=0x6e4cu;return 0;
        case 0x6e4cu: /* ldrb r7,[r12,#0x1f] */
            LOAD(r[7],r[12]+31u,1);r[15]=0x6e50u;return 0;
        case 0x6e50u: /* bl 0x00006e74 */
            r[14]=0x6e54u;r[15]=28276u;return 0;
        case 0x6e54u: /* strb r1,[r12,#0x19] */
            STORE(r[12]+25u,r[1],1);r[15]=0x6e58u;return 0;
        case 0x6e58u: /* strb r2,[r12,#0x16] */
            STORE(r[12]+22u,r[2],1);r[15]=0x6e5cu;return 0;
        case 0x6e5cu: /* strb r2,[r12,#0x1d] */
            STORE(r[12]+29u,r[2],1);r[15]=0x6e60u;return 0;
        case 0x6e60u: /* strb r3,[r12,#0x17] */
            STORE(r[12]+23u,r[3],1);r[15]=0x6e64u;return 0;
        case 0x6e64u: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x6e68u;return 0;
        case 0x6e68u: /* strbeq r1,[r12,#0x1f] */
            if(state->cpsr&0x40000000u){STORE(r[12]+31u,r[1],1);}r[15]=0x6e6cu;return 0;
        case 0x6e6cu: /* bl 0x00006dec */
            r[14]=0x6e70u;r[15]=28140u;return 0;
        case 0x6e70u: /* b 0x00006e40 */
            r[15]=28224u;return 0;
        case 0x6e74u: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x6e78u;return 0;
        case 0x6e78u: /* moveq r3,#0x0 */
            if(state->cpsr&0x40000000u){r[3]=0u;}r[15]=0x6e7cu;return 0;
        case 0x6e7cu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x6e80u;}return 0;
        case 0x6e80u: /* mov r2,r2, lsr #0x2 */
            r[2]=(r[2]>>2);r[15]=0x6e84u;return 0;
        case 0x6e84u: /* rsbs r3,r1,r7 */
            r[3]=subtract(state,r[7],r[1]);r[15]=0x6e88u;return 0;
        case 0x6e88u: /* ldr r7,[0x6ec8] */
            LOAD(r[7],28360u,4);r[15]=0x6e8cu;return 0;
        case 0x6e8cu: /* eormi r3,r3,r7 */
            if(state->cpsr&0x80000000u){r[3]=r[3]^r[7];}r[15]=0x6e90u;return 0;
        case 0x6e90u: /* addmi r3,r3,#0x1 */
            if(state->cpsr&0x80000000u){r[3]=r[3]+1u;}r[15]=0x6e94u;return 0;
        case 0x6e94u: /* and r3,r3,#0xff */
            r[3]=r[3]&255u;r[15]=0x6e98u;return 0;
        case 0x6e98u: /* ldr r7,[0x6ecc] */
            LOAD(r[7],28364u,4);r[15]=0x6e9cu;return 0;
        case 0x6e9cu: /* tst r3,#0xfe */
            temporary=r[3]&254u;logic_flags(state,temporary,0,254u);r[15]=0x6ea0u;return 0;
        case 0x6ea0u: /* bne 0x00006eb4 */
            if(!(state->cpsr&0x40000000u)){r[15]=28340u;}else{r[15]=0x6ea4u;}return 0;
        case 0x6ea4u: /* ldr r7,[0x6ed0] */
            LOAD(r[7],28368u,4);r[15]=0x6ea8u;return 0;
        case 0x6ea8u: /* tst r2,r7 */
            temporary=r[2]&r[7];logic_flags(state,temporary,0,r[7]);r[15]=0x6eacu;return 0;
        case 0x6eacu: /* movne r2,#0xff */
            if(!(state->cpsr&0x40000000u)){r[2]=255u;}r[15]=0x6eb0u;return 0;
        case 0x6eb0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6eb4u: /* tst r2,r7 */
            temporary=r[2]&r[7];logic_flags(state,temporary,0,r[7]);r[15]=0x6eb8u;return 0;
        case 0x6eb8u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x6ebcu;}return 0;
        case 0x6ebcu: /* mov r3,r3, lsr #0x1 */
            r[3]=(r[3]>>1);r[15]=0x6ec0u;return 0;
        case 0x6ec0u: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x6ec4u;return 0;
        case 0x6ec4u: /* b 0x00006e9c */
            r[15]=28316u;return 0;
        case 0x6ed4u: /* ldr r12,[0x70a0] */
            LOAD(r[12],28832u,4);r[15]=0x6ed8u;return 0;
        case 0x6ed8u: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x6edcu;return 0;
        case 0x6edcu: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x6ee0u;return 0;
        case 0x6ee0u: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x6ee4u;return 0;
        case 0x6ee4u: /* mov r8,#0x1 */
            r[8]=1u;r[15]=0x6ee8u;return 0;
        case 0x6ee8u: /* b 0x00006f00 */
            r[15]=28416u;return 0;
        case 0x6eecu: /* ldr r12,[0x7098] */
            LOAD(r[12],28824u,4);r[15]=0x6ef0u;return 0;
        case 0x6ef0u: /* ldr r7,[0x709c] */
            LOAD(r[7],28828u,4);r[15]=0x6ef4u;return 0;
        case 0x6ef4u: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x6ef8u;return 0;
        case 0x6ef8u: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x6efcu;return 0;
        case 0x6efcu: /* mov r8,#0x2 */
            r[8]=2u;r[15]=0x6f00u;return 0;
        case 0x6f00u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6f04u;return 0;
        case 0x6f04u: /* bic r2,r2,#0xf */
            r[2]=r[2]&~15u;r[15]=0x6f08u;return 0;
        case 0x6f08u: /* cmp r1,#0xff */
            (void)subtract(state,r[1],255u);r[15]=0x6f0cu;return 0;
        case 0x6f0cu: /* addne r4,r1,#0x40 */
            if(!(state->cpsr&0x40000000u)){r[4]=r[1]+64u;}r[15]=0x6f10u;return 0;
        case 0x6f10u: /* strb r2,[r12,#0x22] */
            STORE(r[12]+34u,r[2],1);r[15]=0x6f14u;return 0;
        case 0x6f14u: /* strb r4,[r12,#0x29] */
            STORE(r[12]+41u,r[4],1);r[15]=0x6f18u;return 0;
        case 0x6f18u: /* ldrb r3,[r12,#0x0] */
            LOAD(r[3],r[12]+0u,1);r[15]=0x6f1cu;return 0;
        case 0x6f1cu: /* tst r3,#0x1 */
            temporary=r[3]&1u;logic_flags(state,temporary,0,1u);r[15]=0x6f20u;return 0;
        case 0x6f20u: /* beq 0x00006f54 */
            if(state->cpsr&0x40000000u){r[15]=28500u;}else{r[15]=0x6f24u;}return 0;
        case 0x6f24u: /* cmp r1,#0xff */
            (void)subtract(state,r[1],255u);r[15]=0x6f28u;return 0;
        case 0x6f28u: /* movne r6,r2 */
            if(!(state->cpsr&0x40000000u)){r[6]=r[2];}r[15]=0x6f2cu;return 0;
        case 0x6f2cu: /* bne 0x00006f38 */
            if(!(state->cpsr&0x40000000u)){r[15]=28472u;}else{r[15]=0x6f30u;}return 0;
        case 0x6f30u: /* ldrb r1,[r12,#0x25] */
            LOAD(r[1],r[12]+37u,1);r[15]=0x6f34u;return 0;
        case 0x6f34u: /* ldrb r6,[r12,#0x26] */
            LOAD(r[6],r[12]+38u,1);r[15]=0x6f38u;return 0;
        case 0x6f38u: /* bl 0x0000707c */
            r[14]=0x6f3cu;r[15]=28796u;return 0;
        case 0x6f3cu: /* ldrb r7,[r12,#0x1a] */
            LOAD(r[7],r[12]+26u,1);r[15]=0x6f40u;return 0;
        case 0x6f40u: /* mov r6,r6, lsr #0x4 */
            r[6]=(r[6]>>4);r[15]=0x6f44u;return 0;
        case 0x6f44u: /* bl 0x0000705c */
            r[14]=0x6f48u;r[15]=28764u;return 0;
        case 0x6f48u: /* mov r7,r7, lsl #0x4 */
            r[7]=(r[7]<<4);r[15]=0x6f4cu;return 0;
        case 0x6f4cu: /* add r7,r1,r7 */
            r[7]=r[1]+r[7];r[15]=0x6f50u;return 0;
        case 0x6f50u: /* strb r7,[r11,#0x20] */
            STORE(r[11]+32u,r[7],1);r[15]=0x6f54u;return 0;
        case 0x6f54u: /* subs r8,r8,#0x1 */
            r[8]=subtract(state,r[8],1u);r[15]=0x6f58u;return 0;
        case 0x6f58u: /* ldmiaeq sp!,{lr} */
            if(state->cpsr&0x40000000u){LOAD(r[14],r[13]+0u,4);r[13]+=4u;}r[15]=0x6f5cu;return 0;
        case 0x6f5cu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x6f60u;}return 0;
        case 0x6f60u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x6f64u;return 0;
        case 0x6f64u: /* b 0x00006f10 */
            r[15]=28432u;return 0;
        case 0x6f68u: /* ldr r12,[0x70a0] */
            LOAD(r[12],28832u,4);r[15]=0x6f6cu;return 0;
        case 0x6f6cu: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x6f70u;return 0;
        case 0x6f70u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x6f74u;return 0;
        case 0x6f74u: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x6f78u;return 0;
        case 0x6f78u: /* mov r8,#0x1 */
            r[8]=1u;r[15]=0x6f7cu;return 0;
        case 0x6f7cu: /* b 0x00006f94 */
            r[15]=28564u;return 0;
        case 0x6f80u: /* ldr r12,[0x7098] */
            LOAD(r[12],28824u,4);r[15]=0x6f84u;return 0;
        case 0x6f84u: /* ldr r7,[0x709c] */
            LOAD(r[7],28828u,4);r[15]=0x6f88u;return 0;
        case 0x6f88u: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x6f8cu;return 0;
        case 0x6f8cu: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x6f90u;return 0;
        case 0x6f90u: /* mov r8,#0x2 */
            r[8]=2u;r[15]=0x6f94u;return 0;
        case 0x6f94u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6f98u;return 0;
        case 0x6f98u: /* strb r1,[r12,#0x1a] */
            STORE(r[12]+26u,r[1],1);r[15]=0x6f9cu;return 0;
        case 0x6f9cu: /* ldrb r2,[r12,#0x0] */
            LOAD(r[2],r[12]+0u,1);r[15]=0x6fa0u;return 0;
        case 0x6fa0u: /* tst r2,#0x1 */
            temporary=r[2]&1u;logic_flags(state,temporary,0,1u);r[15]=0x6fa4u;return 0;
        case 0x6fa4u: /* beq 0x00006fdc */
            if(state->cpsr&0x40000000u){r[15]=28636u;}else{r[15]=0x6fa8u;}return 0;
        case 0x6fa8u: /* bl 0x0000707c */
            r[14]=0x6facu;r[15]=28796u;return 0;
        case 0x6facu: /* mov r7,r1 */
            r[7]=r[1];r[15]=0x6fb0u;return 0;
        case 0x6fb0u: /* ldrb r2,[r12,#0x29] */
            LOAD(r[2],r[12]+41u,1);r[15]=0x6fb4u;return 0;
        case 0x6fb4u: /* tst r2,#0x40 */
            temporary=r[2]&64u;logic_flags(state,temporary,0,64u);r[15]=0x6fb8u;return 0;
        case 0x6fb8u: /* ldrbne r6,[r12,#0x22] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[6],r[12]+34u,1);}r[15]=0x6fbcu;return 0;
        case 0x6fbcu: /* ldrbeq r6,[r12,#0x26] */
            if(state->cpsr&0x40000000u){LOAD(r[6],r[12]+38u,1);}r[15]=0x6fc0u;return 0;
        case 0x6fc0u: /* mov r6,r6, lsr #0x4 */
            r[6]=(r[6]>>4);r[15]=0x6fc4u;return 0;
        case 0x6fc4u: /* bl 0x0000705c */
            r[14]=0x6fc8u;r[15]=28764u;return 0;
        case 0x6fc8u: /* mov r7,r7, lsl #0x4 */
            r[7]=(r[7]<<4);r[15]=0x6fccu;return 0;
        case 0x6fccu: /* ldrb r6,[r11,#0x20] */
            LOAD(r[6],r[11]+32u,1);r[15]=0x6fd0u;return 0;
        case 0x6fd0u: /* and r6,r6,#0xf */
            r[6]=r[6]&15u;r[15]=0x6fd4u;return 0;
        case 0x6fd4u: /* orr r7,r6,r7 */
            r[7]=r[6]|r[7];r[15]=0x6fd8u;return 0;
        case 0x6fd8u: /* strb r7,[r11,#0x20] */
            STORE(r[11]+32u,r[7],1);r[15]=0x6fdcu;return 0;
        case 0x6fdcu: /* subs r8,r8,#0x1 */
            r[8]=subtract(state,r[8],1u);r[15]=0x6fe0u;return 0;
        case 0x6fe0u: /* ldmiaeq sp!,{lr} */
            if(state->cpsr&0x40000000u){LOAD(r[14],r[13]+0u,4);r[13]+=4u;}r[15]=0x6fe4u;return 0;
        case 0x6fe4u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x6fe8u;}return 0;
        case 0x6fe8u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x6fecu;return 0;
        case 0x6fecu: /* b 0x00006f98 */
            r[15]=28568u;return 0;
        case 0x6ff0u: /* ldr r12,[0x70a0] */
            LOAD(r[12],28832u,4);r[15]=0x6ff4u;return 0;
        case 0x6ff4u: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x6ff8u;return 0;
        case 0x6ff8u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x6ffcu;return 0;
        case 0x6ffcu: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x7000u;return 0;
        case 0x7000u: /* mov r8,#0x1 */
            r[8]=1u;r[15]=0x7004u;return 0;
        case 0x7004u: /* b 0x0000701c */
            r[15]=28700u;return 0;
        case 0x7008u: /* ldr r12,[0x7098] */
            LOAD(r[12],28824u,4);r[15]=0x700cu;return 0;
        case 0x700cu: /* ldr r7,[0x709c] */
            LOAD(r[7],28828u,4);r[15]=0x7010u;return 0;
        case 0x7010u: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x7014u;return 0;
        case 0x7014u: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x7018u;return 0;
        case 0x7018u: /* mov r8,#0x2 */
            r[8]=2u;r[15]=0x701cu;return 0;
        case 0x701cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7020u;return 0;
        case 0x7020u: /* strb r1,[r12,#0x1b] */
            STORE(r[12]+27u,r[1],1);r[15]=0x7024u;return 0;
        case 0x7024u: /* ldrb r2,[r12,#0x0] */
            LOAD(r[2],r[12]+0u,1);r[15]=0x7028u;return 0;
        case 0x7028u: /* tst r2,#0x1 */
            temporary=r[2]&1u;logic_flags(state,temporary,0,1u);r[15]=0x702cu;return 0;
        case 0x702cu: /* beq 0x00007048 */
            if(state->cpsr&0x40000000u){r[15]=28744u;}else{r[15]=0x7030u;}return 0;
        case 0x7030u: /* bl 0x0000707c */
            r[14]=0x7034u;r[15]=28796u;return 0;
        case 0x7034u: /* mov r7,r1 */
            r[7]=r[1];r[15]=0x7038u;return 0;
        case 0x7038u: /* ldrb r6,[r12,#0x2a] */
            LOAD(r[6],r[12]+42u,1);r[15]=0x703cu;return 0;
        case 0x703cu: /* bl 0x0000705c */
            r[14]=0x7040u;r[15]=28764u;return 0;
        case 0x7040u: /* strb r7,[r12,#0x2b] */
            STORE(r[12]+43u,r[7],1);r[15]=0x7044u;return 0;
        case 0x7044u: /* strb r7,[r11,#0x25] */
            STORE(r[11]+37u,r[7],1);r[15]=0x7048u;return 0;
        case 0x7048u: /* subs r8,r8,#0x1 */
            r[8]=subtract(state,r[8],1u);r[15]=0x704cu;return 0;
        case 0x704cu: /* ldmiaeq sp!,{lr} */
            if(state->cpsr&0x40000000u){LOAD(r[14],r[13]+0u,4);r[13]+=4u;}r[15]=0x7050u;return 0;
        case 0x7050u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7054u;}return 0;
        case 0x7054u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x7058u;return 0;
        case 0x7058u: /* b 0x00007020 */
            r[15]=28704u;return 0;
        case 0x705cu: /* subs r7,r7,#0x80 */
            r[7]=subtract(state,r[7],128u);r[15]=0x7060u;return 0;
        case 0x7060u: /* mov r6,r6, lsl #0x3 */
            r[6]=(r[6]<<3);r[15]=0x7064u;return 0;
        case 0x7064u: /* adds r7,r6,r7 */
            r[7]=addition(state,r[6],r[7]);r[15]=0x7068u;return 0;
        case 0x7068u: /* mov r7,r7, asr #0x3 */
            r[7]=(r[7]>>3)|((0u-(r[7]>>31))<<29);r[15]=0x706cu;return 0;
        case 0x706cu: /* movmi r7,#0x0 */
            if(state->cpsr&0x80000000u){r[7]=0u;}r[15]=0x7070u;return 0;
        case 0x7070u: /* cmp r7,#0xf */
            (void)subtract(state,r[7],15u);r[15]=0x7074u;return 0;
        case 0x7074u: /* movgt r7,#0xf */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[7]=15u;r[15]=0x7078u;}else{r[15]=0x7078u;}return 0;
        case 0x7078u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x707cu: /* ldrb r7,[r12,#0x3] */
            LOAD(r[7],r[12]+3u,1);r[15]=0x7080u;return 0;
        case 0x7080u: /* ldr r10,[0x7094] */
            LOAD(r[10],28820u,4);r[15]=0x7084u;return 0;
        case 0x7084u: /* mov r9,#0xc */
            r[9]=12u;r[15]=0x7088u;return 0;
        case 0x7088u: /* mla r10,r7,r9,r10 */
            r[10]=r[7]*r[9]+r[10];r[15]=0x708cu;return 0;
        case 0x708cu: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x7090u;return 0;
        case 0x7090u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7404u: /* ldr r3,[r12,#0xc] */
            LOAD(r[3],r[12]+12u,4);r[15]=0x7408u;return 0;
        case 0x7408u: /* cmp r1,r3 */
            (void)subtract(state,r[1],r[3]);r[15]=0x740cu;return 0;
        case 0x740cu: /* movcs r0,#0x4 */
            if(state->cpsr&0x20000000u){r[0]=4u;}r[15]=0x7410u;return 0;
        case 0x7410u: /* bcs 0x000074d4 */
            if(state->cpsr&0x20000000u){r[15]=29908u;}else{r[15]=0x7414u;}return 0;
        case 0x7414u: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x7418u;return 0;
        case 0x7418u: /* mov r3,#0x40 */
            r[3]=64u;r[15]=0x741cu;return 0;
        case 0x741cu: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x7420u;return 0;
        case 0x7420u: /* mla r11,r3,r0,r11 */
            r[11]=r[3]*r[0]+r[11];r[15]=0x7424u;return 0;
        case 0x7424u: /* bic r2,r2,#0x7 */
            r[2]=r[2]&~7u;r[15]=0x7428u;return 0;
        case 0x7428u: /* ldr r4,[r11,#0x1] */
            LOAD(r[4],r[11]+1u,4);r[15]=0x742cu;return 0;
        case 0x742cu: /* tst r4,#0x1 */
            temporary=r[4]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7430u;return 0;
        case 0x7430u: /* beq 0x00007464 */
            if(state->cpsr&0x40000000u){r[15]=29796u;}else{r[15]=0x7434u;}return 0;
        case 0x7434u: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x7438u;return 0;
        case 0x7438u: /* beq 0x00007458 */
            if(state->cpsr&0x40000000u){r[15]=29784u;}else{r[15]=0x743cu;}return 0;
        case 0x743cu: /* ldrb r3,[r11,#0x0] */
            LOAD(r[3],r[11]+0u,1);r[15]=0x7440u;return 0;
        case 0x7440u: /* cmp r3,#0x0 */
            (void)subtract(state,r[3],0u);r[15]=0x7444u;return 0;
        case 0x7444u: /* beq 0x00007458 */
            if(state->cpsr&0x40000000u){r[15]=29784u;}else{r[15]=0x7448u;}return 0;
        case 0x7448u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x744cu;return 0;
        case 0x744cu: /* bcs 0x00007458 */
            if(state->cpsr&0x20000000u){r[15]=29784u;}else{r[15]=0x7450u;}return 0;
        case 0x7450u: /* mov r0,#0x2 */
            r[0]=2u;r[15]=0x7454u;return 0;
        case 0x7454u: /* b 0x000074d4 */
            r[15]=29908u;return 0;
        case 0x7458u: /* stmdb sp!,{r0,r1,r2,r12,lr} */
            STORE(r[13]-20u,r[0],4);STORE(r[13]-16u,r[1],4);STORE(r[13]-12u,r[2],4);STORE(r[13]-8u,r[12],4);STORE(r[13]-4u,r[14],4);r[13]-=20u;r[15]=0x745cu;return 0;
        case 0x745cu: /* bl 0x000074fc */
            r[14]=0x7460u;r[15]=29948u;return 0;
        case 0x7460u: /* ldmia sp!,{r0,r1,r2,r12,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[1],r[13]+4u,4);LOAD(r[2],r[13]+8u,4);LOAD(r[12],r[13]+12u,4);LOAD(r[14],r[13]+16u,4);r[13]+=20u;r[15]=0x7464u;return 0;
        case 0x7464u: /* strb r2,[r11,#0x0] */
            STORE(r[11]+0u,r[2],1);r[15]=0x7468u;return 0;
        case 0x7468u: /* add r10,r12,#0x10 */
            r[10]=r[12]+16u;r[15]=0x746cu;return 0;
        case 0x746cu: /* add r1,r10,r1, lsl #0x2 */
            r[1]=r[10]+(r[1]<<2);r[15]=0x7470u;return 0;
        case 0x7470u: /* ldr r10,[r1,#0x0] */
            LOAD(r[10],r[1]+0u,4);r[15]=0x7474u;return 0;
        case 0x7474u: /* ldr r1,[r1,#0x4] */
            LOAD(r[1],r[1]+4u,4);r[15]=0x7478u;return 0;
        case 0x7478u: /* ldr r2,[0x76f8] */
            LOAD(r[2],30456u,4);r[15]=0x747cu;return 0;
        case 0x747cu: /* cmp r1,r2 */
            (void)subtract(state,r[1],r[2]);r[15]=0x7480u;return 0;
        case 0x7480u: /* ldreq r1,[r12,#0x8] */
            if(state->cpsr&0x40000000u){LOAD(r[1],r[12]+8u,4);}r[15]=0x7484u;return 0;
        case 0x7484u: /* subeq r1,r1,#0x4 */
            if(state->cpsr&0x40000000u){r[1]=r[1]-4u;}r[15]=0x7488u;return 0;
        case 0x7488u: /* add r1,r1,r12 */
            r[1]=r[1]+r[12];r[15]=0x748cu;return 0;
        case 0x748cu: /* add r12,r12,r10 */
            r[12]=r[12]+r[10];r[15]=0x7490u;return 0;
        case 0x7490u: /* str r12,[r11,#0x14] */
            STORE(r[11]+20u,r[12],4);r[15]=0x7494u;return 0;
        case 0x7494u: /* ldr r2,[r12,#0x4]! */
            LOAD(r[2],r[12]+4u,4);r[12]=r[12]+4u;r[15]=0x7498u;return 0;
        case 0x7498u: /* ldr r3,[r12,#0x4]! */
            LOAD(r[3],r[12]+4u,4);r[12]=r[12]+4u;r[15]=0x749cu;return 0;
        case 0x749cu: /* str r3,[r11,#0x34] */
            STORE(r[11]+52u,r[3],4);r[15]=0x74a0u;return 0;
        case 0x74a0u: /* str r3,[r11,#0x38] */
            STORE(r[11]+56u,r[3],4);r[15]=0x74a4u;return 0;
        case 0x74a4u: /* mul r3,r2,r3 */
            r[3]=r[2]*r[3];r[15]=0x74a8u;return 0;
        case 0x74a8u: /* mov r3,r3, lsr #0x4 */
            r[3]=(r[3]>>4);r[15]=0x74acu;return 0;
        case 0x74acu: /* str r1,[r11,#0x4] */
            STORE(r[11]+4u,r[1],4);r[15]=0x74b0u;return 0;
        case 0x74b0u: /* str r2,[r11,#0x8] */
            STORE(r[11]+8u,r[2],4);r[15]=0x74b4u;return 0;
        case 0x74b4u: /* str r3,[r11,#0xc] */
            STORE(r[11]+12u,r[3],4);r[15]=0x74b8u;return 0;
        case 0x74b8u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x74bcu;return 0;
        case 0x74bcu: /* str r1,[r11,#0x30] */
            STORE(r[11]+48u,r[1],4);r[15]=0x74c0u;return 0;
        case 0x74c0u: /* add r12,r12,#0x4 */
            r[12]=r[12]+4u;r[15]=0x74c4u;return 0;
        case 0x74c4u: /* str r12,[r11,#0x10] */
            STORE(r[11]+16u,r[12],4);r[15]=0x74c8u;return 0;
        case 0x74c8u: /* mov r1,#0x1 */
            r[1]=1u;r[15]=0x74ccu;return 0;
        case 0x74ccu: /* strb r1,[r11,#0x1] */
            STORE(r[11]+1u,r[1],1);r[15]=0x74d0u;return 0;
        case 0x74d0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x74d4u: /* ldr r9,[0x76e8] */
            LOAD(r[9],30440u,4);r[15]=0x74d8u;return 0;
        case 0x74d8u: /* ldr r1,[r9,#0x0] */
            LOAD(r[1],r[9]+0u,4);r[15]=0x74dcu;return 0;
        case 0x74dcu: /* orr r0,r1,r0 */
            r[0]=r[1]|r[0];r[15]=0x74e0u;return 0;
        case 0x74e0u: /* strb r0,[r9,#0x0] */
            STORE(r[9]+0u,r[0],1);r[15]=0x74e4u;return 0;
        case 0x74e4u: /* ldrb r1,[r11,#0x1] */
            LOAD(r[1],r[11]+1u,1);r[15]=0x74e8u;return 0;
        case 0x74e8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x74ecu: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x74f0u;return 0;
        case 0x74f0u: /* mov r2,#0x40 */
            r[2]=64u;r[15]=0x74f4u;return 0;
        case 0x74f4u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x74f8u;return 0;
        case 0x74f8u: /* mla r11,r0,r2,r11 */
            r[11]=r[0]*r[2]+r[11];r[15]=0x74fcu;return 0;
        case 0x74fcu: /* ldrb r3,[r11,#0x1] */
            LOAD(r[3],r[11]+1u,1);r[15]=0x7500u;return 0;
        case 0x7500u: /* bic r3,r3,#0x1 */
            r[3]=r[3]&~1u;r[15]=0x7504u;return 0;
        case 0x7504u: /* strb r3,[r11,#0x1] */
            STORE(r[11]+1u,r[3],1);r[15]=0x7508u;return 0;
        case 0x7508u: /* mov r3,#0x0 */
            r[3]=0u;r[15]=0x750cu;return 0;
        case 0x750cu: /* strb r3,[r11,#0x0] */
            STORE(r[11]+0u,r[3],1);r[15]=0x7510u;return 0;
        case 0x7510u: /* strb r3,[r11,#0x2] */
            STORE(r[11]+2u,r[3],1);r[15]=0x7514u;return 0;
        case 0x7514u: /* str r3,[r11,#0x4] */
            STORE(r[11]+4u,r[3],4);r[15]=0x7518u;return 0;
        case 0x7518u: /* str r3,[r11,#0x8] */
            STORE(r[11]+8u,r[3],4);r[15]=0x751cu;return 0;
        case 0x751cu: /* str r3,[r11,#0xc] */
            STORE(r[11]+12u,r[3],4);r[15]=0x7520u;return 0;
        case 0x7520u: /* strb r3,[r11,#0x3] */
            STORE(r[11]+3u,r[3],1);r[15]=0x7524u;return 0;
        case 0x7524u: /* str r3,[r11,#0x1c] */
            STORE(r[11]+28u,r[3],4);r[15]=0x7528u;return 0;
        case 0x7528u: /* str r3,[r11,#0x20] */
            STORE(r[11]+32u,r[3],4);r[15]=0x752cu;return 0;
        case 0x752cu: /* str r3,[r11,#0x24] */
            STORE(r[11]+36u,r[3],4);r[15]=0x7530u;return 0;
        case 0x7530u: /* str r3,[r11,#0x28] */
            STORE(r[11]+40u,r[3],4);r[15]=0x7534u;return 0;
        case 0x7534u: /* str r3,[r11,#0x2c] */
            STORE(r[11]+44u,r[3],4);r[15]=0x7538u;return 0;
        case 0x7538u: /* str r3,[r11,#0x34] */
            STORE(r[11]+52u,r[3],4);r[15]=0x753cu;return 0;
        case 0x753cu: /* ldr r3,[0x76f0] */
            LOAD(r[3],30448u,4);r[15]=0x7540u;return 0;
        case 0x7540u: /* add r4,r3,#0x240 */
            r[4]=r[3]+576u;r[15]=0x7544u;return 0;
        case 0x7544u: /* ldr r5,[r3,#0x0] */
            LOAD(r[5],r[3]+0u,4);r[15]=0x7548u;return 0;
        case 0x7548u: /* cmp r5,#0x0 */
            (void)subtract(state,r[5],0u);r[15]=0x754cu;return 0;
        case 0x754cu: /* beq 0x0000756c */
            if(state->cpsr&0x40000000u){r[15]=30060u;}else{r[15]=0x7550u;}return 0;
        case 0x7550u: /* and r6,r5,#0xf00000 */
            r[6]=r[5]&15728640u;r[15]=0x7554u;return 0;
        case 0x7554u: /* cmp r0,r6, lsr #0x14 */
            (void)subtract(state,r[0],(r[6]>>20));r[15]=0x7558u;return 0;
        case 0x7558u: /* bne 0x0000756c */
            if(!(state->cpsr&0x40000000u)){r[15]=30060u;}else{r[15]=0x755cu;}return 0;
        case 0x755cu: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7560u;return 0;
        case 0x7560u: /* str r6,[r3,#0x0] */
            STORE(r[3]+0u,r[6],4);r[15]=0x7564u;return 0;
        case 0x7564u: /* str r6,[r3,#0x4] */
            STORE(r[3]+4u,r[6],4);r[15]=0x7568u;return 0;
        case 0x7568u: /* str r6,[r3,#0x8] */
            STORE(r[3]+8u,r[6],4);r[15]=0x756cu;return 0;
        case 0x756cu: /* add r3,r3,#0xc */
            r[3]=r[3]+12u;r[15]=0x7570u;return 0;
        case 0x7570u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x7574u;return 0;
        case 0x7574u: /* bcs 0x000075a0 */
            if(state->cpsr&0x20000000u){r[15]=30112u;}else{r[15]=0x7578u;}return 0;
        case 0x7578u: /* b 0x00007544 */
            r[15]=30020u;return 0;
        case 0x757cu: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x7580u;return 0;
        case 0x7580u: /* mov r2,#0x40 */
            r[2]=64u;r[15]=0x7584u;return 0;
        case 0x7584u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x7588u;return 0;
        case 0x7588u: /* mla r11,r0,r2,r11 */
            r[11]=r[0]*r[2]+r[11];r[15]=0x758cu;return 0;
        case 0x758cu: /* ldrb r2,[r11,#0x1] */
            LOAD(r[2],r[11]+1u,1);r[15]=0x7590u;return 0;
        case 0x7590u: /* orr r1,r2,#0x2 */
            r[1]=r[2]|2u;r[15]=0x7594u;return 0;
        case 0x7594u: /* strb r1,[r11,#0x1] */
            STORE(r[11]+1u,r[1],1);r[15]=0x7598u;return 0;
        case 0x7598u: /* tst r2,#0x1 */
            temporary=r[2]&1u;logic_flags(state,temporary,0,1u);r[15]=0x759cu;return 0;
        case 0x759cu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x75a0u;}return 0;
        case 0x75a0u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x75a4u;return 0;
        case 0x75a4u: /* ldr r7,[0x76f4] */
            LOAD(r[7],30452u,4);r[15]=0x75a8u;return 0;
        case 0x75a8u: /* mov r3,#0x13000 */
            r[3]=77824u;r[15]=0x75acu;return 0;
        case 0x75acu: /* ldr r1,[r3,#0x0] */
            LOAD(r[1],r[3]+0u,4);r[15]=0x75b0u;return 0;
        case 0x75b0u: /* ldr r3,[0x76fc] */
            LOAD(r[3],30460u,4);r[15]=0x75b4u;return 0;
        case 0x75b4u: /* ldr r2,[r3,#0x0] */
            LOAD(r[2],r[3]+0u,4);r[15]=0x75b8u;return 0;
        case 0x75b8u: /* mov r3,#0xb000 */
            r[3]=45056u;r[15]=0x75bcu;return 0;
        case 0x75bcu: /* ldr r4,[r3,r2] */
            LOAD(r[4],r[3]+r[2],4);r[15]=0x75c0u;return 0;
        case 0x75c0u: /* and r5,r4,#0x7000000 */
            r[5]=r[4]&117440512u;r[15]=0x75c4u;return 0;
        case 0x75c4u: /* cmp r5,#0x1000000 */
            (void)subtract(state,r[5],16777216u);r[15]=0x75c8u;return 0;
        case 0x75c8u: /* bne 0x000075d8 */
            if(!(state->cpsr&0x40000000u)){r[15]=30168u;}else{r[15]=0x75ccu;}return 0;
        case 0x75ccu: /* and r5,r4,#0xf00000 */
            r[5]=r[4]&15728640u;r[15]=0x75d0u;return 0;
        case 0x75d0u: /* cmp r0,r5, lsr #0x14 */
            (void)subtract(state,r[0],(r[5]>>20));r[15]=0x75d4u;return 0;
        case 0x75d4u: /* streq r6,[r3,r2] */
            if(state->cpsr&0x40000000u){STORE(r[3]+r[2],r[6],4);}r[15]=0x75d8u;return 0;
        case 0x75d8u: /* add r2,r2,#0x4 */
            r[2]=r[2]+4u;r[15]=0x75dcu;return 0;
        case 0x75dcu: /* and r2,r2,r7 */
            r[2]=r[2]&r[7];r[15]=0x75e0u;return 0;
        case 0x75e0u: /* cmp r1,r2 */
            (void)subtract(state,r[1],r[2]);r[15]=0x75e4u;return 0;
        case 0x75e4u: /* bne 0x000075bc */
            if(!(state->cpsr&0x40000000u)){r[15]=30140u;}else{r[15]=0x75e8u;}return 0;
        case 0x75e8u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x75ecu;return 0;
        case 0x75ecu: /* bl 0x000017b0 */
            r[14]=0x75f0u;r[15]=6064u;return 0;
        case 0x75f0u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x75f4u;return 0;
        case 0x75f4u: /* ldrb r1,[r11,#0x1] */
            LOAD(r[1],r[11]+1u,1);r[15]=0x75f8u;return 0;
        case 0x75f8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x75fcu: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x7600u;return 0;
        case 0x7600u: /* mov r2,#0x40 */
            r[2]=64u;r[15]=0x7604u;return 0;
        case 0x7604u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x7608u;return 0;
        case 0x7608u: /* mla r11,r0,r2,r11 */
            r[11]=r[0]*r[2]+r[11];r[15]=0x760cu;return 0;
        case 0x760cu: /* ldrb r1,[r11,#0x1] */
            LOAD(r[1],r[11]+1u,1);r[15]=0x7610u;return 0;
        case 0x7610u: /* tst r1,#0x2 */
            temporary=r[1]&2u;logic_flags(state,temporary,0,2u);r[15]=0x7614u;return 0;
        case 0x7614u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7618u;}return 0;
        case 0x7618u: /* bic r1,r1,#0x2 */
            r[1]=r[1]&~2u;r[15]=0x761cu;return 0;
        case 0x761cu: /* strb r1,[r11,#0x1] */
            STORE(r[11]+1u,r[1],1);r[15]=0x7620u;return 0;
        case 0x7620u: /* tst r1,#0x1 */
            temporary=r[1]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7624u;return 0;
        case 0x7624u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7628u;}return 0;
        case 0x7628u: /* ldr r10,[0x76f0] */
            LOAD(r[10],30448u,4);r[15]=0x762cu;return 0;
        case 0x762cu: /* add r9,r10,#0x240 */
            r[9]=r[10]+576u;r[15]=0x7630u;return 0;
        case 0x7630u: /* mov r8,#0x13000 */
            r[8]=77824u;r[15]=0x7634u;return 0;
        case 0x7634u: /* ldr r5,[0x76f4] */
            LOAD(r[5],30452u,4);r[15]=0x7638u;return 0;
        case 0x7638u: /* mov r7,#0xb000 */
            r[7]=45056u;r[15]=0x763cu;return 0;
        case 0x763cu: /* ldr r2,[r8,#0x0] */
            LOAD(r[2],r[8]+0u,4);r[15]=0x7640u;return 0;
        case 0x7640u: /* ldr r4,[r8,#0x4] */
            LOAD(r[4],r[8]+4u,4);r[15]=0x7644u;return 0;
        case 0x7644u: /* sub r4,r4,#0x4 */
            r[4]=r[4]-4u;r[15]=0x7648u;return 0;
        case 0x7648u: /* cmp r4,r2 */
            (void)subtract(state,r[4],r[2]);r[15]=0x764cu;return 0;
        case 0x764cu: /* beq 0x00007698 */
            if(state->cpsr&0x40000000u){r[15]=30360u;}else{r[15]=0x7650u;}return 0;
        case 0x7650u: /* cmp r10,r9 */
            (void)subtract(state,r[10],r[9]);r[15]=0x7654u;return 0;
        case 0x7654u: /* streq r2,[r8,#0x0] */
            if(state->cpsr&0x40000000u){STORE(r[8]+0u,r[2],4);}r[15]=0x7658u;return 0;
        case 0x7658u: /* ldrbeq r1,[r11,#0x1] */
            if(state->cpsr&0x40000000u){LOAD(r[1],r[11]+1u,1);}r[15]=0x765cu;return 0;
        case 0x765cu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7660u;}return 0;
        case 0x7660u: /* ldr r1,[r10],#0xc */
            LOAD(r[1],r[10],4);r[10]+=12u;r[15]=0x7664u;return 0;
        case 0x7664u: /* cmp r1,#0x0 */
            (void)subtract(state,r[1],0u);r[15]=0x7668u;return 0;
        case 0x7668u: /* beq 0x00007650 */
            if(state->cpsr&0x40000000u){r[15]=30288u;}else{r[15]=0x766cu;}return 0;
        case 0x766cu: /* and r3,r1,#0xf00000 */
            r[3]=r[1]&15728640u;r[15]=0x7670u;return 0;
        case 0x7670u: /* cmp r0,r3, lsr #0x14 */
            (void)subtract(state,r[0],(r[3]>>20));r[15]=0x7674u;return 0;
        case 0x7674u: /* bne 0x00007650 */
            if(!(state->cpsr&0x40000000u)){r[15]=30288u;}else{r[15]=0x7678u;}return 0;
        case 0x7678u: /* orr r1,r1,#0x1000000 */
            r[1]=r[1]|16777216u;r[15]=0x767cu;return 0;
        case 0x767cu: /* cmp r4,r2 */
            (void)subtract(state,r[4],r[2]);r[15]=0x7680u;return 0;
        case 0x7680u: /* beq 0x00007694 */
            if(state->cpsr&0x40000000u){r[15]=30356u;}else{r[15]=0x7684u;}return 0;
        case 0x7684u: /* str r1,[r7,r2] */
            STORE(r[7]+r[2],r[1],4);r[15]=0x7688u;return 0;
        case 0x7688u: /* add r2,r2,#0x4 */
            r[2]=r[2]+4u;r[15]=0x768cu;return 0;
        case 0x768cu: /* and r2,r2,r5 */
            r[2]=r[2]&r[5];r[15]=0x7690u;return 0;
        case 0x7690u: /* b 0x00007650 */
            r[15]=30288u;return 0;
        case 0x7694u: /* str r1,[r10,#-0xc]! */
            STORE(r[10]-12u,r[1],4);r[10]=r[10]-12u;r[15]=0x7698u;return 0;
        case 0x7698u: /* cmp r10,r9 */
            (void)subtract(state,r[10],r[9]);r[15]=0x769cu;return 0;
        case 0x769cu: /* beq 0x000076c4 */
            if(state->cpsr&0x40000000u){r[15]=30404u;}else{r[15]=0x76a0u;}return 0;
        case 0x76a0u: /* ldr r1,[r10],#0xc */
            LOAD(r[1],r[10],4);r[10]+=12u;r[15]=0x76a4u;return 0;
        case 0x76a4u: /* cmp r1,#0x0 */
            (void)subtract(state,r[1],0u);r[15]=0x76a8u;return 0;
        case 0x76a8u: /* beq 0x00007698 */
            if(state->cpsr&0x40000000u){r[15]=30360u;}else{r[15]=0x76acu;}return 0;
        case 0x76acu: /* and r4,r1,#0xf00000 */
            r[4]=r[1]&15728640u;r[15]=0x76b0u;return 0;
        case 0x76b0u: /* cmp r0,r4, lsr #0x14 */
            (void)subtract(state,r[0],(r[4]>>20));r[15]=0x76b4u;return 0;
        case 0x76b4u: /* bne 0x00007698 */
            if(!(state->cpsr&0x40000000u)){r[15]=30360u;}else{r[15]=0x76b8u;}return 0;
        case 0x76b8u: /* orr r1,r1,#0x1000000 */
            r[1]=r[1]|16777216u;r[15]=0x76bcu;return 0;
        case 0x76bcu: /* str r1,[r10,#-0xc] */
            STORE(r[10]-12u,r[1],4);r[15]=0x76c0u;return 0;
        case 0x76c0u: /* b 0x00007698 */
            r[15]=30360u;return 0;
        case 0x76c4u: /* mov r0,#0x20 */
            r[0]=32u;r[15]=0x76c8u;return 0;
        case 0x76c8u: /* b 0x000074d4 */
            r[15]=29908u;return 0;
        case 0x76ccu: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x76d0u;return 0;
        case 0x76d0u: /* mov r2,#0x40 */
            r[2]=64u;r[15]=0x76d4u;return 0;
        case 0x76d4u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x76d8u;return 0;
        case 0x76d8u: /* mla r11,r0,r2,r11 */
            r[11]=r[0]*r[2]+r[11];r[15]=0x76dcu;return 0;
        case 0x76dcu: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x76e0u;return 0;
        case 0x76e0u: /* strb r1,[r11,#0x3] */
            STORE(r[11]+3u,r[1],1);r[15]=0x76e4u;return 0;
        case 0x76e4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x780cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7810u;return 0;
        case 0x7810u: /* ldr r12,[0x7804] */
            LOAD(r[12],30724u,4);r[15]=0x7814u;return 0;
        case 0x7814u: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x7818u;return 0;
        case 0x7818u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x781cu;return 0;
        case 0x781cu: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x7820u;return 0;
        case 0x7820u: /* mov r0,#0x1 */
            r[0]=1u;r[15]=0x7824u;return 0;
        case 0x7824u: /* ldr r4,[r3,#0xc] */
            LOAD(r[4],r[3]+12u,4);r[15]=0x7828u;return 0;
        case 0x7828u: /* cmp r1,r4 */
            (void)subtract(state,r[1],r[4]);r[15]=0x782cu;return 0;
        case 0x782cu: /* movhi r4,#0x4 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[4]=4u;}r[15]=0x7830u;return 0;
        case 0x7830u: /* bhi 0x0000813c */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=33084u;}else{r[15]=0x7834u;}return 0;
        case 0x7834u: /* mov r5,r2, lsr #0x3 */
            r[5]=(r[2]>>3);r[15]=0x7838u;return 0;
        case 0x7838u: /* bl 0x00007ae0 */
            r[14]=0x783cu;r[15]=31456u;return 0;
        case 0x783cu: /* add r2,r3,#0x10 */
            r[2]=r[3]+16u;r[15]=0x7840u;return 0;
        case 0x7840u: /* add r2,r2,r1, lsl #0x2 */
            r[2]=r[2]+(r[1]<<2);r[15]=0x7844u;return 0;
        case 0x7844u: /* ldr r9,[r2,#0x0] */
            LOAD(r[9],r[2]+0u,4);r[15]=0x7848u;return 0;
        case 0x7848u: /* add r7,r9,r3 */
            r[7]=r[9]+r[3];r[15]=0x784cu;return 0;
        case 0x784cu: /* add r7,r7,#0x4 */
            r[7]=r[7]+4u;r[15]=0x7850u;return 0;
        case 0x7850u: /* mov r9,r7 */
            r[9]=r[7];r[15]=0x7854u;return 0;
        case 0x7854u: /* ldr r2,[r7],#0x4 */
            LOAD(r[2],r[7],4);r[7]+=4u;r[15]=0x7858u;return 0;
        case 0x7858u: /* mov r2,r2, ror #0x10 */
            r[2]=((r[2]>>16)|(r[2]<<16));r[15]=0x785cu;return 0;
        case 0x785cu: /* add r2,r2,r3 */
            r[2]=r[2]+r[3];r[15]=0x7860u;return 0;
        case 0x7860u: /* ldr r3,[0x77f0] */
            LOAD(r[3],30704u,4);r[15]=0x7864u;return 0;
        case 0x7864u: /* and r3,r2,r3 */
            r[3]=r[2]&r[3];r[15]=0x7868u;return 0;
        case 0x7868u: /* str r3,[r12,#0x8] */
            STORE(r[12]+8u,r[3],4);r[15]=0x786cu;return 0;
        case 0x786cu: /* str r3,[r12,#0x30] */
            STORE(r[12]+48u,r[3],4);r[15]=0x7870u;return 0;
        case 0x7870u: /* mov r11,#0x0 */
            r[11]=0u;r[15]=0x7874u;return 0;
        case 0x7874u: /* str r11,[r12,#0x4] */
            STORE(r[12]+4u,r[11],4);r[15]=0x7878u;return 0;
        case 0x7878u: /* str r11,[r12,#0x2c] */
            STORE(r[12]+44u,r[11],4);r[15]=0x787cu;return 0;
        case 0x787cu: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x7880u;return 0;
        case 0x7880u: /* add r4,r11,#0x48 */
            r[4]=r[11]+72u;r[15]=0x7884u;return 0;
        case 0x7884u: /* mov r3,r2, lsr #0x10 */
            r[3]=(r[2]>>16);r[15]=0x7888u;return 0;
        case 0x7888u: /* str r3,[r11],#0x4 */
            STORE(r[11],r[3],4);r[11]+=4u;r[15]=0x788cu;return 0;
        case 0x788cu: /* ldr r5,[0x77e8] */
            LOAD(r[5],30696u,4);r[15]=0x7890u;return 0;
        case 0x7890u: /* and r2,r2,r5 */
            r[2]=r[2]&r[5];r[15]=0x7894u;return 0;
        case 0x7894u: /* str r2,[r11],#0x4 */
            STORE(r[11],r[2],4);r[11]+=4u;r[15]=0x7898u;return 0;
        case 0x7898u: /* ldr r0,[r7],#0x4 */
            LOAD(r[0],r[7],4);r[7]+=4u;r[15]=0x789cu;return 0;
        case 0x789cu: /* mov r1,r0, lsr #0x10 */
            r[1]=(r[0]>>16);r[15]=0x78a0u;return 0;
        case 0x78a0u: /* and r0,r0,r5 */
            r[0]=r[0]&r[5];r[15]=0x78a4u;return 0;
        case 0x78a4u: /* str r0,[r11],#0x4 */
            STORE(r[11],r[0],4);r[11]+=4u;r[15]=0x78a8u;return 0;
        case 0x78a8u: /* str r1,[r11],#0x4 */
            STORE(r[11],r[1],4);r[11]+=4u;r[15]=0x78acu;return 0;
        case 0x78acu: /* cmp r11,r4 */
            (void)subtract(state,r[11],r[4]);r[15]=0x78b0u;return 0;
        case 0x78b0u: /* bne 0x00007898 */
            if(!(state->cpsr&0x40000000u)){r[15]=30872u;}else{r[15]=0x78b4u;}return 0;
        case 0x78b4u: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x78b8u;return 0;
        case 0x78b8u: /* ldrb r0,[r9,#0x13] */
            LOAD(r[0],r[9]+19u,1);r[15]=0x78bcu;return 0;
        case 0x78bcu: /* strb r0,[r12,#0x2a] */
            STORE(r[12]+42u,r[0],1);r[15]=0x78c0u;return 0;
        case 0x78c0u: /* ldrb r1,[r9,#0x10] */
            LOAD(r[1],r[9]+16u,1);r[15]=0x78c4u;return 0;
        case 0x78c4u: /* bic r0,r1,#0xf0 */
            r[0]=r[1]&~240u;r[15]=0x78c8u;return 0;
        case 0x78c8u: /* strb r0,[r12,#0x25] */
            STORE(r[12]+37u,r[0],1);r[15]=0x78ccu;return 0;
        case 0x78ccu: /* bic r1,r1,#0xf */
            r[1]=r[1]&~15u;r[15]=0x78d0u;return 0;
        case 0x78d0u: /* strb r1,[r12,#0x26] */
            STORE(r[12]+38u,r[1],1);r[15]=0x78d4u;return 0;
        case 0x78d4u: /* ldr r1,[r9,#0x0] */
            LOAD(r[1],r[9]+0u,4);r[15]=0x78d8u;return 0;
        case 0x78d8u: /* and r1,r1,#0x1800000 */
            r[1]=r[1]&25165824u;r[15]=0x78dcu;return 0;
        case 0x78dcu: /* mov r1,r1, lsr #0x17 */
            r[1]=(r[1]>>23);r[15]=0x78e0u;return 0;
        case 0x78e0u: /* strb r1,[r12,#0x27] */
            STORE(r[12]+39u,r[1],1);r[15]=0x78e4u;return 0;
        case 0x78e4u: /* ldrb r2,[r9,#0x12] */
            LOAD(r[2],r[9]+18u,1);r[15]=0x78e8u;return 0;
        case 0x78e8u: /* strb r2,[r12,#0x28] */
            STORE(r[12]+40u,r[2],1);r[15]=0x78ecu;return 0;
        case 0x78ecu: /* ldrb r3,[r9,#0x15] */
            LOAD(r[3],r[9]+21u,1);r[15]=0x78f0u;return 0;
        case 0x78f0u: /* strb r3,[r12,#0x24] */
            STORE(r[12]+36u,r[3],1);r[15]=0x78f4u;return 0;
        case 0x78f4u: /* ldr r4,[r11,#0x18] */
            LOAD(r[4],r[11]+24u,4);r[15]=0x78f8u;return 0;
        case 0x78f8u: /* str r4,[r12,#0xc] */
            STORE(r[12]+12u,r[4],4);r[15]=0x78fcu;return 0;
        case 0x78fcu: /* bl 0x00007c8c */
            r[14]=0x7900u;r[15]=31884u;return 0;
        case 0x7900u: /* ldrb r0,[r9,#0x24] */
            LOAD(r[0],r[9]+36u,1);r[15]=0x7904u;return 0;
        case 0x7904u: /* strb r0,[r12,#0x3c] */
            STORE(r[12]+60u,r[0],1);r[15]=0x7908u;return 0;
        case 0x7908u: /* ldrb r1,[r12,#0x23] */
            LOAD(r[1],r[12]+35u,1);r[15]=0x790cu;return 0;
        case 0x790cu: /* ldr r2,[r12,#0x34] */
            LOAD(r[2],r[12]+52u,4);r[15]=0x7910u;return 0;
        case 0x7910u: /* bl 0x00008204 */
            r[14]=0x7914u;r[15]=33284u;return 0;
        case 0x7914u: /* ldrb r1,[r12,#0x0] */
            LOAD(r[1],r[12]+0u,1);r[15]=0x7918u;return 0;
        case 0x7918u: /* bic r1,r1,#0x2 */
            r[1]=r[1]&~2u;r[15]=0x791cu;return 0;
        case 0x791cu: /* strb r1,[r12,#0x0] */
            STORE(r[12]+0u,r[1],1);r[15]=0x7920u;return 0;
        case 0x7920u: /* ldr r1,[r11,#0x1c] */
            LOAD(r[1],r[11]+28u,4);r[15]=0x7924u;return 0;
        case 0x7924u: /* bic r1,r1,#0x8000 */
            r[1]=r[1]&~32768u;r[15]=0x7928u;return 0;
        case 0x7928u: /* str r1,[r11,#0x1c] */
            STORE(r[11]+28u,r[1],4);r[15]=0x792cu;return 0;
        case 0x792cu: /* ldr r9,[0x8248] */
            LOAD(r[9],33352u,4);r[15]=0x7930u;return 0;
        case 0x7930u: /* ldr r0,[r9,#0x10] */
            LOAD(r[0],r[9]+16u,4);r[15]=0x7934u;return 0;
        case 0x7934u: /* ldr r0,[r9,#0x14] */
            LOAD(r[0],r[9]+20u,4);r[15]=0x7938u;return 0;
        case 0x7938u: /* b 0x00008150 */
            r[15]=33104u;return 0;
        case 0x793cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7940u;return 0;
        case 0x7940u: /* ldr r12,[0x77f8] */
            LOAD(r[12],30712u,4);r[15]=0x7944u;return 0;
        case 0x7944u: /* ldr r7,[0x77fc] */
            LOAD(r[7],30716u,4);r[15]=0x7948u;return 0;
        case 0x7948u: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x794cu;return 0;
        case 0x794cu: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x7950u;return 0;
        case 0x7950u: /* mov r0,#0x2 */
            r[0]=2u;r[15]=0x7954u;return 0;
        case 0x7954u: /* str r2,[r12,#0x38] */
            STORE(r[12]+56u,r[2],4);r[15]=0x7958u;return 0;
        case 0x7958u: /* ands r2,r2,#0x10000 */
            temporary=r[2]&65536u;r[2]=temporary;logic_flags(state,temporary,1,65536u);r[15]=0x795cu;return 0;
        case 0x795cu: /* movne r2,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[2]=31u;}r[15]=0x7960u;return 0;
        case 0x7960u: /* mov r5,r5, lsr #0x3 */
            r[5]=(r[5]>>3);r[15]=0x7964u;return 0;
        case 0x7964u: /* bl 0x00007ae0 */
            r[14]=0x7968u;r[15]=31456u;return 0;
        case 0x7968u: /* strb r1,[r12,#0x27] */
            STORE(r[12]+39u,r[1],1);r[15]=0x796cu;return 0;
        case 0x796cu: /* strb r3,[r12,#0x24] */
            STORE(r[12]+36u,r[3],1);r[15]=0x7970u;return 0;
        case 0x7970u: /* str r4,[r12,#0xc] */
            STORE(r[12]+12u,r[4],4);r[15]=0x7974u;return 0;
        case 0x7974u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7978u;return 0;
        case 0x7978u: /* strb r6,[r12,#0x26] */
            STORE(r[12]+38u,r[6],1);r[15]=0x797cu;return 0;
        case 0x797cu: /* ldr r6,[0x7800] */
            LOAD(r[6],30720u,4);r[15]=0x7980u;return 0;
        case 0x7980u: /* cmp r0,#0x2 */
            (void)subtract(state,r[0],2u);r[15]=0x7984u;return 0;
        case 0x7984u: /* ldrbne r7,[r12,#-0x1f] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[7],r[12]-31u,1);}r[15]=0x7988u;return 0;
        case 0x7988u: /* ldrbeq r7,[r12,#0x38] */
            if(state->cpsr&0x40000000u){LOAD(r[7],r[12]+56u,1);}r[15]=0x798cu;return 0;
        case 0x798cu: /* add r6,r6,r7, lsl #0x3 */
            r[6]=r[6]+(r[7]<<3);r[15]=0x7990u;return 0;
        case 0x7990u: /* ldr r7,[r6],#0x4 */
            LOAD(r[7],r[6],4);r[6]+=4u;r[15]=0x7994u;return 0;
        case 0x7994u: /* cmp r7,#0x0 */
            (void)subtract(state,r[7],0u);r[15]=0x7998u;return 0;
        case 0x7998u: /* beq 0x00007ac0 */
            if(state->cpsr&0x40000000u){r[15]=31424u;}else{r[15]=0x799cu;}return 0;
        case 0x799cu: /* str r7,[r12,#0x30] */
            STORE(r[12]+48u,r[7],4);r[15]=0x79a0u;return 0;
        case 0x79a0u: /* ldr r7,[r6,#0x0] */
            LOAD(r[7],r[6]+0u,4);r[15]=0x79a4u;return 0;
        case 0x79a4u: /* cmp r1,#0x1 */
            (void)subtract(state,r[1],1u);r[15]=0x79a8u;return 0;
        case 0x79a8u: /* bgt 0x000079c0 */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[15]=31168u;}else{r[15]=0x79acu;}return 0;
        case 0x79acu: /* blt 0x000079d4 */
            if((((state->cpsr>>31)^(state->cpsr>>28))&1u)!=0){r[15]=31188u;}else{r[15]=0x79b0u;}return 0;
        case 0x79b0u: /* cmp r7,#0xf000 */
            (void)subtract(state,r[7],61440u);r[15]=0x79b4u;return 0;
        case 0x79b4u: /* movgt r7,#0xf000 */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[7]=61440u;r[15]=0x79b8u;}else{r[15]=0x79b8u;}return 0;
        case 0x79b8u: /* mov r8,r7 */
            r[8]=r[7];r[15]=0x79bcu;return 0;
        case 0x79bcu: /* b 0x000079e0 */
            r[15]=31200u;return 0;
        case 0x79c0u: /* cmp r7,#0x7000 */
            (void)subtract(state,r[7],28672u);r[15]=0x79c4u;return 0;
        case 0x79c4u: /* movgt r7,#0x7000 */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[7]=28672u;r[15]=0x79c8u;}else{r[15]=0x79c8u;}return 0;
        case 0x79c8u: /* mov r8,r7, lsl #0x1 */
            r[8]=(r[7]<<1);r[15]=0x79ccu;return 0;
        case 0x79ccu: /* mov r1,#0x3 */
            r[1]=3u;r[15]=0x79d0u;return 0;
        case 0x79d0u: /* b 0x000079e0 */
            r[15]=31200u;return 0;
        case 0x79d4u: /* cmp r7,#0x1f000 */
            (void)subtract(state,r[7],126976u);r[15]=0x79d8u;return 0;
        case 0x79d8u: /* movgt r7,#0x1f000 */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[7]=126976u;r[15]=0x79dcu;}else{r[15]=0x79dcu;}return 0;
        case 0x79dcu: /* mov r8,r7, lsr #0x1 */
            r[8]=(r[7]>>1);r[15]=0x79e0u;return 0;
        case 0x79e0u: /* str r8,[r11,#0xc] */
            STORE(r[11]+12u,r[8],4);r[15]=0x79e4u;return 0;
        case 0x79e4u: /* str r7,[r12,#0x34] */
            STORE(r[12]+52u,r[7],4);r[15]=0x79e8u;return 0;
        case 0x79e8u: /* ldr r7,[r12,#0x30] */
            LOAD(r[7],r[12]+48u,4);r[15]=0x79ecu;return 0;
        case 0x79ecu: /* str r7,[r12,#0x8] */
            STORE(r[12]+8u,r[7],4);r[15]=0x79f0u;return 0;
        case 0x79f0u: /* mov r6,r7, lsr #0x10 */
            r[6]=(r[7]>>16);r[15]=0x79f4u;return 0;
        case 0x79f4u: /* mov r8,#0x4200 */
            r[8]=16896u;r[15]=0x79f8u;return 0;
        case 0x79f8u: /* orr r8,r6,r8 */
            r[8]=r[6]|r[8];r[15]=0x79fcu;return 0;
        case 0x79fcu: /* orr r8,r8,r1, lsl #0x7 */
            r[8]=r[8]|(r[1]<<7);r[15]=0x7a00u;return 0;
        case 0x7a00u: /* str r8,[r11,#0x0] */
            STORE(r[11]+0u,r[8],4);r[15]=0x7a04u;return 0;
        case 0x7a04u: /* mov r8,#0x1f00 */
            r[8]=7936u;r[15]=0x7a08u;return 0;
        case 0x7a08u: /* orr r8,r8,#0xf8 */
            r[8]=r[8]|248u;r[15]=0x7a0cu;return 0;
        case 0x7a0cu: /* str r8,[r11,#0x2c] */
            STORE(r[11]+44u,r[8],4);r[15]=0x7a10u;return 0;
        case 0x7a10u: /* str r8,[r11,#0x30] */
            STORE(r[11]+48u,r[8],4);r[15]=0x7a14u;return 0;
        case 0x7a14u: /* str r8,[r11,#0x34] */
            STORE(r[11]+52u,r[8],4);r[15]=0x7a18u;return 0;
        case 0x7a18u: /* str r8,[r11,#0x38] */
            STORE(r[11]+56u,r[8],4);r[15]=0x7a1cu;return 0;
        case 0x7a1cu: /* str r8,[r11,#0x3c] */
            STORE(r[11]+60u,r[8],4);r[15]=0x7a20u;return 0;
        case 0x7a20u: /* ldr r8,[0x77e8] */
            LOAD(r[8],30696u,4);r[15]=0x7a24u;return 0;
        case 0x7a24u: /* and r7,r7,r8 */
            r[7]=r[7]&r[8];r[15]=0x7a28u;return 0;
        case 0x7a28u: /* str r7,[r11,#0x4] */
            STORE(r[11]+4u,r[7],4);r[15]=0x7a2cu;return 0;
        case 0x7a2cu: /* mov r6,#0x1f */
            r[6]=31u;r[15]=0x7a30u;return 0;
        case 0x7a30u: /* mov r7,#0x8000 */
            r[7]=32768u;r[15]=0x7a34u;return 0;
        case 0x7a34u: /* mov r8,#0x24 */
            r[8]=36u;r[15]=0x7a38u;return 0;
        case 0x7a38u: /* strb r8,[r11,#0x28] */
            STORE(r[11]+40u,r[8],1);r[15]=0x7a3cu;return 0;
        case 0x7a3cu: /* mov r8,#0x0 */
            r[8]=0u;r[15]=0x7a40u;return 0;
        case 0x7a40u: /* str r8,[r11,#0x8] */
            STORE(r[11]+8u,r[8],4);r[15]=0x7a44u;return 0;
        case 0x7a44u: /* str r6,[r11,#0x10] */
            STORE(r[11]+16u,r[6],4);r[15]=0x7a48u;return 0;
        case 0x7a48u: /* str r7,[r11,#0x1c] */
            STORE(r[11]+28u,r[7],4);r[15]=0x7a4cu;return 0;
        case 0x7a4cu: /* str r6,[r11,#0x40] */
            STORE(r[11]+64u,r[6],4);r[15]=0x7a50u;return 0;
        case 0x7a50u: /* str r6,[r11,#0x44] */
            STORE(r[11]+68u,r[6],4);r[15]=0x7a54u;return 0;
        case 0x7a54u: /* str r8,[r12,#0x4] */
            STORE(r[12]+4u,r[8],4);r[15]=0x7a58u;return 0;
        case 0x7a58u: /* str r8,[r12,#0x2c] */
            STORE(r[12]+44u,r[8],4);r[15]=0x7a5cu;return 0;
        case 0x7a5cu: /* strb r2,[r12,#0x28] */
            STORE(r[12]+40u,r[2],1);r[15]=0x7a60u;return 0;
        case 0x7a60u: /* sub r6,r6,#0xa */
            r[6]=r[6]-10u;r[15]=0x7a64u;return 0;
        case 0x7a64u: /* str r6,[r11,#0x14] */
            STORE(r[11]+20u,r[6],4);r[15]=0x7a68u;return 0;
        case 0x7a68u: /* bl 0x00007c8c */
            r[14]=0x7a6cu;r[15]=31884u;return 0;
        case 0x7a6cu: /* ldr r0,[r9,#0x10] */
            LOAD(r[0],r[9]+16u,4);r[15]=0x7a70u;return 0;
        case 0x7a70u: /* ldr r0,[r9,#0x14] */
            LOAD(r[0],r[9]+20u,4);r[15]=0x7a74u;return 0;
        case 0x7a74u: /* ldrb r2,[r12,#0x28] */
            LOAD(r[2],r[12]+40u,1);r[15]=0x7a78u;return 0;
        case 0x7a78u: /* ldrb r3,[r12,#0x24] */
            LOAD(r[3],r[12]+36u,1);r[15]=0x7a7cu;return 0;
        case 0x7a7cu: /* cmp r2,#0xf */
            (void)subtract(state,r[2],15u);r[15]=0x7a80u;return 0;
        case 0x7a80u: /* beq 0x00008150 */
            if(state->cpsr&0x40000000u){r[15]=33104u;}else{r[15]=0x7a84u;}return 0;
        case 0x7a84u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x7a88u;return 0;
        case 0x7a88u: /* mov r2,#0xf */
            r[2]=15u;r[15]=0x7a8cu;return 0;
        case 0x7a8cu: /* movhi r0,#0x3 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[0]=3u;}r[15]=0x7a90u;return 0;
        case 0x7a90u: /* bhi 0x00007964 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=31076u;}else{r[15]=0x7a94u;}return 0;
        case 0x7a94u: /* ldrb r6,[r12,#0x0] */
            LOAD(r[6],r[12]+0u,1);r[15]=0x7a98u;return 0;
        case 0x7a98u: /* tst r6,#0x1 */
            temporary=r[6]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7a9cu;return 0;
        case 0x7a9cu: /* beq 0x00008150 */
            if(state->cpsr&0x40000000u){r[15]=33104u;}else{r[15]=0x7aa0u;}return 0;
        case 0x7aa0u: /* bl 0x000081b8 */
            r[14]=0x7aa4u;r[15]=33208u;return 0;
        case 0x7aa4u: /* ldrb r3,[r12,#0x29] */
            LOAD(r[3],r[12]+41u,1);r[15]=0x7aa8u;return 0;
        case 0x7aa8u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7aacu;return 0;
        case 0x7aacu: /* bl 0x00008188 */
            r[14]=0x7ab0u;r[15]=33160u;return 0;
        case 0x7ab0u: /* strb r2,[r12,#0x28] */
            STORE(r[12]+40u,r[2],1);r[15]=0x7ab4u;return 0;
        case 0x7ab4u: /* strb r3,[r12,#0x29] */
            STORE(r[12]+41u,r[3],1);r[15]=0x7ab8u;return 0;
        case 0x7ab8u: /* bl 0x00008164 */
            r[14]=0x7abcu;r[15]=33124u;return 0;
        case 0x7abcu: /* b 0x00008150 */
            r[15]=33104u;return 0;
        case 0x7ac0u: /* ldrb r0,[r12,#0x0] */
            LOAD(r[0],r[12]+0u,1);r[15]=0x7ac4u;return 0;
        case 0x7ac4u: /* bic r0,r0,#0x1 */
            r[0]=r[0]&~1u;r[15]=0x7ac8u;return 0;
        case 0x7ac8u: /* strb r0,[r12,#0x0] */
            STORE(r[12]+0u,r[0],1);r[15]=0x7accu;return 0;
        case 0x7accu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x7ad0u;return 0;
        case 0x7ad0u: /* strb r0,[r10,#0x1] */
            STORE(r[10]+1u,r[0],1);r[15]=0x7ad4u;return 0;
        case 0x7ad4u: /* str r0,[r10,#0x4] */
            STORE(r[10]+4u,r[0],4);r[15]=0x7ad8u;return 0;
        case 0x7ad8u: /* mov r4,#0x1 */
            r[4]=1u;r[15]=0x7adcu;return 0;
        case 0x7adcu: /* b 0x0000813c */
            r[15]=33084u;return 0;
        case 0x7ae0u: /* ldrb r6,[r12,#0x0] */
            LOAD(r[6],r[12]+0u,1);r[15]=0x7ae4u;return 0;
        case 0x7ae4u: /* tst r6,#0x1 */
            temporary=r[6]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7ae8u;return 0;
        case 0x7ae8u: /* beq 0x00007b10 */
            if(state->cpsr&0x40000000u){r[15]=31504u;}else{r[15]=0x7aecu;}return 0;
        case 0x7aecu: /* ldrb r7,[r12,#0x2] */
            LOAD(r[7],r[12]+2u,1);r[15]=0x7af0u;return 0;
        case 0x7af0u: /* cmp r7,#0x0 */
            (void)subtract(state,r[7],0u);r[15]=0x7af4u;return 0;
        case 0x7af4u: /* beq 0x00007bcc */
            if(state->cpsr&0x40000000u){r[15]=31692u;}else{r[15]=0x7af8u;}return 0;
        case 0x7af8u: /* cmp r5,#0x0 */
            (void)subtract(state,r[5],0u);r[15]=0x7afcu;return 0;
        case 0x7afcu: /* beq 0x00007bcc */
            if(state->cpsr&0x40000000u){r[15]=31692u;}else{r[15]=0x7b00u;}return 0;
        case 0x7b00u: /* cmp r5,r7 */
            (void)subtract(state,r[5],r[7]);r[15]=0x7b04u;return 0;
        case 0x7b04u: /* bcs 0x00007bcc */
            if(state->cpsr&0x20000000u){r[15]=31692u;}else{r[15]=0x7b08u;}return 0;
        case 0x7b08u: /* mov r4,#0x2 */
            r[4]=2u;r[15]=0x7b0cu;return 0;
        case 0x7b0cu: /* b 0x0000813c */
            r[15]=33084u;return 0;
        case 0x7b10u: /* ldr r10,[0x77e4] */
            LOAD(r[10],30692u,4);r[15]=0x7b14u;return 0;
        case 0x7b14u: /* mov r9,#0x0 */
            r[9]=0u;r[15]=0x7b18u;return 0;
        case 0x7b18u: /* ldr r6,[r10,#0x4] */
            LOAD(r[6],r[10]+4u,4);r[15]=0x7b1cu;return 0;
        case 0x7b1cu: /* cmp r6,#0x0 */
            (void)subtract(state,r[6],0u);r[15]=0x7b20u;return 0;
        case 0x7b20u: /* ldreq r11,[r10,#0x8] */
            if(state->cpsr&0x40000000u){LOAD(r[11],r[10]+8u,4);}r[15]=0x7b24u;return 0;
        case 0x7b24u: /* beq 0x00007c1c */
            if(state->cpsr&0x40000000u){r[15]=31772u;}else{r[15]=0x7b28u;}return 0;
        case 0x7b28u: /* add r10,r10,#0xc */
            r[10]=r[10]+12u;r[15]=0x7b2cu;return 0;
        case 0x7b2cu: /* add r9,r9,#0x1 */
            r[9]=r[9]+1u;r[15]=0x7b30u;return 0;
        case 0x7b30u: /* cmp r9,#0x10 */
            (void)subtract(state,r[9],16u);r[15]=0x7b34u;return 0;
        case 0x7b34u: /* bne 0x00007b18 */
            if(!(state->cpsr&0x40000000u)){r[15]=31512u;}else{r[15]=0x7b38u;}return 0;
        case 0x7b38u: /* mov r6,#0xff */
            r[6]=255u;r[15]=0x7b3cu;return 0;
        case 0x7b3cu: /* mov r8,r10 */
            r[8]=r[10];r[15]=0x7b40u;return 0;
        case 0x7b40u: /* subs r9,r9,#0x1 */
            r[9]=subtract(state,r[9],1u);r[15]=0x7b44u;return 0;
        case 0x7b44u: /* bmi 0x00007b84 */
            if(state->cpsr&0x80000000u){r[15]=31620u;}else{r[15]=0x7b48u;}return 0;
        case 0x7b48u: /* sub r10,r10,#0xc */
            r[10]=r[10]-12u;r[15]=0x7b4cu;return 0;
        case 0x7b4cu: /* ldrb r7,[r10,#0x0] */
            LOAD(r[7],r[10]+0u,1);r[15]=0x7b50u;return 0;
        case 0x7b50u: /* cmp r7,#0x0 */
            (void)subtract(state,r[7],0u);r[15]=0x7b54u;return 0;
        case 0x7b54u: /* beq 0x00007bdc */
            if(state->cpsr&0x40000000u){r[15]=31708u;}else{r[15]=0x7b58u;}return 0;
        case 0x7b58u: /* cmp r6,r7 */
            (void)subtract(state,r[6],r[7]);r[15]=0x7b5cu;return 0;
        case 0x7b5cu: /* bhi 0x00007b78 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=31608u;}else{r[15]=0x7b60u;}return 0;
        case 0x7b60u: /* bcc 0x00007b40 */
            if(!(state->cpsr&0x20000000u)){r[15]=31552u;}else{r[15]=0x7b64u;}return 0;
        case 0x7b64u: /* ldr r11,[r8,#0x0] */
            LOAD(r[11],r[8]+0u,4);r[15]=0x7b68u;return 0;
        case 0x7b68u: /* ldr r7,[r10,#0x0] */
            LOAD(r[7],r[10]+0u,4);r[15]=0x7b6cu;return 0;
        case 0x7b6cu: /* cmp r7,r11 */
            (void)subtract(state,r[7],r[11]);r[15]=0x7b70u;return 0;
        case 0x7b70u: /* bhi 0x00007b40 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=31552u;}else{r[15]=0x7b74u;}return 0;
        case 0x7b74u: /* ldrb r7,[r10,#0x0] */
            LOAD(r[7],r[10]+0u,1);r[15]=0x7b78u;return 0;
        case 0x7b78u: /* mov r6,r7 */
            r[6]=r[7];r[15]=0x7b7cu;return 0;
        case 0x7b7cu: /* mov r8,r10 */
            r[8]=r[10];r[15]=0x7b80u;return 0;
        case 0x7b80u: /* b 0x00007b40 */
            r[15]=31552u;return 0;
        case 0x7b84u: /* cmp r5,#0x0 */
            (void)subtract(state,r[5],0u);r[15]=0x7b88u;return 0;
        case 0x7b88u: /* beq 0x00007bc4 */
            if(state->cpsr&0x40000000u){r[15]=31684u;}else{r[15]=0x7b8cu;}return 0;
        case 0x7b8cu: /* cmp r5,r6 */
            (void)subtract(state,r[5],r[6]);r[15]=0x7b90u;return 0;
        case 0x7b90u: /* bcc 0x00007ba8 */
            if(!(state->cpsr&0x20000000u)){r[15]=31656u;}else{r[15]=0x7b94u;}return 0;
        case 0x7b94u: /* bhi 0x00007bc4 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=31684u;}else{r[15]=0x7b98u;}return 0;
        case 0x7b98u: /* ldr r6,[r8,#0x0] */
            LOAD(r[6],r[8]+0u,4);r[15]=0x7b9cu;return 0;
        case 0x7b9cu: /* add r7,r5,r0 */
            r[7]=r[5]+r[0];r[15]=0x7ba0u;return 0;
        case 0x7ba0u: /* cmp r7,r6 */
            (void)subtract(state,r[7],r[6]);r[15]=0x7ba4u;return 0;
        case 0x7ba4u: /* bcs 0x00007bc4 */
            if(state->cpsr&0x20000000u){r[15]=31684u;}else{r[15]=0x7ba8u;}return 0;
        case 0x7ba8u: /* cmp r0,#0x3 */
            (void)subtract(state,r[0],3u);r[15]=0x7bacu;return 0;
        case 0x7bacu: /* bne 0x00007bbc */
            if(!(state->cpsr&0x40000000u)){r[15]=31676u;}else{r[15]=0x7bb0u;}return 0;
        case 0x7bb0u: /* sub r12,r12,#0x58 */
            r[12]=r[12]-88u;r[15]=0x7bb4u;return 0;
        case 0x7bb4u: /* bl 0x000081b8 */
            r[14]=0x7bb8u;r[15]=33208u;return 0;
        case 0x7bb8u: /* bl 0x00007e1c */
            r[14]=0x7bbcu;r[15]=32284u;return 0;
        case 0x7bbcu: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x7bc0u;return 0;
        case 0x7bc0u: /* b 0x0000813c */
            r[15]=33084u;return 0;
        case 0x7bc4u: /* mov r10,r8 */
            r[10]=r[8];r[15]=0x7bc8u;return 0;
        case 0x7bc8u: /* b 0x00007bdc */
            r[15]=31708u;return 0;
        case 0x7bccu: /* ldrb r9,[r12,#0x3] */
            LOAD(r[9],r[12]+3u,1);r[15]=0x7bd0u;return 0;
        case 0x7bd0u: /* ldr r10,[0x77e4] */
            LOAD(r[10],30692u,4);r[15]=0x7bd4u;return 0;
        case 0x7bd4u: /* mov r6,#0xc */
            r[6]=12u;r[15]=0x7bd8u;return 0;
        case 0x7bd8u: /* mla r10,r9,r6,r10 */
            r[10]=r[9]*r[6]+r[10];r[15]=0x7bdcu;return 0;
        case 0x7bdcu: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x7be0u;return 0;
        case 0x7be0u: /* stmdb sp!,{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr} */
            STORE(r[13]-56u,r[0],4);STORE(r[13]-52u,r[1],4);STORE(r[13]-48u,r[2],4);STORE(r[13]-44u,r[3],4);STORE(r[13]-40u,r[4],4);STORE(r[13]-36u,r[5],4);STORE(r[13]-32u,r[6],4);STORE(r[13]-28u,r[7],4);STORE(r[13]-24u,r[8],4);STORE(r[13]-20u,r[9],4);STORE(r[13]-16u,r[10],4);STORE(r[13]-12u,r[11],4);STORE(r[13]-8u,r[12],4);STORE(r[13]-4u,r[14],4);r[13]-=56u;r[15]=0x7be4u;return 0;
        case 0x7be4u: /* ldr r12,[r10,#0x4] */
            LOAD(r[12],r[10]+4u,4);r[15]=0x7be8u;return 0;
        case 0x7be8u: /* ldrb r9,[r12,#0x3] */
            LOAD(r[9],r[12]+3u,1);r[15]=0x7becu;return 0;
        case 0x7becu: /* ldrb r4,[r10,#0x1] */
            LOAD(r[4],r[10]+1u,1);r[15]=0x7bf0u;return 0;
        case 0x7bf0u: /* cmp r4,#0x1 */
            (void)subtract(state,r[4],1u);r[15]=0x7bf4u;return 0;
        case 0x7bf4u: /* beq 0x00007c00 */
            if(state->cpsr&0x40000000u){r[15]=31744u;}else{r[15]=0x7bf8u;}return 0;
        case 0x7bf8u: /* bl 0x00007e1c */
            r[14]=0x7bfcu;r[15]=32284u;return 0;
        case 0x7bfcu: /* b 0x00007c08 */
            r[15]=31752u;return 0;
        case 0x7c00u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7c04u;return 0;
        case 0x7c04u: /* bl 0x00007ec8 */
            r[14]=0x7c08u;r[15]=32456u;return 0;
        case 0x7c08u: /* ldr r1,[r11,#0x0] */
            LOAD(r[1],r[11]+0u,4);r[15]=0x7c0cu;return 0;
        case 0x7c0cu: /* bic r1,r1,#0x4000 */
            r[1]=r[1]&~16384u;r[15]=0x7c10u;return 0;
        case 0x7c10u: /* orr r1,r1,#0x8000 */
            r[1]=r[1]|32768u;r[15]=0x7c14u;return 0;
        case 0x7c14u: /* str r1,[r11,#0x0] */
            STORE(r[11]+0u,r[1],4);r[15]=0x7c18u;return 0;
        case 0x7c18u: /* ldmia sp!,{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[1],r[13]+4u,4);LOAD(r[2],r[13]+8u,4);LOAD(r[3],r[13]+12u,4);LOAD(r[4],r[13]+16u,4);LOAD(r[5],r[13]+20u,4);LOAD(r[6],r[13]+24u,4);LOAD(r[7],r[13]+28u,4);LOAD(r[8],r[13]+32u,4);LOAD(r[9],r[13]+36u,4);LOAD(r[10],r[13]+40u,4);LOAD(r[11],r[13]+44u,4);LOAD(r[12],r[13]+48u,4);LOAD(r[14],r[13]+52u,4);r[13]+=56u;r[15]=0x7c1cu;return 0;
        case 0x7c1cu: /* strb r5,[r12,#0x2] */
            STORE(r[12]+2u,r[5],1);r[15]=0x7c20u;return 0;
        case 0x7c20u: /* strb r9,[r12,#0x3] */
            STORE(r[12]+3u,r[9],1);r[15]=0x7c24u;return 0;
        case 0x7c24u: /* strb r5,[r10,#0x0] */
            STORE(r[10]+0u,r[5],1);r[15]=0x7c28u;return 0;
        case 0x7c28u: /* strb r0,[r10,#0x1] */
            STORE(r[10]+1u,r[0],1);r[15]=0x7c2cu;return 0;
        case 0x7c2cu: /* str r12,[r10,#0x4] */
            STORE(r[10]+4u,r[12],4);r[15]=0x7c30u;return 0;
        case 0x7c30u: /* ldrb r6,[r12,#0x0] */
            LOAD(r[6],r[12]+0u,1);r[15]=0x7c34u;return 0;
        case 0x7c34u: /* orr r6,r6,#0x1 */
            r[6]=r[6]|1u;r[15]=0x7c38u;return 0;
        case 0x7c38u: /* strb r6,[r12,#0x0] */
            STORE(r[12]+0u,r[6],1);r[15]=0x7c3cu;return 0;
        case 0x7c3cu: /* add r6,r9,#0x30 */
            r[6]=r[9]+48u;r[15]=0x7c40u;return 0;
        case 0x7c40u: /* ldr r9,[0x8248] */
            LOAD(r[9],33352u,4);r[15]=0x7c44u;return 0;
        case 0x7c44u: /* strb r6,[r9,#0xd] */
            STORE(r[9]+13u,r[6],1);r[15]=0x7c48u;return 0;
        case 0x7c48u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7c8cu: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7c90u;return 0;
        case 0x7c90u: /* str r6,[r12,#0x2c] */
            STORE(r[12]+44u,r[6],4);r[15]=0x7c94u;return 0;
        case 0x7c94u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7c98u;return 0;
        case 0x7c98u: /* ldrb r6,[r12,#0x1e] */
            LOAD(r[6],r[12]+30u,1);r[15]=0x7c9cu;return 0;
        case 0x7c9cu: /* mov r6,r6, lsl #0x1 */
            r[6]=(r[6]<<1);r[15]=0x7ca0u;return 0;
        case 0x7ca0u: /* rsb r6,r6,#0x100 */
            r[6]=256u-r[6];r[15]=0x7ca4u;return 0;
        case 0x7ca4u: /* add r3,r3,r6 */
            r[3]=r[3]+r[6];r[15]=0x7ca8u;return 0;
        case 0x7ca8u: /* cmp r3,#0x0 */
            (void)subtract(state,r[3],0u);r[15]=0x7cacu;return 0;
        case 0x7cacu: /* movmi r3,#0x0 */
            if(state->cpsr&0x80000000u){r[3]=0u;}r[15]=0x7cb0u;return 0;
        case 0x7cb0u: /* cmp r3,#0xff */
            (void)subtract(state,r[3],255u);r[15]=0x7cb4u;return 0;
        case 0x7cb4u: /* movgt r3,#0xff */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[3]=255u;r[15]=0x7cb8u;}else{r[15]=0x7cb8u;}return 0;
        case 0x7cb8u: /* strb r3,[r11,#0x29] */
            STORE(r[11]+41u,r[3],1);r[15]=0x7cbcu;return 0;
        case 0x7cbcu: /* strb r3,[r12,#0x20] */
            STORE(r[12]+32u,r[3],1);r[15]=0x7cc0u;return 0;
        case 0x7cc0u: /* ldrb r6,[r12,#0x14] */
            LOAD(r[6],r[12]+20u,1);r[15]=0x7cc4u;return 0;
        case 0x7cc4u: /* cmp r6,#0x0 */
            (void)subtract(state,r[6],0u);r[15]=0x7cc8u;return 0;
        case 0x7cc8u: /* bne 0x00007cd8 */
            if(!(state->cpsr&0x40000000u)){r[15]=31960u;}else{r[15]=0x7cccu;}return 0;
        case 0x7cccu: /* ldrb r6,[r12,#0x0] */
            LOAD(r[6],r[12]+0u,1);r[15]=0x7cd0u;return 0;
        case 0x7cd0u: /* bic r6,r6,#0x4 */
            r[6]=r[6]&~4u;r[15]=0x7cd4u;return 0;
        case 0x7cd4u: /* strb r6,[r12,#0x0] */
            STORE(r[12]+0u,r[6],1);r[15]=0x7cd8u;return 0;
        case 0x7cd8u: /* ldrb r6,[r12,#0x1f] */
            LOAD(r[6],r[12]+31u,1);r[15]=0x7cdcu;return 0;
        case 0x7cdcu: /* mov r6,r6, lsr #0x2 */
            r[6]=(r[6]>>2);r[15]=0x7ce0u;return 0;
        case 0x7ce0u: /* sub r6,r6,#0x20 */
            r[6]=r[6]-32u;r[15]=0x7ce4u;return 0;
        case 0x7ce4u: /* ldr r7,[0x824c] */
            LOAD(r[7],33356u,4);r[15]=0x7ce8u;return 0;
        case 0x7ce8u: /* ldrb r2,[r7,r2] */
            LOAD(r[2],r[7]+r[2],1);r[15]=0x7cecu;return 0;
        case 0x7cecu: /* add r2,r2,r6 */
            r[2]=r[2]+r[6];r[15]=0x7cf0u;return 0;
        case 0x7cf0u: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x7cf4u;return 0;
        case 0x7cf4u: /* movmi r2,#0x0 */
            if(state->cpsr&0x80000000u){r[2]=0u;}r[15]=0x7cf8u;return 0;
        case 0x7cf8u: /* cmp r2,#0x1f */
            (void)subtract(state,r[2],31u);r[15]=0x7cfcu;return 0;
        case 0x7cfcu: /* movgt r2,#0x1f */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[2]=31u;r[15]=0x7d00u;}else{r[15]=0x7d00u;}return 0;
        case 0x7d00u: /* ldr r7,[0x8250] */
            LOAD(r[7],33360u,4);r[15]=0x7d04u;return 0;
        case 0x7d04u: /* ldrb r2,[r7,r2] */
            LOAD(r[2],r[7]+r[2],1);r[15]=0x7d08u;return 0;
        case 0x7d08u: /* strb r2,[r11,#0x24] */
            STORE(r[11]+36u,r[2],1);r[15]=0x7d0cu;return 0;
        case 0x7d0cu: /* strb r2,[r12,#0x21] */
            STORE(r[12]+33u,r[2],1);r[15]=0x7d10u;return 0;
        case 0x7d10u: /* ldrb r6,[r12,#0x16] */
            LOAD(r[6],r[12]+22u,1);r[15]=0x7d14u;return 0;
        case 0x7d14u: /* cmp r6,#0x0 */
            (void)subtract(state,r[6],0u);r[15]=0x7d18u;return 0;
        case 0x7d18u: /* bne 0x00007d28 */
            if(!(state->cpsr&0x40000000u)){r[15]=32040u;}else{r[15]=0x7d1cu;}return 0;
        case 0x7d1cu: /* ldrb r6,[r12,#0x0] */
            LOAD(r[6],r[12]+0u,1);r[15]=0x7d20u;return 0;
        case 0x7d20u: /* bic r6,r6,#0x10 */
            r[6]=r[6]&~16u;r[15]=0x7d24u;return 0;
        case 0x7d24u: /* strb r6,[r12,#0x0] */
            STORE(r[12]+0u,r[6],1);r[15]=0x7d28u;return 0;
        case 0x7d28u: /* bl 0x00007d64 */
            r[14]=0x7d2cu;r[15]=32100u;return 0;
        case 0x7d2cu: /* ldrb r7,[r12,#0x1b] */
            LOAD(r[7],r[12]+27u,1);r[15]=0x7d30u;return 0;
        case 0x7d30u: /* ldrb r6,[r12,#0x2a] */
            LOAD(r[6],r[12]+42u,1);r[15]=0x7d34u;return 0;
        case 0x7d34u: /* bl 0x0000705c */
            r[14]=0x7d38u;r[15]=28764u;return 0;
        case 0x7d38u: /* strb r7,[r12,#0x2b] */
            STORE(r[12]+43u,r[7],1);r[15]=0x7d3cu;return 0;
        case 0x7d3cu: /* strb r7,[r11,#0x25] */
            STORE(r[11]+37u,r[7],1);r[15]=0x7d40u;return 0;
        case 0x7d40u: /* ldrb r6,[r12,#0x29] */
            LOAD(r[6],r[12]+41u,1);r[15]=0x7d44u;return 0;
        case 0x7d44u: /* tst r6,#0x40 */
            temporary=r[6]&64u;logic_flags(state,temporary,0,64u);r[15]=0x7d48u;return 0;
        case 0x7d48u: /* ldrbeq r6,[r12,#0x25] */
            if(state->cpsr&0x40000000u){LOAD(r[6],r[12]+37u,1);}r[15]=0x7d4cu;return 0;
        case 0x7d4cu: /* strb r6,[r11,#0x20] */
            STORE(r[11]+32u,r[6],1);r[15]=0x7d50u;return 0;
        case 0x7d50u: /* mov r8,#0x1 */
            r[8]=1u;r[15]=0x7d54u;return 0;
        case 0x7d54u: /* ldrbeq r6,[r12,#0x26] */
            if(state->cpsr&0x40000000u){LOAD(r[6],r[12]+38u,1);}r[15]=0x7d58u;return 0;
        case 0x7d58u: /* ldrbne r6,[r12,#0x22] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[6],r[12]+34u,1);}r[15]=0x7d5cu;return 0;
        case 0x7d5cu: /* ldrb r7,[r12,#0x1a] */
            LOAD(r[7],r[12]+26u,1);r[15]=0x7d60u;return 0;
        case 0x7d60u: /* b 0x00006fc0 */
            r[15]=28608u;return 0;
        case 0x7dc0u: /* ldr r12,[0x7804] */
            LOAD(r[12],30724u,4);r[15]=0x7dc4u;return 0;
        case 0x7dc4u: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x7dc8u;return 0;
        case 0x7dc8u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x7dccu;return 0;
        case 0x7dccu: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x7dd0u;return 0;
        case 0x7dd0u: /* ldrb r2,[r12,#0x0] */
            LOAD(r[2],r[12]+0u,1);r[15]=0x7dd4u;return 0;
        case 0x7dd4u: /* tst r2,#0x1 */
            temporary=r[2]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7dd8u;return 0;
        case 0x7dd8u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7ddcu;}return 0;
        case 0x7ddcu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7de0u;return 0;
        case 0x7de0u: /* bl 0x000081b8 */
            r[14]=0x7de4u;r[15]=33208u;return 0;
        case 0x7de4u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7de8u;return 0;
        case 0x7de8u: /* bl 0x00007ec8 */
            r[14]=0x7decu;r[15]=32456u;return 0;
        case 0x7decu: /* b 0x00008164 */
            r[15]=33124u;return 0;
        case 0x7df0u: /* ldr r12,[0x77f8] */
            LOAD(r[12],30712u,4);r[15]=0x7df4u;return 0;
        case 0x7df4u: /* ldr r7,[0x77fc] */
            LOAD(r[7],30716u,4);r[15]=0x7df8u;return 0;
        case 0x7df8u: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x7dfcu;return 0;
        case 0x7dfcu: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x7e00u;return 0;
        case 0x7e00u: /* ldrb r2,[r12,#0x0] */
            LOAD(r[2],r[12]+0u,1);r[15]=0x7e04u;return 0;
        case 0x7e04u: /* tst r2,#0x1 */
            temporary=r[2]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7e08u;return 0;
        case 0x7e08u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7e0cu;}return 0;
        case 0x7e0cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7e10u;return 0;
        case 0x7e10u: /* bl 0x000081b8 */
            r[14]=0x7e14u;r[15]=33208u;return 0;
        case 0x7e14u: /* bl 0x00007e1c */
            r[14]=0x7e18u;r[15]=32284u;return 0;
        case 0x7e18u: /* b 0x00008164 */
            r[15]=33124u;return 0;
        case 0x7e1cu: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7e20u;return 0;
        case 0x7e20u: /* mov r7,#0xf */
            r[7]=15u;r[15]=0x7e24u;return 0;
        case 0x7e24u: /* mov r3,#0x1f */
            r[3]=31u;r[15]=0x7e28u;return 0;
        case 0x7e28u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7e2cu;return 0;
        case 0x7e2cu: /* ldrb r8,[r12,#0x29] */
            LOAD(r[8],r[12]+41u,1);r[15]=0x7e30u;return 0;
        case 0x7e30u: /* bl 0x00008188 */
            r[14]=0x7e34u;r[15]=33160u;return 0;
        case 0x7e34u: /* strb r8,[r12,#0x29] */
            STORE(r[12]+41u,r[8],1);r[15]=0x7e38u;return 0;
        case 0x7e38u: /* ldr r8,[r11,#0x0] */
            LOAD(r[8],r[11]+0u,4);r[15]=0x7e3cu;return 0;
        case 0x7e3cu: /* bic r8,r8,#0x4000 */
            r[8]=r[8]&~16384u;r[15]=0x7e40u;return 0;
        case 0x7e40u: /* str r8,[r11,#0x0] */
            STORE(r[11]+0u,r[8],4);r[15]=0x7e44u;return 0;
        case 0x7e44u: /* strb r6,[r12,#0x23] */
            STORE(r[12]+35u,r[6],1);r[15]=0x7e48u;return 0;
        case 0x7e48u: /* str r6,[r12,#0x30] */
            STORE(r[12]+48u,r[6],4);r[15]=0x7e4cu;return 0;
        case 0x7e4cu: /* str r6,[r12,#0x34] */
            STORE(r[12]+52u,r[6],4);r[15]=0x7e50u;return 0;
        case 0x7e50u: /* strb r3,[r12,#0x28] */
            STORE(r[12]+40u,r[3],1);r[15]=0x7e54u;return 0;
        case 0x7e54u: /* strb r7,[r12,#0x2a] */
            STORE(r[12]+42u,r[7],1);r[15]=0x7e58u;return 0;
        case 0x7e58u: /* ldrb r8,[r12,#0x0] */
            LOAD(r[8],r[12]+0u,1);r[15]=0x7e5cu;return 0;
        case 0x7e5cu: /* bic r8,r8,#0x1 */
            r[8]=r[8]&~1u;r[15]=0x7e60u;return 0;
        case 0x7e60u: /* strb r8,[r12,#0x0] */
            STORE(r[12]+0u,r[8],1);r[15]=0x7e64u;return 0;
        case 0x7e64u: /* cmp r3,#0xf */
            (void)subtract(state,r[3],15u);r[15]=0x7e68u;return 0;
        case 0x7e68u: /* beq 0x00007e88 */
            if(state->cpsr&0x40000000u){r[15]=32392u;}else{r[15]=0x7e6cu;}return 0;
        case 0x7e6cu: /* mov r3,#0xf */
            r[3]=15u;r[15]=0x7e70u;return 0;
        case 0x7e70u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x7e74u;return 0;
        case 0x7e74u: /* ldrb r4,[r12,#0x0] */
            LOAD(r[4],r[12]+0u,1);r[15]=0x7e78u;return 0;
        case 0x7e78u: /* tst r4,#0x1 */
            temporary=r[4]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7e7cu;return 0;
        case 0x7e7cu: /* beq 0x00007e88 */
            if(state->cpsr&0x40000000u){r[15]=32392u;}else{r[15]=0x7e80u;}return 0;
        case 0x7e80u: /* bl 0x000081b8 */
            r[14]=0x7e84u;r[15]=33208u;return 0;
        case 0x7e84u: /* b 0x00007e2c */
            r[15]=32300u;return 0;
        case 0x7e88u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x7e8cu;return 0;
        case 0x7e8cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7e90u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7e94u;return 0;
        case 0x7e94u: /* ldr r10,[0x77e4] */
            LOAD(r[10],30692u,4);r[15]=0x7e98u;return 0;
        case 0x7e98u: /* mov r9,#0x10 */
            r[9]=16u;r[15]=0x7e9cu;return 0;
        case 0x7e9cu: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7ea0u;return 0;
        case 0x7ea0u: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x7ea4u;return 0;
        case 0x7ea4u: /* ldrb r0,[r10,#0x1] */
            LOAD(r[0],r[10]+1u,1);r[15]=0x7ea8u;return 0;
        case 0x7ea8u: /* cmp r0,#0x1 */
            (void)subtract(state,r[0],1u);r[15]=0x7eacu;return 0;
        case 0x7eacu: /* bne 0x00007eb8 */
            if(!(state->cpsr&0x40000000u)){r[15]=32440u;}else{r[15]=0x7eb0u;}return 0;
        case 0x7eb0u: /* ldr r12,[r10,#0x4] */
            LOAD(r[12],r[10]+4u,4);r[15]=0x7eb4u;return 0;
        case 0x7eb4u: /* bl 0x00007ec8 */
            r[14]=0x7eb8u;r[15]=32456u;return 0;
        case 0x7eb8u: /* subs r9,r9,#0x1 */
            r[9]=subtract(state,r[9],1u);r[15]=0x7ebcu;return 0;
        case 0x7ebcu: /* beq 0x00008164 */
            if(state->cpsr&0x40000000u){r[15]=33124u;}else{r[15]=0x7ec0u;}return 0;
        case 0x7ec0u: /* add r10,r10,#0xc */
            r[10]=r[10]+12u;r[15]=0x7ec4u;return 0;
        case 0x7ec4u: /* b 0x00007ea0 */
            r[15]=32416u;return 0;
        case 0x7ec8u: /* ldrb r8,[r12,#0x29] */
            LOAD(r[8],r[12]+41u,1);r[15]=0x7eccu;return 0;
        case 0x7eccu: /* ldrb r7,[r12,#0x0] */
            LOAD(r[7],r[12]+0u,1);r[15]=0x7ed0u;return 0;
        case 0x7ed0u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7ed4u;return 0;
        case 0x7ed4u: /* bl 0x0000817c */
            r[14]=0x7ed8u;r[15]=33148u;return 0;
        case 0x7ed8u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x7edcu;return 0;
        case 0x7edcu: /* bic r7,r7,#0x1 */
            r[7]=r[7]&~1u;r[15]=0x7ee0u;return 0;
        case 0x7ee0u: /* strb r7,[r12,#0x0] */
            STORE(r[12]+0u,r[7],1);r[15]=0x7ee4u;return 0;
        case 0x7ee4u: /* strb r8,[r12,#0x29] */
            STORE(r[12]+41u,r[8],1);r[15]=0x7ee8u;return 0;
        case 0x7ee8u: /* ldr r7,[r11,#0x0] */
            LOAD(r[7],r[11]+0u,4);r[15]=0x7eecu;return 0;
        case 0x7eecu: /* bic r7,r7,#0x4000 */
            r[7]=r[7]&~16384u;r[15]=0x7ef0u;return 0;
        case 0x7ef0u: /* str r7,[r11,#0x0] */
            STORE(r[11]+0u,r[7],4);r[15]=0x7ef4u;return 0;
        case 0x7ef4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7ef8u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7efcu;return 0;
        case 0x7efcu: /* ldr r10,[0x77e4] */
            LOAD(r[10],30692u,4);r[15]=0x7f00u;return 0;
        case 0x7f00u: /* mov r9,#0x10 */
            r[9]=16u;r[15]=0x7f04u;return 0;
        case 0x7f04u: /* mov r3,#0x1f */
            r[3]=31u;r[15]=0x7f08u;return 0;
        case 0x7f08u: /* mov r7,#0xf */
            r[7]=15u;r[15]=0x7f0cu;return 0;
        case 0x7f0cu: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7f10u;return 0;
        case 0x7f10u: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x7f14u;return 0;
        case 0x7f14u: /* ldrb r0,[r10,#0x1] */
            LOAD(r[0],r[10]+1u,1);r[15]=0x7f18u;return 0;
        case 0x7f18u: /* cmp r0,#0x2 */
            (void)subtract(state,r[0],2u);r[15]=0x7f1cu;return 0;
        case 0x7f1cu: /* moveq r3,#0x1f */
            if(state->cpsr&0x40000000u){r[3]=31u;}r[15]=0x7f20u;return 0;
        case 0x7f20u: /* movhi r3,#0xf */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[3]=15u;}r[15]=0x7f24u;return 0;
        case 0x7f24u: /* bcc 0x00007f64 */
            if(!(state->cpsr&0x20000000u)){r[15]=32612u;}else{r[15]=0x7f28u;}return 0;
        case 0x7f28u: /* ldr r12,[r10,#0x4] */
            LOAD(r[12],r[10]+4u,4);r[15]=0x7f2cu;return 0;
        case 0x7f2cu: /* ldrb r8,[r12,#0x29] */
            LOAD(r[8],r[12]+41u,1);r[15]=0x7f30u;return 0;
        case 0x7f30u: /* bl 0x00008188 */
            r[14]=0x7f34u;r[15]=33160u;return 0;
        case 0x7f34u: /* strb r8,[r12,#0x29] */
            STORE(r[12]+41u,r[8],1);r[15]=0x7f38u;return 0;
        case 0x7f38u: /* strb r7,[r12,#0x2a] */
            STORE(r[12]+42u,r[7],1);r[15]=0x7f3cu;return 0;
        case 0x7f3cu: /* ldrb r8,[r12,#0x0] */
            LOAD(r[8],r[12]+0u,1);r[15]=0x7f40u;return 0;
        case 0x7f40u: /* bic r8,r8,#0x1 */
            r[8]=r[8]&~1u;r[15]=0x7f44u;return 0;
        case 0x7f44u: /* strb r8,[r12,#0x0] */
            STORE(r[12]+0u,r[8],1);r[15]=0x7f48u;return 0;
        case 0x7f48u: /* ldr r8,[r11,#0x0] */
            LOAD(r[8],r[11]+0u,4);r[15]=0x7f4cu;return 0;
        case 0x7f4cu: /* bic r8,r8,#0x4000 */
            r[8]=r[8]&~16384u;r[15]=0x7f50u;return 0;
        case 0x7f50u: /* str r8,[r11,#0x0] */
            STORE(r[11]+0u,r[8],4);r[15]=0x7f54u;return 0;
        case 0x7f54u: /* strb r6,[r12,#0x23] */
            STORE(r[12]+35u,r[6],1);r[15]=0x7f58u;return 0;
        case 0x7f58u: /* str r6,[r12,#0x30] */
            STORE(r[12]+48u,r[6],4);r[15]=0x7f5cu;return 0;
        case 0x7f5cu: /* str r6,[r12,#0x34] */
            STORE(r[12]+52u,r[6],4);r[15]=0x7f60u;return 0;
        case 0x7f60u: /* strb r3,[r12,#0x28] */
            STORE(r[12]+40u,r[3],1);r[15]=0x7f64u;return 0;
        case 0x7f64u: /* subs r9,r9,#0x1 */
            r[9]=subtract(state,r[9],1u);r[15]=0x7f68u;return 0;
        case 0x7f68u: /* beq 0x00008164 */
            if(state->cpsr&0x40000000u){r[15]=33124u;}else{r[15]=0x7f6cu;}return 0;
        case 0x7f6cu: /* add r10,r10,#0xc */
            r[10]=r[10]+12u;r[15]=0x7f70u;return 0;
        case 0x7f70u: /* b 0x00007f10 */
            r[15]=32528u;return 0;
        case 0x7f74u: /* ldr r12,[0x7804] */
            LOAD(r[12],30724u,4);r[15]=0x7f78u;return 0;
        case 0x7f78u: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x7f7cu;return 0;
        case 0x7f7cu: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x7f80u;return 0;
        case 0x7f80u: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x7f84u;return 0;
        case 0x7f84u: /* ldrb r1,[r12,#0x0] */
            LOAD(r[1],r[12]+0u,1);r[15]=0x7f88u;return 0;
        case 0x7f88u: /* orr r5,r1,#0x2 */
            r[5]=r[1]|2u;r[15]=0x7f8cu;return 0;
        case 0x7f8cu: /* strb r5,[r12,#0x0] */
            STORE(r[12]+0u,r[5],1);r[15]=0x7f90u;return 0;
        case 0x7f90u: /* tst r1,#0x1 */
            temporary=r[1]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7f94u;return 0;
        case 0x7f94u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7f98u;}return 0;
        case 0x7f98u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7f9cu;return 0;
        case 0x7f9cu: /* bl 0x000081b8 */
            r[14]=0x7fa0u;r[15]=33208u;return 0;
        case 0x7fa0u: /* ldr r1,[r12,#0x38] */
            LOAD(r[1],r[12]+56u,4);r[15]=0x7fa4u;return 0;
        case 0x7fa4u: /* cmp r1,#0x0 */
            (void)subtract(state,r[1],0u);r[15]=0x7fa8u;return 0;
        case 0x7fa8u: /* beq 0x00008164 */
            if(state->cpsr&0x40000000u){r[15]=33124u;}else{r[15]=0x7facu;}return 0;
        case 0x7facu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x7fb0u;return 0;
        case 0x7fb0u: /* bne 0x00008164 */
            if(!(state->cpsr&0x40000000u)){r[15]=33124u;}else{r[15]=0x7fb4u;}return 0;
        case 0x7fb4u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7fb8u;return 0;
        case 0x7fb8u: /* ldrb r2,[r12,#0x29] */
            LOAD(r[2],r[12]+41u,1);r[15]=0x7fbcu;return 0;
        case 0x7fbcu: /* bl 0x0000817c */
            r[14]=0x7fc0u;r[15]=33148u;return 0;
        case 0x7fc0u: /* bic r5,r5,#0x1 */
            r[5]=r[5]&~1u;r[15]=0x7fc4u;return 0;
        case 0x7fc4u: /* strb r5,[r12,#0x0] */
            STORE(r[12]+0u,r[5],1);r[15]=0x7fc8u;return 0;
        case 0x7fc8u: /* strb r2,[r12,#0x29] */
            STORE(r[12]+41u,r[2],1);r[15]=0x7fccu;return 0;
        case 0x7fccu: /* b 0x00008164 */
            r[15]=33124u;return 0;
        case 0x7fd0u: /* ldr r12,[0x7804] */
            LOAD(r[12],30724u,4);r[15]=0x7fd4u;return 0;
        case 0x7fd4u: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x7fd8u;return 0;
        case 0x7fd8u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x7fdcu;return 0;
        case 0x7fdcu: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x7fe0u;return 0;
        case 0x7fe0u: /* ldrb r1,[r12,#0x0] */
            LOAD(r[1],r[12]+0u,1);r[15]=0x7fe4u;return 0;
        case 0x7fe4u: /* tst r1,#0x2 */
            temporary=r[1]&2u;logic_flags(state,temporary,0,2u);r[15]=0x7fe8u;return 0;
        case 0x7fe8u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7fecu;}return 0;
        case 0x7fecu: /* bic r1,r1,#0x2 */
            r[1]=r[1]&~2u;r[15]=0x7ff0u;return 0;
        case 0x7ff0u: /* strb r1,[r12,#0x0] */
            STORE(r[12]+0u,r[1],1);r[15]=0x7ff4u;return 0;
        case 0x7ff4u: /* tst r1,#0x1 */
            temporary=r[1]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7ff8u;return 0;
        case 0x7ff8u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7ffcu;}return 0;
        case 0x7ffcu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x8000u;return 0;
        case 0x8000u: /* bl 0x000081b8 */
            r[14]=0x8004u;r[15]=33208u;return 0;
        case 0x8004u: /* b 0x00008150 */
            r[15]=33104u;return 0;
        case 0x8008u: /* ldr r12,[0x7804] */
            LOAD(r[12],30724u,4);r[15]=0x800cu;return 0;
        case 0x800cu: /* mov r7,#0x10 */
            r[7]=16u;r[15]=0x8010u;return 0;
        case 0x8010u: /* b 0x00008028 */
            r[15]=32808u;return 0;
        case 0x8014u: /* ldr r12,[0x7804] */
            LOAD(r[12],30724u,4);r[15]=0x8018u;return 0;
        case 0x8018u: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x801cu;return 0;
        case 0x801cu: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x8020u;return 0;
        case 0x8020u: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x8024u;return 0;
        case 0x8024u: /* mov r7,#0x1 */
            r[7]=1u;r[15]=0x8028u;return 0;
        case 0x8028u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x802cu;return 0;
        case 0x802cu: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x8030u;return 0;
        case 0x8030u: /* str r6,[r12,#0x34] */
            STORE(r[12]+52u,r[6],4);r[15]=0x8034u;return 0;
        case 0x8034u: /* strb r6,[r12,#0x23] */
            STORE(r[12]+35u,r[6],1);r[15]=0x8038u;return 0;
        case 0x8038u: /* ldrb r1,[r12,#0x0] */
            LOAD(r[1],r[12]+0u,1);r[15]=0x803cu;return 0;
        case 0x803cu: /* tst r1,#0x1 */
            temporary=r[1]&1u;logic_flags(state,temporary,0,1u);r[15]=0x8040u;return 0;
        case 0x8040u: /* beq 0x00008050 */
            if(state->cpsr&0x40000000u){r[15]=32848u;}else{r[15]=0x8044u;}return 0;
        case 0x8044u: /* bl 0x000081b8 */
            r[14]=0x8048u;r[15]=33208u;return 0;
        case 0x8048u: /* bl 0x000080a0 */
            r[14]=0x804cu;r[15]=32928u;return 0;
        case 0x804cu: /* b 0x00008030 */
            r[15]=32816u;return 0;
        case 0x8050u: /* bl 0x000080d4 */
            r[14]=0x8054u;r[15]=32980u;return 0;
        case 0x8054u: /* b 0x00008030 */
            r[15]=32816u;return 0;
        case 0x8058u: /* ldr r12,[0x77f8] */
            LOAD(r[12],30712u,4);r[15]=0x805cu;return 0;
        case 0x805cu: /* mov r7,#0x10 */
            r[7]=16u;r[15]=0x8060u;return 0;
        case 0x8060u: /* b 0x00008078 */
            r[15]=32888u;return 0;
        case 0x8064u: /* ldr r12,[0x77f8] */
            LOAD(r[12],30712u,4);r[15]=0x8068u;return 0;
        case 0x8068u: /* ldr r7,[0x77fc] */
            LOAD(r[7],30716u,4);r[15]=0x806cu;return 0;
        case 0x806cu: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x8070u;return 0;
        case 0x8070u: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x8074u;return 0;
        case 0x8074u: /* mov r7,#0x2 */
            r[7]=2u;r[15]=0x8078u;return 0;
        case 0x8078u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x807cu;return 0;
        case 0x807cu: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x8080u;return 0;
        case 0x8080u: /* ldrb r1,[r12,#0x0] */
            LOAD(r[1],r[12]+0u,1);r[15]=0x8084u;return 0;
        case 0x8084u: /* tst r1,#0x1 */
            temporary=r[1]&1u;logic_flags(state,temporary,0,1u);r[15]=0x8088u;return 0;
        case 0x8088u: /* beq 0x00008098 */
            if(state->cpsr&0x40000000u){r[15]=32920u;}else{r[15]=0x808cu;}return 0;
        case 0x808cu: /* bl 0x000081b8 */
            r[14]=0x8090u;r[15]=33208u;return 0;
        case 0x8090u: /* bl 0x000080a0 */
            r[14]=0x8094u;r[15]=32928u;return 0;
        case 0x8094u: /* b 0x00008080 */
            r[15]=32896u;return 0;
        case 0x8098u: /* bl 0x000080d4 */
            r[14]=0x809cu;r[15]=32980u;return 0;
        case 0x809cu: /* b 0x00008080 */
            r[15]=32896u;return 0;
        case 0x80a0u: /* ldrb r1,[r12,#0x24] */
            LOAD(r[1],r[12]+36u,1);r[15]=0x80a4u;return 0;
        case 0x80a4u: /* strb r1,[r11,#0x29] */
            STORE(r[11]+41u,r[1],1);r[15]=0x80a8u;return 0;
        case 0x80a8u: /* strb r1,[r12,#0x20] */
            STORE(r[12]+32u,r[1],1);r[15]=0x80acu;return 0;
        case 0x80acu: /* ldrb r1,[r12,#0x28] */
            LOAD(r[1],r[12]+40u,1);r[15]=0x80b0u;return 0;
        case 0x80b0u: /* strb r1,[r11,#0x24] */
            STORE(r[11]+36u,r[1],1);r[15]=0x80b4u;return 0;
        case 0x80b4u: /* strb r1,[r12,#0x21] */
            STORE(r[12]+33u,r[1],1);r[15]=0x80b8u;return 0;
        case 0x80b8u: /* ldrb r1,[r12,#0x26] */
            LOAD(r[1],r[12]+38u,1);r[15]=0x80bcu;return 0;
        case 0x80bcu: /* ldrb r2,[r11,#0x20] */
            LOAD(r[2],r[11]+32u,1);r[15]=0x80c0u;return 0;
        case 0x80c0u: /* and r2,r2,#0xf */
            r[2]=r[2]&15u;r[15]=0x80c4u;return 0;
        case 0x80c4u: /* orr r2,r2,r1, lsl #0x4 */
            r[2]=r[2]|(r[1]<<4);r[15]=0x80c8u;return 0;
        case 0x80c8u: /* strb r2,[r11,#0x20] */
            STORE(r[11]+32u,r[2],1);r[15]=0x80ccu;return 0;
        case 0x80ccu: /* ldrb r1,[r12,#0x2a] */
            LOAD(r[1],r[12]+42u,1);r[15]=0x80d0u;return 0;
        case 0x80d0u: /* strb r1,[r11,#0x25] */
            STORE(r[11]+37u,r[1],1);r[15]=0x80d4u;return 0;
        case 0x80d4u: /* ldr r9,[0x77f4] */
            LOAD(r[9],30708u,4);r[15]=0x80d8u;return 0;
        case 0x80d8u: /* str r6,[r12,#0x40] */
            STORE(r[12]+64u,r[6],4);r[15]=0x80dcu;return 0;
        case 0x80dcu: /* str r6,[r12,#0x44] */
            STORE(r[12]+68u,r[6],4);r[15]=0x80e0u;return 0;
        case 0x80e0u: /* str r6,[r12,#0x4c] */
            STORE(r[12]+76u,r[6],4);r[15]=0x80e4u;return 0;
        case 0x80e4u: /* str r6,[r12,#0x50] */
            STORE(r[12]+80u,r[6],4);r[15]=0x80e8u;return 0;
        case 0x80e8u: /* str r6,[r12,#0x48] */
            STORE(r[12]+72u,r[6],4);r[15]=0x80ecu;return 0;
        case 0x80ecu: /* str r6,[r12,#0x54] */
            STORE(r[12]+84u,r[6],4);r[15]=0x80f0u;return 0;
        case 0x80f0u: /* str r6,[r12,#0x10] */
            STORE(r[12]+16u,r[6],4);r[15]=0x80f4u;return 0;
        case 0x80f4u: /* str r6,[r12,#0x14] */
            STORE(r[12]+20u,r[6],4);r[15]=0x80f8u;return 0;
        case 0x80f8u: /* str r6,[r12,#0x1c] */
            STORE(r[12]+28u,r[6],4);r[15]=0x80fcu;return 0;
        case 0x80fcu: /* str r9,[r12,#0x18] */
            STORE(r[12]+24u,r[9],4);r[15]=0x8100u;return 0;
        case 0x8100u: /* strb r9,[r12,#0x29] */
            STORE(r[12]+41u,r[9],1);r[15]=0x8104u;return 0;
        case 0x8104u: /* strb r9,[r12,#0x1f] */
            STORE(r[12]+31u,r[9],1);r[15]=0x8108u;return 0;
        case 0x8108u: /* strb r9,[r12,#0x1e] */
            STORE(r[12]+30u,r[9],1);r[15]=0x810cu;return 0;
        case 0x810cu: /* strb r9,[r12,#0x41] */
            STORE(r[12]+65u,r[9],1);r[15]=0x8110u;return 0;
        case 0x8110u: /* strb r9,[r12,#0x45] */
            STORE(r[12]+69u,r[9],1);r[15]=0x8114u;return 0;
        case 0x8114u: /* strb r9,[r12,#0x4d] */
            STORE(r[12]+77u,r[9],1);r[15]=0x8118u;return 0;
        case 0x8118u: /* strb r9,[r12,#0x51] */
            STORE(r[12]+81u,r[9],1);r[15]=0x811cu;return 0;
        case 0x811cu: /* ldrb r5,[r12,#0x0] */
            LOAD(r[5],r[12]+0u,1);r[15]=0x8120u;return 0;
        case 0x8120u: /* bic r5,r5,#0x4 */
            r[5]=r[5]&~4u;r[15]=0x8124u;return 0;
        case 0x8124u: /* bic r5,r5,#0x10 */
            r[5]=r[5]&~16u;r[15]=0x8128u;return 0;
        case 0x8128u: /* strb r5,[r12,#0x0] */
            STORE(r[12]+0u,r[5],1);r[15]=0x812cu;return 0;
        case 0x812cu: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x8130u;return 0;
        case 0x8130u: /* ldmiaeq sp!,{lr} */
            if(state->cpsr&0x40000000u){LOAD(r[14],r[13]+0u,4);r[13]+=4u;}r[15]=0x8134u;return 0;
        case 0x8134u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x8138u;return 0;
        case 0x8138u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x813cu: /* ldrb r0,[r12,#0x1] */
            LOAD(r[0],r[12]+1u,1);r[15]=0x8140u;return 0;
        case 0x8140u: /* orr r4,r4,r0 */
            r[4]=r[4]|r[0];r[15]=0x8144u;return 0;
        case 0x8144u: /* strb r4,[r12,#0x1] */
            STORE(r[12]+1u,r[4],1);r[15]=0x8148u;return 0;
        case 0x8148u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x814cu;return 0;
        case 0x814cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x8150u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x8154u;return 0;
        case 0x8154u: /* ldr r0,[r11,#0x0] */
            LOAD(r[0],r[11]+0u,4);r[15]=0x8158u;return 0;
        case 0x8158u: /* orr r0,r0,#0xc000 */
            r[0]=r[0]|49152u;r[15]=0x815cu;return 0;
        case 0x815cu: /* str r0,[r11,#0x0] */
            STORE(r[11]+0u,r[0],4);r[15]=0x8160u;return 0;
        case 0x8160u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x8164u: /* ldr r1,[r11,#0x0] */
            LOAD(r[1],r[11]+0u,4);r[15]=0x8168u;return 0;
        case 0x8168u: /* bic r1,r1,#0x4000 */
            r[1]=r[1]&~16384u;r[15]=0x816cu;return 0;
        case 0x816cu: /* orr r1,r1,#0x8000 */
            r[1]=r[1]|32768u;r[15]=0x8170u;return 0;
        case 0x8170u: /* str r1,[r11,#0x0] */
            STORE(r[11]+0u,r[1],4);r[15]=0x8174u;return 0;
        case 0x8174u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x8178u;return 0;
        case 0x8178u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x81b8u: /* ldr r10,[0x77e4] */
            LOAD(r[10],30692u,4);r[15]=0x81bcu;return 0;
        case 0x81bcu: /* mov r4,#0xc */
            r[4]=12u;r[15]=0x81c0u;return 0;
        case 0x81c0u: /* ldrb r9,[r12,#0x3] */
            LOAD(r[9],r[12]+3u,1);r[15]=0x81c4u;return 0;
        case 0x81c4u: /* mla r10,r4,r9,r10 */
            r[10]=r[4]*r[9]+r[10];r[15]=0x81c8u;return 0;
        case 0x81c8u: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x81ccu;return 0;
        case 0x81ccu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x81d0u: /* ldr r12,[0x7804] */
            LOAD(r[12],30724u,4);r[15]=0x81d4u;return 0;
        case 0x81d4u: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x81d8u;return 0;
        case 0x81d8u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x81dcu;return 0;
        case 0x81dcu: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x81e0u;return 0;
        case 0x81e0u: /* cmp r1,#0x2 */
            (void)subtract(state,r[1],2u);r[15]=0x81e4u;return 0;
        case 0x81e4u: /* movhi r4,#0x20 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[4]=32u;}r[15]=0x81e8u;return 0;
        case 0x81e8u: /* bhi 0x0000813c */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=33084u;}else{r[15]=0x81ecu;}return 0;
        case 0x81ecu: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x81f0u;return 0;
        case 0x81f0u: /* str r2,[r12,#0x34] */
            STORE(r[12]+52u,r[2],4);r[15]=0x81f4u;return 0;
        case 0x81f4u: /* strb r1,[r12,#0x23] */
            STORE(r[12]+35u,r[1],1);r[15]=0x81f8u;return 0;
        case 0x81f8u: /* ldrb r3,[r12,#0x0] */
            LOAD(r[3],r[12]+0u,1);r[15]=0x81fcu;return 0;
        case 0x81fcu: /* cmp r3,#0x1 */
            (void)subtract(state,r[3],1u);r[15]=0x8200u;return 0;
        case 0x8200u: /* movcc pc,lr */
            if(!(state->cpsr&0x20000000u)){r[15]=r[14];}else{r[15]=0x8204u;}return 0;
        case 0x8204u: /* ldr r3,[0x8258] */
            LOAD(r[3],33368u,4);r[15]=0x8208u;return 0;
        case 0x8208u: /* ldr r3,[r3,r1,lsl #0x2] */
            LOAD(r[3],r[3]+(r[1]<<2),4);r[15]=0x820cu;return 0;
        case 0x820cu: /* mov pc,r3 */
            r[15]=r[3];return 0;
        case 0x8220u: /* ldrb r3,[r12,#0x3c] */
            LOAD(r[3],r[12]+60u,1);r[15]=0x8224u;return 0;
        case 0x8224u: /* add r2,r3,r2 */
            r[2]=r[3]+r[2];r[15]=0x8228u;return 0;
        case 0x8228u: /* b 0x00008240 */
            r[15]=33344u;return 0;
        case 0x822cu: /* ldrb r3,[r12,#0x3c] */
            LOAD(r[3],r[12]+60u,1);r[15]=0x8230u;return 0;
        case 0x8230u: /* mul r3,r2,r3 */
            r[3]=r[2]*r[3];r[15]=0x8234u;return 0;
        case 0x8234u: /* mov r2,r3, lsr #0x8 */
            r[2]=(r[3]>>8);r[15]=0x8238u;return 0;
        case 0x8238u: /* b 0x00008240 */
            r[15]=33344u;return 0;
        case 0x823cu: /* ldrb r2,[r12,#0x3c] */
            LOAD(r[2],r[12]+60u,1);r[15]=0x8240u;return 0;
        case 0x8240u: /* str r2,[r12,#0x38] */
            STORE(r[12]+56u,r[2],4);r[15]=0x8244u;return 0;
        case 0x8244u: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}else{switch(r[15]){
        case 0x17bcu: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x17c0u;return 0;
        case 0x17c0u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x17c4u;return 0;
        case 0x17c4u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x17c8u;return 0;
        case 0x17c8u: /* ldrb r1,[r6,#0x4] */
            LOAD(r[1],r[6]+4u,1);r[15]=0x17ccu;return 0;
        case 0x17ccu: /* cmp r1,r0 */
            (void)subtract(state,r[1],r[0]);r[15]=0x17d0u;return 0;
        case 0x17d0u: /* bne 0x000017f4 */
            if(!(state->cpsr&0x40000000u)){r[15]=6132u;}else{r[15]=0x17d4u;}return 0;
        case 0x17d4u: /* ldrb r1,[r6,#0x0] */
            LOAD(r[1],r[6]+0u,1);r[15]=0x17d8u;return 0;
        case 0x17d8u: /* bic r1,r1,#0x80 */
            r[1]=r[1]&~128u;r[15]=0x17dcu;return 0;
        case 0x17dcu: /* strb r1,[r6,#0x0] */
            STORE(r[6]+0u,r[1],1);r[15]=0x17e0u;return 0;
        case 0x17e0u: /* ldr r1,[r8,#0x0] */
            LOAD(r[1],r[8]+0u,4);r[15]=0x17e4u;return 0;
        case 0x17e4u: /* ldr r2,[0x1808] */
            LOAD(r[2],6152u,4);r[15]=0x17e8u;return 0;
        case 0x17e8u: /* and r1,r1,r2 */
            r[1]=r[1]&r[2];r[15]=0x17ecu;return 0;
        case 0x17ecu: /* orr r1,r1,#0x8000 */
            r[1]=r[1]|32768u;r[15]=0x17f0u;return 0;
        case 0x17f0u: /* str r1,[r8,#0x0] */
            STORE(r[8]+0u,r[1],4);r[15]=0x17f4u;return 0;
        case 0x17f4u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x17f8u;return 0;
        case 0x17f8u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x17fcu;return 0;
        case 0x17fcu: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x1800u;return 0;
        case 0x1800u: /* bne 0x000017c8 */
            if(!(state->cpsr&0x40000000u)){r[15]=6088u;}else{r[15]=0x1804u;}return 0;
        case 0x1804u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x48f0u: /* ldr r10,[0x4b34] */
            LOAD(r[10],19252u,4);r[15]=0x48f4u;return 0;
        case 0x48f4u: /* mov r11,#0x20 */
            r[11]=32u;r[15]=0x48f8u;return 0;
        case 0x48f8u: /* ldrb r0,[r10,#0x0] */
            LOAD(r[0],r[10]+0u,1);r[15]=0x48fcu;return 0;
        case 0x48fcu: /* cmp r0,#0x0 */
            (void)subtract(state,r[0],0u);r[15]=0x4900u;return 0;
        case 0x4900u: /* stmdb sp!,{r10,r11,lr} */
            STORE(r[13]-12u,r[10],4);STORE(r[13]-8u,r[11],4);STORE(r[13]-4u,r[14],4);r[13]-=12u;r[15]=0x4904u;return 0;
        case 0x4904u: /* blne 0x00004940 */
            if(!(state->cpsr&0x40000000u)){r[14]=0x4908u;r[15]=18752u;}else{r[15]=0x4908u;}return 0;
        case 0x4908u: /* ldmia sp!,{r10,r11,lr} */
            LOAD(r[10],r[13]+0u,4);LOAD(r[11],r[13]+4u,4);LOAD(r[14],r[13]+8u,4);r[13]+=12u;r[15]=0x490cu;return 0;
        case 0x490cu: /* ldr r1,[0x493c] */
            LOAD(r[1],18748u,4);r[15]=0x4910u;return 0;
        case 0x4910u: /* strb r0,[r1,#0x1] */
            STORE(r[1]+1u,r[0],1);r[15]=0x4914u;return 0;
        case 0x4914u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x4918u;return 0;
        case 0x4918u: /* strb r0,[r10,#0x0] */
            STORE(r[10]+0u,r[0],1);r[15]=0x491cu;return 0;
        case 0x491cu: /* add r10,r10,#0x10 */
            r[10]=r[10]+16u;r[15]=0x4920u;return 0;
        case 0x4920u: /* subs r11,r11,#0x1 */
            r[11]=subtract(state,r[11],1u);r[15]=0x4924u;return 0;
        case 0x4924u: /* bne 0x000048f8 */
            if(!(state->cpsr&0x40000000u)){r[15]=18680u;}else{r[15]=0x4928u;}return 0;
        case 0x4928u: /* mov r0,#0x13400 */
            r[0]=78848u;r[15]=0x492cu;return 0;
        case 0x492cu: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x4930u;return 0;
        case 0x4930u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x4934u;return 0;
        case 0x4934u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4940u: /* mov r3,#0x14800 */
            r[3]=83968u;r[15]=0x4944u;return 0;
        case 0x4944u: /* ldr r4,[0x4b38] */
            LOAD(r[4],19256u,4);r[15]=0x4948u;return 0;
        case 0x4948u: /* ldr r7,[r4,#0x0] */
            LOAD(r[7],r[4]+0u,4);r[15]=0x494cu;return 0;
        case 0x494cu: /* add r3,r3,r7 */
            r[3]=r[3]+r[7];r[15]=0x4950u;return 0;
        case 0x4950u: /* str r3,[0x493c] */
            STORE(18748u,r[3],4);r[15]=0x4954u;return 0;
        case 0x4954u: /* ldr r5,[r10,#0x0] */
            LOAD(r[5],r[10]+0u,4);r[15]=0x4958u;return 0;
        case 0x4958u: /* str r5,[r3,#0x0] */
            STORE(r[3]+0u,r[5],4);r[15]=0x495cu;return 0;
        case 0x495cu: /* ldr r5,[r10,#0x4] */
            LOAD(r[5],r[10]+4u,4);r[15]=0x4960u;return 0;
        case 0x4960u: /* str r5,[r3,#0x4] */
            STORE(r[3]+4u,r[5],4);r[15]=0x4964u;return 0;
        case 0x4964u: /* ldr r5,[r10,#0x8] */
            LOAD(r[5],r[10]+8u,4);r[15]=0x4968u;return 0;
        case 0x4968u: /* str r5,[r3,#0x8] */
            STORE(r[3]+8u,r[5],4);r[15]=0x496cu;return 0;
        case 0x496cu: /* ldr r6,[0x4b3c] */
            LOAD(r[6],19260u,4);r[15]=0x4970u;return 0;
        case 0x4970u: /* ldr r5,[r6,#0x0] */
            LOAD(r[5],r[6]+0u,4);r[15]=0x4974u;return 0;
        case 0x4974u: /* str r5,[r3,#0xc] */
            STORE(r[3]+12u,r[5],4);r[15]=0x4978u;return 0;
        case 0x4978u: /* mov r5,#0x0 */
            r[5]=0u;r[15]=0x497cu;return 0;
        case 0x497cu: /* str r5,[r6,#0x0] */
            STORE(r[6]+0u,r[5],4);r[15]=0x4980u;return 0;
        case 0x4980u: /* add r7,r7,#0x10 */
            r[7]=r[7]+16u;r[15]=0x4984u;return 0;
        case 0x4984u: /* ldr r5,[0x4b2c] */
            LOAD(r[5],19244u,4);r[15]=0x4988u;return 0;
        case 0x4988u: /* and r7,r7,r5 */
            r[7]=r[7]&r[5];r[15]=0x498cu;return 0;
        case 0x498cu: /* str r7,[r4,#0x0] */
            STORE(r[4]+0u,r[7],4);r[15]=0x4990u;return 0;
        case 0x4990u: /* mov r3,#0x14800 */
            r[3]=83968u;r[15]=0x4994u;return 0;
        case 0x4994u: /* add r3,r3,r7 */
            r[3]=r[3]+r[7];r[15]=0x4998u;return 0;
        case 0x4998u: /* mvn r5,#0x0 */
            r[5]=~0u;r[15]=0x499cu;return 0;
        case 0x499cu: /* str r5,[r3,#0x0] */
            STORE(r[3]+0u,r[5],4);r[15]=0x49a0u;return 0;
        case 0x49a0u: /* str r5,[r3,#0x4] */
            STORE(r[3]+4u,r[5],4);r[15]=0x49a4u;return 0;
        case 0x49a4u: /* str r5,[r3,#0x8] */
            STORE(r[3]+8u,r[5],4);r[15]=0x49a8u;return 0;
        case 0x49a8u: /* str r5,[r3,#0xc] */
            STORE(r[3]+12u,r[5],4);r[15]=0x49acu;return 0;
        case 0x49acu: /* ands r0,r0,#0xff */
            temporary=r[0]&255u;r[0]=temporary;logic_flags(state,temporary,0,255u);r[15]=0x49b0u;return 0;
        case 0x49b0u: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x49b4u;return 0;
        case 0x49b4u: /* cmp r0,#0x200 */
            (void)subtract(state,r[0],512u);r[15]=0x49b8u;return 0;
        case 0x49b8u: /* bcs 0x000049d0 */
            if(state->cpsr&0x20000000u){r[15]=18896u;}else{r[15]=0x49bcu;}return 0;
        case 0x49bcu: /* adr r3,0x49ec */
            r[3]=18924u;r[15]=0x49c0u;return 0;
        case 0x49c0u: /* cmp r0,#0x100 */
            (void)subtract(state,r[0],256u);r[15]=0x49c4u;return 0;
        case 0x49c4u: /* movcs pc,lr */
            if(state->cpsr&0x20000000u){r[15]=r[14];}else{r[15]=0x49c8u;}return 0;
        case 0x49c8u: /* add r3,r3,r0 */
            r[3]=r[3]+r[0];r[15]=0x49ccu;return 0;
        case 0x49ccu: /* mov pc,r3 */
            r[15]=r[3];return 0;
        case 0x49d0u: /* ldr r3,[0x4b30] */
            LOAD(r[3],19248u,4);r[15]=0x49d4u;return 0;
        case 0x49d4u: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0x49d8u;return 0;
        case 0x49d8u: /* adr r3,0x4aec */
            r[3]=19180u;r[15]=0x49dcu;return 0;
        case 0x49dcu: /* cmp r0,#0x40 */
            (void)subtract(state,r[0],64u);r[15]=0x49e0u;return 0;
        case 0x49e0u: /* movcs pc,lr */
            if(state->cpsr&0x20000000u){r[15]=r[14];}else{r[15]=0x49e4u;}return 0;
        case 0x49e4u: /* add r3,r3,r0 */
            r[3]=r[3]+r[0];r[15]=0x49e8u;return 0;
        case 0x49e8u: /* mov pc,r3 */
            r[15]=r[3];return 0;
        case 0x49ecu: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x49f0u: /* b 0x00004b44 */
            r[15]=19268u;return 0;
        case 0x49f4u: /* b 0x00004c28 */
            r[15]=19496u;return 0;
        case 0x49f8u: /* b 0x00004cc0 */
            r[15]=19648u;return 0;
        case 0x49fcu: /* b 0x00004d4c */
            r[15]=19788u;return 0;
        case 0x4a00u: /* b 0x00004d98 */
            r[15]=19864u;return 0;
        case 0x4a04u: /* b 0x00004e20 */
            r[15]=20000u;return 0;
        case 0x4a08u: /* b 0x00004ea8 */
            r[15]=20136u;return 0;
        case 0x4a0cu: /* b 0x00004f48 */
            r[15]=20296u;return 0;
        case 0x4a10u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4a14u: /* b 0x00004fe0 */
            r[15]=20448u;return 0;
        case 0x4a18u: /* b 0x00005004 */
            r[15]=20484u;return 0;
        case 0x4a1cu: /* b 0x00005028 */
            r[15]=20520u;return 0;
        case 0x4a20u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4a24u: /* b 0x0000503c */
            r[15]=20540u;return 0;
        case 0x4a28u: /* b 0x000050e8 */
            r[15]=20712u;return 0;
        case 0x4a2cu: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4a30u: /* b 0x00005184 */
            r[15]=20868u;return 0;
        case 0x4a34u: /* b 0x00005274 */
            r[15]=21108u;return 0;
        case 0x4a38u: /* b 0x00005288 */
            r[15]=21128u;return 0;
        case 0x4a3cu: /* b 0x0000529c */
            r[15]=21148u;return 0;
        case 0x4a40u: /* b 0x000052b0 */
            r[15]=21168u;return 0;
        case 0x4a44u: /* b 0x000052d8 */
            r[15]=21208u;return 0;
        case 0x4a48u: /* b 0x00005300 */
            r[15]=21248u;return 0;
        case 0x4a4cu: /* b 0x000053a0 */
            r[15]=21408u;return 0;
        case 0x4a50u: /* b 0x00005440 */
            r[15]=21568u;return 0;
        case 0x4a54u: /* b 0x0000545c */
            r[15]=21596u;return 0;
        case 0x4a58u: /* b 0x00005478 */
            r[15]=21624u;return 0;
        case 0x4a5cu: /* b 0x00005494 */
            r[15]=21652u;return 0;
        case 0x4a60u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4a64u: /* b 0x000054b0 */
            r[15]=21680u;return 0;
        case 0x4a68u: /* b 0x000054dc */
            r[15]=21724u;return 0;
        case 0x4a6cu: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4a70u: /* b 0x000054f0 */
            r[15]=21744u;return 0;
        case 0x4a74u: /* b 0x000055c8 */
            r[15]=21960u;return 0;
        case 0x4a78u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4a7cu: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4a80u: /* b 0x0000560c */
            r[15]=22028u;return 0;
        case 0x4a84u: /* b 0x00005640 */
            r[15]=22080u;return 0;
        case 0x4a88u: /* b 0x00005674 */
            r[15]=22132u;return 0;
        case 0x4a8cu: /* b 0x00005724 */
            r[15]=22308u;return 0;
        case 0x4a90u: /* b 0x000057d4 */
            r[15]=22484u;return 0;
        case 0x4a94u: /* b 0x000057f0 */
            r[15]=22512u;return 0;
        case 0x4a98u: /* b 0x00005808 */
            r[15]=22536u;return 0;
        case 0x4a9cu: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4aa0u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4aa4u: /* b 0x00005820 */
            r[15]=22560u;return 0;
        case 0x4aa8u: /* b 0x0000584c */
            r[15]=22604u;return 0;
        case 0x4aacu: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4ab0u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4ab4u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4ab8u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4abcu: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4ac0u: /* b 0x00005864 */
            r[15]=22628u;return 0;
        case 0x4ac4u: /* b 0x0000588c */
            r[15]=22668u;return 0;
        case 0x4ac8u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4accu: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4ad0u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4ad4u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4ad8u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4adcu: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4ae0u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4ae4u: /* b 0x000058d4 */
            r[15]=22740u;return 0;
        case 0x4ae8u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4aecu: /* b 0x000058f8 */
            r[15]=22776u;return 0;
        case 0x4af0u: /* b 0x000059e0 */
            r[15]=23008u;return 0;
        case 0x4af4u: /* b 0x00005a1c */
            r[15]=23068u;return 0;
        case 0x4af8u: /* b 0x00005b68 */
            r[15]=23400u;return 0;
        case 0x4afcu: /* b 0x00005c24 */
            r[15]=23588u;return 0;
        case 0x4b00u: /* b 0x00005d5c */
            r[15]=23900u;return 0;
        case 0x4b04u: /* b 0x00005e00 */
            r[15]=24064u;return 0;
        case 0x4b08u: /* b 0x00005e30 */
            r[15]=24112u;return 0;
        case 0x4b0cu: /* b 0x00005ee4 */
            r[15]=24292u;return 0;
        case 0x4b10u: /* b 0x00005fd0 */
            r[15]=24528u;return 0;
        case 0x4b14u: /* b 0x00005fd4 */
            r[15]=24532u;return 0;
        case 0x4b18u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4b1cu: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4b20u: /* b 0x00004b40 */
            r[15]=19264u;return 0;
        case 0x4b24u: /* b 0x00006024 */
            r[15]=24612u;return 0;
        case 0x4b28u: /* b 0x00006068 */
            r[15]=24680u;return 0;
        case 0x4b40u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4b44u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4b48u;return 0;
        case 0x4b48u: /* ldrb r3,[r10,#0x3] */
            LOAD(r[3],r[10]+3u,1);r[15]=0x4b4cu;return 0;
        case 0x4b4cu: /* ldrb r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,1);r[15]=0x4b50u;return 0;
        case 0x4b50u: /* ldrb r2,[r10,#0x5] */
            LOAD(r[2],r[10]+5u,1);r[15]=0x4b54u;return 0;
        case 0x4b54u: /* mov r3,r3, lsl #0x3 */
            r[3]=(r[3]<<3);r[15]=0x4b58u;return 0;
        case 0x4b58u: /* mov r4,#0x14000 */
            r[4]=81920u;r[15]=0x4b5cu;return 0;
        case 0x4b5cu: /* add r4,r4,r3 */
            r[4]=r[4]+r[3];r[15]=0x4b60u;return 0;
        case 0x4b60u: /* ldr r12,[r4,#0x0] */
            LOAD(r[12],r[4]+0u,4);r[15]=0x4b64u;return 0;
        case 0x4b64u: /* ldr r5,[r12,#0x0] */
            LOAD(r[5],r[12]+0u,4);r[15]=0x4b68u;return 0;
        case 0x4b68u: /* ldr r4,[0x4c20] */
            LOAD(r[4],19488u,4);r[15]=0x4b6cu;return 0;
        case 0x4b6cu: /* cmp r5,r4 */
            (void)subtract(state,r[5],r[4]);r[15]=0x4b70u;return 0;
        case 0x4b70u: /* movne r1,#0x2 */
            if(!(state->cpsr&0x40000000u)){r[1]=2u;}r[15]=0x4b74u;return 0;
        case 0x4b74u: /* bne 0x00004bf4 */
            if(!(state->cpsr&0x40000000u)){r[15]=19444u;}else{r[15]=0x4b78u;}return 0;
        case 0x4b78u: /* ldrb r5,[r12,#0x4] */
            LOAD(r[5],r[12]+4u,1);r[15]=0x4b7cu;return 0;
        case 0x4b7cu: /* cmp r5,#0x1 */
            (void)subtract(state,r[5],1u);r[15]=0x4b80u;return 0;
        case 0x4b80u: /* movne r1,#0x8 */
            if(!(state->cpsr&0x40000000u)){r[1]=8u;}r[15]=0x4b84u;return 0;
        case 0x4b84u: /* bne 0x00004bf4 */
            if(!(state->cpsr&0x40000000u)){r[15]=19444u;}else{r[15]=0x4b88u;}return 0;
        case 0x4b88u: /* ldr r6,[r12,#0x8] */
            LOAD(r[6],r[12]+8u,4);r[15]=0x4b8cu;return 0;
        case 0x4b8cu: /* sub r6,r6,#0x4 */
            r[6]=r[6]-4u;r[15]=0x4b90u;return 0;
        case 0x4b90u: /* ldr r5,[r12,r6] */
            LOAD(r[5],r[12]+r[6],4);r[15]=0x4b94u;return 0;
        case 0x4b94u: /* ldr r6,[0x4c24] */
            LOAD(r[6],19492u,4);r[15]=0x4b98u;return 0;
        case 0x4b98u: /* cmp r5,r6 */
            (void)subtract(state,r[5],r[6]);r[15]=0x4b9cu;return 0;
        case 0x4b9cu: /* movne r1,#0x4 */
            if(!(state->cpsr&0x40000000u)){r[1]=4u;}r[15]=0x4ba0u;return 0;
        case 0x4ba0u: /* bne 0x00004bf4 */
            if(!(state->cpsr&0x40000000u)){r[15]=19444u;}else{r[15]=0x4ba4u;}return 0;
        case 0x4ba4u: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x4ba8u;return 0;
        case 0x4ba8u: /* bl 0x0000740c */
            r[14]=0x4bacu;r[15]=29708u;return 0;
        case 0x4bacu: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x4bb0u;return 0;
        case 0x4bb0u: /* mov r2,#0xc000 */
            r[2]=49152u;r[15]=0x4bb4u;return 0;
        case 0x4bb4u: /* mov r0,r0, lsl #0x7 */
            r[0]=(r[0]<<7);r[15]=0x4bb8u;return 0;
        case 0x4bb8u: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x4bbcu;return 0;
        case 0x4bbcu: /* ldrb r3,[r2,#0x28] */
            LOAD(r[3],r[2]+40u,1);r[15]=0x4bc0u;return 0;
        case 0x4bc0u: /* and r3,r3,#0xfc */
            r[3]=r[3]&252u;r[15]=0x4bc4u;return 0;
        case 0x4bc4u: /* and r1,r1,#0x3 */
            r[1]=r[1]&3u;r[15]=0x4bc8u;return 0;
        case 0x4bc8u: /* orr r3,r3,r1 */
            r[3]=r[3]|r[1];r[15]=0x4bccu;return 0;
        case 0x4bccu: /* ldr r4,[0x5180] */
            LOAD(r[4],20864u,4);r[15]=0x4bd0u;return 0;
        case 0x4bd0u: /* ldrb r1,[r4,#0x0] */
            LOAD(r[1],r[4]+0u,1);r[15]=0x4bd4u;return 0;
        case 0x4bd4u: /* strb r1,[r2,#0x29] */
            STORE(r[2]+41u,r[1],1);r[15]=0x4bd8u;return 0;
        case 0x4bd8u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x4bdcu;return 0;
        case 0x4bdcu: /* orrne r3,r3,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[3]|128u;}r[15]=0x4be0u;return 0;
        case 0x4be0u: /* biceq r3,r3,#0x80 */
            if(state->cpsr&0x40000000u){r[3]=r[3]&~128u;}r[15]=0x4be4u;return 0;
        case 0x4be4u: /* strb r3,[r2,#0x28] */
            STORE(r[2]+40u,r[3],1);r[15]=0x4be8u;return 0;
        case 0x4be8u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x4becu;return 0;
        case 0x4becu: /* strb r1,[r4,#0x0] */
            STORE(r[4]+0u,r[1],1);r[15]=0x4bf0u;return 0;
        case 0x4bf0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4bf4u: /* mov r2,#0xc000 */
            r[2]=49152u;r[15]=0x4bf8u;return 0;
        case 0x4bf8u: /* mov r0,r0, lsl #0x7 */
            r[0]=(r[0]<<7);r[15]=0x4bfcu;return 0;
        case 0x4bfcu: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x4c00u;return 0;
        case 0x4c00u: /* ldrb r0,[r2,#0x28] */
            LOAD(r[0],r[2]+40u,1);r[15]=0x4c04u;return 0;
        case 0x4c04u: /* orr r0,r0,#0x80 */
            r[0]=r[0]|128u;r[15]=0x4c08u;return 0;
        case 0x4c08u: /* strb r0,[r2,#0x28] */
            STORE(r[2]+40u,r[0],1);r[15]=0x4c0cu;return 0;
        case 0x4c0cu: /* ldrb r0,[r2,#0x2a] */
            LOAD(r[0],r[2]+42u,1);r[15]=0x4c10u;return 0;
        case 0x4c10u: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0x4c14u;return 0;
        case 0x4c14u: /* strb r0,[r2,#0x2a] */
            STORE(r[2]+42u,r[0],1);r[15]=0x4c18u;return 0;
        case 0x4c18u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4c28u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4c2cu;return 0;
        case 0x4c2cu: /* mov r1,#0xe800 */
            r[1]=59392u;r[15]=0x4c30u;return 0;
        case 0x4c30u: /* mov r2,#0x30 */
            r[2]=48u;r[15]=0x4c34u;return 0;
        case 0x4c34u: /* mov r3,#0x800000 */
            r[3]=8388608u;r[15]=0x4c38u;return 0;
        case 0x4c38u: /* ldrb r4,[r1,#0x4] */
            LOAD(r[4],r[1]+4u,1);r[15]=0x4c3cu;return 0;
        case 0x4c3cu: /* cmp r4,r0 */
            (void)subtract(state,r[4],r[0]);r[15]=0x4c40u;return 0;
        case 0x4c40u: /* bne 0x00004c60 */
            if(!(state->cpsr&0x40000000u)){r[15]=19552u;}else{r[15]=0x4c44u;}return 0;
        case 0x4c44u: /* mov r4,#0x0 */
            r[4]=0u;r[15]=0x4c48u;return 0;
        case 0x4c48u: /* str r4,[r1,#0x0] */
            STORE(r[1]+0u,r[4],4);r[15]=0x4c4cu;return 0;
        case 0x4c4cu: /* ldr r5,[r3,#0x0] */
            LOAD(r[5],r[3]+0u,4);r[15]=0x4c50u;return 0;
        case 0x4c50u: /* ldr r4,[0x4cbc] */
            LOAD(r[4],19644u,4);r[15]=0x4c54u;return 0;
        case 0x4c54u: /* and r5,r5,r4 */
            r[5]=r[5]&r[4];r[15]=0x4c58u;return 0;
        case 0x4c58u: /* orr r5,r5,#0x8000 */
            r[5]=r[5]|32768u;r[15]=0x4c5cu;return 0;
        case 0x4c5cu: /* str r5,[r3,#0x0] */
            STORE(r[3]+0u,r[5],4);r[15]=0x4c60u;return 0;
        case 0x4c60u: /* add r1,r1,#0x40 */
            r[1]=r[1]+64u;r[15]=0x4c64u;return 0;
        case 0x4c64u: /* add r3,r3,#0x80 */
            r[3]=r[3]+128u;r[15]=0x4c68u;return 0;
        case 0x4c68u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x4c6cu;return 0;
        case 0x4c6cu: /* bne 0x00004c38 */
            if(!(state->cpsr&0x40000000u)){r[15]=19512u;}else{r[15]=0x4c70u;}return 0;
        case 0x4c70u: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x4c74u;return 0;
        case 0x4c74u: /* bl 0x000074f4 */
            r[14]=0x4c78u;r[15]=29940u;return 0;
        case 0x4c78u: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x4c7cu;return 0;
        case 0x4c7cu: /* mov r2,#0xc000 */
            r[2]=49152u;r[15]=0x4c80u;return 0;
        case 0x4c80u: /* mov r0,r0, lsl #0x7 */
            r[0]=(r[0]<<7);r[15]=0x4c84u;return 0;
        case 0x4c84u: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x4c88u;return 0;
        case 0x4c88u: /* ldrb r3,[r2,#0x28] */
            LOAD(r[3],r[2]+40u,1);r[15]=0x4c8cu;return 0;
        case 0x4c8cu: /* and r3,r3,#0xfc */
            r[3]=r[3]&252u;r[15]=0x4c90u;return 0;
        case 0x4c90u: /* and r1,r1,#0x3 */
            r[1]=r[1]&3u;r[15]=0x4c94u;return 0;
        case 0x4c94u: /* orr r3,r3,r1 */
            r[3]=r[3]|r[1];r[15]=0x4c98u;return 0;
        case 0x4c98u: /* ldr r4,[0x5180] */
            LOAD(r[4],20864u,4);r[15]=0x4c9cu;return 0;
        case 0x4c9cu: /* ldrb r1,[r4,#0x0] */
            LOAD(r[1],r[4]+0u,1);r[15]=0x4ca0u;return 0;
        case 0x4ca0u: /* strb r1,[r2,#0x29] */
            STORE(r[2]+41u,r[1],1);r[15]=0x4ca4u;return 0;
        case 0x4ca4u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x4ca8u;return 0;
        case 0x4ca8u: /* orrne r3,r3,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[3]|128u;}r[15]=0x4cacu;return 0;
        case 0x4cacu: /* biceq r3,r3,#0x80 */
            if(state->cpsr&0x40000000u){r[3]=r[3]&~128u;}r[15]=0x4cb0u;return 0;
        case 0x4cb0u: /* strb r3,[r2,#0x28] */
            STORE(r[2]+40u,r[3],1);r[15]=0x4cb4u;return 0;
        case 0x4cb4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4cc0u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4cc4u;return 0;
        case 0x4cc4u: /* mov r1,#0xe800 */
            r[1]=59392u;r[15]=0x4cc8u;return 0;
        case 0x4cc8u: /* mov r2,#0x30 */
            r[2]=48u;r[15]=0x4cccu;return 0;
        case 0x4cccu: /* mov r3,#0x800000 */
            r[3]=8388608u;r[15]=0x4cd0u;return 0;
        case 0x4cd0u: /* ldrb r4,[r1,#0x4] */
            LOAD(r[4],r[1]+4u,1);r[15]=0x4cd4u;return 0;
        case 0x4cd4u: /* cmp r4,r0 */
            (void)subtract(state,r[4],r[0]);r[15]=0x4cd8u;return 0;
        case 0x4cd8u: /* bne 0x00004cf0 */
            if(!(state->cpsr&0x40000000u)){r[15]=19696u;}else{r[15]=0x4cdcu;}return 0;
        case 0x4cdcu: /* ldr r5,[r3,#0x0] */
            LOAD(r[5],r[3]+0u,4);r[15]=0x4ce0u;return 0;
        case 0x4ce0u: /* ldr r4,[0x4d48] */
            LOAD(r[4],19784u,4);r[15]=0x4ce4u;return 0;
        case 0x4ce4u: /* and r5,r5,r4 */
            r[5]=r[5]&r[4];r[15]=0x4ce8u;return 0;
        case 0x4ce8u: /* orr r5,r5,#0x8000 */
            r[5]=r[5]|32768u;r[15]=0x4cecu;return 0;
        case 0x4cecu: /* str r5,[r3,#0x0] */
            STORE(r[3]+0u,r[5],4);r[15]=0x4cf0u;return 0;
        case 0x4cf0u: /* add r1,r1,#0x40 */
            r[1]=r[1]+64u;r[15]=0x4cf4u;return 0;
        case 0x4cf4u: /* add r3,r3,#0x80 */
            r[3]=r[3]+128u;r[15]=0x4cf8u;return 0;
        case 0x4cf8u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x4cfcu;return 0;
        case 0x4cfcu: /* bne 0x00004cd0 */
            if(!(state->cpsr&0x40000000u)){r[15]=19664u;}else{r[15]=0x4d00u;}return 0;
        case 0x4d00u: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x4d04u;return 0;
        case 0x4d04u: /* bl 0x00007584 */
            r[14]=0x4d08u;r[15]=30084u;return 0;
        case 0x4d08u: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x4d0cu;return 0;
        case 0x4d0cu: /* mov r2,#0xc000 */
            r[2]=49152u;r[15]=0x4d10u;return 0;
        case 0x4d10u: /* mov r0,r0, lsl #0x7 */
            r[0]=(r[0]<<7);r[15]=0x4d14u;return 0;
        case 0x4d14u: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x4d18u;return 0;
        case 0x4d18u: /* ldrb r3,[r2,#0x28] */
            LOAD(r[3],r[2]+40u,1);r[15]=0x4d1cu;return 0;
        case 0x4d1cu: /* and r3,r3,#0xfc */
            r[3]=r[3]&252u;r[15]=0x4d20u;return 0;
        case 0x4d20u: /* and r1,r1,#0x3 */
            r[1]=r[1]&3u;r[15]=0x4d24u;return 0;
        case 0x4d24u: /* orr r3,r3,r1 */
            r[3]=r[3]|r[1];r[15]=0x4d28u;return 0;
        case 0x4d28u: /* ldr r4,[0x5180] */
            LOAD(r[4],20864u,4);r[15]=0x4d2cu;return 0;
        case 0x4d2cu: /* ldrb r1,[r4,#0x0] */
            LOAD(r[1],r[4]+0u,1);r[15]=0x4d30u;return 0;
        case 0x4d30u: /* strb r1,[r2,#0x29] */
            STORE(r[2]+41u,r[1],1);r[15]=0x4d34u;return 0;
        case 0x4d34u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x4d38u;return 0;
        case 0x4d38u: /* orrne r3,r3,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[3]|128u;}r[15]=0x4d3cu;return 0;
        case 0x4d3cu: /* biceq r3,r3,#0x80 */
            if(state->cpsr&0x40000000u){r[3]=r[3]&~128u;}r[15]=0x4d40u;return 0;
        case 0x4d40u: /* strb r3,[r2,#0x28] */
            STORE(r[2]+40u,r[3],1);r[15]=0x4d44u;return 0;
        case 0x4d44u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4d4cu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4d50u;return 0;
        case 0x4d50u: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x4d54u;return 0;
        case 0x4d54u: /* bl 0x00007604 */
            r[14]=0x4d58u;r[15]=30212u;return 0;
        case 0x4d58u: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x4d5cu;return 0;
        case 0x4d5cu: /* mov r2,#0xc000 */
            r[2]=49152u;r[15]=0x4d60u;return 0;
        case 0x4d60u: /* mov r0,r0, lsl #0x7 */
            r[0]=(r[0]<<7);r[15]=0x4d64u;return 0;
        case 0x4d64u: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x4d68u;return 0;
        case 0x4d68u: /* ldrb r3,[r2,#0x28] */
            LOAD(r[3],r[2]+40u,1);r[15]=0x4d6cu;return 0;
        case 0x4d6cu: /* and r3,r3,#0xfc */
            r[3]=r[3]&252u;r[15]=0x4d70u;return 0;
        case 0x4d70u: /* and r1,r1,#0x3 */
            r[1]=r[1]&3u;r[15]=0x4d74u;return 0;
        case 0x4d74u: /* orr r3,r3,r1 */
            r[3]=r[3]|r[1];r[15]=0x4d78u;return 0;
        case 0x4d78u: /* ldr r4,[0x5180] */
            LOAD(r[4],20864u,4);r[15]=0x4d7cu;return 0;
        case 0x4d7cu: /* ldrb r1,[r4,#0x0] */
            LOAD(r[1],r[4]+0u,1);r[15]=0x4d80u;return 0;
        case 0x4d80u: /* strb r1,[r2,#0x29] */
            STORE(r[2]+41u,r[1],1);r[15]=0x4d84u;return 0;
        case 0x4d84u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x4d88u;return 0;
        case 0x4d88u: /* orrne r3,r3,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[3]|128u;}r[15]=0x4d8cu;return 0;
        case 0x4d8cu: /* biceq r3,r3,#0x80 */
            if(state->cpsr&0x40000000u){r[3]=r[3]&~128u;}r[15]=0x4d90u;return 0;
        case 0x4d90u: /* strb r3,[r2,#0x28] */
            STORE(r[2]+40u,r[3],1);r[15]=0x4d94u;return 0;
        case 0x4d94u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4d98u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4d9cu;return 0;
        case 0x4d9cu: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x4da0u;return 0;
        case 0x4da0u: /* ldr r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,4);r[15]=0x4da4u;return 0;
        case 0x4da4u: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x4da8u;return 0;
        case 0x4da8u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x4dacu;return 0;
        case 0x4dacu: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x4db0u;return 0;
        case 0x4db0u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x4db4u;return 0;
        case 0x4db4u: /* ldr r3,[0x4e14] */
            LOAD(r[3],19988u,4);r[15]=0x4db8u;return 0;
        case 0x4db8u: /* ands r2,r2,r3 */
            temporary=r[2]&r[3];r[2]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x4dbcu;return 0;
        case 0x4dbcu: /* beq 0x00004dfc */
            if(state->cpsr&0x40000000u){r[15]=19964u;}else{r[15]=0x4dc0u;}return 0;
        case 0x4dc0u: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x4dc4u;return 0;
        case 0x4dc4u: /* ldrb r0,[r4,#0x1] */
            LOAD(r[0],r[4]+1u,1);r[15]=0x4dc8u;return 0;
        case 0x4dc8u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x4dccu;return 0;
        case 0x4dccu: /* ldr r3,[0x4e18] */
            LOAD(r[3],19992u,4);r[15]=0x4dd0u;return 0;
        case 0x4dd0u: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x4dd4u;return 0;
        case 0x4dd4u: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x4dd8u;return 0;
        case 0x4dd8u: /* and r0,r0,#0xff */
            r[0]=r[0]&255u;r[15]=0x4ddcu;return 0;
        case 0x4ddcu: /* ldr r3,[0x4e1c] */
            LOAD(r[3],19996u,4);r[15]=0x4de0u;return 0;
        case 0x4de0u: /* tst r0,#0xfe */
            temporary=r[0]&254u;logic_flags(state,temporary,0,254u);r[15]=0x4de4u;return 0;
        case 0x4de4u: /* beq 0x00004dfc */
            if(state->cpsr&0x40000000u){r[15]=19964u;}else{r[15]=0x4de8u;}return 0;
        case 0x4de8u: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4decu;return 0;
        case 0x4decu: /* beq 0x00004dfc */
            if(state->cpsr&0x40000000u){r[15]=19964u;}else{r[15]=0x4df0u;}return 0;
        case 0x4df0u: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x4df4u;return 0;
        case 0x4df4u: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x4df8u;return 0;
        case 0x4df8u: /* b 0x00004de0 */
            r[15]=19936u;return 0;
        case 0x4dfcu: /* strb r1,[r4,#0x0] */
            STORE(r[4]+0u,r[1],1);r[15]=0x4e00u;return 0;
        case 0x4e00u: /* strb r2,[r4,#0x2] */
            STORE(r[4]+2u,r[2],1);r[15]=0x4e04u;return 0;
        case 0x4e04u: /* strb r2,[r4,#0x3] */
            STORE(r[4]+3u,r[2],1);r[15]=0x4e08u;return 0;
        case 0x4e08u: /* strb r0,[r4,#0x4] */
            STORE(r[4]+4u,r[0],1);r[15]=0x4e0cu;return 0;
        case 0x4e0cu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x4e10u;return 0;
        case 0x4e10u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4e20u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4e24u;return 0;
        case 0x4e24u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x4e28u;return 0;
        case 0x4e28u: /* ldr r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,4);r[15]=0x4e2cu;return 0;
        case 0x4e2cu: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x4e30u;return 0;
        case 0x4e30u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x4e34u;return 0;
        case 0x4e34u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x4e38u;return 0;
        case 0x4e38u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x4e3cu;return 0;
        case 0x4e3cu: /* ldr r3,[0x4e9c] */
            LOAD(r[3],20124u,4);r[15]=0x4e40u;return 0;
        case 0x4e40u: /* ands r2,r2,r3 */
            temporary=r[2]&r[3];r[2]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x4e44u;return 0;
        case 0x4e44u: /* beq 0x00004e84 */
            if(state->cpsr&0x40000000u){r[15]=20100u;}else{r[15]=0x4e48u;}return 0;
        case 0x4e48u: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x4e4cu;return 0;
        case 0x4e4cu: /* ldrb r0,[r4,#0x7] */
            LOAD(r[0],r[4]+7u,1);r[15]=0x4e50u;return 0;
        case 0x4e50u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x4e54u;return 0;
        case 0x4e54u: /* ldr r3,[0x4ea0] */
            LOAD(r[3],20128u,4);r[15]=0x4e58u;return 0;
        case 0x4e58u: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x4e5cu;return 0;
        case 0x4e5cu: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x4e60u;return 0;
        case 0x4e60u: /* and r0,r0,#0xff */
            r[0]=r[0]&255u;r[15]=0x4e64u;return 0;
        case 0x4e64u: /* ldr r3,[0x4ea4] */
            LOAD(r[3],20132u,4);r[15]=0x4e68u;return 0;
        case 0x4e68u: /* tst r0,#0xfe */
            temporary=r[0]&254u;logic_flags(state,temporary,0,254u);r[15]=0x4e6cu;return 0;
        case 0x4e6cu: /* beq 0x00004e84 */
            if(state->cpsr&0x40000000u){r[15]=20100u;}else{r[15]=0x4e70u;}return 0;
        case 0x4e70u: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4e74u;return 0;
        case 0x4e74u: /* beq 0x00004e84 */
            if(state->cpsr&0x40000000u){r[15]=20100u;}else{r[15]=0x4e78u;}return 0;
        case 0x4e78u: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x4e7cu;return 0;
        case 0x4e7cu: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x4e80u;return 0;
        case 0x4e80u: /* b 0x00004e68 */
            r[15]=20072u;return 0;
        case 0x4e84u: /* strb r1,[r4,#0x6] */
            STORE(r[4]+6u,r[1],1);r[15]=0x4e88u;return 0;
        case 0x4e88u: /* strb r2,[r4,#0x8] */
            STORE(r[4]+8u,r[2],1);r[15]=0x4e8cu;return 0;
        case 0x4e8cu: /* strb r2,[r4,#0x9] */
            STORE(r[4]+9u,r[2],1);r[15]=0x4e90u;return 0;
        case 0x4e90u: /* strb r0,[r4,#0xa] */
            STORE(r[4]+10u,r[0],1);r[15]=0x4e94u;return 0;
        case 0x4e94u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x4e98u;return 0;
        case 0x4e98u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4ea8u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4eacu;return 0;
        case 0x4eacu: /* ldr r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,4);r[15]=0x4eb0u;return 0;
        case 0x4eb0u: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x4eb4u;return 0;
        case 0x4eb4u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x4eb8u;return 0;
        case 0x4eb8u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x4ebcu;return 0;
        case 0x4ebcu: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x4ec0u;return 0;
        case 0x4ec0u: /* mov r2,r1, lsr #0x10 */
            r[2]=(r[1]>>16);r[15]=0x4ec4u;return 0;
        case 0x4ec4u: /* ldr r3,[0x4f38] */
            LOAD(r[3],20280u,4);r[15]=0x4ec8u;return 0;
        case 0x4ec8u: /* and r1,r1,r3 */
            r[1]=r[1]&r[3];r[15]=0x4eccu;return 0;
        case 0x4eccu: /* ands r2,r2,r3 */
            temporary=r[2]&r[3];r[2]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x4ed0u;return 0;
        case 0x4ed0u: /* beq 0x00004f14 */
            if(state->cpsr&0x40000000u){r[15]=20244u;}else{r[15]=0x4ed4u;}return 0;
        case 0x4ed4u: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x4ed8u;return 0;
        case 0x4ed8u: /* ldr r0,[r4,#0x1c] */
            LOAD(r[0],r[4]+28u,4);r[15]=0x4edcu;return 0;
        case 0x4edcu: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x4ee0u;return 0;
        case 0x4ee0u: /* ldr r3,[0x4f3c] */
            LOAD(r[3],20284u,4);r[15]=0x4ee4u;return 0;
        case 0x4ee4u: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x4ee8u;return 0;
        case 0x4ee8u: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x4eecu;return 0;
        case 0x4eecu: /* ldr r3,[0x4f38] */
            LOAD(r[3],20280u,4);r[15]=0x4ef0u;return 0;
        case 0x4ef0u: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0x4ef4u;return 0;
        case 0x4ef4u: /* ldr r3,[0x4f40] */
            LOAD(r[3],20288u,4);r[15]=0x4ef8u;return 0;
        case 0x4ef8u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4efcu;return 0;
        case 0x4efcu: /* beq 0x00004f14 */
            if(state->cpsr&0x40000000u){r[15]=20244u;}else{r[15]=0x4f00u;}return 0;
        case 0x4f00u: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4f04u;return 0;
        case 0x4f04u: /* beq 0x00004f14 */
            if(state->cpsr&0x40000000u){r[15]=20244u;}else{r[15]=0x4f08u;}return 0;
        case 0x4f08u: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x4f0cu;return 0;
        case 0x4f0cu: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x4f10u;return 0;
        case 0x4f10u: /* b 0x00004ef8 */
            r[15]=20216u;return 0;
        case 0x4f14u: /* ldr r3,[0x4f44] */
            LOAD(r[3],20292u,4);r[15]=0x4f18u;return 0;
        case 0x4f18u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4f1cu;return 0;
        case 0x4f1cu: /* movne r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=255u;}r[15]=0x4f20u;return 0;
        case 0x4f20u: /* str r1,[r4,#0x18] */
            STORE(r[4]+24u,r[1],4);r[15]=0x4f24u;return 0;
        case 0x4f24u: /* strb r2,[r4,#0x20] */
            STORE(r[4]+32u,r[2],1);r[15]=0x4f28u;return 0;
        case 0x4f28u: /* strb r2,[r4,#0x21] */
            STORE(r[4]+33u,r[2],1);r[15]=0x4f2cu;return 0;
        case 0x4f2cu: /* strb r0,[r4,#0x22] */
            STORE(r[4]+34u,r[0],1);r[15]=0x4f30u;return 0;
        case 0x4f30u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x4f34u;return 0;
        case 0x4f34u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4f48u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4f4cu;return 0;
        case 0x4f4cu: /* ldr r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,4);r[15]=0x4f50u;return 0;
        case 0x4f50u: /* mov r2,r1, lsr #0x10 */
            r[2]=(r[1]>>16);r[15]=0x4f54u;return 0;
        case 0x4f54u: /* ldr r3,[0x4fd0] */
            LOAD(r[3],20432u,4);r[15]=0x4f58u;return 0;
        case 0x4f58u: /* and r1,r1,r3 */
            r[1]=r[1]&r[3];r[15]=0x4f5cu;return 0;
        case 0x4f5cu: /* and r2,r2,r3 */
            r[2]=r[2]&r[3];r[15]=0x4f60u;return 0;
        case 0x4f60u: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x4f64u;return 0;
        case 0x4f64u: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x4f68u;return 0;
        case 0x4f68u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x4f6cu;return 0;
        case 0x4f6cu: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x4f70u;return 0;
        case 0x4f70u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x4f74u;return 0;
        case 0x4f74u: /* ldr r0,[r4,#0x10] */
            LOAD(r[0],r[4]+16u,4);r[15]=0x4f78u;return 0;
        case 0x4f78u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x4f7cu;return 0;
        case 0x4f7cu: /* ldr r3,[0x4fd4] */
            LOAD(r[3],20436u,4);r[15]=0x4f80u;return 0;
        case 0x4f80u: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x4f84u;return 0;
        case 0x4f84u: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x4f88u;return 0;
        case 0x4f88u: /* ldr r3,[0x4fd0] */
            LOAD(r[3],20432u,4);r[15]=0x4f8cu;return 0;
        case 0x4f8cu: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0x4f90u;return 0;
        case 0x4f90u: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x4f94u;return 0;
        case 0x4f94u: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x4f98u;return 0;
        case 0x4f98u: /* ldr r3,[0x4fd8] */
            LOAD(r[3],20440u,4);r[15]=0x4f9cu;return 0;
        case 0x4f9cu: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4fa0u;return 0;
        case 0x4fa0u: /* beq 0x00004fac */
            if(state->cpsr&0x40000000u){r[15]=20396u;}else{r[15]=0x4fa4u;}return 0;
        case 0x4fa4u: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4fa8u;return 0;
        case 0x4fa8u: /* bne 0x00004f90 */
            if(!(state->cpsr&0x40000000u)){r[15]=20368u;}else{r[15]=0x4facu;}return 0;
        case 0x4facu: /* ldr r3,[0x4fdc] */
            LOAD(r[3],20444u,4);r[15]=0x4fb0u;return 0;
        case 0x4fb0u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x4fb4u;return 0;
        case 0x4fb4u: /* movne r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=255u;}r[15]=0x4fb8u;return 0;
        case 0x4fb8u: /* str r1,[r4,#0xc] */
            STORE(r[4]+12u,r[1],4);r[15]=0x4fbcu;return 0;
        case 0x4fbcu: /* strb r2,[r4,#0x14] */
            STORE(r[4]+20u,r[2],1);r[15]=0x4fc0u;return 0;
        case 0x4fc0u: /* strb r2,[r4,#0x15] */
            STORE(r[4]+21u,r[2],1);r[15]=0x4fc4u;return 0;
        case 0x4fc4u: /* strb r0,[r4,#0x16] */
            STORE(r[4]+22u,r[0],1);r[15]=0x4fc8u;return 0;
        case 0x4fc8u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x4fccu;return 0;
        case 0x4fccu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x4fe0u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x4fe4u;return 0;
        case 0x4fe4u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x4fe8u;return 0;
        case 0x4fe8u: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x4fecu;return 0;
        case 0x4fecu: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x4ff0u;return 0;
        case 0x4ff0u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x4ff4u;return 0;
        case 0x4ff4u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x4ff8u;return 0;
        case 0x4ff8u: /* strb r1,[r4,#0x26] */
            STORE(r[4]+38u,r[1],1);r[15]=0x4ffcu;return 0;
        case 0x4ffcu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5000u;return 0;
        case 0x5000u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5004u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5008u;return 0;
        case 0x5008u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x500cu;return 0;
        case 0x500cu: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x5010u;return 0;
        case 0x5010u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x5014u;return 0;
        case 0x5014u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x5018u;return 0;
        case 0x5018u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x501cu;return 0;
        case 0x501cu: /* strb r1,[r4,#0x24] */
            STORE(r[4]+36u,r[1],1);r[15]=0x5020u;return 0;
        case 0x5020u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5024u;return 0;
        case 0x5024u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5028u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x502cu;return 0;
        case 0x502cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5030u;return 0;
        case 0x5030u: /* bl 0x000076d4 */
            r[14]=0x5034u;r[15]=30420u;return 0;
        case 0x5034u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5038u;return 0;
        case 0x5038u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x503cu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5040u;return 0;
        case 0x5040u: /* cmp r0,#0xff */
            (void)subtract(state,r[0],255u);r[15]=0x5044u;return 0;
        case 0x5044u: /* beq 0x000050c4 */
            if(state->cpsr&0x40000000u){r[15]=20676u;}else{r[15]=0x5048u;}return 0;
        case 0x5048u: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x504cu;return 0;
        case 0x504cu: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x5050u;return 0;
        case 0x5050u: /* mov r0,#0xc000 */
            r[0]=49152u;r[15]=0x5054u;return 0;
        case 0x5054u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x5058u;return 0;
        case 0x5058u: /* mov r1,#0x80 */
            r[1]=128u;r[15]=0x505cu;return 0;
        case 0x505cu: /* mov r2,r1, lsl #0x8 */
            r[2]=(r[1]<<8);r[15]=0x5060u;return 0;
        case 0x5060u: /* mov r3,#0x0 */
            r[3]=0u;r[15]=0x5064u;return 0;
        case 0x5064u: /* strb r1,[r4,#0x0] */
            STORE(r[4]+0u,r[1],1);r[15]=0x5068u;return 0;
        case 0x5068u: /* strb r1,[r4,#0x1] */
            STORE(r[4]+1u,r[1],1);r[15]=0x506cu;return 0;
        case 0x506cu: /* strb r3,[r4,#0x2] */
            STORE(r[4]+2u,r[3],1);r[15]=0x5070u;return 0;
        case 0x5070u: /* strb r3,[r4,#0x3] */
            STORE(r[4]+3u,r[3],1);r[15]=0x5074u;return 0;
        case 0x5074u: /* strb r3,[r4,#0x4] */
            STORE(r[4]+4u,r[3],1);r[15]=0x5078u;return 0;
        case 0x5078u: /* strb r1,[r4,#0x6] */
            STORE(r[4]+6u,r[1],1);r[15]=0x507cu;return 0;
        case 0x507cu: /* strb r1,[r4,#0x7] */
            STORE(r[4]+7u,r[1],1);r[15]=0x5080u;return 0;
        case 0x5080u: /* strb r3,[r4,#0x8] */
            STORE(r[4]+8u,r[3],1);r[15]=0x5084u;return 0;
        case 0x5084u: /* strb r3,[r4,#0x9] */
            STORE(r[4]+9u,r[3],1);r[15]=0x5088u;return 0;
        case 0x5088u: /* strb r3,[r4,#0xa] */
            STORE(r[4]+10u,r[3],1);r[15]=0x508cu;return 0;
        case 0x508cu: /* str r2,[r4,#0xc] */
            STORE(r[4]+12u,r[2],4);r[15]=0x5090u;return 0;
        case 0x5090u: /* str r2,[r4,#0x10] */
            STORE(r[4]+16u,r[2],4);r[15]=0x5094u;return 0;
        case 0x5094u: /* strb r3,[r4,#0x14] */
            STORE(r[4]+20u,r[3],1);r[15]=0x5098u;return 0;
        case 0x5098u: /* strb r3,[r4,#0x15] */
            STORE(r[4]+21u,r[3],1);r[15]=0x509cu;return 0;
        case 0x509cu: /* strb r3,[r4,#0x16] */
            STORE(r[4]+22u,r[3],1);r[15]=0x50a0u;return 0;
        case 0x50a0u: /* str r2,[r4,#0x18] */
            STORE(r[4]+24u,r[2],4);r[15]=0x50a4u;return 0;
        case 0x50a4u: /* str r2,[r4,#0x1c] */
            STORE(r[4]+28u,r[2],4);r[15]=0x50a8u;return 0;
        case 0x50a8u: /* strb r3,[r4,#0x20] */
            STORE(r[4]+32u,r[3],1);r[15]=0x50acu;return 0;
        case 0x50acu: /* strb r3,[r4,#0x21] */
            STORE(r[4]+33u,r[3],1);r[15]=0x50b0u;return 0;
        case 0x50b0u: /* strb r3,[r4,#0x22] */
            STORE(r[4]+34u,r[3],1);r[15]=0x50b4u;return 0;
        case 0x50b4u: /* strb r1,[r4,#0x24] */
            STORE(r[4]+36u,r[1],1);r[15]=0x50b8u;return 0;
        case 0x50b8u: /* strb r1,[r4,#0x26] */
            STORE(r[4]+38u,r[1],1);r[15]=0x50bcu;return 0;
        case 0x50bcu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x50c0u;return 0;
        case 0x50c0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x50c4u: /* mov r4,#0xc000 */
            r[4]=49152u;r[15]=0x50c8u;return 0;
        case 0x50c8u: /* mov r5,#0x8 */
            r[5]=8u;r[15]=0x50ccu;return 0;
        case 0x50ccu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x50d0u;return 0;
        case 0x50d0u: /* bl 0x00005058 */
            r[14]=0x50d4u;r[15]=20568u;return 0;
        case 0x50d4u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x50d8u;return 0;
        case 0x50d8u: /* add r4,r4,#0x80 */
            r[4]=r[4]+128u;r[15]=0x50dcu;return 0;
        case 0x50dcu: /* subs r5,r5,#0x1 */
            r[5]=subtract(state,r[5],1u);r[15]=0x50e0u;return 0;
        case 0x50e0u: /* bne 0x000050cc */
            if(!(state->cpsr&0x40000000u)){r[15]=20684u;}else{r[15]=0x50e4u;}return 0;
        case 0x50e4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x50e8u: /* mov r1,#0xe800 */
            r[1]=59392u;r[15]=0x50ecu;return 0;
        case 0x50ecu: /* mov r2,#0x30 */
            r[2]=48u;r[15]=0x50f0u;return 0;
        case 0x50f0u: /* mov r3,#0x800000 */
            r[3]=8388608u;r[15]=0x50f4u;return 0;
        case 0x50f4u: /* mov r4,#0x0 */
            r[4]=0u;r[15]=0x50f8u;return 0;
        case 0x50f8u: /* str r4,[r1,#0x0] */
            STORE(r[1]+0u,r[4],4);r[15]=0x50fcu;return 0;
        case 0x50fcu: /* ldr r5,[r3,#0x0] */
            LOAD(r[5],r[3]+0u,4);r[15]=0x5100u;return 0;
        case 0x5100u: /* ldr r4,[0x5178] */
            LOAD(r[4],20856u,4);r[15]=0x5104u;return 0;
        case 0x5104u: /* and r5,r5,r4 */
            r[5]=r[5]&r[4];r[15]=0x5108u;return 0;
        case 0x5108u: /* orr r5,r5,#0x8000 */
            r[5]=r[5]|32768u;r[15]=0x510cu;return 0;
        case 0x510cu: /* str r5,[r3,#0x0] */
            STORE(r[3]+0u,r[5],4);r[15]=0x5110u;return 0;
        case 0x5110u: /* add r1,r1,#0x40 */
            r[1]=r[1]+64u;r[15]=0x5114u;return 0;
        case 0x5114u: /* add r3,r3,#0x80 */
            r[3]=r[3]+128u;r[15]=0x5118u;return 0;
        case 0x5118u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x511cu;return 0;
        case 0x511cu: /* bne 0x000050f4 */
            if(!(state->cpsr&0x40000000u)){r[15]=20724u;}else{r[15]=0x5120u;}return 0;
        case 0x5120u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5124u;return 0;
        case 0x5124u: /* mov r2,#0xc000 */
            r[2]=49152u;r[15]=0x5128u;return 0;
        case 0x5128u: /* stmdb sp!,{r0,r2,lr} */
            STORE(r[13]-12u,r[0],4);STORE(r[13]-8u,r[2],4);STORE(r[13]-4u,r[14],4);r[13]-=12u;r[15]=0x512cu;return 0;
        case 0x512cu: /* bl 0x000074f4 */
            r[14]=0x5130u;r[15]=29940u;return 0;
        case 0x5130u: /* ldmia sp!,{r0,r2,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[2],r[13]+4u,4);LOAD(r[14],r[13]+8u,4);r[13]+=12u;r[15]=0x5134u;return 0;
        case 0x5134u: /* ldrb r3,[r2,#0x28] */
            LOAD(r[3],r[2]+40u,1);r[15]=0x5138u;return 0;
        case 0x5138u: /* and r3,r3,#0xfc */
            r[3]=r[3]&252u;r[15]=0x513cu;return 0;
        case 0x513cu: /* and r1,r1,#0x3 */
            r[1]=r[1]&3u;r[15]=0x5140u;return 0;
        case 0x5140u: /* orr r3,r3,r1 */
            r[3]=r[3]|r[1];r[15]=0x5144u;return 0;
        case 0x5144u: /* ldr r4,[0x5180] */
            LOAD(r[4],20864u,4);r[15]=0x5148u;return 0;
        case 0x5148u: /* ldrb r1,[r4,#0x0] */
            LOAD(r[1],r[4]+0u,1);r[15]=0x514cu;return 0;
        case 0x514cu: /* strb r1,[r2,#0x29] */
            STORE(r[2]+41u,r[1],1);r[15]=0x5150u;return 0;
        case 0x5150u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x5154u;return 0;
        case 0x5154u: /* orrne r3,r3,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[3]|128u;}r[15]=0x5158u;return 0;
        case 0x5158u: /* biceq r3,r3,#0x80 */
            if(state->cpsr&0x40000000u){r[3]=r[3]&~128u;}r[15]=0x515cu;return 0;
        case 0x515cu: /* strb r3,[r2,#0x28] */
            STORE(r[2]+40u,r[3],1);r[15]=0x5160u;return 0;
        case 0x5160u: /* add r2,r2,#0x80 */
            r[2]=r[2]+128u;r[15]=0x5164u;return 0;
        case 0x5164u: /* add r0,r0,#0x1 */
            r[0]=r[0]+1u;r[15]=0x5168u;return 0;
        case 0x5168u: /* cmp r0,#0x8 */
            (void)subtract(state,r[0],8u);r[15]=0x516cu;return 0;
        case 0x516cu: /* bne 0x00005128 */
            if(!(state->cpsr&0x40000000u)){r[15]=20776u;}else{r[15]=0x5170u;}return 0;
        case 0x5170u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5184u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5188u;return 0;
        case 0x5188u: /* ldrb r3,[r10,#0x3] */
            LOAD(r[3],r[10]+3u,1);r[15]=0x518cu;return 0;
        case 0x518cu: /* mov r3,r3, lsl #0x3 */
            r[3]=(r[3]<<3);r[15]=0x5190u;return 0;
        case 0x5190u: /* ldr r4,[0x54ec] */
            LOAD(r[4],21740u,4);r[15]=0x5194u;return 0;
        case 0x5194u: /* add r4,r4,r3 */
            r[4]=r[4]+r[3];r[15]=0x5198u;return 0;
        case 0x5198u: /* ldr r3,[r4,#0x0] */
            LOAD(r[3],r[4]+0u,4);r[15]=0x519cu;return 0;
        case 0x519cu: /* ldr r5,[r3,#0x0] */
            LOAD(r[5],r[3]+0u,4);r[15]=0x51a0u;return 0;
        case 0x51a0u: /* ldr r4,[0x526c] */
            LOAD(r[4],21100u,4);r[15]=0x51a4u;return 0;
        case 0x51a4u: /* cmp r5,r4 */
            (void)subtract(state,r[5],r[4]);r[15]=0x51a8u;return 0;
        case 0x51a8u: /* movne r1,#0x2 */
            if(!(state->cpsr&0x40000000u)){r[1]=2u;}r[15]=0x51acu;return 0;
        case 0x51acu: /* bne 0x00005234 */
            if(!(state->cpsr&0x40000000u)){r[15]=21044u;}else{r[15]=0x51b0u;}return 0;
        case 0x51b0u: /* ldrb r5,[r3,#0x4] */
            LOAD(r[5],r[3]+4u,1);r[15]=0x51b4u;return 0;
        case 0x51b4u: /* cmp r5,#0x1 */
            (void)subtract(state,r[5],1u);r[15]=0x51b8u;return 0;
        case 0x51b8u: /* movne r1,#0x8 */
            if(!(state->cpsr&0x40000000u)){r[1]=8u;}r[15]=0x51bcu;return 0;
        case 0x51bcu: /* bne 0x00005234 */
            if(!(state->cpsr&0x40000000u)){r[15]=21044u;}else{r[15]=0x51c0u;}return 0;
        case 0x51c0u: /* ldr r6,[r3,#0x8] */
            LOAD(r[6],r[3]+8u,4);r[15]=0x51c4u;return 0;
        case 0x51c4u: /* sub r6,r6,#0x4 */
            r[6]=r[6]-4u;r[15]=0x51c8u;return 0;
        case 0x51c8u: /* ldr r5,[r3,r6] */
            LOAD(r[5],r[3]+r[6],4);r[15]=0x51ccu;return 0;
        case 0x51ccu: /* ldr r6,[0x5270] */
            LOAD(r[6],21104u,4);r[15]=0x51d0u;return 0;
        case 0x51d0u: /* cmp r5,r6 */
            (void)subtract(state,r[5],r[6]);r[15]=0x51d4u;return 0;
        case 0x51d4u: /* movne r1,#0x4 */
            if(!(state->cpsr&0x40000000u)){r[1]=4u;}r[15]=0x51d8u;return 0;
        case 0x51d8u: /* bne 0x00005234 */
            if(!(state->cpsr&0x40000000u)){r[15]=21044u;}else{r[15]=0x51dcu;}return 0;
        case 0x51dcu: /* ldrb r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,1);r[15]=0x51e0u;return 0;
        case 0x51e0u: /* ldrb r2,[r10,#0x5] */
            LOAD(r[2],r[10]+5u,1);r[15]=0x51e4u;return 0;
        case 0x51e4u: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x51e8u;return 0;
        case 0x51e8u: /* bl 0x00007814 */
            r[14]=0x51ecu;r[15]=30740u;return 0;
        case 0x51ecu: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x51f0u;return 0;
        case 0x51f0u: /* mov r4,#0x11000 */
            r[4]=69632u;r[15]=0x51f4u;return 0;
        case 0x51f4u: /* mov r1,#0x58 */
            r[1]=88u;r[15]=0x51f8u;return 0;
        case 0x51f8u: /* mla r4,r0,r1,r4 */
            r[4]=r[0]*r[1]+r[4];r[15]=0x51fcu;return 0;
        case 0x51fcu: /* ldrb r1,[r4,#0x1] */
            LOAD(r[1],r[4]+1u,1);r[15]=0x5200u;return 0;
        case 0x5200u: /* mov r2,#0x13800 */
            r[2]=79872u;r[15]=0x5204u;return 0;
        case 0x5204u: /* mov r0,r0, lsl #0x5 */
            r[0]=(r[0]<<5);r[15]=0x5208u;return 0;
        case 0x5208u: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x520cu;return 0;
        case 0x520cu: /* ldrb r0,[r4,#0x0] */
            LOAD(r[0],r[4]+0u,1);r[15]=0x5210u;return 0;
        case 0x5210u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x5214u;return 0;
        case 0x5214u: /* orrne r0,r0,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[0]=r[0]|128u;}r[15]=0x5218u;return 0;
        case 0x5218u: /* biceq r0,r0,#0x80 */
            if(state->cpsr&0x40000000u){r[0]=r[0]&~128u;}r[15]=0x521cu;return 0;
        case 0x521cu: /* strb r0,[r4,#0x0] */
            STORE(r[4]+0u,r[0],1);r[15]=0x5220u;return 0;
        case 0x5220u: /* strb r1,[r2,#0x1c] */
            STORE(r[2]+28u,r[1],1);r[15]=0x5224u;return 0;
        case 0x5224u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x5228u;return 0;
        case 0x5228u: /* strb r1,[r4,#0x1] */
            STORE(r[4]+1u,r[1],1);r[15]=0x522cu;return 0;
        case 0x522cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5234u: /* mov r4,#0x11000 */
            r[4]=69632u;r[15]=0x5238u;return 0;
        case 0x5238u: /* mov r2,#0x58 */
            r[2]=88u;r[15]=0x523cu;return 0;
        case 0x523cu: /* mla r4,r0,r2,r4 */
            r[4]=r[0]*r[2]+r[4];r[15]=0x5240u;return 0;
        case 0x5240u: /* ldrb r2,[r4,#0x0] */
            LOAD(r[2],r[4]+0u,1);r[15]=0x5244u;return 0;
        case 0x5244u: /* orr r2,r2,#0x80 */
            r[2]=r[2]|128u;r[15]=0x5248u;return 0;
        case 0x5248u: /* strb r2,[r4,#0x0] */
            STORE(r[4]+0u,r[2],1);r[15]=0x524cu;return 0;
        case 0x524cu: /* mov r2,#0x13800 */
            r[2]=79872u;r[15]=0x5250u;return 0;
        case 0x5250u: /* mov r0,r0, lsl #0x5 */
            r[0]=(r[0]<<5);r[15]=0x5254u;return 0;
        case 0x5254u: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x5258u;return 0;
        case 0x5258u: /* ldrb r0,[r2,#0x1f] */
            LOAD(r[0],r[2]+31u,1);r[15]=0x525cu;return 0;
        case 0x525cu: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0x5260u;return 0;
        case 0x5260u: /* strb r0,[r2,#0x1f] */
            STORE(r[2]+31u,r[0],1);r[15]=0x5264u;return 0;
        case 0x5264u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5274u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5278u;return 0;
        case 0x5278u: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x527cu;return 0;
        case 0x527cu: /* bl 0x00007dc4 */
            r[14]=0x5280u;r[15]=32196u;return 0;
        case 0x5280u: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x5284u;return 0;
        case 0x5284u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5288u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x528cu;return 0;
        case 0x528cu: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x5290u;return 0;
        case 0x5290u: /* bl 0x00007f78 */
            r[14]=0x5294u;r[15]=32632u;return 0;
        case 0x5294u: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x5298u;return 0;
        case 0x5298u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x529cu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x52a0u;return 0;
        case 0x52a0u: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x52a4u;return 0;
        case 0x52a4u: /* bl 0x00007fd4 */
            r[14]=0x52a8u;r[15]=32724u;return 0;
        case 0x52a8u: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x52acu;return 0;
        case 0x52acu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x52b0u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x52b4u;return 0;
        case 0x52b4u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x52b8u;return 0;
        case 0x52b8u: /* ldr r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,4);r[15]=0x52bcu;return 0;
        case 0x52bcu: /* ldr r3,[0x52d4] */
            LOAD(r[3],21204u,4);r[15]=0x52c0u;return 0;
        case 0x52c0u: /* and r2,r2,r3 */
            r[2]=r[2]&r[3];r[15]=0x52c4u;return 0;
        case 0x52c4u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x52c8u;return 0;
        case 0x52c8u: /* bl 0x00006d8c */
            r[14]=0x52ccu;r[15]=28044u;return 0;
        case 0x52ccu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x52d0u;return 0;
        case 0x52d0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x52d8u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x52dcu;return 0;
        case 0x52dcu: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x52e0u;return 0;
        case 0x52e0u: /* ldr r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,4);r[15]=0x52e4u;return 0;
        case 0x52e4u: /* ldr r3,[0x52fc] */
            LOAD(r[3],21244u,4);r[15]=0x52e8u;return 0;
        case 0x52e8u: /* and r2,r2,r3 */
            r[2]=r[2]&r[3];r[15]=0x52ecu;return 0;
        case 0x52ecu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x52f0u;return 0;
        case 0x52f0u: /* bl 0x00006e14 */
            r[14]=0x52f4u;r[15]=28180u;return 0;
        case 0x52f4u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x52f8u;return 0;
        case 0x52f8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5300u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5304u;return 0;
        case 0x5304u: /* ldr r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,4);r[15]=0x5308u;return 0;
        case 0x5308u: /* mov r2,r1, lsr #0x10 */
            r[2]=(r[1]>>16);r[15]=0x530cu;return 0;
        case 0x530cu: /* ldr r3,[0x5390] */
            LOAD(r[3],21392u,4);r[15]=0x5310u;return 0;
        case 0x5310u: /* and r1,r1,r3 */
            r[1]=r[1]&r[3];r[15]=0x5314u;return 0;
        case 0x5314u: /* mov r4,#0x58 */
            r[4]=88u;r[15]=0x5318u;return 0;
        case 0x5318u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x531cu;return 0;
        case 0x531cu: /* mov r0,#0x11000 */
            r[0]=69632u;r[15]=0x5320u;return 0;
        case 0x5320u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x5324u;return 0;
        case 0x5324u: /* ands r2,r2,r3 */
            temporary=r[2]&r[3];r[2]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x5328u;return 0;
        case 0x5328u: /* beq 0x0000536c */
            if(state->cpsr&0x40000000u){r[15]=21356u;}else{r[15]=0x532cu;}return 0;
        case 0x532cu: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x5330u;return 0;
        case 0x5330u: /* ldr r0,[r4,#0x44] */
            LOAD(r[0],r[4]+68u,4);r[15]=0x5334u;return 0;
        case 0x5334u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x5338u;return 0;
        case 0x5338u: /* ldr r3,[0x5394] */
            LOAD(r[3],21396u,4);r[15]=0x533cu;return 0;
        case 0x533cu: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x5340u;return 0;
        case 0x5340u: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x5344u;return 0;
        case 0x5344u: /* ldr r3,[0x5390] */
            LOAD(r[3],21392u,4);r[15]=0x5348u;return 0;
        case 0x5348u: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0x534cu;return 0;
        case 0x534cu: /* ldr r3,[0x5398] */
            LOAD(r[3],21400u,4);r[15]=0x5350u;return 0;
        case 0x5350u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x5354u;return 0;
        case 0x5354u: /* beq 0x0000536c */
            if(state->cpsr&0x40000000u){r[15]=21356u;}else{r[15]=0x5358u;}return 0;
        case 0x5358u: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x535cu;return 0;
        case 0x535cu: /* bne 0x0000536c */
            if(!(state->cpsr&0x40000000u)){r[15]=21356u;}else{r[15]=0x5360u;}return 0;
        case 0x5360u: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x5364u;return 0;
        case 0x5364u: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x5368u;return 0;
        case 0x5368u: /* b 0x00005350 */
            r[15]=21328u;return 0;
        case 0x536cu: /* ldr r3,[0x539c] */
            LOAD(r[3],21404u,4);r[15]=0x5370u;return 0;
        case 0x5370u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x5374u;return 0;
        case 0x5374u: /* movne r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=255u;}r[15]=0x5378u;return 0;
        case 0x5378u: /* str r1,[r4,#0x40] */
            STORE(r[4]+64u,r[1],4);r[15]=0x537cu;return 0;
        case 0x537cu: /* strb r2,[r4,#0x48] */
            STORE(r[4]+72u,r[2],1);r[15]=0x5380u;return 0;
        case 0x5380u: /* strb r2,[r4,#0x49] */
            STORE(r[4]+73u,r[2],1);r[15]=0x5384u;return 0;
        case 0x5384u: /* strb r0,[r4,#0x4a] */
            STORE(r[4]+74u,r[0],1);r[15]=0x5388u;return 0;
        case 0x5388u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x538cu;return 0;
        case 0x538cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x53a0u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x53a4u;return 0;
        case 0x53a4u: /* ldr r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,4);r[15]=0x53a8u;return 0;
        case 0x53a8u: /* mov r2,r1, lsr #0x10 */
            r[2]=(r[1]>>16);r[15]=0x53acu;return 0;
        case 0x53acu: /* ldr r3,[0x5430] */
            LOAD(r[3],21552u,4);r[15]=0x53b0u;return 0;
        case 0x53b0u: /* and r1,r1,r3 */
            r[1]=r[1]&r[3];r[15]=0x53b4u;return 0;
        case 0x53b4u: /* mov r4,#0x58 */
            r[4]=88u;r[15]=0x53b8u;return 0;
        case 0x53b8u: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x53bcu;return 0;
        case 0x53bcu: /* mov r0,#0x11000 */
            r[0]=69632u;r[15]=0x53c0u;return 0;
        case 0x53c0u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x53c4u;return 0;
        case 0x53c4u: /* ands r2,r2,r3 */
            temporary=r[2]&r[3];r[2]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x53c8u;return 0;
        case 0x53c8u: /* beq 0x0000540c */
            if(state->cpsr&0x40000000u){r[15]=21516u;}else{r[15]=0x53ccu;}return 0;
        case 0x53ccu: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x53d0u;return 0;
        case 0x53d0u: /* ldr r0,[r4,#0x50] */
            LOAD(r[0],r[4]+80u,4);r[15]=0x53d4u;return 0;
        case 0x53d4u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x53d8u;return 0;
        case 0x53d8u: /* ldr r3,[0x5434] */
            LOAD(r[3],21556u,4);r[15]=0x53dcu;return 0;
        case 0x53dcu: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x53e0u;return 0;
        case 0x53e0u: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x53e4u;return 0;
        case 0x53e4u: /* ldr r3,[0x5430] */
            LOAD(r[3],21552u,4);r[15]=0x53e8u;return 0;
        case 0x53e8u: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0x53ecu;return 0;
        case 0x53ecu: /* ldr r3,[0x5438] */
            LOAD(r[3],21560u,4);r[15]=0x53f0u;return 0;
        case 0x53f0u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x53f4u;return 0;
        case 0x53f4u: /* beq 0x0000540c */
            if(state->cpsr&0x40000000u){r[15]=21516u;}else{r[15]=0x53f8u;}return 0;
        case 0x53f8u: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x53fcu;return 0;
        case 0x53fcu: /* bne 0x0000540c */
            if(!(state->cpsr&0x40000000u)){r[15]=21516u;}else{r[15]=0x5400u;}return 0;
        case 0x5400u: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x5404u;return 0;
        case 0x5404u: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x5408u;return 0;
        case 0x5408u: /* b 0x000053f0 */
            r[15]=21488u;return 0;
        case 0x540cu: /* ldr r3,[0x543c] */
            LOAD(r[3],21564u,4);r[15]=0x5410u;return 0;
        case 0x5410u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x5414u;return 0;
        case 0x5414u: /* movne r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=255u;}r[15]=0x5418u;return 0;
        case 0x5418u: /* str r1,[r4,#0x4c] */
            STORE(r[4]+76u,r[1],4);r[15]=0x541cu;return 0;
        case 0x541cu: /* strb r2,[r4,#0x54] */
            STORE(r[4]+84u,r[2],1);r[15]=0x5420u;return 0;
        case 0x5420u: /* strb r2,[r4,#0x55] */
            STORE(r[4]+85u,r[2],1);r[15]=0x5424u;return 0;
        case 0x5424u: /* strb r0,[r4,#0x56] */
            STORE(r[4]+86u,r[0],1);r[15]=0x5428u;return 0;
        case 0x5428u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x542cu;return 0;
        case 0x542cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5440u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5444u;return 0;
        case 0x5444u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x5448u;return 0;
        case 0x5448u: /* ldrb r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,1);r[15]=0x544cu;return 0;
        case 0x544cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5450u;return 0;
        case 0x5450u: /* bl 0x00006edc */
            r[14]=0x5454u;r[15]=28380u;return 0;
        case 0x5454u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5458u;return 0;
        case 0x5458u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x545cu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5460u;return 0;
        case 0x5460u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x5464u;return 0;
        case 0x5464u: /* ldrb r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,1);r[15]=0x5468u;return 0;
        case 0x5468u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x546cu;return 0;
        case 0x546cu: /* bl 0x00006f70 */
            r[14]=0x5470u;r[15]=28528u;return 0;
        case 0x5470u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5474u;return 0;
        case 0x5474u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5478u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x547cu;return 0;
        case 0x547cu: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x5480u;return 0;
        case 0x5480u: /* ldrb r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,1);r[15]=0x5484u;return 0;
        case 0x5484u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5488u;return 0;
        case 0x5488u: /* bl 0x00006ff8 */
            r[14]=0x548cu;r[15]=28664u;return 0;
        case 0x548cu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5490u;return 0;
        case 0x5490u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5494u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5498u;return 0;
        case 0x5498u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x549cu;return 0;
        case 0x549cu: /* ldr r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,4);r[15]=0x54a0u;return 0;
        case 0x54a0u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x54a4u;return 0;
        case 0x54a4u: /* bl 0x000081d4 */
            r[14]=0x54a8u;r[15]=33236u;return 0;
        case 0x54a8u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x54acu;return 0;
        case 0x54acu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x54b0u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x54b4u;return 0;
        case 0x54b4u: /* cmp r0,#0xff */
            (void)subtract(state,r[0],255u);r[15]=0x54b8u;return 0;
        case 0x54b8u: /* beq 0x000054cc */
            if(state->cpsr&0x40000000u){r[15]=21708u;}else{r[15]=0x54bcu;}return 0;
        case 0x54bcu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x54c0u;return 0;
        case 0x54c0u: /* bl 0x00008018 */
            r[14]=0x54c4u;r[15]=32792u;return 0;
        case 0x54c4u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x54c8u;return 0;
        case 0x54c8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x54ccu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x54d0u;return 0;
        case 0x54d0u: /* bl 0x0000800c */
            r[14]=0x54d4u;r[15]=32780u;return 0;
        case 0x54d4u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x54d8u;return 0;
        case 0x54d8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x54dcu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x54e0u;return 0;
        case 0x54e0u: /* bl 0x00007e94 */
            r[14]=0x54e4u;r[15]=32404u;return 0;
        case 0x54e4u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x54e8u;return 0;
        case 0x54e8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x54f0u: /* ldrb r2,[r10,#0x2] */
            LOAD(r[2],r[10]+2u,1);r[15]=0x54f4u;return 0;
        case 0x54f4u: /* mov r1,r2, lsr #0x4 */
            r[1]=(r[2]>>4);r[15]=0x54f8u;return 0;
        case 0x54f8u: /* and r1,r1,#0x3 */
            r[1]=r[1]&3u;r[15]=0x54fcu;return 0;
        case 0x54fcu: /* and r0,r2,#0xf */
            r[0]=r[2]&15u;r[15]=0x5500u;return 0;
        case 0x5500u: /* ldrb r3,[r10,#0x3] */
            LOAD(r[3],r[10]+3u,1);r[15]=0x5504u;return 0;
        case 0x5504u: /* ldr r4,[r10,#0x4] */
            LOAD(r[4],r[10]+4u,4);r[15]=0x5508u;return 0;
        case 0x5508u: /* ldr r5,[0x55c4] */
            LOAD(r[5],21956u,4);r[15]=0x550cu;return 0;
        case 0x550cu: /* and r4,r4,r5 */
            r[4]=r[4]&r[5];r[15]=0x5510u;return 0;
        case 0x5510u: /* ldrb r5,[r10,#0x6] */
            LOAD(r[5],r[10]+6u,1);r[15]=0x5514u;return 0;
        case 0x5514u: /* ldrb r6,[r10,#0x7] */
            LOAD(r[6],r[10]+7u,1);r[15]=0x5518u;return 0;
        case 0x5518u: /* ldrb r7,[r10,#0x8] */
            LOAD(r[7],r[10]+8u,1);r[15]=0x551cu;return 0;
        case 0x551cu: /* ands r2,r2,#0x80 */
            temporary=r[2]&128u;r[2]=temporary;logic_flags(state,temporary,0,128u);r[15]=0x5520u;return 0;
        case 0x5520u: /* moveq r7,#0x0 */
            if(state->cpsr&0x40000000u){r[7]=0u;}r[15]=0x5524u;return 0;
        case 0x5524u: /* orr r7,r6,r7, lsl #0x8 */
            r[7]=r[6]|(r[7]<<8);r[15]=0x5528u;return 0;
        case 0x5528u: /* orr r2,r7,r2, lsl #0x9 */
            r[2]=r[7]|(r[2]<<9);r[15]=0x552cu;return 0;
        case 0x552cu: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x5530u;return 0;
        case 0x5530u: /* bl 0x00007944 */
            r[14]=0x5534u;r[15]=31044u;return 0;
        case 0x5534u: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x5538u;return 0;
        case 0x5538u: /* ldr r5,[0x585c] */
            LOAD(r[5],22620u,4);r[15]=0x553cu;return 0;
        case 0x553cu: /* mov r3,r0, lsl #0x6 */
            r[3]=(r[0]<<6);r[15]=0x5540u;return 0;
        case 0x5540u: /* add r5,r5,r3 */
            r[5]=r[5]+r[3];r[15]=0x5544u;return 0;
        case 0x5544u: /* and r1,r1,#0x17 */
            r[1]=r[1]&23u;r[15]=0x5548u;return 0;
        case 0x5548u: /* and r2,r2,#0x17 */
            r[2]=r[2]&23u;r[15]=0x554cu;return 0;
        case 0x554cu: /* orr r3,r3,r1 */
            r[3]=r[3]|r[1];r[15]=0x5550u;return 0;
        case 0x5550u: /* ldrb r3,[r5,#0x0] */
            LOAD(r[3],r[5]+0u,1);r[15]=0x5554u;return 0;
        case 0x5554u: /* and r3,r3,#0xe8 */
            r[3]=r[3]&232u;r[15]=0x5558u;return 0;
        case 0x5558u: /* strb r3,[r5,#0x0] */
            STORE(r[5]+0u,r[3],1);r[15]=0x555cu;return 0;
        case 0x555cu: /* ldrb r3,[r5,#0x20] */
            LOAD(r[3],r[5]+32u,1);r[15]=0x5560u;return 0;
        case 0x5560u: /* and r3,r3,#0xe8 */
            r[3]=r[3]&232u;r[15]=0x5564u;return 0;
        case 0x5564u: /* strb r3,[r5,#0x20] */
            STORE(r[5]+32u,r[3],1);r[15]=0x5568u;return 0;
        case 0x5568u: /* ldr r4,[0x5860] */
            LOAD(r[4],22624u,4);r[15]=0x556cu;return 0;
        case 0x556cu: /* mov r1,#0xb0 */
            r[1]=176u;r[15]=0x5570u;return 0;
        case 0x5570u: /* mla r4,r0,r1,r4 */
            r[4]=r[0]*r[1]+r[4];r[15]=0x5574u;return 0;
        case 0x5574u: /* ldrb r1,[r4,#0x1] */
            LOAD(r[1],r[4]+1u,1);r[15]=0x5578u;return 0;
        case 0x5578u: /* ldrb r0,[r4,#0x0] */
            LOAD(r[0],r[4]+0u,1);r[15]=0x557cu;return 0;
        case 0x557cu: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x5580u;return 0;
        case 0x5580u: /* orrne r0,r0,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[0]=r[0]|128u;}r[15]=0x5584u;return 0;
        case 0x5584u: /* biceq r0,r0,#0x80 */
            if(state->cpsr&0x40000000u){r[0]=r[0]&~128u;}r[15]=0x5588u;return 0;
        case 0x5588u: /* strb r0,[r4,#0x0] */
            STORE(r[4]+0u,r[0],1);r[15]=0x558cu;return 0;
        case 0x558cu: /* strb r1,[r5,#0x1c] */
            STORE(r[5]+28u,r[1],1);r[15]=0x5590u;return 0;
        case 0x5590u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5594u;return 0;
        case 0x5594u: /* strb r0,[r4,#0x1] */
            STORE(r[4]+1u,r[0],1);r[15]=0x5598u;return 0;
        case 0x5598u: /* add r4,r4,#0x58 */
            r[4]=r[4]+88u;r[15]=0x559cu;return 0;
        case 0x559cu: /* ldrb r1,[r4,#0x1] */
            LOAD(r[1],r[4]+1u,1);r[15]=0x55a0u;return 0;
        case 0x55a0u: /* ldrb r0,[r4,#0x0] */
            LOAD(r[0],r[4]+0u,1);r[15]=0x55a4u;return 0;
        case 0x55a4u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x55a8u;return 0;
        case 0x55a8u: /* orrne r0,r0,#0x80 */
            if(!(state->cpsr&0x40000000u)){r[0]=r[0]|128u;}r[15]=0x55acu;return 0;
        case 0x55acu: /* biceq r0,r0,#0x80 */
            if(state->cpsr&0x40000000u){r[0]=r[0]&~128u;}r[15]=0x55b0u;return 0;
        case 0x55b0u: /* strb r0,[r4,#0x0] */
            STORE(r[4]+0u,r[0],1);r[15]=0x55b4u;return 0;
        case 0x55b4u: /* strb r1,[r5,#0x3c] */
            STORE(r[5]+60u,r[1],1);r[15]=0x55b8u;return 0;
        case 0x55b8u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x55bcu;return 0;
        case 0x55bcu: /* strb r0,[r4,#0x1] */
            STORE(r[4]+1u,r[0],1);r[15]=0x55c0u;return 0;
        case 0x55c0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x55c8u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x55ccu;return 0;
        case 0x55ccu: /* stmdb sp!,{r0,lr} */
            STORE(r[13]-8u,r[0],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x55d0u;return 0;
        case 0x55d0u: /* bl 0x00007df4 */
            r[14]=0x55d4u;r[15]=32244u;return 0;
        case 0x55d4u: /* ldmia sp!,{r0,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x55d8u;return 0;
        case 0x55d8u: /* ldr r5,[0x585c] */
            LOAD(r[5],22620u,4);r[15]=0x55dcu;return 0;
        case 0x55dcu: /* mov r3,r0, lsl #0x6 */
            r[3]=(r[0]<<6);r[15]=0x55e0u;return 0;
        case 0x55e0u: /* add r5,r5,r3 */
            r[5]=r[5]+r[3];r[15]=0x55e4u;return 0;
        case 0x55e4u: /* and r1,r1,#0x17 */
            r[1]=r[1]&23u;r[15]=0x55e8u;return 0;
        case 0x55e8u: /* and r2,r2,#0x17 */
            r[2]=r[2]&23u;r[15]=0x55ecu;return 0;
        case 0x55ecu: /* orr r3,r3,r1 */
            r[3]=r[3]|r[1];r[15]=0x55f0u;return 0;
        case 0x55f0u: /* ldrb r3,[r5,#0x0] */
            LOAD(r[3],r[5]+0u,1);r[15]=0x55f4u;return 0;
        case 0x55f4u: /* and r3,r3,#0xe8 */
            r[3]=r[3]&232u;r[15]=0x55f8u;return 0;
        case 0x55f8u: /* strb r3,[r5,#0x0] */
            STORE(r[5]+0u,r[3],1);r[15]=0x55fcu;return 0;
        case 0x55fcu: /* ldrb r3,[r5,#0x20] */
            LOAD(r[3],r[5]+32u,1);r[15]=0x5600u;return 0;
        case 0x5600u: /* and r3,r3,#0xe8 */
            r[3]=r[3]&232u;r[15]=0x5604u;return 0;
        case 0x5604u: /* strb r3,[r5,#0x20] */
            STORE(r[5]+32u,r[3],1);r[15]=0x5608u;return 0;
        case 0x5608u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x560cu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5610u;return 0;
        case 0x5610u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x5614u;return 0;
        case 0x5614u: /* ldr r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,4);r[15]=0x5618u;return 0;
        case 0x5618u: /* ldrb r4,[r10,#0x7] */
            LOAD(r[4],r[10]+7u,1);r[15]=0x561cu;return 0;
        case 0x561cu: /* ldr r5,[r10,#0x8] */
            LOAD(r[5],r[10]+8u,4);r[15]=0x5620u;return 0;
        case 0x5620u: /* ldr r3,[0x563c] */
            LOAD(r[3],22076u,4);r[15]=0x5624u;return 0;
        case 0x5624u: /* and r2,r2,r3 */
            r[2]=r[2]&r[3];r[15]=0x5628u;return 0;
        case 0x5628u: /* and r5,r5,r3 */
            r[5]=r[5]&r[3];r[15]=0x562cu;return 0;
        case 0x562cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5630u;return 0;
        case 0x5630u: /* bl 0x00006da4 */
            r[14]=0x5634u;r[15]=28068u;return 0;
        case 0x5634u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5638u;return 0;
        case 0x5638u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5640u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5644u;return 0;
        case 0x5644u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x5648u;return 0;
        case 0x5648u: /* ldr r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,4);r[15]=0x564cu;return 0;
        case 0x564cu: /* ldrb r4,[r10,#0x7] */
            LOAD(r[4],r[10]+7u,1);r[15]=0x5650u;return 0;
        case 0x5650u: /* ldr r5,[r10,#0x8] */
            LOAD(r[5],r[10]+8u,4);r[15]=0x5654u;return 0;
        case 0x5654u: /* ldr r3,[0x5670] */
            LOAD(r[3],22128u,4);r[15]=0x5658u;return 0;
        case 0x5658u: /* and r2,r2,r3 */
            r[2]=r[2]&r[3];r[15]=0x565cu;return 0;
        case 0x565cu: /* and r5,r5,r3 */
            r[5]=r[5]&r[3];r[15]=0x5660u;return 0;
        case 0x5660u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5664u;return 0;
        case 0x5664u: /* bl 0x00006e2c */
            r[14]=0x5668u;r[15]=28204u;return 0;
        case 0x5668u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x566cu;return 0;
        case 0x566cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5674u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5678u;return 0;
        case 0x5678u: /* ldr r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,4);r[15]=0x567cu;return 0;
        case 0x567cu: /* mov r2,r1, lsr #0x10 */
            r[2]=(r[1]>>16);r[15]=0x5680u;return 0;
        case 0x5680u: /* ldr r3,[0x5714] */
            LOAD(r[3],22292u,4);r[15]=0x5684u;return 0;
        case 0x5684u: /* and r1,r1,r3 */
            r[1]=r[1]&r[3];r[15]=0x5688u;return 0;
        case 0x5688u: /* mov r4,#0xb0 */
            r[4]=176u;r[15]=0x568cu;return 0;
        case 0x568cu: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x5690u;return 0;
        case 0x5690u: /* ldr r0,[0x5860] */
            LOAD(r[0],22624u,4);r[15]=0x5694u;return 0;
        case 0x5694u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x5698u;return 0;
        case 0x5698u: /* ands r2,r2,r3 */
            temporary=r[2]&r[3];r[2]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x569cu;return 0;
        case 0x569cu: /* beq 0x000056e0 */
            if(state->cpsr&0x40000000u){r[15]=22240u;}else{r[15]=0x56a0u;}return 0;
        case 0x56a0u: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x56a4u;return 0;
        case 0x56a4u: /* ldr r0,[r4,#0x44] */
            LOAD(r[0],r[4]+68u,4);r[15]=0x56a8u;return 0;
        case 0x56a8u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x56acu;return 0;
        case 0x56acu: /* ldr r3,[0x5718] */
            LOAD(r[3],22296u,4);r[15]=0x56b0u;return 0;
        case 0x56b0u: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x56b4u;return 0;
        case 0x56b4u: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x56b8u;return 0;
        case 0x56b8u: /* ldr r3,[0x5714] */
            LOAD(r[3],22292u,4);r[15]=0x56bcu;return 0;
        case 0x56bcu: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0x56c0u;return 0;
        case 0x56c0u: /* ldr r3,[0x571c] */
            LOAD(r[3],22300u,4);r[15]=0x56c4u;return 0;
        case 0x56c4u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x56c8u;return 0;
        case 0x56c8u: /* beq 0x000056e0 */
            if(state->cpsr&0x40000000u){r[15]=22240u;}else{r[15]=0x56ccu;}return 0;
        case 0x56ccu: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x56d0u;return 0;
        case 0x56d0u: /* bne 0x000056e0 */
            if(!(state->cpsr&0x40000000u)){r[15]=22240u;}else{r[15]=0x56d4u;}return 0;
        case 0x56d4u: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x56d8u;return 0;
        case 0x56d8u: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x56dcu;return 0;
        case 0x56dcu: /* b 0x000056c4 */
            r[15]=22212u;return 0;
        case 0x56e0u: /* ldr r3,[0x5720] */
            LOAD(r[3],22304u,4);r[15]=0x56e4u;return 0;
        case 0x56e4u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x56e8u;return 0;
        case 0x56e8u: /* movne r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=255u;}r[15]=0x56ecu;return 0;
        case 0x56ecu: /* str r1,[r4,#0x40] */
            STORE(r[4]+64u,r[1],4);r[15]=0x56f0u;return 0;
        case 0x56f0u: /* strb r2,[r4,#0x48] */
            STORE(r[4]+72u,r[2],1);r[15]=0x56f4u;return 0;
        case 0x56f4u: /* strb r2,[r4,#0x49] */
            STORE(r[4]+73u,r[2],1);r[15]=0x56f8u;return 0;
        case 0x56f8u: /* strb r0,[r4,#0x4a] */
            STORE(r[4]+74u,r[0],1);r[15]=0x56fcu;return 0;
        case 0x56fcu: /* str r1,[r4,#0x98] */
            STORE(r[4]+152u,r[1],4);r[15]=0x5700u;return 0;
        case 0x5700u: /* strb r2,[r4,#0xa0] */
            STORE(r[4]+160u,r[2],1);r[15]=0x5704u;return 0;
        case 0x5704u: /* strb r2,[r4,#0xa1] */
            STORE(r[4]+161u,r[2],1);r[15]=0x5708u;return 0;
        case 0x5708u: /* strb r0,[r4,#0xa2] */
            STORE(r[4]+162u,r[0],1);r[15]=0x570cu;return 0;
        case 0x570cu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5710u;return 0;
        case 0x5710u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5724u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5728u;return 0;
        case 0x5728u: /* ldr r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,4);r[15]=0x572cu;return 0;
        case 0x572cu: /* mov r2,r1, lsr #0x10 */
            r[2]=(r[1]>>16);r[15]=0x5730u;return 0;
        case 0x5730u: /* ldr r3,[0x57c4] */
            LOAD(r[3],22468u,4);r[15]=0x5734u;return 0;
        case 0x5734u: /* and r1,r1,r3 */
            r[1]=r[1]&r[3];r[15]=0x5738u;return 0;
        case 0x5738u: /* mov r4,#0xb0 */
            r[4]=176u;r[15]=0x573cu;return 0;
        case 0x573cu: /* mul r4,r0,r4 */
            r[4]=r[0]*r[4];r[15]=0x5740u;return 0;
        case 0x5740u: /* ldr r0,[0x5860] */
            LOAD(r[0],22624u,4);r[15]=0x5744u;return 0;
        case 0x5744u: /* add r4,r4,r0 */
            r[4]=r[4]+r[0];r[15]=0x5748u;return 0;
        case 0x5748u: /* ands r2,r2,r3 */
            temporary=r[2]&r[3];r[2]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x574cu;return 0;
        case 0x574cu: /* beq 0x00005790 */
            if(state->cpsr&0x40000000u){r[15]=22416u;}else{r[15]=0x5750u;}return 0;
        case 0x5750u: /* mov r2,r2, lsr #0x3 */
            r[2]=(r[2]>>3);r[15]=0x5754u;return 0;
        case 0x5754u: /* ldr r0,[r4,#0x50] */
            LOAD(r[0],r[4]+80u,4);r[15]=0x5758u;return 0;
        case 0x5758u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x575cu;return 0;
        case 0x575cu: /* ldr r3,[0x57c8] */
            LOAD(r[3],22472u,4);r[15]=0x5760u;return 0;
        case 0x5760u: /* eormi r0,r0,r3 */
            if(state->cpsr&0x80000000u){r[0]=r[0]^r[3];}r[15]=0x5764u;return 0;
        case 0x5764u: /* addmi r0,r0,#0x1 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+1u;}r[15]=0x5768u;return 0;
        case 0x5768u: /* ldr r3,[0x57c4] */
            LOAD(r[3],22468u,4);r[15]=0x576cu;return 0;
        case 0x576cu: /* and r0,r0,r3 */
            r[0]=r[0]&r[3];r[15]=0x5770u;return 0;
        case 0x5770u: /* ldr r3,[0x57cc] */
            LOAD(r[3],22476u,4);r[15]=0x5774u;return 0;
        case 0x5774u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x5778u;return 0;
        case 0x5778u: /* beq 0x00005790 */
            if(state->cpsr&0x40000000u){r[15]=22416u;}else{r[15]=0x577cu;}return 0;
        case 0x577cu: /* tst r2,r3 */
            temporary=r[2]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x5780u;return 0;
        case 0x5780u: /* bne 0x00005790 */
            if(!(state->cpsr&0x40000000u)){r[15]=22416u;}else{r[15]=0x5784u;}return 0;
        case 0x5784u: /* mov r0,r0, lsr #0x1 */
            r[0]=(r[0]>>1);r[15]=0x5788u;return 0;
        case 0x5788u: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x578cu;return 0;
        case 0x578cu: /* b 0x00005774 */
            r[15]=22388u;return 0;
        case 0x5790u: /* ldr r3,[0x57d0] */
            LOAD(r[3],22480u,4);r[15]=0x5794u;return 0;
        case 0x5794u: /* tst r0,r3 */
            temporary=r[0]&r[3];logic_flags(state,temporary,0,r[3]);r[15]=0x5798u;return 0;
        case 0x5798u: /* movne r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=255u;}r[15]=0x579cu;return 0;
        case 0x579cu: /* str r1,[r4,#0x4c] */
            STORE(r[4]+76u,r[1],4);r[15]=0x57a0u;return 0;
        case 0x57a0u: /* strb r2,[r4,#0x54] */
            STORE(r[4]+84u,r[2],1);r[15]=0x57a4u;return 0;
        case 0x57a4u: /* strb r2,[r4,#0x55] */
            STORE(r[4]+85u,r[2],1);r[15]=0x57a8u;return 0;
        case 0x57a8u: /* strb r0,[r4,#0x56] */
            STORE(r[4]+86u,r[0],1);r[15]=0x57acu;return 0;
        case 0x57acu: /* str r1,[r4,#0xa4] */
            STORE(r[4]+164u,r[1],4);r[15]=0x57b0u;return 0;
        case 0x57b0u: /* strb r2,[r4,#0xac] */
            STORE(r[4]+172u,r[2],1);r[15]=0x57b4u;return 0;
        case 0x57b4u: /* strb r2,[r4,#0xad] */
            STORE(r[4]+173u,r[2],1);r[15]=0x57b8u;return 0;
        case 0x57b8u: /* strb r0,[r4,#0xae] */
            STORE(r[4]+174u,r[0],1);r[15]=0x57bcu;return 0;
        case 0x57bcu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x57c0u;return 0;
        case 0x57c0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x57d4u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x57d8u;return 0;
        case 0x57d8u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x57dcu;return 0;
        case 0x57dcu: /* ldrb r2,[r10,#0x4] */
            LOAD(r[2],r[10]+4u,1);r[15]=0x57e0u;return 0;
        case 0x57e0u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x57e4u;return 0;
        case 0x57e4u: /* bl 0x00006ef4 */
            r[14]=0x57e8u;r[15]=28404u;return 0;
        case 0x57e8u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x57ecu;return 0;
        case 0x57ecu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x57f0u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x57f4u;return 0;
        case 0x57f4u: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x57f8u;return 0;
        case 0x57f8u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x57fcu;return 0;
        case 0x57fcu: /* bl 0x00006f88 */
            r[14]=0x5800u;r[15]=28552u;return 0;
        case 0x5800u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5804u;return 0;
        case 0x5804u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5808u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x580cu;return 0;
        case 0x580cu: /* ldrb r1,[r10,#0x3] */
            LOAD(r[1],r[10]+3u,1);r[15]=0x5810u;return 0;
        case 0x5810u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5814u;return 0;
        case 0x5814u: /* bl 0x00007010 */
            r[14]=0x5818u;r[15]=28688u;return 0;
        case 0x5818u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x581cu;return 0;
        case 0x581cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5820u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5824u;return 0;
        case 0x5824u: /* cmp r0,#0xff */
            (void)subtract(state,r[0],255u);r[15]=0x5828u;return 0;
        case 0x5828u: /* beq 0x0000583c */
            if(state->cpsr&0x40000000u){r[15]=22588u;}else{r[15]=0x582cu;}return 0;
        case 0x582cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5830u;return 0;
        case 0x5830u: /* bl 0x00008068 */
            r[14]=0x5834u;r[15]=32872u;return 0;
        case 0x5834u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5838u;return 0;
        case 0x5838u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x583cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5840u;return 0;
        case 0x5840u: /* bl 0x0000805c */
            r[14]=0x5844u;r[15]=32860u;return 0;
        case 0x5844u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5848u;return 0;
        case 0x5848u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x584cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x5850u;return 0;
        case 0x5850u: /* bl 0x00007efc */
            r[14]=0x5854u;r[15]=32508u;return 0;
        case 0x5854u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x5858u;return 0;
        case 0x5858u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5864u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5868u;return 0;
        case 0x5868u: /* ldr r8,[0x58e8] */
            LOAD(r[8],22760u,4);r[15]=0x586cu;return 0;
        case 0x586cu: /* mov r0,r0, lsr #0x4 */
            r[0]=(r[0]>>4);r[15]=0x5870u;return 0;
        case 0x5870u: /* strb r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],1);r[15]=0x5874u;return 0;
        case 0x5874u: /* ldrb r0,[r10,#0x3] */
            LOAD(r[0],r[10]+3u,1);r[15]=0x5878u;return 0;
        case 0x5878u: /* ldr r8,[0x58ec] */
            LOAD(r[8],22764u,4);r[15]=0x587cu;return 0;
        case 0x587cu: /* mov r0,r0, lsr #0x4 */
            r[0]=(r[0]>>4);r[15]=0x5880u;return 0;
        case 0x5880u: /* strb r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],1);r[15]=0x5884u;return 0;
        case 0x5884u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5888u;return 0;
        case 0x5888u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x588cu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5890u;return 0;
        case 0x5890u: /* ldr r8,[0x58f0] */
            LOAD(r[8],22768u,4);r[15]=0x5894u;return 0;
        case 0x5894u: /* mov r0,r0, lsr #0x3 */
            r[0]=(r[0]>>3);r[15]=0x5898u;return 0;
        case 0x5898u: /* subs r0,r0,#0x10 */
            r[0]=subtract(state,r[0],16u);r[15]=0x589cu;return 0;
        case 0x589cu: /* eormi r0,r0,#0xff */
            if(state->cpsr&0x80000000u){r[0]=r[0]^255u;}r[15]=0x58a0u;return 0;
        case 0x58a0u: /* addmi r0,r0,#0x10 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+16u;}r[15]=0x58a4u;return 0;
        case 0x58a4u: /* and r0,r0,#0x1f */
            r[0]=r[0]&31u;r[15]=0x58a8u;return 0;
        case 0x58a8u: /* strb r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],1);r[15]=0x58acu;return 0;
        case 0x58acu: /* ldrb r0,[r10,#0x3] */
            LOAD(r[0],r[10]+3u,1);r[15]=0x58b0u;return 0;
        case 0x58b0u: /* ldr r8,[0x58f4] */
            LOAD(r[8],22772u,4);r[15]=0x58b4u;return 0;
        case 0x58b4u: /* mov r0,r0, lsr #0x3 */
            r[0]=(r[0]>>3);r[15]=0x58b8u;return 0;
        case 0x58b8u: /* subs r0,r0,#0x10 */
            r[0]=subtract(state,r[0],16u);r[15]=0x58bcu;return 0;
        case 0x58bcu: /* eormi r0,r0,#0xff */
            if(state->cpsr&0x80000000u){r[0]=r[0]^255u;}r[15]=0x58c0u;return 0;
        case 0x58c0u: /* addmi r0,r0,#0x10 */
            if(state->cpsr&0x80000000u){r[0]=r[0]+16u;}r[15]=0x58c4u;return 0;
        case 0x58c4u: /* and r0,r0,#0x1f */
            r[0]=r[0]&31u;r[15]=0x58c8u;return 0;
        case 0x58c8u: /* strb r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],1);r[15]=0x58ccu;return 0;
        case 0x58ccu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x58d0u;return 0;
        case 0x58d0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x58d4u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x58d8u;return 0;
        case 0x58d8u: /* ldr r8,[0x58f0] */
            LOAD(r[8],22768u,4);r[15]=0x58dcu;return 0;
        case 0x58dcu: /* str r0,[r8],#0x4 */
            STORE(r[8],r[0],4);r[8]+=4u;r[15]=0x58e0u;return 0;
        case 0x58e0u: /* str r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],4);r[15]=0x58e4u;return 0;
        case 0x58e4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x58f8u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x58fcu;return 0;
        case 0x58fcu: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x5900u;return 0;
        case 0x5900u: /* beq 0x0000595c */
            if(state->cpsr&0x40000000u){r[15]=22876u;}else{r[15]=0x5904u;}return 0;
        case 0x5904u: /* mov r1,#0xe800 */
            r[1]=59392u;r[15]=0x5908u;return 0;
        case 0x5908u: /* mov r2,#0x30 */
            r[2]=48u;r[15]=0x590cu;return 0;
        case 0x590cu: /* mov r3,#0x800000 */
            r[3]=8388608u;r[15]=0x5910u;return 0;
        case 0x5910u: /* mov r4,#0x0 */
            r[4]=0u;r[15]=0x5914u;return 0;
        case 0x5914u: /* str r4,[r1,#0x0] */
            STORE(r[1]+0u,r[4],4);r[15]=0x5918u;return 0;
        case 0x5918u: /* ldr r5,[r3,#0x0] */
            LOAD(r[5],r[3]+0u,4);r[15]=0x591cu;return 0;
        case 0x591cu: /* mov r4,#0x700 */
            r[4]=1792u;r[15]=0x5920u;return 0;
        case 0x5920u: /* orr r4,r4,#0xff */
            r[4]=r[4]|255u;r[15]=0x5924u;return 0;
        case 0x5924u: /* and r5,r5,r4 */
            r[5]=r[5]&r[4];r[15]=0x5928u;return 0;
        case 0x5928u: /* orr r5,r5,#0x8000 */
            r[5]=r[5]|32768u;r[15]=0x592cu;return 0;
        case 0x592cu: /* str r5,[r3,#0x0] */
            STORE(r[3]+0u,r[5],4);r[15]=0x5930u;return 0;
        case 0x5930u: /* add r1,r1,#0x40 */
            r[1]=r[1]+64u;r[15]=0x5934u;return 0;
        case 0x5934u: /* add r3,r3,#0x80 */
            r[3]=r[3]+128u;r[15]=0x5938u;return 0;
        case 0x5938u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x593cu;return 0;
        case 0x593cu: /* bne 0x00005910 */
            if(!(state->cpsr&0x40000000u)){r[15]=22800u;}else{r[15]=0x5940u;}return 0;
        case 0x5940u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5944u;return 0;
        case 0x5944u: /* stmdb sp!,{r0,r10,lr} */
            STORE(r[13]-12u,r[0],4);STORE(r[13]-8u,r[10],4);STORE(r[13]-4u,r[14],4);r[13]-=12u;r[15]=0x5948u;return 0;
        case 0x5948u: /* bl 0x000074f4 */
            r[14]=0x594cu;r[15]=29940u;return 0;
        case 0x594cu: /* ldmia sp!,{r0,r10,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[10],r[13]+4u,4);LOAD(r[14],r[13]+8u,4);r[13]+=12u;r[15]=0x5950u;return 0;
        case 0x5950u: /* add r0,r0,#0x1 */
            r[0]=r[0]+1u;r[15]=0x5954u;return 0;
        case 0x5954u: /* cmp r0,#0x8 */
            (void)subtract(state,r[0],8u);r[15]=0x5958u;return 0;
        case 0x5958u: /* bne 0x00005944 */
            if(!(state->cpsr&0x40000000u)){r[15]=22852u;}else{r[15]=0x595cu;}return 0;
        case 0x595cu: /* ldrb r0,[r10,#0x3] */
            LOAD(r[0],r[10]+3u,1);r[15]=0x5960u;return 0;
        case 0x5960u: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x5964u;return 0;
        case 0x5964u: /* stmdb sp!,{r10,lr} */
            STORE(r[13]-8u,r[10],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x5968u;return 0;
        case 0x5968u: /* blne 0x00007e94 */
            if(!(state->cpsr&0x40000000u)){r[14]=0x596cu;r[15]=32404u;}else{r[15]=0x596cu;}return 0;
        case 0x596cu: /* ldmia sp!,{r10,lr} */
            LOAD(r[10],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x5970u;return 0;
        case 0x5970u: /* ldrb r0,[r10,#0x4] */
            LOAD(r[0],r[10]+4u,1);r[15]=0x5974u;return 0;
        case 0x5974u: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x5978u;return 0;
        case 0x5978u: /* stmdb sp!,{r10,lr} */
            STORE(r[13]-8u,r[10],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x597cu;return 0;
        case 0x597cu: /* blne 0x00007efc */
            if(!(state->cpsr&0x40000000u)){r[14]=0x5980u;r[15]=32508u;}else{r[15]=0x5980u;}return 0;
        case 0x5980u: /* ldmia sp!,{r10,lr} */
            LOAD(r[10],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x5984u;return 0;
        case 0x5984u: /* ldrb r0,[r10,#0x5] */
            LOAD(r[0],r[10]+5u,1);r[15]=0x5988u;return 0;
        case 0x5988u: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x598cu;return 0;
        case 0x598cu: /* beq 0x000059a0 */
            if(state->cpsr&0x40000000u){r[15]=22944u;}else{r[15]=0x5990u;}return 0;
        case 0x5990u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5994u;return 0;
        case 0x5994u: /* ldr r8,[0x58f0] */
            LOAD(r[8],22768u,4);r[15]=0x5998u;return 0;
        case 0x5998u: /* str r0,[r8],#0x4 */
            STORE(r[8],r[0],4);r[8]+=4u;r[15]=0x599cu;return 0;
        case 0x599cu: /* str r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],4);r[15]=0x59a0u;return 0;
        case 0x59a0u: /* ldrb r0,[r10,#0x6] */
            LOAD(r[0],r[10]+6u,1);r[15]=0x59a4u;return 0;
        case 0x59a4u: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x59a8u;return 0;
        case 0x59a8u: /* stmdb sp!,{r10,lr} */
            STORE(r[13]-8u,r[10],4);STORE(r[13]-4u,r[14],4);r[13]-=8u;r[15]=0x59acu;return 0;
        case 0x59acu: /* blne 0x00005d5c */
            if(!(state->cpsr&0x40000000u)){r[14]=0x59b0u;r[15]=23900u;}else{r[15]=0x59b0u;}return 0;
        case 0x59b0u: /* ldmia sp!,{r10,lr} */
            LOAD(r[10],r[13]+0u,4);LOAD(r[14],r[13]+4u,4);r[13]+=8u;r[15]=0x59b4u;return 0;
        case 0x59b4u: /* ldrb r0,[r10,#0x7] */
            LOAD(r[0],r[10]+7u,1);r[15]=0x59b8u;return 0;
        case 0x59b8u: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x59bcu;return 0;
        case 0x59bcu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x59c0u;}return 0;
        case 0x59c0u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x59c4u;return 0;
        case 0x59c4u: /* mov r1,#0x10 */
            r[1]=16u;r[15]=0x59c8u;return 0;
        case 0x59c8u: /* ldr r8,[0x60fc] */
            LOAD(r[8],24828u,4);r[15]=0x59ccu;return 0;
        case 0x59ccu: /* str r0,[r8],#0x4 */
            STORE(r[8],r[0],4);r[8]+=4u;r[15]=0x59d0u;return 0;
        case 0x59d0u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x59d4u;return 0;
        case 0x59d4u: /* bne 0x000059cc */
            if(!(state->cpsr&0x40000000u)){r[15]=22988u;}else{r[15]=0x59d8u;}return 0;
        case 0x59d8u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x59dcu;return 0;
        case 0x59dcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x59e0u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x59e4u;return 0;
        case 0x59e4u: /* mov r0,r0, lsr #0x4 */
            r[0]=(r[0]>>4);r[15]=0x59e8u;return 0;
        case 0x59e8u: /* and r0,r0,#0xf */
            r[0]=r[0]&15u;r[15]=0x59ecu;return 0;
        case 0x59ecu: /* ldr r1,[0x6100] */
            LOAD(r[1],24832u,4);r[15]=0x59f0u;return 0;
        case 0x59f0u: /* strb r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],1);r[15]=0x59f4u;return 0;
        case 0x59f4u: /* ldr r1,[0x6104] */
            LOAD(r[1],24836u,4);r[15]=0x59f8u;return 0;
        case 0x59f8u: /* ldr r1,[r1,#0x0] */
            LOAD(r[1],r[1]+0u,4);r[15]=0x59fcu;return 0;
        case 0x59fcu: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x5a00u;return 0;
        case 0x5a00u: /* beq 0x00005a0c */
            if(state->cpsr&0x40000000u){r[15]=23052u;}else{r[15]=0x5a04u;}return 0;
        case 0x5a04u: /* cmp r0,#0xd */
            (void)subtract(state,r[0],13u);r[15]=0x5a08u;return 0;
        case 0x5a08u: /* movhi r0,#0xd */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[0]=13u;}r[15]=0x5a0cu;return 0;
        case 0x5a0cu: /* ldr r1,[0x6108] */
            LOAD(r[1],24840u,4);r[15]=0x5a10u;return 0;
        case 0x5a10u: /* strb r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],1);r[15]=0x5a14u;return 0;
        case 0x5a14u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5a18u;return 0;
        case 0x5a18u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5a1cu: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5a20u;return 0;
        case 0x5a20u: /* ldr r3,[0x610c] */
            LOAD(r[3],24844u,4);r[15]=0x5a24u;return 0;
        case 0x5a24u: /* ldr r1,[r3,#0x0] */
            LOAD(r[1],r[3]+0u,4);r[15]=0x5a28u;return 0;
        case 0x5a28u: /* ldr r3,[r1,#0x0] */
            LOAD(r[3],r[1]+0u,4);r[15]=0x5a2cu;return 0;
        case 0x5a2cu: /* ldr r4,[0x5b60] */
            LOAD(r[4],23392u,4);r[15]=0x5a30u;return 0;
        case 0x5a30u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x5a34u;return 0;
        case 0x5a34u: /* movne r1,#0x2 */
            if(!(state->cpsr&0x40000000u)){r[1]=2u;}r[15]=0x5a38u;return 0;
        case 0x5a38u: /* bne 0x00005b4c */
            if(!(state->cpsr&0x40000000u)){r[15]=23372u;}else{r[15]=0x5a3cu;}return 0;
        case 0x5a3cu: /* ldrb r3,[r1,#0x4] */
            LOAD(r[3],r[1]+4u,1);r[15]=0x5a40u;return 0;
        case 0x5a40u: /* cmp r3,#0x1 */
            (void)subtract(state,r[3],1u);r[15]=0x5a44u;return 0;
        case 0x5a44u: /* movne r1,#0x8 */
            if(!(state->cpsr&0x40000000u)){r[1]=8u;}r[15]=0x5a48u;return 0;
        case 0x5a48u: /* bne 0x00005b4c */
            if(!(state->cpsr&0x40000000u)){r[15]=23372u;}else{r[15]=0x5a4cu;}return 0;
        case 0x5a4cu: /* ldr r2,[r1,#0x8] */
            LOAD(r[2],r[1]+8u,4);r[15]=0x5a50u;return 0;
        case 0x5a50u: /* sub r2,r2,#0x4 */
            r[2]=r[2]-4u;r[15]=0x5a54u;return 0;
        case 0x5a54u: /* ldr r3,[r1,r2] */
            LOAD(r[3],r[1]+r[2],4);r[15]=0x5a58u;return 0;
        case 0x5a58u: /* ldr r4,[0x5b64] */
            LOAD(r[4],23396u,4);r[15]=0x5a5cu;return 0;
        case 0x5a5cu: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x5a60u;return 0;
        case 0x5a60u: /* movne r1,#0x4 */
            if(!(state->cpsr&0x40000000u)){r[1]=4u;}r[15]=0x5a64u;return 0;
        case 0x5a64u: /* bne 0x00005b4c */
            if(!(state->cpsr&0x40000000u)){r[15]=23372u;}else{r[15]=0x5a68u;}return 0;
        case 0x5a68u: /* ldrb r2,[r1,#0xc] */
            LOAD(r[2],r[1]+12u,1);r[15]=0x5a6cu;return 0;
        case 0x5a6cu: /* cmp r0,r2 */
            (void)subtract(state,r[0],r[2]);r[15]=0x5a70u;return 0;
        case 0x5a70u: /* movcs r1,#0x10 */
            if(state->cpsr&0x20000000u){r[1]=16u;}r[15]=0x5a74u;return 0;
        case 0x5a74u: /* bcs 0x00005b4c */
            if(state->cpsr&0x20000000u){r[15]=23372u;}else{r[15]=0x5a78u;}return 0;
        case 0x5a78u: /* ldr r2,[0x6110] */
            LOAD(r[2],24848u,4);r[15]=0x5a7cu;return 0;
        case 0x5a7cu: /* strb r0,[r2,#0x0] */
            STORE(r[2]+0u,r[0],1);r[15]=0x5a80u;return 0;
        case 0x5a80u: /* ldrb r2,[r1,#0x10] */
            LOAD(r[2],r[1]+16u,1);r[15]=0x5a84u;return 0;
        case 0x5a84u: /* add r2,r1,r2 */
            r[2]=r[1]+r[2];r[15]=0x5a88u;return 0;
        case 0x5a88u: /* ldr r1,[0x6114] */
            LOAD(r[1],24852u,4);r[15]=0x5a8cu;return 0;
        case 0x5a8cu: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x5a90u;return 0;
        case 0x5a90u: /* mov r3,#0x20 */
            r[3]=32u;r[15]=0x5a94u;return 0;
        case 0x5a94u: /* mla r0,r3,r0,r2 */
            r[0]=r[3]*r[0]+r[2];r[15]=0x5a98u;return 0;
        case 0x5a98u: /* ldr r2,[0x60fc] */
            LOAD(r[2],24828u,4);r[15]=0x5a9cu;return 0;
        case 0x5a9cu: /* mov r4,#0x10 */
            r[4]=16u;r[15]=0x5aa0u;return 0;
        case 0x5aa0u: /* ldr r5,[0x6118] */
            LOAD(r[5],24856u,4);r[15]=0x5aa4u;return 0;
        case 0x5aa4u: /* ldrb r3,[r0],#0x1 */
            LOAD(r[3],r[0],1);r[0]+=1u;r[15]=0x5aa8u;return 0;
        case 0x5aa8u: /* and r3,r3,#0xf */
            r[3]=r[3]&15u;r[15]=0x5aacu;return 0;
        case 0x5aacu: /* ldrb r6,[r5],#0x1 */
            LOAD(r[6],r[5],1);r[5]+=1u;r[15]=0x5ab0u;return 0;
        case 0x5ab0u: /* tst r6,#0x80 */
            temporary=r[6]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5ab4u;return 0;
        case 0x5ab4u: /* orrne r6,r6,#0xff00 */
            if(!(state->cpsr&0x40000000u)){r[6]=r[6]|65280u;}r[15]=0x5ab8u;return 0;
        case 0x5ab8u: /* orrne r6,r6,#0xff0000 */
            if(!(state->cpsr&0x40000000u)){r[6]=r[6]|16711680u;}r[15]=0x5abcu;return 0;
        case 0x5abcu: /* orrne r6,r6,#0xff000000 */
            if(!(state->cpsr&0x40000000u)){r[6]=r[6]|4278190080u;}r[15]=0x5ac0u;return 0;
        case 0x5ac0u: /* adds r6,r6,r3, lsl #0x3 */
            r[6]=addition(state,r[6],(r[3]<<3));r[15]=0x5ac4u;return 0;
        case 0x5ac4u: /* movmi r6,#0x0 */
            if(state->cpsr&0x80000000u){r[6]=0u;}r[15]=0x5ac8u;return 0;
        case 0x5ac8u: /* tst r6,#0x80 */
            temporary=r[6]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5accu;return 0;
        case 0x5accu: /* movne r6,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[6]=127u;}r[15]=0x5ad0u;return 0;
        case 0x5ad0u: /* mov r6,r6, lsl #0x5 */
            r[6]=(r[6]<<5);r[15]=0x5ad4u;return 0;
        case 0x5ad4u: /* and r6,r6,#0xf00 */
            r[6]=r[6]&3840u;r[15]=0x5ad8u;return 0;
        case 0x5ad8u: /* ldrb r3,[r0],#0x1 */
            LOAD(r[3],r[0],1);r[0]+=1u;r[15]=0x5adcu;return 0;
        case 0x5adcu: /* cmp r3,#0x10 */
            (void)subtract(state,r[3],16u);r[15]=0x5ae0u;return 0;
        case 0x5ae0u: /* moveq r3,#0x0 */
            if(state->cpsr&0x40000000u){r[3]=0u;}r[15]=0x5ae4u;return 0;
        case 0x5ae4u: /* tst r3,#0x10 */
            temporary=r[3]&16u;logic_flags(state,temporary,0,16u);r[15]=0x5ae8u;return 0;
        case 0x5ae8u: /* subne r3,r3,#0x10 */
            if(!(state->cpsr&0x40000000u)){r[3]=r[3]-16u;}r[15]=0x5aecu;return 0;
        case 0x5aecu: /* eorne r3,r3,#0xff */
            if(!(state->cpsr&0x40000000u)){r[3]=r[3]^255u;}r[15]=0x5af0u;return 0;
        case 0x5af0u: /* add r3,r3,#0x10 */
            r[3]=r[3]+16u;r[15]=0x5af4u;return 0;
        case 0x5af4u: /* and r3,r3,#0x1f */
            r[3]=r[3]&31u;r[15]=0x5af8u;return 0;
        case 0x5af8u: /* ldrb r7,[r5],#0x1 */
            LOAD(r[7],r[5],1);r[5]+=1u;r[15]=0x5afcu;return 0;
        case 0x5afcu: /* tst r7,#0x80 */
            temporary=r[7]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5b00u;return 0;
        case 0x5b00u: /* orrne r7,r7,#0xff00 */
            if(!(state->cpsr&0x40000000u)){r[7]=r[7]|65280u;}r[15]=0x5b04u;return 0;
        case 0x5b04u: /* orrne r7,r7,#0xff0000 */
            if(!(state->cpsr&0x40000000u)){r[7]=r[7]|16711680u;}r[15]=0x5b08u;return 0;
        case 0x5b08u: /* orrne r7,r7,#0xff000000 */
            if(!(state->cpsr&0x40000000u)){r[7]=r[7]|4278190080u;}r[15]=0x5b0cu;return 0;
        case 0x5b0cu: /* adds r7,r7,r3, lsl #0x2 */
            r[7]=addition(state,r[7],(r[3]<<2));r[15]=0x5b10u;return 0;
        case 0x5b10u: /* movmi r7,#0x0 */
            if(state->cpsr&0x80000000u){r[7]=0u;}r[15]=0x5b14u;return 0;
        case 0x5b14u: /* tst r7,#0x80 */
            temporary=r[7]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5b18u;return 0;
        case 0x5b18u: /* movne r7,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[7]=127u;}r[15]=0x5b1cu;return 0;
        case 0x5b1cu: /* mov r7,r7, lsr #0x2 */
            r[7]=(r[7]>>2);r[15]=0x5b20u;return 0;
        case 0x5b20u: /* and r7,r7,#0x1f */
            r[7]=r[7]&31u;r[15]=0x5b24u;return 0;
        case 0x5b24u: /* subs r7,r7,#0x10 */
            r[7]=subtract(state,r[7],16u);r[15]=0x5b28u;return 0;
        case 0x5b28u: /* eormi r7,r7,#0xff */
            if(state->cpsr&0x80000000u){r[7]=r[7]^255u;}r[15]=0x5b2cu;return 0;
        case 0x5b2cu: /* addmi r7,r7,#0x10 */
            if(state->cpsr&0x80000000u){r[7]=r[7]+16u;}r[15]=0x5b30u;return 0;
        case 0x5b30u: /* and r7,r7,#0x1f */
            r[7]=r[7]&31u;r[15]=0x5b34u;return 0;
        case 0x5b34u: /* orr r3,r7,r6 */
            r[3]=r[7]|r[6];r[15]=0x5b38u;return 0;
        case 0x5b38u: /* str r3,[r2],#0x4 */
            STORE(r[2],r[3],4);r[2]+=4u;r[15]=0x5b3cu;return 0;
        case 0x5b3cu: /* subs r4,r4,#0x1 */
            r[4]=subtract(state,r[4],1u);r[15]=0x5b40u;return 0;
        case 0x5b40u: /* bne 0x00005aa4 */
            if(!(state->cpsr&0x40000000u)){r[15]=23204u;}else{r[15]=0x5b44u;}return 0;
        case 0x5b44u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5b48u;return 0;
        case 0x5b48u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5b4cu: /* ldr r2,[0x611c] */
            LOAD(r[2],24860u,4);r[15]=0x5b50u;return 0;
        case 0x5b50u: /* ldrb r0,[r2,#0x0] */
            LOAD(r[0],r[2]+0u,1);r[15]=0x5b54u;return 0;
        case 0x5b54u: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0x5b58u;return 0;
        case 0x5b58u: /* strb r0,[r2,#0x0] */
            STORE(r[2]+0u,r[0],1);r[15]=0x5b5cu;return 0;
        case 0x5b5cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5b68u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5b6cu;return 0;
        case 0x5b6cu: /* and r0,r0,#0xf */
            r[0]=r[0]&15u;r[15]=0x5b70u;return 0;
        case 0x5b70u: /* ldr r2,[0x60fc] */
            LOAD(r[2],24828u,4);r[15]=0x5b74u;return 0;
        case 0x5b74u: /* add r2,r2,r0, lsl #0x2 */
            r[2]=r[2]+(r[0]<<2);r[15]=0x5b78u;return 0;
        case 0x5b78u: /* ldr r3,[0x6118] */
            LOAD(r[3],24856u,4);r[15]=0x5b7cu;return 0;
        case 0x5b7cu: /* add r3,r3,r0, lsl #0x1 */
            r[3]=r[3]+(r[0]<<1);r[15]=0x5b80u;return 0;
        case 0x5b80u: /* ldr r4,[0x6114] */
            LOAD(r[4],24852u,4);r[15]=0x5b84u;return 0;
        case 0x5b84u: /* ldr r4,[r4,#0x0] */
            LOAD(r[4],r[4]+0u,4);r[15]=0x5b88u;return 0;
        case 0x5b88u: /* movs r4,r4 */
            r[4]=r[4];logic_flags(state,r[4],0,0);r[15]=0x5b8cu;return 0;
        case 0x5b8cu: /* beq 0x00005c20 */
            if(state->cpsr&0x40000000u){r[15]=23584u;}else{r[15]=0x5b90u;}return 0;
        case 0x5b90u: /* add r4,r4,r0, lsl #0x1 */
            r[4]=r[4]+(r[0]<<1);r[15]=0x5b94u;return 0;
        case 0x5b94u: /* ldrb r0,[r10,#0x3] */
            LOAD(r[0],r[10]+3u,1);r[15]=0x5b98u;return 0;
        case 0x5b98u: /* sub r0,r0,#0x80 */
            r[0]=r[0]-128u;r[15]=0x5b9cu;return 0;
        case 0x5b9cu: /* strb r0,[r3],#0x1 */
            STORE(r[3],r[0],1);r[3]+=1u;r[15]=0x5ba0u;return 0;
        case 0x5ba0u: /* ldrb r5,[r4],#0x1 */
            LOAD(r[5],r[4],1);r[4]+=1u;r[15]=0x5ba4u;return 0;
        case 0x5ba4u: /* and r5,r5,#0xf */
            r[5]=r[5]&15u;r[15]=0x5ba8u;return 0;
        case 0x5ba8u: /* adds r0,r0,r5, lsl #0x3 */
            r[0]=addition(state,r[0],(r[5]<<3));r[15]=0x5bacu;return 0;
        case 0x5bacu: /* movmi r0,#0x0 */
            if(state->cpsr&0x80000000u){r[0]=0u;}r[15]=0x5bb0u;return 0;
        case 0x5bb0u: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5bb4u;return 0;
        case 0x5bb4u: /* movne r0,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[0]=127u;}r[15]=0x5bb8u;return 0;
        case 0x5bb8u: /* mov r0,r0, lsl #0x5 */
            r[0]=(r[0]<<5);r[15]=0x5bbcu;return 0;
        case 0x5bbcu: /* and r0,r0,#0xf00 */
            r[0]=r[0]&3840u;r[15]=0x5bc0u;return 0;
        case 0x5bc0u: /* ldrb r1,[r10,#0x4] */
            LOAD(r[1],r[10]+4u,1);r[15]=0x5bc4u;return 0;
        case 0x5bc4u: /* sub r1,r1,#0x80 */
            r[1]=r[1]-128u;r[15]=0x5bc8u;return 0;
        case 0x5bc8u: /* strb r1,[r3,#0x0] */
            STORE(r[3]+0u,r[1],1);r[15]=0x5bccu;return 0;
        case 0x5bccu: /* ldrb r5,[r4,#0x0] */
            LOAD(r[5],r[4]+0u,1);r[15]=0x5bd0u;return 0;
        case 0x5bd0u: /* cmp r5,#0x10 */
            (void)subtract(state,r[5],16u);r[15]=0x5bd4u;return 0;
        case 0x5bd4u: /* moveq r5,#0x0 */
            if(state->cpsr&0x40000000u){r[5]=0u;}r[15]=0x5bd8u;return 0;
        case 0x5bd8u: /* tst r5,#0x10 */
            temporary=r[5]&16u;logic_flags(state,temporary,0,16u);r[15]=0x5bdcu;return 0;
        case 0x5bdcu: /* subne r5,r5,#0x10 */
            if(!(state->cpsr&0x40000000u)){r[5]=r[5]-16u;}r[15]=0x5be0u;return 0;
        case 0x5be0u: /* eorne r5,r5,#0xff */
            if(!(state->cpsr&0x40000000u)){r[5]=r[5]^255u;}r[15]=0x5be4u;return 0;
        case 0x5be4u: /* add r5,r5,#0x10 */
            r[5]=r[5]+16u;r[15]=0x5be8u;return 0;
        case 0x5be8u: /* and r5,r5,#0x1f */
            r[5]=r[5]&31u;r[15]=0x5becu;return 0;
        case 0x5becu: /* adds r1,r1,r5, lsl #0x2 */
            r[1]=addition(state,r[1],(r[5]<<2));r[15]=0x5bf0u;return 0;
        case 0x5bf0u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x5bf4u;return 0;
        case 0x5bf4u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5bf8u;return 0;
        case 0x5bf8u: /* movne r1,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[1]=127u;}r[15]=0x5bfcu;return 0;
        case 0x5bfcu: /* mov r1,r1, lsr #0x2 */
            r[1]=(r[1]>>2);r[15]=0x5c00u;return 0;
        case 0x5c00u: /* subs r1,r1,#0x10 */
            r[1]=subtract(state,r[1],16u);r[15]=0x5c04u;return 0;
        case 0x5c04u: /* eormi r1,r1,#0xff */
            if(state->cpsr&0x80000000u){r[1]=r[1]^255u;}r[15]=0x5c08u;return 0;
        case 0x5c08u: /* addmi r1,r1,#0x10 */
            if(state->cpsr&0x80000000u){r[1]=r[1]+16u;}r[15]=0x5c0cu;return 0;
        case 0x5c0cu: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0x5c10u;return 0;
        case 0x5c10u: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0x5c14u;return 0;
        case 0x5c14u: /* str r0,[r2,#0x0] */
            STORE(r[2]+0u,r[0],4);r[15]=0x5c18u;return 0;
        case 0x5c18u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5c1cu;return 0;
        case 0x5c1cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5c20u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5c24u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5c28u;return 0;
        case 0x5e00u: /* ldr r0,[r10,#0x4] */
            LOAD(r[0],r[10]+4u,4);r[15]=0x5e04u;return 0;
        case 0x5e04u: /* mov r1,#0xb000 */
            r[1]=45056u;r[15]=0x5e08u;return 0;
        case 0x5e08u: /* mov r3,#0x13000 */
            r[3]=77824u;r[15]=0x5e0cu;return 0;
        case 0x5e0cu: /* ldr r2,[r3,#0x0] */
            LOAD(r[2],r[3]+0u,4);r[15]=0x5e10u;return 0;
        case 0x5e10u: /* str r0,[r1,r2] */
            STORE(r[1]+r[2],r[0],4);r[15]=0x5e14u;return 0;
        case 0x5e14u: /* add r2,r2,#0x4 */
            r[2]=r[2]+4u;r[15]=0x5e18u;return 0;
        case 0x5e18u: /* ldr r1,[0x5e2c] */
            LOAD(r[1],24108u,4);r[15]=0x5e1cu;return 0;
        case 0x5e1cu: /* and r2,r2,r1 */
            r[2]=r[2]&r[1];r[15]=0x5e20u;return 0;
        case 0x5e20u: /* str r2,[r3,#0x0] */
            STORE(r[3]+0u,r[2],4);r[15]=0x5e24u;return 0;
        case 0x5e24u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5e28u;return 0;
        case 0x5e28u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5e30u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5e34u;return 0;
        case 0x5e34u: /* and r0,r0,#0x1 */
            r[0]=r[0]&1u;r[15]=0x5e38u;return 0;
        case 0x5e38u: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x5e3cu;return 0;
        case 0x5e3cu: /* adr r1,0x5e48 */
            r[1]=24136u;r[15]=0x5e40u;return 0;
        case 0x5e40u: /* add r0,r0,r1 */
            r[0]=r[0]+r[1];r[15]=0x5e44u;return 0;
        case 0x5e44u: /* mov pc,r0 */
            r[15]=r[0];return 0;
        case 0x5e48u: /* b 0x00005e50 */
            r[15]=24144u;return 0;
        case 0x5e4cu: /* b 0x00005e70 */
            r[15]=24176u;return 0;
        case 0x5e50u: /* ldrb r0,[r10,#0x3] */
            LOAD(r[0],r[10]+3u,1);r[15]=0x5e54u;return 0;
        case 0x5e54u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x5e58u;return 0;
        case 0x5e58u: /* mov r1,#0x480 */
            r[1]=1152u;r[15]=0x5e5cu;return 0;
        case 0x5e5cu: /* ldr r2,[0x614c] */
            LOAD(r[2],24908u,4);r[15]=0x5e60u;return 0;
        case 0x5e60u: /* mla r0,r1,r0,r2 */
            r[0]=r[1]*r[0]+r[2];r[15]=0x5e64u;return 0;
        case 0x5e64u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x5e68u;return 0;
        case 0x5e68u: /* strb r1,[r0,#0x3c] */
            STORE(r[0]+60u,r[1],1);r[15]=0x5e6cu;return 0;
        case 0x5e6cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5e70u: /* ldrb r0,[r10,#0x3] */
            LOAD(r[0],r[10]+3u,1);r[15]=0x5e74u;return 0;
        case 0x5e74u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x5e78u;return 0;
        case 0x5e78u: /* mov r1,#0x480 */
            r[1]=1152u;r[15]=0x5e7cu;return 0;
        case 0x5e7cu: /* ldr r2,[0x614c] */
            LOAD(r[2],24908u,4);r[15]=0x5e80u;return 0;
        case 0x5e80u: /* mla r0,r1,r0,r2 */
            r[0]=r[1]*r[0]+r[2];r[15]=0x5e84u;return 0;
        case 0x5e84u: /* mov r1,#0x1 */
            r[1]=1u;r[15]=0x5e88u;return 0;
        case 0x5e88u: /* strb r1,[r0,#0x3c] */
            STORE(r[0]+60u,r[1],1);r[15]=0x5e8cu;return 0;
        case 0x5e8cu: /* ldr r1,[0x6150] */
            LOAD(r[1],24912u,4);r[15]=0x5e90u;return 0;
        case 0x5e90u: /* ldr r2,[r1,#0x0] */
            LOAD(r[2],r[1]+0u,4);r[15]=0x5e94u;return 0;
        case 0x5e94u: /* movs r2,r2 */
            r[2]=r[2];logic_flags(state,r[2],0,0);r[15]=0x5e98u;return 0;
        case 0x5e98u: /* beq 0x00005ed8 */
            if(state->cpsr&0x40000000u){r[15]=24280u;}else{r[15]=0x5e9cu;}return 0;
        case 0x5e9cu: /* ldr r3,[r1,#0x4] */
            LOAD(r[3],r[1]+4u,4);r[15]=0x5ea0u;return 0;
        case 0x5ea0u: /* movs r3,r3 */
            r[3]=r[3];logic_flags(state,r[3],0,0);r[15]=0x5ea4u;return 0;
        case 0x5ea4u: /* beq 0x00005ed8 */
            if(state->cpsr&0x40000000u){r[15]=24280u;}else{r[15]=0x5ea8u;}return 0;
        case 0x5ea8u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x5eacu;return 0;
        case 0x5eacu: /* strb r1,[r0,#0x1] */
            STORE(r[0]+1u,r[1],1);r[15]=0x5eb0u;return 0;
        case 0x5eb0u: /* str r2,[r0,#0x10] */
            STORE(r[0]+16u,r[2],4);r[15]=0x5eb4u;return 0;
        case 0x5eb4u: /* ldr r1,[r2,#0x10] */
            LOAD(r[1],r[2]+16u,4);r[15]=0x5eb8u;return 0;
        case 0x5eb8u: /* add r1,r1,r2 */
            r[1]=r[1]+r[2];r[15]=0x5ebcu;return 0;
        case 0x5ebcu: /* ldr r1,[r1,#0x0] */
            LOAD(r[1],r[1]+0u,4);r[15]=0x5ec0u;return 0;
        case 0x5ec0u: /* add r1,r1,r2 */
            r[1]=r[1]+r[2];r[15]=0x5ec4u;return 0;
        case 0x5ec4u: /* str r1,[r0,#0x14] */
            STORE(r[0]+20u,r[1],4);r[15]=0x5ec8u;return 0;
        case 0x5ec8u: /* ldr r1,[r2,#0x18] */
            LOAD(r[1],r[2]+24u,4);r[15]=0x5eccu;return 0;
        case 0x5eccu: /* add r1,r1,r2 */
            r[1]=r[1]+r[2];r[15]=0x5ed0u;return 0;
        case 0x5ed0u: /* str r1,[r0,#0x18] */
            STORE(r[0]+24u,r[1],4);r[15]=0x5ed4u;return 0;
        case 0x5ed4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5ed8u: /* ldr r1,[0x6154] */
            LOAD(r[1],24916u,4);r[15]=0x5edcu;return 0;
        case 0x5edcu: /* str r1,[r0,#0x10] */
            STORE(r[0]+16u,r[1],4);r[15]=0x5ee0u;return 0;
        case 0x5ee0u: /* b 0x00005eb4 */
            r[15]=24244u;return 0;
        case 0x5ee4u: /* ldr r0,[0x6158] */
            LOAD(r[0],24920u,4);r[15]=0x5ee8u;return 0;
        case 0x5ee8u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x5eecu;return 0;
        case 0x5eecu: /* tst r1,#0x30 */
            temporary=r[1]&48u;logic_flags(state,temporary,0,48u);r[15]=0x5ef0u;return 0;
        case 0x5ef0u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x5ef4u;}return 0;
        case 0x5ef4u: /* tst r1,#0x20 */
            temporary=r[1]&32u;logic_flags(state,temporary,0,32u);r[15]=0x5ef8u;return 0;
        case 0x5ef8u: /* bne 0x00005f64 */
            if(!(state->cpsr&0x40000000u)){r[15]=24420u;}else{r[15]=0x5efcu;}return 0;
        case 0x5efcu: /* mov r3,#0x8 */
            r[3]=8u;r[15]=0x5f00u;return 0;
        case 0x5f00u: /* add r9,r10,#0x2 */
            r[9]=r[10]+2u;r[15]=0x5f04u;return 0;
        case 0x5f04u: /* ldr r2,[0x615c] */
            LOAD(r[2],24924u,4);r[15]=0x5f08u;return 0;
        case 0x5f08u: /* ldrb r2,[r2,#0x0] */
            LOAD(r[2],r[2]+0u,1);r[15]=0x5f0cu;return 0;
        case 0x5f0cu: /* ldr r1,[0x613c] */
            LOAD(r[1],24892u,4);r[15]=0x5f10u;return 0;
        case 0x5f10u: /* add r1,r1,r2, lsl #0x2 */
            r[1]=r[1]+(r[2]<<2);r[15]=0x5f14u;return 0;
        case 0x5f14u: /* ldr r2,[0x5fcc] */
            LOAD(r[2],24524u,4);r[15]=0x5f18u;return 0;
        case 0x5f18u: /* ldrb r0,[r9],#0x1 */
            LOAD(r[0],r[9],1);r[9]+=1u;r[15]=0x5f1cu;return 0;
        case 0x5f1cu: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5f20u;return 0;
        case 0x5f20u: /* beq 0x00005f54 */
            if(state->cpsr&0x40000000u){r[15]=24404u;}else{r[15]=0x5f24u;}return 0;
        case 0x5f24u: /* and r0,r0,#0x7c */
            r[0]=r[0]&124u;r[15]=0x5f28u;return 0;
        case 0x5f28u: /* add r0,r2,r0, lsl #0x2 */
            r[0]=r[2]+(r[0]<<2);r[15]=0x5f2cu;return 0;
        case 0x5f2cu: /* mov r5,r1 */
            r[5]=r[1];r[15]=0x5f30u;return 0;
        case 0x5f30u: /* ldr r2,[r0,#0x0] */
            LOAD(r[2],r[0]+0u,4);r[15]=0x5f34u;return 0;
        case 0x5f34u: /* str r2,[r5,#0x0] */
            STORE(r[5]+0u,r[2],4);r[15]=0x5f38u;return 0;
        case 0x5f38u: /* ldr r2,[r0,#0x4] */
            LOAD(r[2],r[0]+4u,4);r[15]=0x5f3cu;return 0;
        case 0x5f3cu: /* str r2,[r5,#0x20] */
            STORE(r[5]+32u,r[2],4);r[15]=0x5f40u;return 0;
        case 0x5f40u: /* ldr r2,[r0,#0x8] */
            LOAD(r[2],r[0]+8u,4);r[15]=0x5f44u;return 0;
        case 0x5f44u: /* str r2,[r5,#0x40] */
            STORE(r[5]+64u,r[2],4);r[15]=0x5f48u;return 0;
        case 0x5f48u: /* ldr r2,[r0,#0xc] */
            LOAD(r[2],r[0]+12u,4);r[15]=0x5f4cu;return 0;
        case 0x5f4cu: /* str r2,[r5,#0x60] */
            STORE(r[5]+96u,r[2],4);r[15]=0x5f50u;return 0;
        case 0x5f50u: /* mov r1,r5 */
            r[1]=r[5];r[15]=0x5f54u;return 0;
        case 0x5f54u: /* add r1,r1,#0x4 */
            r[1]=r[1]+4u;r[15]=0x5f58u;return 0;
        case 0x5f58u: /* subs r3,r3,#0x1 */
            r[3]=subtract(state,r[3],1u);r[15]=0x5f5cu;return 0;
        case 0x5f5cu: /* bne 0x00005f14 */
            if(!(state->cpsr&0x40000000u)){r[15]=24340u;}else{r[15]=0x5f60u;}return 0;
        case 0x5f60u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5f64u: /* mov r3,#0x4 */
            r[3]=4u;r[15]=0x5f68u;return 0;
        case 0x5f68u: /* add r9,r10,#0x2 */
            r[9]=r[10]+2u;r[15]=0x5f6cu;return 0;
        case 0x5f6cu: /* ldr r2,[0x615c] */
            LOAD(r[2],24924u,4);r[15]=0x5f70u;return 0;
        case 0x5f70u: /* ldrb r2,[r2,#0x0] */
            LOAD(r[2],r[2]+0u,1);r[15]=0x5f74u;return 0;
        case 0x5f74u: /* ldr r1,[0x613c] */
            LOAD(r[1],24892u,4);r[15]=0x5f78u;return 0;
        case 0x5f78u: /* add r1,r1,r2, lsl #0x2 */
            r[1]=r[1]+(r[2]<<2);r[15]=0x5f7cu;return 0;
        case 0x5f7cu: /* ldr r2,[0x5fcc] */
            LOAD(r[2],24524u,4);r[15]=0x5f80u;return 0;
        case 0x5f80u: /* ldrb r0,[r9],#0x1 */
            LOAD(r[0],r[9],1);r[9]+=1u;r[15]=0x5f84u;return 0;
        case 0x5f84u: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x5f88u;return 0;
        case 0x5f88u: /* beq 0x00005fbc */
            if(state->cpsr&0x40000000u){r[15]=24508u;}else{r[15]=0x5f8cu;}return 0;
        case 0x5f8cu: /* and r0,r0,#0x7c */
            r[0]=r[0]&124u;r[15]=0x5f90u;return 0;
        case 0x5f90u: /* add r0,r2,r0, lsl #0x2 */
            r[0]=r[2]+(r[0]<<2);r[15]=0x5f94u;return 0;
        case 0x5f94u: /* mov r5,r1 */
            r[5]=r[1];r[15]=0x5f98u;return 0;
        case 0x5f98u: /* ldr r2,[r0,#0x0] */
            LOAD(r[2],r[0]+0u,4);r[15]=0x5f9cu;return 0;
        case 0x5f9cu: /* str r2,[r5,#0x0] */
            STORE(r[5]+0u,r[2],4);r[15]=0x5fa0u;return 0;
        case 0x5fa0u: /* ldr r2,[r0,#0x4] */
            LOAD(r[2],r[0]+4u,4);r[15]=0x5fa4u;return 0;
        case 0x5fa4u: /* str r2,[r5,#0x10] */
            STORE(r[5]+16u,r[2],4);r[15]=0x5fa8u;return 0;
        case 0x5fa8u: /* ldr r2,[r0,#0x8] */
            LOAD(r[2],r[0]+8u,4);r[15]=0x5facu;return 0;
        case 0x5facu: /* str r2,[r5,#0x20] */
            STORE(r[5]+32u,r[2],4);r[15]=0x5fb0u;return 0;
        case 0x5fb0u: /* ldr r2,[r0,#0xc] */
            LOAD(r[2],r[0]+12u,4);r[15]=0x5fb4u;return 0;
        case 0x5fb4u: /* str r2,[r5,#0x30] */
            STORE(r[5]+48u,r[2],4);r[15]=0x5fb8u;return 0;
        case 0x5fb8u: /* mov r1,r5 */
            r[1]=r[5];r[15]=0x5fbcu;return 0;
        case 0x5fbcu: /* add r1,r1,#0x4 */
            r[1]=r[1]+4u;r[15]=0x5fc0u;return 0;
        case 0x5fc0u: /* subs r3,r3,#0x1 */
            r[3]=subtract(state,r[3],1u);r[15]=0x5fc4u;return 0;
        case 0x5fc4u: /* bne 0x00005f7c */
            if(!(state->cpsr&0x40000000u)){r[15]=24444u;}else{r[15]=0x5fc8u;}return 0;
        case 0x5fc8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5fd0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5fd4u: /* ldr r1,[0x6100] */
            LOAD(r[1],24832u,4);r[15]=0x5fd8u;return 0;
        case 0x5fd8u: /* ldrb r2,[r1,#0x0] */
            LOAD(r[2],r[1]+0u,1);r[15]=0x5fdcu;return 0;
        case 0x5fdcu: /* ldr r1,[0x6104] */
            LOAD(r[1],24836u,4);r[15]=0x5fe0u;return 0;
        case 0x5fe0u: /* ldrb r0,[r10,#0x2] */
            LOAD(r[0],r[10]+2u,1);r[15]=0x5fe4u;return 0;
        case 0x5fe4u: /* tst r0,#0xff */
            temporary=r[0]&255u;logic_flags(state,temporary,0,255u);r[15]=0x5fe8u;return 0;
        case 0x5fe8u: /* bne 0x00006008 */
            if(!(state->cpsr&0x40000000u)){r[15]=24584u;}else{r[15]=0x5fecu;}return 0;
        case 0x5fecu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5ff0u;return 0;
        case 0x5ff0u: /* str r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],4);r[15]=0x5ff4u;return 0;
        case 0x5ff4u: /* orr r0,r0,r2 */
            r[0]=r[0]|r[2];r[15]=0x5ff8u;return 0;
        case 0x5ff8u: /* ldr r8,[0x6108] */
            LOAD(r[8],24840u,4);r[15]=0x5ffcu;return 0;
        case 0x5ffcu: /* str r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],4);r[15]=0x6000u;return 0;
        case 0x6000u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x6004u;return 0;
        case 0x6004u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6008u: /* mvn r0,#0x0 */
            r[0]=~0u;r[15]=0x600cu;return 0;
        case 0x600cu: /* str r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],4);r[15]=0x6010u;return 0;
        case 0x6010u: /* cmp r2,#0xd */
            (void)subtract(state,r[2],13u);r[15]=0x6014u;return 0;
        case 0x6014u: /* movhi r2,#0xd */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[2]=13u;}r[15]=0x6018u;return 0;
        case 0x6018u: /* mov r0,#0x8000 */
            r[0]=32768u;r[15]=0x601cu;return 0;
        case 0x601cu: /* b 0x00005ff4 */
            r[15]=24564u;return 0;
        case 0x6024u: /* mrs r10,cpsr */
            r[10]=state->cpsr;r[15]=0x6028u;return 0;
        case 0x6028u: /* orr r10,r10,#0x40 */
            r[10]=r[10]|64u;r[15]=0x602cu;return 0;
        case 0x602cu: /* msr cpsr_cf,r10 */
            state->cpsr=(state->cpsr&0x00ffff00u)|(r[10]&0xff0000ffu);r[15]=0x6030u;return 0;
        case 0x6030u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6034u;return 0;
        case 0x6034u: /* bl 0x00000650 */
            r[14]=0x6038u;r[15]=1616u;return 0;
        case 0x6038u: /* bl 0x0000043c */
            r[14]=0x603cu;r[15]=1084u;return 0;
        case 0x603cu: /* bl 0x00000464 */
            r[14]=0x6040u;r[15]=1124u;return 0;
        case 0x6040u: /* bl 0x000004e8 */
            r[14]=0x6044u;r[15]=1256u;return 0;
        case 0x6044u: /* bl 0x00000504 */
            r[14]=0x6048u;r[15]=1284u;return 0;
        case 0x6048u: /* bl 0x00000610 */
            r[14]=0x604cu;r[15]=1552u;return 0;
        case 0x604cu: /* bl 0x000073bc */
            r[14]=0x6050u;r[15]=29628u;return 0;
        case 0x6050u: /* bl 0x00005d5c */
            r[14]=0x6054u;r[15]=23900u;return 0;
        case 0x6054u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x6058u;return 0;
        case 0x6058u: /* mrs r10,cpsr */
            r[10]=state->cpsr;r[15]=0x605cu;return 0;
        case 0x605cu: /* bic r10,r10,#0x40 */
            r[10]=r[10]&~64u;r[15]=0x6060u;return 0;
        case 0x6060u: /* msr cpsr_cf,r10 */
            state->cpsr=(state->cpsr&0x00ffff00u)|(r[10]&0xff0000ffu);r[15]=0x6064u;return 0;
        case 0x6064u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6068u: /* ldrb r0,[r10,#0x4] */
            LOAD(r[0],r[10]+4u,1);r[15]=0x606cu;return 0;
        case 0x606cu: /* cmp r0,#0x53 */
            (void)subtract(state,r[0],83u);r[15]=0x6070u;return 0;
        case 0x6070u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x6074u;}return 0;
        case 0x6074u: /* ldrb r0,[r10,#0x5] */
            LOAD(r[0],r[10]+5u,1);r[15]=0x6078u;return 0;
        case 0x6078u: /* cmp r0,#0x45 */
            (void)subtract(state,r[0],69u);r[15]=0x607cu;return 0;
        case 0x607cu: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x6080u;}return 0;
        case 0x6080u: /* ldrb r0,[r10,#0x6] */
            LOAD(r[0],r[10]+6u,1);r[15]=0x6084u;return 0;
        case 0x6084u: /* cmp r0,#0x47 */
            (void)subtract(state,r[0],71u);r[15]=0x6088u;return 0;
        case 0x6088u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x608cu;}return 0;
        case 0x608cu: /* ldrb r0,[r10,#0x7] */
            LOAD(r[0],r[10]+7u,1);r[15]=0x6090u;return 0;
        case 0x6090u: /* cmp r0,#0x41 */
            (void)subtract(state,r[0],65u);r[15]=0x6094u;return 0;
        case 0x6094u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x6098u;}return 0;
        case 0x6098u: /* mrs r10,cpsr */
            r[10]=state->cpsr;r[15]=0x609cu;return 0;
        case 0x609cu: /* orr r10,r10,#0x40 */
            r[10]=r[10]|64u;r[15]=0x60a0u;return 0;
        case 0x60a0u: /* msr cpsr_cf,r10 */
            state->cpsr=(state->cpsr&0x00ffff00u)|(r[10]&0xff0000ffu);r[15]=0x60a4u;return 0;
        case 0x60a4u: /* ldr r0,[0x6160] */
            LOAD(r[0],24928u,4);r[15]=0x60a8u;return 0;
        case 0x60a8u: /* mov r1,#0xff000000 */
            r[1]=4278190080u;r[15]=0x60acu;return 0;
        case 0x60acu: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x60b0u;return 0;
        case 0x60b0u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x60b4u;return 0;
        case 0x60b4u: /* bl 0x00000650 */
            r[14]=0x60b8u;r[15]=1616u;return 0;
        case 0x60b8u: /* bl 0x0000043c */
            r[14]=0x60bcu;r[15]=1084u;return 0;
        case 0x60bcu: /* bl 0x00000464 */
            r[14]=0x60c0u;r[15]=1124u;return 0;
        case 0x60c0u: /* bl 0x000004e8 */
            r[14]=0x60c4u;r[15]=1256u;return 0;
        case 0x60c4u: /* bl 0x00000504 */
            r[14]=0x60c8u;r[15]=1284u;return 0;
        case 0x60c8u: /* bl 0x00000610 */
            r[14]=0x60ccu;r[15]=1552u;return 0;
        case 0x60ccu: /* bl 0x000073bc */
            r[14]=0x60d0u;r[15]=29628u;return 0;
        case 0x60d0u: /* bl 0x00005d5c */
            r[14]=0x60d4u;r[15]=23900u;return 0;
        case 0x60d4u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x60d8u;return 0;
        case 0x60d8u: /* ldr r0,[0x6160] */
            LOAD(r[0],24928u,4);r[15]=0x60dcu;return 0;
        case 0x60dcu: /* mvn r1,#0x0 */
            r[1]=~0u;r[15]=0x60e0u;return 0;
        case 0x60e0u: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x60e4u;return 0;
        case 0x60e4u: /* mov r0,r0 */
            r[0]=r[0];r[15]=0x60e8u;return 0;
        case 0x60e8u: /* mov r0,r0 */
            r[0]=r[0];r[15]=0x60ecu;return 0;
        case 0x60ecu: /* mov r0,r0 */
            r[0]=r[0];r[15]=0x60f0u;return 0;
        case 0x60f0u: /* mov r0,r0 */
            r[0]=r[0];r[15]=0x60f4u;return 0;
        case 0x60f4u: /* b 0x000060e4 */
            r[15]=24804u;return 0;
        case 0x6d8cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6d90u;return 0;
        case 0x6d90u: /* ldr r12,[0x70a8] */
            LOAD(r[12],28840u,4);r[15]=0x6d94u;return 0;
        case 0x6d94u: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x6d98u;return 0;
        case 0x6d98u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x6d9cu;return 0;
        case 0x6d9cu: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x6da0u;return 0;
        case 0x6da0u: /* b 0x00006dc0 */
            r[15]=28096u;return 0;
        case 0x6da4u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6da8u;return 0;
        case 0x6da8u: /* ldr r12,[0x70a0] */
            LOAD(r[12],28832u,4);r[15]=0x6dacu;return 0;
        case 0x6dacu: /* ldr r7,[0x70a4] */
            LOAD(r[7],28836u,4);r[15]=0x6db0u;return 0;
        case 0x6db0u: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x6db4u;return 0;
        case 0x6db4u: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x6db8u;return 0;
        case 0x6db8u: /* tst r0,#0x20 */
            temporary=r[0]&32u;logic_flags(state,temporary,0,32u);r[15]=0x6dbcu;return 0;
        case 0x6dbcu: /* bleq 0x00006df4 */
            if(state->cpsr&0x40000000u){r[14]=0x6dc0u;r[15]=28148u;}else{r[15]=0x6dc0u;}return 0;
        case 0x6dc0u: /* ldrb r7,[r12,#0x0] */
            LOAD(r[7],r[12]+0u,1);r[15]=0x6dc4u;return 0;
        case 0x6dc4u: /* orr r8,r7,#0x4 */
            r[8]=r[7]|4u;r[15]=0x6dc8u;return 0;
        case 0x6dc8u: /* strb r8,[r12,#0x0] */
            STORE(r[12]+0u,r[8],1);r[15]=0x6dccu;return 0;
        case 0x6dccu: /* ldrb r7,[r12,#0x1e] */
            LOAD(r[7],r[12]+30u,1);r[15]=0x6dd0u;return 0;
        case 0x6dd0u: /* bl 0x00006e7c */
            r[14]=0x6dd4u;r[15]=28284u;return 0;
        case 0x6dd4u: /* strb r1,[r12,#0x18] */
            STORE(r[12]+24u,r[1],1);r[15]=0x6dd8u;return 0;
        case 0x6dd8u: /* strb r2,[r12,#0x14] */
            STORE(r[12]+20u,r[2],1);r[15]=0x6ddcu;return 0;
        case 0x6ddcu: /* strb r2,[r12,#0x1c] */
            STORE(r[12]+28u,r[2],1);r[15]=0x6de0u;return 0;
        case 0x6de0u: /* strb r3,[r12,#0x15] */
            STORE(r[12]+21u,r[3],1);r[15]=0x6de4u;return 0;
        case 0x6de4u: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x6de8u;return 0;
        case 0x6de8u: /* strbeq r1,[r12,#0x1e] */
            if(state->cpsr&0x40000000u){STORE(r[12]+30u,r[1],1);}r[15]=0x6decu;return 0;
        case 0x6decu: /* bl 0x00006df4 */
            r[14]=0x6df0u;r[15]=28148u;return 0;
        case 0x6df0u: /* b 0x00006dc0 */
            r[15]=28096u;return 0;
        case 0x6df4u: /* tst r0,#0x10 */
            temporary=r[0]&16u;logic_flags(state,temporary,0,16u);r[15]=0x6df8u;return 0;
        case 0x6df8u: /* ldmiaeq sp!,{lr} */
            if(state->cpsr&0x40000000u){LOAD(r[14],r[13]+0u,4);r[13]+=4u;}r[15]=0x6dfcu;return 0;
        case 0x6dfcu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x6e00u;}return 0;
        case 0x6e00u: /* bic r0,r0,#0x10 */
            r[0]=r[0]&~16u;r[15]=0x6e04u;return 0;
        case 0x6e04u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x6e08u;return 0;
        case 0x6e08u: /* mov r1,r4 */
            r[1]=r[4];r[15]=0x6e0cu;return 0;
        case 0x6e0cu: /* mov r2,r5 */
            r[2]=r[5];r[15]=0x6e10u;return 0;
        case 0x6e10u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6e14u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6e18u;return 0;
        case 0x6e18u: /* ldr r12,[0x70a8] */
            LOAD(r[12],28840u,4);r[15]=0x6e1cu;return 0;
        case 0x6e1cu: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x6e20u;return 0;
        case 0x6e20u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x6e24u;return 0;
        case 0x6e24u: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x6e28u;return 0;
        case 0x6e28u: /* b 0x00006e48 */
            r[15]=28232u;return 0;
        case 0x6e2cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6e30u;return 0;
        case 0x6e30u: /* ldr r12,[0x70a0] */
            LOAD(r[12],28832u,4);r[15]=0x6e34u;return 0;
        case 0x6e34u: /* ldr r7,[0x70a4] */
            LOAD(r[7],28836u,4);r[15]=0x6e38u;return 0;
        case 0x6e38u: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x6e3cu;return 0;
        case 0x6e3cu: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x6e40u;return 0;
        case 0x6e40u: /* tst r0,#0x20 */
            temporary=r[0]&32u;logic_flags(state,temporary,0,32u);r[15]=0x6e44u;return 0;
        case 0x6e44u: /* bleq 0x00006df4 */
            if(state->cpsr&0x40000000u){r[14]=0x6e48u;r[15]=28148u;}else{r[15]=0x6e48u;}return 0;
        case 0x6e48u: /* ldrb r7,[r12,#0x0] */
            LOAD(r[7],r[12]+0u,1);r[15]=0x6e4cu;return 0;
        case 0x6e4cu: /* orr r7,r7,#0x10 */
            r[7]=r[7]|16u;r[15]=0x6e50u;return 0;
        case 0x6e50u: /* strb r7,[r12,#0x0] */
            STORE(r[12]+0u,r[7],1);r[15]=0x6e54u;return 0;
        case 0x6e54u: /* ldrb r7,[r12,#0x1f] */
            LOAD(r[7],r[12]+31u,1);r[15]=0x6e58u;return 0;
        case 0x6e58u: /* bl 0x00006e7c */
            r[14]=0x6e5cu;r[15]=28284u;return 0;
        case 0x6e5cu: /* strb r1,[r12,#0x19] */
            STORE(r[12]+25u,r[1],1);r[15]=0x6e60u;return 0;
        case 0x6e60u: /* strb r2,[r12,#0x16] */
            STORE(r[12]+22u,r[2],1);r[15]=0x6e64u;return 0;
        case 0x6e64u: /* strb r2,[r12,#0x1d] */
            STORE(r[12]+29u,r[2],1);r[15]=0x6e68u;return 0;
        case 0x6e68u: /* strb r3,[r12,#0x17] */
            STORE(r[12]+23u,r[3],1);r[15]=0x6e6cu;return 0;
        case 0x6e6cu: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x6e70u;return 0;
        case 0x6e70u: /* strbeq r1,[r12,#0x1f] */
            if(state->cpsr&0x40000000u){STORE(r[12]+31u,r[1],1);}r[15]=0x6e74u;return 0;
        case 0x6e74u: /* bl 0x00006df4 */
            r[14]=0x6e78u;r[15]=28148u;return 0;
        case 0x6e78u: /* b 0x00006e48 */
            r[15]=28232u;return 0;
        case 0x6e7cu: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x6e80u;return 0;
        case 0x6e80u: /* moveq r3,#0x0 */
            if(state->cpsr&0x40000000u){r[3]=0u;}r[15]=0x6e84u;return 0;
        case 0x6e84u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x6e88u;}return 0;
        case 0x6e88u: /* mov r2,r2, lsr #0x2 */
            r[2]=(r[2]>>2);r[15]=0x6e8cu;return 0;
        case 0x6e8cu: /* rsbs r3,r1,r7 */
            r[3]=subtract(state,r[7],r[1]);r[15]=0x6e90u;return 0;
        case 0x6e90u: /* ldr r7,[0x6ed0] */
            LOAD(r[7],28368u,4);r[15]=0x6e94u;return 0;
        case 0x6e94u: /* eormi r3,r3,r7 */
            if(state->cpsr&0x80000000u){r[3]=r[3]^r[7];}r[15]=0x6e98u;return 0;
        case 0x6e98u: /* addmi r3,r3,#0x1 */
            if(state->cpsr&0x80000000u){r[3]=r[3]+1u;}r[15]=0x6e9cu;return 0;
        case 0x6e9cu: /* and r3,r3,#0xff */
            r[3]=r[3]&255u;r[15]=0x6ea0u;return 0;
        case 0x6ea0u: /* ldr r7,[0x6ed4] */
            LOAD(r[7],28372u,4);r[15]=0x6ea4u;return 0;
        case 0x6ea4u: /* tst r3,#0xfe */
            temporary=r[3]&254u;logic_flags(state,temporary,0,254u);r[15]=0x6ea8u;return 0;
        case 0x6ea8u: /* bne 0x00006ebc */
            if(!(state->cpsr&0x40000000u)){r[15]=28348u;}else{r[15]=0x6eacu;}return 0;
        case 0x6eacu: /* ldr r7,[0x6ed8] */
            LOAD(r[7],28376u,4);r[15]=0x6eb0u;return 0;
        case 0x6eb0u: /* tst r2,r7 */
            temporary=r[2]&r[7];logic_flags(state,temporary,0,r[7]);r[15]=0x6eb4u;return 0;
        case 0x6eb4u: /* movne r2,#0xff */
            if(!(state->cpsr&0x40000000u)){r[2]=255u;}r[15]=0x6eb8u;return 0;
        case 0x6eb8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x6ebcu: /* tst r2,r7 */
            temporary=r[2]&r[7];logic_flags(state,temporary,0,r[7]);r[15]=0x6ec0u;return 0;
        case 0x6ec0u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x6ec4u;}return 0;
        case 0x6ec4u: /* mov r3,r3, lsr #0x1 */
            r[3]=(r[3]>>1);r[15]=0x6ec8u;return 0;
        case 0x6ec8u: /* mov r2,r2, lsr #0x1 */
            r[2]=(r[2]>>1);r[15]=0x6eccu;return 0;
        case 0x6eccu: /* b 0x00006ea4 */
            r[15]=28324u;return 0;
        case 0x6edcu: /* ldr r12,[0x70a8] */
            LOAD(r[12],28840u,4);r[15]=0x6ee0u;return 0;
        case 0x6ee0u: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x6ee4u;return 0;
        case 0x6ee4u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x6ee8u;return 0;
        case 0x6ee8u: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x6eecu;return 0;
        case 0x6eecu: /* mov r8,#0x1 */
            r[8]=1u;r[15]=0x6ef0u;return 0;
        case 0x6ef0u: /* b 0x00006f08 */
            r[15]=28424u;return 0;
        case 0x6ef4u: /* ldr r12,[0x70a0] */
            LOAD(r[12],28832u,4);r[15]=0x6ef8u;return 0;
        case 0x6ef8u: /* ldr r7,[0x70a4] */
            LOAD(r[7],28836u,4);r[15]=0x6efcu;return 0;
        case 0x6efcu: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x6f00u;return 0;
        case 0x6f00u: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x6f04u;return 0;
        case 0x6f04u: /* mov r8,#0x2 */
            r[8]=2u;r[15]=0x6f08u;return 0;
        case 0x6f08u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6f0cu;return 0;
        case 0x6f0cu: /* bic r2,r2,#0xf */
            r[2]=r[2]&~15u;r[15]=0x6f10u;return 0;
        case 0x6f10u: /* cmp r1,#0xff */
            (void)subtract(state,r[1],255u);r[15]=0x6f14u;return 0;
        case 0x6f14u: /* addne r4,r1,#0x40 */
            if(!(state->cpsr&0x40000000u)){r[4]=r[1]+64u;}r[15]=0x6f18u;return 0;
        case 0x6f18u: /* strb r2,[r12,#0x22] */
            STORE(r[12]+34u,r[2],1);r[15]=0x6f1cu;return 0;
        case 0x6f1cu: /* strb r4,[r12,#0x29] */
            STORE(r[12]+41u,r[4],1);r[15]=0x6f20u;return 0;
        case 0x6f20u: /* ldrb r3,[r12,#0x0] */
            LOAD(r[3],r[12]+0u,1);r[15]=0x6f24u;return 0;
        case 0x6f24u: /* tst r3,#0x1 */
            temporary=r[3]&1u;logic_flags(state,temporary,0,1u);r[15]=0x6f28u;return 0;
        case 0x6f28u: /* beq 0x00006f5c */
            if(state->cpsr&0x40000000u){r[15]=28508u;}else{r[15]=0x6f2cu;}return 0;
        case 0x6f2cu: /* cmp r1,#0xff */
            (void)subtract(state,r[1],255u);r[15]=0x6f30u;return 0;
        case 0x6f30u: /* movne r6,r2 */
            if(!(state->cpsr&0x40000000u)){r[6]=r[2];}r[15]=0x6f34u;return 0;
        case 0x6f34u: /* bne 0x00006f40 */
            if(!(state->cpsr&0x40000000u)){r[15]=28480u;}else{r[15]=0x6f38u;}return 0;
        case 0x6f38u: /* ldrb r1,[r12,#0x25] */
            LOAD(r[1],r[12]+37u,1);r[15]=0x6f3cu;return 0;
        case 0x6f3cu: /* ldrb r6,[r12,#0x26] */
            LOAD(r[6],r[12]+38u,1);r[15]=0x6f40u;return 0;
        case 0x6f40u: /* bl 0x00007084 */
            r[14]=0x6f44u;r[15]=28804u;return 0;
        case 0x6f44u: /* ldrb r7,[r12,#0x1a] */
            LOAD(r[7],r[12]+26u,1);r[15]=0x6f48u;return 0;
        case 0x6f48u: /* mov r6,r6, lsr #0x4 */
            r[6]=(r[6]>>4);r[15]=0x6f4cu;return 0;
        case 0x6f4cu: /* bl 0x00007064 */
            r[14]=0x6f50u;r[15]=28772u;return 0;
        case 0x6f50u: /* mov r7,r7, lsl #0x4 */
            r[7]=(r[7]<<4);r[15]=0x6f54u;return 0;
        case 0x6f54u: /* add r7,r1,r7 */
            r[7]=r[1]+r[7];r[15]=0x6f58u;return 0;
        case 0x6f58u: /* strb r7,[r11,#0x20] */
            STORE(r[11]+32u,r[7],1);r[15]=0x6f5cu;return 0;
        case 0x6f5cu: /* subs r8,r8,#0x1 */
            r[8]=subtract(state,r[8],1u);r[15]=0x6f60u;return 0;
        case 0x6f60u: /* ldmiaeq sp!,{lr} */
            if(state->cpsr&0x40000000u){LOAD(r[14],r[13]+0u,4);r[13]+=4u;}r[15]=0x6f64u;return 0;
        case 0x6f64u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x6f68u;}return 0;
        case 0x6f68u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x6f6cu;return 0;
        case 0x6f6cu: /* b 0x00006f18 */
            r[15]=28440u;return 0;
        case 0x6f70u: /* ldr r12,[0x70a8] */
            LOAD(r[12],28840u,4);r[15]=0x6f74u;return 0;
        case 0x6f74u: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x6f78u;return 0;
        case 0x6f78u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x6f7cu;return 0;
        case 0x6f7cu: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x6f80u;return 0;
        case 0x6f80u: /* mov r8,#0x1 */
            r[8]=1u;r[15]=0x6f84u;return 0;
        case 0x6f84u: /* b 0x00006f9c */
            r[15]=28572u;return 0;
        case 0x6f88u: /* ldr r12,[0x70a0] */
            LOAD(r[12],28832u,4);r[15]=0x6f8cu;return 0;
        case 0x6f8cu: /* ldr r7,[0x70a4] */
            LOAD(r[7],28836u,4);r[15]=0x6f90u;return 0;
        case 0x6f90u: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x6f94u;return 0;
        case 0x6f94u: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x6f98u;return 0;
        case 0x6f98u: /* mov r8,#0x2 */
            r[8]=2u;r[15]=0x6f9cu;return 0;
        case 0x6f9cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x6fa0u;return 0;
        case 0x6fa0u: /* strb r1,[r12,#0x1a] */
            STORE(r[12]+26u,r[1],1);r[15]=0x6fa4u;return 0;
        case 0x6fa4u: /* ldrb r2,[r12,#0x0] */
            LOAD(r[2],r[12]+0u,1);r[15]=0x6fa8u;return 0;
        case 0x6fa8u: /* tst r2,#0x1 */
            temporary=r[2]&1u;logic_flags(state,temporary,0,1u);r[15]=0x6facu;return 0;
        case 0x6facu: /* beq 0x00006fe4 */
            if(state->cpsr&0x40000000u){r[15]=28644u;}else{r[15]=0x6fb0u;}return 0;
        case 0x6fb0u: /* bl 0x00007084 */
            r[14]=0x6fb4u;r[15]=28804u;return 0;
        case 0x6fb4u: /* mov r7,r1 */
            r[7]=r[1];r[15]=0x6fb8u;return 0;
        case 0x6fb8u: /* ldrb r2,[r12,#0x29] */
            LOAD(r[2],r[12]+41u,1);r[15]=0x6fbcu;return 0;
        case 0x6fbcu: /* tst r2,#0x40 */
            temporary=r[2]&64u;logic_flags(state,temporary,0,64u);r[15]=0x6fc0u;return 0;
        case 0x6fc0u: /* ldrbne r6,[r12,#0x22] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[6],r[12]+34u,1);}r[15]=0x6fc4u;return 0;
        case 0x6fc4u: /* ldrbeq r6,[r12,#0x26] */
            if(state->cpsr&0x40000000u){LOAD(r[6],r[12]+38u,1);}r[15]=0x6fc8u;return 0;
        case 0x6fc8u: /* mov r6,r6, lsr #0x4 */
            r[6]=(r[6]>>4);r[15]=0x6fccu;return 0;
        case 0x6fccu: /* bl 0x00007064 */
            r[14]=0x6fd0u;r[15]=28772u;return 0;
        case 0x6fd0u: /* mov r7,r7, lsl #0x4 */
            r[7]=(r[7]<<4);r[15]=0x6fd4u;return 0;
        case 0x6fd4u: /* ldrb r6,[r11,#0x20] */
            LOAD(r[6],r[11]+32u,1);r[15]=0x6fd8u;return 0;
        case 0x6fd8u: /* and r6,r6,#0xf */
            r[6]=r[6]&15u;r[15]=0x6fdcu;return 0;
        case 0x6fdcu: /* orr r7,r6,r7 */
            r[7]=r[6]|r[7];r[15]=0x6fe0u;return 0;
        case 0x6fe0u: /* strb r7,[r11,#0x20] */
            STORE(r[11]+32u,r[7],1);r[15]=0x6fe4u;return 0;
        case 0x6fe4u: /* subs r8,r8,#0x1 */
            r[8]=subtract(state,r[8],1u);r[15]=0x6fe8u;return 0;
        case 0x6fe8u: /* ldmiaeq sp!,{lr} */
            if(state->cpsr&0x40000000u){LOAD(r[14],r[13]+0u,4);r[13]+=4u;}r[15]=0x6fecu;return 0;
        case 0x6fecu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x6ff0u;}return 0;
        case 0x6ff0u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x6ff4u;return 0;
        case 0x6ff4u: /* b 0x00006fa0 */
            r[15]=28576u;return 0;
        case 0x6ff8u: /* ldr r12,[0x70a8] */
            LOAD(r[12],28840u,4);r[15]=0x6ffcu;return 0;
        case 0x6ffcu: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x7000u;return 0;
        case 0x7000u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x7004u;return 0;
        case 0x7004u: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x7008u;return 0;
        case 0x7008u: /* mov r8,#0x1 */
            r[8]=1u;r[15]=0x700cu;return 0;
        case 0x700cu: /* b 0x00007024 */
            r[15]=28708u;return 0;
        case 0x7010u: /* ldr r12,[0x70a0] */
            LOAD(r[12],28832u,4);r[15]=0x7014u;return 0;
        case 0x7014u: /* ldr r7,[0x70a4] */
            LOAD(r[7],28836u,4);r[15]=0x7018u;return 0;
        case 0x7018u: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x701cu;return 0;
        case 0x701cu: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x7020u;return 0;
        case 0x7020u: /* mov r8,#0x2 */
            r[8]=2u;r[15]=0x7024u;return 0;
        case 0x7024u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7028u;return 0;
        case 0x7028u: /* strb r1,[r12,#0x1b] */
            STORE(r[12]+27u,r[1],1);r[15]=0x702cu;return 0;
        case 0x702cu: /* ldrb r2,[r12,#0x0] */
            LOAD(r[2],r[12]+0u,1);r[15]=0x7030u;return 0;
        case 0x7030u: /* tst r2,#0x1 */
            temporary=r[2]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7034u;return 0;
        case 0x7034u: /* beq 0x00007050 */
            if(state->cpsr&0x40000000u){r[15]=28752u;}else{r[15]=0x7038u;}return 0;
        case 0x7038u: /* bl 0x00007084 */
            r[14]=0x703cu;r[15]=28804u;return 0;
        case 0x703cu: /* mov r7,r1 */
            r[7]=r[1];r[15]=0x7040u;return 0;
        case 0x7040u: /* ldrb r6,[r12,#0x2a] */
            LOAD(r[6],r[12]+42u,1);r[15]=0x7044u;return 0;
        case 0x7044u: /* bl 0x00007064 */
            r[14]=0x7048u;r[15]=28772u;return 0;
        case 0x7048u: /* strb r7,[r12,#0x2b] */
            STORE(r[12]+43u,r[7],1);r[15]=0x704cu;return 0;
        case 0x704cu: /* strb r7,[r11,#0x25] */
            STORE(r[11]+37u,r[7],1);r[15]=0x7050u;return 0;
        case 0x7050u: /* subs r8,r8,#0x1 */
            r[8]=subtract(state,r[8],1u);r[15]=0x7054u;return 0;
        case 0x7054u: /* ldmiaeq sp!,{lr} */
            if(state->cpsr&0x40000000u){LOAD(r[14],r[13]+0u,4);r[13]+=4u;}r[15]=0x7058u;return 0;
        case 0x7058u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x705cu;}return 0;
        case 0x705cu: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x7060u;return 0;
        case 0x7060u: /* b 0x00007028 */
            r[15]=28712u;return 0;
        case 0x7064u: /* subs r7,r7,#0x80 */
            r[7]=subtract(state,r[7],128u);r[15]=0x7068u;return 0;
        case 0x7068u: /* mov r6,r6, lsl #0x3 */
            r[6]=(r[6]<<3);r[15]=0x706cu;return 0;
        case 0x706cu: /* adds r7,r6,r7 */
            r[7]=addition(state,r[6],r[7]);r[15]=0x7070u;return 0;
        case 0x7070u: /* mov r7,r7, asr #0x3 */
            r[7]=(r[7]>>3)|((0u-(r[7]>>31))<<29);r[15]=0x7074u;return 0;
        case 0x7074u: /* movmi r7,#0x0 */
            if(state->cpsr&0x80000000u){r[7]=0u;}r[15]=0x7078u;return 0;
        case 0x7078u: /* cmp r7,#0xf */
            (void)subtract(state,r[7],15u);r[15]=0x707cu;return 0;
        case 0x707cu: /* movgt r7,#0xf */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[7]=15u;r[15]=0x7080u;}else{r[15]=0x7080u;}return 0;
        case 0x7080u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7084u: /* ldrb r7,[r12,#0x3] */
            LOAD(r[7],r[12]+3u,1);r[15]=0x7088u;return 0;
        case 0x7088u: /* ldr r10,[0x709c] */
            LOAD(r[10],28828u,4);r[15]=0x708cu;return 0;
        case 0x708cu: /* mov r9,#0xc */
            r[9]=12u;r[15]=0x7090u;return 0;
        case 0x7090u: /* mla r10,r7,r9,r10 */
            r[10]=r[7]*r[9]+r[10];r[15]=0x7094u;return 0;
        case 0x7094u: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x7098u;return 0;
        case 0x7098u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x740cu: /* ldr r3,[r12,#0xc] */
            LOAD(r[3],r[12]+12u,4);r[15]=0x7410u;return 0;
        case 0x7410u: /* cmp r1,r3 */
            (void)subtract(state,r[1],r[3]);r[15]=0x7414u;return 0;
        case 0x7414u: /* movcs r0,#0x4 */
            if(state->cpsr&0x20000000u){r[0]=4u;}r[15]=0x7418u;return 0;
        case 0x7418u: /* bcs 0x000074dc */
            if(state->cpsr&0x20000000u){r[15]=29916u;}else{r[15]=0x741cu;}return 0;
        case 0x741cu: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x7420u;return 0;
        case 0x7420u: /* mov r3,#0x40 */
            r[3]=64u;r[15]=0x7424u;return 0;
        case 0x7424u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x7428u;return 0;
        case 0x7428u: /* mla r11,r3,r0,r11 */
            r[11]=r[3]*r[0]+r[11];r[15]=0x742cu;return 0;
        case 0x742cu: /* bic r2,r2,#0x7 */
            r[2]=r[2]&~7u;r[15]=0x7430u;return 0;
        case 0x7430u: /* ldr r4,[r11,#0x1] */
            LOAD(r[4],r[11]+1u,4);r[15]=0x7434u;return 0;
        case 0x7434u: /* tst r4,#0x1 */
            temporary=r[4]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7438u;return 0;
        case 0x7438u: /* beq 0x0000746c */
            if(state->cpsr&0x40000000u){r[15]=29804u;}else{r[15]=0x743cu;}return 0;
        case 0x743cu: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x7440u;return 0;
        case 0x7440u: /* beq 0x00007460 */
            if(state->cpsr&0x40000000u){r[15]=29792u;}else{r[15]=0x7444u;}return 0;
        case 0x7444u: /* ldrb r3,[r11,#0x0] */
            LOAD(r[3],r[11]+0u,1);r[15]=0x7448u;return 0;
        case 0x7448u: /* cmp r3,#0x0 */
            (void)subtract(state,r[3],0u);r[15]=0x744cu;return 0;
        case 0x744cu: /* beq 0x00007460 */
            if(state->cpsr&0x40000000u){r[15]=29792u;}else{r[15]=0x7450u;}return 0;
        case 0x7450u: /* cmp r2,r3 */
            (void)subtract(state,r[2],r[3]);r[15]=0x7454u;return 0;
        case 0x7454u: /* bcs 0x00007460 */
            if(state->cpsr&0x20000000u){r[15]=29792u;}else{r[15]=0x7458u;}return 0;
        case 0x7458u: /* mov r0,#0x2 */
            r[0]=2u;r[15]=0x745cu;return 0;
        case 0x745cu: /* b 0x000074dc */
            r[15]=29916u;return 0;
        case 0x7460u: /* stmdb sp!,{r0,r1,r2,r12,lr} */
            STORE(r[13]-20u,r[0],4);STORE(r[13]-16u,r[1],4);STORE(r[13]-12u,r[2],4);STORE(r[13]-8u,r[12],4);STORE(r[13]-4u,r[14],4);r[13]-=20u;r[15]=0x7464u;return 0;
        case 0x7464u: /* bl 0x00007504 */
            r[14]=0x7468u;r[15]=29956u;return 0;
        case 0x7468u: /* ldmia sp!,{r0,r1,r2,r12,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[1],r[13]+4u,4);LOAD(r[2],r[13]+8u,4);LOAD(r[12],r[13]+12u,4);LOAD(r[14],r[13]+16u,4);r[13]+=20u;r[15]=0x746cu;return 0;
        case 0x746cu: /* strb r2,[r11,#0x0] */
            STORE(r[11]+0u,r[2],1);r[15]=0x7470u;return 0;
        case 0x7470u: /* add r10,r12,#0x10 */
            r[10]=r[12]+16u;r[15]=0x7474u;return 0;
        case 0x7474u: /* add r1,r10,r1, lsl #0x2 */
            r[1]=r[10]+(r[1]<<2);r[15]=0x7478u;return 0;
        case 0x7478u: /* ldr r10,[r1,#0x0] */
            LOAD(r[10],r[1]+0u,4);r[15]=0x747cu;return 0;
        case 0x747cu: /* ldr r1,[r1,#0x4] */
            LOAD(r[1],r[1]+4u,4);r[15]=0x7480u;return 0;
        case 0x7480u: /* ldr r2,[0x7700] */
            LOAD(r[2],30464u,4);r[15]=0x7484u;return 0;
        case 0x7484u: /* cmp r1,r2 */
            (void)subtract(state,r[1],r[2]);r[15]=0x7488u;return 0;
        case 0x7488u: /* ldreq r1,[r12,#0x8] */
            if(state->cpsr&0x40000000u){LOAD(r[1],r[12]+8u,4);}r[15]=0x748cu;return 0;
        case 0x748cu: /* subeq r1,r1,#0x4 */
            if(state->cpsr&0x40000000u){r[1]=r[1]-4u;}r[15]=0x7490u;return 0;
        case 0x7490u: /* add r1,r1,r12 */
            r[1]=r[1]+r[12];r[15]=0x7494u;return 0;
        case 0x7494u: /* add r12,r12,r10 */
            r[12]=r[12]+r[10];r[15]=0x7498u;return 0;
        case 0x7498u: /* str r12,[r11,#0x14] */
            STORE(r[11]+20u,r[12],4);r[15]=0x749cu;return 0;
        case 0x749cu: /* ldr r2,[r12,#0x4]! */
            LOAD(r[2],r[12]+4u,4);r[12]=r[12]+4u;r[15]=0x74a0u;return 0;
        case 0x74a0u: /* ldr r3,[r12,#0x4]! */
            LOAD(r[3],r[12]+4u,4);r[12]=r[12]+4u;r[15]=0x74a4u;return 0;
        case 0x74a4u: /* str r3,[r11,#0x34] */
            STORE(r[11]+52u,r[3],4);r[15]=0x74a8u;return 0;
        case 0x74a8u: /* str r3,[r11,#0x38] */
            STORE(r[11]+56u,r[3],4);r[15]=0x74acu;return 0;
        case 0x74acu: /* mul r3,r2,r3 */
            r[3]=r[2]*r[3];r[15]=0x74b0u;return 0;
        case 0x74b0u: /* mov r3,r3, lsr #0x4 */
            r[3]=(r[3]>>4);r[15]=0x74b4u;return 0;
        case 0x74b4u: /* str r1,[r11,#0x4] */
            STORE(r[11]+4u,r[1],4);r[15]=0x74b8u;return 0;
        case 0x74b8u: /* str r2,[r11,#0x8] */
            STORE(r[11]+8u,r[2],4);r[15]=0x74bcu;return 0;
        case 0x74bcu: /* str r3,[r11,#0xc] */
            STORE(r[11]+12u,r[3],4);r[15]=0x74c0u;return 0;
        case 0x74c0u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x74c4u;return 0;
        case 0x74c4u: /* str r1,[r11,#0x30] */
            STORE(r[11]+48u,r[1],4);r[15]=0x74c8u;return 0;
        case 0x74c8u: /* add r12,r12,#0x4 */
            r[12]=r[12]+4u;r[15]=0x74ccu;return 0;
        case 0x74ccu: /* str r12,[r11,#0x10] */
            STORE(r[11]+16u,r[12],4);r[15]=0x74d0u;return 0;
        case 0x74d0u: /* mov r1,#0x1 */
            r[1]=1u;r[15]=0x74d4u;return 0;
        case 0x74d4u: /* strb r1,[r11,#0x1] */
            STORE(r[11]+1u,r[1],1);r[15]=0x74d8u;return 0;
        case 0x74d8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x74dcu: /* ldr r9,[0x76f0] */
            LOAD(r[9],30448u,4);r[15]=0x74e0u;return 0;
        case 0x74e0u: /* ldr r1,[r9,#0x0] */
            LOAD(r[1],r[9]+0u,4);r[15]=0x74e4u;return 0;
        case 0x74e4u: /* orr r0,r1,r0 */
            r[0]=r[1]|r[0];r[15]=0x74e8u;return 0;
        case 0x74e8u: /* strb r0,[r9,#0x0] */
            STORE(r[9]+0u,r[0],1);r[15]=0x74ecu;return 0;
        case 0x74ecu: /* ldrb r1,[r11,#0x1] */
            LOAD(r[1],r[11]+1u,1);r[15]=0x74f0u;return 0;
        case 0x74f0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x74f4u: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x74f8u;return 0;
        case 0x74f8u: /* mov r2,#0x40 */
            r[2]=64u;r[15]=0x74fcu;return 0;
        case 0x74fcu: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x7500u;return 0;
        case 0x7500u: /* mla r11,r0,r2,r11 */
            r[11]=r[0]*r[2]+r[11];r[15]=0x7504u;return 0;
        case 0x7504u: /* ldrb r3,[r11,#0x1] */
            LOAD(r[3],r[11]+1u,1);r[15]=0x7508u;return 0;
        case 0x7508u: /* bic r3,r3,#0x1 */
            r[3]=r[3]&~1u;r[15]=0x750cu;return 0;
        case 0x750cu: /* strb r3,[r11,#0x1] */
            STORE(r[11]+1u,r[3],1);r[15]=0x7510u;return 0;
        case 0x7510u: /* mov r3,#0x0 */
            r[3]=0u;r[15]=0x7514u;return 0;
        case 0x7514u: /* strb r3,[r11,#0x0] */
            STORE(r[11]+0u,r[3],1);r[15]=0x7518u;return 0;
        case 0x7518u: /* strb r3,[r11,#0x2] */
            STORE(r[11]+2u,r[3],1);r[15]=0x751cu;return 0;
        case 0x751cu: /* str r3,[r11,#0x4] */
            STORE(r[11]+4u,r[3],4);r[15]=0x7520u;return 0;
        case 0x7520u: /* str r3,[r11,#0x8] */
            STORE(r[11]+8u,r[3],4);r[15]=0x7524u;return 0;
        case 0x7524u: /* str r3,[r11,#0xc] */
            STORE(r[11]+12u,r[3],4);r[15]=0x7528u;return 0;
        case 0x7528u: /* strb r3,[r11,#0x3] */
            STORE(r[11]+3u,r[3],1);r[15]=0x752cu;return 0;
        case 0x752cu: /* str r3,[r11,#0x1c] */
            STORE(r[11]+28u,r[3],4);r[15]=0x7530u;return 0;
        case 0x7530u: /* str r3,[r11,#0x20] */
            STORE(r[11]+32u,r[3],4);r[15]=0x7534u;return 0;
        case 0x7534u: /* str r3,[r11,#0x24] */
            STORE(r[11]+36u,r[3],4);r[15]=0x7538u;return 0;
        case 0x7538u: /* str r3,[r11,#0x28] */
            STORE(r[11]+40u,r[3],4);r[15]=0x753cu;return 0;
        case 0x753cu: /* str r3,[r11,#0x2c] */
            STORE(r[11]+44u,r[3],4);r[15]=0x7540u;return 0;
        case 0x7540u: /* str r3,[r11,#0x34] */
            STORE(r[11]+52u,r[3],4);r[15]=0x7544u;return 0;
        case 0x7544u: /* ldr r3,[0x76f8] */
            LOAD(r[3],30456u,4);r[15]=0x7548u;return 0;
        case 0x7548u: /* add r4,r3,#0x240 */
            r[4]=r[3]+576u;r[15]=0x754cu;return 0;
        case 0x754cu: /* ldr r5,[r3,#0x0] */
            LOAD(r[5],r[3]+0u,4);r[15]=0x7550u;return 0;
        case 0x7550u: /* cmp r5,#0x0 */
            (void)subtract(state,r[5],0u);r[15]=0x7554u;return 0;
        case 0x7554u: /* beq 0x00007574 */
            if(state->cpsr&0x40000000u){r[15]=30068u;}else{r[15]=0x7558u;}return 0;
        case 0x7558u: /* and r6,r5,#0xf00000 */
            r[6]=r[5]&15728640u;r[15]=0x755cu;return 0;
        case 0x755cu: /* cmp r0,r6, lsr #0x14 */
            (void)subtract(state,r[0],(r[6]>>20));r[15]=0x7560u;return 0;
        case 0x7560u: /* bne 0x00007574 */
            if(!(state->cpsr&0x40000000u)){r[15]=30068u;}else{r[15]=0x7564u;}return 0;
        case 0x7564u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7568u;return 0;
        case 0x7568u: /* str r6,[r3,#0x0] */
            STORE(r[3]+0u,r[6],4);r[15]=0x756cu;return 0;
        case 0x756cu: /* str r6,[r3,#0x4] */
            STORE(r[3]+4u,r[6],4);r[15]=0x7570u;return 0;
        case 0x7570u: /* str r6,[r3,#0x8] */
            STORE(r[3]+8u,r[6],4);r[15]=0x7574u;return 0;
        case 0x7574u: /* add r3,r3,#0xc */
            r[3]=r[3]+12u;r[15]=0x7578u;return 0;
        case 0x7578u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x757cu;return 0;
        case 0x757cu: /* bcs 0x000075a8 */
            if(state->cpsr&0x20000000u){r[15]=30120u;}else{r[15]=0x7580u;}return 0;
        case 0x7580u: /* b 0x0000754c */
            r[15]=30028u;return 0;
        case 0x7584u: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x7588u;return 0;
        case 0x7588u: /* mov r2,#0x40 */
            r[2]=64u;r[15]=0x758cu;return 0;
        case 0x758cu: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x7590u;return 0;
        case 0x7590u: /* mla r11,r0,r2,r11 */
            r[11]=r[0]*r[2]+r[11];r[15]=0x7594u;return 0;
        case 0x7594u: /* ldrb r2,[r11,#0x1] */
            LOAD(r[2],r[11]+1u,1);r[15]=0x7598u;return 0;
        case 0x7598u: /* orr r1,r2,#0x2 */
            r[1]=r[2]|2u;r[15]=0x759cu;return 0;
        case 0x759cu: /* strb r1,[r11,#0x1] */
            STORE(r[11]+1u,r[1],1);r[15]=0x75a0u;return 0;
        case 0x75a0u: /* tst r2,#0x1 */
            temporary=r[2]&1u;logic_flags(state,temporary,0,1u);r[15]=0x75a4u;return 0;
        case 0x75a4u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x75a8u;}return 0;
        case 0x75a8u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x75acu;return 0;
        case 0x75acu: /* ldr r7,[0x76fc] */
            LOAD(r[7],30460u,4);r[15]=0x75b0u;return 0;
        case 0x75b0u: /* mov r3,#0x13000 */
            r[3]=77824u;r[15]=0x75b4u;return 0;
        case 0x75b4u: /* ldr r1,[r3,#0x0] */
            LOAD(r[1],r[3]+0u,4);r[15]=0x75b8u;return 0;
        case 0x75b8u: /* ldr r3,[0x7704] */
            LOAD(r[3],30468u,4);r[15]=0x75bcu;return 0;
        case 0x75bcu: /* ldr r2,[r3,#0x0] */
            LOAD(r[2],r[3]+0u,4);r[15]=0x75c0u;return 0;
        case 0x75c0u: /* mov r3,#0xb000 */
            r[3]=45056u;r[15]=0x75c4u;return 0;
        case 0x75c4u: /* ldr r4,[r3,r2] */
            LOAD(r[4],r[3]+r[2],4);r[15]=0x75c8u;return 0;
        case 0x75c8u: /* and r5,r4,#0x7000000 */
            r[5]=r[4]&117440512u;r[15]=0x75ccu;return 0;
        case 0x75ccu: /* cmp r5,#0x1000000 */
            (void)subtract(state,r[5],16777216u);r[15]=0x75d0u;return 0;
        case 0x75d0u: /* bne 0x000075e0 */
            if(!(state->cpsr&0x40000000u)){r[15]=30176u;}else{r[15]=0x75d4u;}return 0;
        case 0x75d4u: /* and r5,r4,#0xf00000 */
            r[5]=r[4]&15728640u;r[15]=0x75d8u;return 0;
        case 0x75d8u: /* cmp r0,r5, lsr #0x14 */
            (void)subtract(state,r[0],(r[5]>>20));r[15]=0x75dcu;return 0;
        case 0x75dcu: /* streq r6,[r3,r2] */
            if(state->cpsr&0x40000000u){STORE(r[3]+r[2],r[6],4);}r[15]=0x75e0u;return 0;
        case 0x75e0u: /* add r2,r2,#0x4 */
            r[2]=r[2]+4u;r[15]=0x75e4u;return 0;
        case 0x75e4u: /* and r2,r2,r7 */
            r[2]=r[2]&r[7];r[15]=0x75e8u;return 0;
        case 0x75e8u: /* cmp r1,r2 */
            (void)subtract(state,r[1],r[2]);r[15]=0x75ecu;return 0;
        case 0x75ecu: /* bne 0x000075c4 */
            if(!(state->cpsr&0x40000000u)){r[15]=30148u;}else{r[15]=0x75f0u;}return 0;
        case 0x75f0u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x75f4u;return 0;
        case 0x75f4u: /* bl 0x000017bc */
            r[14]=0x75f8u;r[15]=6076u;return 0;
        case 0x75f8u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x75fcu;return 0;
        case 0x75fcu: /* ldrb r1,[r11,#0x1] */
            LOAD(r[1],r[11]+1u,1);r[15]=0x7600u;return 0;
        case 0x7600u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7604u: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x7608u;return 0;
        case 0x7608u: /* mov r2,#0x40 */
            r[2]=64u;r[15]=0x760cu;return 0;
        case 0x760cu: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x7610u;return 0;
        case 0x7610u: /* mla r11,r0,r2,r11 */
            r[11]=r[0]*r[2]+r[11];r[15]=0x7614u;return 0;
        case 0x7614u: /* ldrb r1,[r11,#0x1] */
            LOAD(r[1],r[11]+1u,1);r[15]=0x7618u;return 0;
        case 0x7618u: /* tst r1,#0x2 */
            temporary=r[1]&2u;logic_flags(state,temporary,0,2u);r[15]=0x761cu;return 0;
        case 0x761cu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7620u;}return 0;
        case 0x7620u: /* bic r1,r1,#0x2 */
            r[1]=r[1]&~2u;r[15]=0x7624u;return 0;
        case 0x7624u: /* strb r1,[r11,#0x1] */
            STORE(r[11]+1u,r[1],1);r[15]=0x7628u;return 0;
        case 0x7628u: /* tst r1,#0x1 */
            temporary=r[1]&1u;logic_flags(state,temporary,0,1u);r[15]=0x762cu;return 0;
        case 0x762cu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7630u;}return 0;
        case 0x7630u: /* ldr r10,[0x76f8] */
            LOAD(r[10],30456u,4);r[15]=0x7634u;return 0;
        case 0x7634u: /* add r9,r10,#0x240 */
            r[9]=r[10]+576u;r[15]=0x7638u;return 0;
        case 0x7638u: /* mov r8,#0x13000 */
            r[8]=77824u;r[15]=0x763cu;return 0;
        case 0x763cu: /* ldr r5,[0x76fc] */
            LOAD(r[5],30460u,4);r[15]=0x7640u;return 0;
        case 0x7640u: /* mov r7,#0xb000 */
            r[7]=45056u;r[15]=0x7644u;return 0;
        case 0x7644u: /* ldr r2,[r8,#0x0] */
            LOAD(r[2],r[8]+0u,4);r[15]=0x7648u;return 0;
        case 0x7648u: /* ldr r4,[r8,#0x4] */
            LOAD(r[4],r[8]+4u,4);r[15]=0x764cu;return 0;
        case 0x764cu: /* sub r4,r4,#0x4 */
            r[4]=r[4]-4u;r[15]=0x7650u;return 0;
        case 0x7650u: /* cmp r4,r2 */
            (void)subtract(state,r[4],r[2]);r[15]=0x7654u;return 0;
        case 0x7654u: /* beq 0x000076a0 */
            if(state->cpsr&0x40000000u){r[15]=30368u;}else{r[15]=0x7658u;}return 0;
        case 0x7658u: /* cmp r10,r9 */
            (void)subtract(state,r[10],r[9]);r[15]=0x765cu;return 0;
        case 0x765cu: /* streq r2,[r8,#0x0] */
            if(state->cpsr&0x40000000u){STORE(r[8]+0u,r[2],4);}r[15]=0x7660u;return 0;
        case 0x7660u: /* ldrbeq r1,[r11,#0x1] */
            if(state->cpsr&0x40000000u){LOAD(r[1],r[11]+1u,1);}r[15]=0x7664u;return 0;
        case 0x7664u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7668u;}return 0;
        case 0x7668u: /* ldr r1,[r10],#0xc */
            LOAD(r[1],r[10],4);r[10]+=12u;r[15]=0x766cu;return 0;
        case 0x766cu: /* cmp r1,#0x0 */
            (void)subtract(state,r[1],0u);r[15]=0x7670u;return 0;
        case 0x7670u: /* beq 0x00007658 */
            if(state->cpsr&0x40000000u){r[15]=30296u;}else{r[15]=0x7674u;}return 0;
        case 0x7674u: /* and r3,r1,#0xf00000 */
            r[3]=r[1]&15728640u;r[15]=0x7678u;return 0;
        case 0x7678u: /* cmp r0,r3, lsr #0x14 */
            (void)subtract(state,r[0],(r[3]>>20));r[15]=0x767cu;return 0;
        case 0x767cu: /* bne 0x00007658 */
            if(!(state->cpsr&0x40000000u)){r[15]=30296u;}else{r[15]=0x7680u;}return 0;
        case 0x7680u: /* orr r1,r1,#0x1000000 */
            r[1]=r[1]|16777216u;r[15]=0x7684u;return 0;
        case 0x7684u: /* cmp r4,r2 */
            (void)subtract(state,r[4],r[2]);r[15]=0x7688u;return 0;
        case 0x7688u: /* beq 0x0000769c */
            if(state->cpsr&0x40000000u){r[15]=30364u;}else{r[15]=0x768cu;}return 0;
        case 0x768cu: /* str r1,[r7,r2] */
            STORE(r[7]+r[2],r[1],4);r[15]=0x7690u;return 0;
        case 0x7690u: /* add r2,r2,#0x4 */
            r[2]=r[2]+4u;r[15]=0x7694u;return 0;
        case 0x7694u: /* and r2,r2,r5 */
            r[2]=r[2]&r[5];r[15]=0x7698u;return 0;
        case 0x7698u: /* b 0x00007658 */
            r[15]=30296u;return 0;
        case 0x769cu: /* str r1,[r10,#-0xc]! */
            STORE(r[10]-12u,r[1],4);r[10]=r[10]-12u;r[15]=0x76a0u;return 0;
        case 0x76a0u: /* cmp r10,r9 */
            (void)subtract(state,r[10],r[9]);r[15]=0x76a4u;return 0;
        case 0x76a4u: /* beq 0x000076cc */
            if(state->cpsr&0x40000000u){r[15]=30412u;}else{r[15]=0x76a8u;}return 0;
        case 0x76a8u: /* ldr r1,[r10],#0xc */
            LOAD(r[1],r[10],4);r[10]+=12u;r[15]=0x76acu;return 0;
        case 0x76acu: /* cmp r1,#0x0 */
            (void)subtract(state,r[1],0u);r[15]=0x76b0u;return 0;
        case 0x76b0u: /* beq 0x000076a0 */
            if(state->cpsr&0x40000000u){r[15]=30368u;}else{r[15]=0x76b4u;}return 0;
        case 0x76b4u: /* and r4,r1,#0xf00000 */
            r[4]=r[1]&15728640u;r[15]=0x76b8u;return 0;
        case 0x76b8u: /* cmp r0,r4, lsr #0x14 */
            (void)subtract(state,r[0],(r[4]>>20));r[15]=0x76bcu;return 0;
        case 0x76bcu: /* bne 0x000076a0 */
            if(!(state->cpsr&0x40000000u)){r[15]=30368u;}else{r[15]=0x76c0u;}return 0;
        case 0x76c0u: /* orr r1,r1,#0x1000000 */
            r[1]=r[1]|16777216u;r[15]=0x76c4u;return 0;
        case 0x76c4u: /* str r1,[r10,#-0xc] */
            STORE(r[10]-12u,r[1],4);r[15]=0x76c8u;return 0;
        case 0x76c8u: /* b 0x000076a0 */
            r[15]=30368u;return 0;
        case 0x76ccu: /* mov r0,#0x20 */
            r[0]=32u;r[15]=0x76d0u;return 0;
        case 0x76d0u: /* b 0x000074dc */
            r[15]=29916u;return 0;
        case 0x76d4u: /* mov r11,#0x10000 */
            r[11]=65536u;r[15]=0x76d8u;return 0;
        case 0x76d8u: /* mov r2,#0x40 */
            r[2]=64u;r[15]=0x76dcu;return 0;
        case 0x76dcu: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x76e0u;return 0;
        case 0x76e0u: /* mla r11,r0,r2,r11 */
            r[11]=r[0]*r[2]+r[11];r[15]=0x76e4u;return 0;
        case 0x76e4u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x76e8u;return 0;
        case 0x76e8u: /* strb r1,[r11,#0x3] */
            STORE(r[11]+3u,r[1],1);r[15]=0x76ecu;return 0;
        case 0x76ecu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7814u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7818u;return 0;
        case 0x7818u: /* ldr r12,[0x780c] */
            LOAD(r[12],30732u,4);r[15]=0x781cu;return 0;
        case 0x781cu: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x7820u;return 0;
        case 0x7820u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x7824u;return 0;
        case 0x7824u: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x7828u;return 0;
        case 0x7828u: /* mov r0,#0x1 */
            r[0]=1u;r[15]=0x782cu;return 0;
        case 0x782cu: /* ldr r4,[r3,#0xc] */
            LOAD(r[4],r[3]+12u,4);r[15]=0x7830u;return 0;
        case 0x7830u: /* cmp r1,r4 */
            (void)subtract(state,r[1],r[4]);r[15]=0x7834u;return 0;
        case 0x7834u: /* movhi r4,#0x4 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[4]=4u;}r[15]=0x7838u;return 0;
        case 0x7838u: /* bhi 0x00008140 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=33088u;}else{r[15]=0x783cu;}return 0;
        case 0x783cu: /* mov r5,r2, lsr #0x3 */
            r[5]=(r[2]>>3);r[15]=0x7840u;return 0;
        case 0x7840u: /* bl 0x00007ae4 */
            r[14]=0x7844u;r[15]=31460u;return 0;
        case 0x7844u: /* add r2,r3,#0x10 */
            r[2]=r[3]+16u;r[15]=0x7848u;return 0;
        case 0x7848u: /* add r2,r2,r1, lsl #0x2 */
            r[2]=r[2]+(r[1]<<2);r[15]=0x784cu;return 0;
        case 0x784cu: /* ldr r9,[r2,#0x0] */
            LOAD(r[9],r[2]+0u,4);r[15]=0x7850u;return 0;
        case 0x7850u: /* add r7,r9,r3 */
            r[7]=r[9]+r[3];r[15]=0x7854u;return 0;
        case 0x7854u: /* add r7,r7,#0x4 */
            r[7]=r[7]+4u;r[15]=0x7858u;return 0;
        case 0x7858u: /* mov r9,r7 */
            r[9]=r[7];r[15]=0x785cu;return 0;
        case 0x785cu: /* ldr r2,[r7],#0x4 */
            LOAD(r[2],r[7],4);r[7]+=4u;r[15]=0x7860u;return 0;
        case 0x7860u: /* mov r2,r2, ror #0x10 */
            r[2]=((r[2]>>16)|(r[2]<<16));r[15]=0x7864u;return 0;
        case 0x7864u: /* add r2,r2,r3 */
            r[2]=r[2]+r[3];r[15]=0x7868u;return 0;
        case 0x7868u: /* ldr r3,[0x77f8] */
            LOAD(r[3],30712u,4);r[15]=0x786cu;return 0;
        case 0x786cu: /* and r3,r2,r3 */
            r[3]=r[2]&r[3];r[15]=0x7870u;return 0;
        case 0x7870u: /* str r3,[r12,#0x8] */
            STORE(r[12]+8u,r[3],4);r[15]=0x7874u;return 0;
        case 0x7874u: /* str r3,[r12,#0x30] */
            STORE(r[12]+48u,r[3],4);r[15]=0x7878u;return 0;
        case 0x7878u: /* mov r11,#0x0 */
            r[11]=0u;r[15]=0x787cu;return 0;
        case 0x787cu: /* str r11,[r12,#0x4] */
            STORE(r[12]+4u,r[11],4);r[15]=0x7880u;return 0;
        case 0x7880u: /* str r11,[r12,#0x2c] */
            STORE(r[12]+44u,r[11],4);r[15]=0x7884u;return 0;
        case 0x7884u: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x7888u;return 0;
        case 0x7888u: /* add r4,r11,#0x48 */
            r[4]=r[11]+72u;r[15]=0x788cu;return 0;
        case 0x788cu: /* mov r3,r2, lsr #0x10 */
            r[3]=(r[2]>>16);r[15]=0x7890u;return 0;
        case 0x7890u: /* str r3,[r11],#0x4 */
            STORE(r[11],r[3],4);r[11]+=4u;r[15]=0x7894u;return 0;
        case 0x7894u: /* ldr r5,[0x77f0] */
            LOAD(r[5],30704u,4);r[15]=0x7898u;return 0;
        case 0x7898u: /* and r2,r2,r5 */
            r[2]=r[2]&r[5];r[15]=0x789cu;return 0;
        case 0x789cu: /* str r2,[r11],#0x4 */
            STORE(r[11],r[2],4);r[11]+=4u;r[15]=0x78a0u;return 0;
        case 0x78a0u: /* ldr r0,[r7],#0x4 */
            LOAD(r[0],r[7],4);r[7]+=4u;r[15]=0x78a4u;return 0;
        case 0x78a4u: /* mov r1,r0, lsr #0x10 */
            r[1]=(r[0]>>16);r[15]=0x78a8u;return 0;
        case 0x78a8u: /* and r0,r0,r5 */
            r[0]=r[0]&r[5];r[15]=0x78acu;return 0;
        case 0x78acu: /* str r0,[r11],#0x4 */
            STORE(r[11],r[0],4);r[11]+=4u;r[15]=0x78b0u;return 0;
        case 0x78b0u: /* str r1,[r11],#0x4 */
            STORE(r[11],r[1],4);r[11]+=4u;r[15]=0x78b4u;return 0;
        case 0x78b4u: /* cmp r11,r4 */
            (void)subtract(state,r[11],r[4]);r[15]=0x78b8u;return 0;
        case 0x78b8u: /* bne 0x000078a0 */
            if(!(state->cpsr&0x40000000u)){r[15]=30880u;}else{r[15]=0x78bcu;}return 0;
        case 0x78bcu: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x78c0u;return 0;
        case 0x78c0u: /* ldrb r0,[r9,#0x13] */
            LOAD(r[0],r[9]+19u,1);r[15]=0x78c4u;return 0;
        case 0x78c4u: /* strb r0,[r12,#0x2a] */
            STORE(r[12]+42u,r[0],1);r[15]=0x78c8u;return 0;
        case 0x78c8u: /* ldrb r1,[r9,#0x10] */
            LOAD(r[1],r[9]+16u,1);r[15]=0x78ccu;return 0;
        case 0x78ccu: /* bic r0,r1,#0xf0 */
            r[0]=r[1]&~240u;r[15]=0x78d0u;return 0;
        case 0x78d0u: /* strb r0,[r12,#0x25] */
            STORE(r[12]+37u,r[0],1);r[15]=0x78d4u;return 0;
        case 0x78d4u: /* bic r1,r1,#0xf */
            r[1]=r[1]&~15u;r[15]=0x78d8u;return 0;
        case 0x78d8u: /* strb r1,[r12,#0x26] */
            STORE(r[12]+38u,r[1],1);r[15]=0x78dcu;return 0;
        case 0x78dcu: /* ldr r1,[r9,#0x0] */
            LOAD(r[1],r[9]+0u,4);r[15]=0x78e0u;return 0;
        case 0x78e0u: /* and r1,r1,#0x1800000 */
            r[1]=r[1]&25165824u;r[15]=0x78e4u;return 0;
        case 0x78e4u: /* mov r1,r1, lsr #0x17 */
            r[1]=(r[1]>>23);r[15]=0x78e8u;return 0;
        case 0x78e8u: /* strb r1,[r12,#0x27] */
            STORE(r[12]+39u,r[1],1);r[15]=0x78ecu;return 0;
        case 0x78ecu: /* ldrb r2,[r9,#0x12] */
            LOAD(r[2],r[9]+18u,1);r[15]=0x78f0u;return 0;
        case 0x78f0u: /* strb r2,[r12,#0x28] */
            STORE(r[12]+40u,r[2],1);r[15]=0x78f4u;return 0;
        case 0x78f4u: /* ldrb r3,[r9,#0x15] */
            LOAD(r[3],r[9]+21u,1);r[15]=0x78f8u;return 0;
        case 0x78f8u: /* strb r3,[r12,#0x24] */
            STORE(r[12]+36u,r[3],1);r[15]=0x78fcu;return 0;
        case 0x78fcu: /* ldr r4,[r11,#0x18] */
            LOAD(r[4],r[11]+24u,4);r[15]=0x7900u;return 0;
        case 0x7900u: /* str r4,[r12,#0xc] */
            STORE(r[12]+12u,r[4],4);r[15]=0x7904u;return 0;
        case 0x7904u: /* bl 0x00007c90 */
            r[14]=0x7908u;r[15]=31888u;return 0;
        case 0x7908u: /* ldrb r0,[r9,#0x24] */
            LOAD(r[0],r[9]+36u,1);r[15]=0x790cu;return 0;
        case 0x790cu: /* strb r0,[r12,#0x3c] */
            STORE(r[12]+60u,r[0],1);r[15]=0x7910u;return 0;
        case 0x7910u: /* ldrb r1,[r12,#0x23] */
            LOAD(r[1],r[12]+35u,1);r[15]=0x7914u;return 0;
        case 0x7914u: /* ldr r2,[r12,#0x34] */
            LOAD(r[2],r[12]+52u,4);r[15]=0x7918u;return 0;
        case 0x7918u: /* bl 0x00008208 */
            r[14]=0x791cu;r[15]=33288u;return 0;
        case 0x791cu: /* ldrb r1,[r12,#0x0] */
            LOAD(r[1],r[12]+0u,1);r[15]=0x7920u;return 0;
        case 0x7920u: /* bic r1,r1,#0x2 */
            r[1]=r[1]&~2u;r[15]=0x7924u;return 0;
        case 0x7924u: /* strb r1,[r12,#0x0] */
            STORE(r[12]+0u,r[1],1);r[15]=0x7928u;return 0;
        case 0x7928u: /* ldr r1,[r11,#0x1c] */
            LOAD(r[1],r[11]+28u,4);r[15]=0x792cu;return 0;
        case 0x792cu: /* bic r1,r1,#0x8000 */
            r[1]=r[1]&~32768u;r[15]=0x7930u;return 0;
        case 0x7930u: /* str r1,[r11,#0x1c] */
            STORE(r[11]+28u,r[1],4);r[15]=0x7934u;return 0;
        case 0x7934u: /* ldr r9,[0x824c] */
            LOAD(r[9],33356u,4);r[15]=0x7938u;return 0;
        case 0x7938u: /* ldr r0,[r9,#0x10] */
            LOAD(r[0],r[9]+16u,4);r[15]=0x793cu;return 0;
        case 0x793cu: /* ldr r0,[r9,#0x14] */
            LOAD(r[0],r[9]+20u,4);r[15]=0x7940u;return 0;
        case 0x7940u: /* b 0x00008154 */
            r[15]=33108u;return 0;
        case 0x7944u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7948u;return 0;
        case 0x7948u: /* ldr r12,[0x7800] */
            LOAD(r[12],30720u,4);r[15]=0x794cu;return 0;
        case 0x794cu: /* ldr r7,[0x7804] */
            LOAD(r[7],30724u,4);r[15]=0x7950u;return 0;
        case 0x7950u: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x7954u;return 0;
        case 0x7954u: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x7958u;return 0;
        case 0x7958u: /* mov r0,#0x2 */
            r[0]=2u;r[15]=0x795cu;return 0;
        case 0x795cu: /* str r2,[r12,#0x38] */
            STORE(r[12]+56u,r[2],4);r[15]=0x7960u;return 0;
        case 0x7960u: /* ands r2,r2,#0x10000 */
            temporary=r[2]&65536u;r[2]=temporary;logic_flags(state,temporary,1,65536u);r[15]=0x7964u;return 0;
        case 0x7964u: /* movne r2,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[2]=31u;}r[15]=0x7968u;return 0;
        case 0x7968u: /* mov r5,r5, lsr #0x3 */
            r[5]=(r[5]>>3);r[15]=0x796cu;return 0;
        case 0x796cu: /* bl 0x00007ae4 */
            r[14]=0x7970u;r[15]=31460u;return 0;
        case 0x7970u: /* strb r1,[r12,#0x27] */
            STORE(r[12]+39u,r[1],1);r[15]=0x7974u;return 0;
        case 0x7974u: /* strb r3,[r12,#0x24] */
            STORE(r[12]+36u,r[3],1);r[15]=0x7978u;return 0;
        case 0x7978u: /* str r4,[r12,#0xc] */
            STORE(r[12]+12u,r[4],4);r[15]=0x797cu;return 0;
        case 0x797cu: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7980u;return 0;
        case 0x7980u: /* strb r6,[r12,#0x26] */
            STORE(r[12]+38u,r[6],1);r[15]=0x7984u;return 0;
        case 0x7984u: /* ldr r6,[0x7808] */
            LOAD(r[6],30728u,4);r[15]=0x7988u;return 0;
        case 0x7988u: /* cmp r0,#0x2 */
            (void)subtract(state,r[0],2u);r[15]=0x798cu;return 0;
        case 0x798cu: /* ldrbne r7,[r12,#-0x1f] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[7],r[12]-31u,1);}r[15]=0x7990u;return 0;
        case 0x7990u: /* ldrbeq r7,[r12,#0x38] */
            if(state->cpsr&0x40000000u){LOAD(r[7],r[12]+56u,1);}r[15]=0x7994u;return 0;
        case 0x7994u: /* add r6,r6,r7, lsl #0x3 */
            r[6]=r[6]+(r[7]<<3);r[15]=0x7998u;return 0;
        case 0x7998u: /* ldr r7,[r6],#0x4 */
            LOAD(r[7],r[6],4);r[6]+=4u;r[15]=0x799cu;return 0;
        case 0x799cu: /* cmp r7,#0x0 */
            (void)subtract(state,r[7],0u);r[15]=0x79a0u;return 0;
        case 0x79a0u: /* beq 0x00007ac4 */
            if(state->cpsr&0x40000000u){r[15]=31428u;}else{r[15]=0x79a4u;}return 0;
        case 0x79a4u: /* str r7,[r12,#0x30] */
            STORE(r[12]+48u,r[7],4);r[15]=0x79a8u;return 0;
        case 0x79a8u: /* ldr r7,[r6,#0x0] */
            LOAD(r[7],r[6]+0u,4);r[15]=0x79acu;return 0;
        case 0x79acu: /* str r7,[r12,#0x34] */
            STORE(r[12]+52u,r[7],4);r[15]=0x79b0u;return 0;
        case 0x79b0u: /* cmp r1,#0x1 */
            (void)subtract(state,r[1],1u);r[15]=0x79b4u;return 0;
        case 0x79b4u: /* bgt 0x000079c8 */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[15]=31176u;}else{r[15]=0x79b8u;}return 0;
        case 0x79b8u: /* blt 0x000079dc */
            if((((state->cpsr>>31)^(state->cpsr>>28))&1u)!=0){r[15]=31196u;}else{r[15]=0x79bcu;}return 0;
        case 0x79bcu: /* cmp r7,#0xf000 */
            (void)subtract(state,r[7],61440u);r[15]=0x79c0u;return 0;
        case 0x79c0u: /* movgt r7,#0xf000 */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[7]=61440u;r[15]=0x79c4u;}else{r[15]=0x79c4u;}return 0;
        case 0x79c4u: /* b 0x000079e8 */
            r[15]=31208u;return 0;
        case 0x79c8u: /* cmp r7,#0x7000 */
            (void)subtract(state,r[7],28672u);r[15]=0x79ccu;return 0;
        case 0x79ccu: /* movgt r7,#0x7000 */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[7]=28672u;r[15]=0x79d0u;}else{r[15]=0x79d0u;}return 0;
        case 0x79d0u: /* mov r7,r7, lsl #0x1 */
            r[7]=(r[7]<<1);r[15]=0x79d4u;return 0;
        case 0x79d4u: /* mov r1,#0x3 */
            r[1]=3u;r[15]=0x79d8u;return 0;
        case 0x79d8u: /* b 0x000079e8 */
            r[15]=31208u;return 0;
        case 0x79dcu: /* cmp r7,#0x1f000 */
            (void)subtract(state,r[7],126976u);r[15]=0x79e0u;return 0;
        case 0x79e0u: /* movgt r7,#0x1f000 */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[7]=126976u;r[15]=0x79e4u;}else{r[15]=0x79e4u;}return 0;
        case 0x79e4u: /* mov r7,r7, lsr #0x1 */
            r[7]=(r[7]>>1);r[15]=0x79e8u;return 0;
        case 0x79e8u: /* str r7,[r11,#0xc] */
            STORE(r[11]+12u,r[7],4);r[15]=0x79ecu;return 0;
        case 0x79ecu: /* ldr r7,[r12,#0x30] */
            LOAD(r[7],r[12]+48u,4);r[15]=0x79f0u;return 0;
        case 0x79f0u: /* str r7,[r12,#0x8] */
            STORE(r[12]+8u,r[7],4);r[15]=0x79f4u;return 0;
        case 0x79f4u: /* mov r6,r7, lsr #0x10 */
            r[6]=(r[7]>>16);r[15]=0x79f8u;return 0;
        case 0x79f8u: /* mov r8,#0x4200 */
            r[8]=16896u;r[15]=0x79fcu;return 0;
        case 0x79fcu: /* orr r8,r6,r8 */
            r[8]=r[6]|r[8];r[15]=0x7a00u;return 0;
        case 0x7a00u: /* orr r8,r8,r1, lsl #0x7 */
            r[8]=r[8]|(r[1]<<7);r[15]=0x7a04u;return 0;
        case 0x7a04u: /* str r8,[r11,#0x0] */
            STORE(r[11]+0u,r[8],4);r[15]=0x7a08u;return 0;
        case 0x7a08u: /* mov r8,#0x1f00 */
            r[8]=7936u;r[15]=0x7a0cu;return 0;
        case 0x7a0cu: /* orr r8,r8,#0xf8 */
            r[8]=r[8]|248u;r[15]=0x7a10u;return 0;
        case 0x7a10u: /* str r8,[r11,#0x2c] */
            STORE(r[11]+44u,r[8],4);r[15]=0x7a14u;return 0;
        case 0x7a14u: /* str r8,[r11,#0x30] */
            STORE(r[11]+48u,r[8],4);r[15]=0x7a18u;return 0;
        case 0x7a18u: /* str r8,[r11,#0x34] */
            STORE(r[11]+52u,r[8],4);r[15]=0x7a1cu;return 0;
        case 0x7a1cu: /* str r8,[r11,#0x38] */
            STORE(r[11]+56u,r[8],4);r[15]=0x7a20u;return 0;
        case 0x7a20u: /* str r8,[r11,#0x3c] */
            STORE(r[11]+60u,r[8],4);r[15]=0x7a24u;return 0;
        case 0x7a24u: /* ldr r8,[0x77f0] */
            LOAD(r[8],30704u,4);r[15]=0x7a28u;return 0;
        case 0x7a28u: /* and r7,r7,r8 */
            r[7]=r[7]&r[8];r[15]=0x7a2cu;return 0;
        case 0x7a2cu: /* str r7,[r11,#0x4] */
            STORE(r[11]+4u,r[7],4);r[15]=0x7a30u;return 0;
        case 0x7a30u: /* mov r6,#0x1f */
            r[6]=31u;r[15]=0x7a34u;return 0;
        case 0x7a34u: /* mov r7,#0x8000 */
            r[7]=32768u;r[15]=0x7a38u;return 0;
        case 0x7a38u: /* mov r8,#0x24 */
            r[8]=36u;r[15]=0x7a3cu;return 0;
        case 0x7a3cu: /* strb r8,[r11,#0x28] */
            STORE(r[11]+40u,r[8],1);r[15]=0x7a40u;return 0;
        case 0x7a40u: /* mov r8,#0x0 */
            r[8]=0u;r[15]=0x7a44u;return 0;
        case 0x7a44u: /* str r8,[r11,#0x8] */
            STORE(r[11]+8u,r[8],4);r[15]=0x7a48u;return 0;
        case 0x7a48u: /* str r6,[r11,#0x10] */
            STORE(r[11]+16u,r[6],4);r[15]=0x7a4cu;return 0;
        case 0x7a4cu: /* str r7,[r11,#0x1c] */
            STORE(r[11]+28u,r[7],4);r[15]=0x7a50u;return 0;
        case 0x7a50u: /* str r6,[r11,#0x40] */
            STORE(r[11]+64u,r[6],4);r[15]=0x7a54u;return 0;
        case 0x7a54u: /* str r6,[r11,#0x44] */
            STORE(r[11]+68u,r[6],4);r[15]=0x7a58u;return 0;
        case 0x7a58u: /* str r8,[r12,#0x4] */
            STORE(r[12]+4u,r[8],4);r[15]=0x7a5cu;return 0;
        case 0x7a5cu: /* str r8,[r12,#0x2c] */
            STORE(r[12]+44u,r[8],4);r[15]=0x7a60u;return 0;
        case 0x7a60u: /* strb r2,[r12,#0x28] */
            STORE(r[12]+40u,r[2],1);r[15]=0x7a64u;return 0;
        case 0x7a64u: /* sub r6,r6,#0xa */
            r[6]=r[6]-10u;r[15]=0x7a68u;return 0;
        case 0x7a68u: /* str r6,[r11,#0x14] */
            STORE(r[11]+20u,r[6],4);r[15]=0x7a6cu;return 0;
        case 0x7a6cu: /* bl 0x00007c90 */
            r[14]=0x7a70u;r[15]=31888u;return 0;
        case 0x7a70u: /* ldr r0,[r9,#0x10] */
            LOAD(r[0],r[9]+16u,4);r[15]=0x7a74u;return 0;
        case 0x7a74u: /* ldr r0,[r9,#0x14] */
            LOAD(r[0],r[9]+20u,4);r[15]=0x7a78u;return 0;
        case 0x7a78u: /* ldrb r2,[r12,#0x28] */
            LOAD(r[2],r[12]+40u,1);r[15]=0x7a7cu;return 0;
        case 0x7a7cu: /* ldrb r3,[r12,#0x24] */
            LOAD(r[3],r[12]+36u,1);r[15]=0x7a80u;return 0;
        case 0x7a80u: /* cmp r2,#0xf */
            (void)subtract(state,r[2],15u);r[15]=0x7a84u;return 0;
        case 0x7a84u: /* beq 0x00008154 */
            if(state->cpsr&0x40000000u){r[15]=33108u;}else{r[15]=0x7a88u;}return 0;
        case 0x7a88u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x7a8cu;return 0;
        case 0x7a8cu: /* mov r2,#0xf */
            r[2]=15u;r[15]=0x7a90u;return 0;
        case 0x7a90u: /* movhi r0,#0x3 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[0]=3u;}r[15]=0x7a94u;return 0;
        case 0x7a94u: /* bhi 0x0000796c */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=31084u;}else{r[15]=0x7a98u;}return 0;
        case 0x7a98u: /* ldrb r6,[r12,#0x0] */
            LOAD(r[6],r[12]+0u,1);r[15]=0x7a9cu;return 0;
        case 0x7a9cu: /* tst r6,#0x1 */
            temporary=r[6]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7aa0u;return 0;
        case 0x7aa0u: /* beq 0x00008154 */
            if(state->cpsr&0x40000000u){r[15]=33108u;}else{r[15]=0x7aa4u;}return 0;
        case 0x7aa4u: /* bl 0x000081bc */
            r[14]=0x7aa8u;r[15]=33212u;return 0;
        case 0x7aa8u: /* ldrb r3,[r12,#0x29] */
            LOAD(r[3],r[12]+41u,1);r[15]=0x7aacu;return 0;
        case 0x7aacu: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7ab0u;return 0;
        case 0x7ab0u: /* bl 0x0000818c */
            r[14]=0x7ab4u;r[15]=33164u;return 0;
        case 0x7ab4u: /* strb r2,[r12,#0x28] */
            STORE(r[12]+40u,r[2],1);r[15]=0x7ab8u;return 0;
        case 0x7ab8u: /* strb r3,[r12,#0x29] */
            STORE(r[12]+41u,r[3],1);r[15]=0x7abcu;return 0;
        case 0x7abcu: /* bl 0x00008168 */
            r[14]=0x7ac0u;r[15]=33128u;return 0;
        case 0x7ac0u: /* b 0x00008154 */
            r[15]=33108u;return 0;
        case 0x7ac4u: /* ldrb r0,[r12,#0x0] */
            LOAD(r[0],r[12]+0u,1);r[15]=0x7ac8u;return 0;
        case 0x7ac8u: /* bic r0,r0,#0x1 */
            r[0]=r[0]&~1u;r[15]=0x7accu;return 0;
        case 0x7accu: /* strb r0,[r12,#0x0] */
            STORE(r[12]+0u,r[0],1);r[15]=0x7ad0u;return 0;
        case 0x7ad0u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x7ad4u;return 0;
        case 0x7ad4u: /* strb r0,[r10,#0x1] */
            STORE(r[10]+1u,r[0],1);r[15]=0x7ad8u;return 0;
        case 0x7ad8u: /* str r0,[r10,#0x4] */
            STORE(r[10]+4u,r[0],4);r[15]=0x7adcu;return 0;
        case 0x7adcu: /* mov r4,#0x1 */
            r[4]=1u;r[15]=0x7ae0u;return 0;
        case 0x7ae0u: /* b 0x00008140 */
            r[15]=33088u;return 0;
        case 0x7ae4u: /* ldrb r6,[r12,#0x0] */
            LOAD(r[6],r[12]+0u,1);r[15]=0x7ae8u;return 0;
        case 0x7ae8u: /* tst r6,#0x1 */
            temporary=r[6]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7aecu;return 0;
        case 0x7aecu: /* beq 0x00007b14 */
            if(state->cpsr&0x40000000u){r[15]=31508u;}else{r[15]=0x7af0u;}return 0;
        case 0x7af0u: /* ldrb r7,[r12,#0x2] */
            LOAD(r[7],r[12]+2u,1);r[15]=0x7af4u;return 0;
        case 0x7af4u: /* cmp r7,#0x0 */
            (void)subtract(state,r[7],0u);r[15]=0x7af8u;return 0;
        case 0x7af8u: /* beq 0x00007bd0 */
            if(state->cpsr&0x40000000u){r[15]=31696u;}else{r[15]=0x7afcu;}return 0;
        case 0x7afcu: /* cmp r5,#0x0 */
            (void)subtract(state,r[5],0u);r[15]=0x7b00u;return 0;
        case 0x7b00u: /* beq 0x00007bd0 */
            if(state->cpsr&0x40000000u){r[15]=31696u;}else{r[15]=0x7b04u;}return 0;
        case 0x7b04u: /* cmp r5,r7 */
            (void)subtract(state,r[5],r[7]);r[15]=0x7b08u;return 0;
        case 0x7b08u: /* bcs 0x00007bd0 */
            if(state->cpsr&0x20000000u){r[15]=31696u;}else{r[15]=0x7b0cu;}return 0;
        case 0x7b0cu: /* mov r4,#0x2 */
            r[4]=2u;r[15]=0x7b10u;return 0;
        case 0x7b10u: /* b 0x00008140 */
            r[15]=33088u;return 0;
        case 0x7b14u: /* ldr r10,[0x77ec] */
            LOAD(r[10],30700u,4);r[15]=0x7b18u;return 0;
        case 0x7b18u: /* mov r9,#0x0 */
            r[9]=0u;r[15]=0x7b1cu;return 0;
        case 0x7b1cu: /* ldr r6,[r10,#0x4] */
            LOAD(r[6],r[10]+4u,4);r[15]=0x7b20u;return 0;
        case 0x7b20u: /* cmp r6,#0x0 */
            (void)subtract(state,r[6],0u);r[15]=0x7b24u;return 0;
        case 0x7b24u: /* ldreq r11,[r10,#0x8] */
            if(state->cpsr&0x40000000u){LOAD(r[11],r[10]+8u,4);}r[15]=0x7b28u;return 0;
        case 0x7b28u: /* beq 0x00007c20 */
            if(state->cpsr&0x40000000u){r[15]=31776u;}else{r[15]=0x7b2cu;}return 0;
        case 0x7b2cu: /* add r10,r10,#0xc */
            r[10]=r[10]+12u;r[15]=0x7b30u;return 0;
        case 0x7b30u: /* add r9,r9,#0x1 */
            r[9]=r[9]+1u;r[15]=0x7b34u;return 0;
        case 0x7b34u: /* cmp r9,#0x10 */
            (void)subtract(state,r[9],16u);r[15]=0x7b38u;return 0;
        case 0x7b38u: /* bne 0x00007b1c */
            if(!(state->cpsr&0x40000000u)){r[15]=31516u;}else{r[15]=0x7b3cu;}return 0;
        case 0x7b3cu: /* mov r6,#0xff */
            r[6]=255u;r[15]=0x7b40u;return 0;
        case 0x7b40u: /* mov r8,r10 */
            r[8]=r[10];r[15]=0x7b44u;return 0;
        case 0x7b44u: /* subs r9,r9,#0x1 */
            r[9]=subtract(state,r[9],1u);r[15]=0x7b48u;return 0;
        case 0x7b48u: /* bmi 0x00007b88 */
            if(state->cpsr&0x80000000u){r[15]=31624u;}else{r[15]=0x7b4cu;}return 0;
        case 0x7b4cu: /* sub r10,r10,#0xc */
            r[10]=r[10]-12u;r[15]=0x7b50u;return 0;
        case 0x7b50u: /* ldrb r7,[r10,#0x0] */
            LOAD(r[7],r[10]+0u,1);r[15]=0x7b54u;return 0;
        case 0x7b54u: /* cmp r7,#0x0 */
            (void)subtract(state,r[7],0u);r[15]=0x7b58u;return 0;
        case 0x7b58u: /* beq 0x00007be0 */
            if(state->cpsr&0x40000000u){r[15]=31712u;}else{r[15]=0x7b5cu;}return 0;
        case 0x7b5cu: /* cmp r6,r7 */
            (void)subtract(state,r[6],r[7]);r[15]=0x7b60u;return 0;
        case 0x7b60u: /* bhi 0x00007b7c */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=31612u;}else{r[15]=0x7b64u;}return 0;
        case 0x7b64u: /* bcc 0x00007b44 */
            if(!(state->cpsr&0x20000000u)){r[15]=31556u;}else{r[15]=0x7b68u;}return 0;
        case 0x7b68u: /* ldr r11,[r8,#0x0] */
            LOAD(r[11],r[8]+0u,4);r[15]=0x7b6cu;return 0;
        case 0x7b6cu: /* ldr r7,[r10,#0x0] */
            LOAD(r[7],r[10]+0u,4);r[15]=0x7b70u;return 0;
        case 0x7b70u: /* cmp r7,r11 */
            (void)subtract(state,r[7],r[11]);r[15]=0x7b74u;return 0;
        case 0x7b74u: /* bhi 0x00007b44 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=31556u;}else{r[15]=0x7b78u;}return 0;
        case 0x7b78u: /* ldrb r7,[r10,#0x0] */
            LOAD(r[7],r[10]+0u,1);r[15]=0x7b7cu;return 0;
        case 0x7b7cu: /* mov r6,r7 */
            r[6]=r[7];r[15]=0x7b80u;return 0;
        case 0x7b80u: /* mov r8,r10 */
            r[8]=r[10];r[15]=0x7b84u;return 0;
        case 0x7b84u: /* b 0x00007b44 */
            r[15]=31556u;return 0;
        case 0x7b88u: /* cmp r5,#0x0 */
            (void)subtract(state,r[5],0u);r[15]=0x7b8cu;return 0;
        case 0x7b8cu: /* beq 0x00007bc8 */
            if(state->cpsr&0x40000000u){r[15]=31688u;}else{r[15]=0x7b90u;}return 0;
        case 0x7b90u: /* cmp r5,r6 */
            (void)subtract(state,r[5],r[6]);r[15]=0x7b94u;return 0;
        case 0x7b94u: /* bcc 0x00007bac */
            if(!(state->cpsr&0x20000000u)){r[15]=31660u;}else{r[15]=0x7b98u;}return 0;
        case 0x7b98u: /* bhi 0x00007bc8 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=31688u;}else{r[15]=0x7b9cu;}return 0;
        case 0x7b9cu: /* ldr r6,[r8,#0x0] */
            LOAD(r[6],r[8]+0u,4);r[15]=0x7ba0u;return 0;
        case 0x7ba0u: /* add r7,r5,r0 */
            r[7]=r[5]+r[0];r[15]=0x7ba4u;return 0;
        case 0x7ba4u: /* cmp r7,r6 */
            (void)subtract(state,r[7],r[6]);r[15]=0x7ba8u;return 0;
        case 0x7ba8u: /* bcs 0x00007bc8 */
            if(state->cpsr&0x20000000u){r[15]=31688u;}else{r[15]=0x7bacu;}return 0;
        case 0x7bacu: /* cmp r0,#0x3 */
            (void)subtract(state,r[0],3u);r[15]=0x7bb0u;return 0;
        case 0x7bb0u: /* bne 0x00007bc0 */
            if(!(state->cpsr&0x40000000u)){r[15]=31680u;}else{r[15]=0x7bb4u;}return 0;
        case 0x7bb4u: /* sub r12,r12,#0x58 */
            r[12]=r[12]-88u;r[15]=0x7bb8u;return 0;
        case 0x7bb8u: /* bl 0x000081bc */
            r[14]=0x7bbcu;r[15]=33212u;return 0;
        case 0x7bbcu: /* bl 0x00007e20 */
            r[14]=0x7bc0u;r[15]=32288u;return 0;
        case 0x7bc0u: /* mov r4,#0x80 */
            r[4]=128u;r[15]=0x7bc4u;return 0;
        case 0x7bc4u: /* b 0x00008140 */
            r[15]=33088u;return 0;
        case 0x7bc8u: /* mov r10,r8 */
            r[10]=r[8];r[15]=0x7bccu;return 0;
        case 0x7bccu: /* b 0x00007be0 */
            r[15]=31712u;return 0;
        case 0x7bd0u: /* ldrb r9,[r12,#0x3] */
            LOAD(r[9],r[12]+3u,1);r[15]=0x7bd4u;return 0;
        case 0x7bd4u: /* ldr r10,[0x77ec] */
            LOAD(r[10],30700u,4);r[15]=0x7bd8u;return 0;
        case 0x7bd8u: /* mov r6,#0xc */
            r[6]=12u;r[15]=0x7bdcu;return 0;
        case 0x7bdcu: /* mla r10,r9,r6,r10 */
            r[10]=r[9]*r[6]+r[10];r[15]=0x7be0u;return 0;
        case 0x7be0u: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x7be4u;return 0;
        case 0x7be4u: /* stmdb sp!,{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr} */
            STORE(r[13]-56u,r[0],4);STORE(r[13]-52u,r[1],4);STORE(r[13]-48u,r[2],4);STORE(r[13]-44u,r[3],4);STORE(r[13]-40u,r[4],4);STORE(r[13]-36u,r[5],4);STORE(r[13]-32u,r[6],4);STORE(r[13]-28u,r[7],4);STORE(r[13]-24u,r[8],4);STORE(r[13]-20u,r[9],4);STORE(r[13]-16u,r[10],4);STORE(r[13]-12u,r[11],4);STORE(r[13]-8u,r[12],4);STORE(r[13]-4u,r[14],4);r[13]-=56u;r[15]=0x7be8u;return 0;
        case 0x7be8u: /* ldr r12,[r10,#0x4] */
            LOAD(r[12],r[10]+4u,4);r[15]=0x7becu;return 0;
        case 0x7becu: /* ldrb r9,[r12,#0x3] */
            LOAD(r[9],r[12]+3u,1);r[15]=0x7bf0u;return 0;
        case 0x7bf0u: /* ldrb r4,[r10,#0x1] */
            LOAD(r[4],r[10]+1u,1);r[15]=0x7bf4u;return 0;
        case 0x7bf4u: /* cmp r4,#0x1 */
            (void)subtract(state,r[4],1u);r[15]=0x7bf8u;return 0;
        case 0x7bf8u: /* beq 0x00007c04 */
            if(state->cpsr&0x40000000u){r[15]=31748u;}else{r[15]=0x7bfcu;}return 0;
        case 0x7bfcu: /* bl 0x00007e20 */
            r[14]=0x7c00u;r[15]=32288u;return 0;
        case 0x7c00u: /* b 0x00007c0c */
            r[15]=31756u;return 0;
        case 0x7c04u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7c08u;return 0;
        case 0x7c08u: /* bl 0x00007ecc */
            r[14]=0x7c0cu;r[15]=32460u;return 0;
        case 0x7c0cu: /* ldr r1,[r11,#0x0] */
            LOAD(r[1],r[11]+0u,4);r[15]=0x7c10u;return 0;
        case 0x7c10u: /* bic r1,r1,#0x4000 */
            r[1]=r[1]&~16384u;r[15]=0x7c14u;return 0;
        case 0x7c14u: /* orr r1,r1,#0x8000 */
            r[1]=r[1]|32768u;r[15]=0x7c18u;return 0;
        case 0x7c18u: /* str r1,[r11,#0x0] */
            STORE(r[11]+0u,r[1],4);r[15]=0x7c1cu;return 0;
        case 0x7c1cu: /* ldmia sp!,{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr} */
            LOAD(r[0],r[13]+0u,4);LOAD(r[1],r[13]+4u,4);LOAD(r[2],r[13]+8u,4);LOAD(r[3],r[13]+12u,4);LOAD(r[4],r[13]+16u,4);LOAD(r[5],r[13]+20u,4);LOAD(r[6],r[13]+24u,4);LOAD(r[7],r[13]+28u,4);LOAD(r[8],r[13]+32u,4);LOAD(r[9],r[13]+36u,4);LOAD(r[10],r[13]+40u,4);LOAD(r[11],r[13]+44u,4);LOAD(r[12],r[13]+48u,4);LOAD(r[14],r[13]+52u,4);r[13]+=56u;r[15]=0x7c20u;return 0;
        case 0x7c20u: /* strb r5,[r12,#0x2] */
            STORE(r[12]+2u,r[5],1);r[15]=0x7c24u;return 0;
        case 0x7c24u: /* strb r9,[r12,#0x3] */
            STORE(r[12]+3u,r[9],1);r[15]=0x7c28u;return 0;
        case 0x7c28u: /* strb r5,[r10,#0x0] */
            STORE(r[10]+0u,r[5],1);r[15]=0x7c2cu;return 0;
        case 0x7c2cu: /* strb r0,[r10,#0x1] */
            STORE(r[10]+1u,r[0],1);r[15]=0x7c30u;return 0;
        case 0x7c30u: /* str r12,[r10,#0x4] */
            STORE(r[10]+4u,r[12],4);r[15]=0x7c34u;return 0;
        case 0x7c34u: /* ldrb r6,[r12,#0x0] */
            LOAD(r[6],r[12]+0u,1);r[15]=0x7c38u;return 0;
        case 0x7c38u: /* orr r6,r6,#0x1 */
            r[6]=r[6]|1u;r[15]=0x7c3cu;return 0;
        case 0x7c3cu: /* strb r6,[r12,#0x0] */
            STORE(r[12]+0u,r[6],1);r[15]=0x7c40u;return 0;
        case 0x7c40u: /* add r6,r9,#0x30 */
            r[6]=r[9]+48u;r[15]=0x7c44u;return 0;
        case 0x7c44u: /* ldr r9,[0x824c] */
            LOAD(r[9],33356u,4);r[15]=0x7c48u;return 0;
        case 0x7c48u: /* strb r6,[r9,#0xd] */
            STORE(r[9]+13u,r[6],1);r[15]=0x7c4cu;return 0;
        case 0x7c4cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7c90u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7c94u;return 0;
        case 0x7c94u: /* str r6,[r12,#0x2c] */
            STORE(r[12]+44u,r[6],4);r[15]=0x7c98u;return 0;
        case 0x7c98u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7c9cu;return 0;
        case 0x7c9cu: /* ldrb r6,[r12,#0x1e] */
            LOAD(r[6],r[12]+30u,1);r[15]=0x7ca0u;return 0;
        case 0x7ca0u: /* mov r6,r6, lsl #0x1 */
            r[6]=(r[6]<<1);r[15]=0x7ca4u;return 0;
        case 0x7ca4u: /* rsb r6,r6,#0x100 */
            r[6]=256u-r[6];r[15]=0x7ca8u;return 0;
        case 0x7ca8u: /* add r3,r3,r6 */
            r[3]=r[3]+r[6];r[15]=0x7cacu;return 0;
        case 0x7cacu: /* cmp r3,#0x0 */
            (void)subtract(state,r[3],0u);r[15]=0x7cb0u;return 0;
        case 0x7cb0u: /* movmi r3,#0x0 */
            if(state->cpsr&0x80000000u){r[3]=0u;}r[15]=0x7cb4u;return 0;
        case 0x7cb4u: /* cmp r3,#0xff */
            (void)subtract(state,r[3],255u);r[15]=0x7cb8u;return 0;
        case 0x7cb8u: /* movgt r3,#0xff */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[3]=255u;r[15]=0x7cbcu;}else{r[15]=0x7cbcu;}return 0;
        case 0x7cbcu: /* strb r3,[r11,#0x29] */
            STORE(r[11]+41u,r[3],1);r[15]=0x7cc0u;return 0;
        case 0x7cc0u: /* strb r3,[r12,#0x20] */
            STORE(r[12]+32u,r[3],1);r[15]=0x7cc4u;return 0;
        case 0x7cc4u: /* ldrb r6,[r12,#0x14] */
            LOAD(r[6],r[12]+20u,1);r[15]=0x7cc8u;return 0;
        case 0x7cc8u: /* cmp r6,#0x0 */
            (void)subtract(state,r[6],0u);r[15]=0x7cccu;return 0;
        case 0x7cccu: /* bne 0x00007cdc */
            if(!(state->cpsr&0x40000000u)){r[15]=31964u;}else{r[15]=0x7cd0u;}return 0;
        case 0x7cd0u: /* ldrb r6,[r12,#0x0] */
            LOAD(r[6],r[12]+0u,1);r[15]=0x7cd4u;return 0;
        case 0x7cd4u: /* bic r6,r6,#0x4 */
            r[6]=r[6]&~4u;r[15]=0x7cd8u;return 0;
        case 0x7cd8u: /* strb r6,[r12,#0x0] */
            STORE(r[12]+0u,r[6],1);r[15]=0x7cdcu;return 0;
        case 0x7cdcu: /* ldrb r6,[r12,#0x1f] */
            LOAD(r[6],r[12]+31u,1);r[15]=0x7ce0u;return 0;
        case 0x7ce0u: /* mov r6,r6, lsr #0x2 */
            r[6]=(r[6]>>2);r[15]=0x7ce4u;return 0;
        case 0x7ce4u: /* sub r6,r6,#0x20 */
            r[6]=r[6]-32u;r[15]=0x7ce8u;return 0;
        case 0x7ce8u: /* ldr r7,[0x8250] */
            LOAD(r[7],33360u,4);r[15]=0x7cecu;return 0;
        case 0x7cecu: /* ldrb r2,[r7,r2] */
            LOAD(r[2],r[7]+r[2],1);r[15]=0x7cf0u;return 0;
        case 0x7cf0u: /* add r2,r2,r6 */
            r[2]=r[2]+r[6];r[15]=0x7cf4u;return 0;
        case 0x7cf4u: /* cmp r2,#0x0 */
            (void)subtract(state,r[2],0u);r[15]=0x7cf8u;return 0;
        case 0x7cf8u: /* movmi r2,#0x0 */
            if(state->cpsr&0x80000000u){r[2]=0u;}r[15]=0x7cfcu;return 0;
        case 0x7cfcu: /* cmp r2,#0x1f */
            (void)subtract(state,r[2],31u);r[15]=0x7d00u;return 0;
        case 0x7d00u: /* movgt r2,#0x1f */
            if(!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0){r[2]=31u;r[15]=0x7d04u;}else{r[15]=0x7d04u;}return 0;
        case 0x7d04u: /* ldr r7,[0x8254] */
            LOAD(r[7],33364u,4);r[15]=0x7d08u;return 0;
        case 0x7d08u: /* ldrb r2,[r7,r2] */
            LOAD(r[2],r[7]+r[2],1);r[15]=0x7d0cu;return 0;
        case 0x7d0cu: /* strb r2,[r11,#0x24] */
            STORE(r[11]+36u,r[2],1);r[15]=0x7d10u;return 0;
        case 0x7d10u: /* strb r2,[r12,#0x21] */
            STORE(r[12]+33u,r[2],1);r[15]=0x7d14u;return 0;
        case 0x7d14u: /* ldrb r6,[r12,#0x16] */
            LOAD(r[6],r[12]+22u,1);r[15]=0x7d18u;return 0;
        case 0x7d18u: /* cmp r6,#0x0 */
            (void)subtract(state,r[6],0u);r[15]=0x7d1cu;return 0;
        case 0x7d1cu: /* bne 0x00007d2c */
            if(!(state->cpsr&0x40000000u)){r[15]=32044u;}else{r[15]=0x7d20u;}return 0;
        case 0x7d20u: /* ldrb r6,[r12,#0x0] */
            LOAD(r[6],r[12]+0u,1);r[15]=0x7d24u;return 0;
        case 0x7d24u: /* bic r6,r6,#0x10 */
            r[6]=r[6]&~16u;r[15]=0x7d28u;return 0;
        case 0x7d28u: /* strb r6,[r12,#0x0] */
            STORE(r[12]+0u,r[6],1);r[15]=0x7d2cu;return 0;
        case 0x7d2cu: /* bl 0x00007d68 */
            r[14]=0x7d30u;r[15]=32104u;return 0;
        case 0x7d30u: /* ldrb r7,[r12,#0x1b] */
            LOAD(r[7],r[12]+27u,1);r[15]=0x7d34u;return 0;
        case 0x7d34u: /* ldrb r6,[r12,#0x2a] */
            LOAD(r[6],r[12]+42u,1);r[15]=0x7d38u;return 0;
        case 0x7d38u: /* bl 0x00007064 */
            r[14]=0x7d3cu;r[15]=28772u;return 0;
        case 0x7d3cu: /* strb r7,[r12,#0x2b] */
            STORE(r[12]+43u,r[7],1);r[15]=0x7d40u;return 0;
        case 0x7d40u: /* strb r7,[r11,#0x25] */
            STORE(r[11]+37u,r[7],1);r[15]=0x7d44u;return 0;
        case 0x7d44u: /* ldrb r6,[r12,#0x29] */
            LOAD(r[6],r[12]+41u,1);r[15]=0x7d48u;return 0;
        case 0x7d48u: /* tst r6,#0x40 */
            temporary=r[6]&64u;logic_flags(state,temporary,0,64u);r[15]=0x7d4cu;return 0;
        case 0x7d4cu: /* ldrbeq r6,[r12,#0x25] */
            if(state->cpsr&0x40000000u){LOAD(r[6],r[12]+37u,1);}r[15]=0x7d50u;return 0;
        case 0x7d50u: /* strb r6,[r11,#0x20] */
            STORE(r[11]+32u,r[6],1);r[15]=0x7d54u;return 0;
        case 0x7d54u: /* mov r8,#0x1 */
            r[8]=1u;r[15]=0x7d58u;return 0;
        case 0x7d58u: /* ldrbeq r6,[r12,#0x26] */
            if(state->cpsr&0x40000000u){LOAD(r[6],r[12]+38u,1);}r[15]=0x7d5cu;return 0;
        case 0x7d5cu: /* ldrbne r6,[r12,#0x22] */
            if(!(state->cpsr&0x40000000u)){LOAD(r[6],r[12]+34u,1);}r[15]=0x7d60u;return 0;
        case 0x7d60u: /* ldrb r7,[r12,#0x1a] */
            LOAD(r[7],r[12]+26u,1);r[15]=0x7d64u;return 0;
        case 0x7d64u: /* b 0x00006fc8 */
            r[15]=28616u;return 0;
        case 0x7dc4u: /* ldr r12,[0x780c] */
            LOAD(r[12],30732u,4);r[15]=0x7dc8u;return 0;
        case 0x7dc8u: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x7dccu;return 0;
        case 0x7dccu: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x7dd0u;return 0;
        case 0x7dd0u: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x7dd4u;return 0;
        case 0x7dd4u: /* ldrb r2,[r12,#0x0] */
            LOAD(r[2],r[12]+0u,1);r[15]=0x7dd8u;return 0;
        case 0x7dd8u: /* tst r2,#0x1 */
            temporary=r[2]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7ddcu;return 0;
        case 0x7ddcu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7de0u;}return 0;
        case 0x7de0u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7de4u;return 0;
        case 0x7de4u: /* bl 0x000081bc */
            r[14]=0x7de8u;r[15]=33212u;return 0;
        case 0x7de8u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7decu;return 0;
        case 0x7decu: /* bl 0x00007ecc */
            r[14]=0x7df0u;r[15]=32460u;return 0;
        case 0x7df0u: /* b 0x00008168 */
            r[15]=33128u;return 0;
        case 0x7df4u: /* ldr r12,[0x7800] */
            LOAD(r[12],30720u,4);r[15]=0x7df8u;return 0;
        case 0x7df8u: /* ldr r7,[0x7804] */
            LOAD(r[7],30724u,4);r[15]=0x7dfcu;return 0;
        case 0x7dfcu: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x7e00u;return 0;
        case 0x7e00u: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x7e04u;return 0;
        case 0x7e04u: /* ldrb r2,[r12,#0x0] */
            LOAD(r[2],r[12]+0u,1);r[15]=0x7e08u;return 0;
        case 0x7e08u: /* tst r2,#0x1 */
            temporary=r[2]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7e0cu;return 0;
        case 0x7e0cu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7e10u;}return 0;
        case 0x7e10u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7e14u;return 0;
        case 0x7e14u: /* bl 0x000081bc */
            r[14]=0x7e18u;r[15]=33212u;return 0;
        case 0x7e18u: /* bl 0x00007e20 */
            r[14]=0x7e1cu;r[15]=32288u;return 0;
        case 0x7e1cu: /* b 0x00008168 */
            r[15]=33128u;return 0;
        case 0x7e20u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7e24u;return 0;
        case 0x7e24u: /* mov r7,#0xf */
            r[7]=15u;r[15]=0x7e28u;return 0;
        case 0x7e28u: /* mov r3,#0x1f */
            r[3]=31u;r[15]=0x7e2cu;return 0;
        case 0x7e2cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7e30u;return 0;
        case 0x7e30u: /* ldrb r8,[r12,#0x29] */
            LOAD(r[8],r[12]+41u,1);r[15]=0x7e34u;return 0;
        case 0x7e34u: /* bl 0x0000818c */
            r[14]=0x7e38u;r[15]=33164u;return 0;
        case 0x7e38u: /* strb r8,[r12,#0x29] */
            STORE(r[12]+41u,r[8],1);r[15]=0x7e3cu;return 0;
        case 0x7e3cu: /* ldr r8,[r11,#0x0] */
            LOAD(r[8],r[11]+0u,4);r[15]=0x7e40u;return 0;
        case 0x7e40u: /* bic r8,r8,#0x4000 */
            r[8]=r[8]&~16384u;r[15]=0x7e44u;return 0;
        case 0x7e44u: /* str r8,[r11,#0x0] */
            STORE(r[11]+0u,r[8],4);r[15]=0x7e48u;return 0;
        case 0x7e48u: /* strb r6,[r12,#0x23] */
            STORE(r[12]+35u,r[6],1);r[15]=0x7e4cu;return 0;
        case 0x7e4cu: /* str r6,[r12,#0x30] */
            STORE(r[12]+48u,r[6],4);r[15]=0x7e50u;return 0;
        case 0x7e50u: /* str r6,[r12,#0x34] */
            STORE(r[12]+52u,r[6],4);r[15]=0x7e54u;return 0;
        case 0x7e54u: /* strb r3,[r12,#0x28] */
            STORE(r[12]+40u,r[3],1);r[15]=0x7e58u;return 0;
        case 0x7e58u: /* strb r7,[r12,#0x2a] */
            STORE(r[12]+42u,r[7],1);r[15]=0x7e5cu;return 0;
        case 0x7e5cu: /* ldrb r8,[r12,#0x0] */
            LOAD(r[8],r[12]+0u,1);r[15]=0x7e60u;return 0;
        case 0x7e60u: /* bic r8,r8,#0x1 */
            r[8]=r[8]&~1u;r[15]=0x7e64u;return 0;
        case 0x7e64u: /* strb r8,[r12,#0x0] */
            STORE(r[12]+0u,r[8],1);r[15]=0x7e68u;return 0;
        case 0x7e68u: /* cmp r3,#0xf */
            (void)subtract(state,r[3],15u);r[15]=0x7e6cu;return 0;
        case 0x7e6cu: /* beq 0x00007e8c */
            if(state->cpsr&0x40000000u){r[15]=32396u;}else{r[15]=0x7e70u;}return 0;
        case 0x7e70u: /* mov r3,#0xf */
            r[3]=15u;r[15]=0x7e74u;return 0;
        case 0x7e74u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x7e78u;return 0;
        case 0x7e78u: /* ldrb r4,[r12,#0x0] */
            LOAD(r[4],r[12]+0u,1);r[15]=0x7e7cu;return 0;
        case 0x7e7cu: /* tst r4,#0x1 */
            temporary=r[4]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7e80u;return 0;
        case 0x7e80u: /* beq 0x00007e8c */
            if(state->cpsr&0x40000000u){r[15]=32396u;}else{r[15]=0x7e84u;}return 0;
        case 0x7e84u: /* bl 0x000081bc */
            r[14]=0x7e88u;r[15]=33212u;return 0;
        case 0x7e88u: /* b 0x00007e30 */
            r[15]=32304u;return 0;
        case 0x7e8cu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x7e90u;return 0;
        case 0x7e90u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7e94u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7e98u;return 0;
        case 0x7e98u: /* ldr r10,[0x77ec] */
            LOAD(r[10],30700u,4);r[15]=0x7e9cu;return 0;
        case 0x7e9cu: /* mov r9,#0x10 */
            r[9]=16u;r[15]=0x7ea0u;return 0;
        case 0x7ea0u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7ea4u;return 0;
        case 0x7ea4u: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x7ea8u;return 0;
        case 0x7ea8u: /* ldrb r0,[r10,#0x1] */
            LOAD(r[0],r[10]+1u,1);r[15]=0x7eacu;return 0;
        case 0x7eacu: /* cmp r0,#0x1 */
            (void)subtract(state,r[0],1u);r[15]=0x7eb0u;return 0;
        case 0x7eb0u: /* bne 0x00007ebc */
            if(!(state->cpsr&0x40000000u)){r[15]=32444u;}else{r[15]=0x7eb4u;}return 0;
        case 0x7eb4u: /* ldr r12,[r10,#0x4] */
            LOAD(r[12],r[10]+4u,4);r[15]=0x7eb8u;return 0;
        case 0x7eb8u: /* bl 0x00007ecc */
            r[14]=0x7ebcu;r[15]=32460u;return 0;
        case 0x7ebcu: /* subs r9,r9,#0x1 */
            r[9]=subtract(state,r[9],1u);r[15]=0x7ec0u;return 0;
        case 0x7ec0u: /* beq 0x00008168 */
            if(state->cpsr&0x40000000u){r[15]=33128u;}else{r[15]=0x7ec4u;}return 0;
        case 0x7ec4u: /* add r10,r10,#0xc */
            r[10]=r[10]+12u;r[15]=0x7ec8u;return 0;
        case 0x7ec8u: /* b 0x00007ea4 */
            r[15]=32420u;return 0;
        case 0x7eccu: /* ldrb r8,[r12,#0x29] */
            LOAD(r[8],r[12]+41u,1);r[15]=0x7ed0u;return 0;
        case 0x7ed0u: /* ldrb r7,[r12,#0x0] */
            LOAD(r[7],r[12]+0u,1);r[15]=0x7ed4u;return 0;
        case 0x7ed4u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7ed8u;return 0;
        case 0x7ed8u: /* bl 0x00008180 */
            r[14]=0x7edcu;r[15]=33152u;return 0;
        case 0x7edcu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x7ee0u;return 0;
        case 0x7ee0u: /* bic r7,r7,#0x1 */
            r[7]=r[7]&~1u;r[15]=0x7ee4u;return 0;
        case 0x7ee4u: /* strb r7,[r12,#0x0] */
            STORE(r[12]+0u,r[7],1);r[15]=0x7ee8u;return 0;
        case 0x7ee8u: /* strb r8,[r12,#0x29] */
            STORE(r[12]+41u,r[8],1);r[15]=0x7eecu;return 0;
        case 0x7eecu: /* ldr r7,[r11,#0x0] */
            LOAD(r[7],r[11]+0u,4);r[15]=0x7ef0u;return 0;
        case 0x7ef0u: /* bic r7,r7,#0x4000 */
            r[7]=r[7]&~16384u;r[15]=0x7ef4u;return 0;
        case 0x7ef4u: /* str r7,[r11,#0x0] */
            STORE(r[11]+0u,r[7],4);r[15]=0x7ef8u;return 0;
        case 0x7ef8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x7efcu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7f00u;return 0;
        case 0x7f00u: /* ldr r10,[0x77ec] */
            LOAD(r[10],30700u,4);r[15]=0x7f04u;return 0;
        case 0x7f04u: /* mov r9,#0x10 */
            r[9]=16u;r[15]=0x7f08u;return 0;
        case 0x7f08u: /* mov r3,#0x1f */
            r[3]=31u;r[15]=0x7f0cu;return 0;
        case 0x7f0cu: /* mov r7,#0xf */
            r[7]=15u;r[15]=0x7f10u;return 0;
        case 0x7f10u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7f14u;return 0;
        case 0x7f14u: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x7f18u;return 0;
        case 0x7f18u: /* ldrb r0,[r10,#0x1] */
            LOAD(r[0],r[10]+1u,1);r[15]=0x7f1cu;return 0;
        case 0x7f1cu: /* cmp r0,#0x2 */
            (void)subtract(state,r[0],2u);r[15]=0x7f20u;return 0;
        case 0x7f20u: /* moveq r3,#0x1f */
            if(state->cpsr&0x40000000u){r[3]=31u;}r[15]=0x7f24u;return 0;
        case 0x7f24u: /* movhi r3,#0xf */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[3]=15u;}r[15]=0x7f28u;return 0;
        case 0x7f28u: /* bcc 0x00007f68 */
            if(!(state->cpsr&0x20000000u)){r[15]=32616u;}else{r[15]=0x7f2cu;}return 0;
        case 0x7f2cu: /* ldr r12,[r10,#0x4] */
            LOAD(r[12],r[10]+4u,4);r[15]=0x7f30u;return 0;
        case 0x7f30u: /* ldrb r8,[r12,#0x29] */
            LOAD(r[8],r[12]+41u,1);r[15]=0x7f34u;return 0;
        case 0x7f34u: /* bl 0x0000818c */
            r[14]=0x7f38u;r[15]=33164u;return 0;
        case 0x7f38u: /* strb r8,[r12,#0x29] */
            STORE(r[12]+41u,r[8],1);r[15]=0x7f3cu;return 0;
        case 0x7f3cu: /* strb r7,[r12,#0x2a] */
            STORE(r[12]+42u,r[7],1);r[15]=0x7f40u;return 0;
        case 0x7f40u: /* ldrb r8,[r12,#0x0] */
            LOAD(r[8],r[12]+0u,1);r[15]=0x7f44u;return 0;
        case 0x7f44u: /* bic r8,r8,#0x1 */
            r[8]=r[8]&~1u;r[15]=0x7f48u;return 0;
        case 0x7f48u: /* strb r8,[r12,#0x0] */
            STORE(r[12]+0u,r[8],1);r[15]=0x7f4cu;return 0;
        case 0x7f4cu: /* ldr r8,[r11,#0x0] */
            LOAD(r[8],r[11]+0u,4);r[15]=0x7f50u;return 0;
        case 0x7f50u: /* bic r8,r8,#0x4000 */
            r[8]=r[8]&~16384u;r[15]=0x7f54u;return 0;
        case 0x7f54u: /* str r8,[r11,#0x0] */
            STORE(r[11]+0u,r[8],4);r[15]=0x7f58u;return 0;
        case 0x7f58u: /* strb r6,[r12,#0x23] */
            STORE(r[12]+35u,r[6],1);r[15]=0x7f5cu;return 0;
        case 0x7f5cu: /* str r6,[r12,#0x30] */
            STORE(r[12]+48u,r[6],4);r[15]=0x7f60u;return 0;
        case 0x7f60u: /* str r6,[r12,#0x34] */
            STORE(r[12]+52u,r[6],4);r[15]=0x7f64u;return 0;
        case 0x7f64u: /* strb r3,[r12,#0x28] */
            STORE(r[12]+40u,r[3],1);r[15]=0x7f68u;return 0;
        case 0x7f68u: /* subs r9,r9,#0x1 */
            r[9]=subtract(state,r[9],1u);r[15]=0x7f6cu;return 0;
        case 0x7f6cu: /* beq 0x00008168 */
            if(state->cpsr&0x40000000u){r[15]=33128u;}else{r[15]=0x7f70u;}return 0;
        case 0x7f70u: /* add r10,r10,#0xc */
            r[10]=r[10]+12u;r[15]=0x7f74u;return 0;
        case 0x7f74u: /* b 0x00007f14 */
            r[15]=32532u;return 0;
        case 0x7f78u: /* ldr r12,[0x780c] */
            LOAD(r[12],30732u,4);r[15]=0x7f7cu;return 0;
        case 0x7f7cu: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x7f80u;return 0;
        case 0x7f80u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x7f84u;return 0;
        case 0x7f84u: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x7f88u;return 0;
        case 0x7f88u: /* ldrb r1,[r12,#0x0] */
            LOAD(r[1],r[12]+0u,1);r[15]=0x7f8cu;return 0;
        case 0x7f8cu: /* orr r5,r1,#0x2 */
            r[5]=r[1]|2u;r[15]=0x7f90u;return 0;
        case 0x7f90u: /* strb r5,[r12,#0x0] */
            STORE(r[12]+0u,r[5],1);r[15]=0x7f94u;return 0;
        case 0x7f94u: /* tst r1,#0x1 */
            temporary=r[1]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7f98u;return 0;
        case 0x7f98u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7f9cu;}return 0;
        case 0x7f9cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x7fa0u;return 0;
        case 0x7fa0u: /* bl 0x000081bc */
            r[14]=0x7fa4u;r[15]=33212u;return 0;
        case 0x7fa4u: /* ldr r1,[r12,#0x38] */
            LOAD(r[1],r[12]+56u,4);r[15]=0x7fa8u;return 0;
        case 0x7fa8u: /* cmp r1,#0x0 */
            (void)subtract(state,r[1],0u);r[15]=0x7facu;return 0;
        case 0x7facu: /* beq 0x00008168 */
            if(state->cpsr&0x40000000u){r[15]=33128u;}else{r[15]=0x7fb0u;}return 0;
        case 0x7fb0u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x7fb4u;return 0;
        case 0x7fb4u: /* bne 0x00008168 */
            if(!(state->cpsr&0x40000000u)){r[15]=33128u;}else{r[15]=0x7fb8u;}return 0;
        case 0x7fb8u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x7fbcu;return 0;
        case 0x7fbcu: /* ldrb r2,[r12,#0x29] */
            LOAD(r[2],r[12]+41u,1);r[15]=0x7fc0u;return 0;
        case 0x7fc0u: /* bl 0x00008180 */
            r[14]=0x7fc4u;r[15]=33152u;return 0;
        case 0x7fc4u: /* bic r5,r5,#0x1 */
            r[5]=r[5]&~1u;r[15]=0x7fc8u;return 0;
        case 0x7fc8u: /* strb r5,[r12,#0x0] */
            STORE(r[12]+0u,r[5],1);r[15]=0x7fccu;return 0;
        case 0x7fccu: /* strb r2,[r12,#0x29] */
            STORE(r[12]+41u,r[2],1);r[15]=0x7fd0u;return 0;
        case 0x7fd0u: /* b 0x00008168 */
            r[15]=33128u;return 0;
        case 0x7fd4u: /* ldr r12,[0x780c] */
            LOAD(r[12],30732u,4);r[15]=0x7fd8u;return 0;
        case 0x7fd8u: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x7fdcu;return 0;
        case 0x7fdcu: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x7fe0u;return 0;
        case 0x7fe0u: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x7fe4u;return 0;
        case 0x7fe4u: /* ldrb r1,[r12,#0x0] */
            LOAD(r[1],r[12]+0u,1);r[15]=0x7fe8u;return 0;
        case 0x7fe8u: /* tst r1,#0x2 */
            temporary=r[1]&2u;logic_flags(state,temporary,0,2u);r[15]=0x7fecu;return 0;
        case 0x7fecu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x7ff0u;}return 0;
        case 0x7ff0u: /* bic r1,r1,#0x2 */
            r[1]=r[1]&~2u;r[15]=0x7ff4u;return 0;
        case 0x7ff4u: /* strb r1,[r12,#0x0] */
            STORE(r[12]+0u,r[1],1);r[15]=0x7ff8u;return 0;
        case 0x7ff8u: /* tst r1,#0x1 */
            temporary=r[1]&1u;logic_flags(state,temporary,0,1u);r[15]=0x7ffcu;return 0;
        case 0x7ffcu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x8000u;}return 0;
        case 0x8000u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x8004u;return 0;
        case 0x8004u: /* bl 0x000081bc */
            r[14]=0x8008u;r[15]=33212u;return 0;
        case 0x8008u: /* b 0x00008154 */
            r[15]=33108u;return 0;
        case 0x800cu: /* ldr r12,[0x780c] */
            LOAD(r[12],30732u,4);r[15]=0x8010u;return 0;
        case 0x8010u: /* mov r7,#0x10 */
            r[7]=16u;r[15]=0x8014u;return 0;
        case 0x8014u: /* b 0x0000802c */
            r[15]=32812u;return 0;
        case 0x8018u: /* ldr r12,[0x780c] */
            LOAD(r[12],30732u,4);r[15]=0x801cu;return 0;
        case 0x801cu: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x8020u;return 0;
        case 0x8020u: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x8024u;return 0;
        case 0x8024u: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x8028u;return 0;
        case 0x8028u: /* mov r7,#0x1 */
            r[7]=1u;r[15]=0x802cu;return 0;
        case 0x802cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x8030u;return 0;
        case 0x8030u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x8034u;return 0;
        case 0x8034u: /* str r6,[r12,#0x34] */
            STORE(r[12]+52u,r[6],4);r[15]=0x8038u;return 0;
        case 0x8038u: /* strb r6,[r12,#0x23] */
            STORE(r[12]+35u,r[6],1);r[15]=0x803cu;return 0;
        case 0x803cu: /* ldrb r1,[r12,#0x0] */
            LOAD(r[1],r[12]+0u,1);r[15]=0x8040u;return 0;
        case 0x8040u: /* tst r1,#0x1 */
            temporary=r[1]&1u;logic_flags(state,temporary,0,1u);r[15]=0x8044u;return 0;
        case 0x8044u: /* beq 0x00008054 */
            if(state->cpsr&0x40000000u){r[15]=32852u;}else{r[15]=0x8048u;}return 0;
        case 0x8048u: /* bl 0x000081bc */
            r[14]=0x804cu;r[15]=33212u;return 0;
        case 0x804cu: /* bl 0x000080a4 */
            r[14]=0x8050u;r[15]=32932u;return 0;
        case 0x8050u: /* b 0x00008034 */
            r[15]=32820u;return 0;
        case 0x8054u: /* bl 0x000080d8 */
            r[14]=0x8058u;r[15]=32984u;return 0;
        case 0x8058u: /* b 0x00008034 */
            r[15]=32820u;return 0;
        case 0x805cu: /* ldr r12,[0x7800] */
            LOAD(r[12],30720u,4);r[15]=0x8060u;return 0;
        case 0x8060u: /* mov r7,#0x10 */
            r[7]=16u;r[15]=0x8064u;return 0;
        case 0x8064u: /* b 0x0000807c */
            r[15]=32892u;return 0;
        case 0x8068u: /* ldr r12,[0x7800] */
            LOAD(r[12],30720u,4);r[15]=0x806cu;return 0;
        case 0x806cu: /* ldr r7,[0x7804] */
            LOAD(r[7],30724u,4);r[15]=0x8070u;return 0;
        case 0x8070u: /* and r8,r0,#0x7 */
            r[8]=r[0]&7u;r[15]=0x8074u;return 0;
        case 0x8074u: /* mla r12,r8,r7,r12 */
            r[12]=r[8]*r[7]+r[12];r[15]=0x8078u;return 0;
        case 0x8078u: /* mov r7,#0x2 */
            r[7]=2u;r[15]=0x807cu;return 0;
        case 0x807cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x8080u;return 0;
        case 0x8080u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x8084u;return 0;
        case 0x8084u: /* ldrb r1,[r12,#0x0] */
            LOAD(r[1],r[12]+0u,1);r[15]=0x8088u;return 0;
        case 0x8088u: /* tst r1,#0x1 */
            temporary=r[1]&1u;logic_flags(state,temporary,0,1u);r[15]=0x808cu;return 0;
        case 0x808cu: /* beq 0x0000809c */
            if(state->cpsr&0x40000000u){r[15]=32924u;}else{r[15]=0x8090u;}return 0;
        case 0x8090u: /* bl 0x000081bc */
            r[14]=0x8094u;r[15]=33212u;return 0;
        case 0x8094u: /* bl 0x000080a4 */
            r[14]=0x8098u;r[15]=32932u;return 0;
        case 0x8098u: /* b 0x00008084 */
            r[15]=32900u;return 0;
        case 0x809cu: /* bl 0x000080d8 */
            r[14]=0x80a0u;r[15]=32984u;return 0;
        case 0x80a0u: /* b 0x00008084 */
            r[15]=32900u;return 0;
        case 0x80a4u: /* ldrb r1,[r12,#0x24] */
            LOAD(r[1],r[12]+36u,1);r[15]=0x80a8u;return 0;
        case 0x80a8u: /* strb r1,[r11,#0x29] */
            STORE(r[11]+41u,r[1],1);r[15]=0x80acu;return 0;
        case 0x80acu: /* strb r1,[r12,#0x20] */
            STORE(r[12]+32u,r[1],1);r[15]=0x80b0u;return 0;
        case 0x80b0u: /* ldrb r1,[r12,#0x28] */
            LOAD(r[1],r[12]+40u,1);r[15]=0x80b4u;return 0;
        case 0x80b4u: /* strb r1,[r11,#0x24] */
            STORE(r[11]+36u,r[1],1);r[15]=0x80b8u;return 0;
        case 0x80b8u: /* strb r1,[r12,#0x21] */
            STORE(r[12]+33u,r[1],1);r[15]=0x80bcu;return 0;
        case 0x80bcu: /* ldrb r1,[r12,#0x26] */
            LOAD(r[1],r[12]+38u,1);r[15]=0x80c0u;return 0;
        case 0x80c0u: /* ldrb r2,[r11,#0x20] */
            LOAD(r[2],r[11]+32u,1);r[15]=0x80c4u;return 0;
        case 0x80c4u: /* and r2,r2,#0xf */
            r[2]=r[2]&15u;r[15]=0x80c8u;return 0;
        case 0x80c8u: /* orr r2,r2,r1, lsl #0x4 */
            r[2]=r[2]|(r[1]<<4);r[15]=0x80ccu;return 0;
        case 0x80ccu: /* strb r2,[r11,#0x20] */
            STORE(r[11]+32u,r[2],1);r[15]=0x80d0u;return 0;
        case 0x80d0u: /* ldrb r1,[r12,#0x2a] */
            LOAD(r[1],r[12]+42u,1);r[15]=0x80d4u;return 0;
        case 0x80d4u: /* strb r1,[r11,#0x25] */
            STORE(r[11]+37u,r[1],1);r[15]=0x80d8u;return 0;
        case 0x80d8u: /* ldr r9,[0x77fc] */
            LOAD(r[9],30716u,4);r[15]=0x80dcu;return 0;
        case 0x80dcu: /* str r6,[r12,#0x40] */
            STORE(r[12]+64u,r[6],4);r[15]=0x80e0u;return 0;
        case 0x80e0u: /* str r6,[r12,#0x44] */
            STORE(r[12]+68u,r[6],4);r[15]=0x80e4u;return 0;
        case 0x80e4u: /* str r6,[r12,#0x4c] */
            STORE(r[12]+76u,r[6],4);r[15]=0x80e8u;return 0;
        case 0x80e8u: /* str r6,[r12,#0x50] */
            STORE(r[12]+80u,r[6],4);r[15]=0x80ecu;return 0;
        case 0x80ecu: /* str r6,[r12,#0x48] */
            STORE(r[12]+72u,r[6],4);r[15]=0x80f0u;return 0;
        case 0x80f0u: /* str r6,[r12,#0x54] */
            STORE(r[12]+84u,r[6],4);r[15]=0x80f4u;return 0;
        case 0x80f4u: /* str r6,[r12,#0x10] */
            STORE(r[12]+16u,r[6],4);r[15]=0x80f8u;return 0;
        case 0x80f8u: /* str r6,[r12,#0x14] */
            STORE(r[12]+20u,r[6],4);r[15]=0x80fcu;return 0;
        case 0x80fcu: /* str r6,[r12,#0x1c] */
            STORE(r[12]+28u,r[6],4);r[15]=0x8100u;return 0;
        case 0x8100u: /* str r9,[r12,#0x18] */
            STORE(r[12]+24u,r[9],4);r[15]=0x8104u;return 0;
        case 0x8104u: /* strb r9,[r12,#0x29] */
            STORE(r[12]+41u,r[9],1);r[15]=0x8108u;return 0;
        case 0x8108u: /* strb r9,[r12,#0x1f] */
            STORE(r[12]+31u,r[9],1);r[15]=0x810cu;return 0;
        case 0x810cu: /* strb r9,[r12,#0x1e] */
            STORE(r[12]+30u,r[9],1);r[15]=0x8110u;return 0;
        case 0x8110u: /* strb r9,[r12,#0x41] */
            STORE(r[12]+65u,r[9],1);r[15]=0x8114u;return 0;
        case 0x8114u: /* strb r9,[r12,#0x45] */
            STORE(r[12]+69u,r[9],1);r[15]=0x8118u;return 0;
        case 0x8118u: /* strb r9,[r12,#0x4d] */
            STORE(r[12]+77u,r[9],1);r[15]=0x811cu;return 0;
        case 0x811cu: /* strb r9,[r12,#0x51] */
            STORE(r[12]+81u,r[9],1);r[15]=0x8120u;return 0;
        case 0x8120u: /* ldrb r5,[r12,#0x0] */
            LOAD(r[5],r[12]+0u,1);r[15]=0x8124u;return 0;
        case 0x8124u: /* bic r5,r5,#0x4 */
            r[5]=r[5]&~4u;r[15]=0x8128u;return 0;
        case 0x8128u: /* bic r5,r5,#0x10 */
            r[5]=r[5]&~16u;r[15]=0x812cu;return 0;
        case 0x812cu: /* strb r5,[r12,#0x0] */
            STORE(r[12]+0u,r[5],1);r[15]=0x8130u;return 0;
        case 0x8130u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x8134u;return 0;
        case 0x8134u: /* ldmiaeq sp!,{lr} */
            if(state->cpsr&0x40000000u){LOAD(r[14],r[13]+0u,4);r[13]+=4u;}r[15]=0x8138u;return 0;
        case 0x8138u: /* add r12,r12,#0x58 */
            r[12]=r[12]+88u;r[15]=0x813cu;return 0;
        case 0x813cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x8140u: /* ldrb r0,[r12,#0x1] */
            LOAD(r[0],r[12]+1u,1);r[15]=0x8144u;return 0;
        case 0x8144u: /* orr r4,r4,r0 */
            r[4]=r[4]|r[0];r[15]=0x8148u;return 0;
        case 0x8148u: /* strb r4,[r12,#0x1] */
            STORE(r[12]+1u,r[4],1);r[15]=0x814cu;return 0;
        case 0x814cu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x8150u;return 0;
        case 0x8150u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x8154u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x8158u;return 0;
        case 0x8158u: /* ldr r0,[r11,#0x0] */
            LOAD(r[0],r[11]+0u,4);r[15]=0x815cu;return 0;
        case 0x815cu: /* orr r0,r0,#0xc000 */
            r[0]=r[0]|49152u;r[15]=0x8160u;return 0;
        case 0x8160u: /* str r0,[r11,#0x0] */
            STORE(r[11]+0u,r[0],4);r[15]=0x8164u;return 0;
        case 0x8164u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x8168u: /* ldr r1,[r11,#0x0] */
            LOAD(r[1],r[11]+0u,4);r[15]=0x816cu;return 0;
        case 0x816cu: /* bic r1,r1,#0x4000 */
            r[1]=r[1]&~16384u;r[15]=0x8170u;return 0;
        case 0x8170u: /* orr r1,r1,#0x8000 */
            r[1]=r[1]|32768u;r[15]=0x8174u;return 0;
        case 0x8174u: /* str r1,[r11,#0x0] */
            STORE(r[11]+0u,r[1],4);r[15]=0x8178u;return 0;
        case 0x8178u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x817cu;return 0;
        case 0x817cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x81bcu: /* ldr r10,[0x77ec] */
            LOAD(r[10],30700u,4);r[15]=0x81c0u;return 0;
        case 0x81c0u: /* mov r4,#0xc */
            r[4]=12u;r[15]=0x81c4u;return 0;
        case 0x81c4u: /* ldrb r9,[r12,#0x3] */
            LOAD(r[9],r[12]+3u,1);r[15]=0x81c8u;return 0;
        case 0x81c8u: /* mla r10,r4,r9,r10 */
            r[10]=r[4]*r[9]+r[10];r[15]=0x81ccu;return 0;
        case 0x81ccu: /* ldr r11,[r10,#0x8] */
            LOAD(r[11],r[10]+8u,4);r[15]=0x81d0u;return 0;
        case 0x81d0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x81d4u: /* ldr r12,[0x780c] */
            LOAD(r[12],30732u,4);r[15]=0x81d8u;return 0;
        case 0x81d8u: /* mov r7,#0x58 */
            r[7]=88u;r[15]=0x81dcu;return 0;
        case 0x81dcu: /* and r0,r0,#0x7 */
            r[0]=r[0]&7u;r[15]=0x81e0u;return 0;
        case 0x81e0u: /* mla r12,r7,r0,r12 */
            r[12]=r[7]*r[0]+r[12];r[15]=0x81e4u;return 0;
        case 0x81e4u: /* cmp r1,#0x2 */
            (void)subtract(state,r[1],2u);r[15]=0x81e8u;return 0;
        case 0x81e8u: /* movhi r4,#0x20 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[4]=32u;}r[15]=0x81ecu;return 0;
        case 0x81ecu: /* bhi 0x00008140 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[15]=33088u;}else{r[15]=0x81f0u;}return 0;
        case 0x81f0u: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x81f4u;return 0;
        case 0x81f4u: /* str r2,[r12,#0x34] */
            STORE(r[12]+52u,r[2],4);r[15]=0x81f8u;return 0;
        case 0x81f8u: /* strb r1,[r12,#0x23] */
            STORE(r[12]+35u,r[1],1);r[15]=0x81fcu;return 0;
        case 0x81fcu: /* ldrb r3,[r12,#0x0] */
            LOAD(r[3],r[12]+0u,1);r[15]=0x8200u;return 0;
        case 0x8200u: /* cmp r3,#0x1 */
            (void)subtract(state,r[3],1u);r[15]=0x8204u;return 0;
        case 0x8204u: /* movcc pc,lr */
            if(!(state->cpsr&0x20000000u)){r[15]=r[14];}else{r[15]=0x8208u;}return 0;
        case 0x8208u: /* ldr r3,[0x825c] */
            LOAD(r[3],33372u,4);r[15]=0x820cu;return 0;
        case 0x820cu: /* ldr r3,[r3,r1,lsl #0x2] */
            LOAD(r[3],r[3]+(r[1]<<2),4);r[15]=0x8210u;return 0;
        case 0x8210u: /* mov pc,r3 */
            r[15]=r[3];return 0;
        case 0x8224u: /* ldrb r3,[r12,#0x3c] */
            LOAD(r[3],r[12]+60u,1);r[15]=0x8228u;return 0;
        case 0x8228u: /* add r2,r3,r2 */
            r[2]=r[3]+r[2];r[15]=0x822cu;return 0;
        case 0x822cu: /* b 0x00008244 */
            r[15]=33348u;return 0;
        case 0x8230u: /* ldrb r3,[r12,#0x3c] */
            LOAD(r[3],r[12]+60u,1);r[15]=0x8234u;return 0;
        case 0x8234u: /* mul r3,r2,r3 */
            r[3]=r[2]*r[3];r[15]=0x8238u;return 0;
        case 0x8238u: /* mov r2,r3, lsr #0x8 */
            r[2]=(r[3]>>8);r[15]=0x823cu;return 0;
        case 0x823cu: /* b 0x00008244 */
            r[15]=33348u;return 0;
        case 0x8240u: /* ldrb r2,[r12,#0x3c] */
            LOAD(r[2],r[12]+60u,1);r[15]=0x8244u;return 0;
        case 0x8244u: /* str r2,[r12,#0x38] */
            STORE(r[12]+56u,r[2],4);r[15]=0x8248u;return 0;
        case 0x8248u: /* mov pc,lr */
            r[15]=r[14];return 0;
 default:*handled=0;return 0;
 }}
}

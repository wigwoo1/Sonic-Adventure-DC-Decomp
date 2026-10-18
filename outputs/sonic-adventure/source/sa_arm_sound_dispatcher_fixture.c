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
int sa_arm_sound_bank_select_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
int sa_arm_sound_control_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
int sa_arm_sound_note_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
int sa_arm_sound_status_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
int sa_arm_sound_update_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
int sa_arm_sound_snapshot_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
int sa_arm_sound_ramp_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
int sa_arm_sound_effect_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
int sa_arm_sound_stream_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
int sa_arm_sound_sequence_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
int sa_arm_sound_dispatch_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
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
SA_EXPORT int sa_arm_sound_boot_native(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,sa_arm_service service,uint32_t stop_pc,uint32_t budget){
 if(!ram||!state||!read||!write||driver>1||(state->cpsr&0x3fu)!=0x13u)return -2;
 uint32_t *r=state->r,temporary;int started=0;
 for(;;){
    if(started&&r[15]==stop_pc)return 0;
    if(!budget)return -3;--budget;started=1;
    switch(r[15]){
        case 0x0u: /* b 0x00000100 */
            r[15]=0x4u;r[15]=0x100u;break;
        case 0x100u: /* mov sp,#0xb000 */
            r[13]=45056u;r[15]=0x104u;break;
        case 0x104u: /* mrs r10,cpsr */
            r[10]=state->cpsr;r[15]=0x108u;break;
        case 0x108u: /* orr r10,r10,#0x40 */
            r[10]=r[10]|64u;r[15]=0x10cu;break;
        case 0x10cu: /* msr cpsr_cf,r10 */
            state->cpsr=r[10];r[15]=0x110u;break;
        case 0x110u: /* ldr r0,[0x8ec] */
            LOAD(r[0],2284u,4);r[15]=0x114u;break;
        case 0x114u: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x118u;break;
        case 0x118u: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x11cu;break;
        case 0x11cu: /* ldr r0,[0x8f0] */
            LOAD(r[0],2288u,4);r[15]=0x120u;break;
        case 0x120u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x124u;break;
        case 0x124u: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x128u;break;
        case 0x128u: /* bl 0x00000650 */
            r[15]=0x12cu;r[14]=0x12cu;r[15]=0x650u;break;
        case 0x12cu: /* bl 0x0000043c */
            r[15]=0x130u;r[14]=0x130u;r[15]=0x43cu;break;
        case 0x130u: /* bl 0x00000464 */
            r[15]=0x134u;r[14]=0x134u;r[15]=0x464u;break;
        case 0x134u: /* bl 0x000004e8 */
            r[15]=0x138u;r[14]=0x138u;r[15]=0x4e8u;break;
        case 0x138u: /* bl 0x00000504 */
            r[15]=0x13cu;r[14]=0x13cu;r[15]=0x504u;break;
        case 0x13cu: /* bl 0x00000610 */
            r[15]=0x140u;r[14]=0x140u;r[15]=0x610u;break;
        case 0x140u: /* bl 0x000008cc */
            r[15]=0x144u;r[14]=0x144u;r[15]=0x8ccu;break;
        case 0x144u: /* bl 0x00005d5c */
            r[15]=0x148u;r[14]=0x148u;r[15]=driver?0x5d5cu:0x5d54u;break;
        case 0x148u: /* bl 0x000073bc */
            r[15]=0x14cu;r[14]=0x14cu;r[15]=driver?0x73bcu:0x73b4u;break;
        case 0x14cu: /* bl 0x00007708 */
            r[15]=0x150u;r[14]=0x150u;r[15]=driver?0x7708u:0x7700u;break;
        case 0x150u: /* bl 0x00000690 */
            r[15]=0x154u;r[14]=0x154u;r[15]=0x690u;break;
        case 0x154u: /* ldr r0,[0x8f0] */
            LOAD(r[0],2288u,4);r[15]=0x158u;break;
        case 0x158u: /* mov r1,#0xf */
            r[1]=15u;r[15]=0x15cu;break;
        case 0x15cu: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x160u;break;
        case 0x160u: /* ldr r0,[0x8f4] */
            LOAD(r[0],2292u,4);r[15]=0x164u;break;
        case 0x164u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x168u;break;
        case 0x168u: /* bl 0x00000634 */
            r[15]=0x16cu;r[14]=0x16cu;r[15]=0x634u;break;
        case 0x16cu: /* bl 0x000003cc */
            r[15]=0x170u;r[14]=0x170u;r[15]=0x3ccu;break;
        case 0x170u: /* mov r0,#0x18000 */
            r[0]=98304u;r[15]=0x174u;break;
        case 0x174u: /* ldr r1,[0x8f8] */
            LOAD(r[1],2296u,4);r[15]=0x178u;break;
        case 0x178u: /* str r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],4);r[15]=0x17cu;break;
        case 0x17cu: /* ldr r0,[0x8ec] */
            LOAD(r[0],2284u,4);r[15]=0x180u;break;
        case 0x180u: /* ldr r1,[0x38] */
            LOAD(r[1],56u,4);r[15]=0x184u;break;
        case 0x184u: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x188u;break;
        case 0x188u: /* mrs r10,cpsr */
            r[10]=state->cpsr;r[15]=0x18cu;break;
        case 0x18cu: /* bic r10,r10,#0x40 */
            r[10]=r[10]&~64u;r[15]=0x190u;break;
        case 0x190u: /* msr cpsr_cf,r10 */
            state->cpsr=r[10];r[15]=0x194u;break;
        case 0x194u: /* bl 0x000009a4 */
            r[15]=0x198u;r[14]=0x198u;r[15]=0x9a4u;break;
        case 0x198u: /* ldrb r9,[0x22e] */
            LOAD(r[9],558u,1);r[15]=0x19cu;break;
        case 0x19cu: /* tst r9,#0xff */
            temporary=r[9]&255u;logic_flags(state,temporary,0,255u);r[15]=0x1a0u;break;
        case 0x1a0u: /* beq 0x000001b8 */
            r[15]=0x1a4u;if((state->cpsr&0x40000000u)){r[15]=0x1b8u;}break;
        case 0x1a4u: /* movne r9,#0x0 */
            if(!(state->cpsr&0x40000000u)){r[9]=0u;}r[15]=0x1a8u;break;
        case 0x1a8u: /* strb r9,[0x22e] */
            STORE(558u,r[9],1);r[15]=0x1acu;break;
        case 0x1acu: /* bl 0x00000858 */
            r[15]=0x1b0u;r[14]=0x1b0u;r[15]=0x858u;break;
        case 0x1b0u: /* bl 0x0000089c */
            r[15]=0x1b4u;r[14]=0x1b4u;r[15]=0x89cu;break;
        case 0x1b4u: /* b 0x00000194 */
            r[15]=0x1b8u;r[15]=0x194u;break;
        case 0x1b8u: /* ldrb r9,[0x22c] */
            LOAD(r[9],556u,1);r[15]=0x1bcu;break;
        case 0x1bcu: /* tst r9,#0xff */
            temporary=r[9]&255u;logic_flags(state,temporary,0,255u);r[15]=0x1c0u;break;
        case 0x1c0u: /* beq 0x00000194 */
            r[15]=0x1c4u;if((state->cpsr&0x40000000u)){r[15]=0x194u;}break;
        case 0x1c4u: /* movne r9,#0x0 */
            if(!(state->cpsr&0x40000000u)){r[9]=0u;}r[15]=0x1c8u;break;
        case 0x1c8u: /* strb r9,[0x22c] */
            STORE(556u,r[9],1);r[15]=0x1ccu;break;
        case 0x1ccu: /* bl 0x00000858 */
            r[15]=0x1d0u;r[14]=0x1d0u;r[15]=0x858u;break;
        case 0x1d0u: /* mov r0,#0x13400 */
            r[0]=78848u;r[15]=0x1d4u;break;
        case 0x1d4u: /* ldrb r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,1);r[15]=0x1d8u;break;
        case 0x1d8u: /* mov r0,#0xff */
            r[0]=255u;r[15]=0x1dcu;break;
        case 0x1dcu: /* tst r1,r0 */
            temporary=r[1]&r[0];logic_flags(state,temporary,0,r[0]);r[15]=0x1e0u;break;
        case 0x1e0u: /* blne 0x000048f0 */
            r[15]=0x1e4u;if(!(state->cpsr&0x40000000u)){r[14]=0x1e4u;r[15]=driver?0x48f0u:0x48e8u;}break;
        case 0x1e4u: /* bl 0x000070ac */
            r[15]=0x1e8u;r[14]=0x1e8u;r[15]=driver?0x70acu:0x70a4u;break;
        case 0x1e8u: /* bl 0x00004218 */
            r[15]=0x1ecu;r[14]=0x1ecu;r[15]=driver?0x4218u:0x4210u;break;
        case 0x1ecu: /* bl 0x00006788 */
            r[15]=0x1f0u;r[14]=0x1f0u;r[15]=driver?0x6788u:0x6780u;break;
        case 0x1f0u: /* bl 0x00006648 */
            r[15]=0x1f4u;r[14]=0x1f4u;r[15]=driver?0x6648u:0x6640u;break;
        case 0x1f4u: /* bl 0x000066e8 */
            r[15]=0x1f8u;r[14]=0x1f8u;r[15]=driver?0x66e8u:0x66e0u;break;
        case 0x1f8u: /* bl 0x00006854 */
            r[15]=0x1fcu;r[14]=0x1fcu;r[15]=driver?0x6854u:0x684cu;break;
        case 0x1fcu: /* bl 0x00006bdc */
            r[15]=0x200u;r[14]=0x200u;r[15]=driver?0x6bdcu:0x6bd4u;break;
        case 0x200u: /* bl 0x00006400 */
            r[15]=0x204u;r[14]=0x204u;r[15]=driver?0x6400u:0x63f8u;break;
        case 0x204u: /* bl 0x00006628 */
            r[15]=0x208u;r[14]=0x208u;r[15]=driver?0x6628u:0x6620u;break;
        case 0x208u: /* bl 0x00006900 */
            r[15]=0x20cu;r[14]=0x20cu;r[15]=driver?0x6900u:0x68f8u;break;
        case 0x20cu: /* bl 0x000069b0 */
            r[15]=0x210u;r[14]=0x210u;r[15]=driver?0x69b0u:0x69a8u;break;
        case 0x210u: /* bl 0x00006a64 */
            r[15]=0x214u;r[14]=0x214u;r[15]=driver?0x6a64u:0x6a5cu;break;
        case 0x214u: /* bl 0x00000768 */
            r[15]=0x218u;r[14]=0x218u;r[15]=0x768u;break;
        case 0x218u: /* bl 0x000006d0 */
            r[15]=0x21cu;r[14]=0x21cu;r[15]=0x6d0u;break;
        case 0x21cu: /* bl 0x00006164 */
            r[15]=0x220u;r[14]=0x220u;r[15]=driver?0x6164u:0x615cu;break;
        case 0x220u: /* bl 0x0000089c */
            r[15]=0x224u;r[14]=0x224u;r[15]=0x89cu;break;
        case 0x224u: /* b 0x00000194 */
            r[15]=0x228u;r[15]=0x194u;break;
        case 0x3ccu: /* ldr r8,[0x918] */
            LOAD(r[8],2328u,4);r[15]=0x3d0u;break;
        case 0x3d0u: /* mov r9,#0x1 */
            r[9]=1u;r[15]=0x3d4u;break;
        case 0x3d4u: /* mov r10,#0x8 */
            r[10]=8u;r[15]=0x3d8u;break;
        case 0x3d8u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x3dcu;break;
        case 0x3dcu: /* mov r0,r0 */
            r[0]=r[0];r[15]=0x3e0u;break;
        case 0x3e0u: /* mov r0,r0 */
            r[0]=r[0];r[15]=0x3e4u;break;
        case 0x3e4u: /* mov r0,r0 */
            r[0]=r[0];r[15]=0x3e8u;break;
        case 0x3e8u: /* subs r10,r10,#0x1 */
            r[10]=subtract(state,r[10],1u);r[15]=0x3ecu;break;
        case 0x3ecu: /* bne 0x000003d8 */
            r[15]=0x3f0u;if(!(state->cpsr&0x40000000u)){r[15]=0x3d8u;}break;
        case 0x3f0u: /* ldr r8,[0x91c] */
            LOAD(r[8],2332u,4);r[15]=0x3f4u;break;
        case 0x3f4u: /* mov r0,#0x18 */
            r[0]=24u;r[15]=0x3f8u;break;
        case 0x3f8u: /* strb r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],1);r[15]=0x3fcu;break;
        case 0x3fcu: /* ldr r8,[0x920] */
            LOAD(r[8],2336u,4);r[15]=0x400u;break;
        case 0x400u: /* mov r0,#0x50 */
            r[0]=80u;r[15]=0x404u;break;
        case 0x404u: /* strb r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],1);r[15]=0x408u;break;
        case 0x408u: /* ldr r8,[0x924] */
            LOAD(r[8],2340u,4);r[15]=0x40cu;break;
        case 0x40cu: /* mov r0,#0x8 */
            r[0]=8u;r[15]=0x410u;break;
        case 0x410u: /* strb r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],1);r[15]=0x414u;break;
        case 0x414u: /* ldr r8,[0x904] */
            LOAD(r[8],2308u,4);r[15]=0x418u;break;
        case 0x418u: /* ldr r0,[0x438] */
            LOAD(r[0],1080u,4);r[15]=0x41cu;break;
        case 0x41cu: /* str r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],4);r[15]=0x420u;break;
        case 0x420u: /* ldr r8,[0x928] */
            LOAD(r[8],2344u,4);r[15]=0x424u;break;
        case 0x424u: /* mov r0,#0x48 */
            r[0]=72u;r[15]=0x428u;break;
        case 0x428u: /* str r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],4);r[15]=0x42cu;break;
        case 0x42cu: /* ldr r8,[0x908] */
            LOAD(r[8],2312u,4);r[15]=0x430u;break;
        case 0x430u: /* str r0,[r8,#0x0] */
            STORE(r[8]+0u,r[0],4);r[15]=0x434u;break;
        case 0x434u: /* mov pc,lr */
            r[15]=r[14];break;
        case 0x43cu: /* mov r12,#0x13000 */
            r[12]=77824u;r[15]=0x440u;break;
        case 0x440u: /* mov r1,#0x1400 */
            r[1]=5120u;r[15]=0x444u;break;
        case 0x444u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x448u;break;
        case 0x448u: /* str r0,[r12],#0x4 */
            STORE(r[12],r[0],4);r[12]+=4u;r[15]=0x44cu;break;
        case 0x44cu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x450u;break;
        case 0x450u: /* bne 0x00000448 */
            r[15]=0x454u;if(!(state->cpsr&0x40000000u)){r[15]=0x448u;}break;
        case 0x454u: /* ldr r1,[0x92c] */
            LOAD(r[1],2348u,4);r[15]=0x458u;break;
        case 0x458u: /* mov r0,#0xff */
            r[0]=255u;r[15]=0x45cu;break;
        case 0x45cu: /* strb r0,[r1,#0x0] */
            STORE(r[1]+0u,r[0],1);r[15]=0x460u;break;
        case 0x460u: /* mov pc,lr */
            r[15]=r[14];break;
        case 0x464u: /* mov r12,#0x14000 */
            r[12]=81920u;r[15]=0x468u;break;
        case 0x468u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x46cu;break;
        case 0x46cu: /* mov r1,#0x20 */
            r[1]=32u;r[15]=0x470u;break;
        case 0x470u: /* str r0,[r12],#0x4 */
            STORE(r[12],r[0],4);r[12]+=4u;r[15]=0x474u;break;
        case 0x474u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x478u;break;
        case 0x478u: /* bne 0x00000470 */
            r[15]=0x47cu;if(!(state->cpsr&0x40000000u)){r[15]=0x470u;}break;
        case 0x47cu: /* ldr r12,[0x930] */
            LOAD(r[12],2352u,4);r[15]=0x480u;break;
        case 0x480u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x484u;break;
        case 0x484u: /* mov r1,#0x20 */
            r[1]=32u;r[15]=0x488u;break;
        case 0x488u: /* str r0,[r12],#0x4 */
            STORE(r[12],r[0],4);r[12]+=4u;r[15]=0x48cu;break;
        case 0x48cu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x490u;break;
        case 0x490u: /* bne 0x00000488 */
            r[15]=0x494u;if(!(state->cpsr&0x40000000u)){r[15]=0x488u;}break;
        case 0x494u: /* ldr r12,[0x934] */
            LOAD(r[12],2356u,4);r[15]=0x498u;break;
        case 0x498u: /* ldr r0,[0x938] */
            LOAD(r[0],2360u,4);r[15]=0x49cu;break;
        case 0x49cu: /* ldr r2,[0x93c] */
            LOAD(r[2],2364u,4);r[15]=0x4a0u;break;
        case 0x4a0u: /* ldr r1,[r2,#0x0] */
            LOAD(r[1],r[2]+0u,4);r[15]=0x4a4u;break;
        case 0x4a4u: /* str r0,[r12],#0x4 */
            STORE(r[12],r[0],4);r[12]+=4u;r[15]=0x4a8u;break;
        case 0x4a8u: /* str r1,[r12],#0x4 */
            STORE(r[12],r[1],4);r[12]+=4u;r[15]=0x4acu;break;
        case 0x4acu: /* str r0,[r12],#0x4 */
            STORE(r[12],r[0],4);r[12]+=4u;r[15]=0x4b0u;break;
        case 0x4b0u: /* str r1,[r12,#0x0] */
            STORE(r[12]+0u,r[1],4);r[15]=0x4b4u;break;
        case 0x4b4u: /* ldr r12,[0x940] */
            LOAD(r[12],2368u,4);r[15]=0x4b8u;break;
        case 0x4b8u: /* ldr r0,[0x944] */
            LOAD(r[0],2372u,4);r[15]=0x4bcu;break;
        case 0x4bcu: /* ldr r2,[0x948] */
            LOAD(r[2],2376u,4);r[15]=0x4c0u;break;
        case 0x4c0u: /* ldr r1,[r2,#0x0] */
            LOAD(r[1],r[2]+0u,4);r[15]=0x4c4u;break;
        case 0x4c4u: /* str r0,[r12],#0x4 */
            STORE(r[12],r[0],4);r[12]+=4u;r[15]=0x4c8u;break;
        case 0x4c8u: /* str r1,[r12,#0x0] */
            STORE(r[12]+0u,r[1],4);r[15]=0x4ccu;break;
        case 0x4ccu: /* ldr r12,[0x94c] */
            LOAD(r[12],2380u,4);r[15]=0x4d0u;break;
        case 0x4d0u: /* ldr r0,[0x950] */
            LOAD(r[0],2384u,4);r[15]=0x4d4u;break;
        case 0x4d4u: /* ldr r2,[0x954] */
            LOAD(r[2],2388u,4);r[15]=0x4d8u;break;
        case 0x4d8u: /* ldr r1,[r2,#0x0] */
            LOAD(r[1],r[2]+0u,4);r[15]=0x4dcu;break;
        case 0x4dcu: /* str r0,[r12],#0x4 */
            STORE(r[12],r[0],4);r[12]+=4u;r[15]=0x4e0u;break;
        case 0x4e0u: /* str r1,[r12,#0x0] */
            STORE(r[12]+0u,r[1],4);r[15]=0x4e4u;break;
        case 0x4e4u: /* mov pc,lr */
            r[15]=r[14];break;
        case 0x4e8u: /* mov r12,#0xb000 */
            r[12]=45056u;r[15]=0x4ecu;break;
        case 0x4ecu: /* mov r1,#0x400 */
            r[1]=1024u;r[15]=0x4f0u;break;
        case 0x4f0u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x4f4u;break;
        case 0x4f4u: /* str r0,[r12],#0x4 */
            STORE(r[12],r[0],4);r[12]+=4u;r[15]=0x4f8u;break;
        case 0x4f8u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x4fcu;break;
        case 0x4fcu: /* bne 0x000004f4 */
            r[15]=0x500u;if(!(state->cpsr&0x40000000u)){r[15]=0x4f4u;}break;
        case 0x500u: /* mov pc,lr */
            r[15]=r[14];break;
        case 0x504u: /* mov r12,#0xc000 */
            r[12]=49152u;r[15]=0x508u;break;
        case 0x508u: /* mov r11,#0xa00 */
            r[11]=2560u;r[15]=0x50cu;break;
        case 0x50cu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x510u;break;
        case 0x510u: /* str r0,[r12],#0x4 */
            STORE(r[12],r[0],4);r[12]+=4u;r[15]=0x514u;break;
        case 0x514u: /* subs r11,r11,#0x1 */
            r[11]=subtract(state,r[11],1u);r[15]=0x518u;break;
        case 0x518u: /* bne 0x00000510 */
            r[15]=0x51cu;if(!(state->cpsr&0x40000000u)){r[15]=0x510u;}break;
        case 0x51cu: /* mov r12,#0xc000 */
            r[12]=49152u;r[15]=0x520u;break;
        case 0x520u: /* mov r11,#0x8 */
            r[11]=8u;r[15]=0x524u;break;
        case 0x524u: /* ldr r0,[0x608] */
            LOAD(r[0],1544u,4);r[15]=0x528u;break;
        case 0x528u: /* mov r1,#0x80 */
            r[1]=128u;r[15]=0x52cu;break;
        case 0x52cu: /* strb r1,[r12,#0x0] */
            STORE(r[12]+0u,r[1],1);r[15]=0x530u;break;
        case 0x530u: /* strb r1,[r12,#0x1] */
            STORE(r[12]+1u,r[1],1);r[15]=0x534u;break;
        case 0x534u: /* strb r1,[r12,#0x6] */
            STORE(r[12]+6u,r[1],1);r[15]=0x538u;break;
        case 0x538u: /* strb r1,[r12,#0x7] */
            STORE(r[12]+7u,r[1],1);r[15]=0x53cu;break;
        case 0x53cu: /* str r0,[r12,#0xc] */
            STORE(r[12]+12u,r[0],4);r[15]=0x540u;break;
        case 0x540u: /* str r0,[r12,#0x10] */
            STORE(r[12]+16u,r[0],4);r[15]=0x544u;break;
        case 0x544u: /* str r0,[r12,#0x18] */
            STORE(r[12]+24u,r[0],4);r[15]=0x548u;break;
        case 0x548u: /* str r0,[r12,#0x1c] */
            STORE(r[12]+28u,r[0],4);r[15]=0x54cu;break;
        case 0x54cu: /* strb r1,[r12,#0x24] */
            STORE(r[12]+36u,r[1],1);r[15]=0x550u;break;
        case 0x550u: /* strb r1,[r12,#0x26] */
            STORE(r[12]+38u,r[1],1);r[15]=0x554u;break;
        case 0x554u: /* add r12,r12,#0x80 */
            r[12]=r[12]+128u;r[15]=0x558u;break;
        case 0x558u: /* subs r11,r11,#0x1 */
            r[11]=subtract(state,r[11],1u);r[15]=0x55cu;break;
        case 0x55cu: /* bne 0x0000052c */
            r[15]=0x560u;if(!(state->cpsr&0x40000000u)){r[15]=0x52cu;}break;
        case 0x560u: /* mov r12,#0xc400 */
            r[12]=50176u;r[15]=0x564u;break;
        case 0x564u: /* ldr r11,[0x60c] */
            LOAD(r[11],1548u,4);r[15]=0x568u;break;
        case 0x568u: /* mov r0,#0x64 */
            r[0]=100u;r[15]=0x56cu;break;
        case 0x56cu: /* strb r0,[r12,#0x8] */
            STORE(r[12]+8u,r[0],1);r[15]=0x570u;break;
        case 0x570u: /* strb r0,[r12,#0xa] */
            STORE(r[12]+10u,r[0],1);r[15]=0x574u;break;
        case 0x574u: /* mov r0,#0x7f */
            r[0]=127u;r[15]=0x578u;break;
        case 0x578u: /* strb r0,[r12,#0x9] */
            STORE(r[12]+9u,r[0],1);r[15]=0x57cu;break;
        case 0x57cu: /* mov r0,#0xc0 */
            r[0]=192u;r[15]=0x580u;break;
        case 0x580u: /* strb r0,[r12,#0xb] */
            STORE(r[12]+11u,r[0],1);r[15]=0x584u;break;
        case 0x584u: /* mov r0,#0x40 */
            r[0]=64u;r[15]=0x588u;break;
        case 0x588u: /* strb r0,[r12,#0x27] */
            STORE(r[12]+39u,r[0],1);r[15]=0x58cu;break;
        case 0x58cu: /* strb r0,[r12,#0x40] */
            STORE(r[12]+64u,r[0],1);r[15]=0x590u;break;
        case 0x590u: /* mov r0,#0x20 */
            r[0]=32u;r[15]=0x594u;break;
        case 0x594u: /* strb r0,[r12,#0x32] */
            STORE(r[12]+50u,r[0],1);r[15]=0x598u;break;
        case 0x598u: /* strb r0,[r12,#0x33] */
            STORE(r[12]+51u,r[0],1);r[15]=0x59cu;break;
        case 0x59cu: /* strb r0,[r12,#0x34] */
            STORE(r[12]+52u,r[0],1);r[15]=0x5a0u;break;
        case 0x5a0u: /* strb r0,[r12,#0x35] */
            STORE(r[12]+53u,r[0],1);r[15]=0x5a4u;break;
        case 0x5a4u: /* strb r0,[r12,#0x29] */
            STORE(r[12]+41u,r[0],1);r[15]=0x5a8u;break;
        case 0x5a8u: /* strb r0,[r12,#0x2b] */
            STORE(r[12]+43u,r[0],1);r[15]=0x5acu;break;
        case 0x5acu: /* strb r0,[r12,#0x2d] */
            STORE(r[12]+45u,r[0],1);r[15]=0x5b0u;break;
        case 0x5b0u: /* strb r0,[r12,#0x2f] */
            STORE(r[12]+47u,r[0],1);r[15]=0x5b4u;break;
        case 0x5b4u: /* strb r0,[r12,#0x31] */
            STORE(r[12]+49u,r[0],1);r[15]=0x5b8u;break;
        case 0x5b8u: /* strb r0,[r12,#0x37] */
            STORE(r[12]+55u,r[0],1);r[15]=0x5bcu;break;
        case 0x5bcu: /* strb r0,[r12,#0x38] */
            STORE(r[12]+56u,r[0],1);r[15]=0x5c0u;break;
        case 0x5c0u: /* strb r0,[r12,#0x3a] */
            STORE(r[12]+58u,r[0],1);r[15]=0x5c4u;break;
        case 0x5c4u: /* strb r0,[r12,#0x3b] */
            STORE(r[12]+59u,r[0],1);r[15]=0x5c8u;break;
        case 0x5c8u: /* strb r0,[r12,#0x39] */
            STORE(r[12]+57u,r[0],1);r[15]=0x5ccu;break;
        case 0x5ccu: /* ldr r0,[0x958] */
            LOAD(r[0],2392u,4);r[15]=0x5d0u;break;
        case 0x5d0u: /* str r0,[r12,#0x10] */
            STORE(r[12]+16u,r[0],4);r[15]=0x5d4u;break;
        case 0x5d4u: /* ldr r1,[r0,#0x10] */
            LOAD(r[1],r[0]+16u,4);r[15]=0x5d8u;break;
        case 0x5d8u: /* add r1,r1,r0 */
            r[1]=r[1]+r[0];r[15]=0x5dcu;break;
        case 0x5dcu: /* ldr r1,[r1,#0x0] */
            LOAD(r[1],r[1]+0u,4);r[15]=0x5e0u;break;
        case 0x5e0u: /* add r1,r1,r0 */
            r[1]=r[1]+r[0];r[15]=0x5e4u;break;
        case 0x5e4u: /* str r1,[r12,#0x14] */
            STORE(r[12]+20u,r[1],4);r[15]=0x5e8u;break;
        case 0x5e8u: /* ldr r1,[r0,#0x18] */
            LOAD(r[1],r[0]+24u,4);r[15]=0x5ecu;break;
        case 0x5ecu: /* add r1,r1,r0 */
            r[1]=r[1]+r[0];r[15]=0x5f0u;break;
        case 0x5f0u: /* str r1,[r12,#0x18] */
            STORE(r[12]+24u,r[1],4);r[15]=0x5f4u;break;
        case 0x5f4u: /* add r12,r12,#0x48 */
            r[12]=r[12]+72u;r[15]=0x5f8u;break;
        case 0x5f8u: /* subs r11,r11,#0x1 */
            r[11]=subtract(state,r[11],1u);r[15]=0x5fcu;break;
        case 0x5fcu: /* bne 0x00000568 */
            r[15]=0x600u;if(!(state->cpsr&0x40000000u)){r[15]=0x568u;}break;
        case 0x600u: /* mov pc,lr */
            r[15]=r[14];break;
        case 0x610u: /* mov r12,#0xe800 */
            r[12]=59392u;r[15]=0x614u;break;
        case 0x614u: /* ldr r11,[0x630] */
            LOAD(r[11],1584u,4);r[15]=0x618u;break;
        case 0x618u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x61cu;break;
        case 0x61cu: /* str r0,[r12],#0x4 */
            STORE(r[12],r[0],4);r[12]+=4u;r[15]=0x620u;break;
        case 0x620u: /* subs r11,r11,#0x1 */
            r[11]=subtract(state,r[11],1u);r[15]=0x624u;break;
        case 0x624u: /* bne 0x0000061c */
            r[15]=0x628u;if(!(state->cpsr&0x40000000u)){r[15]=0x61cu;}break;
        case 0x628u: /* mov pc,lr */
            r[15]=r[14];break;
        case 0x634u: /* ldr r0,[0x95c] */
            LOAD(r[0],2396u,4);r[15]=0x638u;break;
        case 0x638u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x63cu;break;
        case 0x63cu: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x640u;break;
        case 0x640u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x644u;break;
        case 0x644u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x648u;break;
        case 0x648u: /* mov pc,lr */
            r[15]=r[14];break;
        case 0x650u: /* ldr r0,[0x8f0] */
            LOAD(r[0],2288u,4);r[15]=0x654u;break;
        case 0x654u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x658u;break;
        case 0x658u: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x65cu;break;
        case 0x65cu: /* mov r0,#0x800000 */
            r[0]=8388608u;r[15]=0x660u;break;
        case 0x660u: /* ldr r1,[0x688] */
            LOAD(r[1],1672u,4);r[15]=0x664u;break;
        case 0x664u: /* mov r2,#0x40 */
            r[2]=64u;r[15]=0x668u;break;
        case 0x668u: /* ldr r3,[0x68c] */
            LOAD(r[3],1676u,4);r[15]=0x66cu;break;
        case 0x66cu: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x670u;break;
        case 0x670u: /* str r3,[r0,#0x14] */
            STORE(r[0]+20u,r[3],4);r[15]=0x674u;break;
        case 0x674u: /* add r0,r0,#0x80 */
            r[0]=r[0]+128u;r[15]=0x678u;break;
        case 0x678u: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x67cu;break;
        case 0x67cu: /* bne 0x0000066c */
            r[15]=0x680u;if(!(state->cpsr&0x40000000u)){r[15]=0x66cu;}break;
        case 0x680u: /* mov pc,lr */
            r[15]=r[14];break;
        case 0x690u: /* ldr r2,[0x960] */
            LOAD(r[2],2400u,4);r[15]=0x694u;break;
        case 0x694u: /* ldr r1,[0x964] */
            LOAD(r[1],2404u,4);r[15]=0x698u;break;
        case 0x698u: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x69cu;break;
        case 0x69cu: /* mov r2,#0xc00 */
            r[2]=3072u;r[15]=0x6a0u;break;
        case 0x6a0u: /* ldr r1,[0x968] */
            LOAD(r[1],2408u,4);r[15]=0x6a4u;break;
        case 0x6a4u: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x6a8u;break;
        case 0x6a8u: /* ldr r1,[0x96c] */
            LOAD(r[1],2412u,4);r[15]=0x6acu;break;
        case 0x6acu: /* ldr r2,[r1,#0x0] */
            LOAD(r[2],r[1]+0u,4);r[15]=0x6b0u;break;
        case 0x6b0u: /* mov r2,r2, lsr #0xb */
            r[2]=r[2]>>11;r[15]=0x6b4u;break;
        case 0x6b4u: /* ldr r3,[0x6cc] */
            LOAD(r[3],1740u,4);r[15]=0x6b8u;break;
        case 0x6b8u: /* and r2,r2,r3 */
            r[2]=r[2]&r[3];r[15]=0x6bcu;break;
        case 0x6bcu: /* ldr r1,[0x970] */
            LOAD(r[1],2416u,4);r[15]=0x6c0u;break;
        case 0x6c0u: /* str r2,[r1,#0x0] */
            STORE(r[1]+0u,r[2],4);r[15]=0x6c4u;break;
        case 0x6c4u: /* mov pc,lr */
            r[15]=r[14];break;
        case 0x858u: /* mov r5,#0xe800 */
            r[5]=59392u;r[15]=0x85cu;break;
        case 0x85cu: /* ldrb r0,[0x8c8] */
            LOAD(r[0],2248u,1);r[15]=0x860u;break;
        case 0x860u: /* mov r1,#0x40 */
            r[1]=64u;r[15]=0x864u;break;
        case 0x864u: /* mov r2,#0xe800 */
            r[2]=59392u;r[15]=0x868u;break;
        case 0x868u: /* mla r5,r0,r1,r2 */
            r[5]=r[0]*r[1]+r[2];r[15]=0x86cu;break;
        case 0x86cu: /* ldr r8,[0x998] */
            LOAD(r[8],2456u,4);r[15]=0x870u;break;
        case 0x870u: /* ldr r0,[r8,#0x0] */
            LOAD(r[0],r[8]+0u,4);r[15]=0x874u;break;
        case 0x874u: /* mov r3,r0, lsr #0xd */
            r[3]=r[0]>>13;r[15]=0x878u;break;
        case 0x878u: /* tst r0,#0x1f00 */
            temporary=r[0]&7936u;logic_flags(state,temporary,1,7936u);r[15]=0x87cu;break;
        case 0x87cu: /* movne r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=255u;}r[15]=0x880u;break;
        case 0x880u: /* and r0,r0,#0xff */
            r[0]=r[0]&255u;r[15]=0x884u;break;
        case 0x884u: /* eor r0,r0,#0xff */
            r[0]=r[0]^255u;r[15]=0x888u;break;
        case 0x888u: /* and r3,r3,#0x3 */
            r[3]=r[3]&3u;r[15]=0x88cu;break;
        case 0x88cu: /* eors r3,r3,#0x3 */
            temporary=r[3]^3u;r[3]=temporary;logic_flags(state,temporary,0,3u);r[15]=0x890u;break;
        case 0x890u: /* movne r0,#0xff */
            if(!(state->cpsr&0x40000000u)){r[0]=255u;}r[15]=0x894u;break;
        case 0x894u: /* strb r0,[r5,#0x3] */
            STORE(r[5]+3u,r[0],1);r[15]=0x898u;break;
        case 0x898u: /* mov pc,lr */
            r[15]=r[14];break;
        case 0x89cu: /* mov r0,#0x30 */
            r[0]=48u;r[15]=0x8a0u;break;
        case 0x8a0u: /* ldrb r1,[0x8c8] */
            LOAD(r[1],2248u,1);r[15]=0x8a4u;break;
        case 0x8a4u: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x8a8u;break;
        case 0x8a8u: /* subs r0,r0,r1 */
            r[0]=subtract(state,r[0],r[1]);r[15]=0x8acu;break;
        case 0x8acu: /* moveq r1,r0 */
            if(state->cpsr&0x40000000u){r[1]=r[0];}r[15]=0x8b0u;break;
        case 0x8b0u: /* strb r1,[0x8c8] */
            STORE(2248u,r[1],1);r[15]=0x8b4u;break;
        case 0x8b4u: /* ldr r8,[0x99c] */
            LOAD(r[8],2460u,4);r[15]=0x8b8u;break;
        case 0x8b8u: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x8bcu;break;
        case 0x8bcu: /* strb r1,[r8,#0x0] */
            STORE(r[8]+0u,r[1],1);r[15]=0x8c0u;break;
        case 0x8c0u: /* mov pc,lr */
            r[15]=r[14];break;
        case 0x8ccu: /* ldr r0,[0x9a0] */
            LOAD(r[0],2464u,4);r[15]=0x8d0u;break;
        case 0x8d0u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x8d4u;break;
        case 0x8d4u: /* mov r2,#0x12 */
            r[2]=18u;r[15]=0x8d8u;break;
        case 0x8d8u: /* str r1,[r0],#0x4 */
            STORE(r[0],r[1],4);r[0]+=4u;r[15]=0x8dcu;break;
        case 0x8dcu: /* subs r2,r2,#0x1 */
            r[2]=subtract(state,r[2],1u);r[15]=0x8e0u;break;
        case 0x8e0u: /* bne 0x000008d8 */
            r[15]=0x8e4u;if(!(state->cpsr&0x40000000u)){r[15]=0x8d8u;}break;
        case 0x8e4u: /* mov pc,lr */
            r[15]=r[14];break;
        default: {
            int handled=0;
            int result=sa_arm_sound_reset_step(ram,state,driver,user,read,write,&handled);
            if(handled){if(result)return result;break;}
            result=sa_arm_sound_bank_select_step(ram,state,driver,user,read,write,&handled);
            if(handled){if(result)return result;break;}
            result=sa_arm_sound_command_step(ram,state,driver,user,read,write,&handled);
            if(handled){if(result)return result;break;}
            result=sa_arm_sound_control_step(ram,state,driver,user,read,write,&handled);
            if(handled){if(result)return result;break;}
            result=sa_arm_sound_note_step(ram,state,driver,user,read,write,&handled);
            if(handled){if(result)return result;break;}
            result=sa_arm_sound_status_step(ram,state,driver,user,read,write,&handled);
            if(handled){if(result)return result;break;}
            result=sa_arm_sound_update_step(ram,state,driver,user,read,write,&handled);
            if(handled){if(result)return result;break;}
            result=sa_arm_sound_snapshot_step(ram,state,driver,user,read,write,&handled);
            if(handled){if(result)return result;break;}
            result=sa_arm_sound_ramp_step(ram,state,driver,user,read,write,&handled);
            if(handled){if(result)return result;break;}
            result=sa_arm_sound_effect_step(ram,state,driver,user,read,write,&handled);
            if(handled){if(result)return result;break;}
            result=sa_arm_sound_stream_step(ram,state,driver,user,read,write,&handled);
            if(handled){if(result)return result;break;}
            result=sa_arm_sound_sequence_step(ram,state,driver,user,read,write,&handled);
            if(handled){if(result)return result;break;}
            result=sa_arm_sound_dispatch_step(ram,state,driver,user,read,write,&handled);
            if(handled){if(result)return result;break;}
            switch(r[15]){
            case 0x1360u:
            case 0x1368u:
            case 0x1818u:
            case 0x1820u:
            case 0xad4u:
            case 0x9a4u:
            case 0x48e8u:
            case 0x48f0u:
            case 0x5d54u:
            case 0x5d5cu:
            case 0x73b4u:
            case 0x73bcu:
            case 0x7700u:
            case 0x7708u:
                if(!service)return -8;
                {int rc=service(user,ram,state,r[15]);if(rc)return rc;}break;
            default:return -8;
            }
            break;
        }
    }
 }
}

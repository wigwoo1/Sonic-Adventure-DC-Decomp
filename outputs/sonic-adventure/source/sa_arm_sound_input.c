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
static void logic_flags(struct sa_arm_state *s,uint32_t v,int rotated,uint32_t immediate){
 uint32_t flags=(v&0x80000000u)|(!v?0x40000000u:0);
 s->cpsr=(s->cpsr&0x3fffffffu)|flags;
 if(rotated)s->cpsr=(s->cpsr&~0x20000000u)|((immediate>>31)<<29);
}
#include "arm_sound_memory.h"
#define LOAD(destination,address,length) do {uint32_t v;int rc=load(ram,user,read,address,length,&v);if(rc)return rc;destination=v;}while(0)
#define STORE(address,value,length) do {int rc=store(ram,user,write,address,value,length);if(rc)return rc;}while(0)
int sa_arm_sound_input_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
        case 0x1598u: /* ldr r0,[0x1814] */
            LOAD(r[0],6164u,4);r[15]=0x159cu;return 0;
        case 0x159cu: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x15a0u;return 0;
        case 0x15a0u: /* mov r2,#0x100 */
            r[2]=256u;r[15]=0x15a4u;return 0;
        case 0x15a4u: /* tst r1,r2 */
            temporary=r[1]&r[2];logic_flags(state,temporary,0,r[2]);r[15]=0x15a8u;return 0;
        case 0x15a8u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x15acu;}return 0;
        case 0x15acu: /* and r1,r1,#0xff */
            r[1]=r[1]&255u;r[15]=0x15b0u;return 0;
        case 0x15b0u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x15b4u;return 0;
        case 0x15b4u: /* bne 0x000015dc */
            if(!(state->cpsr&0x40000000u)){r[15]=5596u;}else{r[15]=0x15b8u;}return 0;
        case 0x15b8u: /* ldr r0,[0x15f0] */
            LOAD(r[0],5616u,4);r[15]=0x15bcu;return 0;
        case 0x15bcu: /* ldr r2,[r0,#0x0] */
            LOAD(r[2],r[0]+0u,4);r[15]=0x15c0u;return 0;
        case 0x15c0u: /* and r2,r2,#0xff */
            r[2]=r[2]&255u;r[15]=0x15c4u;return 0;
        case 0x15c4u: /* bne 0x00001768 */
            if(!(state->cpsr&0x40000000u)){r[15]=5992u;}else{r[15]=0x15c8u;}return 0;
        case 0x15c8u: /* ldr r0,[0x15f4] */
            LOAD(r[0],5620u,4);r[15]=0x15ccu;return 0;
        case 0x15ccu: /* ldr r3,[0x15f8] */
            LOAD(r[3],5624u,4);r[15]=0x15d0u;return 0;
        case 0x15d0u: /* ands r0,r0,r3 */
            temporary=r[0]&r[3];r[0]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x15d4u;return 0;
        case 0x15d4u: /* beq 0x00001598 */
            if(state->cpsr&0x40000000u){r[15]=5528u;}else{r[15]=0x15d8u;}return 0;
        case 0x15d8u: /* mov pc,r0 */
            r[15]=r[0];return 0;
        case 0x15dcu: /* and r0,r1,#0x70 */
            r[0]=r[1]&112u;r[15]=0x15e0u;return 0;
        case 0x15e0u: /* mov r0,r0, lsr #0x2 */
            r[0]=(r[0]>>2);r[15]=0x15e4u;return 0;
        case 0x15e4u: /* adr r2,0x1600 */
            r[2]=5632u;r[15]=0x15e8u;return 0;
        case 0x15e8u: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x15ecu;return 0;
        case 0x15ecu: /* mov pc,r2 */
            r[15]=r[2];return 0;
        case 0x1600u: /* b 0x0000162c */
            r[15]=5676u;return 0;
        case 0x1604u: /* b 0x0000162c */
            r[15]=5676u;return 0;
        case 0x1608u: /* b 0x0000162c */
            r[15]=5676u;return 0;
        case 0x160cu: /* b 0x0000162c */
            r[15]=5676u;return 0;
        case 0x1610u: /* b 0x00001670 */
            r[15]=5744u;return 0;
        case 0x1614u: /* b 0x00001670 */
            r[15]=5744u;return 0;
        case 0x1618u: /* b 0x0000162c */
            r[15]=5676u;return 0;
        case 0x161cu: /* b 0x000016f8 */
            r[15]=5880u;return 0;
        case 0x162cu: /* strb r1,[0x15fc] */
            STORE(5628u,r[1],1);r[15]=0x1630u;return 0;
        case 0x1630u: /* ldr r0,[0x1620] */
            LOAD(r[0],5664u,4);r[15]=0x1634u;return 0;
        case 0x1634u: /* str r0,[0x15f4] */
            STORE(5620u,r[0],4);r[15]=0x1638u;return 0;
        case 0x1638u: /* b 0x00001598 */
            r[15]=5528u;return 0;
        case 0x163cu: /* strb r1,[0x15fd] */
            STORE(5629u,r[1],1);r[15]=0x1640u;return 0;
        case 0x1640u: /* ldr r0,[0x1624] */
            LOAD(r[0],5668u,4);r[15]=0x1644u;return 0;
        case 0x1644u: /* str r0,[0x15f4] */
            STORE(5620u,r[0],4);r[15]=0x1648u;return 0;
        case 0x1648u: /* b 0x00001598 */
            r[15]=5528u;return 0;
        case 0x164cu: /* mov r2,r1 */
            r[2]=r[1];r[15]=0x1650u;return 0;
        case 0x1650u: /* ldrb r0,[0x15fc] */
            LOAD(r[0],5628u,1);r[15]=0x1654u;return 0;
        case 0x1654u: /* ldrb r1,[0x15fd] */
            LOAD(r[1],5629u,1);r[15]=0x1658u;return 0;
        case 0x1658u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x165cu;return 0;
        case 0x165cu: /* bl 0x000016a0 */
            r[14]=0x1660u;r[15]=5792u;return 0;
        case 0x1660u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x1664u;return 0;
        case 0x1664u: /* ldr r0,[0x1620] */
            LOAD(r[0],5664u,4);r[15]=0x1668u;return 0;
        case 0x1668u: /* str r0,[0x15f4] */
            STORE(5620u,r[0],4);r[15]=0x166cu;return 0;
        case 0x166cu: /* b 0x00001598 */
            r[15]=5528u;return 0;
        case 0x1670u: /* strb r1,[0x15fc] */
            STORE(5628u,r[1],1);r[15]=0x1674u;return 0;
        case 0x1674u: /* ldr r0,[0x1628] */
            LOAD(r[0],5672u,4);r[15]=0x1678u;return 0;
        case 0x1678u: /* str r0,[0x15f4] */
            STORE(5620u,r[0],4);r[15]=0x167cu;return 0;
        case 0x167cu: /* b 0x00001598 */
            r[15]=5528u;return 0;
        case 0x1680u: /* ldrb r0,[0x15fc] */
            LOAD(r[0],5628u,1);r[15]=0x1684u;return 0;
        case 0x1684u: /* mov r2,#0x0 */
            r[2]=0u;r[15]=0x1688u;return 0;
        case 0x1688u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x168cu;return 0;
        case 0x168cu: /* bl 0x000016a0 */
            r[14]=0x1690u;r[15]=5792u;return 0;
        case 0x1690u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x1694u;return 0;
        case 0x1694u: /* ldr r0,[0x1628] */
            LOAD(r[0],5672u,4);r[15]=0x1698u;return 0;
        case 0x1698u: /* str r0,[0x15f4] */
            STORE(5620u,r[0],4);r[15]=0x169cu;return 0;
        case 0x169cu: /* b 0x00001598 */
            r[15]=5528u;return 0;
        case 0x16a0u: /* mov r3,#0xb000 */
            r[3]=45056u;r[15]=0x16a4u;return 0;
        case 0x16a4u: /* mov r4,#0x13000 */
            r[4]=77824u;r[15]=0x16a8u;return 0;
        case 0x16a8u: /* ldr r5,[r4,#0x0] */
            LOAD(r[5],r[4]+0u,4);r[15]=0x16acu;return 0;
        case 0x16acu: /* add r3,r3,r5 */
            r[3]=r[3]+r[5];r[15]=0x16b0u;return 0;
        case 0x16b0u: /* mov r6,r0, lsr #0x4 */
            r[6]=(r[0]>>4);r[15]=0x16b4u;return 0;
        case 0x16b4u: /* and r6,r6,#0x7 */
            r[6]=r[6]&7u;r[15]=0x16b8u;return 0;
        case 0x16b8u: /* orr r6,r6,#0xf8 */
            r[6]=r[6]|248u;r[15]=0x16bcu;return 0;
        case 0x16bcu: /* mov r6,r6, lsl #0x18 */
            r[6]=(r[6]<<24);r[15]=0x16c0u;return 0;
        case 0x16c0u: /* and r0,r0,#0xf */
            r[0]=r[0]&15u;r[15]=0x16c4u;return 0;
        case 0x16c4u: /* orr r0,r0,#0x70 */
            r[0]=r[0]|112u;r[15]=0x16c8u;return 0;
        case 0x16c8u: /* and r1,r1,#0xff */
            r[1]=r[1]&255u;r[15]=0x16ccu;return 0;
        case 0x16ccu: /* and r2,r2,#0xff */
            r[2]=r[2]&255u;r[15]=0x16d0u;return 0;
        case 0x16d0u: /* orr r0,r6,r0, lsl #0x10 */
            r[0]=r[6]|(r[0]<<16);r[15]=0x16d4u;return 0;
        case 0x16d4u: /* orr r0,r0,r1, lsl #0x8 */
            r[0]=r[0]|(r[1]<<8);r[15]=0x16d8u;return 0;
        case 0x16d8u: /* orr r0,r0,r2 */
            r[0]=r[0]|r[2];r[15]=0x16dcu;return 0;
        case 0x16dcu: /* str r0,[r3,#0x0] */
            STORE(r[3]+0u,r[0],4);r[15]=0x16e0u;return 0;
        case 0x16e0u: /* add r5,r5,#0x4 */
            r[5]=r[5]+4u;r[15]=0x16e4u;return 0;
        case 0x16e4u: /* ldr r0,[0x16f4] */
            LOAD(r[0],5876u,4);r[15]=0x16e8u;return 0;
        case 0x16e8u: /* and r5,r5,r0 */
            r[5]=r[5]&r[0];r[15]=0x16ecu;return 0;
        case 0x16ecu: /* str r5,[r4,#0x0] */
            STORE(r[4]+0u,r[5],4);r[15]=0x16f0u;return 0;
        case 0x16f0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x16f8u: /* adr r2,0x15f4 */
            r[2]=5620u;r[15]=0x16fcu;return 0;
        case 0x16fcu: /* mov r3,#0x0 */
            r[3]=0u;r[15]=0x1700u;return 0;
        case 0x1700u: /* str r3,[r2,#0x0] */
            STORE(r[2]+0u,r[3],4);r[15]=0x1704u;return 0;
        case 0x1704u: /* and r0,r1,#0xf */
            r[0]=r[1]&15u;r[15]=0x1708u;return 0;
        case 0x1708u: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x170cu;return 0;
        case 0x170cu: /* adr r1,0x1718 */
            r[1]=5912u;r[15]=0x1710u;return 0;
        case 0x1710u: /* add r1,r1,r0 */
            r[1]=r[1]+r[0];r[15]=0x1714u;return 0;
        case 0x1714u: /* mov pc,r1 */
            r[15]=r[1];return 0;
        case 0x1718u: /* b 0x00001760 */
            r[15]=5984u;return 0;
        case 0x171cu: /* b 0x00001758 */
            r[15]=5976u;return 0;
        case 0x1720u: /* b 0x00001758 */
            r[15]=5976u;return 0;
        case 0x1724u: /* b 0x00001758 */
            r[15]=5976u;return 0;
        case 0x1728u: /* b 0x00001758 */
            r[15]=5976u;return 0;
        case 0x172cu: /* b 0x00001758 */
            r[15]=5976u;return 0;
        case 0x1730u: /* b 0x00001758 */
            r[15]=5976u;return 0;
        case 0x1734u: /* b 0x00001764 */
            r[15]=5988u;return 0;
        case 0x1738u: /* b 0x00001758 */
            r[15]=5976u;return 0;
        case 0x173cu: /* b 0x00001758 */
            r[15]=5976u;return 0;
        case 0x1740u: /* b 0x00001758 */
            r[15]=5976u;return 0;
        case 0x1744u: /* b 0x00001758 */
            r[15]=5976u;return 0;
        case 0x1748u: /* b 0x00001758 */
            r[15]=5976u;return 0;
        case 0x174cu: /* b 0x00001758 */
            r[15]=5976u;return 0;
        case 0x1750u: /* b 0x00001758 */
            r[15]=5976u;return 0;
        case 0x1754u: /* b 0x0000175c */
            r[15]=5980u;return 0;
        case 0x1758u: /* b 0x00001598 */
            r[15]=5528u;return 0;
        case 0x175cu: /* b 0x00001598 */
            r[15]=5528u;return 0;
        case 0x1760u: /* b 0x00001598 */
            r[15]=5528u;return 0;
        case 0x1764u: /* b 0x00001598 */
            r[15]=5528u;return 0;
        case 0x1768u: /* b 0x00001598 */
            r[15]=5528u;return 0;
 default:*handled=0;return 0;
 }}else{switch(r[15]){
        case 0x15a4u: /* ldr r0,[0x181c] */
            LOAD(r[0],6172u,4);r[15]=0x15a8u;return 0;
        case 0x15a8u: /* ldr r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,4);r[15]=0x15acu;return 0;
        case 0x15acu: /* mov r2,#0x100 */
            r[2]=256u;r[15]=0x15b0u;return 0;
        case 0x15b0u: /* tst r1,r2 */
            temporary=r[1]&r[2];logic_flags(state,temporary,0,r[2]);r[15]=0x15b4u;return 0;
        case 0x15b4u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x15b8u;}return 0;
        case 0x15b8u: /* and r1,r1,#0xff */
            r[1]=r[1]&255u;r[15]=0x15bcu;return 0;
        case 0x15bcu: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x15c0u;return 0;
        case 0x15c0u: /* bne 0x000015e8 */
            if(!(state->cpsr&0x40000000u)){r[15]=5608u;}else{r[15]=0x15c4u;}return 0;
        case 0x15c4u: /* ldr r0,[0x15fc] */
            LOAD(r[0],5628u,4);r[15]=0x15c8u;return 0;
        case 0x15c8u: /* ldr r2,[r0,#0x0] */
            LOAD(r[2],r[0]+0u,4);r[15]=0x15ccu;return 0;
        case 0x15ccu: /* and r2,r2,#0xff */
            r[2]=r[2]&255u;r[15]=0x15d0u;return 0;
        case 0x15d0u: /* bne 0x00001774 */
            if(!(state->cpsr&0x40000000u)){r[15]=6004u;}else{r[15]=0x15d4u;}return 0;
        case 0x15d4u: /* ldr r0,[0x1600] */
            LOAD(r[0],5632u,4);r[15]=0x15d8u;return 0;
        case 0x15d8u: /* ldr r3,[0x1604] */
            LOAD(r[3],5636u,4);r[15]=0x15dcu;return 0;
        case 0x15dcu: /* ands r0,r0,r3 */
            temporary=r[0]&r[3];r[0]=temporary;logic_flags(state,temporary,0,r[3]);r[15]=0x15e0u;return 0;
        case 0x15e0u: /* beq 0x000015a4 */
            if(state->cpsr&0x40000000u){r[15]=5540u;}else{r[15]=0x15e4u;}return 0;
        case 0x15e4u: /* mov pc,r0 */
            r[15]=r[0];return 0;
        case 0x15e8u: /* and r0,r1,#0x70 */
            r[0]=r[1]&112u;r[15]=0x15ecu;return 0;
        case 0x15ecu: /* mov r0,r0, lsr #0x2 */
            r[0]=(r[0]>>2);r[15]=0x15f0u;return 0;
        case 0x15f0u: /* adr r2,0x160c */
            r[2]=5644u;r[15]=0x15f4u;return 0;
        case 0x15f4u: /* add r2,r2,r0 */
            r[2]=r[2]+r[0];r[15]=0x15f8u;return 0;
        case 0x15f8u: /* mov pc,r2 */
            r[15]=r[2];return 0;
        case 0x160cu: /* b 0x00001638 */
            r[15]=5688u;return 0;
        case 0x1610u: /* b 0x00001638 */
            r[15]=5688u;return 0;
        case 0x1614u: /* b 0x00001638 */
            r[15]=5688u;return 0;
        case 0x1618u: /* b 0x00001638 */
            r[15]=5688u;return 0;
        case 0x161cu: /* b 0x0000167c */
            r[15]=5756u;return 0;
        case 0x1620u: /* b 0x0000167c */
            r[15]=5756u;return 0;
        case 0x1624u: /* b 0x00001638 */
            r[15]=5688u;return 0;
        case 0x1628u: /* b 0x00001704 */
            r[15]=5892u;return 0;
        case 0x1638u: /* strb r1,[0x1608] */
            STORE(5640u,r[1],1);r[15]=0x163cu;return 0;
        case 0x163cu: /* ldr r0,[0x162c] */
            LOAD(r[0],5676u,4);r[15]=0x1640u;return 0;
        case 0x1640u: /* str r0,[0x1600] */
            STORE(5632u,r[0],4);r[15]=0x1644u;return 0;
        case 0x1644u: /* b 0x000015a4 */
            r[15]=5540u;return 0;
        case 0x1648u: /* strb r1,[0x1609] */
            STORE(5641u,r[1],1);r[15]=0x164cu;return 0;
        case 0x164cu: /* ldr r0,[0x1630] */
            LOAD(r[0],5680u,4);r[15]=0x1650u;return 0;
        case 0x1650u: /* str r0,[0x1600] */
            STORE(5632u,r[0],4);r[15]=0x1654u;return 0;
        case 0x1654u: /* b 0x000015a4 */
            r[15]=5540u;return 0;
        case 0x1658u: /* mov r2,r1 */
            r[2]=r[1];r[15]=0x165cu;return 0;
        case 0x165cu: /* ldrb r0,[0x1608] */
            LOAD(r[0],5640u,1);r[15]=0x1660u;return 0;
        case 0x1660u: /* ldrb r1,[0x1609] */
            LOAD(r[1],5641u,1);r[15]=0x1664u;return 0;
        case 0x1664u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x1668u;return 0;
        case 0x1668u: /* bl 0x000016ac */
            r[14]=0x166cu;r[15]=5804u;return 0;
        case 0x166cu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x1670u;return 0;
        case 0x1670u: /* ldr r0,[0x162c] */
            LOAD(r[0],5676u,4);r[15]=0x1674u;return 0;
        case 0x1674u: /* str r0,[0x1600] */
            STORE(5632u,r[0],4);r[15]=0x1678u;return 0;
        case 0x1678u: /* b 0x000015a4 */
            r[15]=5540u;return 0;
        case 0x167cu: /* strb r1,[0x1608] */
            STORE(5640u,r[1],1);r[15]=0x1680u;return 0;
        case 0x1680u: /* ldr r0,[0x1634] */
            LOAD(r[0],5684u,4);r[15]=0x1684u;return 0;
        case 0x1684u: /* str r0,[0x1600] */
            STORE(5632u,r[0],4);r[15]=0x1688u;return 0;
        case 0x1688u: /* b 0x000015a4 */
            r[15]=5540u;return 0;
        case 0x168cu: /* ldrb r0,[0x1608] */
            LOAD(r[0],5640u,1);r[15]=0x1690u;return 0;
        case 0x1690u: /* mov r2,#0x0 */
            r[2]=0u;r[15]=0x1694u;return 0;
        case 0x1694u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x1698u;return 0;
        case 0x1698u: /* bl 0x000016ac */
            r[14]=0x169cu;r[15]=5804u;return 0;
        case 0x169cu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x16a0u;return 0;
        case 0x16a0u: /* ldr r0,[0x1634] */
            LOAD(r[0],5684u,4);r[15]=0x16a4u;return 0;
        case 0x16a4u: /* str r0,[0x1600] */
            STORE(5632u,r[0],4);r[15]=0x16a8u;return 0;
        case 0x16a8u: /* b 0x000015a4 */
            r[15]=5540u;return 0;
        case 0x16acu: /* mov r3,#0xb000 */
            r[3]=45056u;r[15]=0x16b0u;return 0;
        case 0x16b0u: /* mov r4,#0x13000 */
            r[4]=77824u;r[15]=0x16b4u;return 0;
        case 0x16b4u: /* ldr r5,[r4,#0x0] */
            LOAD(r[5],r[4]+0u,4);r[15]=0x16b8u;return 0;
        case 0x16b8u: /* add r3,r3,r5 */
            r[3]=r[3]+r[5];r[15]=0x16bcu;return 0;
        case 0x16bcu: /* mov r6,r0, lsr #0x4 */
            r[6]=(r[0]>>4);r[15]=0x16c0u;return 0;
        case 0x16c0u: /* and r6,r6,#0x7 */
            r[6]=r[6]&7u;r[15]=0x16c4u;return 0;
        case 0x16c4u: /* orr r6,r6,#0xf8 */
            r[6]=r[6]|248u;r[15]=0x16c8u;return 0;
        case 0x16c8u: /* mov r6,r6, lsl #0x18 */
            r[6]=(r[6]<<24);r[15]=0x16ccu;return 0;
        case 0x16ccu: /* and r0,r0,#0xf */
            r[0]=r[0]&15u;r[15]=0x16d0u;return 0;
        case 0x16d0u: /* orr r0,r0,#0x70 */
            r[0]=r[0]|112u;r[15]=0x16d4u;return 0;
        case 0x16d4u: /* and r1,r1,#0xff */
            r[1]=r[1]&255u;r[15]=0x16d8u;return 0;
        case 0x16d8u: /* and r2,r2,#0xff */
            r[2]=r[2]&255u;r[15]=0x16dcu;return 0;
        case 0x16dcu: /* orr r0,r6,r0, lsl #0x10 */
            r[0]=r[6]|(r[0]<<16);r[15]=0x16e0u;return 0;
        case 0x16e0u: /* orr r0,r0,r1, lsl #0x8 */
            r[0]=r[0]|(r[1]<<8);r[15]=0x16e4u;return 0;
        case 0x16e4u: /* orr r0,r0,r2 */
            r[0]=r[0]|r[2];r[15]=0x16e8u;return 0;
        case 0x16e8u: /* str r0,[r3,#0x0] */
            STORE(r[3]+0u,r[0],4);r[15]=0x16ecu;return 0;
        case 0x16ecu: /* add r5,r5,#0x4 */
            r[5]=r[5]+4u;r[15]=0x16f0u;return 0;
        case 0x16f0u: /* ldr r0,[0x1700] */
            LOAD(r[0],5888u,4);r[15]=0x16f4u;return 0;
        case 0x16f4u: /* and r5,r5,r0 */
            r[5]=r[5]&r[0];r[15]=0x16f8u;return 0;
        case 0x16f8u: /* str r5,[r4,#0x0] */
            STORE(r[4]+0u,r[5],4);r[15]=0x16fcu;return 0;
        case 0x16fcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1704u: /* adr r2,0x1600 */
            r[2]=5632u;r[15]=0x1708u;return 0;
        case 0x1708u: /* mov r3,#0x0 */
            r[3]=0u;r[15]=0x170cu;return 0;
        case 0x170cu: /* str r3,[r2,#0x0] */
            STORE(r[2]+0u,r[3],4);r[15]=0x1710u;return 0;
        case 0x1710u: /* and r0,r1,#0xf */
            r[0]=r[1]&15u;r[15]=0x1714u;return 0;
        case 0x1714u: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x1718u;return 0;
        case 0x1718u: /* adr r1,0x1724 */
            r[1]=5924u;r[15]=0x171cu;return 0;
        case 0x171cu: /* add r1,r1,r0 */
            r[1]=r[1]+r[0];r[15]=0x1720u;return 0;
        case 0x1720u: /* mov pc,r1 */
            r[15]=r[1];return 0;
        case 0x1724u: /* b 0x0000176c */
            r[15]=5996u;return 0;
        case 0x1728u: /* b 0x00001764 */
            r[15]=5988u;return 0;
        case 0x172cu: /* b 0x00001764 */
            r[15]=5988u;return 0;
        case 0x1730u: /* b 0x00001764 */
            r[15]=5988u;return 0;
        case 0x1734u: /* b 0x00001764 */
            r[15]=5988u;return 0;
        case 0x1738u: /* b 0x00001764 */
            r[15]=5988u;return 0;
        case 0x173cu: /* b 0x00001764 */
            r[15]=5988u;return 0;
        case 0x1740u: /* b 0x00001770 */
            r[15]=6000u;return 0;
        case 0x1744u: /* b 0x00001764 */
            r[15]=5988u;return 0;
        case 0x1748u: /* b 0x00001764 */
            r[15]=5988u;return 0;
        case 0x174cu: /* b 0x00001764 */
            r[15]=5988u;return 0;
        case 0x1750u: /* b 0x00001764 */
            r[15]=5988u;return 0;
        case 0x1754u: /* b 0x00001764 */
            r[15]=5988u;return 0;
        case 0x1758u: /* b 0x00001764 */
            r[15]=5988u;return 0;
        case 0x175cu: /* b 0x00001764 */
            r[15]=5988u;return 0;
        case 0x1760u: /* b 0x00001768 */
            r[15]=5992u;return 0;
        case 0x1764u: /* b 0x000015a4 */
            r[15]=5540u;return 0;
        case 0x1768u: /* b 0x000015a4 */
            r[15]=5540u;return 0;
        case 0x176cu: /* b 0x000015a4 */
            r[15]=5540u;return 0;
        case 0x1770u: /* b 0x000015a4 */
            r[15]=5540u;return 0;
        case 0x1774u: /* b 0x000015a4 */
            r[15]=5540u;return 0;
 default:*handled=0;return 0;
 }}
}

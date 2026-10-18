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
int sa_arm_sound_control_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
        case 0x1818u: /* add r5,r5,r6 */
            r[5]=r[5]+r[6];r[15]=0x181cu;return 0;
        case 0x181cu: /* mov r0,r1, lsr #0x6 */
            r[0]=(r[1]>>6);r[15]=0x1820u;return 0;
        case 0x1820u: /* and r0,r0,#0x1fc */
            r[0]=r[0]&508u;r[15]=0x1824u;return 0;
        case 0x1824u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1828u;return 0;
        case 0x1828u: /* adr r7,0x1834 */
            r[7]=6196u;r[15]=0x182cu;return 0;
        case 0x182cu: /* add r7,r7,r0 */
            r[7]=r[7]+r[0];r[15]=0x1830u;return 0;
        case 0x1830u: /* mov pc,r7 */
            r[15]=r[7];return 0;
        case 0x1834u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1838u: /* b 0x00001a38 */
            r[15]=6712u;return 0;
        case 0x183cu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1840u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1844u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1848u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x184cu: /* b 0x00001adc */
            r[15]=6876u;return 0;
        case 0x1850u: /* b 0x00001b54 */
            r[15]=6996u;return 0;
        case 0x1854u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1858u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x185cu: /* b 0x00001c54 */
            r[15]=7252u;return 0;
        case 0x1860u: /* b 0x00001ce0 */
            r[15]=7392u;return 0;
        case 0x1864u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1868u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x186cu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1870u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1874u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1878u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x187cu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1880u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1884u: /* b 0x00001cf0 */
            r[15]=7408u;return 0;
        case 0x1888u: /* b 0x00001d20 */
            r[15]=7456u;return 0;
        case 0x188cu: /* b 0x00001d50 */
            r[15]=7504u;return 0;
        case 0x1890u: /* b 0x00001d80 */
            r[15]=7552u;return 0;
        case 0x1894u: /* b 0x00001e40 */
            r[15]=7744u;return 0;
        case 0x1898u: /* b 0x00001e70 */
            r[15]=7792u;return 0;
        case 0x189cu: /* b 0x00001e88 */
            r[15]=7816u;return 0;
        case 0x18a0u: /* b 0x00001ea0 */
            r[15]=7840u;return 0;
        case 0x18a4u: /* b 0x00001eb8 */
            r[15]=7864u;return 0;
        case 0x18a8u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18acu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18b0u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18b4u: /* b 0x00001ed0 */
            r[15]=7888u;return 0;
        case 0x18b8u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18bcu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18c0u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18c4u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18c8u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18ccu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18d0u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18d4u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18d8u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18dcu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18e0u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18e4u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18e8u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18ecu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18f0u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18f4u: /* b 0x00001f5c */
            r[15]=8028u;return 0;
        case 0x18f8u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x18fcu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1900u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1904u: /* b 0x00001f78 */
            r[15]=8056u;return 0;
        case 0x1908u: /* b 0x00001f98 */
            r[15]=8088u;return 0;
        case 0x190cu: /* b 0x00001fb8 */
            r[15]=8120u;return 0;
        case 0x1910u: /* b 0x00001fd8 */
            r[15]=8152u;return 0;
        case 0x1914u: /* b 0x00002088 */
            r[15]=8328u;return 0;
        case 0x1918u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x191cu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1920u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1924u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1928u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x192cu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1930u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1934u: /* b 0x000020a8 */
            r[15]=8360u;return 0;
        case 0x1938u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x193cu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1940u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1944u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1948u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x194cu: /* b 0x00002184 */
            r[15]=8580u;return 0;
        case 0x1950u: /* b 0x00002190 */
            r[15]=8592u;return 0;
        case 0x1954u: /* b 0x00002210 */
            r[15]=8720u;return 0;
        case 0x1958u: /* b 0x00002220 */
            r[15]=8736u;return 0;
        case 0x195cu: /* b 0x00002230 */
            r[15]=8752u;return 0;
        case 0x1960u: /* b 0x00002280 */
            r[15]=8832u;return 0;
        case 0x1964u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1968u: /* b 0x00002410 */
            r[15]=9232u;return 0;
        case 0x196cu: /* b 0x00002448 */
            r[15]=9288u;return 0;
        case 0x1970u: /* b 0x00002474 */
            r[15]=9332u;return 0;
        case 0x1974u: /* b 0x000024b0 */
            r[15]=9392u;return 0;
        case 0x1978u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x197cu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1980u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1984u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1988u: /* b 0x00002190 */
            r[15]=8592u;return 0;
        case 0x198cu: /* b 0x00002210 */
            r[15]=8720u;return 0;
        case 0x1990u: /* b 0x00002220 */
            r[15]=8736u;return 0;
        case 0x1994u: /* b 0x000023e0 */
            r[15]=9184u;return 0;
        case 0x1998u: /* b 0x000023f0 */
            r[15]=9200u;return 0;
        case 0x199cu: /* b 0x00002400 */
            r[15]=9216u;return 0;
        case 0x19a0u: /* b 0x00002520 */
            r[15]=9504u;return 0;
        case 0x19a4u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x19a8u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x19acu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x19b0u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x19b4u: /* b 0x00002660 */
            r[15]=9824u;return 0;
        case 0x19b8u: /* b 0x00002694 */
            r[15]=9876u;return 0;
        case 0x19bcu: /* b 0x000026d8 */
            r[15]=9944u;return 0;
        case 0x19c0u: /* b 0x000026f8 */
            r[15]=9976u;return 0;
        case 0x19c4u: /* b 0x00002718 */
            r[15]=10008u;return 0;
        case 0x19c8u: /* b 0x00002738 */
            r[15]=10040u;return 0;
        case 0x19ccu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x19d0u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x19d4u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x19d8u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x19dcu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x19e0u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x19e4u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x19e8u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x19ecu: /* b 0x00002298 */
            r[15]=8856u;return 0;
        case 0x19f0u: /* b 0x000022ac */
            r[15]=8876u;return 0;
        case 0x19f4u: /* b 0x000022c4 */
            r[15]=8900u;return 0;
        case 0x19f8u: /* b 0x000022dc */
            r[15]=8924u;return 0;
        case 0x19fcu: /* b 0x000022f4 */
            r[15]=8948u;return 0;
        case 0x1a00u: /* b 0x0000230c */
            r[15]=8972u;return 0;
        case 0x1a04u: /* b 0x00002324 */
            r[15]=8996u;return 0;
        case 0x1a08u: /* b 0x0000233c */
            r[15]=9020u;return 0;
        case 0x1a0cu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1a10u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1a14u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1a18u: /* b 0x00002758 */
            r[15]=10072u;return 0;
        case 0x1a1cu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1a20u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1a24u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1a28u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1a2cu: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1a30u: /* b 0x00001a34 */
            r[15]=6708u;return 0;
        case 0x1a34u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1a38u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x1a3cu;return 0;
        case 0x1a3cu: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x1a40u;return 0;
        case 0x1a40u: /* mov r11,#0x800000 */
            r[11]=8388608u;r[15]=0x1a44u;return 0;
        case 0x1a44u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1a48u;return 0;
        case 0x1a48u: /* strb r1,[r5,#0x2] */
            STORE(r[5]+2u,r[1],1);r[15]=0x1a4cu;return 0;
        case 0x1a4cu: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x1a50u;return 0;
        case 0x1a50u: /* ldrb r8,[r6,#0x4] */
            LOAD(r[8],r[6]+4u,1);r[15]=0x1a54u;return 0;
        case 0x1a54u: /* cmp r8,r3 */
            (void)subtract(state,r[8],r[3]);r[15]=0x1a58u;return 0;
        case 0x1a58u: /* bne 0x00001ac0 */
            if(!(state->cpsr&0x40000000u)){r[15]=6848u;}else{r[15]=0x1a5cu;}return 0;
        case 0x1a5cu: /* ldrb r8,[r6,#0x5] */
            LOAD(r[8],r[6]+5u,1);r[15]=0x1a60u;return 0;
        case 0x1a60u: /* cmp r8,r2 */
            (void)subtract(state,r[8],r[2]);r[15]=0x1a64u;return 0;
        case 0x1a64u: /* bne 0x00001ac0 */
            if(!(state->cpsr&0x40000000u)){r[15]=6848u;}else{r[15]=0x1a68u;}return 0;
        case 0x1a68u: /* ldr r9,[r6,#0x10] */
            LOAD(r[9],r[6]+16u,4);r[15]=0x1a6cu;return 0;
        case 0x1a6cu: /* ldr r8,[r9,#0xc] */
            LOAD(r[8],r[9]+12u,4);r[15]=0x1a70u;return 0;
        case 0x1a70u: /* mov r8,r8, lsr #0x10 */
            r[8]=(r[8]>>16);r[15]=0x1a74u;return 0;
        case 0x1a74u: /* mov r9,r8, lsr #0x5 */
            r[9]=(r[8]>>5);r[15]=0x1a78u;return 0;
        case 0x1a78u: /* and r9,r9,#0x7 */
            r[9]=r[9]&7u;r[15]=0x1a7cu;return 0;
        case 0x1a7cu: /* add r9,r9,#0x1 */
            r[9]=r[9]+1u;r[15]=0x1a80u;return 0;
        case 0x1a80u: /* mul r9,r1,r9 */
            r[9]=r[1]*r[9];r[15]=0x1a84u;return 0;
        case 0x1a84u: /* sub r9,r9,#0x1 */
            r[9]=r[9]-1u;r[15]=0x1a88u;return 0;
        case 0x1a88u: /* mov r9,r9, lsr #0x2 */
            r[9]=(r[9]>>2);r[15]=0x1a8cu;return 0;
        case 0x1a8cu: /* and r4,r9,#0xe0 */
            r[4]=r[9]&224u;r[15]=0x1a90u;return 0;
        case 0x1a90u: /* and r9,r8,#0x7 */
            r[9]=r[8]&7u;r[15]=0x1a94u;return 0;
        case 0x1a94u: /* add r9,r9,#0x1 */
            r[9]=r[9]+1u;r[15]=0x1a98u;return 0;
        case 0x1a98u: /* mul r9,r1,r9 */
            r[9]=r[1]*r[9];r[15]=0x1a9cu;return 0;
        case 0x1a9cu: /* sub r9,r9,#0x1 */
            r[9]=r[9]-1u;r[15]=0x1aa0u;return 0;
        case 0x1aa0u: /* mov r9,r9, lsr #0x7 */
            r[9]=(r[9]>>7);r[15]=0x1aa4u;return 0;
        case 0x1aa4u: /* and r5,r9,#0x7 */
            r[5]=r[9]&7u;r[15]=0x1aa8u;return 0;
        case 0x1aa8u: /* ldr r9,[0x1ad8] */
            LOAD(r[9],6872u,4);r[15]=0x1aacu;return 0;
        case 0x1aacu: /* and r8,r8,r9 */
            r[8]=r[8]&r[9];r[15]=0x1ab0u;return 0;
        case 0x1ab0u: /* orr r8,r8,r4 */
            r[8]=r[8]|r[4];r[15]=0x1ab4u;return 0;
        case 0x1ab4u: /* orr r8,r8,r5 */
            r[8]=r[8]|r[5];r[15]=0x1ab8u;return 0;
        case 0x1ab8u: /* bic r8,r8,#0x8000 */
            r[8]=r[8]&~32768u;r[15]=0x1abcu;return 0;
        case 0x1abcu: /* str r8,[r11,#0x1c] */
            STORE(r[11]+28u,r[8],4);r[15]=0x1ac0u;return 0;
        case 0x1ac0u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x1ac4u;return 0;
        case 0x1ac4u: /* add r11,r11,#0x80 */
            r[11]=r[11]+128u;r[15]=0x1ac8u;return 0;
        case 0x1ac8u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x1accu;return 0;
        case 0x1accu: /* bne 0x00001a50 */
            if(!(state->cpsr&0x40000000u)){r[15]=6736u;}else{r[15]=0x1ad0u;}return 0;
        case 0x1ad0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1adcu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1ae0u;return 0;
        case 0x1ae0u: /* orr r1,r1,#0x80 */
            r[1]=r[1]|128u;r[15]=0x1ae4u;return 0;
        case 0x1ae4u: /* ldrb r0,[r5,#0x24] */
            LOAD(r[0],r[5]+36u,1);r[15]=0x1ae8u;return 0;
        case 0x1ae8u: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x1aecu;return 0;
        case 0x1aecu: /* bne 0x00001b4c */
            if(!(state->cpsr&0x40000000u)){r[15]=6988u;}else{r[15]=0x1af0u;}return 0;
        case 0x1af0u: /* ldrb r0,[r5,#0x25] */
            LOAD(r[0],r[5]+37u,1);r[15]=0x1af4u;return 0;
        case 0x1af4u: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x1af8u;return 0;
        case 0x1af8u: /* bne 0x00001b00 */
            if(!(state->cpsr&0x40000000u)){r[15]=6912u;}else{r[15]=0x1afcu;}return 0;
        case 0x1afcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1b00u: /* strb r1,[r5,#0x25] */
            STORE(r[5]+37u,r[1],1);r[15]=0x1b04u;return 0;
        case 0x1b04u: /* ldrb r6,[r5,#0x20] */
            LOAD(r[6],r[5]+32u,1);r[15]=0x1b08u;return 0;
        case 0x1b08u: /* tst r6,#0x7f */
            temporary=r[6]&127u;logic_flags(state,temporary,0,127u);r[15]=0x1b0cu;return 0;
        case 0x1b0cu: /* beq 0x00001b34 */
            if(state->cpsr&0x40000000u){r[15]=6964u;}else{r[15]=0x1b10u;}return 0;
        case 0x1b10u: /* cmp r6,#0x7f */
            (void)subtract(state,r[6],127u);r[15]=0x1b14u;return 0;
        case 0x1b14u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x1b18u;}return 0;
        case 0x1b18u: /* ldrb r6,[r5,#0x21] */
            LOAD(r[6],r[5]+33u,1);r[15]=0x1b1cu;return 0;
        case 0x1b1cu: /* cmp r6,#0x7f */
            (void)subtract(state,r[6],127u);r[15]=0x1b20u;return 0;
        case 0x1b20u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x1b24u;}return 0;
        case 0x1b24u: /* ldrb r0,[r5,#0x25] */
            LOAD(r[0],r[5]+37u,1);r[15]=0x1b28u;return 0;
        case 0x1b28u: /* and r0,r0,#0x7f */
            r[0]=r[0]&127u;r[15]=0x1b2cu;return 0;
        case 0x1b2cu: /* strb r0,[r5,#0x25] */
            STORE(r[5]+37u,r[0],1);r[15]=0x1b30u;return 0;
        case 0x1b30u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1b34u: /* ldrb r6,[r5,#0x21] */
            LOAD(r[6],r[5]+33u,1);r[15]=0x1b38u;return 0;
        case 0x1b38u: /* tst r6,#0x7f */
            temporary=r[6]&127u;logic_flags(state,temporary,0,127u);r[15]=0x1b3cu;return 0;
        case 0x1b3cu: /* beq 0x00001b44 */
            if(state->cpsr&0x40000000u){r[15]=6980u;}else{r[15]=0x1b40u;}return 0;
        case 0x1b40u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1b44u: /* strb r1,[r5,#0x3] */
            STORE(r[5]+3u,r[1],1);r[15]=0x1b48u;return 0;
        case 0x1b48u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1b4cu: /* strb r1,[r5,#0x24] */
            STORE(r[5]+36u,r[1],1);r[15]=0x1b50u;return 0;
        case 0x1b50u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1b54u: /* strb r1,[r5,#0x8] */
            STORE(r[5]+8u,r[1],1);r[15]=0x1b58u;return 0;
        case 0x1b58u: /* ldrb r6,[r5,#0x9] */
            LOAD(r[6],r[5]+9u,1);r[15]=0x1b5cu;return 0;
        case 0x1b5cu: /* ldr r8,[0x287c] */
            LOAD(r[8],10364u,4);r[15]=0x1b60u;return 0;
        case 0x1b60u: /* ldrb r1,[r8,r1] */
            LOAD(r[1],r[8]+r[1],1);r[15]=0x1b64u;return 0;
        case 0x1b64u: /* ldrb r6,[r8,r6] */
            LOAD(r[6],r[8]+r[6],1);r[15]=0x1b68u;return 0;
        case 0x1b68u: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x1b6cu;return 0;
        case 0x1b6cu: /* add r6,r6,#0x1 */
            r[6]=r[6]+1u;r[15]=0x1b70u;return 0;
        case 0x1b70u: /* mul r7,r1,r6 */
            r[7]=r[1]*r[6];r[15]=0x1b74u;return 0;
        case 0x1b74u: /* mov r7,r7, lsr #0x7 */
            r[7]=(r[7]>>7);r[15]=0x1b78u;return 0;
        case 0x1b78u: /* sub r7,r7,#0x1 */
            r[7]=r[7]-1u;r[15]=0x1b7cu;return 0;
        case 0x1b7cu: /* strb r7,[r5,#0xa] */
            STORE(r[5]+10u,r[7],1);r[15]=0x1b80u;return 0;
        case 0x1b80u: /* add r7,r7,#0x1 */
            r[7]=r[7]+1u;r[15]=0x1b84u;return 0;
        case 0x1b84u: /* mov r6,#0x80 */
            r[6]=128u;r[15]=0x1b88u;return 0;
        case 0x1b88u: /* mov r8,#0xc000 */
            r[8]=49152u;r[15]=0x1b8cu;return 0;
        case 0x1b8cu: /* mla r10,r3,r6,r8 */
            r[10]=r[3]*r[6]+r[8];r[15]=0x1b90u;return 0;
        case 0x1b90u: /* ldrb r10,[r10,#0x1] */
            LOAD(r[10],r[10]+1u,1);r[15]=0x1b94u;return 0;
        case 0x1b94u: /* sub r10,r10,#0x80 */
            r[10]=r[10]-128u;r[15]=0x1b98u;return 0;
        case 0x1b98u: /* mov r8,#0xe800 */
            r[8]=59392u;r[15]=0x1b9cu;return 0;
        case 0x1b9cu: /* mov r9,#0x30 */
            r[9]=48u;r[15]=0x1ba0u;return 0;
        case 0x1ba0u: /* mov r11,#0x800000 */
            r[11]=8388608u;r[15]=0x1ba4u;return 0;
        case 0x1ba4u: /* ldr r6,[r8,#0x0] */
            LOAD(r[6],r[8]+0u,4);r[15]=0x1ba8u;return 0;
        case 0x1ba8u: /* movs r6,r6 */
            r[6]=r[6];logic_flags(state,r[6],0,0);r[15]=0x1bacu;return 0;
        case 0x1bacu: /* beq 0x00001c40 */
            if(state->cpsr&0x40000000u){r[15]=7232u;}else{r[15]=0x1bb0u;}return 0;
        case 0x1bb0u: /* ldrb r6,[r8,#0x4] */
            LOAD(r[6],r[8]+4u,1);r[15]=0x1bb4u;return 0;
        case 0x1bb4u: /* cmp r6,r3 */
            (void)subtract(state,r[6],r[3]);r[15]=0x1bb8u;return 0;
        case 0x1bb8u: /* bne 0x00001c40 */
            if(!(state->cpsr&0x40000000u)){r[15]=7232u;}else{r[15]=0x1bbcu;}return 0;
        case 0x1bbcu: /* ldrb r6,[r8,#0x5] */
            LOAD(r[6],r[8]+5u,1);r[15]=0x1bc0u;return 0;
        case 0x1bc0u: /* cmp r6,r2 */
            (void)subtract(state,r[6],r[2]);r[15]=0x1bc4u;return 0;
        case 0x1bc4u: /* bne 0x00001c40 */
            if(!(state->cpsr&0x40000000u)){r[15]=7232u;}else{r[15]=0x1bc8u;}return 0;
        case 0x1bc8u: /* ldrb r1,[r8,#0x1c] */
            LOAD(r[1],r[8]+28u,1);r[15]=0x1bccu;return 0;
        case 0x1bccu: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x1bd0u;return 0;
        case 0x1bd0u: /* ldr r6,[r5,#0x18] */
            LOAD(r[6],r[5]+24u,4);r[15]=0x1bd4u;return 0;
        case 0x1bd4u: /* add r1,r1,r6 */
            r[1]=r[1]+r[6];r[15]=0x1bd8u;return 0;
        case 0x1bd8u: /* ldrb r6,[r8,#0x7] */
            LOAD(r[6],r[8]+7u,1);r[15]=0x1bdcu;return 0;
        case 0x1bdcu: /* ldrb r6,[r1,r6] */
            LOAD(r[6],r[1]+r[6],1);r[15]=0x1be0u;return 0;
        case 0x1be0u: /* add r6,r6,#0x1 */
            r[6]=r[6]+1u;r[15]=0x1be4u;return 0;
        case 0x1be4u: /* ldrb r7,[r5,#0xa] */
            LOAD(r[7],r[5]+10u,1);r[15]=0x1be8u;return 0;
        case 0x1be8u: /* add r7,r7,#0x1 */
            r[7]=r[7]+1u;r[15]=0x1becu;return 0;
        case 0x1becu: /* mul r12,r6,r7 */
            r[12]=r[6]*r[7];r[15]=0x1bf0u;return 0;
        case 0x1bf0u: /* ldr r6,[r8,#0x10] */
            LOAD(r[6],r[8]+16u,4);r[15]=0x1bf4u;return 0;
        case 0x1bf4u: /* ldrb r7,[r6,#0x15] */
            LOAD(r[7],r[6]+21u,1);r[15]=0x1bf8u;return 0;
        case 0x1bf8u: /* eor r7,r7,#0xff */
            r[7]=r[7]^255u;r[15]=0x1bfcu;return 0;
        case 0x1bfcu: /* add r7,r7,#0x1 */
            r[7]=r[7]+1u;r[15]=0x1c00u;return 0;
        case 0x1c00u: /* mul r12,r7,r12 */
            r[12]=r[7]*r[12];r[15]=0x1c04u;return 0;
        case 0x1c04u: /* sub r12,r12,#0x1 */
            r[12]=r[12]-1u;r[15]=0x1c08u;return 0;
        case 0x1c08u: /* mov r12,r12, lsr #0xe */
            r[12]=(r[12]>>14);r[15]=0x1c0cu;return 0;
        case 0x1c0cu: /* and r12,r12,#0xff */
            r[12]=r[12]&255u;r[15]=0x1c10u;return 0;
        case 0x1c10u: /* strb r12,[r8,#0x8] */
            STORE(r[8]+8u,r[12],1);r[15]=0x1c14u;return 0;
        case 0x1c14u: /* adds r6,r10,r12 */
            r[6]=addition(state,r[10],r[12]);r[15]=0x1c18u;return 0;
        case 0x1c18u: /* movmi r6,#0x0 */
            if(state->cpsr&0x80000000u){r[6]=0u;}r[15]=0x1c1cu;return 0;
        case 0x1c1cu: /* bmi 0x00001c28 */
            if(state->cpsr&0x80000000u){r[15]=7208u;}else{r[15]=0x1c20u;}return 0;
        case 0x1c20u: /* tst r6,#0x100 */
            temporary=r[6]&256u;logic_flags(state,temporary,1,256u);r[15]=0x1c24u;return 0;
        case 0x1c24u: /* movne r6,#0xff */
            if(!(state->cpsr&0x40000000u)){r[6]=255u;}r[15]=0x1c28u;return 0;
        case 0x1c28u: /* eor r6,r6,#0xff */
            r[6]=r[6]^255u;r[15]=0x1c2cu;return 0;
        case 0x1c2cu: /* and r6,r6,#0xff */
            r[6]=r[6]&255u;r[15]=0x1c30u;return 0;
        case 0x1c30u: /* ldr r4,[r11,#0x28] */
            LOAD(r[4],r[11]+40u,4);r[15]=0x1c34u;return 0;
        case 0x1c34u: /* and r4,r4,#0x3f */
            r[4]=r[4]&63u;r[15]=0x1c38u;return 0;
        case 0x1c38u: /* orr r4,r4,r6, lsl #0x8 */
            r[4]=r[4]|(r[6]<<8);r[15]=0x1c3cu;return 0;
        case 0x1c3cu: /* str r4,[r11,#0x28] */
            STORE(r[11]+40u,r[4],4);r[15]=0x1c40u;return 0;
        case 0x1c40u: /* add r8,r8,#0x40 */
            r[8]=r[8]+64u;r[15]=0x1c44u;return 0;
        case 0x1c44u: /* add r11,r11,#0x80 */
            r[11]=r[11]+128u;r[15]=0x1c48u;return 0;
        case 0x1c48u: /* subs r9,r9,#0x1 */
            r[9]=subtract(state,r[9],1u);r[15]=0x1c4cu;return 0;
        case 0x1c4cu: /* bne 0x00001ba4 */
            if(!(state->cpsr&0x40000000u)){r[15]=7076u;}else{r[15]=0x1c50u;}return 0;
        case 0x1c50u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1c54u: /* strb r1,[r5,#0xb] */
            STORE(r[5]+11u,r[1],1);r[15]=0x1c58u;return 0;
        case 0x1c58u: /* mov r6,#0x80 */
            r[6]=128u;r[15]=0x1c5cu;return 0;
        case 0x1c5cu: /* mov r7,#0xc000 */
            r[7]=49152u;r[15]=0x1c60u;return 0;
        case 0x1c60u: /* mla r7,r3,r6,r7 */
            r[7]=r[3]*r[6]+r[7];r[15]=0x1c64u;return 0;
        case 0x1c64u: /* ldrb r6,[r7,#0x7] */
            LOAD(r[6],r[7]+7u,1);r[15]=0x1c68u;return 0;
        case 0x1c68u: /* and r6,r6,#0xff */
            r[6]=r[6]&255u;r[15]=0x1c6cu;return 0;
        case 0x1c6cu: /* sub r6,r6,#0x80 */
            r[6]=r[6]-128u;r[15]=0x1c70u;return 0;
        case 0x1c70u: /* adds r1,r1,r6 */
            r[1]=addition(state,r[1],r[6]);r[15]=0x1c74u;return 0;
        case 0x1c74u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x1c78u;return 0;
        case 0x1c78u: /* bmi 0x00001c84 */
            if(state->cpsr&0x80000000u){r[15]=7300u;}else{r[15]=0x1c7cu;}return 0;
        case 0x1c7cu: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x1c80u;return 0;
        case 0x1c80u: /* movne r1,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[1]=127u;}r[15]=0x1c84u;return 0;
        case 0x1c84u: /* mov r1,r1, lsr #0x2 */
            r[1]=(r[1]>>2);r[15]=0x1c88u;return 0;
        case 0x1c88u: /* subs r1,r1,#0x10 */
            r[1]=subtract(state,r[1],16u);r[15]=0x1c8cu;return 0;
        case 0x1c8cu: /* eormi r1,r1,#0xff */
            if(state->cpsr&0x80000000u){r[1]=r[1]^255u;}r[15]=0x1c90u;return 0;
        case 0x1c90u: /* addmi r1,r1,#0x10 */
            if(state->cpsr&0x80000000u){r[1]=r[1]+16u;}r[15]=0x1c94u;return 0;
        case 0x1c94u: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0x1c98u;return 0;
        case 0x1c98u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x1c9cu;return 0;
        case 0x1c9cu: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x1ca0u;return 0;
        case 0x1ca0u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x1ca4u;return 0;
        case 0x1ca4u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x1ca8u;return 0;
        case 0x1ca8u: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x1cacu;return 0;
        case 0x1cacu: /* beq 0x00001ccc */
            if(state->cpsr&0x40000000u){r[15]=7372u;}else{r[15]=0x1cb0u;}return 0;
        case 0x1cb0u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x1cb4u;return 0;
        case 0x1cb4u: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x1cb8u;return 0;
        case 0x1cb8u: /* bne 0x00001ccc */
            if(!(state->cpsr&0x40000000u)){r[15]=7372u;}else{r[15]=0x1cbcu;}return 0;
        case 0x1cbcu: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x1cc0u;return 0;
        case 0x1cc0u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x1cc4u;return 0;
        case 0x1cc4u: /* bne 0x00001ccc */
            if(!(state->cpsr&0x40000000u)){r[15]=7372u;}else{r[15]=0x1cc8u;}return 0;
        case 0x1cc8u: /* strb r1,[r8,#0x24] */
            STORE(r[8]+36u,r[1],1);r[15]=0x1cccu;return 0;
        case 0x1cccu: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x1cd0u;return 0;
        case 0x1cd0u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x1cd4u;return 0;
        case 0x1cd4u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x1cd8u;return 0;
        case 0x1cd8u: /* bne 0x00001ca4 */
            if(!(state->cpsr&0x40000000u)){r[15]=7332u;}else{r[15]=0x1cdcu;}return 0;
        case 0x1cdcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1ce0u: /* strb r1,[r5,#0x9] */
            STORE(r[5]+9u,r[1],1);r[15]=0x1ce4u;return 0;
        case 0x1ce4u: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x1ce8u;return 0;
        case 0x1ce8u: /* ldrb r6,[r5,#0x8] */
            LOAD(r[6],r[5]+8u,1);r[15]=0x1cecu;return 0;
        case 0x1cecu: /* b 0x00001b5c */
            r[15]=7004u;return 0;
        case 0x1cf0u: /* ldrb r6,[r5,#0x28] */
            LOAD(r[6],r[5]+40u,1);r[15]=0x1cf4u;return 0;
        case 0x1cf4u: /* and r6,r6,#0x7f */
            r[6]=r[6]&127u;r[15]=0x1cf8u;return 0;
        case 0x1cf8u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1cfcu;return 0;
        case 0x1cfcu: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x1d00u;return 0;
        case 0x1d00u: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1d04u;return 0;
        case 0x1d04u: /* mov r6,r1, lsr #0x8 */
            r[6]=(r[1]>>8);r[15]=0x1d08u;return 0;
        case 0x1d08u: /* strb r6,[r5,#0x29] */
            STORE(r[5]+41u,r[6],1);r[15]=0x1d0cu;return 0;
        case 0x1d0cu: /* and r6,r1,#0xff */
            r[6]=r[1]&255u;r[15]=0x1d10u;return 0;
        case 0x1d10u: /* strb r6,[r5,#0x28] */
            STORE(r[5]+40u,r[6],1);r[15]=0x1d14u;return 0;
        case 0x1d14u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1d18u;return 0;
        case 0x1d18u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1d1cu;return 0;
        case 0x1d1cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1d20u: /* ldrb r6,[r5,#0x2a] */
            LOAD(r[6],r[5]+42u,1);r[15]=0x1d24u;return 0;
        case 0x1d24u: /* and r6,r6,#0x7f */
            r[6]=r[6]&127u;r[15]=0x1d28u;return 0;
        case 0x1d28u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1d2cu;return 0;
        case 0x1d2cu: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x1d30u;return 0;
        case 0x1d30u: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1d34u;return 0;
        case 0x1d34u: /* mov r6,r1, lsr #0x8 */
            r[6]=(r[1]>>8);r[15]=0x1d38u;return 0;
        case 0x1d38u: /* strb r6,[r5,#0x2b] */
            STORE(r[5]+43u,r[6],1);r[15]=0x1d3cu;return 0;
        case 0x1d3cu: /* and r6,r1,#0xff */
            r[6]=r[1]&255u;r[15]=0x1d40u;return 0;
        case 0x1d40u: /* strb r6,[r5,#0x2a] */
            STORE(r[5]+42u,r[6],1);r[15]=0x1d44u;return 0;
        case 0x1d44u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1d48u;return 0;
        case 0x1d48u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1d4cu;return 0;
        case 0x1d4cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1d50u: /* ldrb r6,[r5,#0x2c] */
            LOAD(r[6],r[5]+44u,1);r[15]=0x1d54u;return 0;
        case 0x1d54u: /* and r6,r6,#0x7f */
            r[6]=r[6]&127u;r[15]=0x1d58u;return 0;
        case 0x1d58u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1d5cu;return 0;
        case 0x1d5cu: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x1d60u;return 0;
        case 0x1d60u: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1d64u;return 0;
        case 0x1d64u: /* mov r6,r1, lsr #0x8 */
            r[6]=(r[1]>>8);r[15]=0x1d68u;return 0;
        case 0x1d68u: /* strb r6,[r5,#0x2d] */
            STORE(r[5]+45u,r[6],1);r[15]=0x1d6cu;return 0;
        case 0x1d6cu: /* and r6,r1,#0xff */
            r[6]=r[1]&255u;r[15]=0x1d70u;return 0;
        case 0x1d70u: /* strb r6,[r5,#0x2c] */
            STORE(r[5]+44u,r[6],1);r[15]=0x1d74u;return 0;
        case 0x1d74u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1d78u;return 0;
        case 0x1d78u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1d7cu;return 0;
        case 0x1d7cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1d80u: /* ldrb r6,[r5,#0x2e] */
            LOAD(r[6],r[5]+46u,1);r[15]=0x1d84u;return 0;
        case 0x1d84u: /* and r6,r6,#0x7f */
            r[6]=r[6]&127u;r[15]=0x1d88u;return 0;
        case 0x1d88u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1d8cu;return 0;
        case 0x1d8cu: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x1d90u;return 0;
        case 0x1d90u: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1d94u;return 0;
        case 0x1d94u: /* mov r6,r1, lsr #0x8 */
            r[6]=(r[1]>>8);r[15]=0x1d98u;return 0;
        case 0x1d98u: /* strb r6,[r5,#0x2f] */
            STORE(r[5]+47u,r[6],1);r[15]=0x1d9cu;return 0;
        case 0x1d9cu: /* and r6,r1,#0xff */
            r[6]=r[1]&255u;r[15]=0x1da0u;return 0;
        case 0x1da0u: /* strb r6,[r5,#0x2e] */
            STORE(r[5]+46u,r[6],1);r[15]=0x1da4u;return 0;
        case 0x1da4u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x1da8u;return 0;
        case 0x1da8u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x1dacu;return 0;
        case 0x1dacu: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x1db0u;return 0;
        case 0x1db0u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x1db4u;return 0;
        case 0x1db4u: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x1db8u;return 0;
        case 0x1db8u: /* beq 0x00001e24 */
            if(state->cpsr&0x40000000u){r[15]=7716u;}else{r[15]=0x1dbcu;}return 0;
        case 0x1dbcu: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x1dc0u;return 0;
        case 0x1dc0u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x1dc4u;return 0;
        case 0x1dc4u: /* bne 0x00001e24 */
            if(!(state->cpsr&0x40000000u)){r[15]=7716u;}else{r[15]=0x1dc8u;}return 0;
        case 0x1dc8u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x1dccu;return 0;
        case 0x1dccu: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x1dd0u;return 0;
        case 0x1dd0u: /* bne 0x00001e24 */
            if(!(state->cpsr&0x40000000u)){r[15]=7716u;}else{r[15]=0x1dd4u;}return 0;
        case 0x1dd4u: /* ldr r9,[r6,#0x10] */
            LOAD(r[9],r[6]+16u,4);r[15]=0x1dd8u;return 0;
        case 0x1dd8u: /* ldr r0,[r9,#0x1c] */
            LOAD(r[0],r[9]+28u,4);r[15]=0x1ddcu;return 0;
        case 0x1ddcu: /* ldr r9,[r5,#0x2c] */
            LOAD(r[9],r[5]+44u,4);r[15]=0x1de0u;return 0;
        case 0x1de0u: /* mov r9,r9, ror #0x10 */
            r[9]=((r[9]>>16)|(r[9]<<16));r[15]=0x1de4u;return 0;
        case 0x1de4u: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x1de8u;return 0;
        case 0x1de8u: /* orr r1,r1,#0xff00 */
            r[1]=r[1]|65280u;r[15]=0x1decu;return 0;
        case 0x1decu: /* and r9,r9,r1 */
            r[9]=r[9]&r[1];r[15]=0x1df0u;return 0;
        case 0x1df0u: /* sub r9,r9,#0x2000 */
            r[9]=r[9]-8192u;r[15]=0x1df4u;return 0;
        case 0x1df4u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0x1df8u;return 0;
        case 0x1df8u: /* add r1,r1,r9 */
            r[1]=r[1]+r[9];r[15]=0x1dfcu;return 0;
        case 0x1dfcu: /* ldr r9,[r5,#0x40] */
            LOAD(r[9],r[5]+64u,4);r[15]=0x1e00u;return 0;
        case 0x1e00u: /* adds r1,r1,r9 */
            r[1]=addition(state,r[1],r[9]);r[15]=0x1e04u;return 0;
        case 0x1e04u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x1e08u;return 0;
        case 0x1e08u: /* subs r4,r1,#0x8 */
            r[4]=subtract(state,r[1],8u);r[15]=0x1e0cu;return 0;
        case 0x1e0cu: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0x1e10u;return 0;
        case 0x1e10u: /* add r4,r1,#0x8 */
            r[4]=r[1]+8u;r[15]=0x1e14u;return 0;
        case 0x1e14u: /* tst r4,#0xe000 */
            temporary=r[4]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0x1e18u;return 0;
        case 0x1e18u: /* movne r1,#0xf8 */
            if(!(state->cpsr&0x40000000u)){r[1]=248u;}r[15]=0x1e1cu;return 0;
        case 0x1e1cu: /* orrne r1,r1,#0x1f00 */
            if(!(state->cpsr&0x40000000u)){r[1]=r[1]|7936u;}r[15]=0x1e20u;return 0;
        case 0x1e20u: /* str r1,[r8,#0x38] */
            STORE(r[8]+56u,r[1],4);r[15]=0x1e24u;return 0;
        case 0x1e24u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x1e28u;return 0;
        case 0x1e28u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x1e2cu;return 0;
        case 0x1e2cu: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x1e30u;return 0;
        case 0x1e30u: /* bne 0x00001db0 */
            if(!(state->cpsr&0x40000000u)){r[15]=7600u;}else{r[15]=0x1e34u;}return 0;
        case 0x1e34u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1e38u;return 0;
        case 0x1e38u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1e3cu;return 0;
        case 0x1e3cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1e40u: /* ldrb r6,[r5,#0x30] */
            LOAD(r[6],r[5]+48u,1);r[15]=0x1e44u;return 0;
        case 0x1e44u: /* and r6,r6,#0x7f */
            r[6]=r[6]&127u;r[15]=0x1e48u;return 0;
        case 0x1e48u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1e4cu;return 0;
        case 0x1e4cu: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x1e50u;return 0;
        case 0x1e50u: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1e54u;return 0;
        case 0x1e54u: /* mov r6,r1, lsr #0x8 */
            r[6]=(r[1]>>8);r[15]=0x1e58u;return 0;
        case 0x1e58u: /* strb r6,[r5,#0x31] */
            STORE(r[5]+49u,r[6],1);r[15]=0x1e5cu;return 0;
        case 0x1e5cu: /* and r6,r1,#0xff */
            r[6]=r[1]&255u;r[15]=0x1e60u;return 0;
        case 0x1e60u: /* strb r6,[r5,#0x30] */
            STORE(r[5]+48u,r[6],1);r[15]=0x1e64u;return 0;
        case 0x1e64u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1e68u;return 0;
        case 0x1e68u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1e6cu;return 0;
        case 0x1e6cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1e70u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x1e74u;return 0;
        case 0x1e74u: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x1e78u;return 0;
        case 0x1e78u: /* strb r1,[r5,#0x32] */
            STORE(r[5]+50u,r[1],1);r[15]=0x1e7cu;return 0;
        case 0x1e7cu: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1e80u;return 0;
        case 0x1e80u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1e84u;return 0;
        case 0x1e84u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1e88u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x1e8cu;return 0;
        case 0x1e8cu: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x1e90u;return 0;
        case 0x1e90u: /* strb r1,[r5,#0x33] */
            STORE(r[5]+51u,r[1],1);r[15]=0x1e94u;return 0;
        case 0x1e94u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1e98u;return 0;
        case 0x1e98u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1e9cu;return 0;
        case 0x1e9cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1ea0u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x1ea4u;return 0;
        case 0x1ea4u: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x1ea8u;return 0;
        case 0x1ea8u: /* strb r1,[r5,#0x34] */
            STORE(r[5]+52u,r[1],1);r[15]=0x1eacu;return 0;
        case 0x1eacu: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1eb0u;return 0;
        case 0x1eb0u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1eb4u;return 0;
        case 0x1eb4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1eb8u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x1ebcu;return 0;
        case 0x1ebcu: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x1ec0u;return 0;
        case 0x1ec0u: /* strb r1,[r5,#0x35] */
            STORE(r[5]+53u,r[1],1);r[15]=0x1ec4u;return 0;
        case 0x1ec4u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1ec8u;return 0;
        case 0x1ec8u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1eccu;return 0;
        case 0x1eccu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1ed0u: /* ldrb r6,[r5,#0x3c] */
            LOAD(r[6],r[5]+60u,1);r[15]=0x1ed4u;return 0;
        case 0x1ed4u: /* tst r6,#0x1 */
            temporary=r[6]&1u;logic_flags(state,temporary,0,1u);r[15]=0x1ed8u;return 0;
        case 0x1ed8u: /* bne 0x00001f1c */
            if(!(state->cpsr&0x40000000u)){r[15]=7964u;}else{r[15]=0x1edcu;}return 0;
        case 0x1edcu: /* strb r1,[r5,#0x1] */
            STORE(r[5]+1u,r[1],1);r[15]=0x1ee0u;return 0;
        case 0x1ee0u: /* ldr r6,[0x2880] */
            LOAD(r[6],10368u,4);r[15]=0x1ee4u;return 0;
        case 0x1ee4u: /* add r6,r6,r1, lsl #0x3 */
            r[6]=r[6]+(r[1]<<3);r[15]=0x1ee8u;return 0;
        case 0x1ee8u: /* ldr r7,[r6,#0x0] */
            LOAD(r[7],r[6]+0u,4);r[15]=0x1eecu;return 0;
        case 0x1eecu: /* movs r7,r7 */
            r[7]=r[7];logic_flags(state,r[7],0,0);r[15]=0x1ef0u;return 0;
        case 0x1ef0u: /* beq 0x00001f04 */
            if(state->cpsr&0x40000000u){r[15]=7940u;}else{r[15]=0x1ef4u;}return 0;
        case 0x1ef4u: /* ldr r8,[r6,#0x4] */
            LOAD(r[8],r[6]+4u,4);r[15]=0x1ef8u;return 0;
        case 0x1ef8u: /* movs r8,r8 */
            r[8]=r[8];logic_flags(state,r[8],0,0);r[15]=0x1efcu;return 0;
        case 0x1efcu: /* beq 0x00001f04 */
            if(state->cpsr&0x40000000u){r[15]=7940u;}else{r[15]=0x1f00u;}return 0;
        case 0x1f00u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1f04u: /* ldr r0,[0x2884] */
            LOAD(r[0],10372u,4);r[15]=0x1f08u;return 0;
        case 0x1f08u: /* ldrb r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,1);r[15]=0x1f0cu;return 0;
        case 0x1f0cu: /* mov r2,#0x1 */
            r[2]=1u;r[15]=0x1f10u;return 0;
        case 0x1f10u: /* orr r1,r1,r2 */
            r[1]=r[1]|r[2];r[15]=0x1f14u;return 0;
        case 0x1f14u: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x1f18u;return 0;
        case 0x1f18u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1f1cu: /* cmp r1,#0x2 */
            (void)subtract(state,r[1],2u);r[15]=0x1f20u;return 0;
        case 0x1f20u: /* movcs pc,lr */
            if(state->cpsr&0x20000000u){r[15]=r[14];}else{r[15]=0x1f24u;}return 0;
        case 0x1f24u: /* ldr r6,[0x2888] */
            LOAD(r[6],10376u,4);r[15]=0x1f28u;return 0;
        case 0x1f28u: /* add r6,r6,r1, lsl #0x3 */
            r[6]=r[6]+(r[1]<<3);r[15]=0x1f2cu;return 0;
        case 0x1f2cu: /* ldr r7,[r6,#0x0] */
            LOAD(r[7],r[6]+0u,4);r[15]=0x1f30u;return 0;
        case 0x1f30u: /* movs r7,r7 */
            r[7]=r[7];logic_flags(state,r[7],0,0);r[15]=0x1f34u;return 0;
        case 0x1f34u: /* beq 0x00001f50 */
            if(state->cpsr&0x40000000u){r[15]=8016u;}else{r[15]=0x1f38u;}return 0;
        case 0x1f38u: /* ldr r8,[r6,#0x4] */
            LOAD(r[8],r[6]+4u,4);r[15]=0x1f3cu;return 0;
        case 0x1f3cu: /* movs r8,r8 */
            r[8]=r[8];logic_flags(state,r[8],0,0);r[15]=0x1f40u;return 0;
        case 0x1f40u: /* beq 0x00001f50 */
            if(state->cpsr&0x40000000u){r[15]=8016u;}else{r[15]=0x1f44u;}return 0;
        case 0x1f44u: /* strb r1,[r5,#0x1] */
            STORE(r[5]+1u,r[1],1);r[15]=0x1f48u;return 0;
        case 0x1f48u: /* str r7,[r5,#0x10] */
            STORE(r[5]+16u,r[7],4);r[15]=0x1f4cu;return 0;
        case 0x1f4cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1f50u: /* ldr r0,[0x288c] */
            LOAD(r[0],10380u,4);r[15]=0x1f54u;return 0;
        case 0x1f54u: /* str r0,[r5,#0x10] */
            STORE(r[5]+16u,r[0],4);r[15]=0x1f58u;return 0;
        case 0x1f58u: /* b 0x00001f04 */
            r[15]=7940u;return 0;
        case 0x1f5cu: /* ldr r6,[0x2890] */
            LOAD(r[6],10384u,4);r[15]=0x1f60u;return 0;
        case 0x1f60u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x1f64u;return 0;
        case 0x1f64u: /* beq 0x00001f70 */
            if(state->cpsr&0x40000000u){r[15]=8048u;}else{r[15]=0x1f68u;}return 0;
        case 0x1f68u: /* ldrb r1,[r6,#0x0] */
            LOAD(r[1],r[6]+0u,1);r[15]=0x1f6cu;return 0;
        case 0x1f6cu: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x1f70u;return 0;
        case 0x1f70u: /* strb r1,[r6,#0x0] */
            STORE(r[6]+0u,r[1],1);r[15]=0x1f74u;return 0;
        case 0x1f74u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1f78u: /* ldrb r6,[r5,#0x28] */
            LOAD(r[6],r[5]+40u,1);r[15]=0x1f7cu;return 0;
        case 0x1f7cu: /* and r6,r6,#0x80 */
            r[6]=r[6]&128u;r[15]=0x1f80u;return 0;
        case 0x1f80u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1f84u;return 0;
        case 0x1f84u: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1f88u;return 0;
        case 0x1f88u: /* strb r1,[r5,#0x28] */
            STORE(r[5]+40u,r[1],1);r[15]=0x1f8cu;return 0;
        case 0x1f8cu: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1f90u;return 0;
        case 0x1f90u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1f94u;return 0;
        case 0x1f94u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1f98u: /* ldrb r6,[r5,#0x2a] */
            LOAD(r[6],r[5]+42u,1);r[15]=0x1f9cu;return 0;
        case 0x1f9cu: /* and r6,r6,#0x80 */
            r[6]=r[6]&128u;r[15]=0x1fa0u;return 0;
        case 0x1fa0u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1fa4u;return 0;
        case 0x1fa4u: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1fa8u;return 0;
        case 0x1fa8u: /* strb r1,[r5,#0x2a] */
            STORE(r[5]+42u,r[1],1);r[15]=0x1facu;return 0;
        case 0x1facu: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1fb0u;return 0;
        case 0x1fb0u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1fb4u;return 0;
        case 0x1fb4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1fb8u: /* ldrb r6,[r5,#0x2c] */
            LOAD(r[6],r[5]+44u,1);r[15]=0x1fbcu;return 0;
        case 0x1fbcu: /* and r6,r6,#0x80 */
            r[6]=r[6]&128u;r[15]=0x1fc0u;return 0;
        case 0x1fc0u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1fc4u;return 0;
        case 0x1fc4u: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1fc8u;return 0;
        case 0x1fc8u: /* strb r1,[r5,#0x2c] */
            STORE(r[5]+44u,r[1],1);r[15]=0x1fccu;return 0;
        case 0x1fccu: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1fd0u;return 0;
        case 0x1fd0u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1fd4u;return 0;
        case 0x1fd4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1fd8u: /* ldrb r6,[r5,#0x2e] */
            LOAD(r[6],r[5]+46u,1);r[15]=0x1fdcu;return 0;
        case 0x1fdcu: /* and r6,r6,#0x80 */
            r[6]=r[6]&128u;r[15]=0x1fe0u;return 0;
        case 0x1fe0u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1fe4u;return 0;
        case 0x1fe4u: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1fe8u;return 0;
        case 0x1fe8u: /* strb r1,[r5,#0x2e] */
            STORE(r[5]+46u,r[1],1);r[15]=0x1fecu;return 0;
        case 0x1fecu: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x1ff0u;return 0;
        case 0x1ff0u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x1ff4u;return 0;
        case 0x1ff4u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x1ff8u;return 0;
        case 0x1ff8u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x1ffcu;return 0;
        case 0x1ffcu: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x2000u;return 0;
        case 0x2000u: /* beq 0x0000206c */
            if(state->cpsr&0x40000000u){r[15]=8300u;}else{r[15]=0x2004u;}return 0;
        case 0x2004u: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x2008u;return 0;
        case 0x2008u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x200cu;return 0;
        case 0x200cu: /* bne 0x0000206c */
            if(!(state->cpsr&0x40000000u)){r[15]=8300u;}else{r[15]=0x2010u;}return 0;
        case 0x2010u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x2014u;return 0;
        case 0x2014u: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x2018u;return 0;
        case 0x2018u: /* bne 0x0000206c */
            if(!(state->cpsr&0x40000000u)){r[15]=8300u;}else{r[15]=0x201cu;}return 0;
        case 0x201cu: /* ldr r9,[r6,#0x10] */
            LOAD(r[9],r[6]+16u,4);r[15]=0x2020u;return 0;
        case 0x2020u: /* ldr r0,[r9,#0x1c] */
            LOAD(r[0],r[9]+28u,4);r[15]=0x2024u;return 0;
        case 0x2024u: /* ldr r9,[r5,#0x2c] */
            LOAD(r[9],r[5]+44u,4);r[15]=0x2028u;return 0;
        case 0x2028u: /* mov r9,r9, ror #0x10 */
            r[9]=((r[9]>>16)|(r[9]<<16));r[15]=0x202cu;return 0;
        case 0x202cu: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x2030u;return 0;
        case 0x2030u: /* orr r1,r1,#0xff00 */
            r[1]=r[1]|65280u;r[15]=0x2034u;return 0;
        case 0x2034u: /* and r9,r9,r1 */
            r[9]=r[9]&r[1];r[15]=0x2038u;return 0;
        case 0x2038u: /* sub r9,r9,#0x2000 */
            r[9]=r[9]-8192u;r[15]=0x203cu;return 0;
        case 0x203cu: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0x2040u;return 0;
        case 0x2040u: /* add r1,r1,r9 */
            r[1]=r[1]+r[9];r[15]=0x2044u;return 0;
        case 0x2044u: /* ldr r9,[r5,#0x40] */
            LOAD(r[9],r[5]+64u,4);r[15]=0x2048u;return 0;
        case 0x2048u: /* adds r1,r1,r9 */
            r[1]=addition(state,r[1],r[9]);r[15]=0x204cu;return 0;
        case 0x204cu: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x2050u;return 0;
        case 0x2050u: /* subs r4,r1,#0x8 */
            r[4]=subtract(state,r[1],8u);r[15]=0x2054u;return 0;
        case 0x2054u: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0x2058u;return 0;
        case 0x2058u: /* add r4,r1,#0x8 */
            r[4]=r[1]+8u;r[15]=0x205cu;return 0;
        case 0x205cu: /* tst r4,#0xe000 */
            temporary=r[4]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0x2060u;return 0;
        case 0x2060u: /* movne r1,#0xf8 */
            if(!(state->cpsr&0x40000000u)){r[1]=248u;}r[15]=0x2064u;return 0;
        case 0x2064u: /* orrne r1,r1,#0x1f00 */
            if(!(state->cpsr&0x40000000u)){r[1]=r[1]|7936u;}r[15]=0x2068u;return 0;
        case 0x2068u: /* str r1,[r8,#0x38] */
            STORE(r[8]+56u,r[1],4);r[15]=0x206cu;return 0;
        case 0x206cu: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x2070u;return 0;
        case 0x2070u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x2074u;return 0;
        case 0x2074u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x2078u;return 0;
        case 0x2078u: /* bne 0x00001ff8 */
            if(!(state->cpsr&0x40000000u)){r[15]=8184u;}else{r[15]=0x207cu;}return 0;
        case 0x207cu: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x2080u;return 0;
        case 0x2080u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x2084u;return 0;
        case 0x2084u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2088u: /* ldrb r6,[r5,#0x30] */
            LOAD(r[6],r[5]+48u,1);r[15]=0x208cu;return 0;
        case 0x208cu: /* and r6,r6,#0x80 */
            r[6]=r[6]&128u;r[15]=0x2090u;return 0;
        case 0x2090u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2094u;return 0;
        case 0x2094u: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x2098u;return 0;
        case 0x2098u: /* strb r1,[r5,#0x30] */
            STORE(r[5]+48u,r[1],1);r[15]=0x209cu;return 0;
        case 0x209cu: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x20a0u;return 0;
        case 0x20a0u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x20a4u;return 0;
        case 0x20a4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x20a8u: /* ands r1,r1,#0x40 */
            temporary=r[1]&64u;r[1]=temporary;logic_flags(state,temporary,0,64u);r[15]=0x20acu;return 0;
        case 0x20acu: /* strb r1,[r5,#0x26] */
            STORE(r[5]+38u,r[1],1);r[15]=0x20b0u;return 0;
        case 0x20b0u: /* bne 0x00002130 */
            if(!(state->cpsr&0x40000000u)){r[15]=8496u;}else{r[15]=0x20b4u;}return 0;
        case 0x20b4u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x20b8u;return 0;
        case 0x20b8u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x20bcu;return 0;
        case 0x20bcu: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x20c0u;return 0;
        case 0x20c0u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x20c4u;return 0;
        case 0x20c4u: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x20c8u;return 0;
        case 0x20c8u: /* beq 0x00002114 */
            if(state->cpsr&0x40000000u){r[15]=8468u;}else{r[15]=0x20ccu;}return 0;
        case 0x20ccu: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x20d0u;return 0;
        case 0x20d0u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x20d4u;return 0;
        case 0x20d4u: /* bne 0x00002114 */
            if(!(state->cpsr&0x40000000u)){r[15]=8468u;}else{r[15]=0x20d8u;}return 0;
        case 0x20d8u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x20dcu;return 0;
        case 0x20dcu: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x20e0u;return 0;
        case 0x20e0u: /* bne 0x00002114 */
            if(!(state->cpsr&0x40000000u)){r[15]=8468u;}else{r[15]=0x20e4u;}return 0;
        case 0x20e4u: /* ldrb r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,1);r[15]=0x20e8u;return 0;
        case 0x20e8u: /* bic r9,r9,#0x40 */
            r[9]=r[9]&~64u;r[15]=0x20ecu;return 0;
        case 0x20ecu: /* strb r9,[r6,#0x0] */
            STORE(r[6]+0u,r[9],1);r[15]=0x20f0u;return 0;
        case 0x20f0u: /* tst r9,#0x80 */
            temporary=r[9]&128u;logic_flags(state,temporary,0,128u);r[15]=0x20f4u;return 0;
        case 0x20f4u: /* bne 0x00002114 */
            if(!(state->cpsr&0x40000000u)){r[15]=8468u;}else{r[15]=0x20f8u;}return 0;
        case 0x20f8u: /* mov r9,#0x0 */
            r[9]=0u;r[15]=0x20fcu;return 0;
        case 0x20fcu: /* strb r9,[r6,#0x3f] */
            STORE(r[6]+63u,r[9],1);r[15]=0x2100u;return 0;
        case 0x2100u: /* ldr r9,[r8,#0x0] */
            LOAD(r[9],r[8]+0u,4);r[15]=0x2104u;return 0;
        case 0x2104u: /* ldr r4,[0x212c] */
            LOAD(r[4],8492u,4);r[15]=0x2108u;return 0;
        case 0x2108u: /* and r9,r9,r4 */
            r[9]=r[9]&r[4];r[15]=0x210cu;return 0;
        case 0x210cu: /* orr r9,r9,#0x8000 */
            r[9]=r[9]|32768u;r[15]=0x2110u;return 0;
        case 0x2110u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x2114u;return 0;
        case 0x2114u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x2118u;return 0;
        case 0x2118u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x211cu;return 0;
        case 0x211cu: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x2120u;return 0;
        case 0x2120u: /* bne 0x000020c0 */
            if(!(state->cpsr&0x40000000u)){r[15]=8384u;}else{r[15]=0x2124u;}return 0;
        case 0x2124u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2130u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x2134u;return 0;
        case 0x2134u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x2138u;return 0;
        case 0x2138u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x213cu;return 0;
        case 0x213cu: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x2140u;return 0;
        case 0x2140u: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x2144u;return 0;
        case 0x2144u: /* beq 0x0000216c */
            if(state->cpsr&0x40000000u){r[15]=8556u;}else{r[15]=0x2148u;}return 0;
        case 0x2148u: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x214cu;return 0;
        case 0x214cu: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x2150u;return 0;
        case 0x2150u: /* bne 0x0000216c */
            if(!(state->cpsr&0x40000000u)){r[15]=8556u;}else{r[15]=0x2154u;}return 0;
        case 0x2154u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x2158u;return 0;
        case 0x2158u: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x215cu;return 0;
        case 0x215cu: /* bne 0x0000216c */
            if(!(state->cpsr&0x40000000u)){r[15]=8556u;}else{r[15]=0x2160u;}return 0;
        case 0x2160u: /* ldrb r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,1);r[15]=0x2164u;return 0;
        case 0x2164u: /* orr r9,r9,#0x40 */
            r[9]=r[9]|64u;r[15]=0x2168u;return 0;
        case 0x2168u: /* strb r9,[r6,#0x0] */
            STORE(r[6]+0u,r[9],1);r[15]=0x216cu;return 0;
        case 0x216cu: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x2170u;return 0;
        case 0x2170u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x2174u;return 0;
        case 0x2174u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x2178u;return 0;
        case 0x2178u: /* bne 0x0000213c */
            if(!(state->cpsr&0x40000000u)){r[15]=8508u;}else{r[15]=0x217cu;}return 0;
        case 0x217cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2184u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2188u;return 0;
        case 0x2188u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x218cu;return 0;
        case 0x218cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2190u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2194u;return 0;
        case 0x2194u: /* strb r1,[r5,#0x27] */
            STORE(r[5]+39u,r[1],1);r[15]=0x2198u;return 0;
        case 0x2198u: /* sub r1,r1,#0x40 */
            r[1]=r[1]-64u;r[15]=0x219cu;return 0;
        case 0x219cu: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x21a0u;return 0;
        case 0x21a0u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x21a4u;return 0;
        case 0x21a4u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x21a8u;return 0;
        case 0x21a8u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x21acu;return 0;
        case 0x21acu: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x21b0u;return 0;
        case 0x21b0u: /* beq 0x000021f8 */
            if(state->cpsr&0x40000000u){r[15]=8696u;}else{r[15]=0x21b4u;}return 0;
        case 0x21b4u: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x21b8u;return 0;
        case 0x21b8u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x21bcu;return 0;
        case 0x21bcu: /* bne 0x000021f8 */
            if(!(state->cpsr&0x40000000u)){r[15]=8696u;}else{r[15]=0x21c0u;}return 0;
        case 0x21c0u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x21c4u;return 0;
        case 0x21c4u: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x21c8u;return 0;
        case 0x21c8u: /* bne 0x000021f8 */
            if(!(state->cpsr&0x40000000u)){r[15]=8696u;}else{r[15]=0x21ccu;}return 0;
        case 0x21ccu: /* ldr r9,[r6,#0x10] */
            LOAD(r[9],r[6]+16u,4);r[15]=0x21d0u;return 0;
        case 0x21d0u: /* ldr r4,[r9,#0x14] */
            LOAD(r[4],r[9]+20u,4);r[15]=0x21d4u;return 0;
        case 0x21d4u: /* ldrb r0,[r8,#0x28] */
            LOAD(r[0],r[8]+40u,1);r[15]=0x21d8u;return 0;
        case 0x21d8u: /* and r0,r0,#0x20 */
            r[0]=r[0]&32u;r[15]=0x21dcu;return 0;
        case 0x21dcu: /* and r4,r4,#0x1f */
            r[4]=r[4]&31u;r[15]=0x21e0u;return 0;
        case 0x21e0u: /* adds r1,r1,r4 */
            r[1]=addition(state,r[1],r[4]);r[15]=0x21e4u;return 0;
        case 0x21e4u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x21e8u;return 0;
        case 0x21e8u: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0x21ecu;return 0;
        case 0x21ecu: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0x21f0u;return 0;
        case 0x21f0u: /* orr r1,r1,r0 */
            r[1]=r[1]|r[0];r[15]=0x21f4u;return 0;
        case 0x21f4u: /* strb r1,[r8,#0x28] */
            STORE(r[8]+40u,r[1],1);r[15]=0x21f8u;return 0;
        case 0x21f8u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x21fcu;return 0;
        case 0x21fcu: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x2200u;return 0;
        case 0x2200u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x2204u;return 0;
        case 0x2204u: /* bne 0x000021a8 */
            if(!(state->cpsr&0x40000000u)){r[15]=8616u;}else{r[15]=0x2208u;}return 0;
        case 0x2208u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2210u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x2214u;return 0;
        case 0x2214u: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x2218u;return 0;
        case 0x2218u: /* strb r1,[r5,#0x3b] */
            STORE(r[5]+59u,r[1],1);r[15]=0x221cu;return 0;
        case 0x221cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2220u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x2224u;return 0;
        case 0x2224u: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x2228u;return 0;
        case 0x2228u: /* strb r1,[r5,#0x37] */
            STORE(r[5]+55u,r[1],1);r[15]=0x222cu;return 0;
        case 0x222cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2230u: /* ldrb r6,[r5,#0x3e] */
            LOAD(r[6],r[5]+62u,1);r[15]=0x2234u;return 0;
        case 0x2234u: /* adr r7,0x2240 */
            r[7]=8768u;r[15]=0x2238u;return 0;
        case 0x2238u: /* add r6,r6,r7 */
            r[6]=r[6]+r[7];r[15]=0x223cu;return 0;
        case 0x223cu: /* mov pc,r6 */
            r[15]=r[6];return 0;
        case 0x2240u: /* b 0x0000233c */
            r[15]=9020u;return 0;
        case 0x2244u: /* b 0x00002298 */
            r[15]=8856u;return 0;
        case 0x2248u: /* b 0x000022ac */
            r[15]=8876u;return 0;
        case 0x224cu: /* b 0x000022c4 */
            r[15]=8900u;return 0;
        case 0x2250u: /* b 0x000022dc */
            r[15]=8924u;return 0;
        case 0x2254u: /* b 0x000022f4 */
            r[15]=8948u;return 0;
        case 0x2258u: /* b 0x0000230c */
            r[15]=8972u;return 0;
        case 0x225cu: /* b 0x00002324 */
            r[15]=8996u;return 0;
        case 0x2260u: /* b 0x0000233c */
            r[15]=9020u;return 0;
        case 0x2280u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2284u;return 0;
        case 0x2284u: /* cmp r1,#0x8 */
            (void)subtract(state,r[1],8u);r[15]=0x2288u;return 0;
        case 0x2288u: /* movhi r1,#0x8 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[1]=8u;}r[15]=0x228cu;return 0;
        case 0x228cu: /* mov r1,r1, lsl #0x2 */
            r[1]=(r[1]<<2);r[15]=0x2290u;return 0;
        case 0x2290u: /* strb r1,[r5,#0x3e] */
            STORE(r[5]+62u,r[1],1);r[15]=0x2294u;return 0;
        case 0x2294u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2298u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x229cu;return 0;
        case 0x229cu: /* sub r1,r1,#0x40 */
            r[1]=r[1]-64u;r[15]=0x22a0u;return 0;
        case 0x22a0u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x22a4u;return 0;
        case 0x22a4u: /* b 0x0000234c */
            r[15]=9036u;return 0;
        case 0x22acu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x22b0u;return 0;
        case 0x22b0u: /* mov r1,r1, lsl #0x1 */
            r[1]=(r[1]<<1);r[15]=0x22b4u;return 0;
        case 0x22b4u: /* sub r1,r1,#0x80 */
            r[1]=r[1]-128u;r[15]=0x22b8u;return 0;
        case 0x22b8u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x22bcu;return 0;
        case 0x22bcu: /* b 0x0000234c */
            r[15]=9036u;return 0;
        case 0x22c4u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x22c8u;return 0;
        case 0x22c8u: /* mov r1,r1, lsl #0x2 */
            r[1]=(r[1]<<2);r[15]=0x22ccu;return 0;
        case 0x22ccu: /* sub r1,r1,#0x100 */
            r[1]=r[1]-256u;r[15]=0x22d0u;return 0;
        case 0x22d0u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x22d4u;return 0;
        case 0x22d4u: /* b 0x0000234c */
            r[15]=9036u;return 0;
        case 0x22dcu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x22e0u;return 0;
        case 0x22e0u: /* mov r1,r1, lsl #0x3 */
            r[1]=(r[1]<<3);r[15]=0x22e4u;return 0;
        case 0x22e4u: /* sub r1,r1,#0x200 */
            r[1]=r[1]-512u;r[15]=0x22e8u;return 0;
        case 0x22e8u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x22ecu;return 0;
        case 0x22ecu: /* b 0x0000234c */
            r[15]=9036u;return 0;
        case 0x22f4u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x22f8u;return 0;
        case 0x22f8u: /* mov r1,r1, lsl #0x4 */
            r[1]=(r[1]<<4);r[15]=0x22fcu;return 0;
        case 0x22fcu: /* sub r1,r1,#0x400 */
            r[1]=r[1]-1024u;r[15]=0x2300u;return 0;
        case 0x2300u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x2304u;return 0;
        case 0x2304u: /* b 0x0000234c */
            r[15]=9036u;return 0;
        case 0x230cu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2310u;return 0;
        case 0x2310u: /* mov r1,r1, lsl #0x5 */
            r[1]=(r[1]<<5);r[15]=0x2314u;return 0;
        case 0x2314u: /* sub r1,r1,#0x800 */
            r[1]=r[1]-2048u;r[15]=0x2318u;return 0;
        case 0x2318u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x231cu;return 0;
        case 0x231cu: /* b 0x0000234c */
            r[15]=9036u;return 0;
        case 0x2324u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2328u;return 0;
        case 0x2328u: /* mov r1,r1, lsl #0x6 */
            r[1]=(r[1]<<6);r[15]=0x232cu;return 0;
        case 0x232cu: /* sub r1,r1,#0x1000 */
            r[1]=r[1]-4096u;r[15]=0x2330u;return 0;
        case 0x2330u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x2334u;return 0;
        case 0x2334u: /* b 0x0000234c */
            r[15]=9036u;return 0;
        case 0x233cu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2340u;return 0;
        case 0x2340u: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x2344u;return 0;
        case 0x2344u: /* sub r1,r1,#0x2000 */
            r[1]=r[1]-8192u;r[15]=0x2348u;return 0;
        case 0x2348u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x234cu;return 0;
        case 0x234cu: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x2350u;return 0;
        case 0x2350u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x2354u;return 0;
        case 0x2354u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x2358u;return 0;
        case 0x2358u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x235cu;return 0;
        case 0x235cu: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x2360u;return 0;
        case 0x2360u: /* beq 0x000023cc */
            if(state->cpsr&0x40000000u){r[15]=9164u;}else{r[15]=0x2364u;}return 0;
        case 0x2364u: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x2368u;return 0;
        case 0x2368u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x236cu;return 0;
        case 0x236cu: /* bne 0x000023cc */
            if(!(state->cpsr&0x40000000u)){r[15]=9164u;}else{r[15]=0x2370u;}return 0;
        case 0x2370u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x2374u;return 0;
        case 0x2374u: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x2378u;return 0;
        case 0x2378u: /* bne 0x000023cc */
            if(!(state->cpsr&0x40000000u)){r[15]=9164u;}else{r[15]=0x237cu;}return 0;
        case 0x237cu: /* ldr r9,[r6,#0x10] */
            LOAD(r[9],r[6]+16u,4);r[15]=0x2380u;return 0;
        case 0x2380u: /* ldr r0,[r9,#0x1c] */
            LOAD(r[0],r[9]+28u,4);r[15]=0x2384u;return 0;
        case 0x2384u: /* ldr r9,[r5,#0x2c] */
            LOAD(r[9],r[5]+44u,4);r[15]=0x2388u;return 0;
        case 0x2388u: /* mov r9,r9, ror #0x10 */
            r[9]=((r[9]>>16)|(r[9]<<16));r[15]=0x238cu;return 0;
        case 0x238cu: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x2390u;return 0;
        case 0x2390u: /* orr r1,r1,#0xff00 */
            r[1]=r[1]|65280u;r[15]=0x2394u;return 0;
        case 0x2394u: /* and r9,r9,r1 */
            r[9]=r[9]&r[1];r[15]=0x2398u;return 0;
        case 0x2398u: /* sub r9,r9,#0x2000 */
            r[9]=r[9]-8192u;r[15]=0x239cu;return 0;
        case 0x239cu: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0x23a0u;return 0;
        case 0x23a0u: /* add r1,r1,r9 */
            r[1]=r[1]+r[9];r[15]=0x23a4u;return 0;
        case 0x23a4u: /* ldr r9,[r5,#0x40] */
            LOAD(r[9],r[5]+64u,4);r[15]=0x23a8u;return 0;
        case 0x23a8u: /* adds r1,r1,r9 */
            r[1]=addition(state,r[1],r[9]);r[15]=0x23acu;return 0;
        case 0x23acu: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x23b0u;return 0;
        case 0x23b0u: /* subs r4,r1,#0x8 */
            r[4]=subtract(state,r[1],8u);r[15]=0x23b4u;return 0;
        case 0x23b4u: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0x23b8u;return 0;
        case 0x23b8u: /* add r4,r1,#0x8 */
            r[4]=r[1]+8u;r[15]=0x23bcu;return 0;
        case 0x23bcu: /* tst r4,#0xe000 */
            temporary=r[4]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0x23c0u;return 0;
        case 0x23c0u: /* movne r1,#0xf8 */
            if(!(state->cpsr&0x40000000u)){r[1]=248u;}r[15]=0x23c4u;return 0;
        case 0x23c4u: /* orrne r1,r1,#0x1f00 */
            if(!(state->cpsr&0x40000000u)){r[1]=r[1]|7936u;}r[15]=0x23c8u;return 0;
        case 0x23c8u: /* str r1,[r8,#0x38] */
            STORE(r[8]+56u,r[1],4);r[15]=0x23ccu;return 0;
        case 0x23ccu: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x23d0u;return 0;
        case 0x23d0u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x23d4u;return 0;
        case 0x23d4u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x23d8u;return 0;
        case 0x23d8u: /* bne 0x00002358 */
            if(!(state->cpsr&0x40000000u)){r[15]=9048u;}else{r[15]=0x23dcu;}return 0;
        case 0x23dcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x23e0u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x23e4u;return 0;
        case 0x23e4u: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x23e8u;return 0;
        case 0x23e8u: /* strb r1,[r5,#0x39] */
            STORE(r[5]+57u,r[1],1);r[15]=0x23ecu;return 0;
        case 0x23ecu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x23f0u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x23f4u;return 0;
        case 0x23f4u: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x23f8u;return 0;
        case 0x23f8u: /* strb r1,[r5,#0x38] */
            STORE(r[5]+56u,r[1],1);r[15]=0x23fcu;return 0;
        case 0x23fcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2400u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x2404u;return 0;
        case 0x2404u: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x2408u;return 0;
        case 0x2408u: /* strb r1,[r5,#0x3a] */
            STORE(r[5]+58u,r[1],1);r[15]=0x240cu;return 0;
        case 0x240cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2410u: /* mov r0,#0x7 */
            r[0]=7u;r[15]=0x2414u;return 0;
        case 0x2414u: /* cmp r3,r0 */
            (void)subtract(state,r[3],r[0]);r[15]=0x2418u;return 0;
        case 0x2418u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x241cu;}return 0;
        case 0x241cu: /* mov r1,r1, lsr #0x2 */
            r[1]=(r[1]>>2);r[15]=0x2420u;return 0;
        case 0x2420u: /* subs r1,r1,#0x10 */
            r[1]=subtract(state,r[1],16u);r[15]=0x2424u;return 0;
        case 0x2424u: /* eormi r1,r1,#0xff */
            if(state->cpsr&0x80000000u){r[1]=r[1]^255u;}r[15]=0x2428u;return 0;
        case 0x2428u: /* addmi r1,r1,#0x10 */
            if(state->cpsr&0x80000000u){r[1]=r[1]+16u;}r[15]=0x242cu;return 0;
        case 0x242cu: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0x2430u;return 0;
        case 0x2430u: /* and r2,r2,#0xf */
            r[2]=r[2]&15u;r[15]=0x2434u;return 0;
        case 0x2434u: /* mov r2,r2, lsl #0x2 */
            r[2]=(r[2]<<2);r[15]=0x2438u;return 0;
        case 0x2438u: /* ldr r3,[0x2894] */
            LOAD(r[3],10388u,4);r[15]=0x243cu;return 0;
        case 0x243cu: /* strb r1,[r3,r2] */
            STORE(r[3]+r[2],r[1],1);r[15]=0x2440u;return 0;
        case 0x2440u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2448u: /* mov r0,#0x7 */
            r[0]=7u;r[15]=0x244cu;return 0;
        case 0x244cu: /* cmp r3,r0 */
            (void)subtract(state,r[3],r[0]);r[15]=0x2450u;return 0;
        case 0x2450u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x2454u;}return 0;
        case 0x2454u: /* mov r1,r1, lsr #0x3 */
            r[1]=(r[1]>>3);r[15]=0x2458u;return 0;
        case 0x2458u: /* and r1,r1,#0xf */
            r[1]=r[1]&15u;r[15]=0x245cu;return 0;
        case 0x245cu: /* and r2,r2,#0xf */
            r[2]=r[2]&15u;r[15]=0x2460u;return 0;
        case 0x2460u: /* mov r2,r2, lsl #0x2 */
            r[2]=(r[2]<<2);r[15]=0x2464u;return 0;
        case 0x2464u: /* ldr r3,[0x2898] */
            LOAD(r[3],10392u,4);r[15]=0x2468u;return 0;
        case 0x2468u: /* strb r1,[r3,r2] */
            STORE(r[3]+r[2],r[1],1);r[15]=0x246cu;return 0;
        case 0x246cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2474u: /* mov r0,#0x7 */
            r[0]=7u;r[15]=0x2478u;return 0;
        case 0x2478u: /* cmp r3,r0 */
            (void)subtract(state,r[3],r[0]);r[15]=0x247cu;return 0;
        case 0x247cu: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x2480u;}return 0;
        case 0x2480u: /* ands r0,r1,#0x7f */
            temporary=r[1]&127u;r[0]=temporary;logic_flags(state,temporary,0,127u);r[15]=0x2484u;return 0;
        case 0x2484u: /* beq 0x0000249c */
            if(state->cpsr&0x40000000u){r[15]=9372u;}else{r[15]=0x2488u;}return 0;
        case 0x2488u: /* sub r0,r0,#0x1 */
            r[0]=r[0]-1u;r[15]=0x248cu;return 0;
        case 0x248cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x2490u;return 0;
        case 0x2490u: /* bl 0x00005c20 */
            r[14]=0x2494u;r[15]=23584u;return 0;
        case 0x2494u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x2498u;return 0;
        case 0x2498u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x249cu: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x24a0u;return 0;
        case 0x24a0u: /* bl 0x00005d54 */
            r[14]=0x24a4u;r[15]=23892u;return 0;
        case 0x24a4u: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x24a8u;return 0;
        case 0x24a8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x24b0u: /* ldr r0,[0x289c] */
            LOAD(r[0],10396u,4);r[15]=0x24b4u;return 0;
        case 0x24b4u: /* ldrb r3,[r0,#0x0] */
            LOAD(r[3],r[0]+0u,1);r[15]=0x24b8u;return 0;
        case 0x24b8u: /* tst r3,#0x30 */
            temporary=r[3]&48u;logic_flags(state,temporary,0,48u);r[15]=0x24bcu;return 0;
        case 0x24bcu: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x24c0u;}return 0;
        case 0x24c0u: /* tst r3,#0x10 */
            temporary=r[3]&16u;logic_flags(state,temporary,0,16u);r[15]=0x24c4u;return 0;
        case 0x24c4u: /* andeq r2,r2,#0x3 */
            if(state->cpsr&0x40000000u){r[2]=r[2]&3u;}r[15]=0x24c8u;return 0;
        case 0x24c8u: /* moveq r0,#0x10 */
            if(state->cpsr&0x40000000u){r[0]=16u;}r[15]=0x24ccu;return 0;
        case 0x24ccu: /* andne r2,r2,#0x7 */
            if(!(state->cpsr&0x40000000u)){r[2]=r[2]&7u;}r[15]=0x24d0u;return 0;
        case 0x24d0u: /* movne r0,#0x20 */
            if(!(state->cpsr&0x40000000u)){r[0]=32u;}r[15]=0x24d4u;return 0;
        case 0x24d4u: /* ldr r3,[0x251c] */
            LOAD(r[3],9500u,4);r[15]=0x24d8u;return 0;
        case 0x24d8u: /* and r1,r1,#0x7c */
            r[1]=r[1]&124u;r[15]=0x24dcu;return 0;
        case 0x24dcu: /* add r1,r3,r1, lsl #0x2 */
            r[1]=r[3]+(r[1]<<2);r[15]=0x24e0u;return 0;
        case 0x24e0u: /* ldr r3,[0x28a0] */
            LOAD(r[3],10400u,4);r[15]=0x24e4u;return 0;
        case 0x24e4u: /* ldrb r4,[r3,#0x0] */
            LOAD(r[4],r[3]+0u,1);r[15]=0x24e8u;return 0;
        case 0x24e8u: /* ldr r3,[0x28a4] */
            LOAD(r[3],10404u,4);r[15]=0x24ecu;return 0;
        case 0x24ecu: /* add r3,r3,r4, lsl #0x2 */
            r[3]=r[3]+(r[4]<<2);r[15]=0x24f0u;return 0;
        case 0x24f0u: /* add r2,r3,r2, lsl #0x2 */
            r[2]=r[3]+(r[2]<<2);r[15]=0x24f4u;return 0;
        case 0x24f4u: /* ldr r3,[r1],#0x4 */
            LOAD(r[3],r[1],4);r[1]+=4u;r[15]=0x24f8u;return 0;
        case 0x24f8u: /* str r3,[r2],r0 */
            STORE(r[2],r[3],4);r[2]+=r[0];r[15]=0x24fcu;return 0;
        case 0x24fcu: /* ldr r3,[r1],#0x4 */
            LOAD(r[3],r[1],4);r[1]+=4u;r[15]=0x2500u;return 0;
        case 0x2500u: /* str r3,[r2],r0 */
            STORE(r[2],r[3],4);r[2]+=r[0];r[15]=0x2504u;return 0;
        case 0x2504u: /* ldr r3,[r1],#0x4 */
            LOAD(r[3],r[1],4);r[1]+=4u;r[15]=0x2508u;return 0;
        case 0x2508u: /* str r3,[r2],r0 */
            STORE(r[2],r[3],4);r[2]+=r[0];r[15]=0x250cu;return 0;
        case 0x250cu: /* ldr r3,[r1],#0x4 */
            LOAD(r[3],r[1],4);r[1]+=4u;r[15]=0x2510u;return 0;
        case 0x2510u: /* str r3,[r2,#0x0] */
            STORE(r[2]+0u,r[3],4);r[15]=0x2514u;return 0;
        case 0x2514u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2520u: /* ands r1,r1,#0xff */
            temporary=r[1]&255u;r[1]=temporary;logic_flags(state,temporary,0,255u);r[15]=0x2524u;return 0;
        case 0x2524u: /* beq 0x000025c4 */
            if(state->cpsr&0x40000000u){r[15]=9668u;}else{r[15]=0x2528u;}return 0;
        case 0x2528u: /* mov r1,r1, lsl #0x1 */
            r[1]=(r[1]<<1);r[15]=0x252cu;return 0;
        case 0x252cu: /* strb r1,[r5,#0x36] */
            STORE(r[5]+54u,r[1],1);r[15]=0x2530u;return 0;
        case 0x2530u: /* mov r1,r1, lsr #0x4 */
            r[1]=(r[1]>>4);r[15]=0x2534u;return 0;
        case 0x2534u: /* mov r6,#0x80 */
            r[6]=128u;r[15]=0x2538u;return 0;
        case 0x2538u: /* mov r7,#0xc000 */
            r[7]=49152u;r[15]=0x253cu;return 0;
        case 0x253cu: /* mla r7,r3,r6,r7 */
            r[7]=r[3]*r[6]+r[7];r[15]=0x2540u;return 0;
        case 0x2540u: /* ldrb r4,[r7,#0x26] */
            LOAD(r[4],r[7]+38u,1);r[15]=0x2544u;return 0;
        case 0x2544u: /* mov r4,r4, lsr #0x3 */
            r[4]=(r[4]>>3);r[15]=0x2548u;return 0;
        case 0x2548u: /* and r4,r4,#0x1f */
            r[4]=r[4]&31u;r[15]=0x254cu;return 0;
        case 0x254cu: /* sub r4,r4,#0x10 */
            r[4]=r[4]-16u;r[15]=0x2550u;return 0;
        case 0x2550u: /* adds r1,r1,r4 */
            r[1]=addition(state,r[1],r[4]);r[15]=0x2554u;return 0;
        case 0x2554u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x2558u;return 0;
        case 0x2558u: /* bmi 0x00002564 */
            if(state->cpsr&0x80000000u){r[15]=9572u;}else{r[15]=0x255cu;}return 0;
        case 0x255cu: /* tst r1,#0x10 */
            temporary=r[1]&16u;logic_flags(state,temporary,0,16u);r[15]=0x2560u;return 0;
        case 0x2560u: /* movne r1,#0xf */
            if(!(state->cpsr&0x40000000u)){r[1]=15u;}r[15]=0x2564u;return 0;
        case 0x2564u: /* and r1,r1,#0xf */
            r[1]=r[1]&15u;r[15]=0x2568u;return 0;
        case 0x2568u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x256cu;return 0;
        case 0x256cu: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x2570u;return 0;
        case 0x2570u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x2574u;return 0;
        case 0x2574u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x2578u;return 0;
        case 0x2578u: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x257cu;return 0;
        case 0x257cu: /* beq 0x000025ac */
            if(state->cpsr&0x40000000u){r[15]=9644u;}else{r[15]=0x2580u;}return 0;
        case 0x2580u: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x2584u;return 0;
        case 0x2584u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x2588u;return 0;
        case 0x2588u: /* bne 0x000025ac */
            if(!(state->cpsr&0x40000000u)){r[15]=9644u;}else{r[15]=0x258cu;}return 0;
        case 0x258cu: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x2590u;return 0;
        case 0x2590u: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x2594u;return 0;
        case 0x2594u: /* bne 0x000025ac */
            if(!(state->cpsr&0x40000000u)){r[15]=9644u;}else{r[15]=0x2598u;}return 0;
        case 0x2598u: /* ldr r9,[r6,#0x10] */
            LOAD(r[9],r[6]+16u,4);r[15]=0x259cu;return 0;
        case 0x259cu: /* ldr r4,[r9,#0x10] */
            LOAD(r[4],r[9]+16u,4);r[15]=0x25a0u;return 0;
        case 0x25a0u: /* and r4,r4,#0xf */
            r[4]=r[4]&15u;r[15]=0x25a4u;return 0;
        case 0x25a4u: /* orr r4,r4,r1, lsl #0x4 */
            r[4]=r[4]|(r[1]<<4);r[15]=0x25a8u;return 0;
        case 0x25a8u: /* strb r4,[r8,#0x20] */
            STORE(r[8]+32u,r[4],1);r[15]=0x25acu;return 0;
        case 0x25acu: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x25b0u;return 0;
        case 0x25b0u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x25b4u;return 0;
        case 0x25b4u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x25b8u;return 0;
        case 0x25b8u: /* bne 0x00002574 */
            if(!(state->cpsr&0x40000000u)){r[15]=9588u;}else{r[15]=0x25bcu;}return 0;
        case 0x25bcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x25c4u: /* strb r1,[r5,#0x36] */
            STORE(r[5]+54u,r[1],1);r[15]=0x25c8u;return 0;
        case 0x25c8u: /* mov r6,#0x80 */
            r[6]=128u;r[15]=0x25ccu;return 0;
        case 0x25ccu: /* mov r7,#0xc000 */
            r[7]=49152u;r[15]=0x25d0u;return 0;
        case 0x25d0u: /* mla r7,r3,r6,r7 */
            r[7]=r[3]*r[6]+r[7];r[15]=0x25d4u;return 0;
        case 0x25d4u: /* ldrb r5,[r7,#0x26] */
            LOAD(r[5],r[7]+38u,1);r[15]=0x25d8u;return 0;
        case 0x25d8u: /* mov r5,r5, lsr #0x3 */
            r[5]=(r[5]>>3);r[15]=0x25dcu;return 0;
        case 0x25dcu: /* and r5,r5,#0x1f */
            r[5]=r[5]&31u;r[15]=0x25e0u;return 0;
        case 0x25e0u: /* sub r5,r5,#0x20 */
            r[5]=r[5]-32u;r[15]=0x25e4u;return 0;
        case 0x25e4u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x25e8u;return 0;
        case 0x25e8u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x25ecu;return 0;
        case 0x25ecu: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x25f0u;return 0;
        case 0x25f0u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x25f4u;return 0;
        case 0x25f4u: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x25f8u;return 0;
        case 0x25f8u: /* beq 0x00002648 */
            if(state->cpsr&0x40000000u){r[15]=9800u;}else{r[15]=0x25fcu;}return 0;
        case 0x25fcu: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x2600u;return 0;
        case 0x2600u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x2604u;return 0;
        case 0x2604u: /* bne 0x00002648 */
            if(!(state->cpsr&0x40000000u)){r[15]=9800u;}else{r[15]=0x2608u;}return 0;
        case 0x2608u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x260cu;return 0;
        case 0x260cu: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x2610u;return 0;
        case 0x2610u: /* bne 0x00002648 */
            if(!(state->cpsr&0x40000000u)){r[15]=9800u;}else{r[15]=0x2614u;}return 0;
        case 0x2614u: /* ldr r9,[r6,#0x10] */
            LOAD(r[9],r[6]+16u,4);r[15]=0x2618u;return 0;
        case 0x2618u: /* ldr r1,[r9,#0x10] */
            LOAD(r[1],r[9]+16u,4);r[15]=0x261cu;return 0;
        case 0x261cu: /* and r4,r1,#0xf */
            r[4]=r[1]&15u;r[15]=0x2620u;return 0;
        case 0x2620u: /* mov r1,r1, lsr #0x4 */
            r[1]=(r[1]>>4);r[15]=0x2624u;return 0;
        case 0x2624u: /* and r1,r1,#0xf */
            r[1]=r[1]&15u;r[15]=0x2628u;return 0;
        case 0x2628u: /* adds r1,r1,r5 */
            r[1]=addition(state,r[1],r[5]);r[15]=0x262cu;return 0;
        case 0x262cu: /* movcs r1,#0x0 */
            if(state->cpsr&0x20000000u){r[1]=0u;}r[15]=0x2630u;return 0;
        case 0x2630u: /* bcs 0x0000263c */
            if(state->cpsr&0x20000000u){r[15]=9788u;}else{r[15]=0x2634u;}return 0;
        case 0x2634u: /* tst r1,#0x10 */
            temporary=r[1]&16u;logic_flags(state,temporary,0,16u);r[15]=0x2638u;return 0;
        case 0x2638u: /* movne r1,#0xf */
            if(!(state->cpsr&0x40000000u)){r[1]=15u;}r[15]=0x263cu;return 0;
        case 0x263cu: /* and r1,r1,#0x0 */
            r[1]=r[1]&0u;r[15]=0x2640u;return 0;
        case 0x2640u: /* orr r4,r4,r1, lsl #0x4 */
            r[4]=r[4]|(r[1]<<4);r[15]=0x2644u;return 0;
        case 0x2644u: /* strb r4,[r8,#0x20] */
            STORE(r[8]+32u,r[4],1);r[15]=0x2648u;return 0;
        case 0x2648u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x264cu;return 0;
        case 0x264cu: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x2650u;return 0;
        case 0x2650u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x2654u;return 0;
        case 0x2654u: /* bne 0x000025f0 */
            if(!(state->cpsr&0x40000000u)){r[15]=9712u;}else{r[15]=0x2658u;}return 0;
        case 0x2658u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2660u: /* ldrb r1,[r5,#0x24] */
            LOAD(r[1],r[5]+36u,1);r[15]=0x2664u;return 0;
        case 0x2664u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x2668u;return 0;
        case 0x2668u: /* bne 0x0000267c */
            if(!(state->cpsr&0x40000000u)){r[15]=9852u;}else{r[15]=0x266cu;}return 0;
        case 0x266cu: /* ldrb r1,[r5,#0x25] */
            LOAD(r[1],r[5]+37u,1);r[15]=0x2670u;return 0;
        case 0x2670u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x2674u;return 0;
        case 0x2674u: /* bne 0x00002688 */
            if(!(state->cpsr&0x40000000u)){r[15]=9864u;}else{r[15]=0x2678u;}return 0;
        case 0x2678u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x267cu: /* adds r1,r1,#0x1 */
            r[1]=addition(state,r[1],1u);r[15]=0x2680u;return 0;
        case 0x2680u: /* movcs r1,#0xff */
            if(state->cpsr&0x20000000u){r[1]=255u;}r[15]=0x2684u;return 0;
        case 0x2684u: /* b 0x00001b4c */
            r[15]=6988u;return 0;
        case 0x2688u: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x268cu;return 0;
        case 0x268cu: /* movcs r1,#0xff */
            if(state->cpsr&0x20000000u){r[1]=255u;}r[15]=0x2690u;return 0;
        case 0x2690u: /* b 0x00001b00 */
            r[15]=6912u;return 0;
        case 0x2694u: /* ldrb r1,[r5,#0x24] */
            LOAD(r[1],r[5]+36u,1);r[15]=0x2698u;return 0;
        case 0x2698u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x269cu;return 0;
        case 0x269cu: /* bne 0x000026b0 */
            if(!(state->cpsr&0x40000000u)){r[15]=9904u;}else{r[15]=0x26a0u;}return 0;
        case 0x26a0u: /* ldrb r0,[r5,#0x25] */
            LOAD(r[0],r[5]+37u,1);r[15]=0x26a4u;return 0;
        case 0x26a4u: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x26a8u;return 0;
        case 0x26a8u: /* bne 0x000026c4 */
            if(!(state->cpsr&0x40000000u)){r[15]=9924u;}else{r[15]=0x26acu;}return 0;
        case 0x26acu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x26b0u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x26b4u;return 0;
        case 0x26b4u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x26b8u;return 0;
        case 0x26b8u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x26bcu;return 0;
        case 0x26bcu: /* orr r1,r1,#0x80 */
            r[1]=r[1]|128u;r[15]=0x26c0u;return 0;
        case 0x26c0u: /* b 0x00001b4c */
            r[15]=6988u;return 0;
        case 0x26c4u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x26c8u;return 0;
        case 0x26c8u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x26ccu;return 0;
        case 0x26ccu: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x26d0u;return 0;
        case 0x26d0u: /* orr r1,r1,#0x80 */
            r[1]=r[1]|128u;r[15]=0x26d4u;return 0;
        case 0x26d4u: /* b 0x00001b00 */
            r[15]=6912u;return 0;
        case 0x26d8u: /* strb r1,[r5,#0x23] */
            STORE(r[5]+35u,r[1],1);r[15]=0x26dcu;return 0;
        case 0x26dcu: /* ldrb r1,[r5,#0x24] */
            LOAD(r[1],r[5]+36u,1);r[15]=0x26e0u;return 0;
        case 0x26e0u: /* orr r1,r1,#0x80 */
            r[1]=r[1]|128u;r[15]=0x26e4u;return 0;
        case 0x26e4u: /* strb r1,[r5,#0x24] */
            STORE(r[5]+36u,r[1],1);r[15]=0x26e8u;return 0;
        case 0x26e8u: /* ldrb r1,[r5,#0x25] */
            LOAD(r[1],r[5]+37u,1);r[15]=0x26ecu;return 0;
        case 0x26ecu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x26f0u;return 0;
        case 0x26f0u: /* strb r1,[r5,#0x25] */
            STORE(r[5]+37u,r[1],1);r[15]=0x26f4u;return 0;
        case 0x26f4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x26f8u: /* strb r1,[r5,#0x22] */
            STORE(r[5]+34u,r[1],1);r[15]=0x26fcu;return 0;
        case 0x26fcu: /* ldrb r1,[r5,#0x24] */
            LOAD(r[1],r[5]+36u,1);r[15]=0x2700u;return 0;
        case 0x2700u: /* orr r1,r1,#0x80 */
            r[1]=r[1]|128u;r[15]=0x2704u;return 0;
        case 0x2704u: /* strb r1,[r5,#0x24] */
            STORE(r[5]+36u,r[1],1);r[15]=0x2708u;return 0;
        case 0x2708u: /* ldrb r1,[r5,#0x25] */
            LOAD(r[1],r[5]+37u,1);r[15]=0x270cu;return 0;
        case 0x270cu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2710u;return 0;
        case 0x2710u: /* strb r1,[r5,#0x25] */
            STORE(r[5]+37u,r[1],1);r[15]=0x2714u;return 0;
        case 0x2714u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2718u: /* strb r1,[r5,#0x23] */
            STORE(r[5]+35u,r[1],1);r[15]=0x271cu;return 0;
        case 0x271cu: /* ldrb r1,[r5,#0x25] */
            LOAD(r[1],r[5]+37u,1);r[15]=0x2720u;return 0;
        case 0x2720u: /* orr r1,r1,#0x80 */
            r[1]=r[1]|128u;r[15]=0x2724u;return 0;
        case 0x2724u: /* strb r1,[r5,#0x25] */
            STORE(r[5]+37u,r[1],1);r[15]=0x2728u;return 0;
        case 0x2728u: /* ldrb r1,[r5,#0x24] */
            LOAD(r[1],r[5]+36u,1);r[15]=0x272cu;return 0;
        case 0x272cu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2730u;return 0;
        case 0x2730u: /* strb r1,[r5,#0x24] */
            STORE(r[5]+36u,r[1],1);r[15]=0x2734u;return 0;
        case 0x2734u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2738u: /* strb r1,[r5,#0x22] */
            STORE(r[5]+34u,r[1],1);r[15]=0x273cu;return 0;
        case 0x273cu: /* ldrb r1,[r5,#0x25] */
            LOAD(r[1],r[5]+37u,1);r[15]=0x2740u;return 0;
        case 0x2740u: /* orr r1,r1,#0x80 */
            r[1]=r[1]|128u;r[15]=0x2744u;return 0;
        case 0x2744u: /* strb r1,[r5,#0x25] */
            STORE(r[5]+37u,r[1],1);r[15]=0x2748u;return 0;
        case 0x2748u: /* ldrb r1,[r5,#0x24] */
            LOAD(r[1],r[5]+36u,1);r[15]=0x274cu;return 0;
        case 0x274cu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2750u;return 0;
        case 0x2750u: /* strb r1,[r5,#0x24] */
            STORE(r[5]+36u,r[1],1);r[15]=0x2754u;return 0;
        case 0x2754u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2758u: /* mov r0,#0x10 */
            r[0]=16u;r[15]=0x275cu;return 0;
        case 0x275cu: /* mov r1,#0x64 */
            r[1]=100u;r[15]=0x2760u;return 0;
        case 0x2760u: /* strb r1,[r6,#0x8] */
            STORE(r[6]+8u,r[1],1);r[15]=0x2764u;return 0;
        case 0x2764u: /* strb r1,[r6,#0xa] */
            STORE(r[6]+10u,r[1],1);r[15]=0x2768u;return 0;
        case 0x2768u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x276cu;return 0;
        case 0x276cu: /* strb r1,[r6,#0x9] */
            STORE(r[6]+9u,r[1],1);r[15]=0x2770u;return 0;
        case 0x2770u: /* mov r1,#0xc0 */
            r[1]=192u;r[15]=0x2774u;return 0;
        case 0x2774u: /* strb r1,[r6,#0xb] */
            STORE(r[6]+11u,r[1],1);r[15]=0x2778u;return 0;
        case 0x2778u: /* mov r1,#0x40 */
            r[1]=64u;r[15]=0x277cu;return 0;
        case 0x277cu: /* strb r1,[r6,#0x27] */
            STORE(r[6]+39u,r[1],1);r[15]=0x2780u;return 0;
        case 0x2780u: /* mov r1,#0x20 */
            r[1]=32u;r[15]=0x2784u;return 0;
        case 0x2784u: /* strb r1,[r6,#0x32] */
            STORE(r[6]+50u,r[1],1);r[15]=0x2788u;return 0;
        case 0x2788u: /* strb r1,[r6,#0x33] */
            STORE(r[6]+51u,r[1],1);r[15]=0x278cu;return 0;
        case 0x278cu: /* strb r1,[r6,#0x34] */
            STORE(r[6]+52u,r[1],1);r[15]=0x2790u;return 0;
        case 0x2790u: /* strb r1,[r6,#0x35] */
            STORE(r[6]+53u,r[1],1);r[15]=0x2794u;return 0;
        case 0x2794u: /* strb r1,[r6,#0x29] */
            STORE(r[6]+41u,r[1],1);r[15]=0x2798u;return 0;
        case 0x2798u: /* strb r1,[r6,#0x2b] */
            STORE(r[6]+43u,r[1],1);r[15]=0x279cu;return 0;
        case 0x279cu: /* strb r1,[r6,#0x2d] */
            STORE(r[6]+45u,r[1],1);r[15]=0x27a0u;return 0;
        case 0x27a0u: /* strb r1,[r6,#0x2f] */
            STORE(r[6]+47u,r[1],1);r[15]=0x27a4u;return 0;
        case 0x27a4u: /* strb r1,[r6,#0x31] */
            STORE(r[6]+49u,r[1],1);r[15]=0x27a8u;return 0;
        case 0x27a8u: /* strb r1,[r6,#0x37] */
            STORE(r[6]+55u,r[1],1);r[15]=0x27acu;return 0;
        case 0x27acu: /* strb r1,[r6,#0x38] */
            STORE(r[6]+56u,r[1],1);r[15]=0x27b0u;return 0;
        case 0x27b0u: /* strb r1,[r6,#0x3a] */
            STORE(r[6]+58u,r[1],1);r[15]=0x27b4u;return 0;
        case 0x27b4u: /* strb r1,[r6,#0x3b] */
            STORE(r[6]+59u,r[1],1);r[15]=0x27b8u;return 0;
        case 0x27b8u: /* strb r1,[r6,#0x39] */
            STORE(r[6]+57u,r[1],1);r[15]=0x27bcu;return 0;
        case 0x27bcu: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x27c0u;return 0;
        case 0x27c0u: /* str r1,[r6,#0x40] */
            STORE(r[6]+64u,r[1],4);r[15]=0x27c4u;return 0;
        case 0x27c4u: /* strb r1,[r6,#0x3] */
            STORE(r[6]+3u,r[1],1);r[15]=0x27c8u;return 0;
        case 0x27c8u: /* str r1,[r6,#0x4] */
            STORE(r[6]+4u,r[1],4);r[15]=0x27ccu;return 0;
        case 0x27ccu: /* strb r1,[r6,#0x2] */
            STORE(r[6]+2u,r[1],1);r[15]=0x27d0u;return 0;
        case 0x27d0u: /* strb r1,[r6,#0x28] */
            STORE(r[6]+40u,r[1],1);r[15]=0x27d4u;return 0;
        case 0x27d4u: /* strb r1,[r6,#0x2a] */
            STORE(r[6]+42u,r[1],1);r[15]=0x27d8u;return 0;
        case 0x27d8u: /* strb r1,[r6,#0x2c] */
            STORE(r[6]+44u,r[1],1);r[15]=0x27dcu;return 0;
        case 0x27dcu: /* strb r1,[r6,#0x2e] */
            STORE(r[6]+46u,r[1],1);r[15]=0x27e0u;return 0;
        case 0x27e0u: /* strb r1,[r6,#0x30] */
            STORE(r[6]+48u,r[1],1);r[15]=0x27e4u;return 0;
        case 0x27e4u: /* strb r1,[r6,#0x3d] */
            STORE(r[6]+61u,r[1],1);r[15]=0x27e8u;return 0;
        case 0x27e8u: /* strb r1,[r6,#0x26] */
            STORE(r[6]+38u,r[1],1);r[15]=0x27ecu;return 0;
        case 0x27ecu: /* strb r1,[r6,#0x36] */
            STORE(r[6]+54u,r[1],1);r[15]=0x27f0u;return 0;
        case 0x27f0u: /* add r6,r6,#0x48 */
            r[6]=r[6]+72u;r[15]=0x27f4u;return 0;
        case 0x27f4u: /* subs r0,r0,#0x1 */
            r[0]=subtract(state,r[0],1u);r[15]=0x27f8u;return 0;
        case 0x27f8u: /* bne 0x0000275c */
            if(!(state->cpsr&0x40000000u)){r[15]=10076u;}else{r[15]=0x27fcu;}return 0;
        case 0x27fcu: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x2800u;return 0;
        case 0x2800u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x2804u;return 0;
        case 0x2804u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x2808u;return 0;
        case 0x2808u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x280cu;return 0;
        case 0x280cu: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x2810u;return 0;
        case 0x2810u: /* beq 0x00002854 */
            if(state->cpsr&0x40000000u){r[15]=10324u;}else{r[15]=0x2814u;}return 0;
        case 0x2814u: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x2818u;return 0;
        case 0x2818u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x281cu;return 0;
        case 0x281cu: /* bne 0x00002854 */
            if(!(state->cpsr&0x40000000u)){r[15]=10324u;}else{r[15]=0x2820u;}return 0;
        case 0x2820u: /* ldrb r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,1);r[15]=0x2824u;return 0;
        case 0x2824u: /* bic r9,r9,#0x40 */
            r[9]=r[9]&~64u;r[15]=0x2828u;return 0;
        case 0x2828u: /* strb r9,[r6,#0x0] */
            STORE(r[6]+0u,r[9],1);r[15]=0x282cu;return 0;
        case 0x282cu: /* tst r9,#0x80 */
            temporary=r[9]&128u;logic_flags(state,temporary,0,128u);r[15]=0x2830u;return 0;
        case 0x2830u: /* bne 0x00002854 */
            if(!(state->cpsr&0x40000000u)){r[15]=10324u;}else{r[15]=0x2834u;}return 0;
        case 0x2834u: /* mov r9,#0x0 */
            r[9]=0u;r[15]=0x2838u;return 0;
        case 0x2838u: /* strb r9,[r6,#0x3f] */
            STORE(r[6]+63u,r[9],1);r[15]=0x283cu;return 0;
        case 0x283cu: /* ldr r9,[r8,#0x0] */
            LOAD(r[9],r[8]+0u,4);r[15]=0x2840u;return 0;
        case 0x2840u: /* bic r9,r9,#0xff000000 */
            r[9]=r[9]&~4278190080u;r[15]=0x2844u;return 0;
        case 0x2844u: /* bic r9,r9,#0xff0000 */
            r[9]=r[9]&~16711680u;r[15]=0x2848u;return 0;
        case 0x2848u: /* bic r9,r9,#0x4000 */
            r[9]=r[9]&~16384u;r[15]=0x284cu;return 0;
        case 0x284cu: /* orr r9,r9,#0x8000 */
            r[9]=r[9]|32768u;r[15]=0x2850u;return 0;
        case 0x2850u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x2854u;return 0;
        case 0x2854u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x2858u;return 0;
        case 0x2858u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x285cu;return 0;
        case 0x285cu: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x2860u;return 0;
        case 0x2860u: /* bne 0x00002808 */
            if(!(state->cpsr&0x40000000u)){r[15]=10248u;}else{r[15]=0x2864u;}return 0;
        case 0x2864u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5c20u: /* ldr r3,[0x6118] */
            LOAD(r[3],24856u,4);r[15]=0x5c24u;return 0;
        case 0x5c24u: /* ldr r1,[r3,#0x0] */
            LOAD(r[1],r[3]+0u,4);r[15]=0x5c28u;return 0;
        case 0x5c28u: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x5c2cu;return 0;
        case 0x5c2cu: /* ldreq r1,[0x611c] */
            if(state->cpsr&0x40000000u){LOAD(r[1],24860u,4);}r[15]=0x5c30u;return 0;
        case 0x5c30u: /* ldr r3,[r1,#0x0] */
            LOAD(r[3],r[1]+0u,4);r[15]=0x5c34u;return 0;
        case 0x5c34u: /* ldr r4,[0x5d20] */
            LOAD(r[4],23840u,4);r[15]=0x5c38u;return 0;
        case 0x5c38u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x5c3cu;return 0;
        case 0x5c3cu: /* movne r1,#0x2 */
            if(!(state->cpsr&0x40000000u)){r[1]=2u;}r[15]=0x5c40u;return 0;
        case 0x5c40u: /* bne 0x00005d3c */
            if(!(state->cpsr&0x40000000u)){r[15]=23868u;}else{r[15]=0x5c44u;}return 0;
        case 0x5c44u: /* ldrb r3,[r1,#0x4] */
            LOAD(r[3],r[1]+4u,1);r[15]=0x5c48u;return 0;
        case 0x5c48u: /* cmp r3,#0x1 */
            (void)subtract(state,r[3],1u);r[15]=0x5c4cu;return 0;
        case 0x5c4cu: /* movne r1,#0x8 */
            if(!(state->cpsr&0x40000000u)){r[1]=8u;}r[15]=0x5c50u;return 0;
        case 0x5c50u: /* bne 0x00005d3c */
            if(!(state->cpsr&0x40000000u)){r[15]=23868u;}else{r[15]=0x5c54u;}return 0;
        case 0x5c54u: /* ldr r2,[r1,#0x8] */
            LOAD(r[2],r[1]+8u,4);r[15]=0x5c58u;return 0;
        case 0x5c58u: /* sub r2,r2,#0x4 */
            r[2]=r[2]-4u;r[15]=0x5c5cu;return 0;
        case 0x5c5cu: /* ldr r3,[r1,r2] */
            LOAD(r[3],r[1]+r[2],4);r[15]=0x5c60u;return 0;
        case 0x5c60u: /* ldr r4,[0x5d24] */
            LOAD(r[4],23844u,4);r[15]=0x5c64u;return 0;
        case 0x5c64u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x5c68u;return 0;
        case 0x5c68u: /* movne r1,#0x4 */
            if(!(state->cpsr&0x40000000u)){r[1]=4u;}r[15]=0x5c6cu;return 0;
        case 0x5c6cu: /* bne 0x00005d3c */
            if(!(state->cpsr&0x40000000u)){r[15]=23868u;}else{r[15]=0x5c70u;}return 0;
        case 0x5c70u: /* ldrb r2,[r1,#0xc] */
            LOAD(r[2],r[1]+12u,1);r[15]=0x5c74u;return 0;
        case 0x5c74u: /* cmp r0,r2 */
            (void)subtract(state,r[0],r[2]);r[15]=0x5c78u;return 0;
        case 0x5c78u: /* movcs r1,#0x10 */
            if(state->cpsr&0x20000000u){r[1]=16u;}r[15]=0x5c7cu;return 0;
        case 0x5c7cu: /* bcs 0x00005d3c */
            if(state->cpsr&0x20000000u){r[15]=23868u;}else{r[15]=0x5c80u;}return 0;
        case 0x5c80u: /* adr r2,0x5d50 */
            r[2]=23888u;r[15]=0x5c84u;return 0;
        case 0x5c84u: /* strb r0,[r2,#0x0] */
            STORE(r[2]+0u,r[0],1);r[15]=0x5c88u;return 0;
        case 0x5c88u: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x5c8cu;return 0;
        case 0x5c8cu: /* add r0,r0,#0x10 */
            r[0]=r[0]+16u;r[15]=0x5c90u;return 0;
        case 0x5c90u: /* ldr r2,[r1,r0] */
            LOAD(r[2],r[1]+r[0],4);r[15]=0x5c94u;return 0;
        case 0x5c94u: /* add r2,r2,r1 */
            r[2]=r[2]+r[1];r[15]=0x5c98u;return 0;
        case 0x5c98u: /* ldrb r1,[r2,#0x20] */
            LOAD(r[1],r[2]+32u,1);r[15]=0x5c9cu;return 0;
        case 0x5c9cu: /* mov r1,r1, lsl #0x2 */
            r[1]=(r[1]<<2);r[15]=0x5ca0u;return 0;
        case 0x5ca0u: /* adr r3,0x5d28 */
            r[3]=23848u;r[15]=0x5ca4u;return 0;
        case 0x5ca4u: /* ldr r0,[r3,r1] */
            LOAD(r[0],r[3]+r[1],4);r[15]=0x5ca8u;return 0;
        case 0x5ca8u: /* ldr r3,[0x6120] */
            LOAD(r[3],24864u,4);r[15]=0x5cacu;return 0;
        case 0x5cacu: /* ldr r4,[r3,#0x0] */
            LOAD(r[4],r[3]+0u,4);r[15]=0x5cb0u;return 0;
        case 0x5cb0u: /* ldr r3,[r3,#0x4] */
            LOAD(r[3],r[3]+4u,4);r[15]=0x5cb4u;return 0;
        case 0x5cb4u: /* movs r4,r4 */
            r[4]=r[4];logic_flags(state,r[4],0,0);r[15]=0x5cb8u;return 0;
        case 0x5cb8u: /* moveq r1,#0x1 */
            if(state->cpsr&0x40000000u){r[1]=1u;}r[15]=0x5cbcu;return 0;
        case 0x5cbcu: /* beq 0x00005d3c */
            if(state->cpsr&0x40000000u){r[15]=23868u;}else{r[15]=0x5cc0u;}return 0;
        case 0x5cc0u: /* mov r5,#0x18000 */
            r[5]=98304u;r[15]=0x5cc4u;return 0;
        case 0x5cc4u: /* cmp r4,r5 */
            (void)subtract(state,r[4],r[5]);r[15]=0x5cc8u;return 0;
        case 0x5cc8u: /* movmi r1,#0x1 */
            if(state->cpsr&0x80000000u){r[1]=1u;}r[15]=0x5cccu;return 0;
        case 0x5cccu: /* bmi 0x00005d3c */
            if(state->cpsr&0x80000000u){r[15]=23868u;}else{r[15]=0x5cd0u;}return 0;
        case 0x5cd0u: /* ldrb r5,[r2,#0x23] */
            LOAD(r[5],r[2]+35u,1);r[15]=0x5cd4u;return 0;
        case 0x5cd4u: /* tst r5,#0x30 */
            temporary=r[5]&48u;logic_flags(state,temporary,0,48u);r[15]=0x5cd8u;return 0;
        case 0x5cd8u: /* bne 0x00005ce4 */
            if(!(state->cpsr&0x40000000u)){r[15]=23780u;}else{r[15]=0x5cdcu;}return 0;
        case 0x5cdcu: /* cmp r3,r0 */
            (void)subtract(state,r[3],r[0]);r[15]=0x5ce0u;return 0;
        case 0x5ce0u: /* bmi 0x00005d3c */
            if(state->cpsr&0x80000000u){r[15]=23868u;}else{r[15]=0x5ce4u;}return 0;
        case 0x5ce4u: /* mov r4,r4, lsr #0xb */
            r[4]=(r[4]>>11);r[15]=0x5ce8u;return 0;
        case 0x5ce8u: /* ldr r3,[0x5d38] */
            LOAD(r[3],23864u,4);r[15]=0x5cecu;return 0;
        case 0x5cecu: /* and r4,r4,r3 */
            r[4]=r[4]&r[3];r[15]=0x5cf0u;return 0;
        case 0x5cf0u: /* and r1,r1,#0xc */
            r[1]=r[1]&12u;r[15]=0x5cf4u;return 0;
        case 0x5cf4u: /* orr r1,r4,r1, lsl #0xb */
            r[1]=r[4]|(r[1]<<11);r[15]=0x5cf8u;return 0;
        case 0x5cf8u: /* ldr r0,[0x6124] */
            LOAD(r[0],24868u,4);r[15]=0x5cfcu;return 0;
        case 0x5cfcu: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x5d00u;return 0;
        case 0x5d00u: /* ldr r0,[0x6128] */
            LOAD(r[0],24872u,4);r[15]=0x5d04u;return 0;
        case 0x5d04u: /* str r2,[r0,#0x0] */
            STORE(r[0]+0u,r[2],4);r[15]=0x5d08u;return 0;
        case 0x5d08u: /* ldr r0,[0x612c] */
            LOAD(r[0],24876u,4);r[15]=0x5d0cu;return 0;
        case 0x5d0cu: /* mov r2,#0x1 */
            r[2]=1u;r[15]=0x5d10u;return 0;
        case 0x5d10u: /* strb r2,[r0,#0x0] */
            STORE(r[0]+0u,r[2],1);r[15]=0x5d14u;return 0;
        case 0x5d14u: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5d18u;return 0;
        case 0x5d18u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5d3cu: /* ldr r2,[0x6114] */
            LOAD(r[2],24852u,4);r[15]=0x5d40u;return 0;
        case 0x5d40u: /* ldrb r0,[r2,#0x0] */
            LOAD(r[0],r[2]+0u,1);r[15]=0x5d44u;return 0;
        case 0x5d44u: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0x5d48u;return 0;
        case 0x5d48u: /* strb r0,[r2,#0x0] */
            STORE(r[2]+0u,r[0],1);r[15]=0x5d4cu;return 0;
        case 0x5d4cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        default:*handled=0;return 0;
 }}else{switch(r[15]){
        case 0x1820u: /* add r5,r5,r6 */
            r[5]=r[5]+r[6];r[15]=0x1824u;return 0;
        case 0x1824u: /* mov r0,r1, lsr #0x6 */
            r[0]=(r[1]>>6);r[15]=0x1828u;return 0;
        case 0x1828u: /* and r0,r0,#0x1fc */
            r[0]=r[0]&508u;r[15]=0x182cu;return 0;
        case 0x182cu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1830u;return 0;
        case 0x1830u: /* adr r7,0x183c */
            r[7]=6204u;r[15]=0x1834u;return 0;
        case 0x1834u: /* add r7,r7,r0 */
            r[7]=r[7]+r[0];r[15]=0x1838u;return 0;
        case 0x1838u: /* mov pc,r7 */
            r[15]=r[7];return 0;
        case 0x183cu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1840u: /* b 0x00001a40 */
            r[15]=6720u;return 0;
        case 0x1844u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1848u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x184cu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1850u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1854u: /* b 0x00001ae4 */
            r[15]=6884u;return 0;
        case 0x1858u: /* b 0x00001b5c */
            r[15]=7004u;return 0;
        case 0x185cu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1860u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1864u: /* b 0x00001c5c */
            r[15]=7260u;return 0;
        case 0x1868u: /* b 0x00001ce8 */
            r[15]=7400u;return 0;
        case 0x186cu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1870u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1874u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1878u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x187cu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1880u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1884u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1888u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x188cu: /* b 0x00001cf8 */
            r[15]=7416u;return 0;
        case 0x1890u: /* b 0x00001d28 */
            r[15]=7464u;return 0;
        case 0x1894u: /* b 0x00001d58 */
            r[15]=7512u;return 0;
        case 0x1898u: /* b 0x00001d88 */
            r[15]=7560u;return 0;
        case 0x189cu: /* b 0x00001e48 */
            r[15]=7752u;return 0;
        case 0x18a0u: /* b 0x00001e78 */
            r[15]=7800u;return 0;
        case 0x18a4u: /* b 0x00001e90 */
            r[15]=7824u;return 0;
        case 0x18a8u: /* b 0x00001ea8 */
            r[15]=7848u;return 0;
        case 0x18acu: /* b 0x00001ec0 */
            r[15]=7872u;return 0;
        case 0x18b0u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18b4u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18b8u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18bcu: /* b 0x00001ed8 */
            r[15]=7896u;return 0;
        case 0x18c0u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18c4u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18c8u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18ccu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18d0u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18d4u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18d8u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18dcu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18e0u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18e4u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18e8u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18ecu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18f0u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18f4u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18f8u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x18fcu: /* b 0x00001f64 */
            r[15]=8036u;return 0;
        case 0x1900u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1904u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1908u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x190cu: /* b 0x00001f80 */
            r[15]=8064u;return 0;
        case 0x1910u: /* b 0x00001fa0 */
            r[15]=8096u;return 0;
        case 0x1914u: /* b 0x00001fc0 */
            r[15]=8128u;return 0;
        case 0x1918u: /* b 0x00001fe0 */
            r[15]=8160u;return 0;
        case 0x191cu: /* b 0x00002090 */
            r[15]=8336u;return 0;
        case 0x1920u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1924u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1928u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x192cu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1930u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1934u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1938u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x193cu: /* b 0x000020b0 */
            r[15]=8368u;return 0;
        case 0x1940u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1944u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1948u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x194cu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1950u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1954u: /* b 0x0000218c */
            r[15]=8588u;return 0;
        case 0x1958u: /* b 0x00002198 */
            r[15]=8600u;return 0;
        case 0x195cu: /* b 0x00002218 */
            r[15]=8728u;return 0;
        case 0x1960u: /* b 0x00002228 */
            r[15]=8744u;return 0;
        case 0x1964u: /* b 0x00002238 */
            r[15]=8760u;return 0;
        case 0x1968u: /* b 0x00002288 */
            r[15]=8840u;return 0;
        case 0x196cu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1970u: /* b 0x00002418 */
            r[15]=9240u;return 0;
        case 0x1974u: /* b 0x00002450 */
            r[15]=9296u;return 0;
        case 0x1978u: /* b 0x0000247c */
            r[15]=9340u;return 0;
        case 0x197cu: /* b 0x000024b8 */
            r[15]=9400u;return 0;
        case 0x1980u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1984u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1988u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x198cu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1990u: /* b 0x00002198 */
            r[15]=8600u;return 0;
        case 0x1994u: /* b 0x00002218 */
            r[15]=8728u;return 0;
        case 0x1998u: /* b 0x00002228 */
            r[15]=8744u;return 0;
        case 0x199cu: /* b 0x000023e8 */
            r[15]=9192u;return 0;
        case 0x19a0u: /* b 0x000023f8 */
            r[15]=9208u;return 0;
        case 0x19a4u: /* b 0x00002408 */
            r[15]=9224u;return 0;
        case 0x19a8u: /* b 0x00002528 */
            r[15]=9512u;return 0;
        case 0x19acu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x19b0u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x19b4u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x19b8u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x19bcu: /* b 0x00002668 */
            r[15]=9832u;return 0;
        case 0x19c0u: /* b 0x0000269c */
            r[15]=9884u;return 0;
        case 0x19c4u: /* b 0x000026e0 */
            r[15]=9952u;return 0;
        case 0x19c8u: /* b 0x00002700 */
            r[15]=9984u;return 0;
        case 0x19ccu: /* b 0x00002720 */
            r[15]=10016u;return 0;
        case 0x19d0u: /* b 0x00002740 */
            r[15]=10048u;return 0;
        case 0x19d4u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x19d8u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x19dcu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x19e0u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x19e4u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x19e8u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x19ecu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x19f0u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x19f4u: /* b 0x000022a0 */
            r[15]=8864u;return 0;
        case 0x19f8u: /* b 0x000022b4 */
            r[15]=8884u;return 0;
        case 0x19fcu: /* b 0x000022cc */
            r[15]=8908u;return 0;
        case 0x1a00u: /* b 0x000022e4 */
            r[15]=8932u;return 0;
        case 0x1a04u: /* b 0x000022fc */
            r[15]=8956u;return 0;
        case 0x1a08u: /* b 0x00002314 */
            r[15]=8980u;return 0;
        case 0x1a0cu: /* b 0x0000232c */
            r[15]=9004u;return 0;
        case 0x1a10u: /* b 0x00002344 */
            r[15]=9028u;return 0;
        case 0x1a14u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1a18u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1a1cu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1a20u: /* b 0x00002760 */
            r[15]=10080u;return 0;
        case 0x1a24u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1a28u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1a2cu: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1a30u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1a34u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1a38u: /* b 0x00001a3c */
            r[15]=6716u;return 0;
        case 0x1a3cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1a40u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x1a44u;return 0;
        case 0x1a44u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x1a48u;return 0;
        case 0x1a48u: /* mov r11,#0x800000 */
            r[11]=8388608u;r[15]=0x1a4cu;return 0;
        case 0x1a4cu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1a50u;return 0;
        case 0x1a50u: /* strb r1,[r5,#0x2] */
            STORE(r[5]+2u,r[1],1);r[15]=0x1a54u;return 0;
        case 0x1a54u: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x1a58u;return 0;
        case 0x1a58u: /* ldrb r8,[r6,#0x4] */
            LOAD(r[8],r[6]+4u,1);r[15]=0x1a5cu;return 0;
        case 0x1a5cu: /* cmp r8,r3 */
            (void)subtract(state,r[8],r[3]);r[15]=0x1a60u;return 0;
        case 0x1a60u: /* bne 0x00001ac8 */
            if(!(state->cpsr&0x40000000u)){r[15]=6856u;}else{r[15]=0x1a64u;}return 0;
        case 0x1a64u: /* ldrb r8,[r6,#0x5] */
            LOAD(r[8],r[6]+5u,1);r[15]=0x1a68u;return 0;
        case 0x1a68u: /* cmp r8,r2 */
            (void)subtract(state,r[8],r[2]);r[15]=0x1a6cu;return 0;
        case 0x1a6cu: /* bne 0x00001ac8 */
            if(!(state->cpsr&0x40000000u)){r[15]=6856u;}else{r[15]=0x1a70u;}return 0;
        case 0x1a70u: /* ldr r9,[r6,#0x10] */
            LOAD(r[9],r[6]+16u,4);r[15]=0x1a74u;return 0;
        case 0x1a74u: /* ldr r8,[r9,#0xc] */
            LOAD(r[8],r[9]+12u,4);r[15]=0x1a78u;return 0;
        case 0x1a78u: /* mov r8,r8, lsr #0x10 */
            r[8]=(r[8]>>16);r[15]=0x1a7cu;return 0;
        case 0x1a7cu: /* mov r9,r8, lsr #0x5 */
            r[9]=(r[8]>>5);r[15]=0x1a80u;return 0;
        case 0x1a80u: /* and r9,r9,#0x7 */
            r[9]=r[9]&7u;r[15]=0x1a84u;return 0;
        case 0x1a84u: /* add r9,r9,#0x1 */
            r[9]=r[9]+1u;r[15]=0x1a88u;return 0;
        case 0x1a88u: /* mul r9,r1,r9 */
            r[9]=r[1]*r[9];r[15]=0x1a8cu;return 0;
        case 0x1a8cu: /* sub r9,r9,#0x1 */
            r[9]=r[9]-1u;r[15]=0x1a90u;return 0;
        case 0x1a90u: /* mov r9,r9, lsr #0x2 */
            r[9]=(r[9]>>2);r[15]=0x1a94u;return 0;
        case 0x1a94u: /* and r4,r9,#0xe0 */
            r[4]=r[9]&224u;r[15]=0x1a98u;return 0;
        case 0x1a98u: /* and r9,r8,#0x7 */
            r[9]=r[8]&7u;r[15]=0x1a9cu;return 0;
        case 0x1a9cu: /* add r9,r9,#0x1 */
            r[9]=r[9]+1u;r[15]=0x1aa0u;return 0;
        case 0x1aa0u: /* mul r9,r1,r9 */
            r[9]=r[1]*r[9];r[15]=0x1aa4u;return 0;
        case 0x1aa4u: /* sub r9,r9,#0x1 */
            r[9]=r[9]-1u;r[15]=0x1aa8u;return 0;
        case 0x1aa8u: /* mov r9,r9, lsr #0x7 */
            r[9]=(r[9]>>7);r[15]=0x1aacu;return 0;
        case 0x1aacu: /* and r5,r9,#0x7 */
            r[5]=r[9]&7u;r[15]=0x1ab0u;return 0;
        case 0x1ab0u: /* ldr r9,[0x1ae0] */
            LOAD(r[9],6880u,4);r[15]=0x1ab4u;return 0;
        case 0x1ab4u: /* and r8,r8,r9 */
            r[8]=r[8]&r[9];r[15]=0x1ab8u;return 0;
        case 0x1ab8u: /* orr r8,r8,r4 */
            r[8]=r[8]|r[4];r[15]=0x1abcu;return 0;
        case 0x1abcu: /* orr r8,r8,r5 */
            r[8]=r[8]|r[5];r[15]=0x1ac0u;return 0;
        case 0x1ac0u: /* bic r8,r8,#0x8000 */
            r[8]=r[8]&~32768u;r[15]=0x1ac4u;return 0;
        case 0x1ac4u: /* str r8,[r11,#0x1c] */
            STORE(r[11]+28u,r[8],4);r[15]=0x1ac8u;return 0;
        case 0x1ac8u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x1accu;return 0;
        case 0x1accu: /* add r11,r11,#0x80 */
            r[11]=r[11]+128u;r[15]=0x1ad0u;return 0;
        case 0x1ad0u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x1ad4u;return 0;
        case 0x1ad4u: /* bne 0x00001a58 */
            if(!(state->cpsr&0x40000000u)){r[15]=6744u;}else{r[15]=0x1ad8u;}return 0;
        case 0x1ad8u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1ae4u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1ae8u;return 0;
        case 0x1ae8u: /* orr r1,r1,#0x80 */
            r[1]=r[1]|128u;r[15]=0x1aecu;return 0;
        case 0x1aecu: /* ldrb r0,[r5,#0x24] */
            LOAD(r[0],r[5]+36u,1);r[15]=0x1af0u;return 0;
        case 0x1af0u: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x1af4u;return 0;
        case 0x1af4u: /* bne 0x00001b54 */
            if(!(state->cpsr&0x40000000u)){r[15]=6996u;}else{r[15]=0x1af8u;}return 0;
        case 0x1af8u: /* ldrb r0,[r5,#0x25] */
            LOAD(r[0],r[5]+37u,1);r[15]=0x1afcu;return 0;
        case 0x1afcu: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x1b00u;return 0;
        case 0x1b00u: /* bne 0x00001b08 */
            if(!(state->cpsr&0x40000000u)){r[15]=6920u;}else{r[15]=0x1b04u;}return 0;
        case 0x1b04u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1b08u: /* strb r1,[r5,#0x25] */
            STORE(r[5]+37u,r[1],1);r[15]=0x1b0cu;return 0;
        case 0x1b0cu: /* ldrb r6,[r5,#0x20] */
            LOAD(r[6],r[5]+32u,1);r[15]=0x1b10u;return 0;
        case 0x1b10u: /* tst r6,#0x7f */
            temporary=r[6]&127u;logic_flags(state,temporary,0,127u);r[15]=0x1b14u;return 0;
        case 0x1b14u: /* beq 0x00001b3c */
            if(state->cpsr&0x40000000u){r[15]=6972u;}else{r[15]=0x1b18u;}return 0;
        case 0x1b18u: /* cmp r6,#0x7f */
            (void)subtract(state,r[6],127u);r[15]=0x1b1cu;return 0;
        case 0x1b1cu: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x1b20u;}return 0;
        case 0x1b20u: /* ldrb r6,[r5,#0x21] */
            LOAD(r[6],r[5]+33u,1);r[15]=0x1b24u;return 0;
        case 0x1b24u: /* cmp r6,#0x7f */
            (void)subtract(state,r[6],127u);r[15]=0x1b28u;return 0;
        case 0x1b28u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x1b2cu;}return 0;
        case 0x1b2cu: /* ldrb r0,[r5,#0x25] */
            LOAD(r[0],r[5]+37u,1);r[15]=0x1b30u;return 0;
        case 0x1b30u: /* and r0,r0,#0x7f */
            r[0]=r[0]&127u;r[15]=0x1b34u;return 0;
        case 0x1b34u: /* strb r0,[r5,#0x25] */
            STORE(r[5]+37u,r[0],1);r[15]=0x1b38u;return 0;
        case 0x1b38u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1b3cu: /* ldrb r6,[r5,#0x21] */
            LOAD(r[6],r[5]+33u,1);r[15]=0x1b40u;return 0;
        case 0x1b40u: /* tst r6,#0x7f */
            temporary=r[6]&127u;logic_flags(state,temporary,0,127u);r[15]=0x1b44u;return 0;
        case 0x1b44u: /* beq 0x00001b4c */
            if(state->cpsr&0x40000000u){r[15]=6988u;}else{r[15]=0x1b48u;}return 0;
        case 0x1b48u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1b4cu: /* strb r1,[r5,#0x3] */
            STORE(r[5]+3u,r[1],1);r[15]=0x1b50u;return 0;
        case 0x1b50u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1b54u: /* strb r1,[r5,#0x24] */
            STORE(r[5]+36u,r[1],1);r[15]=0x1b58u;return 0;
        case 0x1b58u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1b5cu: /* strb r1,[r5,#0x8] */
            STORE(r[5]+8u,r[1],1);r[15]=0x1b60u;return 0;
        case 0x1b60u: /* ldrb r6,[r5,#0x9] */
            LOAD(r[6],r[5]+9u,1);r[15]=0x1b64u;return 0;
        case 0x1b64u: /* ldr r8,[0x2884] */
            LOAD(r[8],10372u,4);r[15]=0x1b68u;return 0;
        case 0x1b68u: /* ldrb r1,[r8,r1] */
            LOAD(r[1],r[8]+r[1],1);r[15]=0x1b6cu;return 0;
        case 0x1b6cu: /* ldrb r6,[r8,r6] */
            LOAD(r[6],r[8]+r[6],1);r[15]=0x1b70u;return 0;
        case 0x1b70u: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x1b74u;return 0;
        case 0x1b74u: /* add r6,r6,#0x1 */
            r[6]=r[6]+1u;r[15]=0x1b78u;return 0;
        case 0x1b78u: /* mul r7,r1,r6 */
            r[7]=r[1]*r[6];r[15]=0x1b7cu;return 0;
        case 0x1b7cu: /* mov r7,r7, lsr #0x7 */
            r[7]=(r[7]>>7);r[15]=0x1b80u;return 0;
        case 0x1b80u: /* sub r7,r7,#0x1 */
            r[7]=r[7]-1u;r[15]=0x1b84u;return 0;
        case 0x1b84u: /* strb r7,[r5,#0xa] */
            STORE(r[5]+10u,r[7],1);r[15]=0x1b88u;return 0;
        case 0x1b88u: /* add r7,r7,#0x1 */
            r[7]=r[7]+1u;r[15]=0x1b8cu;return 0;
        case 0x1b8cu: /* mov r6,#0x80 */
            r[6]=128u;r[15]=0x1b90u;return 0;
        case 0x1b90u: /* mov r8,#0xc000 */
            r[8]=49152u;r[15]=0x1b94u;return 0;
        case 0x1b94u: /* mla r10,r3,r6,r8 */
            r[10]=r[3]*r[6]+r[8];r[15]=0x1b98u;return 0;
        case 0x1b98u: /* ldrb r10,[r10,#0x1] */
            LOAD(r[10],r[10]+1u,1);r[15]=0x1b9cu;return 0;
        case 0x1b9cu: /* sub r10,r10,#0x80 */
            r[10]=r[10]-128u;r[15]=0x1ba0u;return 0;
        case 0x1ba0u: /* mov r8,#0xe800 */
            r[8]=59392u;r[15]=0x1ba4u;return 0;
        case 0x1ba4u: /* mov r9,#0x30 */
            r[9]=48u;r[15]=0x1ba8u;return 0;
        case 0x1ba8u: /* mov r11,#0x800000 */
            r[11]=8388608u;r[15]=0x1bacu;return 0;
        case 0x1bacu: /* ldr r6,[r8,#0x0] */
            LOAD(r[6],r[8]+0u,4);r[15]=0x1bb0u;return 0;
        case 0x1bb0u: /* movs r6,r6 */
            r[6]=r[6];logic_flags(state,r[6],0,0);r[15]=0x1bb4u;return 0;
        case 0x1bb4u: /* beq 0x00001c48 */
            if(state->cpsr&0x40000000u){r[15]=7240u;}else{r[15]=0x1bb8u;}return 0;
        case 0x1bb8u: /* ldrb r6,[r8,#0x4] */
            LOAD(r[6],r[8]+4u,1);r[15]=0x1bbcu;return 0;
        case 0x1bbcu: /* cmp r6,r3 */
            (void)subtract(state,r[6],r[3]);r[15]=0x1bc0u;return 0;
        case 0x1bc0u: /* bne 0x00001c48 */
            if(!(state->cpsr&0x40000000u)){r[15]=7240u;}else{r[15]=0x1bc4u;}return 0;
        case 0x1bc4u: /* ldrb r6,[r8,#0x5] */
            LOAD(r[6],r[8]+5u,1);r[15]=0x1bc8u;return 0;
        case 0x1bc8u: /* cmp r6,r2 */
            (void)subtract(state,r[6],r[2]);r[15]=0x1bccu;return 0;
        case 0x1bccu: /* bne 0x00001c48 */
            if(!(state->cpsr&0x40000000u)){r[15]=7240u;}else{r[15]=0x1bd0u;}return 0;
        case 0x1bd0u: /* ldrb r1,[r8,#0x1c] */
            LOAD(r[1],r[8]+28u,1);r[15]=0x1bd4u;return 0;
        case 0x1bd4u: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x1bd8u;return 0;
        case 0x1bd8u: /* ldr r6,[r5,#0x18] */
            LOAD(r[6],r[5]+24u,4);r[15]=0x1bdcu;return 0;
        case 0x1bdcu: /* add r1,r1,r6 */
            r[1]=r[1]+r[6];r[15]=0x1be0u;return 0;
        case 0x1be0u: /* ldrb r6,[r8,#0x7] */
            LOAD(r[6],r[8]+7u,1);r[15]=0x1be4u;return 0;
        case 0x1be4u: /* ldrb r6,[r1,r6] */
            LOAD(r[6],r[1]+r[6],1);r[15]=0x1be8u;return 0;
        case 0x1be8u: /* add r6,r6,#0x1 */
            r[6]=r[6]+1u;r[15]=0x1becu;return 0;
        case 0x1becu: /* ldrb r7,[r5,#0xa] */
            LOAD(r[7],r[5]+10u,1);r[15]=0x1bf0u;return 0;
        case 0x1bf0u: /* add r7,r7,#0x1 */
            r[7]=r[7]+1u;r[15]=0x1bf4u;return 0;
        case 0x1bf4u: /* mul r12,r6,r7 */
            r[12]=r[6]*r[7];r[15]=0x1bf8u;return 0;
        case 0x1bf8u: /* ldr r6,[r8,#0x10] */
            LOAD(r[6],r[8]+16u,4);r[15]=0x1bfcu;return 0;
        case 0x1bfcu: /* ldrb r7,[r6,#0x15] */
            LOAD(r[7],r[6]+21u,1);r[15]=0x1c00u;return 0;
        case 0x1c00u: /* eor r7,r7,#0xff */
            r[7]=r[7]^255u;r[15]=0x1c04u;return 0;
        case 0x1c04u: /* add r7,r7,#0x1 */
            r[7]=r[7]+1u;r[15]=0x1c08u;return 0;
        case 0x1c08u: /* mul r12,r7,r12 */
            r[12]=r[7]*r[12];r[15]=0x1c0cu;return 0;
        case 0x1c0cu: /* sub r12,r12,#0x1 */
            r[12]=r[12]-1u;r[15]=0x1c10u;return 0;
        case 0x1c10u: /* mov r12,r12, lsr #0xe */
            r[12]=(r[12]>>14);r[15]=0x1c14u;return 0;
        case 0x1c14u: /* and r12,r12,#0xff */
            r[12]=r[12]&255u;r[15]=0x1c18u;return 0;
        case 0x1c18u: /* strb r12,[r8,#0x8] */
            STORE(r[8]+8u,r[12],1);r[15]=0x1c1cu;return 0;
        case 0x1c1cu: /* adds r6,r10,r12 */
            r[6]=addition(state,r[10],r[12]);r[15]=0x1c20u;return 0;
        case 0x1c20u: /* movmi r6,#0x0 */
            if(state->cpsr&0x80000000u){r[6]=0u;}r[15]=0x1c24u;return 0;
        case 0x1c24u: /* bmi 0x00001c30 */
            if(state->cpsr&0x80000000u){r[15]=7216u;}else{r[15]=0x1c28u;}return 0;
        case 0x1c28u: /* tst r6,#0x100 */
            temporary=r[6]&256u;logic_flags(state,temporary,1,256u);r[15]=0x1c2cu;return 0;
        case 0x1c2cu: /* movne r6,#0xff */
            if(!(state->cpsr&0x40000000u)){r[6]=255u;}r[15]=0x1c30u;return 0;
        case 0x1c30u: /* eor r6,r6,#0xff */
            r[6]=r[6]^255u;r[15]=0x1c34u;return 0;
        case 0x1c34u: /* and r6,r6,#0xff */
            r[6]=r[6]&255u;r[15]=0x1c38u;return 0;
        case 0x1c38u: /* ldr r4,[r11,#0x28] */
            LOAD(r[4],r[11]+40u,4);r[15]=0x1c3cu;return 0;
        case 0x1c3cu: /* and r4,r4,#0x3f */
            r[4]=r[4]&63u;r[15]=0x1c40u;return 0;
        case 0x1c40u: /* orr r4,r4,r6, lsl #0x8 */
            r[4]=r[4]|(r[6]<<8);r[15]=0x1c44u;return 0;
        case 0x1c44u: /* str r4,[r11,#0x28] */
            STORE(r[11]+40u,r[4],4);r[15]=0x1c48u;return 0;
        case 0x1c48u: /* add r8,r8,#0x40 */
            r[8]=r[8]+64u;r[15]=0x1c4cu;return 0;
        case 0x1c4cu: /* add r11,r11,#0x80 */
            r[11]=r[11]+128u;r[15]=0x1c50u;return 0;
        case 0x1c50u: /* subs r9,r9,#0x1 */
            r[9]=subtract(state,r[9],1u);r[15]=0x1c54u;return 0;
        case 0x1c54u: /* bne 0x00001bac */
            if(!(state->cpsr&0x40000000u)){r[15]=7084u;}else{r[15]=0x1c58u;}return 0;
        case 0x1c58u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1c5cu: /* strb r1,[r5,#0xb] */
            STORE(r[5]+11u,r[1],1);r[15]=0x1c60u;return 0;
        case 0x1c60u: /* mov r6,#0x80 */
            r[6]=128u;r[15]=0x1c64u;return 0;
        case 0x1c64u: /* mov r7,#0xc000 */
            r[7]=49152u;r[15]=0x1c68u;return 0;
        case 0x1c68u: /* mla r7,r3,r6,r7 */
            r[7]=r[3]*r[6]+r[7];r[15]=0x1c6cu;return 0;
        case 0x1c6cu: /* ldrb r6,[r7,#0x7] */
            LOAD(r[6],r[7]+7u,1);r[15]=0x1c70u;return 0;
        case 0x1c70u: /* and r6,r6,#0xff */
            r[6]=r[6]&255u;r[15]=0x1c74u;return 0;
        case 0x1c74u: /* sub r6,r6,#0x80 */
            r[6]=r[6]-128u;r[15]=0x1c78u;return 0;
        case 0x1c78u: /* adds r1,r1,r6 */
            r[1]=addition(state,r[1],r[6]);r[15]=0x1c7cu;return 0;
        case 0x1c7cu: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x1c80u;return 0;
        case 0x1c80u: /* bmi 0x00001c8c */
            if(state->cpsr&0x80000000u){r[15]=7308u;}else{r[15]=0x1c84u;}return 0;
        case 0x1c84u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x1c88u;return 0;
        case 0x1c88u: /* movne r1,#0x7f */
            if(!(state->cpsr&0x40000000u)){r[1]=127u;}r[15]=0x1c8cu;return 0;
        case 0x1c8cu: /* mov r1,r1, lsr #0x2 */
            r[1]=(r[1]>>2);r[15]=0x1c90u;return 0;
        case 0x1c90u: /* subs r1,r1,#0x10 */
            r[1]=subtract(state,r[1],16u);r[15]=0x1c94u;return 0;
        case 0x1c94u: /* eormi r1,r1,#0xff */
            if(state->cpsr&0x80000000u){r[1]=r[1]^255u;}r[15]=0x1c98u;return 0;
        case 0x1c98u: /* addmi r1,r1,#0x10 */
            if(state->cpsr&0x80000000u){r[1]=r[1]+16u;}r[15]=0x1c9cu;return 0;
        case 0x1c9cu: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0x1ca0u;return 0;
        case 0x1ca0u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x1ca4u;return 0;
        case 0x1ca4u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x1ca8u;return 0;
        case 0x1ca8u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x1cacu;return 0;
        case 0x1cacu: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x1cb0u;return 0;
        case 0x1cb0u: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x1cb4u;return 0;
        case 0x1cb4u: /* beq 0x00001cd4 */
            if(state->cpsr&0x40000000u){r[15]=7380u;}else{r[15]=0x1cb8u;}return 0;
        case 0x1cb8u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x1cbcu;return 0;
        case 0x1cbcu: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x1cc0u;return 0;
        case 0x1cc0u: /* bne 0x00001cd4 */
            if(!(state->cpsr&0x40000000u)){r[15]=7380u;}else{r[15]=0x1cc4u;}return 0;
        case 0x1cc4u: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x1cc8u;return 0;
        case 0x1cc8u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x1cccu;return 0;
        case 0x1cccu: /* bne 0x00001cd4 */
            if(!(state->cpsr&0x40000000u)){r[15]=7380u;}else{r[15]=0x1cd0u;}return 0;
        case 0x1cd0u: /* strb r1,[r8,#0x24] */
            STORE(r[8]+36u,r[1],1);r[15]=0x1cd4u;return 0;
        case 0x1cd4u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x1cd8u;return 0;
        case 0x1cd8u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x1cdcu;return 0;
        case 0x1cdcu: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x1ce0u;return 0;
        case 0x1ce0u: /* bne 0x00001cac */
            if(!(state->cpsr&0x40000000u)){r[15]=7340u;}else{r[15]=0x1ce4u;}return 0;
        case 0x1ce4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1ce8u: /* strb r1,[r5,#0x9] */
            STORE(r[5]+9u,r[1],1);r[15]=0x1cecu;return 0;
        case 0x1cecu: /* mov r6,#0x0 */
            r[6]=0u;r[15]=0x1cf0u;return 0;
        case 0x1cf0u: /* ldrb r6,[r5,#0x8] */
            LOAD(r[6],r[5]+8u,1);r[15]=0x1cf4u;return 0;
        case 0x1cf4u: /* b 0x00001b64 */
            r[15]=7012u;return 0;
        case 0x1cf8u: /* ldrb r6,[r5,#0x28] */
            LOAD(r[6],r[5]+40u,1);r[15]=0x1cfcu;return 0;
        case 0x1cfcu: /* and r6,r6,#0x7f */
            r[6]=r[6]&127u;r[15]=0x1d00u;return 0;
        case 0x1d00u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1d04u;return 0;
        case 0x1d04u: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x1d08u;return 0;
        case 0x1d08u: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1d0cu;return 0;
        case 0x1d0cu: /* mov r6,r1, lsr #0x8 */
            r[6]=(r[1]>>8);r[15]=0x1d10u;return 0;
        case 0x1d10u: /* strb r6,[r5,#0x29] */
            STORE(r[5]+41u,r[6],1);r[15]=0x1d14u;return 0;
        case 0x1d14u: /* and r6,r1,#0xff */
            r[6]=r[1]&255u;r[15]=0x1d18u;return 0;
        case 0x1d18u: /* strb r6,[r5,#0x28] */
            STORE(r[5]+40u,r[6],1);r[15]=0x1d1cu;return 0;
        case 0x1d1cu: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1d20u;return 0;
        case 0x1d20u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1d24u;return 0;
        case 0x1d24u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1d28u: /* ldrb r6,[r5,#0x2a] */
            LOAD(r[6],r[5]+42u,1);r[15]=0x1d2cu;return 0;
        case 0x1d2cu: /* and r6,r6,#0x7f */
            r[6]=r[6]&127u;r[15]=0x1d30u;return 0;
        case 0x1d30u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1d34u;return 0;
        case 0x1d34u: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x1d38u;return 0;
        case 0x1d38u: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1d3cu;return 0;
        case 0x1d3cu: /* mov r6,r1, lsr #0x8 */
            r[6]=(r[1]>>8);r[15]=0x1d40u;return 0;
        case 0x1d40u: /* strb r6,[r5,#0x2b] */
            STORE(r[5]+43u,r[6],1);r[15]=0x1d44u;return 0;
        case 0x1d44u: /* and r6,r1,#0xff */
            r[6]=r[1]&255u;r[15]=0x1d48u;return 0;
        case 0x1d48u: /* strb r6,[r5,#0x2a] */
            STORE(r[5]+42u,r[6],1);r[15]=0x1d4cu;return 0;
        case 0x1d4cu: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1d50u;return 0;
        case 0x1d50u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1d54u;return 0;
        case 0x1d54u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1d58u: /* ldrb r6,[r5,#0x2c] */
            LOAD(r[6],r[5]+44u,1);r[15]=0x1d5cu;return 0;
        case 0x1d5cu: /* and r6,r6,#0x7f */
            r[6]=r[6]&127u;r[15]=0x1d60u;return 0;
        case 0x1d60u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1d64u;return 0;
        case 0x1d64u: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x1d68u;return 0;
        case 0x1d68u: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1d6cu;return 0;
        case 0x1d6cu: /* mov r6,r1, lsr #0x8 */
            r[6]=(r[1]>>8);r[15]=0x1d70u;return 0;
        case 0x1d70u: /* strb r6,[r5,#0x2d] */
            STORE(r[5]+45u,r[6],1);r[15]=0x1d74u;return 0;
        case 0x1d74u: /* and r6,r1,#0xff */
            r[6]=r[1]&255u;r[15]=0x1d78u;return 0;
        case 0x1d78u: /* strb r6,[r5,#0x2c] */
            STORE(r[5]+44u,r[6],1);r[15]=0x1d7cu;return 0;
        case 0x1d7cu: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1d80u;return 0;
        case 0x1d80u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1d84u;return 0;
        case 0x1d84u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1d88u: /* ldrb r6,[r5,#0x2e] */
            LOAD(r[6],r[5]+46u,1);r[15]=0x1d8cu;return 0;
        case 0x1d8cu: /* and r6,r6,#0x7f */
            r[6]=r[6]&127u;r[15]=0x1d90u;return 0;
        case 0x1d90u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1d94u;return 0;
        case 0x1d94u: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x1d98u;return 0;
        case 0x1d98u: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1d9cu;return 0;
        case 0x1d9cu: /* mov r6,r1, lsr #0x8 */
            r[6]=(r[1]>>8);r[15]=0x1da0u;return 0;
        case 0x1da0u: /* strb r6,[r5,#0x2f] */
            STORE(r[5]+47u,r[6],1);r[15]=0x1da4u;return 0;
        case 0x1da4u: /* and r6,r1,#0xff */
            r[6]=r[1]&255u;r[15]=0x1da8u;return 0;
        case 0x1da8u: /* strb r6,[r5,#0x2e] */
            STORE(r[5]+46u,r[6],1);r[15]=0x1dacu;return 0;
        case 0x1dacu: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x1db0u;return 0;
        case 0x1db0u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x1db4u;return 0;
        case 0x1db4u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x1db8u;return 0;
        case 0x1db8u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x1dbcu;return 0;
        case 0x1dbcu: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x1dc0u;return 0;
        case 0x1dc0u: /* beq 0x00001e2c */
            if(state->cpsr&0x40000000u){r[15]=7724u;}else{r[15]=0x1dc4u;}return 0;
        case 0x1dc4u: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x1dc8u;return 0;
        case 0x1dc8u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x1dccu;return 0;
        case 0x1dccu: /* bne 0x00001e2c */
            if(!(state->cpsr&0x40000000u)){r[15]=7724u;}else{r[15]=0x1dd0u;}return 0;
        case 0x1dd0u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x1dd4u;return 0;
        case 0x1dd4u: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x1dd8u;return 0;
        case 0x1dd8u: /* bne 0x00001e2c */
            if(!(state->cpsr&0x40000000u)){r[15]=7724u;}else{r[15]=0x1ddcu;}return 0;
        case 0x1ddcu: /* ldr r9,[r6,#0x10] */
            LOAD(r[9],r[6]+16u,4);r[15]=0x1de0u;return 0;
        case 0x1de0u: /* ldr r0,[r9,#0x1c] */
            LOAD(r[0],r[9]+28u,4);r[15]=0x1de4u;return 0;
        case 0x1de4u: /* ldr r9,[r5,#0x2c] */
            LOAD(r[9],r[5]+44u,4);r[15]=0x1de8u;return 0;
        case 0x1de8u: /* mov r9,r9, ror #0x10 */
            r[9]=((r[9]>>16)|(r[9]<<16));r[15]=0x1decu;return 0;
        case 0x1decu: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x1df0u;return 0;
        case 0x1df0u: /* orr r1,r1,#0xff00 */
            r[1]=r[1]|65280u;r[15]=0x1df4u;return 0;
        case 0x1df4u: /* and r9,r9,r1 */
            r[9]=r[9]&r[1];r[15]=0x1df8u;return 0;
        case 0x1df8u: /* sub r9,r9,#0x2000 */
            r[9]=r[9]-8192u;r[15]=0x1dfcu;return 0;
        case 0x1dfcu: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0x1e00u;return 0;
        case 0x1e00u: /* add r1,r1,r9 */
            r[1]=r[1]+r[9];r[15]=0x1e04u;return 0;
        case 0x1e04u: /* ldr r9,[r5,#0x40] */
            LOAD(r[9],r[5]+64u,4);r[15]=0x1e08u;return 0;
        case 0x1e08u: /* adds r1,r1,r9 */
            r[1]=addition(state,r[1],r[9]);r[15]=0x1e0cu;return 0;
        case 0x1e0cu: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x1e10u;return 0;
        case 0x1e10u: /* subs r4,r1,#0x8 */
            r[4]=subtract(state,r[1],8u);r[15]=0x1e14u;return 0;
        case 0x1e14u: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0x1e18u;return 0;
        case 0x1e18u: /* add r4,r1,#0x8 */
            r[4]=r[1]+8u;r[15]=0x1e1cu;return 0;
        case 0x1e1cu: /* tst r4,#0xe000 */
            temporary=r[4]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0x1e20u;return 0;
        case 0x1e20u: /* movne r1,#0xf8 */
            if(!(state->cpsr&0x40000000u)){r[1]=248u;}r[15]=0x1e24u;return 0;
        case 0x1e24u: /* orrne r1,r1,#0x1f00 */
            if(!(state->cpsr&0x40000000u)){r[1]=r[1]|7936u;}r[15]=0x1e28u;return 0;
        case 0x1e28u: /* str r1,[r8,#0x38] */
            STORE(r[8]+56u,r[1],4);r[15]=0x1e2cu;return 0;
        case 0x1e2cu: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x1e30u;return 0;
        case 0x1e30u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x1e34u;return 0;
        case 0x1e34u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x1e38u;return 0;
        case 0x1e38u: /* bne 0x00001db8 */
            if(!(state->cpsr&0x40000000u)){r[15]=7608u;}else{r[15]=0x1e3cu;}return 0;
        case 0x1e3cu: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1e40u;return 0;
        case 0x1e40u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1e44u;return 0;
        case 0x1e44u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1e48u: /* ldrb r6,[r5,#0x30] */
            LOAD(r[6],r[5]+48u,1);r[15]=0x1e4cu;return 0;
        case 0x1e4cu: /* and r6,r6,#0x7f */
            r[6]=r[6]&127u;r[15]=0x1e50u;return 0;
        case 0x1e50u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1e54u;return 0;
        case 0x1e54u: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x1e58u;return 0;
        case 0x1e58u: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1e5cu;return 0;
        case 0x1e5cu: /* mov r6,r1, lsr #0x8 */
            r[6]=(r[1]>>8);r[15]=0x1e60u;return 0;
        case 0x1e60u: /* strb r6,[r5,#0x31] */
            STORE(r[5]+49u,r[6],1);r[15]=0x1e64u;return 0;
        case 0x1e64u: /* and r6,r1,#0xff */
            r[6]=r[1]&255u;r[15]=0x1e68u;return 0;
        case 0x1e68u: /* strb r6,[r5,#0x30] */
            STORE(r[5]+48u,r[6],1);r[15]=0x1e6cu;return 0;
        case 0x1e6cu: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1e70u;return 0;
        case 0x1e70u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1e74u;return 0;
        case 0x1e74u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1e78u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x1e7cu;return 0;
        case 0x1e7cu: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x1e80u;return 0;
        case 0x1e80u: /* strb r1,[r5,#0x32] */
            STORE(r[5]+50u,r[1],1);r[15]=0x1e84u;return 0;
        case 0x1e84u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1e88u;return 0;
        case 0x1e88u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1e8cu;return 0;
        case 0x1e8cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1e90u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x1e94u;return 0;
        case 0x1e94u: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x1e98u;return 0;
        case 0x1e98u: /* strb r1,[r5,#0x33] */
            STORE(r[5]+51u,r[1],1);r[15]=0x1e9cu;return 0;
        case 0x1e9cu: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1ea0u;return 0;
        case 0x1ea0u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1ea4u;return 0;
        case 0x1ea4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1ea8u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x1eacu;return 0;
        case 0x1eacu: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x1eb0u;return 0;
        case 0x1eb0u: /* strb r1,[r5,#0x34] */
            STORE(r[5]+52u,r[1],1);r[15]=0x1eb4u;return 0;
        case 0x1eb4u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1eb8u;return 0;
        case 0x1eb8u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1ebcu;return 0;
        case 0x1ebcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1ec0u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x1ec4u;return 0;
        case 0x1ec4u: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x1ec8u;return 0;
        case 0x1ec8u: /* strb r1,[r5,#0x35] */
            STORE(r[5]+53u,r[1],1);r[15]=0x1eccu;return 0;
        case 0x1eccu: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1ed0u;return 0;
        case 0x1ed0u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1ed4u;return 0;
        case 0x1ed4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1ed8u: /* ldrb r6,[r5,#0x3c] */
            LOAD(r[6],r[5]+60u,1);r[15]=0x1edcu;return 0;
        case 0x1edcu: /* tst r6,#0x1 */
            temporary=r[6]&1u;logic_flags(state,temporary,0,1u);r[15]=0x1ee0u;return 0;
        case 0x1ee0u: /* bne 0x00001f24 */
            if(!(state->cpsr&0x40000000u)){r[15]=7972u;}else{r[15]=0x1ee4u;}return 0;
        case 0x1ee4u: /* strb r1,[r5,#0x1] */
            STORE(r[5]+1u,r[1],1);r[15]=0x1ee8u;return 0;
        case 0x1ee8u: /* ldr r6,[0x2888] */
            LOAD(r[6],10376u,4);r[15]=0x1eecu;return 0;
        case 0x1eecu: /* add r6,r6,r1, lsl #0x3 */
            r[6]=r[6]+(r[1]<<3);r[15]=0x1ef0u;return 0;
        case 0x1ef0u: /* ldr r7,[r6,#0x0] */
            LOAD(r[7],r[6]+0u,4);r[15]=0x1ef4u;return 0;
        case 0x1ef4u: /* movs r7,r7 */
            r[7]=r[7];logic_flags(state,r[7],0,0);r[15]=0x1ef8u;return 0;
        case 0x1ef8u: /* beq 0x00001f0c */
            if(state->cpsr&0x40000000u){r[15]=7948u;}else{r[15]=0x1efcu;}return 0;
        case 0x1efcu: /* ldr r8,[r6,#0x4] */
            LOAD(r[8],r[6]+4u,4);r[15]=0x1f00u;return 0;
        case 0x1f00u: /* movs r8,r8 */
            r[8]=r[8];logic_flags(state,r[8],0,0);r[15]=0x1f04u;return 0;
        case 0x1f04u: /* beq 0x00001f0c */
            if(state->cpsr&0x40000000u){r[15]=7948u;}else{r[15]=0x1f08u;}return 0;
        case 0x1f08u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1f0cu: /* ldr r0,[0x288c] */
            LOAD(r[0],10380u,4);r[15]=0x1f10u;return 0;
        case 0x1f10u: /* ldrb r1,[r0,#0x0] */
            LOAD(r[1],r[0]+0u,1);r[15]=0x1f14u;return 0;
        case 0x1f14u: /* mov r2,#0x1 */
            r[2]=1u;r[15]=0x1f18u;return 0;
        case 0x1f18u: /* orr r1,r1,r2 */
            r[1]=r[1]|r[2];r[15]=0x1f1cu;return 0;
        case 0x1f1cu: /* strb r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],1);r[15]=0x1f20u;return 0;
        case 0x1f20u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1f24u: /* cmp r1,#0x2 */
            (void)subtract(state,r[1],2u);r[15]=0x1f28u;return 0;
        case 0x1f28u: /* movcs pc,lr */
            if(state->cpsr&0x20000000u){r[15]=r[14];}else{r[15]=0x1f2cu;}return 0;
        case 0x1f2cu: /* ldr r6,[0x2890] */
            LOAD(r[6],10384u,4);r[15]=0x1f30u;return 0;
        case 0x1f30u: /* add r6,r6,r1, lsl #0x3 */
            r[6]=r[6]+(r[1]<<3);r[15]=0x1f34u;return 0;
        case 0x1f34u: /* ldr r7,[r6,#0x0] */
            LOAD(r[7],r[6]+0u,4);r[15]=0x1f38u;return 0;
        case 0x1f38u: /* movs r7,r7 */
            r[7]=r[7];logic_flags(state,r[7],0,0);r[15]=0x1f3cu;return 0;
        case 0x1f3cu: /* beq 0x00001f58 */
            if(state->cpsr&0x40000000u){r[15]=8024u;}else{r[15]=0x1f40u;}return 0;
        case 0x1f40u: /* ldr r8,[r6,#0x4] */
            LOAD(r[8],r[6]+4u,4);r[15]=0x1f44u;return 0;
        case 0x1f44u: /* movs r8,r8 */
            r[8]=r[8];logic_flags(state,r[8],0,0);r[15]=0x1f48u;return 0;
        case 0x1f48u: /* beq 0x00001f58 */
            if(state->cpsr&0x40000000u){r[15]=8024u;}else{r[15]=0x1f4cu;}return 0;
        case 0x1f4cu: /* strb r1,[r5,#0x1] */
            STORE(r[5]+1u,r[1],1);r[15]=0x1f50u;return 0;
        case 0x1f50u: /* str r7,[r5,#0x10] */
            STORE(r[5]+16u,r[7],4);r[15]=0x1f54u;return 0;
        case 0x1f54u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1f58u: /* ldr r0,[0x2894] */
            LOAD(r[0],10388u,4);r[15]=0x1f5cu;return 0;
        case 0x1f5cu: /* str r0,[r5,#0x10] */
            STORE(r[5]+16u,r[0],4);r[15]=0x1f60u;return 0;
        case 0x1f60u: /* b 0x00001f0c */
            r[15]=7948u;return 0;
        case 0x1f64u: /* ldr r6,[0x2898] */
            LOAD(r[6],10392u,4);r[15]=0x1f68u;return 0;
        case 0x1f68u: /* tst r1,#0xff */
            temporary=r[1]&255u;logic_flags(state,temporary,0,255u);r[15]=0x1f6cu;return 0;
        case 0x1f6cu: /* beq 0x00001f78 */
            if(state->cpsr&0x40000000u){r[15]=8056u;}else{r[15]=0x1f70u;}return 0;
        case 0x1f70u: /* ldrb r1,[r6,#0x0] */
            LOAD(r[1],r[6]+0u,1);r[15]=0x1f74u;return 0;
        case 0x1f74u: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x1f78u;return 0;
        case 0x1f78u: /* strb r1,[r6,#0x0] */
            STORE(r[6]+0u,r[1],1);r[15]=0x1f7cu;return 0;
        case 0x1f7cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1f80u: /* ldrb r6,[r5,#0x28] */
            LOAD(r[6],r[5]+40u,1);r[15]=0x1f84u;return 0;
        case 0x1f84u: /* and r6,r6,#0x80 */
            r[6]=r[6]&128u;r[15]=0x1f88u;return 0;
        case 0x1f88u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1f8cu;return 0;
        case 0x1f8cu: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1f90u;return 0;
        case 0x1f90u: /* strb r1,[r5,#0x28] */
            STORE(r[5]+40u,r[1],1);r[15]=0x1f94u;return 0;
        case 0x1f94u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1f98u;return 0;
        case 0x1f98u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1f9cu;return 0;
        case 0x1f9cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1fa0u: /* ldrb r6,[r5,#0x2a] */
            LOAD(r[6],r[5]+42u,1);r[15]=0x1fa4u;return 0;
        case 0x1fa4u: /* and r6,r6,#0x80 */
            r[6]=r[6]&128u;r[15]=0x1fa8u;return 0;
        case 0x1fa8u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1facu;return 0;
        case 0x1facu: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1fb0u;return 0;
        case 0x1fb0u: /* strb r1,[r5,#0x2a] */
            STORE(r[5]+42u,r[1],1);r[15]=0x1fb4u;return 0;
        case 0x1fb4u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1fb8u;return 0;
        case 0x1fb8u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1fbcu;return 0;
        case 0x1fbcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1fc0u: /* ldrb r6,[r5,#0x2c] */
            LOAD(r[6],r[5]+44u,1);r[15]=0x1fc4u;return 0;
        case 0x1fc4u: /* and r6,r6,#0x80 */
            r[6]=r[6]&128u;r[15]=0x1fc8u;return 0;
        case 0x1fc8u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1fccu;return 0;
        case 0x1fccu: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1fd0u;return 0;
        case 0x1fd0u: /* strb r1,[r5,#0x2c] */
            STORE(r[5]+44u,r[1],1);r[15]=0x1fd4u;return 0;
        case 0x1fd4u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x1fd8u;return 0;
        case 0x1fd8u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x1fdcu;return 0;
        case 0x1fdcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x1fe0u: /* ldrb r6,[r5,#0x2e] */
            LOAD(r[6],r[5]+46u,1);r[15]=0x1fe4u;return 0;
        case 0x1fe4u: /* and r6,r6,#0x80 */
            r[6]=r[6]&128u;r[15]=0x1fe8u;return 0;
        case 0x1fe8u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x1fecu;return 0;
        case 0x1fecu: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x1ff0u;return 0;
        case 0x1ff0u: /* strb r1,[r5,#0x2e] */
            STORE(r[5]+46u,r[1],1);r[15]=0x1ff4u;return 0;
        case 0x1ff4u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x1ff8u;return 0;
        case 0x1ff8u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x1ffcu;return 0;
        case 0x1ffcu: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x2000u;return 0;
        case 0x2000u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x2004u;return 0;
        case 0x2004u: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x2008u;return 0;
        case 0x2008u: /* beq 0x00002074 */
            if(state->cpsr&0x40000000u){r[15]=8308u;}else{r[15]=0x200cu;}return 0;
        case 0x200cu: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x2010u;return 0;
        case 0x2010u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x2014u;return 0;
        case 0x2014u: /* bne 0x00002074 */
            if(!(state->cpsr&0x40000000u)){r[15]=8308u;}else{r[15]=0x2018u;}return 0;
        case 0x2018u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x201cu;return 0;
        case 0x201cu: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x2020u;return 0;
        case 0x2020u: /* bne 0x00002074 */
            if(!(state->cpsr&0x40000000u)){r[15]=8308u;}else{r[15]=0x2024u;}return 0;
        case 0x2024u: /* ldr r9,[r6,#0x10] */
            LOAD(r[9],r[6]+16u,4);r[15]=0x2028u;return 0;
        case 0x2028u: /* ldr r0,[r9,#0x1c] */
            LOAD(r[0],r[9]+28u,4);r[15]=0x202cu;return 0;
        case 0x202cu: /* ldr r9,[r5,#0x2c] */
            LOAD(r[9],r[5]+44u,4);r[15]=0x2030u;return 0;
        case 0x2030u: /* mov r9,r9, ror #0x10 */
            r[9]=((r[9]>>16)|(r[9]<<16));r[15]=0x2034u;return 0;
        case 0x2034u: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x2038u;return 0;
        case 0x2038u: /* orr r1,r1,#0xff00 */
            r[1]=r[1]|65280u;r[15]=0x203cu;return 0;
        case 0x203cu: /* and r9,r9,r1 */
            r[9]=r[9]&r[1];r[15]=0x2040u;return 0;
        case 0x2040u: /* sub r9,r9,#0x2000 */
            r[9]=r[9]-8192u;r[15]=0x2044u;return 0;
        case 0x2044u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0x2048u;return 0;
        case 0x2048u: /* add r1,r1,r9 */
            r[1]=r[1]+r[9];r[15]=0x204cu;return 0;
        case 0x204cu: /* ldr r9,[r5,#0x40] */
            LOAD(r[9],r[5]+64u,4);r[15]=0x2050u;return 0;
        case 0x2050u: /* adds r1,r1,r9 */
            r[1]=addition(state,r[1],r[9]);r[15]=0x2054u;return 0;
        case 0x2054u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x2058u;return 0;
        case 0x2058u: /* subs r4,r1,#0x8 */
            r[4]=subtract(state,r[1],8u);r[15]=0x205cu;return 0;
        case 0x205cu: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0x2060u;return 0;
        case 0x2060u: /* add r4,r1,#0x8 */
            r[4]=r[1]+8u;r[15]=0x2064u;return 0;
        case 0x2064u: /* tst r4,#0xe000 */
            temporary=r[4]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0x2068u;return 0;
        case 0x2068u: /* movne r1,#0xf8 */
            if(!(state->cpsr&0x40000000u)){r[1]=248u;}r[15]=0x206cu;return 0;
        case 0x206cu: /* orrne r1,r1,#0x1f00 */
            if(!(state->cpsr&0x40000000u)){r[1]=r[1]|7936u;}r[15]=0x2070u;return 0;
        case 0x2070u: /* str r1,[r8,#0x38] */
            STORE(r[8]+56u,r[1],4);r[15]=0x2074u;return 0;
        case 0x2074u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x2078u;return 0;
        case 0x2078u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x207cu;return 0;
        case 0x207cu: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x2080u;return 0;
        case 0x2080u: /* bne 0x00002000 */
            if(!(state->cpsr&0x40000000u)){r[15]=8192u;}else{r[15]=0x2084u;}return 0;
        case 0x2084u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x2088u;return 0;
        case 0x2088u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x208cu;return 0;
        case 0x208cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2090u: /* ldrb r6,[r5,#0x30] */
            LOAD(r[6],r[5]+48u,1);r[15]=0x2094u;return 0;
        case 0x2094u: /* and r6,r6,#0x80 */
            r[6]=r[6]&128u;r[15]=0x2098u;return 0;
        case 0x2098u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x209cu;return 0;
        case 0x209cu: /* orr r1,r1,r6 */
            r[1]=r[1]|r[6];r[15]=0x20a0u;return 0;
        case 0x20a0u: /* strb r1,[r5,#0x30] */
            STORE(r[5]+48u,r[1],1);r[15]=0x20a4u;return 0;
        case 0x20a4u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x20a8u;return 0;
        case 0x20a8u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x20acu;return 0;
        case 0x20acu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x20b0u: /* ands r1,r1,#0x40 */
            temporary=r[1]&64u;r[1]=temporary;logic_flags(state,temporary,0,64u);r[15]=0x20b4u;return 0;
        case 0x20b4u: /* strb r1,[r5,#0x26] */
            STORE(r[5]+38u,r[1],1);r[15]=0x20b8u;return 0;
        case 0x20b8u: /* bne 0x00002138 */
            if(!(state->cpsr&0x40000000u)){r[15]=8504u;}else{r[15]=0x20bcu;}return 0;
        case 0x20bcu: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x20c0u;return 0;
        case 0x20c0u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x20c4u;return 0;
        case 0x20c4u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x20c8u;return 0;
        case 0x20c8u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x20ccu;return 0;
        case 0x20ccu: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x20d0u;return 0;
        case 0x20d0u: /* beq 0x0000211c */
            if(state->cpsr&0x40000000u){r[15]=8476u;}else{r[15]=0x20d4u;}return 0;
        case 0x20d4u: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x20d8u;return 0;
        case 0x20d8u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x20dcu;return 0;
        case 0x20dcu: /* bne 0x0000211c */
            if(!(state->cpsr&0x40000000u)){r[15]=8476u;}else{r[15]=0x20e0u;}return 0;
        case 0x20e0u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x20e4u;return 0;
        case 0x20e4u: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x20e8u;return 0;
        case 0x20e8u: /* bne 0x0000211c */
            if(!(state->cpsr&0x40000000u)){r[15]=8476u;}else{r[15]=0x20ecu;}return 0;
        case 0x20ecu: /* ldrb r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,1);r[15]=0x20f0u;return 0;
        case 0x20f0u: /* bic r9,r9,#0x40 */
            r[9]=r[9]&~64u;r[15]=0x20f4u;return 0;
        case 0x20f4u: /* strb r9,[r6,#0x0] */
            STORE(r[6]+0u,r[9],1);r[15]=0x20f8u;return 0;
        case 0x20f8u: /* tst r9,#0x80 */
            temporary=r[9]&128u;logic_flags(state,temporary,0,128u);r[15]=0x20fcu;return 0;
        case 0x20fcu: /* bne 0x0000211c */
            if(!(state->cpsr&0x40000000u)){r[15]=8476u;}else{r[15]=0x2100u;}return 0;
        case 0x2100u: /* mov r9,#0x0 */
            r[9]=0u;r[15]=0x2104u;return 0;
        case 0x2104u: /* strb r9,[r6,#0x3f] */
            STORE(r[6]+63u,r[9],1);r[15]=0x2108u;return 0;
        case 0x2108u: /* ldr r9,[r8,#0x0] */
            LOAD(r[9],r[8]+0u,4);r[15]=0x210cu;return 0;
        case 0x210cu: /* ldr r4,[0x2134] */
            LOAD(r[4],8500u,4);r[15]=0x2110u;return 0;
        case 0x2110u: /* and r9,r9,r4 */
            r[9]=r[9]&r[4];r[15]=0x2114u;return 0;
        case 0x2114u: /* orr r9,r9,#0x8000 */
            r[9]=r[9]|32768u;r[15]=0x2118u;return 0;
        case 0x2118u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x211cu;return 0;
        case 0x211cu: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x2120u;return 0;
        case 0x2120u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x2124u;return 0;
        case 0x2124u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x2128u;return 0;
        case 0x2128u: /* bne 0x000020c8 */
            if(!(state->cpsr&0x40000000u)){r[15]=8392u;}else{r[15]=0x212cu;}return 0;
        case 0x212cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2138u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x213cu;return 0;
        case 0x213cu: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x2140u;return 0;
        case 0x2140u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x2144u;return 0;
        case 0x2144u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x2148u;return 0;
        case 0x2148u: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x214cu;return 0;
        case 0x214cu: /* beq 0x00002174 */
            if(state->cpsr&0x40000000u){r[15]=8564u;}else{r[15]=0x2150u;}return 0;
        case 0x2150u: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x2154u;return 0;
        case 0x2154u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x2158u;return 0;
        case 0x2158u: /* bne 0x00002174 */
            if(!(state->cpsr&0x40000000u)){r[15]=8564u;}else{r[15]=0x215cu;}return 0;
        case 0x215cu: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x2160u;return 0;
        case 0x2160u: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x2164u;return 0;
        case 0x2164u: /* bne 0x00002174 */
            if(!(state->cpsr&0x40000000u)){r[15]=8564u;}else{r[15]=0x2168u;}return 0;
        case 0x2168u: /* ldrb r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,1);r[15]=0x216cu;return 0;
        case 0x216cu: /* orr r9,r9,#0x40 */
            r[9]=r[9]|64u;r[15]=0x2170u;return 0;
        case 0x2170u: /* strb r9,[r6,#0x0] */
            STORE(r[6]+0u,r[9],1);r[15]=0x2174u;return 0;
        case 0x2174u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x2178u;return 0;
        case 0x2178u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x217cu;return 0;
        case 0x217cu: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x2180u;return 0;
        case 0x2180u: /* bne 0x00002144 */
            if(!(state->cpsr&0x40000000u)){r[15]=8516u;}else{r[15]=0x2184u;}return 0;
        case 0x2184u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x218cu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2190u;return 0;
        case 0x2190u: /* strb r1,[r5,#0x3d] */
            STORE(r[5]+61u,r[1],1);r[15]=0x2194u;return 0;
        case 0x2194u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2198u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x219cu;return 0;
        case 0x219cu: /* strb r1,[r5,#0x27] */
            STORE(r[5]+39u,r[1],1);r[15]=0x21a0u;return 0;
        case 0x21a0u: /* sub r1,r1,#0x40 */
            r[1]=r[1]-64u;r[15]=0x21a4u;return 0;
        case 0x21a4u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x21a8u;return 0;
        case 0x21a8u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x21acu;return 0;
        case 0x21acu: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x21b0u;return 0;
        case 0x21b0u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x21b4u;return 0;
        case 0x21b4u: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x21b8u;return 0;
        case 0x21b8u: /* beq 0x00002200 */
            if(state->cpsr&0x40000000u){r[15]=8704u;}else{r[15]=0x21bcu;}return 0;
        case 0x21bcu: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x21c0u;return 0;
        case 0x21c0u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x21c4u;return 0;
        case 0x21c4u: /* bne 0x00002200 */
            if(!(state->cpsr&0x40000000u)){r[15]=8704u;}else{r[15]=0x21c8u;}return 0;
        case 0x21c8u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x21ccu;return 0;
        case 0x21ccu: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x21d0u;return 0;
        case 0x21d0u: /* bne 0x00002200 */
            if(!(state->cpsr&0x40000000u)){r[15]=8704u;}else{r[15]=0x21d4u;}return 0;
        case 0x21d4u: /* ldr r9,[r6,#0x10] */
            LOAD(r[9],r[6]+16u,4);r[15]=0x21d8u;return 0;
        case 0x21d8u: /* ldr r4,[r9,#0x14] */
            LOAD(r[4],r[9]+20u,4);r[15]=0x21dcu;return 0;
        case 0x21dcu: /* ldrb r0,[r8,#0x28] */
            LOAD(r[0],r[8]+40u,1);r[15]=0x21e0u;return 0;
        case 0x21e0u: /* and r0,r0,#0x20 */
            r[0]=r[0]&32u;r[15]=0x21e4u;return 0;
        case 0x21e4u: /* and r4,r4,#0x1f */
            r[4]=r[4]&31u;r[15]=0x21e8u;return 0;
        case 0x21e8u: /* adds r1,r1,r4 */
            r[1]=addition(state,r[1],r[4]);r[15]=0x21ecu;return 0;
        case 0x21ecu: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x21f0u;return 0;
        case 0x21f0u: /* tst r1,#0xe0 */
            temporary=r[1]&224u;logic_flags(state,temporary,0,224u);r[15]=0x21f4u;return 0;
        case 0x21f4u: /* movne r1,#0x1f */
            if(!(state->cpsr&0x40000000u)){r[1]=31u;}r[15]=0x21f8u;return 0;
        case 0x21f8u: /* orr r1,r1,r0 */
            r[1]=r[1]|r[0];r[15]=0x21fcu;return 0;
        case 0x21fcu: /* strb r1,[r8,#0x28] */
            STORE(r[8]+40u,r[1],1);r[15]=0x2200u;return 0;
        case 0x2200u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x2204u;return 0;
        case 0x2204u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x2208u;return 0;
        case 0x2208u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x220cu;return 0;
        case 0x220cu: /* bne 0x000021b0 */
            if(!(state->cpsr&0x40000000u)){r[15]=8624u;}else{r[15]=0x2210u;}return 0;
        case 0x2210u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2218u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x221cu;return 0;
        case 0x221cu: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x2220u;return 0;
        case 0x2220u: /* strb r1,[r5,#0x3b] */
            STORE(r[5]+59u,r[1],1);r[15]=0x2224u;return 0;
        case 0x2224u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2228u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x222cu;return 0;
        case 0x222cu: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x2230u;return 0;
        case 0x2230u: /* strb r1,[r5,#0x37] */
            STORE(r[5]+55u,r[1],1);r[15]=0x2234u;return 0;
        case 0x2234u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2238u: /* ldrb r6,[r5,#0x3e] */
            LOAD(r[6],r[5]+62u,1);r[15]=0x223cu;return 0;
        case 0x223cu: /* adr r7,0x2248 */
            r[7]=8776u;r[15]=0x2240u;return 0;
        case 0x2240u: /* add r6,r6,r7 */
            r[6]=r[6]+r[7];r[15]=0x2244u;return 0;
        case 0x2244u: /* mov pc,r6 */
            r[15]=r[6];return 0;
        case 0x2248u: /* b 0x00002344 */
            r[15]=9028u;return 0;
        case 0x224cu: /* b 0x000022a0 */
            r[15]=8864u;return 0;
        case 0x2250u: /* b 0x000022b4 */
            r[15]=8884u;return 0;
        case 0x2254u: /* b 0x000022cc */
            r[15]=8908u;return 0;
        case 0x2258u: /* b 0x000022e4 */
            r[15]=8932u;return 0;
        case 0x225cu: /* b 0x000022fc */
            r[15]=8956u;return 0;
        case 0x2260u: /* b 0x00002314 */
            r[15]=8980u;return 0;
        case 0x2264u: /* b 0x0000232c */
            r[15]=9004u;return 0;
        case 0x2268u: /* b 0x00002344 */
            r[15]=9028u;return 0;
        case 0x2288u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x228cu;return 0;
        case 0x228cu: /* cmp r1,#0x8 */
            (void)subtract(state,r[1],8u);r[15]=0x2290u;return 0;
        case 0x2290u: /* movhi r1,#0x8 */
            if((state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)){r[1]=8u;}r[15]=0x2294u;return 0;
        case 0x2294u: /* mov r1,r1, lsl #0x2 */
            r[1]=(r[1]<<2);r[15]=0x2298u;return 0;
        case 0x2298u: /* strb r1,[r5,#0x3e] */
            STORE(r[5]+62u,r[1],1);r[15]=0x229cu;return 0;
        case 0x229cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x22a0u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x22a4u;return 0;
        case 0x22a4u: /* sub r1,r1,#0x40 */
            r[1]=r[1]-64u;r[15]=0x22a8u;return 0;
        case 0x22a8u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x22acu;return 0;
        case 0x22acu: /* b 0x00002354 */
            r[15]=9044u;return 0;
        case 0x22b4u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x22b8u;return 0;
        case 0x22b8u: /* mov r1,r1, lsl #0x1 */
            r[1]=(r[1]<<1);r[15]=0x22bcu;return 0;
        case 0x22bcu: /* sub r1,r1,#0x80 */
            r[1]=r[1]-128u;r[15]=0x22c0u;return 0;
        case 0x22c0u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x22c4u;return 0;
        case 0x22c4u: /* b 0x00002354 */
            r[15]=9044u;return 0;
        case 0x22ccu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x22d0u;return 0;
        case 0x22d0u: /* mov r1,r1, lsl #0x2 */
            r[1]=(r[1]<<2);r[15]=0x22d4u;return 0;
        case 0x22d4u: /* sub r1,r1,#0x100 */
            r[1]=r[1]-256u;r[15]=0x22d8u;return 0;
        case 0x22d8u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x22dcu;return 0;
        case 0x22dcu: /* b 0x00002354 */
            r[15]=9044u;return 0;
        case 0x22e4u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x22e8u;return 0;
        case 0x22e8u: /* mov r1,r1, lsl #0x3 */
            r[1]=(r[1]<<3);r[15]=0x22ecu;return 0;
        case 0x22ecu: /* sub r1,r1,#0x200 */
            r[1]=r[1]-512u;r[15]=0x22f0u;return 0;
        case 0x22f0u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x22f4u;return 0;
        case 0x22f4u: /* b 0x00002354 */
            r[15]=9044u;return 0;
        case 0x22fcu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2300u;return 0;
        case 0x2300u: /* mov r1,r1, lsl #0x4 */
            r[1]=(r[1]<<4);r[15]=0x2304u;return 0;
        case 0x2304u: /* sub r1,r1,#0x400 */
            r[1]=r[1]-1024u;r[15]=0x2308u;return 0;
        case 0x2308u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x230cu;return 0;
        case 0x230cu: /* b 0x00002354 */
            r[15]=9044u;return 0;
        case 0x2314u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2318u;return 0;
        case 0x2318u: /* mov r1,r1, lsl #0x5 */
            r[1]=(r[1]<<5);r[15]=0x231cu;return 0;
        case 0x231cu: /* sub r1,r1,#0x800 */
            r[1]=r[1]-2048u;r[15]=0x2320u;return 0;
        case 0x2320u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x2324u;return 0;
        case 0x2324u: /* b 0x00002354 */
            r[15]=9044u;return 0;
        case 0x232cu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2330u;return 0;
        case 0x2330u: /* mov r1,r1, lsl #0x6 */
            r[1]=(r[1]<<6);r[15]=0x2334u;return 0;
        case 0x2334u: /* sub r1,r1,#0x1000 */
            r[1]=r[1]-4096u;r[15]=0x2338u;return 0;
        case 0x2338u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x233cu;return 0;
        case 0x233cu: /* b 0x00002354 */
            r[15]=9044u;return 0;
        case 0x2344u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2348u;return 0;
        case 0x2348u: /* mov r1,r1, lsl #0x7 */
            r[1]=(r[1]<<7);r[15]=0x234cu;return 0;
        case 0x234cu: /* sub r1,r1,#0x2000 */
            r[1]=r[1]-8192u;r[15]=0x2350u;return 0;
        case 0x2350u: /* str r1,[r5,#0x40] */
            STORE(r[5]+64u,r[1],4);r[15]=0x2354u;return 0;
        case 0x2354u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x2358u;return 0;
        case 0x2358u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x235cu;return 0;
        case 0x235cu: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x2360u;return 0;
        case 0x2360u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x2364u;return 0;
        case 0x2364u: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x2368u;return 0;
        case 0x2368u: /* beq 0x000023d4 */
            if(state->cpsr&0x40000000u){r[15]=9172u;}else{r[15]=0x236cu;}return 0;
        case 0x236cu: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x2370u;return 0;
        case 0x2370u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x2374u;return 0;
        case 0x2374u: /* bne 0x000023d4 */
            if(!(state->cpsr&0x40000000u)){r[15]=9172u;}else{r[15]=0x2378u;}return 0;
        case 0x2378u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x237cu;return 0;
        case 0x237cu: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x2380u;return 0;
        case 0x2380u: /* bne 0x000023d4 */
            if(!(state->cpsr&0x40000000u)){r[15]=9172u;}else{r[15]=0x2384u;}return 0;
        case 0x2384u: /* ldr r9,[r6,#0x10] */
            LOAD(r[9],r[6]+16u,4);r[15]=0x2388u;return 0;
        case 0x2388u: /* ldr r0,[r9,#0x1c] */
            LOAD(r[0],r[9]+28u,4);r[15]=0x238cu;return 0;
        case 0x238cu: /* ldr r9,[r5,#0x2c] */
            LOAD(r[9],r[5]+44u,4);r[15]=0x2390u;return 0;
        case 0x2390u: /* mov r9,r9, ror #0x10 */
            r[9]=((r[9]>>16)|(r[9]<<16));r[15]=0x2394u;return 0;
        case 0x2394u: /* mov r1,#0xff */
            r[1]=255u;r[15]=0x2398u;return 0;
        case 0x2398u: /* orr r1,r1,#0xff00 */
            r[1]=r[1]|65280u;r[15]=0x239cu;return 0;
        case 0x239cu: /* and r9,r9,r1 */
            r[9]=r[9]&r[1];r[15]=0x23a0u;return 0;
        case 0x23a0u: /* sub r9,r9,#0x2000 */
            r[9]=r[9]-8192u;r[15]=0x23a4u;return 0;
        case 0x23a4u: /* and r1,r0,r1 */
            r[1]=r[0]&r[1];r[15]=0x23a8u;return 0;
        case 0x23a8u: /* add r1,r1,r9 */
            r[1]=r[1]+r[9];r[15]=0x23acu;return 0;
        case 0x23acu: /* ldr r9,[r5,#0x40] */
            LOAD(r[9],r[5]+64u,4);r[15]=0x23b0u;return 0;
        case 0x23b0u: /* adds r1,r1,r9 */
            r[1]=addition(state,r[1],r[9]);r[15]=0x23b4u;return 0;
        case 0x23b4u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x23b8u;return 0;
        case 0x23b8u: /* subs r4,r1,#0x8 */
            r[4]=subtract(state,r[1],8u);r[15]=0x23bcu;return 0;
        case 0x23bcu: /* movmi r1,#0x8 */
            if(state->cpsr&0x80000000u){r[1]=8u;}r[15]=0x23c0u;return 0;
        case 0x23c0u: /* add r4,r1,#0x8 */
            r[4]=r[1]+8u;r[15]=0x23c4u;return 0;
        case 0x23c4u: /* tst r4,#0xe000 */
            temporary=r[4]&57344u;logic_flags(state,temporary,1,57344u);r[15]=0x23c8u;return 0;
        case 0x23c8u: /* movne r1,#0xf8 */
            if(!(state->cpsr&0x40000000u)){r[1]=248u;}r[15]=0x23ccu;return 0;
        case 0x23ccu: /* orrne r1,r1,#0x1f00 */
            if(!(state->cpsr&0x40000000u)){r[1]=r[1]|7936u;}r[15]=0x23d0u;return 0;
        case 0x23d0u: /* str r1,[r8,#0x38] */
            STORE(r[8]+56u,r[1],4);r[15]=0x23d4u;return 0;
        case 0x23d4u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x23d8u;return 0;
        case 0x23d8u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x23dcu;return 0;
        case 0x23dcu: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x23e0u;return 0;
        case 0x23e0u: /* bne 0x00002360 */
            if(!(state->cpsr&0x40000000u)){r[15]=9056u;}else{r[15]=0x23e4u;}return 0;
        case 0x23e4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x23e8u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x23ecu;return 0;
        case 0x23ecu: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x23f0u;return 0;
        case 0x23f0u: /* strb r1,[r5,#0x39] */
            STORE(r[5]+57u,r[1],1);r[15]=0x23f4u;return 0;
        case 0x23f4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x23f8u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x23fcu;return 0;
        case 0x23fcu: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x2400u;return 0;
        case 0x2400u: /* strb r1,[r5,#0x38] */
            STORE(r[5]+56u,r[1],1);r[15]=0x2404u;return 0;
        case 0x2404u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2408u: /* mov r1,r1, lsr #0x1 */
            r[1]=(r[1]>>1);r[15]=0x240cu;return 0;
        case 0x240cu: /* and r1,r1,#0x3f */
            r[1]=r[1]&63u;r[15]=0x2410u;return 0;
        case 0x2410u: /* strb r1,[r5,#0x3a] */
            STORE(r[5]+58u,r[1],1);r[15]=0x2414u;return 0;
        case 0x2414u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2418u: /* mov r0,#0x7 */
            r[0]=7u;r[15]=0x241cu;return 0;
        case 0x241cu: /* cmp r3,r0 */
            (void)subtract(state,r[3],r[0]);r[15]=0x2420u;return 0;
        case 0x2420u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x2424u;}return 0;
        case 0x2424u: /* mov r1,r1, lsr #0x2 */
            r[1]=(r[1]>>2);r[15]=0x2428u;return 0;
        case 0x2428u: /* subs r1,r1,#0x10 */
            r[1]=subtract(state,r[1],16u);r[15]=0x242cu;return 0;
        case 0x242cu: /* eormi r1,r1,#0xff */
            if(state->cpsr&0x80000000u){r[1]=r[1]^255u;}r[15]=0x2430u;return 0;
        case 0x2430u: /* addmi r1,r1,#0x10 */
            if(state->cpsr&0x80000000u){r[1]=r[1]+16u;}r[15]=0x2434u;return 0;
        case 0x2434u: /* and r1,r1,#0x1f */
            r[1]=r[1]&31u;r[15]=0x2438u;return 0;
        case 0x2438u: /* and r2,r2,#0xf */
            r[2]=r[2]&15u;r[15]=0x243cu;return 0;
        case 0x243cu: /* mov r2,r2, lsl #0x2 */
            r[2]=(r[2]<<2);r[15]=0x2440u;return 0;
        case 0x2440u: /* ldr r3,[0x289c] */
            LOAD(r[3],10396u,4);r[15]=0x2444u;return 0;
        case 0x2444u: /* strb r1,[r3,r2] */
            STORE(r[3]+r[2],r[1],1);r[15]=0x2448u;return 0;
        case 0x2448u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2450u: /* mov r0,#0x7 */
            r[0]=7u;r[15]=0x2454u;return 0;
        case 0x2454u: /* cmp r3,r0 */
            (void)subtract(state,r[3],r[0]);r[15]=0x2458u;return 0;
        case 0x2458u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x245cu;}return 0;
        case 0x245cu: /* mov r1,r1, lsr #0x3 */
            r[1]=(r[1]>>3);r[15]=0x2460u;return 0;
        case 0x2460u: /* and r1,r1,#0xf */
            r[1]=r[1]&15u;r[15]=0x2464u;return 0;
        case 0x2464u: /* and r2,r2,#0xf */
            r[2]=r[2]&15u;r[15]=0x2468u;return 0;
        case 0x2468u: /* mov r2,r2, lsl #0x2 */
            r[2]=(r[2]<<2);r[15]=0x246cu;return 0;
        case 0x246cu: /* ldr r3,[0x28a0] */
            LOAD(r[3],10400u,4);r[15]=0x2470u;return 0;
        case 0x2470u: /* strb r1,[r3,r2] */
            STORE(r[3]+r[2],r[1],1);r[15]=0x2474u;return 0;
        case 0x2474u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x247cu: /* mov r0,#0x7 */
            r[0]=7u;r[15]=0x2480u;return 0;
        case 0x2480u: /* cmp r3,r0 */
            (void)subtract(state,r[3],r[0]);r[15]=0x2484u;return 0;
        case 0x2484u: /* movne pc,lr */
            if(!(state->cpsr&0x40000000u)){r[15]=r[14];}else{r[15]=0x2488u;}return 0;
        case 0x2488u: /* ands r0,r1,#0x7f */
            temporary=r[1]&127u;r[0]=temporary;logic_flags(state,temporary,0,127u);r[15]=0x248cu;return 0;
        case 0x248cu: /* beq 0x000024a4 */
            if(state->cpsr&0x40000000u){r[15]=9380u;}else{r[15]=0x2490u;}return 0;
        case 0x2490u: /* sub r0,r0,#0x1 */
            r[0]=r[0]-1u;r[15]=0x2494u;return 0;
        case 0x2494u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x2498u;return 0;
        case 0x2498u: /* bl 0x00005c28 */
            r[14]=0x249cu;r[15]=23592u;return 0;
        case 0x249cu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x24a0u;return 0;
        case 0x24a0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x24a4u: /* stmdb sp!,{lr} */
            STORE(r[13]-4u,r[14],4);r[13]-=4u;r[15]=0x24a8u;return 0;
        case 0x24a8u: /* bl 0x00005d5c */
            r[14]=0x24acu;r[15]=23900u;return 0;
        case 0x24acu: /* ldmia sp!,{lr} */
            LOAD(r[14],r[13]+0u,4);r[13]+=4u;r[15]=0x24b0u;return 0;
        case 0x24b0u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x24b8u: /* ldr r0,[0x28a4] */
            LOAD(r[0],10404u,4);r[15]=0x24bcu;return 0;
        case 0x24bcu: /* ldrb r3,[r0,#0x0] */
            LOAD(r[3],r[0]+0u,1);r[15]=0x24c0u;return 0;
        case 0x24c0u: /* tst r3,#0x30 */
            temporary=r[3]&48u;logic_flags(state,temporary,0,48u);r[15]=0x24c4u;return 0;
        case 0x24c4u: /* moveq pc,lr */
            if(state->cpsr&0x40000000u){r[15]=r[14];}else{r[15]=0x24c8u;}return 0;
        case 0x24c8u: /* tst r3,#0x10 */
            temporary=r[3]&16u;logic_flags(state,temporary,0,16u);r[15]=0x24ccu;return 0;
        case 0x24ccu: /* andeq r2,r2,#0x3 */
            if(state->cpsr&0x40000000u){r[2]=r[2]&3u;}r[15]=0x24d0u;return 0;
        case 0x24d0u: /* moveq r0,#0x10 */
            if(state->cpsr&0x40000000u){r[0]=16u;}r[15]=0x24d4u;return 0;
        case 0x24d4u: /* andne r2,r2,#0x7 */
            if(!(state->cpsr&0x40000000u)){r[2]=r[2]&7u;}r[15]=0x24d8u;return 0;
        case 0x24d8u: /* movne r0,#0x20 */
            if(!(state->cpsr&0x40000000u)){r[0]=32u;}r[15]=0x24dcu;return 0;
        case 0x24dcu: /* ldr r3,[0x2524] */
            LOAD(r[3],9508u,4);r[15]=0x24e0u;return 0;
        case 0x24e0u: /* and r1,r1,#0x7c */
            r[1]=r[1]&124u;r[15]=0x24e4u;return 0;
        case 0x24e4u: /* add r1,r3,r1, lsl #0x2 */
            r[1]=r[3]+(r[1]<<2);r[15]=0x24e8u;return 0;
        case 0x24e8u: /* ldr r3,[0x28a8] */
            LOAD(r[3],10408u,4);r[15]=0x24ecu;return 0;
        case 0x24ecu: /* ldrb r4,[r3,#0x0] */
            LOAD(r[4],r[3]+0u,1);r[15]=0x24f0u;return 0;
        case 0x24f0u: /* ldr r3,[0x28ac] */
            LOAD(r[3],10412u,4);r[15]=0x24f4u;return 0;
        case 0x24f4u: /* add r3,r3,r4, lsl #0x2 */
            r[3]=r[3]+(r[4]<<2);r[15]=0x24f8u;return 0;
        case 0x24f8u: /* add r2,r3,r2, lsl #0x2 */
            r[2]=r[3]+(r[2]<<2);r[15]=0x24fcu;return 0;
        case 0x24fcu: /* ldr r3,[r1],#0x4 */
            LOAD(r[3],r[1],4);r[1]+=4u;r[15]=0x2500u;return 0;
        case 0x2500u: /* str r3,[r2],r0 */
            STORE(r[2],r[3],4);r[2]+=r[0];r[15]=0x2504u;return 0;
        case 0x2504u: /* ldr r3,[r1],#0x4 */
            LOAD(r[3],r[1],4);r[1]+=4u;r[15]=0x2508u;return 0;
        case 0x2508u: /* str r3,[r2],r0 */
            STORE(r[2],r[3],4);r[2]+=r[0];r[15]=0x250cu;return 0;
        case 0x250cu: /* ldr r3,[r1],#0x4 */
            LOAD(r[3],r[1],4);r[1]+=4u;r[15]=0x2510u;return 0;
        case 0x2510u: /* str r3,[r2],r0 */
            STORE(r[2],r[3],4);r[2]+=r[0];r[15]=0x2514u;return 0;
        case 0x2514u: /* ldr r3,[r1],#0x4 */
            LOAD(r[3],r[1],4);r[1]+=4u;r[15]=0x2518u;return 0;
        case 0x2518u: /* str r3,[r2,#0x0] */
            STORE(r[2]+0u,r[3],4);r[15]=0x251cu;return 0;
        case 0x251cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2528u: /* ands r1,r1,#0xff */
            temporary=r[1]&255u;r[1]=temporary;logic_flags(state,temporary,0,255u);r[15]=0x252cu;return 0;
        case 0x252cu: /* beq 0x000025cc */
            if(state->cpsr&0x40000000u){r[15]=9676u;}else{r[15]=0x2530u;}return 0;
        case 0x2530u: /* mov r1,r1, lsl #0x1 */
            r[1]=(r[1]<<1);r[15]=0x2534u;return 0;
        case 0x2534u: /* strb r1,[r5,#0x36] */
            STORE(r[5]+54u,r[1],1);r[15]=0x2538u;return 0;
        case 0x2538u: /* mov r1,r1, lsr #0x4 */
            r[1]=(r[1]>>4);r[15]=0x253cu;return 0;
        case 0x253cu: /* mov r6,#0x80 */
            r[6]=128u;r[15]=0x2540u;return 0;
        case 0x2540u: /* mov r7,#0xc000 */
            r[7]=49152u;r[15]=0x2544u;return 0;
        case 0x2544u: /* mla r7,r3,r6,r7 */
            r[7]=r[3]*r[6]+r[7];r[15]=0x2548u;return 0;
        case 0x2548u: /* ldrb r4,[r7,#0x26] */
            LOAD(r[4],r[7]+38u,1);r[15]=0x254cu;return 0;
        case 0x254cu: /* mov r4,r4, lsr #0x3 */
            r[4]=(r[4]>>3);r[15]=0x2550u;return 0;
        case 0x2550u: /* and r4,r4,#0x1f */
            r[4]=r[4]&31u;r[15]=0x2554u;return 0;
        case 0x2554u: /* sub r4,r4,#0x10 */
            r[4]=r[4]-16u;r[15]=0x2558u;return 0;
        case 0x2558u: /* adds r1,r1,r4 */
            r[1]=addition(state,r[1],r[4]);r[15]=0x255cu;return 0;
        case 0x255cu: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x2560u;return 0;
        case 0x2560u: /* bmi 0x0000256c */
            if(state->cpsr&0x80000000u){r[15]=9580u;}else{r[15]=0x2564u;}return 0;
        case 0x2564u: /* tst r1,#0x10 */
            temporary=r[1]&16u;logic_flags(state,temporary,0,16u);r[15]=0x2568u;return 0;
        case 0x2568u: /* movne r1,#0xf */
            if(!(state->cpsr&0x40000000u)){r[1]=15u;}r[15]=0x256cu;return 0;
        case 0x256cu: /* and r1,r1,#0xf */
            r[1]=r[1]&15u;r[15]=0x2570u;return 0;
        case 0x2570u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x2574u;return 0;
        case 0x2574u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x2578u;return 0;
        case 0x2578u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x257cu;return 0;
        case 0x257cu: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x2580u;return 0;
        case 0x2580u: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x2584u;return 0;
        case 0x2584u: /* beq 0x000025b4 */
            if(state->cpsr&0x40000000u){r[15]=9652u;}else{r[15]=0x2588u;}return 0;
        case 0x2588u: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x258cu;return 0;
        case 0x258cu: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x2590u;return 0;
        case 0x2590u: /* bne 0x000025b4 */
            if(!(state->cpsr&0x40000000u)){r[15]=9652u;}else{r[15]=0x2594u;}return 0;
        case 0x2594u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x2598u;return 0;
        case 0x2598u: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x259cu;return 0;
        case 0x259cu: /* bne 0x000025b4 */
            if(!(state->cpsr&0x40000000u)){r[15]=9652u;}else{r[15]=0x25a0u;}return 0;
        case 0x25a0u: /* ldr r9,[r6,#0x10] */
            LOAD(r[9],r[6]+16u,4);r[15]=0x25a4u;return 0;
        case 0x25a4u: /* ldr r4,[r9,#0x10] */
            LOAD(r[4],r[9]+16u,4);r[15]=0x25a8u;return 0;
        case 0x25a8u: /* and r4,r4,#0xf */
            r[4]=r[4]&15u;r[15]=0x25acu;return 0;
        case 0x25acu: /* orr r4,r4,r1, lsl #0x4 */
            r[4]=r[4]|(r[1]<<4);r[15]=0x25b0u;return 0;
        case 0x25b0u: /* strb r4,[r8,#0x20] */
            STORE(r[8]+32u,r[4],1);r[15]=0x25b4u;return 0;
        case 0x25b4u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x25b8u;return 0;
        case 0x25b8u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x25bcu;return 0;
        case 0x25bcu: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x25c0u;return 0;
        case 0x25c0u: /* bne 0x0000257c */
            if(!(state->cpsr&0x40000000u)){r[15]=9596u;}else{r[15]=0x25c4u;}return 0;
        case 0x25c4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x25ccu: /* strb r1,[r5,#0x36] */
            STORE(r[5]+54u,r[1],1);r[15]=0x25d0u;return 0;
        case 0x25d0u: /* mov r6,#0x80 */
            r[6]=128u;r[15]=0x25d4u;return 0;
        case 0x25d4u: /* mov r7,#0xc000 */
            r[7]=49152u;r[15]=0x25d8u;return 0;
        case 0x25d8u: /* mla r7,r3,r6,r7 */
            r[7]=r[3]*r[6]+r[7];r[15]=0x25dcu;return 0;
        case 0x25dcu: /* ldrb r5,[r7,#0x26] */
            LOAD(r[5],r[7]+38u,1);r[15]=0x25e0u;return 0;
        case 0x25e0u: /* mov r5,r5, lsr #0x3 */
            r[5]=(r[5]>>3);r[15]=0x25e4u;return 0;
        case 0x25e4u: /* and r5,r5,#0x1f */
            r[5]=r[5]&31u;r[15]=0x25e8u;return 0;
        case 0x25e8u: /* sub r5,r5,#0x20 */
            r[5]=r[5]-32u;r[15]=0x25ecu;return 0;
        case 0x25ecu: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x25f0u;return 0;
        case 0x25f0u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x25f4u;return 0;
        case 0x25f4u: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x25f8u;return 0;
        case 0x25f8u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x25fcu;return 0;
        case 0x25fcu: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x2600u;return 0;
        case 0x2600u: /* beq 0x00002650 */
            if(state->cpsr&0x40000000u){r[15]=9808u;}else{r[15]=0x2604u;}return 0;
        case 0x2604u: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x2608u;return 0;
        case 0x2608u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x260cu;return 0;
        case 0x260cu: /* bne 0x00002650 */
            if(!(state->cpsr&0x40000000u)){r[15]=9808u;}else{r[15]=0x2610u;}return 0;
        case 0x2610u: /* ldrb r9,[r6,#0x5] */
            LOAD(r[9],r[6]+5u,1);r[15]=0x2614u;return 0;
        case 0x2614u: /* cmp r9,r2 */
            (void)subtract(state,r[9],r[2]);r[15]=0x2618u;return 0;
        case 0x2618u: /* bne 0x00002650 */
            if(!(state->cpsr&0x40000000u)){r[15]=9808u;}else{r[15]=0x261cu;}return 0;
        case 0x261cu: /* ldr r9,[r6,#0x10] */
            LOAD(r[9],r[6]+16u,4);r[15]=0x2620u;return 0;
        case 0x2620u: /* ldr r1,[r9,#0x10] */
            LOAD(r[1],r[9]+16u,4);r[15]=0x2624u;return 0;
        case 0x2624u: /* and r4,r1,#0xf */
            r[4]=r[1]&15u;r[15]=0x2628u;return 0;
        case 0x2628u: /* mov r1,r1, lsr #0x4 */
            r[1]=(r[1]>>4);r[15]=0x262cu;return 0;
        case 0x262cu: /* and r1,r1,#0xf */
            r[1]=r[1]&15u;r[15]=0x2630u;return 0;
        case 0x2630u: /* adds r1,r1,r5 */
            r[1]=addition(state,r[1],r[5]);r[15]=0x2634u;return 0;
        case 0x2634u: /* movcs r1,#0x0 */
            if(state->cpsr&0x20000000u){r[1]=0u;}r[15]=0x2638u;return 0;
        case 0x2638u: /* bcs 0x00002644 */
            if(state->cpsr&0x20000000u){r[15]=9796u;}else{r[15]=0x263cu;}return 0;
        case 0x263cu: /* tst r1,#0x10 */
            temporary=r[1]&16u;logic_flags(state,temporary,0,16u);r[15]=0x2640u;return 0;
        case 0x2640u: /* movne r1,#0xf */
            if(!(state->cpsr&0x40000000u)){r[1]=15u;}r[15]=0x2644u;return 0;
        case 0x2644u: /* and r1,r1,#0x0 */
            r[1]=r[1]&0u;r[15]=0x2648u;return 0;
        case 0x2648u: /* orr r4,r4,r1, lsl #0x4 */
            r[4]=r[4]|(r[1]<<4);r[15]=0x264cu;return 0;
        case 0x264cu: /* strb r4,[r8,#0x20] */
            STORE(r[8]+32u,r[4],1);r[15]=0x2650u;return 0;
        case 0x2650u: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x2654u;return 0;
        case 0x2654u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x2658u;return 0;
        case 0x2658u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x265cu;return 0;
        case 0x265cu: /* bne 0x000025f8 */
            if(!(state->cpsr&0x40000000u)){r[15]=9720u;}else{r[15]=0x2660u;}return 0;
        case 0x2660u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2668u: /* ldrb r1,[r5,#0x24] */
            LOAD(r[1],r[5]+36u,1);r[15]=0x266cu;return 0;
        case 0x266cu: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x2670u;return 0;
        case 0x2670u: /* bne 0x00002684 */
            if(!(state->cpsr&0x40000000u)){r[15]=9860u;}else{r[15]=0x2674u;}return 0;
        case 0x2674u: /* ldrb r1,[r5,#0x25] */
            LOAD(r[1],r[5]+37u,1);r[15]=0x2678u;return 0;
        case 0x2678u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x267cu;return 0;
        case 0x267cu: /* bne 0x00002690 */
            if(!(state->cpsr&0x40000000u)){r[15]=9872u;}else{r[15]=0x2680u;}return 0;
        case 0x2680u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2684u: /* adds r1,r1,#0x1 */
            r[1]=addition(state,r[1],1u);r[15]=0x2688u;return 0;
        case 0x2688u: /* movcs r1,#0xff */
            if(state->cpsr&0x20000000u){r[1]=255u;}r[15]=0x268cu;return 0;
        case 0x268cu: /* b 0x00001b54 */
            r[15]=6996u;return 0;
        case 0x2690u: /* add r1,r1,#0x1 */
            r[1]=r[1]+1u;r[15]=0x2694u;return 0;
        case 0x2694u: /* movcs r1,#0xff */
            if(state->cpsr&0x20000000u){r[1]=255u;}r[15]=0x2698u;return 0;
        case 0x2698u: /* b 0x00001b08 */
            r[15]=6920u;return 0;
        case 0x269cu: /* ldrb r1,[r5,#0x24] */
            LOAD(r[1],r[5]+36u,1);r[15]=0x26a0u;return 0;
        case 0x26a0u: /* tst r1,#0x80 */
            temporary=r[1]&128u;logic_flags(state,temporary,0,128u);r[15]=0x26a4u;return 0;
        case 0x26a4u: /* bne 0x000026b8 */
            if(!(state->cpsr&0x40000000u)){r[15]=9912u;}else{r[15]=0x26a8u;}return 0;
        case 0x26a8u: /* ldrb r0,[r5,#0x25] */
            LOAD(r[0],r[5]+37u,1);r[15]=0x26acu;return 0;
        case 0x26acu: /* tst r0,#0x80 */
            temporary=r[0]&128u;logic_flags(state,temporary,0,128u);r[15]=0x26b0u;return 0;
        case 0x26b0u: /* bne 0x000026cc */
            if(!(state->cpsr&0x40000000u)){r[15]=9932u;}else{r[15]=0x26b4u;}return 0;
        case 0x26b4u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x26b8u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x26bcu;return 0;
        case 0x26bcu: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x26c0u;return 0;
        case 0x26c0u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x26c4u;return 0;
        case 0x26c4u: /* orr r1,r1,#0x80 */
            r[1]=r[1]|128u;r[15]=0x26c8u;return 0;
        case 0x26c8u: /* b 0x00001b54 */
            r[15]=6996u;return 0;
        case 0x26ccu: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x26d0u;return 0;
        case 0x26d0u: /* subs r1,r1,#0x1 */
            r[1]=subtract(state,r[1],1u);r[15]=0x26d4u;return 0;
        case 0x26d4u: /* movmi r1,#0x0 */
            if(state->cpsr&0x80000000u){r[1]=0u;}r[15]=0x26d8u;return 0;
        case 0x26d8u: /* orr r1,r1,#0x80 */
            r[1]=r[1]|128u;r[15]=0x26dcu;return 0;
        case 0x26dcu: /* b 0x00001b08 */
            r[15]=6920u;return 0;
        case 0x26e0u: /* strb r1,[r5,#0x23] */
            STORE(r[5]+35u,r[1],1);r[15]=0x26e4u;return 0;
        case 0x26e4u: /* ldrb r1,[r5,#0x24] */
            LOAD(r[1],r[5]+36u,1);r[15]=0x26e8u;return 0;
        case 0x26e8u: /* orr r1,r1,#0x80 */
            r[1]=r[1]|128u;r[15]=0x26ecu;return 0;
        case 0x26ecu: /* strb r1,[r5,#0x24] */
            STORE(r[5]+36u,r[1],1);r[15]=0x26f0u;return 0;
        case 0x26f0u: /* ldrb r1,[r5,#0x25] */
            LOAD(r[1],r[5]+37u,1);r[15]=0x26f4u;return 0;
        case 0x26f4u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x26f8u;return 0;
        case 0x26f8u: /* strb r1,[r5,#0x25] */
            STORE(r[5]+37u,r[1],1);r[15]=0x26fcu;return 0;
        case 0x26fcu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2700u: /* strb r1,[r5,#0x22] */
            STORE(r[5]+34u,r[1],1);r[15]=0x2704u;return 0;
        case 0x2704u: /* ldrb r1,[r5,#0x24] */
            LOAD(r[1],r[5]+36u,1);r[15]=0x2708u;return 0;
        case 0x2708u: /* orr r1,r1,#0x80 */
            r[1]=r[1]|128u;r[15]=0x270cu;return 0;
        case 0x270cu: /* strb r1,[r5,#0x24] */
            STORE(r[5]+36u,r[1],1);r[15]=0x2710u;return 0;
        case 0x2710u: /* ldrb r1,[r5,#0x25] */
            LOAD(r[1],r[5]+37u,1);r[15]=0x2714u;return 0;
        case 0x2714u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2718u;return 0;
        case 0x2718u: /* strb r1,[r5,#0x25] */
            STORE(r[5]+37u,r[1],1);r[15]=0x271cu;return 0;
        case 0x271cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2720u: /* strb r1,[r5,#0x23] */
            STORE(r[5]+35u,r[1],1);r[15]=0x2724u;return 0;
        case 0x2724u: /* ldrb r1,[r5,#0x25] */
            LOAD(r[1],r[5]+37u,1);r[15]=0x2728u;return 0;
        case 0x2728u: /* orr r1,r1,#0x80 */
            r[1]=r[1]|128u;r[15]=0x272cu;return 0;
        case 0x272cu: /* strb r1,[r5,#0x25] */
            STORE(r[5]+37u,r[1],1);r[15]=0x2730u;return 0;
        case 0x2730u: /* ldrb r1,[r5,#0x24] */
            LOAD(r[1],r[5]+36u,1);r[15]=0x2734u;return 0;
        case 0x2734u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2738u;return 0;
        case 0x2738u: /* strb r1,[r5,#0x24] */
            STORE(r[5]+36u,r[1],1);r[15]=0x273cu;return 0;
        case 0x273cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2740u: /* strb r1,[r5,#0x22] */
            STORE(r[5]+34u,r[1],1);r[15]=0x2744u;return 0;
        case 0x2744u: /* ldrb r1,[r5,#0x25] */
            LOAD(r[1],r[5]+37u,1);r[15]=0x2748u;return 0;
        case 0x2748u: /* orr r1,r1,#0x80 */
            r[1]=r[1]|128u;r[15]=0x274cu;return 0;
        case 0x274cu: /* strb r1,[r5,#0x25] */
            STORE(r[5]+37u,r[1],1);r[15]=0x2750u;return 0;
        case 0x2750u: /* ldrb r1,[r5,#0x24] */
            LOAD(r[1],r[5]+36u,1);r[15]=0x2754u;return 0;
        case 0x2754u: /* and r1,r1,#0x7f */
            r[1]=r[1]&127u;r[15]=0x2758u;return 0;
        case 0x2758u: /* strb r1,[r5,#0x24] */
            STORE(r[5]+36u,r[1],1);r[15]=0x275cu;return 0;
        case 0x275cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x2760u: /* mov r0,#0x10 */
            r[0]=16u;r[15]=0x2764u;return 0;
        case 0x2764u: /* mov r1,#0x64 */
            r[1]=100u;r[15]=0x2768u;return 0;
        case 0x2768u: /* strb r1,[r6,#0x8] */
            STORE(r[6]+8u,r[1],1);r[15]=0x276cu;return 0;
        case 0x276cu: /* strb r1,[r6,#0xa] */
            STORE(r[6]+10u,r[1],1);r[15]=0x2770u;return 0;
        case 0x2770u: /* mov r1,#0x7f */
            r[1]=127u;r[15]=0x2774u;return 0;
        case 0x2774u: /* strb r1,[r6,#0x9] */
            STORE(r[6]+9u,r[1],1);r[15]=0x2778u;return 0;
        case 0x2778u: /* mov r1,#0xc0 */
            r[1]=192u;r[15]=0x277cu;return 0;
        case 0x277cu: /* strb r1,[r6,#0xb] */
            STORE(r[6]+11u,r[1],1);r[15]=0x2780u;return 0;
        case 0x2780u: /* mov r1,#0x40 */
            r[1]=64u;r[15]=0x2784u;return 0;
        case 0x2784u: /* strb r1,[r6,#0x27] */
            STORE(r[6]+39u,r[1],1);r[15]=0x2788u;return 0;
        case 0x2788u: /* mov r1,#0x20 */
            r[1]=32u;r[15]=0x278cu;return 0;
        case 0x278cu: /* strb r1,[r6,#0x32] */
            STORE(r[6]+50u,r[1],1);r[15]=0x2790u;return 0;
        case 0x2790u: /* strb r1,[r6,#0x33] */
            STORE(r[6]+51u,r[1],1);r[15]=0x2794u;return 0;
        case 0x2794u: /* strb r1,[r6,#0x34] */
            STORE(r[6]+52u,r[1],1);r[15]=0x2798u;return 0;
        case 0x2798u: /* strb r1,[r6,#0x35] */
            STORE(r[6]+53u,r[1],1);r[15]=0x279cu;return 0;
        case 0x279cu: /* strb r1,[r6,#0x29] */
            STORE(r[6]+41u,r[1],1);r[15]=0x27a0u;return 0;
        case 0x27a0u: /* strb r1,[r6,#0x2b] */
            STORE(r[6]+43u,r[1],1);r[15]=0x27a4u;return 0;
        case 0x27a4u: /* strb r1,[r6,#0x2d] */
            STORE(r[6]+45u,r[1],1);r[15]=0x27a8u;return 0;
        case 0x27a8u: /* strb r1,[r6,#0x2f] */
            STORE(r[6]+47u,r[1],1);r[15]=0x27acu;return 0;
        case 0x27acu: /* strb r1,[r6,#0x31] */
            STORE(r[6]+49u,r[1],1);r[15]=0x27b0u;return 0;
        case 0x27b0u: /* strb r1,[r6,#0x37] */
            STORE(r[6]+55u,r[1],1);r[15]=0x27b4u;return 0;
        case 0x27b4u: /* strb r1,[r6,#0x38] */
            STORE(r[6]+56u,r[1],1);r[15]=0x27b8u;return 0;
        case 0x27b8u: /* strb r1,[r6,#0x3a] */
            STORE(r[6]+58u,r[1],1);r[15]=0x27bcu;return 0;
        case 0x27bcu: /* strb r1,[r6,#0x3b] */
            STORE(r[6]+59u,r[1],1);r[15]=0x27c0u;return 0;
        case 0x27c0u: /* strb r1,[r6,#0x39] */
            STORE(r[6]+57u,r[1],1);r[15]=0x27c4u;return 0;
        case 0x27c4u: /* mov r1,#0x0 */
            r[1]=0u;r[15]=0x27c8u;return 0;
        case 0x27c8u: /* str r1,[r6,#0x40] */
            STORE(r[6]+64u,r[1],4);r[15]=0x27ccu;return 0;
        case 0x27ccu: /* strb r1,[r6,#0x3] */
            STORE(r[6]+3u,r[1],1);r[15]=0x27d0u;return 0;
        case 0x27d0u: /* str r1,[r6,#0x4] */
            STORE(r[6]+4u,r[1],4);r[15]=0x27d4u;return 0;
        case 0x27d4u: /* strb r1,[r6,#0x2] */
            STORE(r[6]+2u,r[1],1);r[15]=0x27d8u;return 0;
        case 0x27d8u: /* strb r1,[r6,#0x28] */
            STORE(r[6]+40u,r[1],1);r[15]=0x27dcu;return 0;
        case 0x27dcu: /* strb r1,[r6,#0x2a] */
            STORE(r[6]+42u,r[1],1);r[15]=0x27e0u;return 0;
        case 0x27e0u: /* strb r1,[r6,#0x2c] */
            STORE(r[6]+44u,r[1],1);r[15]=0x27e4u;return 0;
        case 0x27e4u: /* strb r1,[r6,#0x2e] */
            STORE(r[6]+46u,r[1],1);r[15]=0x27e8u;return 0;
        case 0x27e8u: /* strb r1,[r6,#0x30] */
            STORE(r[6]+48u,r[1],1);r[15]=0x27ecu;return 0;
        case 0x27ecu: /* strb r1,[r6,#0x3d] */
            STORE(r[6]+61u,r[1],1);r[15]=0x27f0u;return 0;
        case 0x27f0u: /* strb r1,[r6,#0x26] */
            STORE(r[6]+38u,r[1],1);r[15]=0x27f4u;return 0;
        case 0x27f4u: /* strb r1,[r6,#0x36] */
            STORE(r[6]+54u,r[1],1);r[15]=0x27f8u;return 0;
        case 0x27f8u: /* add r6,r6,#0x48 */
            r[6]=r[6]+72u;r[15]=0x27fcu;return 0;
        case 0x27fcu: /* subs r0,r0,#0x1 */
            r[0]=subtract(state,r[0],1u);r[15]=0x2800u;return 0;
        case 0x2800u: /* bne 0x00002764 */
            if(!(state->cpsr&0x40000000u)){r[15]=10084u;}else{r[15]=0x2804u;}return 0;
        case 0x2804u: /* mov r6,#0xe800 */
            r[6]=59392u;r[15]=0x2808u;return 0;
        case 0x2808u: /* mov r7,#0x30 */
            r[7]=48u;r[15]=0x280cu;return 0;
        case 0x280cu: /* mov r8,#0x800000 */
            r[8]=8388608u;r[15]=0x2810u;return 0;
        case 0x2810u: /* ldr r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,4);r[15]=0x2814u;return 0;
        case 0x2814u: /* movs r9,r9 */
            r[9]=r[9];logic_flags(state,r[9],0,0);r[15]=0x2818u;return 0;
        case 0x2818u: /* beq 0x0000285c */
            if(state->cpsr&0x40000000u){r[15]=10332u;}else{r[15]=0x281cu;}return 0;
        case 0x281cu: /* ldrb r9,[r6,#0x4] */
            LOAD(r[9],r[6]+4u,1);r[15]=0x2820u;return 0;
        case 0x2820u: /* cmp r9,r3 */
            (void)subtract(state,r[9],r[3]);r[15]=0x2824u;return 0;
        case 0x2824u: /* bne 0x0000285c */
            if(!(state->cpsr&0x40000000u)){r[15]=10332u;}else{r[15]=0x2828u;}return 0;
        case 0x2828u: /* ldrb r9,[r6,#0x0] */
            LOAD(r[9],r[6]+0u,1);r[15]=0x282cu;return 0;
        case 0x282cu: /* bic r9,r9,#0x40 */
            r[9]=r[9]&~64u;r[15]=0x2830u;return 0;
        case 0x2830u: /* strb r9,[r6,#0x0] */
            STORE(r[6]+0u,r[9],1);r[15]=0x2834u;return 0;
        case 0x2834u: /* tst r9,#0x80 */
            temporary=r[9]&128u;logic_flags(state,temporary,0,128u);r[15]=0x2838u;return 0;
        case 0x2838u: /* bne 0x0000285c */
            if(!(state->cpsr&0x40000000u)){r[15]=10332u;}else{r[15]=0x283cu;}return 0;
        case 0x283cu: /* mov r9,#0x0 */
            r[9]=0u;r[15]=0x2840u;return 0;
        case 0x2840u: /* strb r9,[r6,#0x3f] */
            STORE(r[6]+63u,r[9],1);r[15]=0x2844u;return 0;
        case 0x2844u: /* ldr r9,[r8,#0x0] */
            LOAD(r[9],r[8]+0u,4);r[15]=0x2848u;return 0;
        case 0x2848u: /* bic r9,r9,#0xff000000 */
            r[9]=r[9]&~4278190080u;r[15]=0x284cu;return 0;
        case 0x284cu: /* bic r9,r9,#0xff0000 */
            r[9]=r[9]&~16711680u;r[15]=0x2850u;return 0;
        case 0x2850u: /* bic r9,r9,#0x4000 */
            r[9]=r[9]&~16384u;r[15]=0x2854u;return 0;
        case 0x2854u: /* orr r9,r9,#0x8000 */
            r[9]=r[9]|32768u;r[15]=0x2858u;return 0;
        case 0x2858u: /* str r9,[r8,#0x0] */
            STORE(r[8]+0u,r[9],4);r[15]=0x285cu;return 0;
        case 0x285cu: /* add r6,r6,#0x40 */
            r[6]=r[6]+64u;r[15]=0x2860u;return 0;
        case 0x2860u: /* add r8,r8,#0x80 */
            r[8]=r[8]+128u;r[15]=0x2864u;return 0;
        case 0x2864u: /* subs r7,r7,#0x1 */
            r[7]=subtract(state,r[7],1u);r[15]=0x2868u;return 0;
        case 0x2868u: /* bne 0x00002810 */
            if(!(state->cpsr&0x40000000u)){r[15]=10256u;}else{r[15]=0x286cu;}return 0;
        case 0x286cu: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5c28u: /* ldr r3,[0x6120] */
            LOAD(r[3],24864u,4);r[15]=0x5c2cu;return 0;
        case 0x5c2cu: /* ldr r1,[r3,#0x0] */
            LOAD(r[1],r[3]+0u,4);r[15]=0x5c30u;return 0;
        case 0x5c30u: /* movs r1,r1 */
            r[1]=r[1];logic_flags(state,r[1],0,0);r[15]=0x5c34u;return 0;
        case 0x5c34u: /* ldreq r1,[0x6124] */
            if(state->cpsr&0x40000000u){LOAD(r[1],24868u,4);}r[15]=0x5c38u;return 0;
        case 0x5c38u: /* ldr r3,[r1,#0x0] */
            LOAD(r[3],r[1]+0u,4);r[15]=0x5c3cu;return 0;
        case 0x5c3cu: /* ldr r4,[0x5d28] */
            LOAD(r[4],23848u,4);r[15]=0x5c40u;return 0;
        case 0x5c40u: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x5c44u;return 0;
        case 0x5c44u: /* movne r1,#0x2 */
            if(!(state->cpsr&0x40000000u)){r[1]=2u;}r[15]=0x5c48u;return 0;
        case 0x5c48u: /* bne 0x00005d44 */
            if(!(state->cpsr&0x40000000u)){r[15]=23876u;}else{r[15]=0x5c4cu;}return 0;
        case 0x5c4cu: /* ldrb r3,[r1,#0x4] */
            LOAD(r[3],r[1]+4u,1);r[15]=0x5c50u;return 0;
        case 0x5c50u: /* cmp r3,#0x1 */
            (void)subtract(state,r[3],1u);r[15]=0x5c54u;return 0;
        case 0x5c54u: /* movne r1,#0x8 */
            if(!(state->cpsr&0x40000000u)){r[1]=8u;}r[15]=0x5c58u;return 0;
        case 0x5c58u: /* bne 0x00005d44 */
            if(!(state->cpsr&0x40000000u)){r[15]=23876u;}else{r[15]=0x5c5cu;}return 0;
        case 0x5c5cu: /* ldr r2,[r1,#0x8] */
            LOAD(r[2],r[1]+8u,4);r[15]=0x5c60u;return 0;
        case 0x5c60u: /* sub r2,r2,#0x4 */
            r[2]=r[2]-4u;r[15]=0x5c64u;return 0;
        case 0x5c64u: /* ldr r3,[r1,r2] */
            LOAD(r[3],r[1]+r[2],4);r[15]=0x5c68u;return 0;
        case 0x5c68u: /* ldr r4,[0x5d2c] */
            LOAD(r[4],23852u,4);r[15]=0x5c6cu;return 0;
        case 0x5c6cu: /* cmp r3,r4 */
            (void)subtract(state,r[3],r[4]);r[15]=0x5c70u;return 0;
        case 0x5c70u: /* movne r1,#0x4 */
            if(!(state->cpsr&0x40000000u)){r[1]=4u;}r[15]=0x5c74u;return 0;
        case 0x5c74u: /* bne 0x00005d44 */
            if(!(state->cpsr&0x40000000u)){r[15]=23876u;}else{r[15]=0x5c78u;}return 0;
        case 0x5c78u: /* ldrb r2,[r1,#0xc] */
            LOAD(r[2],r[1]+12u,1);r[15]=0x5c7cu;return 0;
        case 0x5c7cu: /* cmp r0,r2 */
            (void)subtract(state,r[0],r[2]);r[15]=0x5c80u;return 0;
        case 0x5c80u: /* movcs r1,#0x10 */
            if(state->cpsr&0x20000000u){r[1]=16u;}r[15]=0x5c84u;return 0;
        case 0x5c84u: /* bcs 0x00005d44 */
            if(state->cpsr&0x20000000u){r[15]=23876u;}else{r[15]=0x5c88u;}return 0;
        case 0x5c88u: /* adr r2,0x5d58 */
            r[2]=23896u;r[15]=0x5c8cu;return 0;
        case 0x5c8cu: /* strb r0,[r2,#0x0] */
            STORE(r[2]+0u,r[0],1);r[15]=0x5c90u;return 0;
        case 0x5c90u: /* mov r0,r0, lsl #0x2 */
            r[0]=(r[0]<<2);r[15]=0x5c94u;return 0;
        case 0x5c94u: /* add r0,r0,#0x10 */
            r[0]=r[0]+16u;r[15]=0x5c98u;return 0;
        case 0x5c98u: /* ldr r2,[r1,r0] */
            LOAD(r[2],r[1]+r[0],4);r[15]=0x5c9cu;return 0;
        case 0x5c9cu: /* add r2,r2,r1 */
            r[2]=r[2]+r[1];r[15]=0x5ca0u;return 0;
        case 0x5ca0u: /* ldrb r1,[r2,#0x20] */
            LOAD(r[1],r[2]+32u,1);r[15]=0x5ca4u;return 0;
        case 0x5ca4u: /* mov r1,r1, lsl #0x2 */
            r[1]=(r[1]<<2);r[15]=0x5ca8u;return 0;
        case 0x5ca8u: /* adr r3,0x5d30 */
            r[3]=23856u;r[15]=0x5cacu;return 0;
        case 0x5cacu: /* ldr r0,[r3,r1] */
            LOAD(r[0],r[3]+r[1],4);r[15]=0x5cb0u;return 0;
        case 0x5cb0u: /* ldr r3,[0x6128] */
            LOAD(r[3],24872u,4);r[15]=0x5cb4u;return 0;
        case 0x5cb4u: /* ldr r4,[r3,#0x0] */
            LOAD(r[4],r[3]+0u,4);r[15]=0x5cb8u;return 0;
        case 0x5cb8u: /* ldr r3,[r3,#0x4] */
            LOAD(r[3],r[3]+4u,4);r[15]=0x5cbcu;return 0;
        case 0x5cbcu: /* movs r4,r4 */
            r[4]=r[4];logic_flags(state,r[4],0,0);r[15]=0x5cc0u;return 0;
        case 0x5cc0u: /* moveq r1,#0x1 */
            if(state->cpsr&0x40000000u){r[1]=1u;}r[15]=0x5cc4u;return 0;
        case 0x5cc4u: /* beq 0x00005d44 */
            if(state->cpsr&0x40000000u){r[15]=23876u;}else{r[15]=0x5cc8u;}return 0;
        case 0x5cc8u: /* mov r5,#0x18000 */
            r[5]=98304u;r[15]=0x5cccu;return 0;
        case 0x5cccu: /* cmp r4,r5 */
            (void)subtract(state,r[4],r[5]);r[15]=0x5cd0u;return 0;
        case 0x5cd0u: /* movmi r1,#0x1 */
            if(state->cpsr&0x80000000u){r[1]=1u;}r[15]=0x5cd4u;return 0;
        case 0x5cd4u: /* bmi 0x00005d44 */
            if(state->cpsr&0x80000000u){r[15]=23876u;}else{r[15]=0x5cd8u;}return 0;
        case 0x5cd8u: /* ldrb r5,[r2,#0x23] */
            LOAD(r[5],r[2]+35u,1);r[15]=0x5cdcu;return 0;
        case 0x5cdcu: /* tst r5,#0x30 */
            temporary=r[5]&48u;logic_flags(state,temporary,0,48u);r[15]=0x5ce0u;return 0;
        case 0x5ce0u: /* bne 0x00005cec */
            if(!(state->cpsr&0x40000000u)){r[15]=23788u;}else{r[15]=0x5ce4u;}return 0;
        case 0x5ce4u: /* cmp r3,r0 */
            (void)subtract(state,r[3],r[0]);r[15]=0x5ce8u;return 0;
        case 0x5ce8u: /* bmi 0x00005d44 */
            if(state->cpsr&0x80000000u){r[15]=23876u;}else{r[15]=0x5cecu;}return 0;
        case 0x5cecu: /* mov r4,r4, lsr #0xb */
            r[4]=(r[4]>>11);r[15]=0x5cf0u;return 0;
        case 0x5cf0u: /* ldr r3,[0x5d40] */
            LOAD(r[3],23872u,4);r[15]=0x5cf4u;return 0;
        case 0x5cf4u: /* and r4,r4,r3 */
            r[4]=r[4]&r[3];r[15]=0x5cf8u;return 0;
        case 0x5cf8u: /* and r1,r1,#0xc */
            r[1]=r[1]&12u;r[15]=0x5cfcu;return 0;
        case 0x5cfcu: /* orr r1,r4,r1, lsl #0xb */
            r[1]=r[4]|(r[1]<<11);r[15]=0x5d00u;return 0;
        case 0x5d00u: /* ldr r0,[0x612c] */
            LOAD(r[0],24876u,4);r[15]=0x5d04u;return 0;
        case 0x5d04u: /* str r1,[r0,#0x0] */
            STORE(r[0]+0u,r[1],4);r[15]=0x5d08u;return 0;
        case 0x5d08u: /* ldr r0,[0x6130] */
            LOAD(r[0],24880u,4);r[15]=0x5d0cu;return 0;
        case 0x5d0cu: /* str r2,[r0,#0x0] */
            STORE(r[0]+0u,r[2],4);r[15]=0x5d10u;return 0;
        case 0x5d10u: /* ldr r0,[0x6134] */
            LOAD(r[0],24884u,4);r[15]=0x5d14u;return 0;
        case 0x5d14u: /* mov r2,#0x1 */
            r[2]=1u;r[15]=0x5d18u;return 0;
        case 0x5d18u: /* strb r2,[r0,#0x0] */
            STORE(r[0]+0u,r[2],1);r[15]=0x5d1cu;return 0;
        case 0x5d1cu: /* mov r0,#0x0 */
            r[0]=0u;r[15]=0x5d20u;return 0;
        case 0x5d20u: /* mov pc,lr */
            r[15]=r[14];return 0;
        case 0x5d44u: /* ldr r2,[0x611c] */
            LOAD(r[2],24860u,4);r[15]=0x5d48u;return 0;
        case 0x5d48u: /* ldrb r0,[r2,#0x0] */
            LOAD(r[0],r[2]+0u,1);r[15]=0x5d4cu;return 0;
        case 0x5d4cu: /* orr r0,r0,r1 */
            r[0]=r[0]|r[1];r[15]=0x5d50u;return 0;
        case 0x5d50u: /* strb r0,[r2,#0x0] */
            STORE(r[2]+0u,r[0],1);r[15]=0x5d54u;return 0;
        case 0x5d54u: /* mov pc,lr */
            r[15]=r[14];return 0;
        default:*handled=0;return 0;
 }}
}

#include <stdint.h>
struct sa_cpu_state { uint32_t r[16],sr,pr,pc; };
typedef int (*sa_dispatch)(uint8_t *,struct sa_cpu_state *,uint32_t,void *);
static uint32_t rd(const uint8_t *m,uint32_t a) {
    a&=0xffffffu;
    return (uint32_t)m[a]|((uint32_t)m[a+1]<<8)|((uint32_t)m[a+2]<<16)|((uint32_t)m[a+3]<<24);
}
static void wr(uint8_t *m,uint32_t a,uint32_t v) {
    a&=0xffffffu;
    for(unsigned i=0;i<4;i++)m[a+i]=(uint8_t)(v>>(8*i));
}
/* Same 36 retail bytes as sa_file_backend_poll, using the connected CPU ABI.
 * Each service sees the retail frame and PC; host errors preserve that frame. */
int sa_file_backend_poll_state(uint8_t *m,struct sa_cpu_state *s,sa_dispatch call,void *ctx) {
    s->r[2]=rd(m,0x8c6006b4u);
    s->r[0]=64;
    s->r[15]-=4;wr(m,s->r[15],s->pr);
    s->r[3]=rd(m,s->r[2]);s->r[1]=rd(m,s->r[3]+12);
    s->r[0]=rd(m,s->r[1]+s->r[0]);
    s->pr=0x8c6005d2u;s->pc=s->r[0];
    int result=call(m,s,s->pc,ctx);if(result)return result;
    s->r[2]=rd(m,0x8c6006b4u);s->r[3]=rd(m,s->r[2]);
    s->r[1]=rd(m,s->r[3]+28);
    s->sr=(s->sr&~1u)|!s->r[1];
    s->pr=rd(m,s->r[15]);s->r[15]+=4;
    s->pc=s->r[1]?0x8c5ffa12u:s->pr;
    if(s->r[1])return call(m,s,s->pc,ctx);
    return 0;
}

/* Error-aware connected ABI for retail wrapper at 8c6452d6. */
int sa_device_pump_call_connected(uint8_t *m,struct sa_cpu_state *s,sa_dispatch call,void *ctx) {
    s->r[15]-=4;wr(m,s->r[15],s->pr);
    s->r[3]=rd(m,0x8c64533cu);
    s->pr=0x8c6452deu;s->pc=s->r[3];
    int result=call(m,s,s->pc,ctx);if(result)return result;
    s->pr=rd(m,s->r[15]);s->r[15]+=4;
    s->r[0]=1;s->pc=s->pr;
    return 0;
}

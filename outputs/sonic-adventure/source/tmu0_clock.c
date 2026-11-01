/* Authored TMU0 peripheral core, not recovered game code.
 * Renesas R01UH0456EJ0702, sections 12.2-12.5:
 * https://www.renesas.com/en/document/mah/sh7750-sh7750s-sh7750r-group-users-manual-hardware
 * Elapsed PCLK cycles are supplied by a clock domain, never by register reads.
 * P4 registers, internal divided clocks, channel 0 only. External/RTC clocks,
 * channels 1/2, running configuration changes and SH7750 UNF write-one erratum
 * domains stop explicitly (-2), rather than inventing hardware behavior.
 * The common prescaler phase is the declared clock-domain reset phase.
 */
#include "tmu0_clock.h"
static const uint32_t addresses[5]={0xffd80000u,0xffd80004u,0xffd80008u,0xffd8000cu,0xffd80010u};
static const unsigned widths[5]={1,1,4,4,2};
static int slot(uint32_t address){
 for(unsigned i=0;i<5;i++)if(address==addresses[i])return (int)i;
 return -1;
}
SA_STAGE_EXPORT int sa_tmu0_init(struct sa_tmu0_clock *s){
 if(!s)return -6;
 volatile uint8_t *bytes=(volatile uint8_t *)s;for(unsigned i=0;i<sizeof(*s);i++)bytes[i]=0;
 s->tcor=s->tcnt=0xffffffffu;return 0;
}
SA_STAGE_EXPORT int sa_tmu0_read(const struct sa_tmu0_clock *s,uint32_t a,unsigned n,uint32_t *out){
 if(!s||!out)return -6;
 int i=slot(a);if(i<0)return -2;if(n!=widths[i])return -3;
 switch(i){case 0:*out=s->tocr;break;case 1:*out=s->tstr;break;case 2:*out=s->tcor;break;case 3:*out=s->tcnt;break;default:*out=s->tcr;break;}
 return 0;
}
SA_STAGE_EXPORT int sa_tmu0_write(struct sa_tmu0_clock *s,uint32_t a,uint32_t value,unsigned n){
 if(!s)return -6;
 int i=slot(a);if(i<0)return -2;if(n!=widths[i])return -3;
 if(n<4u)value&=(1u<<(8u*n))-1u;
 if(i==0){if(value&~1u)return -2;s->tocr=value;return 0;}
 if(i==1){
  if(value&~1u)return -2;
  if((value&1u)&&(s->tcr&7u)>4u)return -2;
  s->tstr=value;return 0;
 }
 if(i==2||i==3){
  if(s->tstr&1u)return -2;
  if(i==2)s->tcor=value;else s->tcnt=value;return 0;
 }
 if(value&~0x13fu)return -2;
 if((s->tstr&1u)&&((value^s->tcr)&0x3fu))return -2;
 if((value&s->tcr&0x100u)!=0)return -2; /* SH7750 erratum is not simulated. */
 s->tcr=(value&0x3fu)|(s->tcr&value&0x100u);return 0;
}
SA_STAGE_EXPORT int sa_tmu0_advance(struct sa_tmu0_clock *s,uint64_t cycles){
 if(!s)return -6;
 if(cycles>UINT64_MAX-s->peripheral_cycles)return -6;
 if(!(s->tstr&1u)){s->peripheral_cycles+=cycles;return 0;}
 unsigned select=s->tcr&7u;if(select>4u)return -2;
 uint32_t divisor=4u<<(2u*select);
 uint64_t steps=cycles/divisor+((s->peripheral_cycles%divisor)+(cycles%divisor))/divisor;
 uint32_t next=s->tcnt;uint64_t wraps=0;
 if(steps<=s->tcnt)next-= (uint32_t)steps;
 else{
  uint64_t remaining=steps-(uint64_t)s->tcnt-1u,period=(uint64_t)s->tcor+1u;
  wraps=1u+remaining/period;next=s->tcor-(uint32_t)(remaining%period);
 }
 if(wraps>UINT64_MAX-s->underflows)return -6;
 s->peripheral_cycles+=cycles;s->tcnt=next;s->underflows+=wraps;
 if(wraps)s->tcr|=0x100u;
 return 0;
}
SA_STAGE_EXPORT uint32_t sa_tmu0_irq_pending(const struct sa_tmu0_clock *s){return s&&((s->tcr&0x120u)==0x120u);}
static int read_bus(void *opaque,uint32_t a,unsigned n,uint32_t *out){
 struct sa_tmu0_binding *b=opaque;
 return slot(a)>=0?sa_tmu0_read(b->state,a,n,out):(b->parent.read?b->parent.read(b->parent.context,a,n,out):-2);
}
static int write_bus(void *opaque,uint32_t a,uint32_t value,unsigned n){
 struct sa_tmu0_binding *b=opaque;
 return slot(a)>=0?sa_tmu0_write(b->state,a,value,n):(b->parent.write?b->parent.write(b->parent.context,a,value,n):-2);
}
static int cache_bus(void *opaque,uint32_t a,unsigned op){
 struct sa_tmu0_binding *b=opaque;return b->parent.cache?b->parent.cache(b->parent.context,a,op):-2;
}
SA_STAGE_EXPORT int sa_tmu0_bind(struct sa_tmu0_binding *b,struct sa_tmu0_clock *s,const struct sa_vertex_bus *parent,struct sa_vertex_bus *out){
 if(!b||!s||!parent||!out)return -6;
 b->state=s;b->parent=*parent;*out=(struct sa_vertex_bus){b,read_bus,write_bus,cache_bus};return 0;
}

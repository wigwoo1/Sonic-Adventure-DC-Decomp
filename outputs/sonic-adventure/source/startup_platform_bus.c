#include "startup_platform_bus.h"
/* Explicit software storage for observed startup writes. Previous register,
 * IRQ-status/mask, DMA, timing and context policies retain their old routes.
 * No interrupt delivery, timer advancement, DMA or physical cache lines. */
static const uint32_t system_addresses[SA_STARTUP_SYSTEM_WORDS]={
 0xffd00000u,0xffd00004u,0xffd00008u,0xffd0000cu,
 0xffd80000u,0xffd80004u,0xffd80008u,0xffd8000cu,0xffd80010u};
static const unsigned system_widths[SA_STARTUP_SYSTEM_WORDS]={2,2,2,2,1,1,4,4,2};
static int previous(uint32_t p){
 return p==0x005f6800u||p==0x005f6804u||p==0x005f6808u||p==0x005f688cu||
 p==0x005f6900u||p==0x005f6920u||p==0x005f6924u||(p>=0x005f7800u&&p<0x005f78c0u);
}
static uint32_t *slot(struct sa_startup_platform_memory *m,uint32_t a,unsigned *width,unsigned *alignment){
 uint32_t p=a&0x1fffffffu;
 for(unsigned i=0;i<SA_STARTUP_SYSTEM_WORDS;i++)if(a==system_addresses[i]){
  *width=system_widths[i];*alignment=*width;return &m->system_registers[i];
 }
 if(p>=0x005f6800u&&p<0x005f7d00u&&!previous(p)){
  *width=4;*alignment=4;return &m->startup_registers[(p-0x005f6800u)/4u];
 }
 if(a>=0xf4000000u&&a<0xf4004000u){
  *width=4;*alignment=32;return &m->cache_tags[(a-0xf4000000u)/32u];
 }
 return 0;
}
static void event(struct sa_startup_platform_memory *m,uint32_t kind,uint32_t a,uint32_t value,unsigned n){
 struct sa_vertex_memory *v=&m->context.hardware.timing.video.cache.event.interrupt.submit.frame.graphics.vertex;
 if(v->trace_count<4096u)v->trace[v->trace_count++]=(struct sa_vertex_bus_event){kind,a,value,n};else v->trace_dropped++;
}
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *out){
 struct sa_startup_platform_memory *m=ctx;unsigned width=0,alignment=0;uint32_t *value=slot(m,a,&width,&alignment);
 if(!value)return m->context_bus.read(m->context_bus.context,a,n,out);
 if(!out||n!=width||(a&(alignment-1u)))return -3;
 *out=*value;event(m,27u,a,*out,n);return 0;
}
static int write(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct sa_startup_platform_memory *m=ctx;unsigned width=0,alignment=0;uint32_t *out=slot(m,a,&width,&alignment);
 if(!out)return m->context_bus.write(m->context_bus.context,a,value,n);
 if(n!=width||(a&(alignment-1u)))return -3;
 uint32_t masked=value;if(n<4u)masked&=(1u<<(n*8u))-1u;
 *out=masked;event(m,28u,a,masked,n);return 0;
}
static int cache(void *ctx,uint32_t a,unsigned op){
 struct sa_startup_platform_memory *m=ctx;return m->context_bus.cache(m->context_bus.context,a,op);
}
void sa_startup_platform_bus_init(struct sa_startup_platform_memory *m,uint8_t *ram,struct sa_vertex_bus *bus,uint32_t port,uint32_t control){
 sa_transfer_context_bus_init(&m->context,ram,&m->context_bus,port,control);
 volatile uint32_t *startup=m->startup_registers,*system=m->system_registers,*tags=m->cache_tags;
 for(unsigned i=0;i<SA_STARTUP_WORDS;i++)startup[i]=0;
 for(unsigned i=0;i<SA_STARTUP_SYSTEM_WORDS;i++)system[i]=0;
 for(unsigned i=0;i<SA_STARTUP_CACHE_TAGS;i++)tags[i]=0;
 bus->context=m;bus->read=read;bus->write=write;bus->cache=cache;
}

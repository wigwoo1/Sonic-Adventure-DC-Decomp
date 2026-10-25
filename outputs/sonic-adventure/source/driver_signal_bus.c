#include "driver_signal_bus.h"
/* Borrowed raw normal-status observations, no generated events or timing.
 * A consumed observation becomes the visible last status. Acknowledge writes
 * retain the older clear-on-write policy. Unconfigured reads use that policy. */
static void event(struct sa_driver_signal_memory *m,uint32_t kind,uint32_t a,uint32_t value,unsigned n){
 struct sa_vertex_memory *v=&m->startup.context.hardware.timing.video.cache.event.interrupt.submit.frame.graphics.vertex;
 if(v->trace_count<4096u)v->trace[v->trace_count++]=(struct sa_vertex_bus_event){kind,a,value,n};else v->trace_dropped++;
}
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *value){
 struct sa_driver_signal_memory *m=ctx;
 if(a==0xffa00040u){
  if(n!=4u||!value)return -3;
  *value=m->dma_operation;event(m,29u,a,*value,n);return 0;
 }
 if((a&0x1fffffffu)==0x005f6900u&&m->configured){
  if(n!=4u||!value)return -3;
  if(m->cursor>=m->count)return -4;
  uint32_t observed=m->samples[m->cursor++];
  m->startup.context.hardware.timing.video.cache.event.interrupt.normal_status=observed;
  return m->startup_bus.read(m->startup_bus.context,a,n,value);
 }
 return m->startup_bus.read(m->startup_bus.context,a,n,value);
}
static int write(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct sa_driver_signal_memory *m=ctx;
 if(a==0xffa00040u){if(n!=4u)return -3;m->dma_operation=value;event(m,30u,a,value,n);return 0;}
 return m->startup_bus.write(m->startup_bus.context,a,value,n);
}
static int cache(void *ctx,uint32_t a,unsigned op){
 struct sa_driver_signal_memory *m=ctx;return m->startup_bus.cache(m->startup_bus.context,a,op);
}
void sa_driver_signal_bus_init(struct sa_driver_signal_memory *m,uint8_t *ram,struct sa_vertex_bus *bus,uint32_t port,uint32_t control){
 sa_startup_platform_bus_init(&m->startup,ram,&m->startup_bus,port,control);
 m->samples=0;m->count=0;m->cursor=0;m->configured=0;m->dma_operation=0;
 bus->context=m;bus->read=read;bus->write=write;bus->cache=cache;
}
int sa_driver_signal_set_samples(struct sa_driver_signal_memory *m,const uint32_t *samples,uint32_t count){
 if(!m||(!samples&&count))return -3;
 m->samples=samples;m->count=count;m->cursor=0;m->configured=1;return 0;
}

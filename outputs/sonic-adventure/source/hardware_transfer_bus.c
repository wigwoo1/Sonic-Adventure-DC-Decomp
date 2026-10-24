#include "hardware_transfer_bus.h"
static int control_address(uint32_t a,unsigned n){
 uint32_t p=a&0x1fffffffu;
 return (n==1u||n==2u||n==4u)&&!(a&(n-1u))&&p>=0x00702c00u&&p+n<=0x00702c04u;
}
static void event(struct sa_hardware_transfer_memory *m,uint32_t kind,uint32_t a,uint32_t value,unsigned n){
 struct sa_vertex_memory *v=&m->timing.video.cache.event.interrupt.submit.frame.graphics.vertex;
 if(v->trace_count<4096u)v->trace[v->trace_count++]=(struct sa_vertex_bus_event){kind,a,value,n};
 else v->trace_dropped++;
}
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *value){
 struct sa_hardware_transfer_memory *m=ctx;uint32_t p=a&0x1fffffffu;
 if(p==0x005f688cu){
  if(n!=4u||!value)return -3;
  if(!m->fifo_samples||m->fifo_cursor>=m->fifo_count)return -4;
  *value=m->fifo_samples[m->fifo_cursor++];event(m,22u,a,*value,4u);return 0;
 }
 if(p>=0x00702c00u&&p<0x00702c04u){
  if(!control_address(a,n)||!value)return -3;
  *value=m->control_word>>((p-0x00702c00u)*8u);
  if(n<4u)*value&=(1u<<(n*8u))-1u;
  event(m,23u,a,*value,n);return 0;
 }
 return m->timing_bus.read(m->timing_bus.context,a,n,value);
}
static int write(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct sa_hardware_transfer_memory *m=ctx;uint32_t p=a&0x1fffffffu;
 if(p==0x005f688cu)return -3;
 if(p>=0x00702c00u&&p<0x00702c04u){
  if(!control_address(a,n))return -3;
  uint32_t shift=(p-0x00702c00u)*8u,mask=n==4u?0xffffffffu:(1u<<(n*8u))-1u;
  value&=mask;m->control_word=(m->control_word&~(mask<<shift))|(value<<shift);
  event(m,24u,a,value,n);return 0;
 }
 return m->timing_bus.write(m->timing_bus.context,a,value,n);
}
static int cache(void *ctx,uint32_t a,unsigned op){
 struct sa_hardware_transfer_memory *m=ctx;return m->timing_bus.cache(m->timing_bus.context,a,op);
}
void sa_hardware_transfer_bus_init(struct sa_hardware_transfer_memory *m,uint8_t *ram,struct sa_vertex_bus *bus,uint32_t port,uint32_t control){
 sa_display_timing_bus_init(&m->timing,ram,&m->timing_bus,port);m->fifo_samples=0;m->fifo_count=0;m->fifo_cursor=0;m->control_word=control;
 bus->context=m;bus->read=read;bus->write=write;bus->cache=cache;
}
int sa_hardware_transfer_set_samples(struct sa_hardware_transfer_memory *m,const uint32_t *samples,uint32_t count){
 if(!m||(!samples&&count))return -3;
 m->fifo_samples=samples;m->fifo_count=count;m->fifo_cursor=0;return 0;
}

#include "transfer_context_bus.h"
static int reached(uint32_t a){uint32_t p=a&0x1fffffffu;return p>=0x005f7800u&&p<0x005f78c0u;}
static void event(struct sa_transfer_context_memory *m,uint32_t kind,uint32_t a,uint32_t value){
 struct sa_vertex_memory *v=&m->hardware.timing.video.cache.event.interrupt.submit.frame.graphics.vertex;
 if(v->trace_count<4096u)v->trace[v->trace_count++]=(struct sa_vertex_bus_event){kind,a,value,4u};else v->trace_dropped++;
}
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *value){
 struct sa_transfer_context_memory *m=ctx;
 if(!reached(a))return m->hardware_bus.read(m->hardware_bus.context,a,n,value);
 if(n!=4u||(a&3u)||!value)return -3;
 *value=m->transfer_registers[((a&0x1fffffffu)-0x005f7800u)/4u];event(m,25u,a,*value);return 0;
}
static int write(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct sa_transfer_context_memory *m=ctx;
 if(!reached(a))return m->hardware_bus.write(m->hardware_bus.context,a,value,n);
 if(n!=4u||(a&3u))return -3;
 m->transfer_registers[((a&0x1fffffffu)-0x005f7800u)/4u]=value;event(m,26u,a,value);return 0;
}
static int cache(void *ctx,uint32_t a,unsigned op){
 struct sa_transfer_context_memory *m=ctx;return m->hardware_bus.cache(m->hardware_bus.context,a,op);
}
void sa_transfer_context_bus_init(struct sa_transfer_context_memory *m,uint8_t *ram,struct sa_vertex_bus *bus,uint32_t port,uint32_t control){
 sa_hardware_transfer_bus_init(&m->hardware,ram,&m->hardware_bus,port,control);
 for(unsigned i=0;i<SA_TRANSFER_CONTEXT_WORDS;i++)m->transfer_registers[i]=0;
 bus->context=m;bus->read=read;bus->write=write;bus->cache=cache;
}

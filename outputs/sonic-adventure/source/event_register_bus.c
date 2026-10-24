#include "event_register_bus.h"
static int mask_index(uint32_t a,unsigned n){
 uint32_t p=a&0x1fffffffu;
 return n==4u && !(a&3u) && (p==0x005f6920u||p==0x005f6924u)?(int)((p-0x005f6920u)/4u):-1;
}
static void event(struct sa_event_memory *m,uint32_t kind,uint32_t a,uint32_t value){
 struct sa_vertex_memory *v=&m->interrupt.submit.frame.graphics.vertex;
 if(v->trace_count<4096u)v->trace[v->trace_count++]=(struct sa_vertex_bus_event){kind,a,value,4u};
 else v->trace_dropped++;
}
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *value){
 struct sa_event_memory *m=ctx;int i=mask_index(a,n);
 if(i<0)return m->interrupt_bus.read(m->interrupt_bus.context,a,n,value);
 *value=m->masks[i];event(m,17u,a,*value);return 0;
}
static int write(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct sa_event_memory *m=ctx;int i=mask_index(a,n);
 if(i<0)return m->interrupt_bus.write(m->interrupt_bus.context,a,value,n);
 m->masks[i]=value;event(m,16u,a,value);return 0;
}
static int cache(void *ctx,uint32_t a,unsigned op){
 struct sa_event_memory *m=ctx;return m->interrupt_bus.cache(m->interrupt_bus.context,a,op);
}
void sa_event_bus_init(struct sa_event_memory *m,uint8_t *ram,struct sa_vertex_bus *bus){
 sa_interrupt_bus_init(&m->interrupt,ram,&m->interrupt_bus);m->masks[0]=m->masks[1]=0;
 bus->context=m;bus->read=read;bus->write=write;bus->cache=cache;
}

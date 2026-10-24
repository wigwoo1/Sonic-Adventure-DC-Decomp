#include "submit_register_bus.h"
static int index(uint32_t a,unsigned n){
 if(n!=4u || a&3u)return -1;
 if(a>=0xffa00020u && a<=0xffa0002cu)return (int)((a-0xffa00020u)/4u);
 if(a>=0xa05f6800u && a<=0xa05f6808u)return 4+(int)((a-0xa05f6800u)/4u);
 return -1;
}
static void event(struct sa_vertex_memory *v,uint32_t kind,uint32_t a,uint32_t value){
 if(v->trace_count<4096u)v->trace[v->trace_count++]=(struct sa_vertex_bus_event){kind,a,value,4u};
 else v->trace_dropped++;
}
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *value){
 struct sa_submit_register_memory *m=ctx;int i=index(a,n);
 if(i<0)return m->frame_bus.read(m->frame_bus.context,a,n,value);
 *value=m->dma_registers[i];event(&m->frame.graphics.vertex,11u,a,*value);return 0;
}
static int write(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct sa_submit_register_memory *m=ctx;int i=index(a,n);
 if(i<0)return m->frame_bus.write(m->frame_bus.context,a,value,n);
 m->dma_registers[i]=value;event(&m->frame.graphics.vertex,10u,a,value);return 0;
}
static int cache(void *ctx,uint32_t a,unsigned op){
 struct sa_submit_register_memory *m=ctx;return m->frame_bus.cache(m->frame_bus.context,a,op);
}
void sa_submit_register_bus_init(struct sa_submit_register_memory *m,uint8_t *ram,struct sa_vertex_bus *bus){
 sa_frame_command_bus_init(&m->frame,ram,&m->frame_bus);
 for(unsigned i=0;i<7u;i++)m->dma_registers[i]=0;
 bus->context=m;bus->read=read;bus->write=write;bus->cache=cache;
}

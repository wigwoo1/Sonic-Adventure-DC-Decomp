#include "interrupt_bus.h"
static int vram_index(uint32_t a,unsigned n){
 uint32_t p=a&0x1fffffffu;
 if(n!=4u || a&3u || p<0x05000000u || p>=0x05800000u)return -1;
 return (int)((p-0x05000000u)/4u);
}
static void event(struct sa_vertex_memory *v,uint32_t kind,uint32_t a,uint32_t value){
 if(v->trace_count<4096u)v->trace[v->trace_count++]=(struct sa_vertex_bus_event){kind,a,value,4u};
 else v->trace_dropped++;
}
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *value){
 struct sa_interrupt_memory *m=ctx;int i=vram_index(a,n);
 if(n==4u && (a&0x1fffffffu)==0x005f6900u){
  *value=m->normal_status;event(&m->submit.frame.graphics.vertex,13u,a,*value);return 0;
 }
 if(i>=0){*value=m->vram[i];event(&m->submit.frame.graphics.vertex,15u,a,*value);return 0;}
 return m->submit_bus.read(m->submit_bus.context,a,n,value);
}
static int write(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct sa_interrupt_memory *m=ctx;int i=vram_index(a,n);
 if(n==4u && (a&0x1fffffffu)==0x005f6900u){
  event(&m->submit.frame.graphics.vertex,12u,a,value);m->normal_status&=~value;return 0;
 }
 if(i>=0){m->vram[i]=value;event(&m->submit.frame.graphics.vertex,14u,a,value);return 0;}
 return m->submit_bus.write(m->submit_bus.context,a,value,n);
}
static int cache(void *ctx,uint32_t a,unsigned op){
 struct sa_interrupt_memory *m=ctx;return m->submit_bus.cache(m->submit_bus.context,a,op);
}
void sa_interrupt_bus_init(struct sa_interrupt_memory *m,uint8_t *ram,struct sa_vertex_bus *bus){
 sa_submit_register_bus_init(&m->submit,ram,&m->submit_bus);m->normal_status=0;
 volatile uint32_t *vram=m->vram;
 for(unsigned i=0;i<SA_INTERRUPT_VRAM_WORDS;i++)vram[i]=0;
 bus->context=m;bus->read=read;bus->write=write;bus->cache=cache;
}

#include "render_register_bus.h"
static int register_address(uint32_t a,unsigned n){
 uint32_t physical=a&0x1fffffffu;
 return n==4u && !(a&3u) && physical>=0x005f8000u && physical<0x005fa000u;
}
static int register_event(struct sa_render_register_memory *m,uint32_t kind,uint32_t a,uint32_t value){
 struct sa_vertex_memory *v=&m->vertex;
 if(v->trace_count>=4096u){v->trace_dropped++;return 0;}
 v->trace[v->trace_count++]=(struct sa_vertex_bus_event){kind,a,value,4u};return 0;
}
static int read_register(void *ctx,uint32_t a,unsigned n,uint32_t *value){
 struct sa_render_register_memory *m=ctx;
 if(!register_address(a,n))return m->vertex_bus.read(m->vertex_bus.context,a,n,value);
 *value=m->registers[((a&0x1fffffffu)-0x005f8000u)/4u];
 return register_event(m,8u,a,*value);
}
static int write_register(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct sa_render_register_memory *m=ctx;
 if(!register_address(a,n))return m->vertex_bus.write(m->vertex_bus.context,a,value,n);
 int status=register_event(m,7u,a,value);if(status)return status;
 m->registers[((a&0x1fffffffu)-0x005f8000u)/4u]=value;return 0;
}
static int cache_register(void *ctx,uint32_t a,unsigned op){
 struct sa_render_register_memory *m=ctx;return m->vertex_bus.cache(m->vertex_bus.context,a,op);
}
void sa_render_register_bus_init(struct sa_render_register_memory *m,uint8_t *ram,struct sa_vertex_bus *bus){
 sa_vertex_bus_init(&m->vertex,ram,&m->vertex_bus);
 volatile uint32_t *registers=m->registers;
 for(unsigned i=0;i<SA_RENDER_REGISTER_WORDS;i++)registers[i]=0;
 bus->context=m;bus->read=read_register;bus->write=write_register;bus->cache=cache_register;
}

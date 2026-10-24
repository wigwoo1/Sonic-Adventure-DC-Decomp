#include "cache_register_bus.h"
/* Explicit coherent-RAM platform policy. CCR invalidation commands read zero;
 * other bits are stored, including unspecified/reserved input bits. No dirty
 * cache lines, MMU address translation, physical timing or fault delivery.
 * Reference: ST SH-4 Core Architecture, Table 18, PDF pages 77-79. */
static void event(struct sa_cache_register_memory *m,uint32_t kind,uint32_t value){
 struct sa_vertex_memory *v=&m->event.interrupt.submit.frame.graphics.vertex;
 if(v->trace_count<4096u)v->trace[v->trace_count++]=(struct sa_vertex_bus_event){kind,0xff00001cu,value,4u};
 else v->trace_dropped++;
}
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *value){
 struct sa_cache_register_memory *m=ctx;
 if(a!=0xff00001cu)return m->event_bus.read(m->event_bus.context,a,n,value);
 if(n!=4u||!value)return -3;
 *value=m->ccr&~0x808u;event(m,20u,*value);return 0;
}
static int write(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct sa_cache_register_memory *m=ctx;
 if(a!=0xff00001cu)return m->event_bus.write(m->event_bus.context,a,value,n);
 if(n!=4u)return -3;
 event(m,19u,value);m->ccr=value&~0x808u;
 if(value&0x800u)m->ic_requests++;
 if(value&8u)m->oc_requests++;
 return 0;
}
static int cache(void *ctx,uint32_t a,unsigned op){
 struct sa_cache_register_memory *m=ctx;return m->event_bus.cache(m->event_bus.context,a,op);
}
void sa_cache_register_bus_init(struct sa_cache_register_memory *m,uint8_t *ram,struct sa_vertex_bus *bus){
 sa_event_bus_init(&m->event,ram,&m->event_bus);m->ccr=0;m->ic_requests=0;m->oc_requests=0;
 bus->context=m;bus->read=read;bus->write=write;bus->cache=cache;
}

#include "video_signal_bus.h"
/* Reached word-only, read-only PDTRA interface. The host supplies all 16 bits.
 * Original game code extracts bits 8/9 itself. No callback result, PCTRA
 * configuration, electrical behavior or automatic cable detection is supplied. */
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *value){
 struct sa_video_signal_memory *m=ctx;
 if(a!=0xff800030u)return m->cache_bus.read(m->cache_bus.context,a,n,value);
 if(n!=2u||!value)return -3;
 *value=m->port_data&0xffffu;
 struct sa_vertex_memory *v=&m->cache.event.interrupt.submit.frame.graphics.vertex;
 if(v->trace_count<4096u)v->trace[v->trace_count++]=(struct sa_vertex_bus_event){21u,a,*value,2u};
 else v->trace_dropped++;
 return 0;
}
static int write(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct sa_video_signal_memory *m=ctx;
 if(a==0xff800030u)return -3;
 return m->cache_bus.write(m->cache_bus.context,a,value,n);
}
static int cache(void *ctx,uint32_t a,unsigned op){
 struct sa_video_signal_memory *m=ctx;return m->cache_bus.cache(m->cache_bus.context,a,op);
}
void sa_video_signal_bus_init(struct sa_video_signal_memory *m,uint8_t *ram,struct sa_vertex_bus *bus,uint32_t sample){
 sa_cache_register_bus_init(&m->cache,ram,&m->cache_bus);m->port_data=sample&0xffffu;
 bus->context=m;bus->read=read;bus->write=write;bus->cache=cache;
}

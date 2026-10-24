#include "display_timing_bus.h"
static int timing_address(uint32_t a){return (a&0x1fffffffu)==0x005f810cu;}
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *value){
 struct sa_display_timing_memory *m=ctx;
 if(!timing_address(a))return m->video_bus.read(m->video_bus.context,a,n,value);
 if(n!=4u||!value)return -3;
 if(!m->samples||m->sample_cursor>=m->sample_count)return -4;
 *value=m->samples[m->sample_cursor++];
 struct sa_render_register_memory *render=&m->video.cache.event.interrupt.submit.frame.graphics;
 render->registers[0x10cu/4u]=*value;
 struct sa_vertex_memory *v=&render->vertex;
 if(v->trace_count<4096u)v->trace[v->trace_count++]=(struct sa_vertex_bus_event){8u,a,*value,4u};
 else v->trace_dropped++;
 return 0;
}
static int write(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct sa_display_timing_memory *m=ctx;
 if(timing_address(a))return -3;
 return m->video_bus.write(m->video_bus.context,a,value,n);
}
static int cache(void *ctx,uint32_t a,unsigned op){
 struct sa_display_timing_memory *m=ctx;return m->video_bus.cache(m->video_bus.context,a,op);
}
void sa_display_timing_bus_init(struct sa_display_timing_memory *m,uint8_t *ram,struct sa_vertex_bus *bus,uint32_t port){
 sa_video_signal_bus_init(&m->video,ram,&m->video_bus,port);
 m->samples=0;m->sample_count=0;m->sample_cursor=0;
 bus->context=m;bus->read=read;bus->write=write;bus->cache=cache;
}
int sa_display_timing_set_samples(struct sa_display_timing_memory *m,const uint32_t *samples,uint32_t count){
 if(!m||(!samples&&count))return -3;
 m->samples=samples;m->sample_count=count;m->sample_cursor=0;return 0;
}

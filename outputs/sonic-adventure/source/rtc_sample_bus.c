#include "rtc_sample_bus.h"
/* Explicit borrowed words in read order. No host clock, automatic advancement,
 * fabricated stable value or extrapolation after the finite input ends. */
static int rtc_address(uint32_t a){uint32_t p=a&0x1fffffffu;return p==0x00710000u||p==0x00710004u;}
static void event(struct sa_rtc_sample_memory *m,uint32_t a,uint32_t value,unsigned n){
 struct sa_vertex_memory *v=&m->driver.startup.context.hardware.timing.video.cache.event.interrupt.submit.frame.graphics.vertex;
 if(v->trace_count<4096u)v->trace[v->trace_count++]=(struct sa_vertex_bus_event){31u,a,value,n};else v->trace_dropped++;
}
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *value){
 struct sa_rtc_sample_memory *m=ctx;
 if(!rtc_address(a))return m->driver_bus.read(m->driver_bus.context,a,n,value);
 if(n!=4u||!value)return -3;
 if(!m->rtc_configured)return -2;
 if(m->rtc_cursor>=m->rtc_count)return -4;
 *value=m->rtc_samples[m->rtc_cursor++];event(m,a,*value,n);return 0;
}
static int write(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct sa_rtc_sample_memory *m=ctx;
 if(rtc_address(a))return -3;
 return m->driver_bus.write(m->driver_bus.context,a,value,n);
}
static int cache(void *ctx,uint32_t a,unsigned op){
 struct sa_rtc_sample_memory *m=ctx;return m->driver_bus.cache(m->driver_bus.context,a,op);
}
void sa_rtc_sample_bus_init(struct sa_rtc_sample_memory *m,uint8_t *ram,struct sa_vertex_bus *bus,uint32_t port,uint32_t control){
 sa_driver_signal_bus_init(&m->driver,ram,&m->driver_bus,port,control);
 m->rtc_samples=0;m->rtc_count=0;m->rtc_cursor=0;m->rtc_configured=0;
 bus->context=m;bus->read=read;bus->write=write;bus->cache=cache;
}
int sa_rtc_set_samples(struct sa_rtc_sample_memory *m,const uint32_t *samples,uint32_t count){
 if(!m||(!samples&&count))return -3;
 m->rtc_samples=samples;m->rtc_count=count;m->rtc_cursor=0;m->rtc_configured=1;return 0;
}

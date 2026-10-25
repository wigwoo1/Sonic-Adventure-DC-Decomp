#include "sound_ram_bus.h"
static int inside(uint32_t a){uint32_t p=a&0x1fffffffu;return p>=0x00800000u&&p<0x00a00000u;}
static int valid(uint32_t a,unsigned n){uint32_t p=a&0x1fffffffu;return (n==1u||n==2u||n==4u)&&!(a&(n-1u))&&p>=0x00800000u&&p<=0x00a00000u-n;}
static void event(struct sa_sound_ram_memory *m,uint32_t kind,uint32_t a,uint32_t value,unsigned n){
 if(m->trace_count<m->trace_capacity)m->trace[m->trace_count++]=(struct sa_vertex_bus_event){kind,a,value,n};else m->trace_dropped++;
}
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *value){
 struct sa_sound_ram_memory *m=ctx;
 if(!inside(a))return m->parent.read(m->parent.context,a,n,value);
 if(!valid(a,n)||!value)return -3;
 if(!m->ram||m->bytes!=0x200000u)return -2;
 uint32_t offset=(a&0x1fffffffu)-0x00800000u,v=0;
 for(unsigned i=0;i<n;i++)v|=(uint32_t)m->ram[offset+i]<<(i*8u);
 *value=v;m->reads++;m->read_bytes+=n;event(m,32u,a,v,n);return 0;
}
static int write(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct sa_sound_ram_memory *m=ctx;
 if(!inside(a))return m->parent.write(m->parent.context,a,value,n);
 if(!valid(a,n))return -3;
 if(!m->ram||m->bytes!=0x200000u)return -2;
 uint32_t offset=(a&0x1fffffffu)-0x00800000u;
 if(n<4u)value&=(1u<<(n*8u))-1u;
 for(unsigned i=0;i<n;i++)m->ram[offset+i]=(uint8_t)(value>>(i*8u));
 m->writes++;m->write_bytes+=n;event(m,33u,a,value,n);return 0;
}
static int cache(void *ctx,uint32_t a,unsigned op){struct sa_sound_ram_memory *m=ctx;if(!m->parent.cache)return -2;return m->parent.cache(m->parent.context,a,op);}
int sa_sound_ram_bus_init(struct sa_sound_ram_memory *m,const struct sa_vertex_bus *parent,uint8_t *ram,uint32_t bytes,struct sa_vertex_bus_event *trace,uint32_t capacity,struct sa_vertex_bus *bus){
 if(!m||!parent||!bus||!parent->read||!parent->write||(!ram&&bytes)||(ram&&bytes!=0x200000u)||(!trace&&capacity))return -3;
 m->parent=*parent;m->ram=ram;m->bytes=bytes;m->trace=trace;m->trace_capacity=capacity;m->trace_count=0;m->trace_dropped=0;m->reads=0;m->writes=0;m->read_bytes=0;m->write_bytes=0;
 bus->context=m;bus->read=read;bus->write=write;bus->cache=cache;return 0;
}

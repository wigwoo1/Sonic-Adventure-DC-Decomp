#include "vram_aperture_bus.h"
/* The 64-bit aperture interleaves the two 4 MiB banks one word at a time.
 * Decode a lane and word number independently, then address the bank-linear
 * parent. This does not change any earlier bus implementation or layout. */
static int aperture(uint32_t address) {
 uint32_t physical=address&0x1fffffffu;
 return physical>=0x04000000u && physical<0x04800000u;
}
static uint32_t mapped(uint32_t address) {
 uint32_t word=(address&0x007fffffu)/4u;
 uint32_t bank=word%2u,within=word/2u;
 return 0xa5000000u+bank*0x00400000u+within*4u;
}
static void record(struct sa_vram_aperture_bus *s,uint32_t a,uint32_t m,uint32_t value,uint32_t op) {
 uint32_t words[4]={a,m,value,op};
 for(unsigned i=0;i<4;i++)s->signature=(s->signature^words[i])*16777619u;
 if(s->count<s->capacity)s->trace[s->count++]=(struct sa_vram_aperture_event){a,m,value,op};
 else s->dropped++;
}
static int read(void *context,uint32_t a,unsigned n,uint32_t *value) {
 struct sa_vram_aperture_bus *s=context;
 if(!aperture(a))return s->parent.read?s->parent.read(s->parent.context,a,n,value):-2;
 s->attempts++;
 if(n!=4u || (a&3u) || !value)return -3;
 if(!s->parent.read)return -2;
 uint32_t target=mapped(a);int status=s->parent.read(s->parent.context,target,n,value);
 if(!status)record(s,a,target,*value,1);
 return status;
}
static int write(void *context,uint32_t a,uint32_t value,unsigned n) {
 struct sa_vram_aperture_bus *s=context;
 if(!aperture(a))return s->parent.write?s->parent.write(s->parent.context,a,value,n):-2;
 s->attempts++;
 if(n!=4u || (a&3u))return -3;
 if(!s->parent.write)return -2;
 uint32_t target=mapped(a);int status=s->parent.write(s->parent.context,target,value,n);
 if(!status)record(s,a,target,value,2);
 return status;
}
static int cache(void *context,uint32_t a,unsigned operation) {
 struct sa_vram_aperture_bus *s=context;
 return s->parent.cache?s->parent.cache(s->parent.context,a,operation):-2;
}
int sa_vram_aperture_bus_init(struct sa_vram_aperture_bus *s,const struct sa_vertex_bus *parent,
 struct sa_vertex_bus *out,struct sa_vram_aperture_event *trace,uint32_t capacity) {
 if(!s || !parent || !out || (capacity && !trace))return -3;
 s->parent=*parent;s->trace=trace;s->capacity=capacity;s->count=0;s->dropped=0;s->attempts=0;s->signature=2166136261u;
 out->context=s;out->read=read;out->write=write;out->cache=cache;
 return 0;
}

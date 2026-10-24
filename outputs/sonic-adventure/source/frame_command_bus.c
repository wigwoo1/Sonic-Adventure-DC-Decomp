#include "frame_command_bus.h"
static void event(struct sa_vertex_memory *v,uint32_t kind,uint32_t a,uint32_t value,unsigned n){
 if(v->trace_count<4096u)v->trace[v->trace_count++]=(struct sa_vertex_bus_event){kind,a,value,n};
 else v->trace_dropped++;
}
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *value){
 struct sa_frame_command_memory *m=ctx;
 return m->graphics_bus.read(m->graphics_bus.context,a,n,value);
}
static int write(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct sa_frame_command_memory *m=ctx;
 return m->graphics_bus.write(m->graphics_bus.context,a,value,n);
}
static int cache(void *ctx,uint32_t a,unsigned op){
 struct sa_frame_command_memory *m=ctx;struct sa_vertex_memory *v=&m->graphics.vertex;
 if(op==1u && (a&0xfc000000u)==0xe0000000u){
  unsigned q=(a>>5)&1u;
  uint32_t destination=(a&0x03ffffe0u)|((v->qacr[q]&0x1cu)<<24);
  /* Only the polygon command window is captured. Other targets retain the
   * earlier backend's explicit status; no texture/YUV/VRAM alias is inferred. */
  if(destination>=0x10000000u && destination<0x10800000u){
   event(v,3u,a,0,0);v->prefetches++;
   if(m->packet_count>=m->packet_capacity || m->packet_count>=SA_FRAME_COMMAND_CAPACITY)return -4;
   struct sa_frame_command_packet *p=&m->packets[m->packet_count];
   p->destination=destination;p->queue=q;
   for(unsigned i=0;i<8u;i++)p->words[i]=v->queue[q*8u+i];
   event(v,9u,destination,q,32u);m->packet_count++;v->flushes++;return 0;
  }
 }
 return m->graphics_bus.cache(m->graphics_bus.context,a,op);
}
void sa_frame_command_bus_init(struct sa_frame_command_memory *m,uint8_t *ram,struct sa_vertex_bus *bus){
 sa_render_register_bus_init(&m->graphics,ram,&m->graphics_bus);
 m->packet_count=0;m->packet_capacity=SA_FRAME_COMMAND_CAPACITY;
 for(unsigned i=0;i<SA_FRAME_COMMAND_CAPACITY;i++){
  m->packets[i].destination=0;m->packets[i].queue=0;
  for(unsigned j=0;j<8u;j++)m->packets[i].words[j]=0;
 }
 bus->context=m;bus->read=read;bus->write=write;bus->cache=cache;
}

#include "vertex_bus.h"
static int event(struct sa_vertex_memory *m,uint32_t kind,uint32_t a,uint32_t v,unsigned n){
 if(m->trace_count>=4096u){m->trace_dropped++;return 0;}
 m->trace[m->trace_count++]=(struct sa_vertex_bus_event){kind,a,v,n};return 0;
}
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *v){
 struct sa_vertex_memory *m=ctx;if(n!=4u)return -3;
 if(a==0xff000038u||a==0xff00003cu)*v=m->qacr[(a>>2)&1u];
 else return -3;
 return event(m,0u,a,*v,n);
}
static int write(void *ctx,uint32_t a,uint32_t v,unsigned n){
 struct sa_vertex_memory *m=ctx;if(n!=4u||a&3u)return -3;
 int status;
 if(a==0xff000038u||a==0xff00003cu){
  status=event(m,1u,a,v,n);if(status)return status;m->qacr[(a>>2)&1u]=v;return 0;
 }
 if((a&0xfc000000u)==0xe0000000u){
  status=event(m,2u,a,v,n);if(status)return status;m->queue[(a&63u)>>2]=v;return 0;
 }
 return -3;
}
static int cache(void *ctx,uint32_t a,unsigned op){
 struct sa_vertex_memory *m=ctx;int status;
 if(op==0u){status=event(m,4u,a,0,0);if(!status)m->invalidates++;return status;}
 /* OCBWB is observable but already coherent in this bounded RAM backend. */
 if(op==2u){if((a&0x1f000000u)!=0x0c000000u)return -3;return event(m,6u,a,0,0);}
 /* Explicit coherent-RAM OCBP policy; event 18 distinguishes purge. */
 if(op==3u){uint32_t region=a&0xe0000000u;if((region!=0u&&region!=0x80000000u&&region!=0xa0000000u)||(a&0x1f000000u)!=0x0c000000u)return -3;status=event(m,18u,a,0,0);if(!status)m->invalidates++;return status;}
 if(op!=1u)return -3;
 status=event(m,3u,a,0,0);if(status)return status;m->prefetches++;
 if((a&0xfc000000u)!=0xe0000000u)return 0;
 unsigned q=(a>>5)&1u;uint32_t destination=(a&0x03ffffe0u)|((m->qacr[q]&0x1cu)<<24);
 if((destination&0x1f000000u)!=0x0c000000u)return -3;
 status=event(m,5u,destination,q,32u);if(status)return status;
 for(unsigned i=0;i<8u;i++)for(unsigned b=0;b<4u;b++)m->ram[(destination&0xffffffu)+i*4u+b]=(uint8_t)(m->queue[q*8u+i]>>(b*8u));
 m->flushes++;return 0;
}
void sa_vertex_bus_init(struct sa_vertex_memory *memory,uint8_t *ram,struct sa_vertex_bus *bus){
 for(unsigned i=0;i<2u;i++)memory->qacr[i]=0;
 for(unsigned i=0;i<16u;i++)memory->queue[i]=0;
 memory->invalidates=0;memory->prefetches=0;memory->flushes=0;memory->trace_count=0;memory->trace_dropped=0;
 memory->ram=ram;bus->context=memory;bus->read=read;bus->write=write;bus->cache=cache;
}

#include "pvr_dma_bus.h"
static const uint32_t address[10]={0xffa00020u,0xffa00024u,0xffa00028u,0xffa0002cu,
 0xffa00040u,0xa05f6800u,0xa05f6804u,0xa05f6808u,0xa05f6884u,0xa05f6888u};
static int index(uint32_t a) {
 for(unsigned i=0;i<10;i++)if(a==address[i])return (int)i;
 return -1;
}
static int parent_write(struct sa_pvr_dma_bus *s,unsigned i,uint32_t value) {
 if(!s->parent.write)return -2;
 int status=s->parent.write(s->parent.context,address[i],value,4);
 if(!status)s->registers[i]=value;
 return status;
}
static int execute(struct sa_pvr_dma_bus *s) {
 if(s->active)return -4;
 s->active=1;s->started++;s->progress=0;s->status=0;
 uint32_t source=s->registers[0]&0x1fffffe0u,raw=s->registers[5];
 unsigned mode_index=(raw&0x02000000u)?9u:8u;
 uint32_t mode=s->registers[mode_index],length=s->registers[6],offset=raw&0x007fffe0u;
 s->source=source;s->length=length;
 s->destination=(mode?0xa5000000u:0xa4000000u)+offset;
 if(!s->enabled)return 0;
 if((s->registers[4]&0xffff8201u)!=0x8201u || mode>1u ||
    (raw&0xfc000000u)!=0x10000000u || !(raw&0x01000000u) ||
    source<0x0c000000u || source>0x0d000000u || length>0x0d000000u-source ||
    (length&31u) || length>0x00800000u-offset)return s->status=-2;
 if(!s->parent.write)return s->status=-2;
 for(uint32_t pos=0;pos<length;pos+=4u) {
  uint32_t at=source-0x0c000000u+pos,value=0;
  for(unsigned byte=0;byte<4;byte++)value|=(uint32_t)s->ram[at+byte]<<(byte*8u);
  int status=s->parent.write(s->parent.context,s->destination+pos,value,4);
  if(status)return s->status=status;
  uint32_t words[3]={source+pos,s->destination+pos,value};
  for(unsigned i=0;i<3;i++)s->signature=(s->signature^words[i])*16777619u;
  s->progress=pos+4u;
 }
 /* These are device-register effects after the final accepted data word.
  * The original interrupt handler, not this service, retires game work. */
 const unsigned fields[5]={3,2,5,6,7};
 uint32_t values[5]={s->registers[3]|2u,0,s->destination+length,0,0};
 for(unsigned i=0;i<5;i++) {
  int status=parent_write(s,fields[i],values[i]);if(status)return s->status=status;
 }
 s->active=0;s->completed++;s->pending|=1u<<19;
 if(s->interrupt_pending)*s->interrupt_pending|=1u<<19;
 return 0;
}
static void error_event(struct sa_pvr_dma_bus *s,uint32_t a,uint32_t value,uint32_t op) {
 uint32_t words[3]={a,value,op};
 for(unsigned i=0;i<3;i++)s->error_signature=(s->error_signature^words[i])*16777619u;
 if(s->error_count<256u){for(unsigned i=0;i<3;i++)s->error_trace[s->error_count][i]=words[i];s->error_count++;}
 else s->error_dropped++;
}
static int read(void *context,uint32_t a,unsigned n,uint32_t *value) {
 struct sa_pvr_dma_bus *s=context;if(!s->parent.read)return -2;
 if((a&0x1fffffffu)>=0x005f6908u && (a&0x1fffffffu)<0x005f690cu) {
  if(n!=4u || (a&3u) || !value)return -3;
  *value=s->error_status;s->error_reads++;error_event(s,a,*value,1u);return 0;
 }
 int status=s->parent.read(s->parent.context,a,n,value);
 if(!status && n==4u && (a&0x1fffffffu)==0x005f6900u)*value|=s->pending;
 return status;
}
static int write(void *context,uint32_t a,uint32_t value,unsigned n) {
 struct sa_pvr_dma_bus *s=context;if(!s->parent.write)return -2;
 if((a&0x1fffffffu)>=0x005f6908u && (a&0x1fffffffu)<0x005f690cu) {
  if(n!=4u || (a&3u))return -3;
  s->error_status&=~value;s->error_writes++;error_event(s,a,value,2u);return 0;
 }
 int status=s->parent.write(s->parent.context,a,value,n);if(status)return status;
 if(n==4u && (a&0x1fffffffu)==0x005f6900u) {
  s->pending&=~value;if(s->interrupt_pending)*s->interrupt_pending&=~value;
 }
 int slot=index(a);
 if(slot<0 || n!=4u)return 0;
 s->registers[slot]=value;
 if(slot==7) {
  if(value&1u)return execute(s);
  s->active=0;
 }
 return 0;
}
static int cache(void *context,uint32_t a,unsigned op) {
 struct sa_pvr_dma_bus *s=context;
 return s->parent.cache?s->parent.cache(s->parent.context,a,op):-2;
}
int sa_pvr_dma_bus_init(struct sa_pvr_dma_bus *s,uint8_t *ram,const struct sa_vertex_bus *parent,
 struct sa_vertex_bus *out,uint32_t *pending,const uint32_t *seed,uint32_t enabled) {
 if(!s || !ram || !parent || !out)return -3;
 s->parent=*parent;s->ram=ram;s->interrupt_pending=pending;
 for(unsigned i=0;i<10;i++)s->registers[i]=seed?seed[i]:0;
 s->source=0;s->destination=0;s->length=0;s->progress=0;s->started=0;s->completed=0;
 s->pending=0;s->signature=2166136261u;s->status=0;s->enabled=!!enabled;s->active=0;
 s->error_status=0;s->error_reads=0;s->error_writes=0;s->error_signature=2166136261u;s->error_count=0;s->error_dropped=0;
 out->context=s;out->read=read;out->write=write;out->cache=cache;return 0;
}

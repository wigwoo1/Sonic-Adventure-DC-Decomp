#include "ta_stream.h"
struct page {struct page *next;uint32_t used;struct sa_frame_command_packet data[SA_FRAME_COMMAND_CAPACITY];};
struct store {struct page *first,*last;uint32_t count,pages;};
struct sa_ta_stream {
 sa_ta_allocate allocate;sa_ta_release release;void *allocator;
 struct store active,retired,submitted;
 struct sa_vertex_memory *vertex;struct sa_vertex_bus parent;uint32_t *pending;
 uint32_t events[9]; /* Same cursor/pending/list/active/counter profile as before. */
 uint64_t lifecycle[8]; /* inits,retired,discarded,resets,OPB,ISP,last_count,enabled */
 uint32_t submitted_cursor,partial_lane,partial_valid;
 struct sa_frame_command_packet partial;
};
static void clear(void *out,size_t size){volatile uint8_t *p=out;while(size--)*p++=0;}
static const struct store *select_store(const struct sa_ta_stream *s,unsigned which){return which==0?&s->active:which==1?&s->retired:which==2?&s->submitted:0;}
static void free_store(struct sa_ta_stream *s,struct store *v){
 struct page *p=v->first;while(p){struct page *next=p->next;s->release(s->allocator,p);p=next;}clear(v,sizeof(*v));
}
static int reserve(struct sa_ta_stream *s,struct store *v){
 if(v->count==UINT32_MAX)return -6;
 if(v->last&&v->last->used<SA_FRAME_COMMAND_CAPACITY)return 0;
 struct page *p=s->allocate(s->allocator,sizeof(*p));if(!p)return -4;
 clear(p,sizeof(*p));if(v->last)v->last->next=p;else v->first=p;v->last=p;v->pages++;return 0;
}
static int append(struct sa_ta_stream *s,struct store *v,const struct sa_frame_command_packet *packet){
 int rc=reserve(s,v);if(rc)return rc;v->last->data[v->last->used++]=*packet;v->count++;return 0;
}
static const struct sa_frame_command_packet *at(const struct store *v,uint32_t index){
 if(index>=v->count)return 0;const struct page *p=v->first;
 while(p&&index>=p->used){index-=p->used;p=p->next;}return p?&p->data[index]:0;
}
SA_STAGE_EXPORT struct sa_ta_stream *sa_ta_stream_create(sa_ta_allocate allocate,sa_ta_release release,void *context){
 if(!allocate||!release)return 0;struct sa_ta_stream *s=allocate(context,sizeof(*s));if(!s)return 0;
 clear(s,sizeof(*s));s->allocate=allocate;s->release=release;s->allocator=context;return s;
}
SA_STAGE_EXPORT void sa_ta_stream_destroy(struct sa_ta_stream *s){
 if(!s)return;free_store(s,&s->active);free_store(s,&s->retired);free_store(s,&s->submitted);s->release(s->allocator,s);
}
SA_STAGE_EXPORT int sa_ta_stream_append(struct sa_ta_stream *s,const struct sa_frame_command_packet *packet){return s&&packet?append(s,&s->active,packet):-6;}
SA_STAGE_EXPORT int sa_ta_stream_snapshot(struct sa_ta_stream *s){
 if(!s||s->partial_valid)return -6;struct store copy={0};
 for(const struct page *p=s->active.first;p;p=p->next)for(uint32_t i=0;i<p->used;i++){
  int rc=append(s,&copy,&p->data[i]);if(rc){free_store(s,&copy);return rc;}
 }
 free_store(s,&s->submitted);s->submitted=copy;s->submitted_cursor=s->active.count;return 0;
}
SA_STAGE_EXPORT uint32_t sa_ta_stream_count(const struct sa_ta_stream *s,unsigned which){const struct store *v=s?select_store(s,which):0;return v?v->count:0;}
SA_STAGE_EXPORT uint32_t sa_ta_stream_pages(const struct sa_ta_stream *s,unsigned which){const struct store *v=s?select_store(s,which):0;return v?v->pages:0;}
SA_STAGE_EXPORT int sa_ta_stream_packet(const struct sa_ta_stream *s,unsigned which,uint32_t index,struct sa_frame_command_packet *out){
 const struct store *v=s?select_store(s,which):0;const struct sa_frame_command_packet *p=v?at(v,index):0;if(!p||!out)return -6;*out=*p;return 0;
}
SA_STAGE_EXPORT int sa_ta_stream_events(const struct sa_ta_stream *s,uint32_t *out){if(!s||!out)return -6;for(unsigned i=0;i<9;i++)out[i]=s->events[i];return 0;}
SA_STAGE_EXPORT int sa_ta_stream_lifecycle(const struct sa_ta_stream *s,uint64_t *out){if(!s||!out)return -6;for(unsigned i=0;i<8;i++)out[i]=s->lifecycle[i];return 0;}
static void event(struct sa_ta_stream *s,uint32_t kind,uint32_t a,uint32_t value,unsigned width){
 struct sa_vertex_memory *v=s->vertex;if(v->trace_count<4096u)v->trace[v->trace_count++]=(struct sa_vertex_bus_event){kind,a,value,width};else v->trace_dropped++;
}
static int consume(struct sa_ta_stream *s){
 static const uint32_t bits[5]={128,256,512,1024,2097152};uint32_t *e=s->events;
 if(!e[8])return 0;if(e[0]>s->active.count)return -6;
 while(e[0]<s->active.count){
  const struct sa_frame_command_packet *p=at(&s->active,e[0]);uint32_t word=p->words[0],kind=word>>29;
  if(kind==4u||kind==5u){uint32_t list=(word>>24)&7u;
   if(list>4u||(kind==4u&&(word&0xd0u)&&((word&0xfdu)!=0x10u))){e[7]++;return -2;}
   e[2]=list;e[3]=(kind==4u&&!(list&1u))?1u:2u;e[4]++;
  }else if(kind==7u){if(e[3]!=1u){e[7]++;return -2;}e[5]++;
  }else if(kind==0u){if(word||!e[3]||e[2]>4u){e[7]++;return -2;}e[1]|=bits[e[2]];if(s->pending)*s->pending|=bits[e[2]];e[3]=0;e[6]++;
  }else if(kind!=1u){e[7]++;return -2;}
  e[0]++;
 }
 return 0;
}
static int read_bus(void *context,uint32_t a,unsigned n,uint32_t *out){
 struct sa_ta_stream *s=context;if(!s->parent.read)return -2;int rc=s->parent.read(s->parent.context,a,n,out);
 if(!rc&&n==4u&&(a&0x1fffffffu)==0x5f6900u)*out|=s->events[1];return rc;
}
static int write_bus(void *context,uint32_t a,uint32_t value,unsigned n){
 struct sa_ta_stream *s=context;
 if(a>=0x10000000u&&a<0x10800000u){
  if(n!=4u||(a&3u))return -3;uint32_t lane=(a>>2)&7u,base=a&~31u;
  int rc=reserve(s,&s->active);if(rc)return rc;
  if(!lane){clear(&s->partial,sizeof(s->partial));s->partial.destination=base;s->partial.queue=2;s->partial_valid=1;s->partial_lane=0;}
  if(!s->partial_valid||s->partial.destination!=base||s->partial_lane!=lane)return -3;
  s->partial.words[lane]=value;s->partial_lane++;
  if(lane==7u){rc=append(s,&s->active,&s->partial);if(rc)return rc;s->partial_valid=0;s->partial_lane=0;event(s,9,base,2,32);return consume(s);}return 0;
 }
 uint32_t address=a&0x1fffffffu;uint64_t *l=s->lifecycle;int init=l[7]&&address==0x5f8144u&&(value&0x80000000u);
 if(l[7]){
  if((address==0x5f8008u||address==0x5f8124u||address==0x5f8128u||address==0x5f8144u)&&n!=4u)return -3;
  if(address==0x5f8008u&&(value&1u)&&l[3]==UINT64_MAX)return -6;
  if(init){uint32_t count=s->active.count,cursor=s->submitted_cursor;
   if(cursor>count||s->events[0]>count||l[0]==UINT64_MAX||l[1]>UINT64_MAX-count||l[2]>UINT64_MAX-(count-cursor))return -6;
  }
 }
 if(!s->parent.write)return -2;int rc=s->parent.write(s->parent.context,a,value,n);if(rc)return rc;
 if(n==4u&&address==0x5f6900u){s->events[1]&=~value;if(s->pending)*s->pending&=~value;}
 rc=consume(s);if(rc)return rc;
 if(init){
  if(s->events[8]&&s->events[0]!=s->active.count)return -6;
  uint32_t count=s->active.count;l[0]++;l[1]+=count;l[2]+=count-s->submitted_cursor;l[6]=count;
  free_store(s,&s->retired);s->retired=s->active;clear(&s->active,sizeof(s->active));s->submitted_cursor=0;s->partial_valid=s->partial_lane=0;
  s->events[0]=s->events[2]=s->events[3]=0;
 }
 if(l[7]){if(address==0x5f8124u)l[4]=value;if(address==0x5f8128u)l[5]=value;if(address==0x5f8008u&&(value&1u))l[3]++;}
 return 0;
}
static int cache_bus(void *context,uint32_t a,unsigned op){
 struct sa_ta_stream *s=context;struct sa_vertex_memory *v=s->vertex;
 if(op==1u&&(a&0xfc000000u)==0xe0000000u){
  uint32_t q=(a>>5)&1u,destination=(a&0x03ffffe0u)|((v->qacr[q]&0x1cu)<<24);
  if(destination>=0x10000000u&&destination<0x10800000u){
   event(s,3,a,0,0);v->prefetches++;struct sa_frame_command_packet p={0};p.destination=destination;p.queue=q;
   for(unsigned i=0;i<8;i++)p.words[i]=v->queue[q*8u+i];
   int rc=append(s,&s->active,&p);if(rc)return rc;event(s,9,destination,q,32);v->flushes++;return consume(s);
  }
 }
 if(!s->parent.cache)return -2;int rc=s->parent.cache(s->parent.context,a,op);return rc?rc:consume(s);
}
SA_STAGE_EXPORT int sa_ta_stream_bind(struct sa_ta_stream *s,struct sa_vertex_memory *vertex,const struct sa_vertex_bus *parent,struct sa_vertex_bus *out,uint32_t *pending,const uint32_t *events,const uint64_t *lifecycle){
 if(!s||!vertex||!parent||!out)return -6;s->vertex=vertex;s->parent=*parent;s->pending=pending;
 for(unsigned i=0;i<9;i++)s->events[i]=events?events[i]:0;if(!events)s->events[8]=1;
 for(unsigned i=0;i<8;i++)s->lifecycle[i]=lifecycle?lifecycle[i]:0;if(!lifecycle)s->lifecycle[7]=1;
 if(s->events[8]>1||s->lifecycle[7]>1)return -6;if(pending)*pending|=s->events[1];
 *out=(struct sa_vertex_bus){s,read_bus,write_bus,cache_bus};return consume(s);
}

/* Integration extensions; original page capture implementation above retained. */
#include "stream_pixel_jobs.h"
SA_STAGE_EXPORT int sa_ta_stream_restore_packet(struct sa_ta_stream *s,unsigned which,const struct sa_frame_command_packet *p){
 if(!s||!p||which<1u||which>2u)return -6;return append(s,which==1u?&s->retired:&s->submitted,p);
}
SA_STAGE_EXPORT int sa_ta_stream_set_cursor(struct sa_ta_stream *s,uint32_t cursor){if(!s||cursor>s->active.count)return -6;s->submitted_cursor=cursor;return 0;}
SA_STAGE_EXPORT int sa_ta_stream_clone_range(struct sa_ta_stream *s,uint32_t cursor,struct sa_ta_stream **out){
 if(!s||!out||s->partial_valid||cursor>s->active.count)return -6;
 struct sa_ta_stream *copy=sa_ta_stream_create(s->allocate,s->release,s->allocator);if(!copy)return -4;
 for(uint32_t i=cursor;i<s->active.count;i++){int rc=append(copy,&copy->active,at(&s->active,i));if(rc){sa_ta_stream_destroy(copy);return rc;}}
 *out=copy;return 0;
}
SA_STAGE_EXPORT int sa_ta_stream_flatten(struct sa_ta_stream *s,uint32_t **out,uint32_t *count){
 if(!s||!out||!count||s->partial_valid)return -6;
 if(s->active.count>UINT32_MAX/8u||(uint64_t)s->active.count>SIZE_MAX/32u)return -3;
 if(!s->active.count){*out=0;*count=0;return 0;}
 uint32_t *words=s->allocate(s->allocator,(size_t)s->active.count*32u);if(!words)return -4;
 uint32_t index=0;for(const struct page *p=s->active.first;p;p=p->next)for(uint32_t i=0;i<p->used;i++)for(unsigned k=0;k<8;k++)words[index++]=p->data[i].words[k];
 *out=words;*count=s->active.count;return 0;
}
SA_STAGE_EXPORT void sa_ta_stream_release_flat(struct sa_ta_stream *s,uint32_t *p){if(s&&p)s->release(s->allocator,p);}

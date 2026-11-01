/* Authored discrete-event timing support. No game flag or callback result.
 * Configuration is taken from actual guest display registers. Clock rates and
 * initial phase remain an explicit software profile, not board measurements.
 * Register facts: KallistiOS hardware/video.c and dc/asic.h; Flycast pvr_regs.h
 * at 7adbf3caef251b153e348df3f469a0f35878339d. No third-party code copied.
 * Time advances only through advance/next_in, never because a register is read.
 * Only scanline, field and vertical-window status bits are modeled; electrical
 * hsync/blank bits, capture and live mode changes remain outside this profile.
 */
#include "frame_clock.h"
static void copy_core(struct sa_frame_clock_core *out,const struct sa_frame_clock_core *in){
 volatile uint8_t *d=(volatile uint8_t *)out;const uint8_t *s=(const uint8_t *)in;
 for(unsigned i=0;i<sizeof(*out);i++)d[i]=s[i];
}
static int room(const struct sa_frame_clock_state *s,uint32_t n){return s&&s->trace_count<=SA_FRAME_CLOCK_TRACE_CAPACITY-n;}
static void record(struct sa_frame_clock_state *s,uint32_t kind,uint32_t a,uint32_t value,uint32_t width){s->trace[s->trace_count++]=(struct sa_vertex_bus_event){kind,a,value,width};}
static int timer_address(uint32_t a){return a==0xffd80000u||a==0xffd80004u||a==0xffd80008u||a==0xffd8000cu||a==0xffd80010u;}
static uint32_t *configuration(struct sa_frame_clock_core *c,uint32_t a){
 switch(a&0x1fffffffu){case 0x005f8044u:return &c->fb_control;case 0x005f80d0u:return &c->spg_control;case 0x005f80d8u:return &c->load;case 0x005f80ccu:return &c->vblank_int;case 0x005f80dcu:return &c->vblank;default:return 0;}
}
static int configure_check(struct sa_frame_clock_core *c,uint32_t a,uint32_t value,unsigned n){
 uint32_t *field=configuration(c,a);if(!field)return -2;if(n!=4u)return -3;
 uint32_t mask=(a&0x1fffffffu)==0x005f8044u?(1u<<23):(a&0x1fffffffu)==0x005f80d0u?0x30u:0xffffffffu;
 return ((*field^value)&mask)?-2:0;
}
static uint64_t hits(uint64_t lines,uint32_t from,uint32_t at,uint32_t total){
 if(at>=total)return 0;
 uint32_t distance=(at+total-from)%total;if(!distance)distance=total;
 return lines<distance?0:1u+(lines-distance)/total;
}
static uint32_t rate(const struct sa_frame_clock_core *s){
 uint32_t result=s->video_hz/((s->fb_control&(1u<<23))?1u:2u);
 return result*((s->spg_control&16u)?2u:1u);
}
SA_STAGE_EXPORT int sa_frame_clock_init(struct sa_frame_clock_state *s,uint32_t pclk,uint32_t video,uint32_t fb,uint32_t control,uint32_t load,uint32_t interrupts,uint32_t blank){
 if(!s||!pclk||pclk>1000000000u||!video||video>500000000u)return -6;
 if(control&32u)return -2;
 uint32_t total=((load>>16)&1023u)+1u;
 if((blank&1023u)>=total||((blank>>16)&1023u)>=total)return -2;
 if(!(fb&(1u<<23))&&(video&1u))return -2;
 volatile uint8_t *bytes=(volatile uint8_t *)s;for(unsigned i=0;i<sizeof(*s);i++)bytes[i]=0;
 sa_tmu0_init(&s->core.timer);s->core.pclk_hz=pclk;s->core.video_hz=video;s->core.fb_control=fb;s->core.spg_control=control;s->core.load=load;s->core.vblank_int=interrupts;s->core.vblank=blank;s->core.enabled=1;return 0;
}
SA_STAGE_EXPORT int sa_frame_clock_begin_entry(struct sa_frame_clock_state *s){
 if(!s)return -6;s->trace_count=0;
 volatile uint8_t *p=(volatile uint8_t *)s->trace;for(unsigned i=0;i<sizeof(s->trace);i++)p[i]=0;
 return 0;
}
SA_STAGE_EXPORT int sa_frame_clock_advance(struct sa_frame_clock_state *s,uint64_t cycles){
 if(!s)return -6;if(!s->core.enabled)return 0;if(!room(s,1))return -6;
 struct sa_frame_clock_core next;copy_core(&next,&s->core);
 uint32_t hz=rate(&next),pclk=next.pclk_hz;
 if(!pclk||!hz)return -6;
 uint64_t seconds=cycles/pclk;if(seconds>UINT64_MAX/hz)return -6;
 uint64_t product=(cycles%pclk)*hz+next.fraction,extra=product/pclk,pixels=seconds*hz;
 if(extra>UINT64_MAX-pixels)return -6;pixels+=extra;
 if(pixels>UINT64_MAX-next.pixels)return -6;
 int status=sa_tmu0_advance(&next.timer,cycles);if(status)return status;
 uint32_t h=(next.load&1023u)+1u,v=((next.load>>16)&1023u)+1u;
 uint64_t lines=pixels/h;uint32_t residual=(uint32_t)(pixels%h)+next.hpos;
 lines+=residual/h;uint32_t position=residual%h;
 uint64_t in=hits(lines,next.line,next.vblank_int&1023u,v),out=hits(lines,next.line,(next.vblank_int>>16)&1023u,v),wraps=hits(lines,next.line,0,v);
 if(in>UINT64_MAX-next.in_events||out>UINT64_MAX-next.out_events||wraps>UINT64_MAX-next.wraps)return -6;
 next.fraction=product%pclk;next.pixels+=pixels;next.hpos=position;next.line=(next.line+(uint32_t)(lines%v))%v;
 next.field=(next.spg_control&16u)?(next.field^(uint32_t)(wraps&1u)):0;
 next.in_events+=in;next.out_events+=out;next.wraps+=wraps;
 if(in)next.pending|=8u;if(out)next.pending|=16u;
 copy_core(&s->core,&next);record(s,5u,(uint32_t)cycles,(uint32_t)(cycles>>32),0);return 0;
}
SA_STAGE_EXPORT int sa_frame_clock_next_in(struct sa_frame_clock_state *s,uint32_t reason){
 if(!s||reason>1u)return -6;if(!s->core.enabled)return 0;if(!room(s,2))return -6;
 struct sa_frame_clock_core *c=&s->core;
 uint32_t h=(c->load&1023u)+1u,v=((c->load>>16)&1023u)+1u,target=c->vblank_int&1023u;
 if(target>=v)return -4;
 uint32_t distance=((target+v-c->line)%v)*h;
 if(distance<=c->hpos)distance+=h*v;
 distance-=c->hpos;
 uint64_t needed=(uint64_t)distance*c->pclk_hz-c->fraction;uint32_t hz=rate(c);
 if(!hz)return -6;
 uint64_t cycles=needed/hz+(needed%hz!=0);
 if((reason?c->idle_waits:c->host_frames)==UINT64_MAX)return -6;
 int status=sa_frame_clock_advance(s,cycles);if(status)return status;
 if(reason)c->idle_waits++;else c->host_frames++;
 record(s,6u,reason,0,0);return 0;
}
SA_STAGE_EXPORT uint32_t sa_frame_clock_status(const struct sa_frame_clock_state *s){
 if(!s)return 0;
 const struct sa_frame_clock_core *c=&s->core;uint32_t start=c->vblank&1023u,end=(c->vblank>>16)&1023u;
 uint32_t active=start<end?(c->line>=start&&c->line<end):(start>end&&(c->line>=start||c->line<end));
 return c->line|(c->field<<10)|(active<<13);
}
SA_STAGE_EXPORT int sa_frame_clock_read(struct sa_frame_clock_state *s,uint32_t a,unsigned n,uint32_t *out){
 if(!s||!out)return -6;if(!room(s,1))return -6;
 uint32_t value;int status;
 if(timer_address(a)){
  if(a==0xffd8000cu&&s->timer_reads==UINT64_MAX)return -6;
  status=sa_tmu0_read(&s->core.timer,a,n,&value);if(status)return status;
  if(a==0xffd8000cu)s->timer_reads++;
 }else if((a&0x1fffffffu)==0x005f810cu){
  if(n!=4u)return -3;if(s->scan_reads==UINT64_MAX)return -6;
  value=sa_frame_clock_status(s);s->scan_reads++;
 }else return -2;
 *out=value;record(s,1u,a,value,n);return 0;
}
SA_STAGE_EXPORT int sa_frame_clock_write(struct sa_frame_clock_state *s,uint32_t a,uint32_t value,unsigned n){
 if(!s)return -6;if(!room(s,1))return -6;
 if((a&0x1fffffffu)==0x005f810cu)return -3;
 if(configuration(&s->core,a)){
  int status=configure_check(&s->core,a,value,n);if(status)return status;
  *configuration(&s->core,a)=value;record(s,7u,a,value,n);return 0;
 }
 if(!timer_address(a))return -2;
 int status=sa_tmu0_write(&s->core.timer,a,value,n);if(status)return status;
 if(n<4u)value&=(1u<<(8u*n))-1u;record(s,2u,a,value,n);return 0;
}
static int read_bus(void *opaque,uint32_t a,unsigned n,uint32_t *out){
 struct sa_frame_clock_binding *b=opaque;struct sa_frame_clock_state *s=b->state;
 if(timer_address(a)||(a&0x1fffffffu)==0x005f810cu)return sa_frame_clock_read(s,a,n,out);
 if((a&0x1fffffffu)==0x005f6900u){
  if(n!=4u||!out)return -3;if(!room(s,1))return -6;
  if(!b->parent.read)return -2;int status=b->parent.read(b->parent.context,a,n,out);if(status)return status;
  *out|=s->core.pending;record(s,3u,a,*out,n);return 0;
 }
 return b->parent.read?b->parent.read(b->parent.context,a,n,out):-2;
}
static int write_bus(void *opaque,uint32_t a,uint32_t value,unsigned n){
 struct sa_frame_clock_binding *b=opaque;struct sa_frame_clock_state *s=b->state;
 if(timer_address(a)||(a&0x1fffffffu)==0x005f810cu)return sa_frame_clock_write(s,a,value,n);
 if((a&0x1fffffffu)==0x005f6900u){
  if(n!=4u)return -3;if(!room(s,1))return -6;
  if(!b->parent.write)return -2;int status=b->parent.write(b->parent.context,a,value,n);if(status)return status;
  s->core.pending&=~value;record(s,4u,a,value,n);return 0;
 }
 if(configuration(&s->core,a)){
  if(!room(s,1))return -6;int status=configure_check(&s->core,a,value,n);if(status)return status;
  if(!b->parent.write)return -2;status=b->parent.write(b->parent.context,a,value,n);if(status)return status;
  *configuration(&s->core,a)=value;record(s,7u,a,value,n);return 0;
 }
 return b->parent.write?b->parent.write(b->parent.context,a,value,n):-2;
}
static int cache_bus(void *opaque,uint32_t a,unsigned op){struct sa_frame_clock_binding *b=opaque;return b->parent.cache?b->parent.cache(b->parent.context,a,op):-2;}
SA_STAGE_EXPORT int sa_frame_clock_bind(struct sa_frame_clock_binding *b,struct sa_frame_clock_state *s,const struct sa_vertex_bus *parent,struct sa_vertex_bus *out){
 if(!b||!s||!parent||!out)return -6;
 b->state=s;b->parent=*parent;*out=(struct sa_vertex_bus){b,read_bus,write_bus,cache_bus};return 0;
}

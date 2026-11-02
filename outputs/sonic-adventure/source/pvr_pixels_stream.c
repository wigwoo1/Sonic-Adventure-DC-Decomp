/* Add the original untextured floating ARGB32 vertex layout.
 * This bounded host profile rounds finite [0,1] channels to nearest 8-bit values;
 * it does not claim hardware colour conversion or precision. */
#include "pvr_pixels.h"
#if defined(_WIN32)
/* MSVC ABI floating-point marker; all arithmetic uses native instructions. */
int _fltused=0;
#endif
static uint32_t expand5(uint32_t v){return (v<<3)|(v>>2);}
static uint32_t unpack(uint32_t v,uint32_t format){
 uint32_t a,r,g,b;
 if(format==0){a=(v&0x8000u)?255u:0u;r=expand5((v>>10)&31u);g=expand5((v>>5)&31u);b=expand5(v&31u);}
 else if(format==1){a=255;r=expand5((v>>11)&31u);g=((v>>5)&63u);g=(g<<2)|(g>>4);b=expand5(v&31u);}
 else{a=((v>>12)&15u)*17u;r=((v>>8)&15u)*17u;g=((v>>4)&15u)*17u;b=(v&15u)*17u;}
 return (a<<24)|(r<<16)|(g<<8)|b;
}
SA_STAGE_EXPORT int sa_pixel_texture_info(uint32_t tsp,uint32_t tcw,uint32_t level,struct sa_pixel_texture *out){
 if(!out)return -6;
 uint32_t w=8u<<((tsp>>3)&7u),h=8u<<(tsp&7u),mip=tcw>>31,linear=(tcw>>26)&1u,fmt=(tcw>>27)&7u;
 if(fmt>2u||(tcw&0x42000000u))return -2; /* VQ and stride are separate unfinished domains. */
 if(mip){if(linear)return -2;h=w;}
 if(!mip&&level)return -3;
 uint32_t maximum=0;for(uint32_t n=w;n>1;n>>=1)maximum++;
 if(level>maximum)return -3;
 uint32_t side=w>>level;w=side;h>>=level;if(!h)h=1;
 uint32_t offset=mip?2u*((side*side+8u)/3u):0;
 *out=(struct sa_pixel_texture){w,h,((tcw&0x1ffffffu)<<3)+offset,fmt,linear};return 0;
}
static uint32_t texel_index(uint32_t x,uint32_t y,const struct sa_pixel_texture *t){
 if(t->linear)return y*t->width+x;
 uint32_t side=t->width<t->height?t->width:t->height,index=(x/side+y/side)*side*side;
 x%=side;y%=side;
 for(uint32_t bit=1,place=1;bit<side;bit<<=1,place<<=2){if(y&bit)index|=place;if(x&bit)index|=place<<1;}
 return index;
}
static uint32_t sample(const uint8_t *vram,const struct sa_pixel_texture *t,uint32_t x,uint32_t y){
 uint32_t a=t->address+2u*texel_index(x,y,t);return unpack(vram[a]|((uint32_t)vram[a+1u]<<8),t->format);
}
static int texture_range(const struct sa_pixel_texture *t,uint32_t size){uint64_t end=(uint64_t)t->address+2u*t->width*t->height;return end<=size?0:-3;}
SA_STAGE_EXPORT int sa_pixel_texture_decode(const uint8_t *vram,uint32_t size,uint32_t tsp,uint32_t tcw,uint32_t level,uint32_t *pixels,uint32_t capacity,struct sa_pixel_texture *out){
 if(!vram||!pixels||!out)return -6;struct sa_pixel_texture t;int rc=sa_pixel_texture_info(tsp,tcw,level,&t);if(rc)return rc;
 if(size>0x800000u||texture_range(&t,size)||capacity<t.width*t.height)return -3;
 for(uint32_t y=0;y<t.height;y++)for(uint32_t x=0;x<t.width;x++)pixels[y*t.width+x]=sample(vram,&t,x,y);
 *out=t;return 0;
}
struct vertex {int32_t x,y,z,u,v;uint32_t color;};
struct header {uint32_t pcw,isp,tsp,face[4],textured;struct sa_pixel_texture texture;};
static double number(uint32_t bits){union{uint32_t u;float f;}v;v.u=bits;return (double)v.f;}
static int finite(double x){return x==x&&x>=-1.0e30&&x<=1.0e30;}
static int32_t quantize(double x,double scale){double n=x*scale;return (int32_t)(n>=0?n+0.5:n-0.5);}
static uint32_t mul8(uint32_t a,uint32_t b){return (a*b+127u)/255u;}
static uint32_t color_component(uint32_t c,unsigned n){return (c>>(n*8u))&255u;}
static int prepare_header(const uint32_t *w,const struct sa_pixel_options *o,uint32_t size,struct header *h){
 uint32_t pcw=w[0],format=(pcw>>4)&3u,list=(pcw>>24)&7u;
 if(pcw&(0x000300c5u)||((pcw>>29)!=4u)||(list!=0u&&list!=2u&&list!=4u)||(format==3u)||(format==1u&&(pcw&8u)))return -2;
 h->pcw=pcw;h->isp=w[1];h->tsp=w[2];h->textured=!!(pcw&8u)&&!!o->textures;
 if(((h->tsp>>22)&3u)!=2u&&(pcw&8u))return -2; /* Fog remains outside this host profile. */
 if(h->tsp&0x03000000u)return -2;
 for(unsigned i=0;i<4;i++){
  double f=number(w[4+i]);if(format==2u&&(!finite(f)||f<0||f>1))return -3;
  h->face[i]=format==2u?(uint32_t)quantize(f,255.0):0;
 }
 if(h->textured){
  if((h->tsp>>13)&3u)return -2;
  int rc=sa_pixel_texture_info(h->tsp,w[3],o->mip_level,&h->texture);if(rc)return rc;
  if(texture_range(&h->texture,size))return -3;
 }
 return 0;
}
static int prepare_vertex(const uint32_t *w,const struct header *h,const struct sa_pixel_options *o,struct vertex *v){
 double x=number(w[1]),y=number(w[2]),z=number(w[3]),u=number(w[4]),vv=number(w[5]);
 if(!finite(x)||!finite(y)||!finite(z)||z<=0||z>16)return -3;
 x=x*o->width/o->input_width;y=y*o->height/o->input_height;
 if(x < -16384||x>16384||y < -16384||y>16384)return -3;
 if(h->textured&&(!finite(u)||!finite(vv)||u < -16||u>16||vv < -16||vv>16))return -3;
 v->x=quantize(x,16.0);v->y=quantize(y,16.0);v->z=quantize(z,65536.0);if(!v->z)return -3;
 v->u=h->textured?quantize(u*z,65536.0):0;v->v=h->textured?quantize(vv*z,65536.0):0;
 if(((h->pcw>>4)&3u)==2u){
  double intensity=number(w[6]);if(!finite(intensity)||intensity<0||intensity>1)return -3;
  uint32_t factor=(uint32_t)quantize(intensity,255.0);v->color=0;
  for(unsigned i=0;i<4;i++)v->color|=mul8(h->face[3u-i],factor)<<(i*8u);
 }else if(((h->pcw>>4)&3u)==1u){
  v->color=0;
  for(unsigned i=0;i<4;i++){
   double component=number(w[4u+i]);
   if(!finite(component)||component<0||component>1)return -3;
   v->color|=(uint32_t)quantize(component,255.0)<<(24u-8u*i);
  }
 }else v->color=w[6];
 return 0;
}
static int64_t edge(const struct vertex *a,const struct vertex *b,int32_t x,int32_t y){return (int64_t)(b->x-a->x)*(y-a->y)-(int64_t)(b->y-a->y)*(x-a->x);}
static int top_left(const struct vertex *a,const struct vertex *b){int32_t dy=b->y-a->y,dx=b->x-a->x;return dy<0||(dy==0&&dx>0);}
static int depth_test(uint32_t mode,uint32_t a,uint32_t b){switch(mode){case 0:return 0;case 1:return a<b;case 2:return a==b;case 3:return a<=b;case 4:return a>b;case 5:return a!=b;case 6:return a>=b;default:return 1;}}
static uint32_t factor(uint32_t mode,uint32_t s,uint32_t d,unsigned c,int source){
 switch(mode){case 0:return 0;case 1:return 255;case 2:return color_component(source?d:s,c);case 3:return 255-color_component(source?d:s,c);case 4:return s>>24;case 5:return 255-(s>>24);case 6:return d>>24;default:return 255-(d>>24);}
}
static uint32_t blend(uint32_t s,uint32_t d,uint32_t tsp){
 uint32_t out=0;for(unsigned c=0;c<4;c++){uint32_t value=(color_component(s,c)*factor(tsp>>29,s,d,c,1)+color_component(d,c)*factor((tsp>>26)&7u,s,d,c,0)+127u)/255u;if(value>255)value=255;out|=value<<(c*8u);}return out;
}
static uint32_t shade(uint32_t col,uint32_t tex,uint32_t tsp){
 if(tsp&(1u<<19))tex|=0xff000000u;
 uint32_t mode=(tsp>>6)&3u;if(!mode)return tex;
 uint32_t result=(mode==1u)?tex&0xff000000u:col&0xff000000u;
 if(mode==3u)result=mul8(col>>24,tex>>24)<<24;
 for(unsigned c=0;c<3;c++){
  uint32_t a=color_component(col,c),b=color_component(tex,c),value=mode==2u?(b*(tex>>24)+a*(255-(tex>>24))+127u)/255u:mul8(a,b);
  result|=value<<(c*8u);
 }return result;
}
static int32_t floor_ratio(int64_t a,int64_t b){int64_t q=a/b,r=a%b;return (int32_t)(q-(r<0));}
static uint32_t coordinate(int32_t v,uint32_t total,uint32_t clamp,uint32_t flip){
 if(clamp){if(v<0)return 0;if((uint32_t)v>=total)return total-1;return (uint32_t)v;}
 int32_t period=(int32_t)(flip?2u*total:total),n=v%period;if(n<0)n+=period;
 return flip&&(uint32_t)n>=total?2u*total-1u-(uint32_t)n:(uint32_t)n;
}
static void triangle(struct vertex a,struct vertex b,struct vertex c,const struct header *h,const uint8_t *vram,uint32_t *pixels,uint32_t *depth,const struct sa_pixel_options *o,struct sa_pixel_stats *s){
 int64_t area=edge(&a,&b,c.x,c.y);uint32_t cull=(h->isp>>27)&3u;
 if(!area||(cull==2u&&area<0)||(cull==3u&&area>0))return;
 if(area<0){struct vertex swap=a;a=b;b=swap;area=-area;}
 if(cull==1u&&(uint64_t)area<=o->cull_area)return;
 s->triangles++;
 int32_t minx=a.x,maxx=a.x,miny=a.y,maxy=a.y;
 if(b.x<minx)minx=b.x;if(c.x<minx)minx=c.x;if(b.x>maxx)maxx=b.x;if(c.x>maxx)maxx=c.x;
 if(b.y<miny)miny=b.y;if(c.y<miny)miny=c.y;if(b.y>maxy)maxy=b.y;if(c.y>maxy)maxy=c.y;
 minx=floor_ratio(minx,16);maxx=floor_ratio((int64_t)maxx+15,16);miny=floor_ratio(miny,16);maxy=floor_ratio((int64_t)maxy+15,16);
 if(minx<0)minx=0;if(miny<0)miny=0;if(maxx>(int32_t)o->width)maxx=(int32_t)o->width;if(maxy>(int32_t)o->height)maxy=(int32_t)o->height;
 for(int32_t y=miny;y<maxy;y++)for(int32_t x=minx;x<maxx;x++){
  int64_t w0=edge(&b,&c,x*16+8,y*16+8),w1=edge(&c,&a,x*16+8,y*16+8),w2=edge(&a,&b,x*16+8,y*16+8);
  if(w0<0||w1<0||w2<0||(!w0&&!top_left(&b,&c))||(!w1&&!top_left(&c,&a))||(!w2&&!top_left(&a,&b)))continue;
  s->fragments++;uint32_t z=(uint32_t)((w0*a.z+w1*b.z+w2*c.z)/area);uint32_t index=(uint32_t)y*o->width+(uint32_t)x;
  if(!depth_test(h->isp>>29,z,depth[index]))continue;
  s->depth_passed++;uint32_t col=c.color;
  if(h->pcw&2u){col=0;for(unsigned k=0;k<4;k++)col|=(uint32_t)((w0*color_component(a.color,k)+w1*color_component(b.color,k)+w2*color_component(c.color,k))/area)<<(k*8u);}
  if(!(h->tsp&(1u<<20)))col|=0xff000000u;
  if(h->textured){
   int64_t uz=(w0*a.u+w1*b.u+w2*c.u)/area,vz=(w0*a.v+w1*b.v+w2*c.v)/area;
   uint32_t tx=coordinate(floor_ratio(uz*h->texture.width,z),h->texture.width,(h->tsp>>16)&1u,(h->tsp>>18)&1u);
   uint32_t ty=coordinate(floor_ratio(vz*h->texture.height,z),h->texture.height,(h->tsp>>15)&1u,(h->tsp>>17)&1u);
   col=shade(col,sample(vram,&h->texture,tx,ty),h->tsp);s->texture_samples++;
  }
  uint32_t list=(h->pcw>>24)&7u;if(list==4u&&(col>>24)<o->alpha_ref)continue;
  pixels[index]=list==2u?blend(col,pixels[index],h->tsp):col;
  if(!(h->isp&(1u<<26)))depth[index]=z;s->pixels++;
 }
}
static int walk(const uint32_t *packets,uint32_t count,const uint8_t *vram,uint32_t size,uint32_t *pixels,uint32_t *depth,const struct sa_pixel_options *o,struct sa_pixel_stats *stats,int draw){
 struct header h;struct vertex previous[2];uint32_t have=0,active=0;
 for(uint32_t i=0;i<count;i++){
  const uint32_t *w=packets+8u*i;uint32_t kind=w[0]>>29;
  if(kind==4u){if(have)return -3;int rc=prepare_header(w,o,size,&h);if(rc)return rc;active=1;}
  else if(kind==7u){
   if(!active)return -3;struct vertex v;int rc=prepare_vertex(w,&h,o,&v);if(rc)return rc;
   if(have>=2u&&draw){if(have&1u)triangle(previous[1],previous[0],v,&h,vram,pixels,depth,o,stats);else triangle(previous[0],previous[1],v,&h,vram,pixels,depth,o,stats);}
   if(!have)previous[0]=v;else if(have==1u)previous[1]=v;else{previous[0]=previous[1];previous[1]=v;}
   have++;if(w[0]&0x10000000u){if(have<3u)return -3;have=0;}
  }else if(kind==0u){if(w[0]||have||!active)return -3;active=0;}
  else return -2;
 }return have?-3:0;
}
SA_STAGE_EXPORT int sa_pixel_draw(const uint32_t *packets,uint32_t count,const uint8_t *vram,uint32_t size,uint32_t *pixels,uint32_t *depth,uint64_t capacity,const struct sa_pixel_options *o,struct sa_pixel_stats *stats){
 if(!packets||!pixels||!depth||!o||!stats)return -6;
 if(!count||count>UINT32_MAX/8u||!o->width||!o->height||o->width>8192u||o->height>8192u||!o->input_width||!o->input_height||o->input_width>8192u||o->input_height>8192u||o->textures>1u||o->alpha_ref>255u||capacity<(uint64_t)o->width*o->height||size>0x800000u)return -3;
 if(o->textures&&!vram)return -6;
 int rc=walk(packets,count,vram,size,0,0,o,stats,0);if(rc)return rc;
 volatile uint8_t *p=(volatile uint8_t *)stats;for(unsigned i=0;i<sizeof(*stats);i++)p[i]=0;
 return walk(packets,count,vram,size,pixels,depth,o,stats,1);
}

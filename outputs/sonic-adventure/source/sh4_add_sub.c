#include "sh4_add_sub.h"
/* Ten little-endian limbs hold every finite binary32 value scaled by 2^149. */
struct wide {uint32_t limb[10];};
static unsigned bit(const struct wide *v,unsigned n){return n<320u?(v->limb[n/32u]>>(n%32u))&1u:0u;}
static struct wide expand(uint32_t raw){
 struct wide v={{0}};unsigned e=(raw>>23)&255u;
 uint32_t s=(raw&0x7fffffu)|(e!=0u?0x800000u:0u);unsigned shift=e!=0u?e-1u:0u;
 for(unsigned i=0;i<24u;i++)if((s>>i)&1u)v.limb[(shift+i)/32u]|=1u<<((shift+i)%32u);
 return v;
}
static int compare(const struct wide *a,const struct wide *b){for(unsigned i=10u;i!=0u;i--){if(a->limb[i-1u]>b->limb[i-1u])return 1;if(a->limb[i-1u]<b->limb[i-1u])return -1;}return 0;}
static struct wide sum(const struct wide *a,const struct wide *b,int subtract){
 struct wide r={{0}};uint64_t carry=0u;
 for(unsigned i=0;i<10u;i++){
  if(subtract){uint64_t rhs=(uint64_t)b->limb[i]+carry;r.limb[i]=(uint32_t)((uint64_t)a->limb[i]-rhs);carry=(uint64_t)a->limb[i]<rhs;}
  else{uint64_t v=(uint64_t)a->limb[i]+b->limb[i]+carry;r.limb[i]=(uint32_t)v;carry=v>>32;}
 }
 return r;
}
static uint32_t quantize(const struct wide *v,uint32_t sign,unsigned mode,int *inexact){
 unsigned h=320u;while(h!=0u && bit(v,h-1u)==0u)h--;
 if(h==0u){*inexact=0;return sign;}h--;
 unsigned shift=h>23u?h-23u:0u;uint32_t s=0u;
 for(unsigned i=0;i<24u;i++)s|=bit(v,shift+i)<<i;
 int discarded=0,below_half=0;
 for(unsigned i=0;i<shift;i++)if(bit(v,i)){discarded=1;if(i+1u<shift)below_half=1;}
 *inexact=discarded;
 if(mode==0u && shift!=0u && bit(v,shift-1u) && (below_half || (s&1u)))s++;
 if(s==0x1000000u){s>>=1u;h++;}
 if(h>=277u){*inexact=1;return sign|(mode==0u?0x7f800000u:0x7f7fffffu);}
 if(h<23u)return sign|s;
 return sign|((h-22u)<<23)|(s&0x7fffffu);
}
int sa_sh4_add_sub(uint32_t *a,uint32_t *b,uint32_t *fpscr,uint32_t *destination,int subtract){
 uint32_t status=*fpscr,mode=status&3u;
 if((status&0x80000u)!=0u || mode>1u)return -1;
 unsigned na=(*a&0x7fffffffu)>0x7f800000u,nb=(*b&0x7fffffffu)>0x7f800000u;
 if(na||nb){
  unsigned invalid=(na&&(*a&0x400000u))||(nb&&(*b&0x400000u));status&=~0x3f000u;
  if(invalid)status|=0x10040u;*fpscr=status;if((status&0x380u)||(invalid&&(status&0x800u)))return 1;
  *destination=0x7fbfffffu;return 0;
 }
 if((*a&0x7fffffffu)==0x7f800000u || (*b&0x7fffffffu)==0x7f800000u)return -1;
 status&=~0x3f000u;
 if((status&0x40000u)==0u && (((*a&0x7fffffffu)!=0u && (*a&0x7fffffffu)<0x800000u) || ((*b&0x7fffffffu)!=0u && (*b&0x7fffffffu)<0x800000u))){*fpscr=status|0x20000u;return 1;}
 if((status&0x40000u)!=0u){if((*a&0x7fffffffu)<0x800000u)*a&=0x80000000u;if((*b&0x7fffffffu)<0x800000u)*b&=0x80000000u;}
 uint32_t left=*a,right=*b;unsigned negative_left=left>>31,negative_right=(right>>31)^(subtract!=0);
 struct wide x=expand(left),y=expand(right),v;uint32_t sign;
 if(negative_left==negative_right){v=sum(&x,&y,0);sign=negative_left<<31;}
 else{int ordering=compare(&x,&y);if(ordering>=0){v=sum(&x,&y,1);sign=ordering==0?0u:negative_left<<31;}else{v=sum(&y,&x,1);sign=negative_right<<31;}}
 int inexact,unused;uint32_t result=quantize(&v,sign,mode,&inexact),nearest=quantize(&v,sign,0u,&unused);
 if((nearest&0x7fffffffu)==0x7f800000u){status|=0x4010u;inexact=1;}
 if((result&0x7fffffffu)<0x800000u){if((status&0x40000u)!=0u && (result&0x7fffffffu)!=0u){result&=0x80000000u;inexact=1;}if(inexact)status|=0x2008u;}
 if(inexact)status|=0x1004u;
 *fpscr=status;
 if((status&0x380u)!=0u)return 1;
 *destination=result;return 0;
}


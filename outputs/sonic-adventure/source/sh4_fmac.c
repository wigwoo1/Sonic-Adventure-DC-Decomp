#include "sh4_fmac.h"
/* Twenty limbs hold exact products scaled by 2^298. */
struct wide {uint32_t limb[20];};
static unsigned bit(const struct wide *v,unsigned n){return n<640u?(v->limb[n/32u]>>(n%32u))&1u:0u;}
static struct wide expand(uint32_t raw){
 struct wide v={{0}};unsigned e=(raw>>23)&255u;
 uint32_t s=(raw&0x7fffffu)|(e!=0u?0x800000u:0u);unsigned shift=e!=0u?e-1u:0u;
 for(unsigned i=0;i<24u;i++)if((s>>i)&1u)v.limb[(shift+i)/32u]|=1u<<((shift+i)%32u);
 return v;
}
static int compare(const struct wide *a,const struct wide *b){for(unsigned i=20u;i!=0u;i--){if(a->limb[i-1u]>b->limb[i-1u])return 1;if(a->limb[i-1u]<b->limb[i-1u])return -1;}return 0;}
static struct wide sum(const struct wide *a,const struct wide *b,int subtract){
 struct wide r={{0}};uint64_t carry=0u;
 for(unsigned i=0;i<20u;i++){
  if(subtract){uint64_t rhs=(uint64_t)b->limb[i]+carry;r.limb[i]=(uint32_t)((uint64_t)a->limb[i]-rhs);carry=(uint64_t)a->limb[i]<rhs;}
  else{uint64_t v=(uint64_t)a->limb[i]+b->limb[i]+carry;r.limb[i]=(uint32_t)v;carry=v>>32;}
 }
 return r;
}
static uint32_t quantize(const struct wide *v,uint32_t sign,unsigned mode,int *inexact){
 unsigned h=640u;while(h!=0u && bit(v,h-1u)==0u)h--;
 if(h==0u){*inexact=0;return sign;}h--;
 unsigned shift=h>172u?h-23u:149u;uint32_t s=0u;
 for(unsigned i=0;i<24u;i++)s|=bit(v,shift+i)<<i;
 int discarded=0,below_half=0;
 for(unsigned i=0;i<shift;i++)if(bit(v,i)){discarded=1;if(i+1u<shift)below_half=1;}
 *inexact=discarded;
 if(mode==0u && shift!=0u && bit(v,shift-1u) && (below_half || (s&1u)))s++;
 if(s==0x1000000u){s>>=1u;h++;}
 if(h>=426u){*inexact=1;return sign|(mode==0u?0x7f800000u:0x7f7fffffu);}
 if(h<172u)return sign|s;
 return sign|((h-171u)<<23)|(s&0x7fffffu);
}
static struct wide product(const struct wide *a,const struct wide *b){
 struct wide r={{0}};
 for(unsigned i=0;i<20u;i++){
  uint64_t carry=0u;
  for(unsigned j=0;i+j<20u;j++){
   uint64_t v=(uint64_t)a->limb[i]*b->limb[j]+r.limb[i+j]+carry;
   r.limb[i+j]=(uint32_t)v;carry=v>>32;
  }
 }
 return r;
}
static struct wide align_addend(const struct wide *a){struct wide r={{0}};for(unsigned i=0;i<491u;i++)if(bit(a,i))r.limb[(i+149u)/32u]|=1u<<((i+149u)%32u);return r;}
int sa_sh4_fmac(uint32_t *a,uint32_t *b,uint32_t *c,uint32_t *fpscr,uint32_t *destination){
 uint32_t status=*fpscr,mode=status&3u;
 if((status&0x80000u)!=0u || mode>1u || (*a&0x7fffffffu)>=0x7f800000u || (*b&0x7fffffffu)>=0x7f800000u || (*c&0x7fffffffu)>=0x7f800000u)return -1;
 status&=~0x3f000u;
 uint32_t *operands[3]={a,b,c};
 for(unsigned i=0;i<3u;i++){
  uint32_t absolute=*operands[i]&0x7fffffffu;
  if(absolute!=0u && absolute<0x800000u && (status&0x40000u)==0u){*fpscr=status|0x20000u;return 1;}
 }
 if((status&0x40000u)!=0u)for(unsigned i=0;i<3u;i++)if((*operands[i]&0x7fffffffu)<0x800000u)*operands[i]&=0x80000000u;
 uint32_t left=*a,right=*b,addend=*c;unsigned negative_product=(left^right)>>31,negative_addend=addend>>31;
 struct wide x=expand(left),y=expand(right),z=expand(addend),prod=product(&x,&y),aligned=align_addend(&z),v;
 uint32_t sign,result;int inexact=0,unused;
 if((left&0x7fffffffu)==0u || (right&0x7fffffffu)==0u){result=(addend&0x7fffffffu)!=0u?addend:(left^right^addend)&0x80000000u;}
 else{
  if(negative_product==negative_addend){v=sum(&prod,&aligned,0);sign=negative_product<<31;}
  else{int ordering=compare(&prod,&aligned);if(ordering>=0){v=sum(&prod,&aligned,1);sign=ordering==0?0u:negative_product<<31;}else{v=sum(&aligned,&prod,1);sign=negative_addend<<31;}}
  result=quantize(&v,sign,mode,&inexact);uint32_t nearest=quantize(&v,sign,0u,&unused);
  if((nearest&0x7fffffffu)==0x7f800000u){status|=0x4010u;inexact=1;}
  if((result&0x7fffffffu)<0x800000u){if((status&0x40000u)!=0u && (result&0x7fffffffu)!=0u){result&=0x80000000u;inexact=1;}if(inexact)status|=0x2008u;}
 }
 if(inexact)status|=0x1004u;
 *fpscr=status;if((status&0x380u)!=0u)return 1;
 *destination=result;return 0;
}

#include "sh4_widen_compare.h"
#include "sh4_compare_bits.h"
/* SH-4 Software Manual Rev.6.00, 9.27 and 9.29. Independent bit arithmetic;
 * no host floating-point conversions, rounding mode, or NaN propagation.
 */
int sa_sh4_fcnvsd(struct sa_fpu_banks *f,uint32_t raw,unsigned n){
 if(!f || n>14u || (n&1u) || !(f->fpscr&0x80000u))return -1;
 uint32_t status=f->fpscr&~0x3f000u,abs=raw&0x7fffffffu;
 uint64_t bits=(uint64_t)(raw>>31)<<63;
 if(abs && abs<0x800000u){
  if(!(status&0x40000u)){f->fpscr=status|0x20000u;return 1;}
 }else if(abs>0x7f800000u){
  bits=UINT64_C(0x7ff7ffffffffffff);
  if(raw&0x400000u){status|=0x10040u;if(status&0x800u){f->fpscr=status;return 1;}}
 }else if(abs==0x7f800000u)bits|=UINT64_C(0x7ff0000000000000);
 else if(abs)bits|=((uint64_t)((raw>>23&255u)+896u)<<52)|((uint64_t)(raw&0x7fffffu)<<29);
 unsigned bank=(status>>21)&1u;
 f->fpscr=status;f->bank[bank][n]=(uint32_t)(bits>>32);f->bank[bank][n+1u]=(uint32_t)bits;
 return 0;
}
int sa_sh4_compare_pair(struct sa_fpu_banks *f,uint32_t *sr,unsigned n,unsigned m,int greater){
 if(!f || !sr || n>15u || m>15u)return -1;
 unsigned bank=(f->fpscr>>21)&1u;
 if(!(f->fpscr&0x80000u))return sa_sh4_compare_bits(f->bank[bank][n],f->bank[bank][m],&f->fpscr,sr,greater);
 if((n|m)&1u)return -1;
 uint64_t a=((uint64_t)f->bank[bank][n]<<32)|f->bank[bank][n+1u];
 uint64_t b=((uint64_t)f->bank[bank][m]<<32)|f->bank[bank][m+1u];
 uint64_t values[2]={a,b},mask=UINT64_C(0x7fffffffffffffff),inf=UINT64_C(0x7ff0000000000000);
 uint32_t status=f->fpscr&~0x3f000u;unsigned nan=0,signaling=0;
 for(unsigned i=0;i<2u;i++)if((values[i]&mask)>inf){nan=1;signaling|=!!(values[i]&UINT64_C(0x0008000000000000));}
 if(signaling || (greater && nan)){
  status|=0x10040u;f->fpscr=status;if(status&0x800u)return 1;*sr&=~1u;return 0;
 }
 if(nan){f->fpscr=status;*sr&=~1u;return 0;}
 if(status&0x40000u)for(unsigned i=0;i<2u;i++)if((values[i]&mask)<UINT64_C(0x0010000000000000))values[i]&=~mask;
 a=values[0];b=values[1];unsigned zero=!(a&mask)&&!(b&mask),t;
 if(!greater)t=a==b||zero;
 else if(zero)t=0;
 else if((a^b)>>63)t=!(a>>63);
 else t=(a>>63)?a<b:a>b;
 f->fpscr=status;*sr=(*sr&~1u)|t;return 0;
}

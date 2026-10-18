#ifndef SA_SH4_COMPARE_BITS_H
#define SA_SH4_COMPARE_BITS_H
#include <stdint.h>
/* SH-4 manual9.27: qNaN has bit22 clear, sNaN has bit22 set. */
static int sa_sh4_compare_bits(uint32_t a,uint32_t b,uint32_t *fpscr,uint32_t *sr,int greater){
 uint32_t status=*fpscr;if(status&0x80000u)return -1;status&=~0x3f000u;
 unsigned nan=0,signaling=0;
 uint32_t values[2]={a,b};
 for(unsigned i=0;i<2;i++){
  uint32_t v=values[i],x=v&0x7fffffffu;
  if(x>0x7f800000u){nan=1;signaling|=!!(v&0x400000u);}
 }
 if(signaling||(greater&&nan)){status|=0x10040u;*fpscr=status;if(status&0x800u)return 1;*sr&=~1u;return 0;}
 if(nan){*fpscr=status;*sr&=~1u;return 0;}
 for(unsigned i=0;i<2;i++){uint32_t x=values[i]&0x7fffffffu;if(x&&x<0x800000u){if(!(status&0x40000u))return -1;values[i]&=0x80000000u;}}
 a=values[0];b=values[1];unsigned zero=!(a&0x7fffffffu)&&!(b&0x7fffffffu),t;
 if(nan)t=0;else if(!greater)t=a==b||zero;else if(zero)t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;
 *fpscr=status;*sr=(*sr&~1u)|t;return 0;
}
#endif

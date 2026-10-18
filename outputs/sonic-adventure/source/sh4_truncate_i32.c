#include "stage00_task.h"
/* FTRC ignores RM; fractional finite conversion sets no inexact flag. */
SA_STAGE_EXPORT int sa_sh4_truncate_i32(uint32_t raw,uint32_t *fpscr,uint32_t *out){
 uint32_t status=*fpscr;if(status&0x80000u)return -1;status&=~0x3f000u;
 uint32_t absolute=raw&0x7fffffffu,result;int negative=(raw>>31)!=0u;
 int invalid=negative?absolute>0x4f000000u:absolute>0x4effffffu;
 if(invalid){status|=0x10040u;*fpscr=status;if(status&0x800u)return 1;*out=(!negative && absolute<=0x7f800000u)?0x7fffffffu:0x80000000u;return 0;}
 int exponent=(int)(absolute>>23)-127;
 if(exponent<0)result=0u;
 else{uint32_t significand=(absolute&0x7fffffu)|0x800000u;result=exponent>=23?significand<<(unsigned)(exponent-23):significand>>(unsigned)(23-exponent);if(negative)result=0u-result;}
 *fpscr=status;*out=result;return 0;
}

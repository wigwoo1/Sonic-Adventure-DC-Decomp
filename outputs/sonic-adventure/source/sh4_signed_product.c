#include "sh4_signed_product.h"
void sa_sh4_dmuls(uint32_t left,uint32_t right,uint32_t *macl,uint32_t *mach){
 int64_t a=(left&0x80000000u)?(int64_t)left-0x100000000LL:(int64_t)left;
 int64_t b=(right&0x80000000u)?(int64_t)right-0x100000000LL:(int64_t)right;
 uint64_t product=(uint64_t)(a*b);
 *macl=(uint32_t)product;
 *mach=(uint32_t)(product>>32);
}

#include "sh4_vector_base.h"
int sa_sh4_stc_vbr(uint32_t vbr,uint32_t sr,uint32_t *out,unsigned delay){
 uint32_t unused;int status=sa_sh4_stc_sr(sr,&unused,delay);
 if(status)return status;
 *out=vbr;return 0;
}

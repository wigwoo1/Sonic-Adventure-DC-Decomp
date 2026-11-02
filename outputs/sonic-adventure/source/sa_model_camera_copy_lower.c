/* Bridge the existing lower dispatcher to the complete original copy body.
 * Its documented internal context begins with the stop-publication pointer.
 * Public/main entry points pass that pointer explicitly instead. */
#include "fpu_cleanup_transfer.h"
typedef int (*service)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
int sa_camera_record_copy_8c10cd38(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*);
SA_STAGE_EXPORT int sa_engine_record_copy_dispatch(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service call){
 uint32_t local_stop=0,*stop=ctx?*(uint32_t**)ctx:0;
 return sa_camera_record_copy_8c10cd38(m,f,sr,r,pr,fpul,macl,ctx,call,stop?stop:&local_stop);
}

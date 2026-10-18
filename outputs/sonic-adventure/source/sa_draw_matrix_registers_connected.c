#include "fpu_cleanup_transfer.h"
struct cpu {uint32_t r[16],sr,pr,pc;};
typedef int (*service)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
#define DECL(name) int name(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*)
DECL(sa_draw_matrix_scale_registers_8c63a69c);
DECL(sa_draw_matrix_translate_registers_8c63a878);
SA_STAGE_EXPORT int sa_draw_matrix_registers_connected(uint8_t *m,struct cpu *s,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *macl,uint32_t entry){
 uint32_t stop=entry;int status;
 if(entry==0x8c63a69cu)status=sa_draw_matrix_scale_registers_8c63a69c(m,f,&s->sr,s->r,&s->pr,fpul,macl,0,0,&stop);
 else if(entry==0x8c63a878u)status=sa_draw_matrix_translate_registers_8c63a878(m,f,&s->sr,s->r,&s->pr,fpul,macl,0,0,&stop);
 else status=-2;
 s->pc=status?stop:s->pr;return status;
}

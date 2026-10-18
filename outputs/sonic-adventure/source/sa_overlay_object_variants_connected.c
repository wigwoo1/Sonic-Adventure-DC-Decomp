#include "fpu_cleanup_transfer.h"
struct sa_selector_cpu {uint32_t r[16],sr,pr,pc;};
typedef int (*service)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
#define DECL(name) int name(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service)
DECL(sa_overlay_variant_palette_0c904aea);
DECL(sa_overlay_variant_material_158_0c904bbc);
DECL(sa_overlay_variant_material_161_0c904c80);
SA_STAGE_EXPORT int sa_overlay_object_variant_connected(uint8_t *m,struct sa_selector_cpu *s,struct sa_fpu_banks *f,uint32_t entry){
 uint32_t fpul=0,macl=0;int status;
 switch(entry){
 case 0x0c904aeau:status=sa_overlay_variant_palette_0c904aea(m,f,&s->sr,s->r,&s->pr,&fpul,&macl,0,0);break;
 case 0x0c904bbcu:status=sa_overlay_variant_material_158_0c904bbc(m,f,&s->sr,s->r,&s->pr,&fpul,&macl,0,0);break;
 case 0x0c904c80u:status=sa_overlay_variant_material_161_0c904c80(m,f,&s->sr,s->r,&s->pr,&fpul,&macl,0,0);break;
 default:return -2;
 }
 s->pc=status?entry:s->pr;return status;
}

#include "fpu_cleanup_transfer.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+(a&0xffffffu);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* Original8c07a786..8c07a800 after hierarchy preparation: matrix pointer
 * frame, transform service registers, constants and empty-list branch. */
SA_STAGE_EXPORT int sa_model_render_frame_setup(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *next){
 if(f->fpscr&0x180000u)return -1;
 const uint32_t a=0x8c798698u,b=0x8c7986d8u,c=0x8c798718u,d=0x8c798758u;
 const uint32_t entries[][2]={{68,a},{64,b},{60,c},{56,d},{52,a},{48,b},{44,c},{40,d},{88,a},{104,a},{80,c},{116,b},{92,b},{96,d},{120,a},{84,d},{100,a},{108,c},{112,b}};
 for(unsigned i=0;i<sizeof(entries)/sizeof(entries[0]);i++)wr(ram,r[15]+entries[i][0],entries[i][1]);
 r[2]=b;r[0]=0x8c07a8f8u;r[3]=rd(ram,r[14]);r[13]=0x8c638eccu;r[9]=0x8c639b98u;r[12]=0x8c638f28u;
 *sr=(*sr&~1u)|(r[3]==0);unsigned bank=(f->fpscr>>21)&1u;f->bank[bank][13]=0;f->bank[bank][14]=0x3f800000u;f->bank[bank][15]=rd(ram,r[0]);
 *next=r[3]?0x8c07a800u:0x8c07aedau;return 0;
}

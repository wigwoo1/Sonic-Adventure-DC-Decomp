/* AL_MAIN.BIN 0c905548: object model selection and scale initialization.
 * Inferred name. Original integer/FMOV register effects and bit priority kept.
 * Valid bounded RAM/transfer addresses only; no hardware exception handling.
 */
#include "fpu_cleanup_transfer.h"
struct sa_selector_cpu {uint32_t r[16],sr,pr,pc;};
static uint32_t rd(const uint8_t *m,uint32_t a){
 a&=0xffffffu;return (uint32_t)m[a]|((uint32_t)m[a+1]<<8)|((uint32_t)m[a+2]<<16)|((uint32_t)m[a+3]<<24);
}
static void wr(uint8_t *m,uint32_t a,uint32_t v){
 a&=0xffffffu;for(unsigned i=0;i<4;i++)m[a+i]=(uint8_t)(v>>(8*i));
}
SA_STAGE_EXPORT int sa_overlay_face_model_selector_0c905548(uint8_t *m,struct sa_selector_cpu *s,struct sa_fpu_banks *f){
 uint32_t *r=s->r;unsigned bank=(f->fpscr>>21)&1u;int wide=(f->fpscr&0x100000u)!=0;
 r[4]=2;r[0]=0xffffffecu;r[4]<<=8;r[0]&=255;r[4]+=0x48;
 r[7]=rd(m,r[0]+r[5]);r[4]+=r[5];r[5]=0xffffff88u;r[3]=r[4];r[5]&=255;
 r[0]=0x34;r[3]+=4;r[7]=(uint32_t)(int32_t)(int8_t)m[(r[0]+r[7])&0xffffffu];r[3]+=r[5];r[2]=rd(m,r[3]);
 r[0]=0x0c9055e8u;r[3]=r[4];f->bank[bank][4]=rd(m,r[0]);if(wide)f->bank[bank][5]=rd(m,r[0]+4u);
 r[3]+=4;r[0]=0x20;r[3]+=r[5];wr(m,r[0]+r[2],f->bank[bank][4]);if(wide)wr(m,r[0]+r[2]+4u,f->bank[bank][5]);
 r[2]=rd(m,r[3]);r[3]=r[4];r[3]+=4;r[0]=0x24;r[3]+=r[5];
 wr(m,r[0]+r[2],f->bank[bank][4]);if(wide)wr(m,r[0]+r[2]+4u,f->bank[bank][5]);
 r[2]=rd(m,r[3]);r[0]=0x28;r[6]=r[7]&255;
 wr(m,r[0]+r[2],f->bank[bank][4]);if(wide)wr(m,r[0]+r[2]+4u,f->bank[bank][5]);
 r[3]=0x10;s->sr=(s->sr&~1u)|!(r[6]&r[3]);if(!(s->sr&1u)){r[1]=r[4];r[2]=rd(m,0x0c9055ecu);goto selected;}
 r[3]=8;s->sr=(s->sr&~1u)|!(r[6]&r[3]);if(!(s->sr&1u)){r[1]=r[4];r[2]=rd(m,0x0c9055f0u);goto selected;}
 r[3]=4;s->sr=(s->sr&~1u)|!(r[6]&r[3]);if(!(s->sr&1u)){r[1]=r[4];r[2]=rd(m,0x0c9055f4u);goto selected;}
 r[3]=2;s->sr=(s->sr&~1u)|!(r[6]&r[3]);if(!(s->sr&1u)){r[1]=r[4];r[2]=rd(m,0x0c9055f8u);goto selected;}
 r[3]=1;s->sr=(s->sr&~1u)|!(r[3]&r[6]);if(s->sr&1u)goto done;
 r[1]=r[4];r[1]+=4;r[5]+=r[1];r[1]=rd(m,0x0c905740u);r[2]=rd(m,r[5]);r[0]=rd(m,r[1]+4u);wr(m,r[2]+4u,r[0]);goto done;
selected:
 r[1]+=4;r[5]+=r[1];r[1]=rd(m,r[2]+4u);r[3]=rd(m,r[5]);wr(m,r[3]+4u,r[1]);
done:
 s->pc=s->pr;return 0;
}

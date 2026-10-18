#include "fpu_cleanup_transfer.h"
#include <stdint.h>

#ifdef _WIN32
int _fltused=0x9875;
#endif

typedef int (*instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
struct sa_angle_cpu_state { uint32_t r[16],sr,pr,pc,fpul; };

/* Callback form remains for larger native routines that inject FSCA results. */
SA_STAGE_EXPORT int sa_angle_trig(struct sa_fpu_banks *f,uint32_t *r,uint32_t *fpul,void *ctx,instruction execute,int cosine){
 if(f->fpscr&0x80000u)return -1;
 r[4]&=0xffffu;*fpul=r[4];int status=execute(ctx,0xf0fdu,f,*fpul);if(status)return status;
 if(cosine){uint32_t *fr=f->bank[(f->fpscr>>21)&1u];if(f->fpscr&0x100000u){uint32_t *xf=f->bank[((f->fpscr>>21)&1u)^1u];fr[0]=xf[0];fr[1]=xf[1];}else fr[0]=fr[1];}return 0;
}

static double sa_sin_quadrant(double x){
 double z=x*x;
 return x*(1.0+z*(-1.0/6.0+z*(1.0/120.0+z*(-1.0/5040.0+z*(1.0/362880.0+z*(-1.0/39916800.0+z*(1.0/6227020800.0+z*(-1.0/1307674368000.0+z*(1.0/355687428096000.0)))))))));
}

static double sa_cos_quadrant(double x){
 double z=x*x;
 return 1.0+z*(-1.0/2.0+z*(1.0/24.0+z*(-1.0/720.0+z*(1.0/40320.0+z*(-1.0/3628800.0+z*(1.0/479001600.0+z*(-1.0/87178291200.0+z*(1.0/20922789888000.0))))))));
}

static uint32_t sa_float_bits(double value){
 union { float f; uint32_t u; } bits;
 bits.f=(float)value;
 return bits.u;
}

/* SH-4 FSCA: FPUL is a 16-bit turn fraction; the manual allows 2^-21 error. */
static int sa_fsca(struct sa_fpu_banks *f,uint32_t fpul){
 if(f->fpscr&0x80000u)return -1;
 uint32_t fpscr=f->fpscr;
 f->fpscr=(fpscr&~0x3f000u)|0x1000u|0x4u; /* Clear cause; set inexact cause and sticky flag. */
 if(fpscr&0x80u)return -2; /* Enabled inexact trap: outputs and return PC stay untouched. */
 uint32_t phase=fpul&0xffffu;
 uint32_t quadrant=phase>>14;
 uint32_t remainder=phase&0x3fffu;
 double x=(double)remainder*(6.2831853071795864769252867665590057683943387987502/65536.0);
 double s=sa_sin_quadrant(x),c=sa_cos_quadrant(x),sine,cosine;
 if(quadrant==0u){sine=s;cosine=c;}
 else if(quadrant==1u){sine=c;cosine=-s;}
 else if(quadrant==2u){sine=-s;cosine=-c;}
 else {sine=-c;cosine=s;}
 unsigned bank=(fpscr>>21)&1u;
 f->bank[bank][0]=sa_float_bits(sine);
 f->bank[bank][1]=sa_float_bits(cosine);
 return 0;
}

static int sa_angle_entry(struct sa_fpu_banks *f,struct sa_angle_cpu_state *cpu,int cosine){
 cpu->r[4]&=0xffffu;
 cpu->fpul=cpu->r[4];
 int status=sa_fsca(f,cpu->fpul);
 if(status)return status;
 if(cosine){
  uint32_t *fr=f->bank[(f->fpscr>>21)&1u];
  if(f->fpscr&0x100000u){
   uint32_t *xf=f->bank[((f->fpscr>>21)&1u)^1u];
   fr[0]=xf[0];fr[1]=xf[1];
  }else fr[0]=fr[1];
 }
 cpu->pc=cpu->pr;
 return 0;
}

/* Retail entry at 0x8c63a9b8: FSCA executes in the RTS delay slot. */
SA_STAGE_EXPORT int sa_sincos_phase_8c63a9b8(struct sa_fpu_banks *f,struct sa_angle_cpu_state *cpu){
 return sa_angle_entry(f,cpu,0);
}

/* Retail entry at 0x8c63a970: FMOV FR1,FR0 executes in the RTS delay slot. */
SA_STAGE_EXPORT int sa_cosine_phase_8c63a970(struct sa_fpu_banks *f,struct sa_angle_cpu_state *cpu){
 return sa_angle_entry(f,cpu,1);
}


/* Test-only adapter oracle: direct calls to the pinned vendor primitive API.
 * This checks register/opcode plumbing, not independent numerical fidelity.
 * Mathematical error/invariant checks in the Python verifier are separate.
 */
#include "fpu_cleanup_transfer.h"
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"
SA_STAGE_EXPORT int sa_vector_primitive_reference(unsigned kind,unsigned n,unsigned m,struct sa_fpu_banks *f,uint32_t fpul){
 const unsigned b=(f->fpscr>>21)&1u;
 if(kind==0)return sh4_fsca_ex(fpul,&f->fpscr,&f->bank[b][n],&f->bank[b][n+1]);
 if(kind==1)return sh4_ftrv_ex(f->bank[b^1u],&f->bank[b][n],&f->fpscr);
 if(kind==2)return sh4_fipr_ex(&f->bank[b][m],&f->bank[b][n],&f->fpscr,&f->bank[b][n+3]);
 return sh4_fsrra_ex(f->bank[b][n],&f->fpscr,&f->bank[b][n]);
}

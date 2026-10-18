#include "fpu_cleanup_transfer.h"
typedef int (*sa_rotation_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
static void move_pair(struct sa_fpu_banks *f,unsigned n,unsigned m){
 unsigned bank=(f->fpscr>>21)&1u;
 if(!(f->fpscr&0x100000u)){f->bank[bank][n]=f->bank[bank][m];return;}
 unsigned source=bank^(m&1u),dest=bank^(n&1u);m&=~1u;n&=~1u;uint32_t a=f->bank[source][m],b=f->bank[source][m+1u];f->bank[dest][n]=a;f->bank[dest][n+1u]=b;
}
/* Original implicit matrix rotation8c639cf4..8c639d7a. FSCA/FTRV explicit. */
SA_STAGE_EXPORT int sa_model_matrix_rotate_implicit(struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,void *ctx,sa_rotation_instruction vector){
 if(f->fpscr&0x180000u||r[4]||!vector)return -1;unsigned bank=(f->fpscr>>21)&1u;int status;*sr=(*sr&~1u)|1u;
*sr=(*sr&~1u)|(r[7]==0);*fpul=r[7];if(r[7]){
f->bank[bank][3]=0;
status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;
f->bank[bank][6]=0;
f->bank[bank][7]=0;
move_pair(f,2,0);
move_pair(f,0,1);
move_pair(f,1,2);
f->bank[bank][2]=0;
move_pair(f,5,0);
move_pair(f,4,1);
f->bank[bank][4]^=0x80000000u;
status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;
status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;
f->fpscr^=0x100000u;
move_pair(f,1,0);
move_pair(f,3,2);
move_pair(f,5,4);
move_pair(f,7,6);
f->fpscr^=0x100000u;
}
*sr=(*sr&~1u)|(r[6]==0);*fpul=r[6];if(r[6]){
f->bank[bank][3]=0;
status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;
f->bank[bank][5]=0;
f->bank[bank][7]=0;
move_pair(f,2,0);
move_pair(f,0,1);
f->bank[bank][1]=0;
move_pair(f,6,0);
move_pair(f,4,2);
f->bank[bank][2]^=0x80000000u;
status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;
status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;
f->fpscr^=0x100000u;
move_pair(f,1,0);
move_pair(f,3,2);
move_pair(f,9,4);
move_pair(f,11,6);
f->fpscr^=0x100000u;
}
*sr=(*sr&~1u)|(r[5]==0);*fpul=r[5];if(r[5]){
f->bank[bank][3]=0;
status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;
f->bank[bank][4]=0;
f->bank[bank][7]=0;
move_pair(f,2,0);
f->bank[bank][0]=0;
move_pair(f,5,2);
move_pair(f,6,1);
f->bank[bank][5]^=0x80000000u;
status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;
status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;
f->fpscr^=0x100000u;
move_pair(f,5,0);
move_pair(f,7,2);
move_pair(f,9,4);
move_pair(f,11,6);
f->fpscr^=0x100000u;
}
return 0;
}

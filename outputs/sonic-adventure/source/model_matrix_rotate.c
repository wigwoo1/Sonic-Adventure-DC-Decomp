#include "fpu_cleanup_transfer.h"
typedef int (*sa_rotation_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
static void move_pair(struct sa_fpu_banks *f,unsigned n,unsigned m){
 unsigned bank=(f->fpscr>>21)&1u;
 if(!(f->fpscr&0x100000u)){f->bank[bank][n]=f->bank[bank][m];return;}
 unsigned source=bank^(m&1u),dest=bank^(n&1u);m&=~1u;n&=~1u;uint32_t a=f->bank[source][m],b=f->bank[source][m+1u];f->bank[dest][n]=a;f->bank[dest][n+1u]=b;
}
int sa_model_matrix_rotate_implicit(struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,void *,sa_rotation_instruction);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)ram[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
/* Original8c639cf4..8c639ec2 complete rotation dataflow. FSCA/FTRV explicit. */
SA_STAGE_EXPORT int sa_model_matrix_rotate(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,void *ctx,sa_rotation_instruction vector){
 if(f->fpscr&0x180000u||!vector)return -1;if(!r[4])return sa_model_matrix_rotate_implicit(f,sr,r,fpul,ctx,vector);unsigned bank=(f->fpscr>>21)&1u;int status;*sr&=~1u;
*sr=(*sr&~1u)|(r[7]==0);*fpul=r[7];if(r[7]){
f->bank[bank][3]=0; /* 8c639d80 */
status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status; /* 8c639d82 */
f->bank[bank][6]=0; /* 8c639d84 */
f->bank[bank][7]=0; /* 8c639d86 */
move_pair(f,2,0); /* 8c639d88 */
move_pair(f,0,1); /* 8c639d8a */
move_pair(f,1,2); /* 8c639d8c */
f->bank[bank][2]=0; /* 8c639d8e */
move_pair(f,5,0); /* 8c639d90 */
move_pair(f,4,1); /* 8c639d92 */
f->bank[bank][4]^=0x80000000u; /* 8c639d94 */
r[15]-=4u;wr(ram,r[15],f->bank[bank][12]); /* 8c639d96 */
r[15]-=4u;wr(ram,r[15],f->bank[bank][13]); /* 8c639d98 */
r[15]-=4u;wr(ram,r[15],f->bank[bank][14]); /* 8c639d9a */
r[15]-=4u;wr(ram,r[15],f->bank[bank][15]); /* 8c639d9c */
f->fpscr^=0x100000u; /* 8c639d9e */
move_pair(f,8,1); /* 8c639da0 */
move_pair(f,10,3); /* 8c639da2 */
move_pair(f,12,5); /* 8c639da4 */
move_pair(f,14,7); /* 8c639da6 */
f->fpscr^=0x100000u; /* 8c639da8 */
f->fpscr^=0x200000u;bank=(f->fpscr>>21)&1u; /* 8c639daa */
f->bank[bank][0]=rd(ram,r[4]);r[4]+=4u; /* 8c639dac */
f->bank[bank][1]=rd(ram,r[4]);r[4]+=4u; /* 8c639dae */
f->bank[bank][2]=rd(ram,r[4]);r[4]+=4u; /* 8c639db0 */
f->bank[bank][3]=rd(ram,r[4]);r[4]+=4u; /* 8c639db2 */
f->bank[bank][4]=rd(ram,r[4]);r[4]+=4u; /* 8c639db4 */
f->bank[bank][5]=rd(ram,r[4]);r[4]+=4u; /* 8c639db6 */
f->bank[bank][6]=rd(ram,r[4]);r[4]+=4u; /* 8c639db8 */
f->bank[bank][7]=rd(ram,r[4]);r[4]+=4u; /* 8c639dba */
f->fpscr^=0x200000u;bank=(f->fpscr>>21)&1u; /* 8c639dbc */
status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status; /* 8c639dbe */
status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status; /* 8c639dc0 */
r[4]-=4u;wr(ram,r[4],f->bank[bank][7]); /* 8c639dc2 */
r[4]-=4u;wr(ram,r[4],f->bank[bank][6]); /* 8c639dc4 */
r[4]-=4u;wr(ram,r[4],f->bank[bank][5]); /* 8c639dc6 */
r[4]-=4u;wr(ram,r[4],f->bank[bank][4]); /* 8c639dc8 */
r[4]-=4u;wr(ram,r[4],f->bank[bank][3]); /* 8c639dca */
r[4]-=4u;wr(ram,r[4],f->bank[bank][2]); /* 8c639dcc */
r[4]-=4u;wr(ram,r[4],f->bank[bank][1]); /* 8c639dce */
r[4]-=4u;wr(ram,r[4],f->bank[bank][0]); /* 8c639dd0 */
f->fpscr^=0x100000u; /* 8c639dd2 */
move_pair(f,1,8); /* 8c639dd4 */
move_pair(f,3,10); /* 8c639dd6 */
move_pair(f,5,12); /* 8c639dd8 */
move_pair(f,7,14); /* 8c639dda */
f->fpscr^=0x100000u; /* 8c639ddc */
f->bank[bank][15]=rd(ram,r[15]);r[15]+=4u; /* 8c639dde */
f->bank[bank][14]=rd(ram,r[15]);r[15]+=4u; /* 8c639de0 */
f->bank[bank][13]=rd(ram,r[15]);r[15]+=4u; /* 8c639de2 */
f->bank[bank][12]=rd(ram,r[15]);r[15]+=4u; /* 8c639de4 */
}
*sr=(*sr&~1u)|(r[6]==0);*fpul=r[6];if(r[6]){
f->bank[bank][3]=0; /* 8c639dec */
status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status; /* 8c639dee */
f->bank[bank][5]=0; /* 8c639df0 */
f->bank[bank][7]=0; /* 8c639df2 */
move_pair(f,2,0); /* 8c639df4 */
move_pair(f,0,1); /* 8c639df6 */
f->bank[bank][1]=0; /* 8c639df8 */
move_pair(f,6,0); /* 8c639dfa */
move_pair(f,4,2); /* 8c639dfc */
f->bank[bank][2]^=0x80000000u; /* 8c639dfe */
r[15]-=4u;wr(ram,r[15],f->bank[bank][12]); /* 8c639e00 */
r[15]-=4u;wr(ram,r[15],f->bank[bank][13]); /* 8c639e02 */
r[15]-=4u;wr(ram,r[15],f->bank[bank][14]); /* 8c639e04 */
r[15]-=4u;wr(ram,r[15],f->bank[bank][15]); /* 8c639e06 */
f->fpscr^=0x100000u; /* 8c639e08 */
move_pair(f,8,1); /* 8c639e0a */
move_pair(f,10,3); /* 8c639e0c */
move_pair(f,12,9); /* 8c639e0e */
move_pair(f,14,11); /* 8c639e10 */
f->fpscr^=0x100000u; /* 8c639e12 */
f->fpscr^=0x200000u;bank=(f->fpscr>>21)&1u; /* 8c639e14 */
f->bank[bank][0]=rd(ram,r[4]);r[4]+=4u; /* 8c639e16 */
f->bank[bank][1]=rd(ram,r[4]);r[4]+=4u; /* 8c639e18 */
f->bank[bank][2]=rd(ram,r[4]);r[4]+=4u; /* 8c639e1a */
f->bank[bank][3]=rd(ram,r[4]);r[4]+=4u; /* 8c639e1c */
r[4]+=16; /* 8c639e1e */
f->bank[bank][8]=rd(ram,r[4]);r[4]+=4u; /* 8c639e20 */
f->bank[bank][9]=rd(ram,r[4]);r[4]+=4u; /* 8c639e22 */
f->bank[bank][10]=rd(ram,r[4]);r[4]+=4u; /* 8c639e24 */
f->bank[bank][11]=rd(ram,r[4]);r[4]+=4u; /* 8c639e26 */
f->fpscr^=0x200000u;bank=(f->fpscr>>21)&1u; /* 8c639e28 */
status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status; /* 8c639e2a */
status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status; /* 8c639e2c */
r[4]-=4u;wr(ram,r[4],f->bank[bank][7]); /* 8c639e2e */
r[4]-=4u;wr(ram,r[4],f->bank[bank][6]); /* 8c639e30 */
r[4]-=4u;wr(ram,r[4],f->bank[bank][5]); /* 8c639e32 */
r[4]-=4u;wr(ram,r[4],f->bank[bank][4]); /* 8c639e34 */
r[4]+=-16; /* 8c639e36 */
r[4]-=4u;wr(ram,r[4],f->bank[bank][3]); /* 8c639e38 */
r[4]-=4u;wr(ram,r[4],f->bank[bank][2]); /* 8c639e3a */
r[4]-=4u;wr(ram,r[4],f->bank[bank][1]); /* 8c639e3c */
r[4]-=4u;wr(ram,r[4],f->bank[bank][0]); /* 8c639e3e */
f->fpscr^=0x100000u; /* 8c639e40 */
move_pair(f,1,8); /* 8c639e42 */
move_pair(f,3,10); /* 8c639e44 */
move_pair(f,9,12); /* 8c639e46 */
move_pair(f,11,14); /* 8c639e48 */
f->fpscr^=0x100000u; /* 8c639e4a */
f->bank[bank][15]=rd(ram,r[15]);r[15]+=4u; /* 8c639e4c */
f->bank[bank][14]=rd(ram,r[15]);r[15]+=4u; /* 8c639e4e */
f->bank[bank][13]=rd(ram,r[15]);r[15]+=4u; /* 8c639e50 */
f->bank[bank][12]=rd(ram,r[15]);r[15]+=4u; /* 8c639e52 */
}
*sr=(*sr&~1u)|(r[5]==0);*fpul=r[5];if(r[5]){
f->bank[bank][3]=0; /* 8c639e5a */
status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status; /* 8c639e5c */
f->bank[bank][4]=0; /* 8c639e5e */
f->bank[bank][7]=0; /* 8c639e60 */
move_pair(f,2,0); /* 8c639e62 */
f->bank[bank][0]=0; /* 8c639e64 */
move_pair(f,5,2); /* 8c639e66 */
move_pair(f,6,1); /* 8c639e68 */
f->bank[bank][5]^=0x80000000u; /* 8c639e6a */
r[15]-=4u;wr(ram,r[15],f->bank[bank][12]); /* 8c639e6c */
r[15]-=4u;wr(ram,r[15],f->bank[bank][13]); /* 8c639e6e */
r[15]-=4u;wr(ram,r[15],f->bank[bank][14]); /* 8c639e70 */
r[15]-=4u;wr(ram,r[15],f->bank[bank][15]); /* 8c639e72 */
f->fpscr^=0x100000u; /* 8c639e74 */
move_pair(f,8,5); /* 8c639e76 */
move_pair(f,10,7); /* 8c639e78 */
move_pair(f,12,9); /* 8c639e7a */
move_pair(f,14,11); /* 8c639e7c */
f->fpscr^=0x100000u; /* 8c639e7e */
f->fpscr^=0x200000u;bank=(f->fpscr>>21)&1u; /* 8c639e80 */
r[4]+=16; /* 8c639e82 */
f->bank[bank][4]=rd(ram,r[4]);r[4]+=4u; /* 8c639e84 */
f->bank[bank][5]=rd(ram,r[4]);r[4]+=4u; /* 8c639e86 */
f->bank[bank][6]=rd(ram,r[4]);r[4]+=4u; /* 8c639e88 */
f->bank[bank][7]=rd(ram,r[4]);r[4]+=4u; /* 8c639e8a */
f->bank[bank][8]=rd(ram,r[4]);r[4]+=4u; /* 8c639e8c */
f->bank[bank][9]=rd(ram,r[4]);r[4]+=4u; /* 8c639e8e */
f->bank[bank][10]=rd(ram,r[4]);r[4]+=4u; /* 8c639e90 */
f->bank[bank][11]=rd(ram,r[4]);r[4]+=4u; /* 8c639e92 */
f->fpscr^=0x200000u;bank=(f->fpscr>>21)&1u; /* 8c639e94 */
status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status; /* 8c639e96 */
status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status; /* 8c639e98 */
r[4]-=4u;wr(ram,r[4],f->bank[bank][7]); /* 8c639e9a */
r[4]-=4u;wr(ram,r[4],f->bank[bank][6]); /* 8c639e9c */
r[4]-=4u;wr(ram,r[4],f->bank[bank][5]); /* 8c639e9e */
r[4]-=4u;wr(ram,r[4],f->bank[bank][4]); /* 8c639ea0 */
r[4]-=4u;wr(ram,r[4],f->bank[bank][3]); /* 8c639ea2 */
r[4]-=4u;wr(ram,r[4],f->bank[bank][2]); /* 8c639ea4 */
r[4]-=4u;wr(ram,r[4],f->bank[bank][1]); /* 8c639ea6 */
r[4]-=4u;wr(ram,r[4],f->bank[bank][0]); /* 8c639ea8 */
f->fpscr^=0x100000u; /* 8c639eaa */
move_pair(f,5,8); /* 8c639eac */
move_pair(f,7,10); /* 8c639eae */
move_pair(f,9,12); /* 8c639eb0 */
move_pair(f,11,14); /* 8c639eb2 */
f->fpscr^=0x100000u; /* 8c639eb4 */
f->bank[bank][15]=rd(ram,r[15]);r[15]+=4u; /* 8c639eb6 */
f->bank[bank][14]=rd(ram,r[15]);r[15]+=4u; /* 8c639eb8 */
f->bank[bank][13]=rd(ram,r[15]);r[15]+=4u; /* 8c639eba */
f->bank[bank][12]=rd(ram,r[15]);r[15]+=4u; /* 8c639ebc */
}
return 0;
}

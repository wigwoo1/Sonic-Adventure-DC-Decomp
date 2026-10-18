#include "fpu_cleanup_transfer.h"
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_model_matrix_determinant(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_matrix_inverse_singular(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)ram[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
static void move_pair(struct sa_fpu_banks *f,unsigned n,unsigned m){
 unsigned bank=(f->fpscr>>21)&1u;
 if(!(f->fpscr&0x100000u)){f->bank[bank][n]=f->bank[bank][m];return;}
 unsigned source=bank^(m&1u),dest=bank^(n&1u);m&=~1u;n&=~1u;uint32_t a=f->bank[source][m],b=f->bank[source][m+1u];f->bank[dest][n]=a;f->bank[dest][n+1u]=b;
}
/* Original8c6390b0..8c6398ae. Native determinant, explicit/implicit inverse,
 * operation order/register aliases/scratch preserved. Finite PR0/SZ0 scope;
 * unsupported arithmetic returns status at first operation, not SH4 trap ABI. */
SA_STAGE_EXPORT int sa_model_matrix_inverse(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,uint32_t incoming_pr){
 if(f->fpscr&0x180000u)return -1;unsigned bank=(f->fpscr>>21)&1u;int status;uint32_t next;
 r[15]-=4u;wr(ram,r[15],r[14]);r[3]=0x8c64f3ecu;r[15]-=4u;wr(ram,r[15],incoming_pr);r[14]=r[4];
 status=sa_model_matrix_determinant(ram,f,sr,r);if(status)return status;
 status=sa_model_matrix_inverse_singular(ram,f,sr,r,&next);if(status)return status;
 if(next==0x8c639112u){
  f->bank[bank][1]=0x3f800000u;status=sa_sh4_divide(&f->bank[bank][1],&f->bank[bank][0],&f->fpscr,&f->bank[bank][1]);if(status)return status;
  r[15]-=4u;wr(ram,r[15],f->bank[bank][12]);r[15]-=4u;wr(ram,r[15],f->bank[bank][13]);*sr=(*sr&~1u)|(r[14]==0);*fpul=f->bank[bank][1];r[15]-=4u;wr(ram,r[15],f->bank[bank][14]);
  if(!r[14]){
 f->fpscr^=0x100000u; /* 8c639126 */
 move_pair(f,0u,9u); /* 8c639128 */
 move_pair(f,2u,11u); /* 8c63912a */
 move_pair(f,4u,13u); /* 8c63912c */
 move_pair(f,6u,15u); /* 8c63912e */
 move_pair(f,8u,5u); /* 8c639130 */
 move_pair(f,10u,7u); /* 8c639132 */
 f->fpscr^=0x100000u; /* 8c639134 */
 move_pair(f,0u,1u); /* 8c639136 */
 move_pair(f,4u,2u); /* 8c639138 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][5],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63913a */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][7],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c63913c */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][3],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c63913e */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][6],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c639140 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][7],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c639142 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][6],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c639144 */
 move_pair(f,6u,9u); /* 8c639146 */
 move_pair(f,7u,10u); /* 8c639148 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][4],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c63914a */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][5],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c63914c */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][0],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c63914e */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][7],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c639150 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][3],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c639152 */
 move_pair(f,7u,11u); /* 8c639154 */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][1],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c639156 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][2],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c639158 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][11],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c63915a */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][7],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c63915c */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][10],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c63915e */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][9],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c639160 */
 f->fpscr^=0x100000u; /* 8c639162 */
 move_pair(f,8u,1u); /* 8c639164 */
 move_pair(f,10u,3u); /* 8c639166 */
 f->fpscr^=0x100000u; /* 8c639168 */
 move_pair(f,8u,9u); /* 8c63916a */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][10],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c63916c */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][4],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c63916e */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][11],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c639170 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][10],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c639172 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][11],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c639174 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][3],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c639176 */
 status=sa_sh4_add_sub(&f->bank[bank][5],&f->bank[bank][8],&f->fpscr,&f->bank[bank][5],0);if(status)return status; /* 8c639178 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][5],&f->fpscr,&f->bank[bank][1],0);if(status)return status; /* 8c63917a */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][2],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c63917c */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][0],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c63917e */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][9],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c639180 */
 move_pair(f,0u,6u); /* 8c639182 */
 f->bank[bank][1]^=0x80000000u; /* 8c639184 */
 f->fpscr^=0x100000u; /* 8c639186 */
 move_pair(f,6u,9u); /* 8c639188 */
 move_pair(f,8u,11u); /* 8c63918a */
 move_pair(f,10u,13u); /* 8c63918c */
 move_pair(f,12u,15u); /* 8c63918e */
 f->fpscr^=0x100000u; /* 8c639190 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][10],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c639192 */
 status=sa_sh4_multiply(&f->bank[bank][12],&f->bank[bank][6],&f->fpscr,&f->bank[bank][12]);if(status)return status; /* 8c639194 */
 move_pair(f,2u,9u); /* 8c639196 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][10],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c639198 */
 move_pair(f,5u,12u); /* 8c63919a */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][13],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c63919c */
 move_pair(f,7u,8u); /* 8c63919e */
 f->fpscr^=0x100000u; /* 8c6391a0 */
 move_pair(f,8u,5u); /* 8c6391a2 */
 move_pair(f,10u,7u); /* 8c6391a4 */
 move_pair(f,12u,7u); /* 8c6391a6 */
 f->fpscr^=0x100000u; /* 8c6391a8 */
 move_pair(f,9u,8u); /* 8c6391aa */
 status=sa_sh4_multiply(&f->bank[bank][12],&f->bank[bank][2],&f->fpscr,&f->bank[bank][12]);if(status)return status; /* 8c6391ac */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][4],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c6391ae */
 status=sa_sh4_multiply(&f->bank[bank][13],&f->bank[bank][5],&f->fpscr,&f->bank[bank][13]);if(status)return status; /* 8c6391b0 */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][7],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c6391b2 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][6],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c6391b4 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][3],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c6391b6 */
 status=sa_sh4_add_sub(&f->bank[bank][9],&f->bank[bank][12],&f->fpscr,&f->bank[bank][9],0);if(status)return status; /* 8c6391b8 */
 status=sa_sh4_add_sub(&f->bank[bank][9],&f->bank[bank][13],&f->fpscr,&f->bank[bank][9],0);if(status)return status; /* 8c6391ba */
 status=sa_sh4_add_sub(&f->bank[bank][9],&f->bank[bank][11],&f->fpscr,&f->bank[bank][9],1);if(status)return status; /* 8c6391bc */
 status=sa_sh4_add_sub(&f->bank[bank][9],&f->bank[bank][10],&f->fpscr,&f->bank[bank][9],1);if(status)return status; /* 8c6391be */
 status=sa_sh4_add_sub(&f->bank[bank][9],&f->bank[bank][8],&f->fpscr,&f->bank[bank][9],1);if(status)return status; /* 8c6391c0 */
 f->fpscr^=0x100000u; /* 8c6391c2 */
 move_pair(f,10u,1u); /* 8c6391c4 */
 move_pair(f,12u,3u); /* 8c6391c6 */
 f->fpscr^=0x100000u; /* 8c6391c8 */
 f->bank[bank][9]^=0x80000000u; /* 8c6391ca */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][10],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c6391cc */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][12],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c6391ce */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][13],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c6391d0 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][12],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c6391d2 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][13],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c6391d4 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][10],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c6391d6 */
 status=sa_sh4_add_sub(&f->bank[bank][4],&f->bank[bank][2],&f->fpscr,&f->bank[bank][4],0);if(status)return status; /* 8c6391d8 */
 status=sa_sh4_add_sub(&f->bank[bank][4],&f->bank[bank][5],&f->fpscr,&f->bank[bank][4],0);if(status)return status; /* 8c6391da */
 f->bank[bank][3]^=0x80000000u; /* 8c6391dc */
 status=sa_sh4_add_sub(&f->bank[bank][4],&f->bank[bank][7],&f->fpscr,&f->bank[bank][4],1);if(status)return status; /* 8c6391de */
 move_pair(f,2u,9u); /* 8c6391e0 */
 status=sa_sh4_add_sub(&f->bank[bank][4],&f->bank[bank][6],&f->fpscr,&f->bank[bank][4],1);if(status)return status; /* 8c6391e2 */
 status=sa_sh4_add_sub(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3],0);if(status)return status; /* 8c6391e4 */
 f->bank[bank][4]=*fpul; /* 8c6391e6 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][4],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c6391e8 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][4],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c6391ea */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c6391ec */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c6391ee */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][0]); /* 8c6391f0 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][1]); /* 8c6391f2 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][2]); /* 8c6391f4 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][3]); /* 8c6391f6 */
 f->fpscr^=0x100000u; /* 8c6391f8 */
 move_pair(f,0u,9u); /* 8c6391fa */
 move_pair(f,2u,11u); /* 8c6391fc */
 move_pair(f,4u,13u); /* 8c6391fe */
 move_pair(f,6u,15u); /* 8c639200 */
 move_pair(f,8u,5u); /* 8c639202 */
 move_pair(f,10u,7u); /* 8c639204 */
 f->fpscr^=0x100000u; /* 8c639206 */
 move_pair(f,2u,1u); /* 8c639208 */
 move_pair(f,6u,5u); /* 8c63920a */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][7],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63920c */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][0],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c63920e */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][5],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c639210 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][3],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c639212 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c639214 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c639216 */
 move_pair(f,4u,7u); /* 8c639218 */
 move_pair(f,12u,11u); /* 8c63921a */
 move_pair(f,6u,8u); /* 8c63921c */
 move_pair(f,7u,9u); /* 8c63921e */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][1],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c639220 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][3],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c639222 */
 status=sa_sh4_multiply(&f->bank[bank][12],&f->bank[bank][0],&f->fpscr,&f->bank[bank][12]);if(status)return status; /* 8c639224 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][7],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c639226 */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][2],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c639228 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][12],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c63922a */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][4],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c63922c */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][11],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c63922e */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][5],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c639230 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][9],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c639232 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][8],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c639234 */
 f->fpscr^=0x100000u; /* 8c639236 */
 move_pair(f,8u,1u); /* 8c639238 */
 move_pair(f,10u,3u); /* 8c63923a */
 f->fpscr^=0x100000u; /* 8c63923c */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][8],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63923e */
 move_pair(f,10u,11u); /* 8c639240 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][9],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c639242 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][0],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c639244 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][3],&f->fpscr,&f->bank[bank][1],0);if(status)return status; /* 8c639246 */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][2],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c639248 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][10],&f->fpscr,&f->bank[bank][1],0);if(status)return status; /* 8c63924a */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][9],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c63924c */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][11],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c63924e */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][8],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c639250 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][4],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c639252 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][5],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c639254 */
 move_pair(f,3u,6u); /* 8c639256 */
 f->bank[bank][1]^=0x80000000u; /* 8c639258 */
 f->fpscr^=0x100000u; /* 8c63925a */
 move_pair(f,4u,9u); /* 8c63925c */
 move_pair(f,6u,11u); /* 8c63925e */
 move_pair(f,8u,13u); /* 8c639260 */
 move_pair(f,10u,15u); /* 8c639262 */
 f->fpscr^=0x100000u; /* 8c639264 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][10],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c639266 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][6],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c639268 */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][10],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c63926a */
 move_pair(f,7u,8u); /* 8c63926c */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][9],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c63926e */
 f->fpscr^=0x100000u; /* 8c639270 */
 move_pair(f,8u,5u); /* 8c639272 */
 move_pair(f,10u,7u); /* 8c639274 */
 f->fpscr^=0x100000u; /* 8c639276 */
 move_pair(f,11u,8u); /* 8c639278 */
 move_pair(f,12u,9u); /* 8c63927a */
 move_pair(f,13u,10u); /* 8c63927c */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][5],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c63927e */
 status=sa_sh4_multiply(&f->bank[bank][12],&f->bank[bank][7],&f->fpscr,&f->bank[bank][12]);if(status)return status; /* 8c639280 */
 status=sa_sh4_multiply(&f->bank[bank][13],&f->bank[bank][0],&f->fpscr,&f->bank[bank][13]);if(status)return status; /* 8c639282 */
 status=sa_sh4_add_sub(&f->bank[bank][12],&f->bank[bank][11],&f->fpscr,&f->bank[bank][12],0);if(status)return status; /* 8c639284 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][2],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c639286 */
 status=sa_sh4_add_sub(&f->bank[bank][12],&f->bank[bank][13],&f->fpscr,&f->bank[bank][12],0);if(status)return status; /* 8c639288 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][4],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c63928a */
 status=sa_sh4_add_sub(&f->bank[bank][12],&f->bank[bank][10],&f->fpscr,&f->bank[bank][12],1);if(status)return status; /* 8c63928c */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][6],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c63928e */
 status=sa_sh4_add_sub(&f->bank[bank][12],&f->bank[bank][9],&f->fpscr,&f->bank[bank][12],1);if(status)return status; /* 8c639290 */
 status=sa_sh4_add_sub(&f->bank[bank][12],&f->bank[bank][8],&f->fpscr,&f->bank[bank][12],1);if(status)return status; /* 8c639292 */
 f->bank[bank][12]^=0x80000000u; /* 8c639294 */
 f->fpscr^=0x100000u; /* 8c639296 */
 move_pair(f,8u,1u); /* 8c639298 */
 move_pair(f,10u,3u); /* 8c63929a */
 f->fpscr^=0x100000u; /* 8c63929c */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][8],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c63929e */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][9],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c6392a0 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][10],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c6392a2 */
 status=sa_sh4_add_sub(&f->bank[bank][5],&f->bank[bank][7],&f->fpscr,&f->bank[bank][5],0);if(status)return status; /* 8c6392a4 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][10],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c6392a6 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][5],&f->fpscr,&f->bank[bank][0],0);if(status)return status; /* 8c6392a8 */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][9],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c6392aa */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][2],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c6392ac */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][8],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c6392ae */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][4],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c6392b0 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][6],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c6392b2 */
 f->bank[bank][8]=*fpul; /* 8c6392b4 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][8],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c6392b6 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][8],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c6392b8 */
 status=sa_sh4_multiply(&f->bank[bank][12],&f->bank[bank][8],&f->fpscr,&f->bank[bank][12]);if(status)return status; /* 8c6392ba */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][8],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c6392bc */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][3]); /* 8c6392be */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][1]); /* 8c6392c0 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][12]); /* 8c6392c2 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][0]); /* 8c6392c4 */
 f->fpscr^=0x100000u; /* 8c6392c6 */
 move_pair(f,0u,1u); /* 8c6392c8 */
 move_pair(f,2u,3u); /* 8c6392ca */
 move_pair(f,4u,5u); /* 8c6392cc */
 move_pair(f,6u,7u); /* 8c6392ce */
 f->fpscr^=0x100000u; /* 8c6392d0 */
 move_pair(f,0u,1u); /* 8c6392d2 */
 move_pair(f,4u,7u); /* 8c6392d4 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][6],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c6392d6 */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][2],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c6392d8 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][7],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c6392da */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][5],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c6392dc */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][3],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c6392de */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][6],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c6392e0 */
 f->fpscr^=0x100000u; /* 8c6392e2 */
 move_pair(f,6u,13u); /* 8c6392e4 */
 move_pair(f,8u,15u); /* 8c6392e6 */
 f->fpscr^=0x100000u; /* 8c6392e8 */
 move_pair(f,6u,9u); /* 8c6392ea */
 move_pair(f,10u,7u); /* 8c6392ec */
 move_pair(f,11u,8u); /* 8c6392ee */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][0],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c6392f0 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][4],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c6392f2 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][5],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c6392f4 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][7],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c6392f6 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][3],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c6392f8 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][8],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c6392fa */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][2],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c6392fc */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][10],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6392fe */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][1],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c639300 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][9],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c639302 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][11],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c639304 */
 f->fpscr^=0x100000u; /* 8c639306 */
 move_pair(f,8u,9u); /* 8c639308 */
 move_pair(f,10u,11u); /* 8c63930a */
 f->fpscr^=0x100000u; /* 8c63930c */
 move_pair(f,8u,9u); /* 8c63930e */
 move_pair(f,12u,9u); /* 8c639310 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][11],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c639312 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][4],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c639314 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][10],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c639316 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][8],&f->fpscr,&f->bank[bank][0],0);if(status)return status; /* 8c639318 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][3],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c63931a */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][5],&f->fpscr,&f->bank[bank][0],0);if(status)return status; /* 8c63931c */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][11],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63931e */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][9],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c639320 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][10],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c639322 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][2],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c639324 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][1],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c639326 */
 move_pair(f,2u,4u); /* 8c639328 */
 move_pair(f,1u,6u); /* 8c63932a */
 f->bank[bank][0]^=0x80000000u; /* 8c63932c */
 f->fpscr^=0x100000u; /* 8c63932e */
 move_pair(f,4u,1u); /* 8c639330 */
 move_pair(f,6u,3u); /* 8c639332 */
 move_pair(f,8u,5u); /* 8c639334 */
 move_pair(f,10u,7u); /* 8c639336 */
 f->fpscr^=0x100000u; /* 8c639338 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][8],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c63933a */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][8],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c63933c */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][4],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c63933e */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][11],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c639340 */
 move_pair(f,5u,10u); /* 8c639342 */
 f->fpscr^=0x100000u; /* 8c639344 */
 move_pair(f,8u,13u); /* 8c639346 */
 move_pair(f,10u,15u); /* 8c639348 */
 move_pair(f,12u,15u); /* 8c63934a */
 f->fpscr^=0x100000u; /* 8c63934c */
 move_pair(f,9u,8u); /* 8c63934e */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][5],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c639350 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][2],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c639352 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][7],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c639354 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][11],&f->fpscr,&f->bank[bank][8],0);if(status)return status; /* 8c639356 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][3],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c639358 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][10],&f->fpscr,&f->bank[bank][8],0);if(status)return status; /* 8c63935a */
 status=sa_sh4_multiply(&f->bank[bank][13],&f->bank[bank][6],&f->fpscr,&f->bank[bank][13]);if(status)return status; /* 8c63935c */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][9],&f->fpscr,&f->bank[bank][8],1);if(status)return status; /* 8c63935e */
 status=sa_sh4_multiply(&f->bank[bank][12],&f->bank[bank][4],&f->fpscr,&f->bank[bank][12]);if(status)return status; /* 8c639360 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][13],&f->fpscr,&f->bank[bank][8],1);if(status)return status; /* 8c639362 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][12],&f->fpscr,&f->bank[bank][8],1);if(status)return status; /* 8c639364 */
 f->fpscr^=0x100000u; /* 8c639366 */
 move_pair(f,10u,9u); /* 8c639368 */
 move_pair(f,12u,11u); /* 8c63936a */
 f->fpscr^=0x100000u; /* 8c63936c */
 f->bank[bank][8]^=0x80000000u; /* 8c63936e */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][13],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c639370 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][10],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c639372 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][12],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c639374 */
 status=sa_sh4_add_sub(&f->bank[bank][2],&f->bank[bank][5],&f->fpscr,&f->bank[bank][2],0);if(status)return status; /* 8c639376 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][10],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c639378 */
 status=sa_sh4_add_sub(&f->bank[bank][2],&f->bank[bank][7],&f->fpscr,&f->bank[bank][2],0);if(status)return status; /* 8c63937a */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][13],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c63937c */
 status=sa_sh4_add_sub(&f->bank[bank][2],&f->bank[bank][3],&f->fpscr,&f->bank[bank][2],1);if(status)return status; /* 8c63937e */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][12],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c639380 */
 status=sa_sh4_add_sub(&f->bank[bank][2],&f->bank[bank][6],&f->fpscr,&f->bank[bank][2],1);if(status)return status; /* 8c639382 */
 move_pair(f,3u,8u); /* 8c639384 */
 status=sa_sh4_add_sub(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2],1);if(status)return status; /* 8c639386 */
 f->bank[bank][4]=*fpul; /* 8c639388 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][4],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63938a */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][4],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c63938c */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63938e */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c639390 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][1]); /* 8c639392 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][0]); /* 8c639394 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][3]); /* 8c639396 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][2]); /* 8c639398 */
 f->fpscr^=0x100000u; /* 8c63939a */
 move_pair(f,0u,1u); /* 8c63939c */
 move_pair(f,2u,3u); /* 8c63939e */
 move_pair(f,4u,5u); /* 8c6393a0 */
 move_pair(f,6u,7u); /* 8c6393a2 */
 move_pair(f,8u,1u); /* 8c6393a4 */
 f->fpscr^=0x100000u; /* 8c6393a6 */
 move_pair(f,2u,3u); /* 8c6393a8 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][7],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c6393aa */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][5],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c6393ac */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][7],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c6393ae */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c6393b0 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][5],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c6393b2 */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][9],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c6393b4 */
 move_pair(f,5u,8u); /* 8c6393b6 */
 f->fpscr^=0x100000u; /* 8c6393b8 */
 move_pair(f,6u,13u); /* 8c6393ba */
 move_pair(f,8u,15u); /* 8c6393bc */
 move_pair(f,10u,13u); /* 8c6393be */
 move_pair(f,12u,15u); /* 8c6393c0 */
 f->fpscr^=0x100000u; /* 8c6393c2 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][0],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c6393c4 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][1],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c6393c6 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][2],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c6393c8 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][9],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c6393ca */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][3],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c6393cc */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][7],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c6393ce */
 status=sa_sh4_multiply(&f->bank[bank][13],&f->bank[bank][4],&f->fpscr,&f->bank[bank][13]);if(status)return status; /* 8c6393d0 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][10],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6393d2 */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][5],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c6393d4 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][13],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6393d6 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][11],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6393d8 */
 f->fpscr^=0x100000u; /* 8c6393da */
 move_pair(f,8u,9u); /* 8c6393dc */
 move_pair(f,10u,11u); /* 8c6393de */
 f->fpscr^=0x100000u; /* 8c6393e0 */
 move_pair(f,10u,11u); /* 8c6393e2 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][8],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c6393e4 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][0],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c6393e6 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][9],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c6393e8 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][10],&f->fpscr,&f->bank[bank][1],0);if(status)return status; /* 8c6393ea */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][3],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c6393ec */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][2],&f->fpscr,&f->bank[bank][1],0);if(status)return status; /* 8c6393ee */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][4],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c6393f0 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][8],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c6393f2 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][9],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c6393f4 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][11],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c6393f6 */
 move_pair(f,3u,4u); /* 8c6393f8 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][5],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c6393fa */
 move_pair(f,2u,6u); /* 8c6393fc */
 f->bank[bank][1]^=0x80000000u; /* 8c6393fe */
 f->fpscr^=0x100000u; /* 8c639400 */
 move_pair(f,4u,1u); /* 8c639402 */
 move_pair(f,6u,3u); /* 8c639404 */
 move_pair(f,8u,5u); /* 8c639406 */
 move_pair(f,10u,7u); /* 8c639408 */
 f->fpscr^=0x100000u; /* 8c63940a */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][10],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c63940c */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][6],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c63940e */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][10],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c639410 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][9],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c639412 */
 move_pair(f,7u,8u); /* 8c639414 */
 f->fpscr^=0x100000u; /* 8c639416 */
 move_pair(f,8u,13u); /* 8c639418 */
 move_pair(f,10u,15u); /* 8c63941a */
 move_pair(f,12u,13u); /* 8c63941c */
 f->fpscr^=0x100000u; /* 8c63941e */
 move_pair(f,11u,10u); /* 8c639420 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][5],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c639422 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][0],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c639424 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][7],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c639426 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][10],&f->fpscr,&f->bank[bank][8],0);if(status)return status; /* 8c639428 */
 status=sa_sh4_multiply(&f->bank[bank][12],&f->bank[bank][6],&f->fpscr,&f->bank[bank][12]);if(status)return status; /* 8c63942a */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][9],&f->fpscr,&f->bank[bank][8],0);if(status)return status; /* 8c63942c */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][3],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c63942e */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][12],&f->fpscr,&f->bank[bank][8],1);if(status)return status; /* 8c639430 */
 status=sa_sh4_multiply(&f->bank[bank][13],&f->bank[bank][4],&f->fpscr,&f->bank[bank][13]);if(status)return status; /* 8c639432 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][11],&f->fpscr,&f->bank[bank][8],1);if(status)return status; /* 8c639434 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][13],&f->fpscr,&f->bank[bank][8],1);if(status)return status; /* 8c639436 */
 f->bank[bank][8]^=0x80000000u; /* 8c639438 */
 f->fpscr^=0x100000u; /* 8c63943a */
 move_pair(f,10u,9u); /* 8c63943c */
 move_pair(f,12u,11u); /* 8c63943e */
 f->fpscr^=0x100000u; /* 8c639440 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][12],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c639442 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][10],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c639444 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][11],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c639446 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][5],&f->fpscr,&f->bank[bank][0],0);if(status)return status; /* 8c639448 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][10],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c63944a */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][7],&f->fpscr,&f->bank[bank][0],0);if(status)return status; /* 8c63944c */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][12],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63944e */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][6],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c639450 */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][11],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c639452 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][3],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c639454 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][4],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c639456 */
 f->bank[bank][4]=*fpul; /* 8c639458 */
 move_pair(f,3u,1u); /* 8c63945a */
 move_pair(f,9u,0u); /* 8c63945c */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63945e */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c639460 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][4],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c639462 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][4],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c639464 */
 f->fpscr^=0x100000u; /* 8c639466 */
 move_pair(f,11u,2u); /* 8c639468 */
 move_pair(f,15u,8u); /* 8c63946a */
 f->fpscr^=0x100000u; /* 8c63946c */
 f->bank[bank][1]=rd(ram,r[15]);r[15]+=4u; /* 8c63946e */
 f->bank[bank][0]=rd(ram,r[15]);r[15]+=4u; /* 8c639470 */
 f->bank[bank][3]=rd(ram,r[15]);r[15]+=4u; /* 8c639472 */
 f->bank[bank][2]=rd(ram,r[15]);r[15]+=4u; /* 8c639474 */
 f->bank[bank][5]=rd(ram,r[15]);r[15]+=4u; /* 8c639476 */
 f->bank[bank][4]=rd(ram,r[15]);r[15]+=4u; /* 8c639478 */
 f->bank[bank][7]=rd(ram,r[15]);r[15]+=4u; /* 8c63947a */
 f->bank[bank][6]=rd(ram,r[15]);r[15]+=4u; /* 8c63947c */
 f->bank[bank][9]=rd(ram,r[15]);r[15]+=4u; /* 8c63947e */
 f->bank[bank][8]=rd(ram,r[15]);r[15]+=4u; /* 8c639480 */
 f->bank[bank][11]=rd(ram,r[15]);r[15]+=4u; /* 8c639482 */
 f->bank[bank][10]=rd(ram,r[15]);r[15]+=4u; /* 8c639484 */
 f->fpscr^=0x100000u; /* 8c639486 */
 move_pair(f,7u,0u); /* 8c639488 */
 move_pair(f,3u,2u); /* 8c63948a */
 move_pair(f,13u,4u); /* 8c63948c */
 move_pair(f,9u,6u); /* 8c63948e */
 move_pair(f,5u,8u); /* 8c639490 */
 move_pair(f,1u,10u); /* 8c639492 */
 f->fpscr^=0x100000u; /* 8c639494 */
 r[0]=1u; /* 8c639498 */
  }else{
 r[3]=r[14]; /* 8c63949a */
 r[2]=r[14]; /* 8c63949c */
 r[3]+=32u; /* 8c63949e */
 r[2]+=16u; /* 8c6394a0 */
 f->bank[bank][0]=rd(ram,r[3]);r[3]+=4u; /* 8c6394a2 */
 f->bank[bank][1]=rd(ram,r[3]);r[3]+=4u; /* 8c6394a4 */
 f->bank[bank][2]=rd(ram,r[3]);r[3]+=4u; /* 8c6394a6 */
 f->bank[bank][3]=rd(ram,r[3]);r[3]+=4u; /* 8c6394a8 */
 f->bank[bank][4]=rd(ram,r[3]);r[3]+=4u; /* 8c6394aa */
 f->bank[bank][5]=rd(ram,r[3]);r[3]+=4u; /* 8c6394ac */
 f->bank[bank][6]=rd(ram,r[3]);r[3]+=4u; /* 8c6394ae */
 f->bank[bank][7]=rd(ram,r[3]);r[3]+=4u; /* 8c6394b0 */
 f->bank[bank][8]=rd(ram,r[2]);r[2]+=4u; /* 8c6394b2 */
 f->bank[bank][9]=rd(ram,r[2]);r[2]+=4u; /* 8c6394b4 */
 f->bank[bank][10]=rd(ram,r[2]);r[2]+=4u; /* 8c6394b6 */
 f->bank[bank][11]=rd(ram,r[2]);r[2]+=4u; /* 8c6394b8 */
 move_pair(f,0u,1u); /* 8c6394ba */
 move_pair(f,4u,2u); /* 8c6394bc */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][5],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c6394be */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][7],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c6394c0 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][3],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c6394c2 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][6],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c6394c4 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][7],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c6394c6 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][6],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c6394c8 */
 move_pair(f,6u,9u); /* 8c6394ca */
 move_pair(f,7u,10u); /* 8c6394cc */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][4],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c6394ce */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][5],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c6394d0 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][0],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c6394d2 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][7],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c6394d4 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][3],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c6394d6 */
 move_pair(f,7u,11u); /* 8c6394d8 */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][1],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c6394da */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][2],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c6394dc */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][11],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c6394de */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][7],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6394e0 */
 r[5]=r[14]; /* 8c6394e2 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][10],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6394e4 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][9],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6394e6 */
 f->bank[bank][8]=rd(ram,r[5]);r[5]+=4u; /* 8c6394e8 */
 f->bank[bank][9]=rd(ram,r[5]);r[5]+=4u; /* 8c6394ea */
 f->bank[bank][10]=rd(ram,r[5]);r[5]+=4u; /* 8c6394ec */
 f->bank[bank][11]=rd(ram,r[5]);r[5]+=4u; /* 8c6394ee */
 move_pair(f,8u,9u); /* 8c6394f0 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][10],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c6394f2 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][4],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c6394f4 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][11],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c6394f6 */
 status=sa_sh4_add_sub(&f->bank[bank][5],&f->bank[bank][8],&f->fpscr,&f->bank[bank][5],0);if(status)return status; /* 8c6394f8 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][10],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c6394fa */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][5],&f->fpscr,&f->bank[bank][1],0);if(status)return status; /* 8c6394fc */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][11],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c6394fe */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][3],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c639500 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][2],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c639502 */
 r[7]=r[14]; /* 8c639504 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][0],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c639506 */
 r[7]+=16u; /* 8c639508 */
 r[6]=r[14]; /* 8c63950a */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][9],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c63950c */
 move_pair(f,0u,6u); /* 8c63950e */
 r[6]+=32u; /* 8c639510 */
 f->bank[bank][1]^=0x80000000u; /* 8c639512 */
 f->bank[bank][6]=rd(ram,r[6]);r[6]+=4u; /* 8c639514 */
 f->bank[bank][7]=rd(ram,r[6]);r[6]+=4u; /* 8c639516 */
 f->bank[bank][8]=rd(ram,r[6]);r[6]+=4u; /* 8c639518 */
 f->bank[bank][9]=rd(ram,r[6]);r[6]+=4u; /* 8c63951a */
 f->bank[bank][10]=rd(ram,r[6]);r[6]+=4u; /* 8c63951c */
 f->bank[bank][11]=rd(ram,r[6]);r[6]+=4u; /* 8c63951e */
 f->bank[bank][12]=rd(ram,r[6]);r[6]+=4u; /* 8c639520 */
 f->bank[bank][13]=rd(ram,r[6]);r[6]+=4u; /* 8c639522 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][10],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c639524 */
 status=sa_sh4_multiply(&f->bank[bank][12],&f->bank[bank][6],&f->fpscr,&f->bank[bank][12]);if(status)return status; /* 8c639526 */
 move_pair(f,2u,9u); /* 8c639528 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][10],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c63952a */
 move_pair(f,5u,12u); /* 8c63952c */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][13],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c63952e */
 move_pair(f,7u,8u); /* 8c639530 */
 f->bank[bank][8]=rd(ram,r[7]);r[7]+=4u; /* 8c639532 */
 f->bank[bank][9]=rd(ram,r[7]);r[7]+=4u; /* 8c639534 */
 f->bank[bank][10]=rd(ram,r[7]); /* 8c639536 */
 f->bank[bank][12]=rd(ram,r[7]);r[7]+=4u; /* 8c639538 */
 f->bank[bank][11]=rd(ram,r[7]); /* 8c63953a */
 f->bank[bank][13]=rd(ram,r[7]); /* 8c63953c */
 move_pair(f,9u,8u); /* 8c63953e */
 status=sa_sh4_multiply(&f->bank[bank][12],&f->bank[bank][2],&f->fpscr,&f->bank[bank][12]);if(status)return status; /* 8c639540 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][4],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c639542 */
 status=sa_sh4_multiply(&f->bank[bank][13],&f->bank[bank][5],&f->fpscr,&f->bank[bank][13]);if(status)return status; /* 8c639544 */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][7],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c639546 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][6],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c639548 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][3],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c63954a */
 status=sa_sh4_add_sub(&f->bank[bank][9],&f->bank[bank][12],&f->fpscr,&f->bank[bank][9],0);if(status)return status; /* 8c63954c */
 status=sa_sh4_add_sub(&f->bank[bank][9],&f->bank[bank][13],&f->fpscr,&f->bank[bank][9],0);if(status)return status; /* 8c63954e */
 status=sa_sh4_add_sub(&f->bank[bank][9],&f->bank[bank][11],&f->fpscr,&f->bank[bank][9],1);if(status)return status; /* 8c639550 */
 r[6]=r[14]; /* 8c639552 */
 status=sa_sh4_add_sub(&f->bank[bank][9],&f->bank[bank][10],&f->fpscr,&f->bank[bank][9],1);if(status)return status; /* 8c639554 */
 status=sa_sh4_add_sub(&f->bank[bank][9],&f->bank[bank][8],&f->fpscr,&f->bank[bank][9],1);if(status)return status; /* 8c639556 */
 f->bank[bank][10]=rd(ram,r[6]);r[6]+=4u; /* 8c639558 */
 f->bank[bank][11]=rd(ram,r[6]);r[6]+=4u; /* 8c63955a */
 f->bank[bank][12]=rd(ram,r[6]);r[6]+=4u; /* 8c63955c */
 f->bank[bank][13]=rd(ram,r[6]);r[6]+=4u; /* 8c63955e */
 f->bank[bank][9]^=0x80000000u; /* 8c639560 */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][10],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c639562 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][12],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c639564 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][13],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c639566 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][12],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c639568 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][13],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c63956a */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][10],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63956c */
 status=sa_sh4_add_sub(&f->bank[bank][4],&f->bank[bank][2],&f->fpscr,&f->bank[bank][4],0);if(status)return status; /* 8c63956e */
 status=sa_sh4_add_sub(&f->bank[bank][4],&f->bank[bank][5],&f->fpscr,&f->bank[bank][4],0);if(status)return status; /* 8c639570 */
 f->bank[bank][3]^=0x80000000u; /* 8c639572 */
 status=sa_sh4_add_sub(&f->bank[bank][4],&f->bank[bank][7],&f->fpscr,&f->bank[bank][4],1);if(status)return status; /* 8c639574 */
 move_pair(f,2u,9u); /* 8c639576 */
 status=sa_sh4_add_sub(&f->bank[bank][4],&f->bank[bank][6],&f->fpscr,&f->bank[bank][4],1);if(status)return status; /* 8c639578 */
 status=sa_sh4_add_sub(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3],0);if(status)return status; /* 8c63957a */
 f->bank[bank][4]=*fpul; /* 8c63957c */
 r[5]=r[14]; /* 8c63957e */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][4],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c639580 */
 r[6]=r[14]; /* 8c639582 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][4],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c639584 */
 r[5]+=32u; /* 8c639586 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c639588 */
 r[6]+=16u; /* 8c63958a */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63958c */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][0]); /* 8c63958e */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][1]); /* 8c639590 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][2]); /* 8c639592 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][3]); /* 8c639594 */
 f->bank[bank][0]=rd(ram,r[5]);r[5]+=4u; /* 8c639596 */
 f->bank[bank][1]=rd(ram,r[5]);r[5]+=4u; /* 8c639598 */
 f->bank[bank][2]=rd(ram,r[5]);r[5]+=4u; /* 8c63959a */
 f->bank[bank][3]=rd(ram,r[5]);r[5]+=4u; /* 8c63959c */
 f->bank[bank][4]=rd(ram,r[5]);r[5]+=4u; /* 8c63959e */
 f->bank[bank][5]=rd(ram,r[5]);r[5]+=4u; /* 8c6395a0 */
 f->bank[bank][6]=rd(ram,r[5]);r[5]+=4u; /* 8c6395a2 */
 f->bank[bank][7]=rd(ram,r[5]);r[5]+=4u; /* 8c6395a4 */
 f->bank[bank][8]=rd(ram,r[6]);r[6]+=4u; /* 8c6395a6 */
 f->bank[bank][9]=rd(ram,r[6]);r[6]+=4u; /* 8c6395a8 */
 f->bank[bank][10]=rd(ram,r[6]);r[6]+=4u; /* 8c6395aa */
 f->bank[bank][11]=rd(ram,r[6]);r[6]+=4u; /* 8c6395ac */
 move_pair(f,2u,1u); /* 8c6395ae */
 move_pair(f,6u,5u); /* 8c6395b0 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][7],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c6395b2 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][0],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c6395b4 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][5],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c6395b6 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][3],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c6395b8 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c6395ba */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c6395bc */
 move_pair(f,4u,7u); /* 8c6395be */
 move_pair(f,12u,11u); /* 8c6395c0 */
 move_pair(f,6u,8u); /* 8c6395c2 */
 move_pair(f,7u,9u); /* 8c6395c4 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][1],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c6395c6 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][3],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c6395c8 */
 status=sa_sh4_multiply(&f->bank[bank][12],&f->bank[bank][0],&f->fpscr,&f->bank[bank][12]);if(status)return status; /* 8c6395ca */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][7],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c6395cc */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][2],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c6395ce */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][12],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c6395d0 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][4],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c6395d2 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][11],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6395d4 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][5],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c6395d6 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][9],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6395d8 */
 r[5]=r[14]; /* 8c6395da */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][8],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6395dc */
 f->bank[bank][8]=rd(ram,r[5]);r[5]+=4u; /* 8c6395de */
 f->bank[bank][9]=rd(ram,r[5]);r[5]+=4u; /* 8c6395e0 */
 f->bank[bank][10]=rd(ram,r[5]);r[5]+=4u; /* 8c6395e2 */
 f->bank[bank][11]=rd(ram,r[5]);r[5]+=4u; /* 8c6395e4 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][8],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c6395e6 */
 move_pair(f,10u,11u); /* 8c6395e8 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][9],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c6395ea */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][0],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c6395ec */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][3],&f->fpscr,&f->bank[bank][1],0);if(status)return status; /* 8c6395ee */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][2],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c6395f0 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][10],&f->fpscr,&f->bank[bank][1],0);if(status)return status; /* 8c6395f2 */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][9],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c6395f4 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][11],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c6395f6 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][8],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c6395f8 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][4],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c6395fa */
 r[5]=r[14]; /* 8c6395fc */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][5],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c6395fe */
 r[5]+=32u; /* 8c639600 */
 move_pair(f,3u,6u); /* 8c639602 */
 r[6]=r[14]; /* 8c639604 */
 f->bank[bank][1]^=0x80000000u; /* 8c639606 */
 r[6]+=16u; /* 8c639608 */
 f->bank[bank][4]=rd(ram,r[5]);r[5]+=4u; /* 8c63960a */
 f->bank[bank][5]=rd(ram,r[5]);r[5]+=4u; /* 8c63960c */
 f->bank[bank][6]=rd(ram,r[5]);r[5]+=4u; /* 8c63960e */
 f->bank[bank][7]=rd(ram,r[5]);r[5]+=4u; /* 8c639610 */
 f->bank[bank][8]=rd(ram,r[5]);r[5]+=4u; /* 8c639612 */
 f->bank[bank][9]=rd(ram,r[5]);r[5]+=4u; /* 8c639614 */
 f->bank[bank][10]=rd(ram,r[5]);r[5]+=4u; /* 8c639616 */
 f->bank[bank][11]=rd(ram,r[5]);r[5]+=4u; /* 8c639618 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][10],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c63961a */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][6],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c63961c */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][10],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c63961e */
 move_pair(f,7u,8u); /* 8c639620 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][9],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c639622 */
 f->bank[bank][8]=rd(ram,r[6]);r[6]+=4u; /* 8c639624 */
 f->bank[bank][9]=rd(ram,r[6]);r[6]+=4u; /* 8c639626 */
 f->bank[bank][10]=rd(ram,r[6]);r[6]+=4u; /* 8c639628 */
 f->bank[bank][11]=rd(ram,r[6]);r[6]+=4u; /* 8c63962a */
 move_pair(f,11u,8u); /* 8c63962c */
 move_pair(f,12u,9u); /* 8c63962e */
 move_pair(f,13u,10u); /* 8c639630 */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][5],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c639632 */
 status=sa_sh4_multiply(&f->bank[bank][12],&f->bank[bank][7],&f->fpscr,&f->bank[bank][12]);if(status)return status; /* 8c639634 */
 status=sa_sh4_multiply(&f->bank[bank][13],&f->bank[bank][0],&f->fpscr,&f->bank[bank][13]);if(status)return status; /* 8c639636 */
 status=sa_sh4_add_sub(&f->bank[bank][12],&f->bank[bank][11],&f->fpscr,&f->bank[bank][12],0);if(status)return status; /* 8c639638 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][2],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c63963a */
 status=sa_sh4_add_sub(&f->bank[bank][12],&f->bank[bank][13],&f->fpscr,&f->bank[bank][12],0);if(status)return status; /* 8c63963c */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][4],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c63963e */
 status=sa_sh4_add_sub(&f->bank[bank][12],&f->bank[bank][10],&f->fpscr,&f->bank[bank][12],1);if(status)return status; /* 8c639640 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][6],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c639642 */
 status=sa_sh4_add_sub(&f->bank[bank][12],&f->bank[bank][9],&f->fpscr,&f->bank[bank][12],1);if(status)return status; /* 8c639644 */
 r[5]=r[14]; /* 8c639646 */
 status=sa_sh4_add_sub(&f->bank[bank][12],&f->bank[bank][8],&f->fpscr,&f->bank[bank][12],1);if(status)return status; /* 8c639648 */
 f->bank[bank][12]^=0x80000000u; /* 8c63964a */
 f->bank[bank][8]=rd(ram,r[5]);r[5]+=4u; /* 8c63964c */
 f->bank[bank][9]=rd(ram,r[5]);r[5]+=4u; /* 8c63964e */
 f->bank[bank][10]=rd(ram,r[5]);r[5]+=4u; /* 8c639650 */
 f->bank[bank][11]=rd(ram,r[5]);r[5]+=4u; /* 8c639652 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][8],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c639654 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][9],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c639656 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][10],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c639658 */
 status=sa_sh4_add_sub(&f->bank[bank][5],&f->bank[bank][7],&f->fpscr,&f->bank[bank][5],0);if(status)return status; /* 8c63965a */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][10],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63965c */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][5],&f->fpscr,&f->bank[bank][0],0);if(status)return status; /* 8c63965e */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][9],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c639660 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][2],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c639662 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][8],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c639664 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][4],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c639666 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][6],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c639668 */
 f->bank[bank][8]=*fpul; /* 8c63966a */
 r[5]=r[14]; /* 8c63966c */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][8],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63966e */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][8],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c639670 */
 status=sa_sh4_multiply(&f->bank[bank][12],&f->bank[bank][8],&f->fpscr,&f->bank[bank][12]);if(status)return status; /* 8c639672 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][8],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c639674 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][3]); /* 8c639676 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][1]); /* 8c639678 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][12]); /* 8c63967a */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][0]); /* 8c63967c */
 f->bank[bank][0]=rd(ram,r[5]);r[5]+=4u; /* 8c63967e */
 f->bank[bank][1]=rd(ram,r[5]);r[5]+=4u; /* 8c639680 */
 f->bank[bank][2]=rd(ram,r[5]);r[5]+=4u; /* 8c639682 */
 f->bank[bank][3]=rd(ram,r[5]);r[5]+=4u; /* 8c639684 */
 f->bank[bank][4]=rd(ram,r[5]);r[5]+=4u; /* 8c639686 */
 f->bank[bank][5]=rd(ram,r[5]);r[5]+=4u; /* 8c639688 */
 f->bank[bank][6]=rd(ram,r[5]);r[5]+=4u; /* 8c63968a */
 f->bank[bank][7]=rd(ram,r[5]);r[5]+=4u; /* 8c63968c */
 move_pair(f,0u,1u); /* 8c63968e */
 move_pair(f,4u,7u); /* 8c639690 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][6],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c639692 */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][2],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c639694 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][7],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c639696 */
 r[5]+=16u; /* 8c639698 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][5],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63969a */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][3],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c63969c */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][6],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63969e */
 f->bank[bank][6]=rd(ram,r[5]);r[5]+=4u; /* 8c6396a0 */
 f->bank[bank][7]=rd(ram,r[5]);r[5]+=4u; /* 8c6396a2 */
 f->bank[bank][8]=rd(ram,r[5]);r[5]+=4u; /* 8c6396a4 */
 f->bank[bank][9]=rd(ram,r[5]);r[5]+=4u; /* 8c6396a6 */
 move_pair(f,6u,9u); /* 8c6396a8 */
 move_pair(f,10u,7u); /* 8c6396aa */
 move_pair(f,11u,8u); /* 8c6396ac */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][0],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c6396ae */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][4],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c6396b0 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][5],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c6396b2 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][7],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c6396b4 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][3],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c6396b6 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][8],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c6396b8 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][2],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c6396ba */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][10],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6396bc */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][1],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c6396be */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][9],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6396c0 */
 r[5]=r[14]; /* 8c6396c2 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][11],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6396c4 */
 r[5]+=32u; /* 8c6396c6 */
 f->bank[bank][8]=rd(ram,r[5]);r[5]+=4u; /* 8c6396c8 */
 f->bank[bank][9]=rd(ram,r[5]);r[5]+=4u; /* 8c6396ca */
 f->bank[bank][10]=rd(ram,r[5]);r[5]+=4u; /* 8c6396cc */
 f->bank[bank][11]=rd(ram,r[5]);r[5]+=4u; /* 8c6396ce */
 move_pair(f,8u,9u); /* 8c6396d0 */
 move_pair(f,12u,9u); /* 8c6396d2 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][11],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c6396d4 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][4],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c6396d6 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][10],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c6396d8 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][8],&f->fpscr,&f->bank[bank][0],0);if(status)return status; /* 8c6396da */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][3],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c6396dc */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][5],&f->fpscr,&f->bank[bank][0],0);if(status)return status; /* 8c6396de */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][11],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c6396e0 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][9],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c6396e2 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][10],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c6396e4 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][2],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c6396e6 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][1],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c6396e8 */
 r[5]=r[14]; /* 8c6396ea */
 move_pair(f,2u,4u); /* 8c6396ec */
 move_pair(f,1u,6u); /* 8c6396ee */
 f->bank[bank][0]^=0x80000000u; /* 8c6396f0 */
 f->bank[bank][4]=rd(ram,r[5]);r[5]+=4u; /* 8c6396f2 */
 f->bank[bank][5]=rd(ram,r[5]);r[5]+=4u; /* 8c6396f4 */
 f->bank[bank][6]=rd(ram,r[5]);r[5]+=4u; /* 8c6396f6 */
 f->bank[bank][7]=rd(ram,r[5]);r[5]+=4u; /* 8c6396f8 */
 f->bank[bank][8]=rd(ram,r[5]);r[5]+=4u; /* 8c6396fa */
 f->bank[bank][9]=rd(ram,r[5]);r[5]+=4u; /* 8c6396fc */
 f->bank[bank][10]=rd(ram,r[5]);r[5]+=4u; /* 8c6396fe */
 f->bank[bank][11]=rd(ram,r[5]);r[5]+=4u; /* 8c639700 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][8],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c639702 */
 r[5]+=16u; /* 8c639704 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][8],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c639706 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][4],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c639708 */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][11],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c63970a */
 move_pair(f,5u,10u); /* 8c63970c */
 f->bank[bank][8]=rd(ram,r[5]);r[5]+=4u; /* 8c63970e */
 f->bank[bank][9]=rd(ram,r[5]);r[5]+=4u; /* 8c639710 */
 f->bank[bank][10]=rd(ram,r[5]); /* 8c639712 */
 f->bank[bank][12]=rd(ram,r[5]);r[5]+=4u; /* 8c639714 */
 f->bank[bank][11]=rd(ram,r[5]); /* 8c639716 */
 f->bank[bank][13]=rd(ram,r[5]); /* 8c639718 */
 move_pair(f,9u,8u); /* 8c63971a */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][5],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c63971c */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][2],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c63971e */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][7],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c639720 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][11],&f->fpscr,&f->bank[bank][8],0);if(status)return status; /* 8c639722 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][3],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c639724 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][10],&f->fpscr,&f->bank[bank][8],0);if(status)return status; /* 8c639726 */
 status=sa_sh4_multiply(&f->bank[bank][13],&f->bank[bank][6],&f->fpscr,&f->bank[bank][13]);if(status)return status; /* 8c639728 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][9],&f->fpscr,&f->bank[bank][8],1);if(status)return status; /* 8c63972a */
 status=sa_sh4_multiply(&f->bank[bank][12],&f->bank[bank][4],&f->fpscr,&f->bank[bank][12]);if(status)return status; /* 8c63972c */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][13],&f->fpscr,&f->bank[bank][8],1);if(status)return status; /* 8c63972e */
 r[5]=r[14]; /* 8c639730 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][12],&f->fpscr,&f->bank[bank][8],1);if(status)return status; /* 8c639732 */
 r[5]+=32u; /* 8c639734 */
 f->bank[bank][10]=rd(ram,r[5]);r[5]+=4u; /* 8c639736 */
 f->bank[bank][11]=rd(ram,r[5]);r[5]+=4u; /* 8c639738 */
 f->bank[bank][12]=rd(ram,r[5]);r[5]+=4u; /* 8c63973a */
 f->bank[bank][13]=rd(ram,r[5]);r[5]+=4u; /* 8c63973c */
 f->bank[bank][8]^=0x80000000u; /* 8c63973e */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][13],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c639740 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][10],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c639742 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][12],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c639744 */
 status=sa_sh4_add_sub(&f->bank[bank][2],&f->bank[bank][5],&f->fpscr,&f->bank[bank][2],0);if(status)return status; /* 8c639746 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][10],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c639748 */
 status=sa_sh4_add_sub(&f->bank[bank][2],&f->bank[bank][7],&f->fpscr,&f->bank[bank][2],0);if(status)return status; /* 8c63974a */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][13],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c63974c */
 status=sa_sh4_add_sub(&f->bank[bank][2],&f->bank[bank][3],&f->fpscr,&f->bank[bank][2],1);if(status)return status; /* 8c63974e */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][12],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c639750 */
 status=sa_sh4_add_sub(&f->bank[bank][2],&f->bank[bank][6],&f->fpscr,&f->bank[bank][2],1);if(status)return status; /* 8c639752 */
 move_pair(f,3u,8u); /* 8c639754 */
 status=sa_sh4_add_sub(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2],1);if(status)return status; /* 8c639756 */
 f->bank[bank][4]=*fpul; /* 8c639758 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][4],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63975a */
 r[5]=r[14]; /* 8c63975c */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][4],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c63975e */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c639760 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c639762 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][1]); /* 8c639764 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][0]); /* 8c639766 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][3]); /* 8c639768 */
 r[15]-=4u;wr(ram,r[15],f->bank[bank][2]); /* 8c63976a */
 f->bank[bank][8]=rd(ram,r[5]); /* 8c63976c */
 f->bank[bank][0]=rd(ram,r[5]);r[5]+=4u; /* 8c63976e */
 f->bank[bank][9]=rd(ram,r[5]); /* 8c639770 */
 f->bank[bank][1]=rd(ram,r[5]);r[5]+=4u; /* 8c639772 */
 f->bank[bank][2]=rd(ram,r[5]);r[5]+=4u; /* 8c639774 */
 f->bank[bank][3]=rd(ram,r[5]);r[5]+=4u; /* 8c639776 */
 f->bank[bank][4]=rd(ram,r[5]);r[5]+=4u; /* 8c639778 */
 f->bank[bank][5]=rd(ram,r[5]);r[5]+=4u; /* 8c63977a */
 f->bank[bank][6]=rd(ram,r[5]);r[5]+=4u; /* 8c63977c */
 f->bank[bank][7]=rd(ram,r[5]);r[5]+=4u; /* 8c63977e */
 move_pair(f,2u,3u); /* 8c639780 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][7],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c639782 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][5],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c639784 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][7],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c639786 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c639788 */
 r[5]+=16u; /* 8c63978a */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][5],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63978c */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][9],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c63978e */
 move_pair(f,5u,8u); /* 8c639790 */
 f->bank[bank][6]=rd(ram,r[5]); /* 8c639792 */
 f->bank[bank][10]=rd(ram,r[5]);r[5]+=4u; /* 8c639794 */
 f->bank[bank][7]=rd(ram,r[5]); /* 8c639796 */
 f->bank[bank][11]=rd(ram,r[5]);r[5]+=4u; /* 8c639798 */
 f->bank[bank][8]=rd(ram,r[5]); /* 8c63979a */
 f->bank[bank][12]=rd(ram,r[5]);r[5]+=4u; /* 8c63979c */
 f->bank[bank][9]=rd(ram,r[5]); /* 8c63979e */
 f->bank[bank][13]=rd(ram,r[5]);r[5]+=4u; /* 8c6397a0 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][0],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c6397a2 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][1],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c6397a4 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][2],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c6397a6 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][9],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c6397a8 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][3],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c6397aa */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][7],&f->fpscr,&f->bank[bank][6],0);if(status)return status; /* 8c6397ac */
 status=sa_sh4_multiply(&f->bank[bank][13],&f->bank[bank][4],&f->fpscr,&f->bank[bank][13]);if(status)return status; /* 8c6397ae */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][10],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6397b0 */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][5],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c6397b2 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][13],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6397b4 */
 r[5]-=32u; /* 8c6397b6 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][11],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c6397b8 */
 f->bank[bank][8]=rd(ram,r[5]);r[5]+=4u; /* 8c6397ba */
 f->bank[bank][9]=rd(ram,r[5]);r[5]+=4u; /* 8c6397bc */
 f->bank[bank][10]=rd(ram,r[5]);r[5]+=4u; /* 8c6397be */
 f->bank[bank][11]=rd(ram,r[5]);r[5]+=4u; /* 8c6397c0 */
 move_pair(f,10u,11u); /* 8c6397c2 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][8],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c6397c4 */
 r[5]=r[14]; /* 8c6397c6 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][0],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c6397c8 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][9],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c6397ca */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][10],&f->fpscr,&f->bank[bank][1],0);if(status)return status; /* 8c6397cc */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][3],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c6397ce */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][2],&f->fpscr,&f->bank[bank][1],0);if(status)return status; /* 8c6397d0 */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][4],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c6397d2 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][8],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c6397d4 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][9],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c6397d6 */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][11],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c6397d8 */
 move_pair(f,3u,4u); /* 8c6397da */
 status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][5],&f->fpscr,&f->bank[bank][1],1);if(status)return status; /* 8c6397dc */
 move_pair(f,2u,6u); /* 8c6397de */
 f->bank[bank][1]^=0x80000000u; /* 8c6397e0 */
 f->bank[bank][4]=rd(ram,r[5]);r[5]+=4u; /* 8c6397e2 */
 f->bank[bank][5]=rd(ram,r[5]);r[5]+=4u; /* 8c6397e4 */
 f->bank[bank][6]=rd(ram,r[5]);r[5]+=4u; /* 8c6397e6 */
 f->bank[bank][7]=rd(ram,r[5]);r[5]+=4u; /* 8c6397e8 */
 f->bank[bank][8]=rd(ram,r[5]);r[5]+=4u; /* 8c6397ea */
 f->bank[bank][9]=rd(ram,r[5]);r[5]+=4u; /* 8c6397ec */
 f->bank[bank][10]=rd(ram,r[5]);r[5]+=4u; /* 8c6397ee */
 f->bank[bank][11]=rd(ram,r[5]);r[5]+=4u; /* 8c6397f0 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][10],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c6397f2 */
 r[5]+=16u; /* 8c6397f4 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][6],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c6397f6 */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][10],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c6397f8 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][9],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c6397fa */
 move_pair(f,7u,8u); /* 8c6397fc */
 f->bank[bank][8]=rd(ram,r[5]); /* 8c6397fe */
 f->bank[bank][12]=rd(ram,r[5]);r[5]+=4u; /* 8c639800 */
 f->bank[bank][9]=rd(ram,r[5]); /* 8c639802 */
 f->bank[bank][13]=rd(ram,r[5]);r[5]+=4u; /* 8c639804 */
 f->bank[bank][10]=rd(ram,r[5]);r[5]+=4u; /* 8c639806 */
 f->bank[bank][11]=rd(ram,r[5]); /* 8c639808 */
 move_pair(f,11u,10u); /* 8c63980a */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][5],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c63980c */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][0],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c63980e */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][7],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c639810 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][10],&f->fpscr,&f->bank[bank][8],0);if(status)return status; /* 8c639812 */
 status=sa_sh4_multiply(&f->bank[bank][12],&f->bank[bank][6],&f->fpscr,&f->bank[bank][12]);if(status)return status; /* 8c639814 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][9],&f->fpscr,&f->bank[bank][8],0);if(status)return status; /* 8c639816 */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][3],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c639818 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][12],&f->fpscr,&f->bank[bank][8],1);if(status)return status; /* 8c63981a */
 status=sa_sh4_multiply(&f->bank[bank][13],&f->bank[bank][4],&f->fpscr,&f->bank[bank][13]);if(status)return status; /* 8c63981c */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][11],&f->fpscr,&f->bank[bank][8],1);if(status)return status; /* 8c63981e */
 r[5]=r[14]; /* 8c639820 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][13],&f->fpscr,&f->bank[bank][8],1);if(status)return status; /* 8c639822 */
 r[5]+=32u; /* 8c639824 */
 f->bank[bank][8]^=0x80000000u; /* 8c639826 */
 f->bank[bank][10]=rd(ram,r[5]);r[5]+=4u; /* 8c639828 */
 f->bank[bank][11]=rd(ram,r[5]);r[5]+=4u; /* 8c63982a */
 f->bank[bank][12]=rd(ram,r[5]);r[5]+=4u; /* 8c63982c */
 f->bank[bank][13]=rd(ram,r[5]);r[5]+=4u; /* 8c63982e */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][12],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c639830 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][10],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c639832 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][11],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c639834 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][5],&f->fpscr,&f->bank[bank][0],0);if(status)return status; /* 8c639836 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][10],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c639838 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][7],&f->fpscr,&f->bank[bank][0],0);if(status)return status; /* 8c63983a */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][12],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63983c */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][6],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c63983e */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][11],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c639840 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][3],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c639842 */
 r[5]=r[14]; /* 8c639844 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][4],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c639846 */
 f->bank[bank][4]=*fpul; /* 8c639848 */
 move_pair(f,3u,1u); /* 8c63984a */
 move_pair(f,9u,0u); /* 8c63984c */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63984e */
 r[0]=44u; /* 8c639850 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c639852 */
 r[6]=r[14]; /* 8c639854 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][4],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c639856 */
 r[6]+=64u; /* 8c639858 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][4],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c63985a */
 wr(ram,r[0]+r[5],f->bank[bank][3]); /* 8c63985c */
 r[6]-=4u;wr(ram,r[6],f->bank[bank][9]); /* 8c63985e */
 r[0]=40u; /* 8c639860 */
 r[6]-=4u;wr(ram,r[6],f->bank[bank][8]); /* 8c639862 */
 wr(ram,r[0]+r[5],f->bank[bank][2]); /* 8c639864 */
 f->bank[bank][1]=rd(ram,r[15]);r[15]+=4u; /* 8c639866 */
 r[5]=r[14]; /* 8c639868 */
 f->bank[bank][0]=rd(ram,r[15]);r[15]+=4u; /* 8c63986a */
 r[6]=r[14]; /* 8c63986c */
 f->bank[bank][3]=rd(ram,r[15]);r[15]+=4u; /* 8c63986e */
 r[6]+=56u; /* 8c639870 */
 f->bank[bank][2]=rd(ram,r[15]);r[15]+=4u; /* 8c639872 */
 r[5]+=40u; /* 8c639874 */
 f->bank[bank][5]=rd(ram,r[15]);r[15]+=4u; /* 8c639876 */
 f->bank[bank][4]=rd(ram,r[15]);r[15]+=4u; /* 8c639878 */
 f->bank[bank][7]=rd(ram,r[15]);r[15]+=4u; /* 8c63987a */
 f->bank[bank][6]=rd(ram,r[15]);r[15]+=4u; /* 8c63987c */
 f->bank[bank][9]=rd(ram,r[15]);r[15]+=4u; /* 8c63987e */
 f->bank[bank][8]=rd(ram,r[15]);r[15]+=4u; /* 8c639880 */
 f->bank[bank][11]=rd(ram,r[15]);r[15]+=4u; /* 8c639882 */
 f->bank[bank][10]=rd(ram,r[15]);r[15]+=4u; /* 8c639884 */
 r[7]=r[14]; /* 8c639886 */
 r[5]-=4u;wr(ram,r[5],f->bank[bank][7]); /* 8c639888 */
 r[5]-=4u;wr(ram,r[5],f->bank[bank][6]); /* 8c63988a */
 r[5]-=4u;wr(ram,r[5],f->bank[bank][1]); /* 8c63988c */
 r[5]-=4u;wr(ram,r[5],f->bank[bank][0]); /* 8c63988e */
 r[5]-=4u;wr(ram,r[5],f->bank[bank][9]); /* 8c639890 */
 r[5]-=4u;wr(ram,r[5],f->bank[bank][8]); /* 8c639892 */
 r[5]-=4u;wr(ram,r[5],f->bank[bank][3]); /* 8c639894 */
 r[5]-=4u;wr(ram,r[5],f->bank[bank][2]); /* 8c639896 */
 r[5]-=4u;wr(ram,r[5],f->bank[bank][11]); /* 8c639898 */
 r[5]-=4u;wr(ram,r[5],f->bank[bank][10]); /* 8c63989a */
 r[6]-=4u;wr(ram,r[6],f->bank[bank][5]); /* 8c63989c */
 r[6]-=4u;wr(ram,r[6],f->bank[bank][4]); /* 8c63989e */
 r[0]=rd(ram,0x8c6398b0u); /* 8c6398a0 */
  }
  f->bank[bank][14]=rd(ram,r[15]);r[15]+=4u;f->bank[bank][13]=rd(ram,r[15]);r[15]+=4u;f->bank[bank][12]=rd(ram,r[15]);r[15]+=4u;
 }
 r[15]+=4u;r[14]=rd(ram,r[15]);r[15]+=4u;return 0;
}

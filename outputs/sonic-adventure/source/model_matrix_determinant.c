#include "fpu_cleanup_transfer.h"
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void move_pair(struct sa_fpu_banks *f,unsigned n,unsigned m){
 unsigned bank=(f->fpscr>>21)&1u;
 if(!(f->fpscr&0x100000u)){f->bank[bank][n]=f->bank[bank][m];return;}
 unsigned source=bank^(m&1u),dest=bank^(n&1u);m&=~1u;n&=~1u;uint32_t a=f->bank[source][m],b=f->bank[source][m+1u];f->bank[dest][n]=a;f->bank[dest][n+1u]=b;
}
/* Original8c64f3ec..8c64f544 scalar determinant. Separate rounded operations,
 * original register aliases and matrix loads retained; PR0/SZ0 entry. */
SA_STAGE_EXPORT int sa_model_matrix_determinant(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r){
 if(f->fpscr&0x180000u)return -1;unsigned bank=(f->fpscr>>21)&1u;int status;
 *sr=(*sr&~1u)|(r[4]==0);
 if(!r[4]){
 f->fpscr^=0x100000u; /* 8c64f3f0 */
 move_pair(f,0u,9u); /* 8c64f3f2 */
 move_pair(f,2u,11u); /* 8c64f3f4 */
 move_pair(f,4u,13u); /* 8c64f3f6 */
 move_pair(f,6u,15u); /* 8c64f3f8 */
 move_pair(f,8u,9u); /* 8c64f3fa */
 move_pair(f,10u,11u); /* 8c64f3fc */
 f->fpscr^=0x100000u; /* 8c64f3fe */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][7],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c64f400 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][6],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c64f402 */
 status=sa_sh4_add_sub(&f->bank[bank][10],&f->bank[bank][3],&f->fpscr,&f->bank[bank][10],1);if(status)return status; /* 8c64f404 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][6],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c64f406 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][5],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c64f408 */
 status=sa_sh4_add_sub(&f->bank[bank][9],&f->bank[bank][2],&f->fpscr,&f->bank[bank][9],1);if(status)return status; /* 8c64f40a */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][4],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c64f40c */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][7],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c64f40e */
 status=sa_sh4_add_sub(&f->bank[bank][11],&f->bank[bank][0],&f->fpscr,&f->bank[bank][11],1);if(status)return status; /* 8c64f410 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][5],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c64f412 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][4],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c64f414 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][1],&f->fpscr,&f->bank[bank][8],1);if(status)return status; /* 8c64f416 */
 f->fpscr^=0x100000u; /* 8c64f418 */
 move_pair(f,0u,9u); /* 8c64f41a */
 move_pair(f,2u,11u); /* 8c64f41c */
 move_pair(f,4u,13u); /* 8c64f41e */
 move_pair(f,6u,15u); /* 8c64f420 */
 f->fpscr^=0x100000u; /* 8c64f422 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][3],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c64f424 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][0],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c64f426 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][1],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c64f428 */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][2],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c64f42a */
 move_pair(f,0u,5u); /* 8c64f42c */
 move_pair(f,1u,6u); /* 8c64f42e */
 move_pair(f,2u,7u); /* 8c64f430 */
 move_pair(f,3u,4u); /* 8c64f432 */
 status=sa_sh4_add_sub(&f->bank[bank][5],&f->bank[bank][2],&f->fpscr,&f->bank[bank][5],1);if(status)return status; /* 8c64f434 */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][3],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c64f436 */
 status=sa_sh4_add_sub(&f->bank[bank][7],&f->bank[bank][0],&f->fpscr,&f->bank[bank][7],1);if(status)return status; /* 8c64f438 */
 status=sa_sh4_add_sub(&f->bank[bank][4],&f->bank[bank][1],&f->fpscr,&f->bank[bank][4],1);if(status)return status; /* 8c64f43a */
 f->fpscr^=0x100000u; /* 8c64f43c */
 move_pair(f,0u,5u); /* 8c64f43e */
 move_pair(f,2u,5u); /* 8c64f440 */
 f->fpscr^=0x100000u; /* 8c64f442 */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][2],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c64f444 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][10],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c64f446 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][9],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c64f448 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][3],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c64f44a */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][11],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c64f44c */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c64f44e */
 status=sa_sh4_add_sub(&f->bank[bank][7],&f->bank[bank][1],&f->fpscr,&f->bank[bank][7],0);if(status)return status; /* 8c64f450 */
 status=sa_sh4_add_sub(&f->bank[bank][3],&f->bank[bank][0],&f->fpscr,&f->bank[bank][3],0);if(status)return status; /* 8c64f452 */
 f->fpscr^=0x100000u; /* 8c64f454 */
 move_pair(f,0u,7u); /* 8c64f456 */
 f->fpscr^=0x100000u; /* 8c64f458 */
 move_pair(f,1u,0u); /* 8c64f45a */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][1],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c64f45c */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][0],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c64f45e */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][8],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c64f460 */
 status=sa_sh4_add_sub(&f->bank[bank][5],&f->bank[bank][10],&f->fpscr,&f->bank[bank][5],0);if(status)return status; /* 8c64f462 */
 status=sa_sh4_add_sub(&f->bank[bank][2],&f->bank[bank][11],&f->fpscr,&f->bank[bank][2],0);if(status)return status; /* 8c64f464 */
 status=sa_sh4_add_sub(&f->bank[bank][3],&f->bank[bank][1],&f->fpscr,&f->bank[bank][3],0);if(status)return status; /* 8c64f466 */
 f->fpscr^=0x100000u; /* 8c64f468 */
 move_pair(f,0u,7u); /* 8c64f46a */
 f->fpscr^=0x100000u; /* 8c64f46c */
 move_pair(f,0u,1u); /* 8c64f46e */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][0],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c64f470 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][1],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c64f472 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][8],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c64f474 */
 status=sa_sh4_add_sub(&f->bank[bank][5],&f->bank[bank][9],&f->fpscr,&f->bank[bank][5],0);if(status)return status; /* 8c64f476 */
 status=sa_sh4_add_sub(&f->bank[bank][2],&f->bank[bank][6],&f->fpscr,&f->bank[bank][2],0);if(status)return status; /* 8c64f478 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][7],&f->fpscr,&f->bank[bank][0],0);if(status)return status; /* 8c64f47a */
 f->fpscr^=0x100000u; /* 8c64f47c */
 move_pair(f,6u,1u); /* 8c64f47e */
 move_pair(f,8u,3u); /* 8c64f480 */
 f->fpscr^=0x100000u; /* 8c64f482 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][6],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c64f484 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][7],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c64f486 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][8],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c64f488 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][9],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c64f48a */
 status=sa_sh4_add_sub(&f->bank[bank][5],&f->bank[bank][2],&f->fpscr,&f->bank[bank][5],1);if(status)return status; /* 8c64f48c */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][3],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c64f48e */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][5],&f->fpscr,&f->bank[bank][0],0);if(status)return status; /* 8c64f492 */
 return 0;
 }
 r[1]=r[4]; /* 8c64f494 */
 r[1]+=32u; /* 8c64f496 */
 f->bank[bank][0]=rd(ram,r[1]);r[1]+=4u; /* 8c64f498 */
 f->bank[bank][1]=rd(ram,r[1]);r[1]+=4u; /* 8c64f49a */
 f->bank[bank][2]=rd(ram,r[1]);r[1]+=4u; /* 8c64f49c */
 f->bank[bank][3]=rd(ram,r[1]);r[1]+=4u; /* 8c64f49e */
 f->bank[bank][4]=rd(ram,r[1]);r[1]+=4u; /* 8c64f4a0 */
 f->bank[bank][5]=rd(ram,r[1]);r[1]+=4u; /* 8c64f4a2 */
 f->bank[bank][6]=rd(ram,r[1]);r[1]+=4u; /* 8c64f4a4 */
 f->bank[bank][7]=rd(ram,r[1]);r[1]+=4u; /* 8c64f4a6 */
 move_pair(f,8u,0u); /* 8c64f4a8 */
 move_pair(f,9u,1u); /* 8c64f4aa */
 move_pair(f,10u,2u); /* 8c64f4ac */
 move_pair(f,11u,3u); /* 8c64f4ae */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][7],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c64f4b0 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][6],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c64f4b2 */
 status=sa_sh4_add_sub(&f->bank[bank][10],&f->bank[bank][3],&f->fpscr,&f->bank[bank][10],1);if(status)return status; /* 8c64f4b4 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][6],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c64f4b6 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][5],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c64f4b8 */
 status=sa_sh4_add_sub(&f->bank[bank][9],&f->bank[bank][2],&f->fpscr,&f->bank[bank][9],1);if(status)return status; /* 8c64f4ba */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][4],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c64f4bc */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][7],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c64f4be */
 status=sa_sh4_add_sub(&f->bank[bank][11],&f->bank[bank][0],&f->fpscr,&f->bank[bank][11],1);if(status)return status; /* 8c64f4c0 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][5],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c64f4c2 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][4],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c64f4c4 */
 status=sa_sh4_add_sub(&f->bank[bank][8],&f->bank[bank][1],&f->fpscr,&f->bank[bank][8],1);if(status)return status; /* 8c64f4c6 */
 r[1]-=32u; /* 8c64f4c8 */
 f->bank[bank][0]=rd(ram,r[1]);r[1]+=4u; /* 8c64f4ca */
 f->bank[bank][1]=rd(ram,r[1]);r[1]+=4u; /* 8c64f4cc */
 f->bank[bank][2]=rd(ram,r[1]);r[1]+=4u; /* 8c64f4ce */
 f->bank[bank][3]=rd(ram,r[1]);r[1]+=4u; /* 8c64f4d0 */
 f->bank[bank][4]=rd(ram,r[1]);r[1]+=4u; /* 8c64f4d2 */
 f->bank[bank][5]=rd(ram,r[1]);r[1]+=4u; /* 8c64f4d4 */
 f->bank[bank][6]=rd(ram,r[1]);r[1]+=4u; /* 8c64f4d6 */
 f->bank[bank][7]=rd(ram,r[1]);r[1]+=4u; /* 8c64f4d8 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][3],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c64f4da */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][0],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c64f4dc */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][1],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c64f4de */
 status=sa_sh4_multiply(&f->bank[bank][4],&f->bank[bank][2],&f->fpscr,&f->bank[bank][4]);if(status)return status; /* 8c64f4e0 */
 move_pair(f,0u,5u); /* 8c64f4e2 */
 move_pair(f,1u,6u); /* 8c64f4e4 */
 move_pair(f,2u,7u); /* 8c64f4e6 */
 move_pair(f,3u,4u); /* 8c64f4e8 */
 status=sa_sh4_add_sub(&f->bank[bank][5],&f->bank[bank][2],&f->fpscr,&f->bank[bank][5],1);if(status)return status; /* 8c64f4ea */
 status=sa_sh4_add_sub(&f->bank[bank][6],&f->bank[bank][3],&f->fpscr,&f->bank[bank][6],1);if(status)return status; /* 8c64f4ec */
 status=sa_sh4_add_sub(&f->bank[bank][7],&f->bank[bank][0],&f->fpscr,&f->bank[bank][7],1);if(status)return status; /* 8c64f4ee */
 status=sa_sh4_add_sub(&f->bank[bank][4],&f->bank[bank][1],&f->fpscr,&f->bank[bank][4],1);if(status)return status; /* 8c64f4f0 */
 r[1]-=48u; /* 8c64f4f2 */
 f->bank[bank][0]=rd(ram,r[1]);r[1]+=4u; /* 8c64f4f4 */
 f->bank[bank][1]=rd(ram,r[1]);r[1]+=4u; /* 8c64f4f6 */
 move_pair(f,2u,0u); /* 8c64f4f8 */
 move_pair(f,3u,1u); /* 8c64f4fa */
 status=sa_sh4_multiply(&f->bank[bank][7],&f->bank[bank][2],&f->fpscr,&f->bank[bank][7]);if(status)return status; /* 8c64f4fc */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][10],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c64f4fe */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][9],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c64f500 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][3],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c64f502 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][11],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c64f504 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c64f506 */
 status=sa_sh4_add_sub(&f->bank[bank][7],&f->bank[bank][1],&f->fpscr,&f->bank[bank][7],0);if(status)return status; /* 8c64f508 */
 status=sa_sh4_add_sub(&f->bank[bank][3],&f->bank[bank][0],&f->fpscr,&f->bank[bank][3],0);if(status)return status; /* 8c64f50a */
 f->bank[bank][0]=rd(ram,r[1]);r[1]+=4u; /* 8c64f50c */
 move_pair(f,1u,0u); /* 8c64f50e */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][1],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c64f510 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][0],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c64f512 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][8],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c64f514 */
 status=sa_sh4_add_sub(&f->bank[bank][5],&f->bank[bank][10],&f->fpscr,&f->bank[bank][5],0);if(status)return status; /* 8c64f516 */
 status=sa_sh4_add_sub(&f->bank[bank][2],&f->bank[bank][11],&f->fpscr,&f->bank[bank][2],0);if(status)return status; /* 8c64f518 */
 status=sa_sh4_add_sub(&f->bank[bank][3],&f->bank[bank][1],&f->fpscr,&f->bank[bank][3],0);if(status)return status; /* 8c64f51a */
 f->bank[bank][0]=rd(ram,r[1]); /* 8c64f51c */
 move_pair(f,1u,0u); /* 8c64f51e */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][0],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c64f520 */
 status=sa_sh4_multiply(&f->bank[bank][6],&f->bank[bank][1],&f->fpscr,&f->bank[bank][6]);if(status)return status; /* 8c64f522 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][8],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c64f524 */
 status=sa_sh4_add_sub(&f->bank[bank][5],&f->bank[bank][9],&f->fpscr,&f->bank[bank][5],0);if(status)return status; /* 8c64f526 */
 status=sa_sh4_add_sub(&f->bank[bank][2],&f->bank[bank][6],&f->fpscr,&f->bank[bank][2],0);if(status)return status; /* 8c64f528 */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][7],&f->fpscr,&f->bank[bank][0],0);if(status)return status; /* 8c64f52a */
 f->bank[bank][6]=rd(ram,r[4]);r[4]+=4u; /* 8c64f52c */
 f->bank[bank][7]=rd(ram,r[4]);r[4]+=4u; /* 8c64f52e */
 f->bank[bank][8]=rd(ram,r[4]);r[4]+=4u; /* 8c64f530 */
 f->bank[bank][9]=rd(ram,r[4]);r[4]+=4u; /* 8c64f532 */
 status=sa_sh4_multiply(&f->bank[bank][5],&f->bank[bank][6],&f->fpscr,&f->bank[bank][5]);if(status)return status; /* 8c64f534 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][7],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c64f536 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][8],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c64f538 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][9],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c64f53a */
 status=sa_sh4_add_sub(&f->bank[bank][5],&f->bank[bank][2],&f->fpscr,&f->bank[bank][5],1);if(status)return status; /* 8c64f53c */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][3],&f->fpscr,&f->bank[bank][0],1);if(status)return status; /* 8c64f53e */
 status=sa_sh4_add_sub(&f->bank[bank][0],&f->bank[bank][5],&f->fpscr,&f->bank[bank][0],0);if(status)return status; /* 8c64f542 */
 return 0;
}

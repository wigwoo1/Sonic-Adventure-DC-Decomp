#include "fpu_cleanup_transfer.h"
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)ram[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
static void move_pair(struct sa_fpu_banks *f,unsigned n,unsigned m){
 unsigned bank=(f->fpscr>>21)&1u;
 if(!(f->fpscr&0x100000u)){f->bank[bank][n]=f->bank[bank][m];return;}
 unsigned source=bank^(m&1u),dest=bank^(n&1u);m&=~1u;n&=~1u;uint32_t a=f->bank[source][m],b=f->bank[source][m+1u];f->bank[dest][n]=a;f->bank[dest][n+1u]=b;
}
/* Original8c63a69c..8c63a698 matrix scale, PR0/SZ0 entry. */
SA_STAGE_EXPORT int sa_model_matrix_scale_registers(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r){
 if(f->fpscr&0x180000u)return -1;unsigned bank=(f->fpscr>>21)&1u;int status;
 *sr=(*sr&~1u)|(r[4]==0);

 if(!r[4]){
 f->fpscr^=0x100000u; /* 8c63a6a0 */
 move_pair(f,0u,1u); /* 8c63a6a2 */
 move_pair(f,2u,3u); /* 8c63a6a4 */
 move_pair(f,8u,5u); /* 8c63a6a6 */
 move_pair(f,10u,7u); /* 8c63a6a8 */
 f->fpscr^=0x100000u; /* 8c63a6aa */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][4],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c63a6ac */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][4],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63a6ae */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63a6b0 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63a6b2 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][5],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c63a6b4 */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][5],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c63a6b6 */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][5],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c63a6b8 */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][5],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c63a6ba */
 f->fpscr^=0x100000u; /* 8c63a6bc */
 move_pair(f,1u,0u); /* 8c63a6be */
 move_pair(f,3u,2u); /* 8c63a6c0 */
 move_pair(f,5u,8u); /* 8c63a6c2 */
 move_pair(f,7u,10u); /* 8c63a6c4 */
 f->fpscr^=0x100000u; /* 8c63a6c6 */
 f->fpscr^=0x100000u; /* 8c63a6c8 */
 move_pair(f,0u,9u); /* 8c63a6ca */
 move_pair(f,2u,11u); /* 8c63a6cc */
 f->fpscr^=0x100000u; /* 8c63a6ce */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][6],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c63a6d0 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][6],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63a6d2 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][6],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63a6d4 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][6],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63a6d6 */
 f->fpscr^=0x100000u; /* 8c63a6d8 */
 move_pair(f,9u,0u); /* 8c63a6da */
 move_pair(f,11u,2u); /* 8c63a6dc */
 f->fpscr^=0x100000u; /* 8c63a6e0 */
 return 0;
 }
 r[5]=r[4]; /* 8c63a6e2 */
 f->bank[bank][3]=rd(ram,r[4]);r[4]+=4u; /* 8c63a6e4 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63a6e6 */
 f->bank[bank][2]=rd(ram,r[4]);r[4]+=4u; /* 8c63a6e8 */
 r[0]=4u; /* 8c63a6ea */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63a6ec */
 wr(ram,r[5],f->bank[bank][3]); /* 8c63a6ee */
 f->bank[bank][1]=rd(ram,r[4]);r[4]+=4u; /* 8c63a6f0 */
 wr(ram,r[0]+r[5],f->bank[bank][2]); /* 8c63a6f2 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][4],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63a6f4 */
 f->bank[bank][3]=rd(ram,r[4]);r[4]+=4u; /* 8c63a6f6 */
 r[0]=8u; /* 8c63a6f8 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63a6fa */
 wr(ram,r[0]+r[5],f->bank[bank][1]); /* 8c63a6fc */
 f->bank[bank][2]=rd(ram,r[4]);r[4]+=4u; /* 8c63a6fe */
 r[0]=12u; /* 8c63a700 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][5],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63a702 */
 wr(ram,r[0]+r[5],f->bank[bank][3]); /* 8c63a704 */
 f->bank[bank][1]=rd(ram,r[4]);r[4]+=4u; /* 8c63a706 */
 r[0]=16u; /* 8c63a708 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][5],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63a70a */
 wr(ram,r[0]+r[5],f->bank[bank][2]); /* 8c63a70c */
 f->bank[bank][3]=rd(ram,r[4]);r[4]+=4u; /* 8c63a70e */
 r[0]=20u; /* 8c63a710 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][5],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63a712 */
 wr(ram,r[0]+r[5],f->bank[bank][1]); /* 8c63a714 */
 f->bank[bank][2]=rd(ram,r[4]);r[4]+=4u; /* 8c63a716 */
 r[0]=24u; /* 8c63a718 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][5],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63a71a */
 wr(ram,r[0]+r[5],f->bank[bank][3]); /* 8c63a71c */
 f->bank[bank][1]=rd(ram,r[4]);r[4]+=4u; /* 8c63a71e */
 r[0]=28u; /* 8c63a720 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][6],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63a722 */
 wr(ram,r[0]+r[5],f->bank[bank][2]); /* 8c63a724 */
 f->bank[bank][3]=rd(ram,r[4]);r[4]+=4u; /* 8c63a726 */
 r[0]=32u; /* 8c63a728 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][6],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63a72a */
 wr(ram,r[0]+r[5],f->bank[bank][1]); /* 8c63a72c */
 f->bank[bank][2]=rd(ram,r[4]);r[4]+=4u; /* 8c63a72e */
 r[0]=36u; /* 8c63a730 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][6],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63a732 */
 wr(ram,r[0]+r[5],f->bank[bank][3]); /* 8c63a734 */
 f->bank[bank][1]=rd(ram,r[4]); /* 8c63a736 */
 r[0]=40u; /* 8c63a738 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][6],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63a73a */
 wr(ram,r[0]+r[5],f->bank[bank][2]); /* 8c63a73c */
 wr(ram,r[4],f->bank[bank][1]); /* 8c63a740 */
 return 0;
}

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
/* Original8c63a5ec..8c63a698 matrix scale, PR0/SZ0 entry. */
SA_STAGE_EXPORT int sa_model_matrix_scale(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r){
 if(f->fpscr&0x180000u)return -1;unsigned bank=(f->fpscr>>21)&1u;int status;
 *sr=(*sr&~1u)|(r[4]==0);
 f->bank[bank][4]=rd(ram,r[5]);r[5]+=4u; /* 8c63a5ee */
 f->bank[bank][5]=rd(ram,r[5]);r[5]+=4u; /* 8c63a5f0 */ f->bank[bank][6]=rd(ram,r[5]);r[5]+=4u; /* 8c63a5f4 */
 if(!r[4]){
 f->fpscr^=0x100000u; /* 8c63a5f6 */
 move_pair(f,0u,1u); /* 8c63a5f8 */
 move_pair(f,2u,3u); /* 8c63a5fa */
 move_pair(f,8u,5u); /* 8c63a5fc */
 move_pair(f,10u,7u); /* 8c63a5fe */
 f->fpscr^=0x100000u; /* 8c63a600 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][4],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c63a602 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][4],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63a604 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63a606 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63a608 */
 status=sa_sh4_multiply(&f->bank[bank][8],&f->bank[bank][5],&f->fpscr,&f->bank[bank][8]);if(status)return status; /* 8c63a60a */
 status=sa_sh4_multiply(&f->bank[bank][9],&f->bank[bank][5],&f->fpscr,&f->bank[bank][9]);if(status)return status; /* 8c63a60c */
 status=sa_sh4_multiply(&f->bank[bank][10],&f->bank[bank][5],&f->fpscr,&f->bank[bank][10]);if(status)return status; /* 8c63a60e */
 status=sa_sh4_multiply(&f->bank[bank][11],&f->bank[bank][5],&f->fpscr,&f->bank[bank][11]);if(status)return status; /* 8c63a610 */
 f->fpscr^=0x100000u; /* 8c63a612 */
 move_pair(f,1u,0u); /* 8c63a614 */
 move_pair(f,3u,2u); /* 8c63a616 */
 move_pair(f,5u,8u); /* 8c63a618 */
 move_pair(f,7u,10u); /* 8c63a61a */
 f->fpscr^=0x100000u; /* 8c63a61c */
 f->fpscr^=0x100000u; /* 8c63a61e */
 move_pair(f,0u,9u); /* 8c63a620 */
 move_pair(f,2u,11u); /* 8c63a622 */
 f->fpscr^=0x100000u; /* 8c63a624 */
 status=sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][6],&f->fpscr,&f->bank[bank][0]);if(status)return status; /* 8c63a626 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][6],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63a628 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][6],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63a62a */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][6],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63a62c */
 f->fpscr^=0x100000u; /* 8c63a62e */
 move_pair(f,9u,0u); /* 8c63a630 */
 move_pair(f,11u,2u); /* 8c63a632 */
 f->fpscr^=0x100000u; /* 8c63a636 */
 return 0;
 }
 r[5]=r[4]; /* 8c63a638 */
 f->bank[bank][3]=rd(ram,r[4]);r[4]+=4u; /* 8c63a63a */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63a63c */
 f->bank[bank][2]=rd(ram,r[4]);r[4]+=4u; /* 8c63a63e */
 r[0]=4u; /* 8c63a640 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][4],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63a642 */
 wr(ram,r[5],f->bank[bank][3]); /* 8c63a644 */
 f->bank[bank][1]=rd(ram,r[4]);r[4]+=4u; /* 8c63a646 */
 wr(ram,r[0]+r[5],f->bank[bank][2]); /* 8c63a648 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][4],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63a64a */
 f->bank[bank][3]=rd(ram,r[4]);r[4]+=4u; /* 8c63a64c */
 r[0]=8u; /* 8c63a64e */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][4],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63a650 */
 wr(ram,r[0]+r[5],f->bank[bank][1]); /* 8c63a652 */
 f->bank[bank][2]=rd(ram,r[4]);r[4]+=4u; /* 8c63a654 */
 r[0]=12u; /* 8c63a656 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][5],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63a658 */
 wr(ram,r[0]+r[5],f->bank[bank][3]); /* 8c63a65a */
 f->bank[bank][1]=rd(ram,r[4]);r[4]+=4u; /* 8c63a65c */
 r[0]=16u; /* 8c63a65e */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][5],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63a660 */
 wr(ram,r[0]+r[5],f->bank[bank][2]); /* 8c63a662 */
 f->bank[bank][3]=rd(ram,r[4]);r[4]+=4u; /* 8c63a664 */
 r[0]=20u; /* 8c63a666 */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][5],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63a668 */
 wr(ram,r[0]+r[5],f->bank[bank][1]); /* 8c63a66a */
 f->bank[bank][2]=rd(ram,r[4]);r[4]+=4u; /* 8c63a66c */
 r[0]=24u; /* 8c63a66e */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][5],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63a670 */
 wr(ram,r[0]+r[5],f->bank[bank][3]); /* 8c63a672 */
 f->bank[bank][1]=rd(ram,r[4]);r[4]+=4u; /* 8c63a674 */
 r[0]=28u; /* 8c63a676 */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][6],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63a678 */
 wr(ram,r[0]+r[5],f->bank[bank][2]); /* 8c63a67a */
 f->bank[bank][3]=rd(ram,r[4]);r[4]+=4u; /* 8c63a67c */
 r[0]=32u; /* 8c63a67e */
 status=sa_sh4_multiply(&f->bank[bank][3],&f->bank[bank][6],&f->fpscr,&f->bank[bank][3]);if(status)return status; /* 8c63a680 */
 wr(ram,r[0]+r[5],f->bank[bank][1]); /* 8c63a682 */
 f->bank[bank][2]=rd(ram,r[4]);r[4]+=4u; /* 8c63a684 */
 r[0]=36u; /* 8c63a686 */
 status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][6],&f->fpscr,&f->bank[bank][2]);if(status)return status; /* 8c63a688 */
 wr(ram,r[0]+r[5],f->bank[bank][3]); /* 8c63a68a */
 f->bank[bank][1]=rd(ram,r[4]); /* 8c63a68c */
 r[0]=40u; /* 8c63a68e */
 status=sa_sh4_multiply(&f->bank[bank][1],&f->bank[bank][6],&f->fpscr,&f->bank[bank][1]);if(status)return status; /* 8c63a690 */
 wr(ram,r[0]+r[5],f->bank[bank][2]); /* 8c63a692 */
 wr(ram,r[4],f->bank[bank][1]); /* 8c63a696 */
 return 0;
}

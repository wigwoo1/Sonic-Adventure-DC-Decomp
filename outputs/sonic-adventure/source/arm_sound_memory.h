#ifndef SA_ARM_SOUND_MEMORY_H
#define SA_ARM_SOUND_MEMORY_H
/* Private helpers; component defines sa_arm_read/sa_arm_write first.
 * ARM DDI 0100I A2.8: pre-ARMv6 word bus accesses truncate low address bits;
 * single LDR rotates the aligned word. Byte accesses remain byte-addressed.
 * Current recovered block transfers use word-aligned stack addresses. */
static inline int load(uint8_t *ram,void *user,sa_arm_read read,uint32_t a,unsigned n,uint32_t *v){
 unsigned shift=n==4?(a&3u)*8u:0;
 if(n==4)a&=~3u;
 int result=0;
 if(a<=0x200000u-n){*v=0;for(unsigned i=0;i<n;i++)*v|=(uint32_t)ram[a+i]<<(8*i);}
 else result=read(user,a,n,v);
 if(!result&&shift)*v=(*v>>shift)|(*v<<(32u-shift));
 return result;
}
static inline int store(uint8_t *ram,void *user,sa_arm_write write,uint32_t a,uint32_t v,unsigned n){
 if(n==4)a&=~3u;
 if(a<=0x200000u-n){for(unsigned i=0;i<n;i++)ram[a+i]=(uint8_t)(v>>(8*i));return 0;}
 return write(user,a,v,n);
}
#endif

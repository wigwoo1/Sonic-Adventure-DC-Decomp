/* Original 0x8c04b82e..0x8c04b867; mode-aware native arena recovery.
 * Returns 0 if FLDI0's PR=0 precondition is violated, after list callbacks;
 * exception delivery for that undefined operation is not implemented. */
#include "resource_list_release.h"
static uint32_t read_word(uint8_t *p) {
 return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
static void write_word(uint8_t *p,uint32_t value) {
 for(unsigned i=0;i<4;i++)p[i]=(uint8_t)(value>>(8*i));
}
SA_STAGE_EXPORT int sa_resource_list_release_8c04b82e(uint8_t *arena,uint32_t owner,
 struct sa_fpu_banks *fpu,void *context,void (*free_node)(void *,uint32_t)) {
 uint32_t resource=read_word(arena+owner+60);
 uint32_t node=read_word(arena+resource+76);
 while(node) {
  uint32_t next=read_word(arena+node+12);
  free_node(context,node);
  node=next;
 }
 uint8_t transfer[8];
 if(!sa_fpu_cleanup_transfer(fpu,transfer))return 0;
 resource=read_word(arena+owner+60);
 write_word(arena+resource+76,0);
 resource=read_word(arena+owner+60);
 unsigned size=(fpu->fpscr & 0x100000u)?8:4;
 for(unsigned i=0;i<size;i++)arena[resource+72+i]=transfer[i];
 return 1;
}
/* Original 0x8c04b48c..0x8c04b4d1. Keeps its initial resource across callbacks. */
SA_STAGE_EXPORT int sa_resource_list_release_8c04b48c(uint8_t *arena,uint32_t owner,
 struct sa_fpu_banks *fpu,void *context,void (*free_node)(void *,uint32_t)) {
 uint32_t resource=read_word(arena+owner+60);
 uint32_t node=read_word(arena+resource+64);
 while(node) {
  uint32_t next=read_word(arena+node+16);
  free_node(context,node);
  node=next;
 }
 write_word(arena+resource+64,0);
 uint32_t extra=read_word(arena+resource+68);
 if(extra) {
  free_node(context,extra);
  write_word(arena+resource+68,0);
 }
 return sa_fpu_cleanup_transfer(fpu,arena+resource+60);
}
SA_STAGE_EXPORT int sa_secondary_resource_release_in_arena(uint8_t *arena,uint32_t owner,
 struct sa_fpu_banks *fpu,void *context,void (*free_node)(void *,uint32_t)) {
 if(read_word(arena+owner+60)) {
  if(!sa_resource_list_release_8c04b48c(arena,owner,fpu,context,free_node))return 0;
  if(!sa_resource_list_release_8c04b82e(arena,owner,fpu,context,free_node))return 0;
  free_node(context,read_word(arena+owner+60));
  write_word(arena+owner+60,0);
 }
 return 1;
}

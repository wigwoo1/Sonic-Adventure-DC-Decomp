/* Recovered first-fit heap allocation 0x8c098a80..0x8c098b0b.
 * Arena offsets replace retail pointers; header size and 32-bit arithmetic remain.
 * Caller supplies initialized, four-byte aligned valid block metadata.
 */
#include "task_heap.h"
static volatile uint32_t *header(struct sa_task_heap *heap,uint32_t offset) {
 return (volatile uint32_t *)(heap->arena+offset);
}
/* Original fixed-size initializer, including terminal free-list sentinel. */
SA_STAGE_EXPORT void sa_task_heap_initialize_8c098a10(struct sa_task_heap *heap) {
 heap->free_head=16;
 volatile uint8_t *bytes=heap->arena+16;
 for(uint32_t i=0;i<0x90000u;i++)bytes[i]=255;
 volatile uint32_t *first=header(heap,16),*terminal=header(heap,0x90010u);
 first[0]=0x90010u;first[1]=0;first[2]=0x90010u;first[3]=0;
 terminal[0]=0;terminal[1]=16;terminal[2]=0;terminal[3]=16;
 heap->field_8c79f040=0;
}
SA_STAGE_EXPORT void *sa_task_heap_allocate_8c098a80(struct sa_task_heap *heap,uint32_t size) {
 if(!size)return 0;
 uint32_t block=heap->free_head;
 if(!block || !header(heap,block)[2])return 0;
 uint32_t total=(size+31u)&~15u;
 while(block && header(heap,block)[2]) {
  volatile uint32_t *current=header(heap,block);
  uint32_t available=current[2]-block;
  if(available==total) {
   uint32_t next=current[0],previous=current[1];
   if(next)header(heap,next)[1]=previous;
   if(previous)header(heap,previous)[0]=next;
   else heap->free_head=next;
  } else if(available>total) {
   uint32_t previous=current[1],split=block+total,next=current[0];
   volatile uint32_t *replacement=header(heap,split);
   replacement[0]=next;
   replacement[1]=previous;
   replacement[2]=current[2];
   replacement[3]=block;
   header(heap,current[2])[3]=split;
   current[2]=split;
   if(next)header(heap,next)[1]=split;
   if(previous)header(heap,previous)[0]=split;
   else heap->free_head=split;
  } else { block=current[0];continue; }
  current[0]=0;current[1]=0;
  return heap->arena+block+16;
 }
 return 0;
}
/* Original free/coalescing routine 0x8c098b42..0x8c098c2b. */
SA_STAGE_EXPORT void sa_task_heap_free_8c098b42(struct sa_task_heap *heap,void *pointer) {
 if(!pointer)return;
 uint32_t block=(uint32_t)((uint8_t *)pointer-heap->arena)-16u;
 volatile uint32_t *current=header(heap,block);
 if(current[0] && current[1]) { for(;;) { (void)current[0]; } }
 uint32_t previous=current[3],next=current[2];
 if(previous && (header(heap,previous)[0] || header(heap,previous)[1])) {
  if((header(heap,next)[0] || header(heap,next)[1]) && header(heap,next)[2]) {
   header(heap,next)[3]=previous;
   header(heap,previous)[2]=next;
   uint32_t free_next=header(heap,next)[0],free_previous=header(heap,next)[1];
   if(free_next)header(heap,free_next)[1]=free_previous;
   if(free_previous)header(heap,free_previous)[0]=free_next;
   else heap->free_head=free_next;
   header(heap,header(heap,next)[2])[3]=previous;
   header(heap,previous)[2]=header(heap,next)[2];
  } else {
   header(heap,next)[3]=previous;
   header(heap,previous)[2]=next;
  }
  return;
 }
 if((header(heap,next)[0] || header(heap,next)[1]) && header(heap,next)[2]) {
  uint32_t free_next=header(heap,next)[0],free_previous=header(heap,next)[1];
  current[0]=free_next;current[1]=free_previous;
  if(free_next)header(heap,header(heap,next)[0])[1]=block;
  if(free_previous)header(heap,header(heap,next)[1])[0]=block;
  else heap->free_head=block;
  current[2]=header(heap,next)[2];
  header(heap,header(heap,next)[2])[3]=block;
  return;
 }
 uint32_t head=heap->free_head;
 heap->free_head=block;
 if(head)header(heap,head)[1]=block;
 current[0]=head;current[1]=0;
}

#include "flash_storage.h"
#include <stddef.h>
#include <stdint.h>

static int overlap(const void *a,uint32_t an,const void *b,uint32_t bn){
 uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;
 return an&&bn&&(x<=y?y-x<an:x-y<bn);
}
static int layout(uint32_t size,const struct sa_flash_partition *parts,uint32_t count){
 if(count>5u||(!parts&&count))return 0;
 for(uint32_t i=0;i<count;i++){
  uint32_t start=parts[i].start,n=parts[i].size;
  if(!n||start>size||n>size-start)return 0;
  for(uint32_t j=0;j<i;j++)
   if(start<parts[j].start+parts[j].size&&parts[j].start<start+n)return 0;
 }
 return 1;
}
static int guest(uint32_t address,uint32_t count,uint32_t *offset){
 uint32_t segment=address&0xe0000000u,physical=address&0x1fffffffu;
 if(segment!=0u&&segment!=0x80000000u&&segment!=0xa0000000u)return 0;
 if(physical<0x0c000000u||physical>0x0d000000u)return 0;
 uint32_t start=physical-0x0c000000u;
 if(count>0x1000000u-start)return 0;
 *offset=start;return 1;
}
static void word(uint8_t *out,uint32_t value){
 for(unsigned i=0;i<4;i++)out[i]=(uint8_t)(value>>(8u*i));
}
uint32_t sa_flash_storage_size(void){return (uint32_t)sizeof(struct sa_flash_storage);}
int sa_flash_storage_init(struct sa_flash_storage *s){
 if(!s)return -2;
 volatile uint8_t *out=(volatile uint8_t *)s;
 for(unsigned i=0;i<sizeof(*s);i++)out[i]=0;
 return 0;
}
int sa_flash_storage_configure(struct sa_flash_storage *s,uint8_t *bytes,uint32_t size,const struct sa_flash_partition *parts,uint32_t count,uint32_t writable){
 if(!s||!bytes||(!parts&&count))return -2;
 if(!size||writable>1u||!layout(size,parts,count)||overlap(s,sizeof(*s),bytes,size))return -3;
 /* Copy metadata first: a caller may reconfigure from this state's own table. */
 volatile struct sa_flash_partition copied[5];
 for(uint32_t i=0;i<count;i++){copied[i].start=parts[i].start;copied[i].size=parts[i].size;}
 sa_flash_storage_init(s);s->bytes=bytes;s->size=size;s->configured=1;
 s->writable=writable;s->partition_count=count;
 for(uint32_t i=0;i<count;i++){s->partitions[i].start=copied[i].start;s->partitions[i].size=copied[i].size;}
 return 0;
}
int sa_flash_storage_service(struct sa_flash_storage *s,uint8_t *ram,uint32_t selector,uint32_t first,uint32_t second,uint32_t count,uint32_t *result){
 if(!s||!ram||!result)return -2;
 if(selector>3u)return -8;
 if(!s->configured)return -2;
 if(s->configured!=1u||s->writable>1u||!s->bytes||!s->size||!layout(s->size,s->partitions,s->partition_count))return -3;
 if(overlap(s,sizeof(*s),ram,0x1000000u)||overlap(s,sizeof(*s),s->bytes,s->size)||overlap(s->bytes,s->size,ram,0x1000000u)||overlap(result,4,s,sizeof(*s))||overlap(result,4,s->bytes,s->size)||overlap(result,4,ram,0x1000000u))return -3;
 uint32_t offset=0,answer=UINT32_MAX;
 if(selector==0u){
  if(first<s->partition_count){
   if(!guest(second,8u,&offset))return -3;
   word(ram+offset,s->partitions[first].start);
   word(ram+offset+4,s->partitions[first].size);answer=0;
  }
 }else if(selector==1u||selector==2u){
  if(count<=INT32_MAX&&first<=s->size&&count<=s->size-first){
   if(!guest(second,count,&offset))return -3;
   if(selector==1u){
    for(uint32_t i=0;i<count;i++)ram[offset+i]=s->bytes[first+i];
    s->read_bytes+=count;answer=count;
   }else if(s->writable){
    int allowed=1;
    for(uint32_t i=0;i<count;i++)if((s->bytes[first+i]&ram[offset+i])!=ram[offset+i]){allowed=0;break;}
    if(allowed){
     for(uint32_t i=0;i<count;i++)s->bytes[first+i]=ram[offset+i];
     s->programmed_bytes+=count;answer=count;
    }
   }
  }
 }else if(s->writable){
  for(uint32_t i=0;i<s->partition_count;i++)if(s->partitions[i].start==first){
   uint32_t n=s->partitions[i].size;
   for(uint32_t j=0;j<n;j++)s->bytes[first+j]=0xffu;
   s->erased_bytes+=n;answer=0;break;
  }
 }
 s->calls[selector]++;*result=answer;return 0;
}

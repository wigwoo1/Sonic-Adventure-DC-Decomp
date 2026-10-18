#include "input_command_transport.h"
#include "fpu_cleanup_transfer.h"
static uint32_t read_word(const uint8_t *p){return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static int ram_offset(uint32_t address,uint32_t size,uint32_t bytes,uint32_t *offset){
 uint32_t physical=address&0x1fffffffu;
 if(physical<0x0c000000u)return -1;
 *offset=physical-0x0c000000u;
 return *offset>size||bytes>size-*offset?-1:0;
}
SA_STAGE_EXPORT int sa_input_command_transport(uint8_t *ram,uint32_t ram_size,uint32_t list_address,uint32_t list_bytes,uint32_t mode,void *user,sa_input_transport_exchange exchange){
 struct sa_input_transport_request requests[24];uint32_t count=0,start,pos=0;int ended=0;
 if(!ram||!exchange||mode>1||ram_offset(list_address,ram_size,list_bytes,&start))return -1;
 while(pos<list_bytes){
  if(list_bytes-pos<4)return -2;
  uint32_t control=read_word(ram+start+pos);
  if(control==0x700u){pos+=4;continue;}
  if(control&~0x800300ffu)return -2;
  uint32_t words=control&255u,bytes=12u+words*4u,response_offset;
  if(bytes>list_bytes-pos||count==24)return -2;
  uint32_t response=read_word(ram+start+pos+4),packet=read_word(ram+start+pos+8);
  if(ram_offset(response,ram_size,1024,&response_offset))return -3;
  uint32_t packet_words=mode?(packet&255u):(packet>>24);
  if(packet_words!=words)return -2;
  struct sa_input_transport_request *r=&requests[count++];
  r->port=(control>>16)&3u;r->command=mode?(packet>>24):(packet&255u);
  r->destination=mode?((packet>>16)&255u):((packet>>8)&255u);
  r->source=mode?((packet>>8)&255u):((packet>>16)&255u);
  r->payload_words=words;r->response_address=response;r->packet_mode=mode;r->payload=ram+start+pos+12;
  pos+=bytes;
  if(control&0x80000000u){ended=1;break;}
 }
 if(!ended)return -2;
 for(uint32_t i=0;i<count;i++){
  uint8_t response[1024];int bytes=exchange(user,&requests[i],response,1024);uint32_t offset;
  if(bytes<0)return bytes;
  if(bytes>1024)return -4;
  if(ram_offset(requests[i].response_address,ram_size,(uint32_t)bytes,&offset))return -3;
  for(int j=0;j<bytes;j++)ram[offset+(uint32_t)j]=response[j];
 }
 return (int)count;
}

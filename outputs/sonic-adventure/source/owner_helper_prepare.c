#include "owner_helper_prepare.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t half(uint8_t *p){return p[0]|((uint32_t)p[1]<<8);}
static void store(uint8_t *p,uint32_t v){p[0]=(uint8_t)v;p[1]=(uint8_t)(v>>8);}
void sa_owner_helper_prepare(uint8_t *ram,uint32_t *sr,uint32_t config,struct sa_owner_helper_selection *selection){
 uint8_t *work=at(ram,config);uint32_t current=half(work+4),previous=half(work+6),state=half(work);
 store(work+8,previous);store(work+14,0u);
 /* Switching an index restarts active states; idle/stop/reset states retain it. */
 if(current==previous || state==0u || state==2u || state==14u)store(work+12,half(work+12)+1u);
 else{store(work,1u);store(work+12,0u);state=1u;}
 uint32_t signed_state=(state&0x8000u)!=0u?state|0xffff0000u:state;
 selection->state=signed_state;selection->descriptor_offset=current<<4;
 /* The table contains offsets relative to the BRAF instruction's PC+4. */
 static const uint16_t displacement[15]={32,120,186,250,818,1210,946,1294,1004,434,588,742,1394,1526,1526};
 selection->target=signed_state<15u?0x8c07950eu+displacement[signed_state]:0x8c079b04u;
 *sr=(*sr&~1u)|(signed_state>=15u);
}

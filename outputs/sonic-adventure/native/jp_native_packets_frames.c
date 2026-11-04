/* Separate real-owner adapter; no alternative queue or resource state. */
#include "jp_native_packets.h"
#include "jp_native_frames.h"
int jp_packets_decode_frame(uint64_t frame,size_t budget,struct jp_packet_plan **out,
                           struct jp_packet_failure *failure)
{
    struct jp_packet_span lists[5];
    for(unsigned i=0;i<5;i++){
        size_t bytes=SIZE_MAX;const void *p=jp_frame_list(frame,i,&bytes);
        if(bytes==SIZE_MAX){
            if(failure){failure->code=JP_PACKET_FRAME;failure->list=i;failure->offset=0;}
            return JP_PACKET_FRAME;
        }
        lists[i].data=p;lists[i].bytes=bytes;
    }
    return jp_packets_decode(lists,budget,out,failure);
}

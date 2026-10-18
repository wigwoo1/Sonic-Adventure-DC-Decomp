#include "native_disc_queue.h"
static uint32_t rd(const uint8_t *m,uint32_t at) {
    return (uint32_t)m[at]|((uint32_t)m[at+1]<<8)|((uint32_t)m[at+2]<<16)|((uint32_t)m[at+3]<<24);
}
static int span(uint32_t address,uint64_t size,uint32_t *offset) {
    uint32_t region=address&0xe0000000u;
    if(region!=0 && region!=0x80000000u && region!=0xa0000000u)return 0;
    uint32_t physical=address&0x1fffffffu;
    if(physical<0x0c000000u || physical>0x0d000000u)return 0;
    *offset=physical-0x0c000000u;
    return size<=0x1000000u-*offset;
}
/* New native platform bridge for recovered read commands 16 and 17.
 * Parameters: FAD, sector count, guest destination, reserved zero.
 * Accepted address aliases map to the caller's stable 16 MiB RAM allocation.
 * Submission only queues work. Guest RAM must remain alive until release.
 * No firmware status, cache coherency, DMA or timing equivalence is implied. */
SA_DISC_EXPORT int sa_disc_submit_read_command(struct sa_disc_queue *queue,uint8_t *ram,
    uint32_t command,uint32_t parameters,uint32_t *handle)
{
    if(!queue || !ram || !handle)return -2;
    if(command!=16 && command!=17)return -8;
    uint32_t at,dest;
    if(!span(parameters,16,&at))return -3;
    uint32_t fad=rd(ram,at),count=rd(ram,at+4),address=rd(ram,at+8);
    if(fad<150 || rd(ram,at+12)!=0 || !span(address,(uint64_t)count*2048,&dest))return -3;
    return sa_disc_queue_submit(queue,fad-150,count,ram+dest,0x1000000u-dest,handle);
}

static void wr(uint8_t *ram,uint32_t at,uint32_t value) {
    for(unsigned i=0;i<4;i++)ram[at+i]=(uint8_t)(value>>(i*8));
}
/* Read-only-disc subset of runtime operations 0/1/2/8. Host transport errors
 * are distinct from firmware-style results. This is platform policy, not a
 * decompilation of firmware: one sector per pump, cancelled requests report
 * failure/error 0, I/O failures report generic error 8 plus the host detail.
 * Terminal status consumes its handle, preventing slot leaks in game callers. */
int sa_disc_service(struct sa_disc_queue *queue,uint8_t *ram,uint32_t operation,
    uint32_t arg4,uint32_t arg5,uint32_t *result)
{
    if(!queue || !ram || !result)return -2;
    if(operation==3) {
        /* Host startup validation: caller already configured the track/I/O.
         * Never discard outstanding ownership or silently reuse handles. */
        if(queue->busy)return -6;
        for(unsigned i=0;i<8;i++)if(queue->jobs[i].state!=SA_DISC_FREE)return -6;
        *result=0;return 0;
    }
    if(operation==0) {
        uint32_t handle=0;
        /* Queued drive initialization acknowledges the configured read-only
         * track. It transfers no bytes and does not reset other ownership. */
        int status;
        if(arg4==24 && arg5==0)
            status=sa_disc_queue_submit(queue,queue->track_lba,0,0,0,&handle);
        else if(arg4==40) {
            uint32_t at,dest;
            if(!span(arg5,8,&at) || rd(ram,at+4)!=0 || !span(rd(ram,at),16,&dest))status=-3;
            else status=sa_disc_queue_submit_version(queue,ram+dest,0x1000000u-dest,&handle);
        }
        else status=sa_disc_submit_read_command(queue,ram,arg4,arg5,&handle);
        *result=status?0:handle;return 0;
    }
    if(operation==4) {
        uint32_t at;if(!span(arg4,8,&at))return -3;
        /* Read-only image drive policy: standby/GD-ROM, or empty/no disc.
         * A callback in progress reports busy. No physical drive is implied. */
        wr(ram,at,queue->busy?0:queue->track_sectors?2:7);
        wr(ram,at+4,queue->track_sectors?0x80u:0);
        *result=0;return 0;
    }
    if(operation==2) {
        int status=sa_disc_queue_step(queue);if(status)return status;
        *result=0;return 0;
    }
    if(operation==8) {
        *result=sa_disc_queue_cancel(queue,arg4)?1:0;return 0;
    }
    if(operation!=1)return -8;
    uint32_t at;if(!span(arg5,16,&at))return -3;
    if(queue->busy){*result=4;return 0;}
    uint32_t state=0,completed=0;int32_t error=0;
    int found=sa_disc_queue_status(queue,arg4,&state,&completed,&error)==0;
    uint32_t bytes=0;
    if(found) {int status=sa_disc_queue_transfer_bytes(queue,arg4,&bytes);if(status)return status;}
    uint32_t raw=0,err1=0,err2=0;
    if(found) {
        if(state==SA_DISC_QUEUED)raw=1;
        else if(state==SA_DISC_COMPLETE)raw=2;
        else {raw=UINT32_MAX;err1=state==SA_DISC_CANCELLED?0:8;err2=(uint32_t)error;}
    }
    wr(ram,at,err1);wr(ram,at+4,err2);wr(ram,at+8,bytes);wr(ram,at+12,0);
    if(found && state!=SA_DISC_QUEUED) {
        int status=sa_disc_queue_release(queue,arg4);if(status)return status;
    }
    *result=raw;return 0;
}

/* Synchronous native host sector service for recovered directory-loader calls.
 * LBA input, stable guest RAM, same declared queue policies. Other queued work
 * may advance while waiting; this call owns/releases only its own request. */
int sa_disc_read_guest_sectors(struct sa_disc_queue *queue,uint8_t *ram,
    uint32_t first,uint32_t count,uint32_t address)
{
    if(!queue || !ram)return -2;
    uint32_t at,handle;
    if(!span(address,(uint64_t)count*2048,&at))return -3;
    int status=sa_disc_queue_submit(queue,first,count,ram+at,0x1000000u-at,&handle);
    if(status)return status;
    for(;;) {
        status=sa_disc_queue_step(queue);if(status)return status;
        uint32_t state,completed;int32_t error;
        status=sa_disc_queue_status(queue,handle,&state,&completed,&error);if(status)return status;
        if(state==SA_DISC_QUEUED)continue;
        status=sa_disc_queue_release(queue,handle);if(status)return status;
        return state==SA_DISC_COMPLETE?0:state==SA_DISC_FAILED?error:-9;
    }
}

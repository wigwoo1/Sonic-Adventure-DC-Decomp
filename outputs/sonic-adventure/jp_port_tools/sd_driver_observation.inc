/* Authored observations of actual original SDK construction and driver loads.
 * No seeded work, fabricated driver info or initialization-success exports. */
_Static_assert(sizeof(struct sd_work)+31<=sizeof sdgDefaultWrkImg,"native work fits original default allocation with alignment");
_Static_assert(sizeof(struct SDS_MEM_BLK)==48&&sizeof(struct sd_memblk)==48,"host descriptors");
_Static_assert(sizeof(struct sd_primary_command)==16&&sizeof(struct driver_header)==32,"unchanged driver wire layouts");
static uint64_t jp_sd_boot_frames;
static int jp_sd_wait_driver(void)
{
    uint32_t status=0;
    if(jp_aica_read(0x20+(uint32_t)offsetof(struct drv_info,status),&status,4)||
        status>=SD_SND_MEM_SIZE||status&3)return 1;
    int16_t pcm[4096*2];uint32_t remaining=44100;
    while(remaining) {
        uint32_t frames=remaining>4096?4096:remaining;
        if(jp_aica_render(frames,pcm,sizeof pcm))return 1;
        remaining-=frames;jp_sd_boot_frames+=frames;
    }
    uint8_t ready=0xff;
    return jp_aica_read(status,&ready,1)||ready!=0;
}
__declspec(dllexport) int peer_initialize(void)
{
    struct jp_g2_info value;
    if(jp_g2_snapshot(&value)||!value.ready||sdgWrkPtr)return 1;
    return sdLibInit(NULL,0,0);
}
__declspec(dllexport) int peer_observe(uint64_t *out)
{
    if(!out||!sdgWrkPtr)return 1;
    out[0]=sizeof(struct sd_work);out[1]=(uintptr_t)sdgWrkPtr;
    out[2]=(uintptr_t)sdgWrkPtr->host.primary;out[3]=(uintptr_t)sdgWrkPtr->memblk;
    out[4]=sdgWrkPtr->memblk_num;out[5]=sdgWrkPtr->transfer_num;
    out[6]=sdgWrkPtr->transfer_end_num;out[7]=(uintptr_t)sdgWrkPtr->transfer;
    out[8]=sdgWrkPtr->queue_num;out[9]=sdgExistSndDrv;out[10]=jp_sd_boot_frames;
    out[11]=sdgWrkPtr->system_info;out[12]=sdgWrkPtr->tool_interface;
    out[13]=sdgWrkPtr->sound_status;out[14]=sdgWrkPtr->status;
    out[15]=sdgWrkPtr->data_map;out[16]=sdgWrkPtr->host_command;
    out[17]=sdgWrkPtr->command_history;out[18]=sdgWrkPtr->transfer_mode;
    out[19]=sdgMemBlkG2DmaErrNum;return 0;
}
__declspec(dllexport) int peer_record(SDMEMBLK handle,uint64_t *out)
{
    uintptr_t address=(uintptr_t)handle,first=(uintptr_t)sdgDefMemBlkBuf;
    if(!out||address<first||address>=first+sizeof sdgDefMemBlkBuf||
        (address-first)%sizeof(struct sd_memblk))return 1;
    struct sd_memblk *block=(struct sd_memblk *)handle;
    out[0]=block->used;out[1]=block->stat;out[2]=(uintptr_t)block->data;
    out[3]=(uint32_t)block->size;out[4]=(uintptr_t)block->callback;
    out[5]=(uintptr_t)block->callback_first_arg;out[6]=block->done;out[7]=block->unk_1c;return 0;
}

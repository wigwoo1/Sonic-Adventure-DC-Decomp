/* Same translation unit as the actual SDK banks. Stream-buffer ownership is
 * read from the original middleware pool; no shadow allocation map is kept. */
extern uint32_t jp_effects_active_stream_buffers(void);
int jp_effects_active_ring_conflict(long address,long bytes)
{
    if(bytes<=0)return 0;
    uint32_t mask=jp_effects_active_stream_buffers();
    for(unsigned i=0;i<8;i++)if(mask&(1u<<i)){
        uint32_t first=SD_SND_MEM_SIZE-(i+1)*0x4040;
        if((int64_t)address<(int64_t)first+0x4040&&first<(int64_t)address+bytes)return 1;
    }
    return 0;
}
int jp_effects_ring_reservation_allowed(int channels)
{
    if(!sdgWrkPtr||channels<1||channels>2)return 0;
    uint32_t mask=jp_effects_active_stream_buffers();
    for(unsigned i=0;i<8&&channels;i++)if(!(mask&(1u<<i))){
        int64_t a=SD_SND_MEM_SIZE-(i+1)*0x4040,b=a+0x4040;
        struct sd_banks *banks=&sdgWrkPtr->banks;
        struct sd_bank *groups[]={banks->midi_seq,banks->midi_prg,banks->shot,&banks->fx_prg,&banks->fx_out,&banks->fx_wrk};
        for(unsigned g=0;g<6;g++)for(unsigned n=0;n<(g<3?16u:1u);n++){
            struct sd_bank *bank=groups[g]+n;
            if(bank->address>0&&bank->size>0&&a<(int64_t)bank->address+bank->size&&bank->address<b)return 0;
        }
        channels--;
    }
    return channels==0;
}
static int jp_effects_single_bank_valid(void *data,long size,SDE_DATA_TYPE type,int bank)
{
    uint32_t magic=0;Sint32 at=0,capacity=0;
    if(!data||size<32)return 0;memcpy(&magic,data,4);
    if(magic!=(uint32_t)type||sdSndMemGetBankStat(type,(Sint8)bank,&at,&capacity)||
       at<=0||capacity<size||at>SD_SND_MEM_SIZE||size>SD_SND_MEM_SIZE-at||
       jp_effects_active_ring_conflict(at,size))return 0;
    uint32_t mask=sdgWrkPtr->transfer_mode==SDE_MEMBLK_TRANSFER_MODE_DMA?31:3;
    return !(((uintptr_t)data|(uint32_t)at|(uint32_t)size)&mask);
}

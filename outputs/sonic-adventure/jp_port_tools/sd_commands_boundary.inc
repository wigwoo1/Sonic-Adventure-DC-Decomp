/* Native validation before entering the original trusted-data MLT loader. */
_Static_assert(sizeof(struct multi_unit)==32&&sizeof(struct multi_unit_bank)==32,"MLT wire records stay32bit");
_Static_assert(sizeof(struct sd_bank)==8&&sizeof(struct sd_primary_command)==16,"driver wire records");
static struct sd_bank *jp_sd_bank(struct sd_banks *banks,SDE_DATA_TYPE type,int number)
{
    if(number<0)return NULL;
    switch(type) {
    case SDE_DATA_TYPE_MIDI_SEQ_BANK:return number<16?&banks->midi_seq[number]:NULL;
    case SDE_DATA_TYPE_MIDI_PRG_BANK:return number<16?&banks->midi_prg[number]:NULL;
    case SDE_DATA_TYPE_SHOT_BANK:return number<16?&banks->shot[number]:NULL;
    case SDE_DATA_TYPE_PSTM_RING_BUF:return number<16?&banks->pstm_ring[number]:NULL;
    case SDE_DATA_TYPE_FX_PRG_BANK:return number==0?&banks->fx_prg:NULL;
    case SDE_DATA_TYPE_FX_OUT_BANK:return number==0?&banks->fx_out:NULL;
    case SDE_DATA_TYPE_FX_PRG_WRK:return number==0?&banks->fx_wrk:NULL;
    default:return NULL;
    }
}
__declspec(dllexport) SDE_ERR jp_sd_download_multi(SDMEMBLK handle)
{
    uintptr_t at=(uintptr_t)handle,first=(uintptr_t)sdgDefMemBlkBuf;
    struct jp_g2_info transport;
    if(jp_g2_snapshot(&transport)||!transport.ready||!sdgWrkPtr||!sdgExistSndDrv)return SDE_ERR_NO_INIT;
    if(at<first||at>=first+sizeof sdgDefMemBlkBuf||(at-first)%sizeof(struct sd_memblk))return SDE_ERR_HANDLE_ILLEGAL_VALUE;
    struct sd_memblk *block=(struct sd_memblk *)handle;
    if(!block->used||!block->data||block->size<32)return SDE_ERR_PRM_OVER_RANGE;
    if(block->stat==SDE_MEMBLK_STAT_TRANSFER_PROGRESS)return SDE_ERR_MEMBLK_ALREADY_TRANSFER;
    struct multi_unit unit;memcpy(&unit,block->data,sizeof unit);
    if(unit.type!=SDE_DATA_TYPE_MULTI_UNIT)return SDE_ERR_DATA_ILLEGAL_TYPE;
    uint32_t start=unit.version<1&&unit.unk_10!=-1?16:32;
    if(unit.bank_num<0||(uint32_t)unit.bank_num>((uint32_t)block->size-start)/32)return SDE_ERR_PRM_OVER_RANGE;
    struct sd_banks proposed=sdgWrkPtr->banks;
    for(int i=0;i<unit.bank_num;i++) {
        struct multi_unit_bank record;
        memcpy(&record,(const char *)block->data+start+(size_t)i*32,32);
        if(record.size<0&&record.data_size<=0)continue;
        struct sd_bank *bank=jp_sd_bank(&proposed,record.type,record.num);
        if(!bank)return SDE_ERR_BANK_ILLEGAL_NUM;
        if(record.size>=0) {
            if(record.address<0||record.size>SD_SND_MEM_SIZE-record.address||record.address>SD_SND_MEM_SIZE)return SDE_ERR_PRM_OVER_RANGE;
            bank->address=record.address;bank->size=record.size;
        }
        if(record.data_size>0) {
            if(record.data_offset<0||record.data_offset>block->size||record.data_size>block->size-record.data_offset||
                bank->address<=0||bank->address>=SD_SND_MEM_SIZE||bank->size<=0||
                record.data_size>bank->size||record.data_size>SD_SND_MEM_SIZE-bank->address)return SDE_ERR_PRM_OVER_RANGE;
            uintptr_t source=(uintptr_t)block->data+(uint32_t)record.data_offset;
            uint32_t mask=sdgWrkPtr->transfer_mode==SDE_MEMBLK_TRANSFER_MODE_DMA?31:3;
            if((source&mask)||((uint32_t)record.data_size&mask)||((uint32_t)bank->address&mask))return SDE_ERR_PRM_OVER_RANGE;
        }
    }
    return sdMultiUnitDownload(handle);
}

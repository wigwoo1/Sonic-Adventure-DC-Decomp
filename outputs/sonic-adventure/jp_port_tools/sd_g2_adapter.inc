/* Actual native transport bindings for the pinned SDK. No console DMA/SR
 * implementation is exported. Unsupported transport operations return errors. */
static SYE_G2_ERR jp_sd_g2_result(int error)
{
    return error==0?SYE_G2_ERR_NOTHING:error==JP_G2_BUSY?SYE_G2_ERR_DMA_BUSY:
        (SYE_G2_ERR)0x0009ffff;
}
SYE_G2_ERR syG2Read(void *bus,void *host,SYE_G2_DATA_WIDTH width,Sint32 count,
    SYE_G2_ADR_MODE source,SYE_G2_ADR_MODE destination)
{
    if((uintptr_t)bus>UINT32_MAX)return (SYE_G2_ERR)0x0009ffff;
    return jp_sd_g2_result(jp_g2_read((uint32_t)(uintptr_t)bus,host,width,count,source,destination));
}
SYE_G2_ERR syG2Write(void *host,void *bus,SYE_G2_DATA_WIDTH width,Sint32 count,
    SYE_G2_ADR_MODE source,SYE_G2_ADR_MODE destination)
{
    if((uintptr_t)bus>UINT32_MAX)return (SYE_G2_ERR)0x0009ffff;
    return jp_sd_g2_result(jp_g2_write(host,(uint32_t)(uintptr_t)bus,width,count,source,destination));
}
SYE_G2_ERR syG2DmaOpen(SYG2DMA *handle,SYE_G2_DMA_TYPE type)
{
    if(!handle)return (SYE_G2_ERR)0x0009ffff;
    void *native=NULL;int error=jp_g2_dma_open(&native,type);
    if(!error)*handle=native;
    if(error==JP_G2_STATE)return SYE_G2_ERR_DMA_ALREADY_OPEN;
    return jp_sd_g2_result(error);
}
SYE_G2_ERR syG2DmaSetPrm(SYG2DMA handle,void *bus,void *host,Sint32 bytes,
    SYE_G2_DMA_DIR direction,Bool control,Bool suspend,SYE_G2_CALLBACK_FUNC callback,void *context)
{
    if((uintptr_t)bus>UINT32_MAX)return (SYE_G2_ERR)0x0009ffff;
    return jp_sd_g2_result(jp_g2_dma_prepare(handle,(uint32_t)(uintptr_t)bus,host,bytes,
        direction,control,suspend,callback,context));
}
SYE_G2_ERR syG2DmaTrigger(SYG2DMA handle,SYE_G2_DMA_TRIGGER trigger)
{return jp_sd_g2_result(jp_g2_dma_trigger(handle,trigger));}
SYE_G2_ERR syG2DmaTriger(SYG2DMA handle,SYE_G2_DMA_TRIGGER trigger)
{return syG2DmaTrigger(handle,trigger);}
SYE_G2_ERR syG2DmaIsBusy(SYG2DMA handle,Bool *busy)
{
    if(!busy)return (SYE_G2_ERR)0x0009ffff;
    uint32_t value=0;int error=jp_g2_dma_busy(handle,&value);
    if(!error)*busy=(Bool)value;return jp_sd_g2_result(error);
}
/* Preserve the original SDK's documented trigger-counter bug: its public
 * getter returns end_count. The transport's snapshot retains both real counts. */
SYE_G2_ERR syG2DmaGetTriggerCounter(SYE_G2_DMA_TYPE type,Uint32 *counter)
{
    if(type!=SYE_G2_DMA_TYPE_AICA||!counter)return (SYE_G2_ERR)0x0009ffff;
    struct jp_g2_info value;int error=jp_g2_snapshot(&value);
    if(!error&&!value.ready)error=JP_G2_STATE;
    if(!error)*counter=(Uint32)value.dma_ended;return jp_sd_g2_result(error);
}
SYE_G2_ERR syG2DmaGetEndCounter(SYE_G2_DMA_TYPE type,Uint32 *counter)
{return syG2DmaGetTriggerCounter(type,counter);}

static int jp_sd_transfer_error;
static void jp_sd_wait_transfer(void)
{
    uint32_t completed=0;int error=jp_g2_service(1,&completed);
    if(error||!completed)jp_sd_transfer_error=1;
}

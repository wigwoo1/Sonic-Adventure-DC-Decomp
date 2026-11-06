/* Appended beside the genuine private FM state; never mirrors its cache. */
#include "jp_native_flash.h"
#define FM_ENTER(value) do {if(jp_flash_fm_begin())return (value);} while(0)
#define FM_RETURN(expr) do {Sint32 result=(expr);jp_flash_fm_end();return result;} while(0)
__declspec(dllexport) Sint32 fmMngInit(Sint32 part,void *work)
{
    FM_ENTER(FMD_NOT_OPEN);
    if(active&&active->request!=REQUEST_NONE)FM_RETURN(FMD_REQUEST_PENDING);
    if(!work||((uintptr_t)work&3u)){jp_flash_argument_error();FM_RETURN(FMD_READ_FAILED);}
    if(jp_flash_status())FM_RETURN(FMD_READ_FAILED);
    FM_RETURN(jp_flash_source_fmMngInit(part,work));
}
__declspec(dllexport) Sint32 fmMngFormat(Sint32 type)
{
    FM_ENTER(FMD_NOT_OPEN);
    if(active&&active->request!=REQUEST_NONE)FM_RETURN(FMD_REQUEST_PENDING);
    FM_RETURN(jp_flash_source_fmMngFormat(type));
}
__declspec(dllexport) Sint32 fmMngRead(Sint32 packet,void *buffer)
{
    FM_ENTER(FMD_NOT_OPEN);
    if(!buffer||(uintptr_t)buffer>UINTPTR_MAX-FMD_DATA_BYTES){jp_flash_argument_error();FM_RETURN(FMD_READ_FAILED);}
    FM_RETURN(jp_flash_source_fmMngRead(packet,buffer));
}
__declspec(dllexport) Sint32 fmMngWrite(Sint32 packet,void *buffer)
{
    FM_ENTER(FMD_NOT_OPEN);
    if(!buffer||(uintptr_t)buffer>UINTPTR_MAX-FMD_DATA_BYTES){jp_flash_argument_error();FM_RETURN(FMD_WRITE_FAILED);}
    FM_RETURN(jp_flash_source_fmMngWrite(packet,buffer));
}
__declspec(dllexport) Sint32 fmMngExecServer(void)
{FM_ENTER(FMD_STAT_ERR);FM_RETURN(jp_flash_source_fmMngExecServer());}
__declspec(dllexport) Sint32 fmMngGetErr(void)
{FM_ENTER(FMD_NOT_OPEN);FM_RETURN(jp_flash_source_fmMngGetErr());}
__declspec(dllexport) Sint32 fmMngGetFree(void)
{FM_ENTER(FMD_NOT_OPEN);FM_RETURN(jp_flash_source_fmMngGetFree());}
__declspec(dllexport) Sint32 fmMngRefresh(void)
{
    FM_ENTER(FMD_NOT_OPEN);
    if(active&&active->request!=REQUEST_NONE)FM_RETURN(FMD_REQUEST_PENDING);
    FM_RETURN(jp_flash_source_fmMngRefresh());
}
__declspec(dllexport) void fmMngFinish(void)
{if(jp_flash_fm_begin())return;jp_flash_source_fmMngFinish();jp_flash_fm_end();}
int jp_flash_fm_is_open(void){return active!=NULL;}
int jp_flash_fm_close(void)
{
    if(active&&active->request!=REQUEST_NONE)return JP_FLASH_BUSY;
    jp_flash_source_fmMngFinish();return 0;
}
int jp_flash_fm_snapshot(struct jp_flash_fm_info *out)
{
    int rc=jp_flash_owner_access();if(rc)return rc;if(!out)return JP_FLASH_ARGUMENT;
    if(!active){*out=(struct jp_flash_fm_info){.part=-1};return 0;}
    *out=(struct jp_flash_fm_info){active->part,active->type,active->address,active->size,
        active->capacity,active->used,active->request,active->status,active->phase,
        active->error,(uint64_t)(uintptr_t)active->cache};return 0;
}
#undef FM_ENTER
#undef FM_RETURN

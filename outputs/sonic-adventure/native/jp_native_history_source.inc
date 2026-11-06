/* Privately compiled with SDK declarations; Windows ownership stays separate. */
#include "jp_native_history.h"
_Static_assert(sizeof(UFS_AllPlayHistory)==240,"four original sixty-byte packets");
_Static_assert(offsetof(UFS_AllPlayHistory,CRC)==112,"original title CRC extent");
_Static_assert(offsetof(UFS_AllPlayHistory,PrevStartTime)==120,"original packet two");
_Static_assert(offsetof(UFS_AllPlayHistory,SaveCount)==180,"original packet three");
extern int jp_input_status(void);
extern int jp_rtc_status(void);
extern int jp_history_rtc_get(SYS_RTC_DATE *);
extern int jp_history_rtc_count(const SYS_RTC_DATE *,Uint32 *);
extern void jp_history_source_init(void);
extern void *jp_history_source_work(unsigned);
static const PDS_PERIPHERALINFO absent_expansion;
int jp_history_source_identity_valid(void)
{
    SYS_BT_SYSTEMID id={0};
    return syBtGetBootSystemID(&id)&&id.nNo>0&&id.nNo<=id.nAll&&id.nAll<=99;
}
const PDS_PERIPHERAL *jp_history_get_peripheral(Uint32 port)
{
    if(jp_input_status()){jp_history_source_problem(JP_HISTORY_INPUT,0);return NULL;}
    return pdGetPeripheral(port);
}
const PDS_PERIPHERALINFO *jp_history_get_peripheral_info(Uint32 port)
{
    if(jp_input_status()||port>=24){jp_history_source_problem(JP_HISTORY_INPUT,0);return NULL;}
    const PDS_PERIPHERALINFO *out=pdGetPeripheralInfo(port);
    if(out)return out;
    const PDS_PERIPHERAL *record=pdGetPeripheral(port);
    /* The real provider represents a disconnected socket by id=0/info=NULL.
     * UF expects a description whose type=0 for that same absence. */
    if(record&&!record->id&&!record->info)return &absent_expansion;
    jp_history_source_problem(JP_HISTORY_INPUT,0);return NULL;
}
int jp_history_rtc_get(SYS_RTC_DATE *out)
{
    int rc=(int)syRtcGetDate(out);
    /* Successful SDK reads return the nonnegative server-mode bits. */
    if(rc<0||jp_rtc_status()){jp_history_source_problem(JP_HISTORY_RTC,(uint32_t)jp_rtc_status());return -1;}return 0;
}
int jp_history_rtc_count(const SYS_RTC_DATE *date,Uint32 *out)
{
    syRtcDateToCount(date,out);int rc=jp_rtc_status();
    if(rc)jp_history_source_problem(JP_HISTORY_RTC,(uint32_t)rc);return rc;
}
static int seed_write(Sint32 number,const void *data)
{
    int code=fhFmWriteExec(number,data);
    if(code<0){jp_history_source_problem(JP_HISTORY_FLASH,(uint32_t)ufgErrno);return 0;}return 1;
}
int jp_history_source_seed(void)
{
    void *work=jp_history_source_work(0);SYS_BT_SYSTEMID id={0};SYS_RTC_DATE date;Uint32 now=0;
    if(!syBtGetBootSystemID(&id)){jp_history_source_problem(JP_HISTORY_IDENTITY,0);return 0;}
    if(jp_history_rtc_get(&date)||jp_history_rtc_count(&date,&now))return 0;
    for(int part=4;part>=3;part--){
        int code=jp_history_fmMngInit(part,work);
        if(code!=FMD_BAD_HEADER||jp_history_fmMngFormat(FMD_WRITABLE)!=0){jp_history_source_problem(JP_HISTORY_FLASH,(uint32_t)code);return 0;}
    }
    ufgWorkBuff=work;ufgReadBuff=jp_history_source_work(1);
    ufgReflash3=jp_history_work(2);ufgReflash4=jp_history_work(3);
    ufgRestPacket=jp_history_fmMngGetFree();
    memset(&ufgUserFlag,255,sizeof ufgUserFlag);
    ufSetAutoSaveFlag(1);ufSetVersion(1);ufSetNetuseFlag(0);ufSetData(1);
    if(!ufSaveUserFlag())return 0;
    UFS_AllPlayHistory title;memset(&title,255,sizeof title);
    title.version=1;title.AutoSave=1;memcpy(title.ProdNum,id.szProduct,10);
    memcpy(title.ProdName,jp_history_boot_header()+128,48);
    memcpy(title.ProdName2,jp_history_boot_header()+176,44);
    title.Kind=0;title.FirstStartTime=now;title.PrevStartTime=now;title.StartCount=1;
    memset(title.PlayTime,0,sizeof title.PlayTime);title.LoadCount=title.SaveCount=0;
    title.GameEst=title.GameCarry=0;title.SaveOccur=0;
    title.FirstNetPlay=title.PrevNetPlay=0;title.NetPlayCount=title.NetPlayTotalTime=0;
    title.CRC=(Uint16)fmcalccrc(110,(Uint8 *)&title+2);
    for(int i=0;i<4;i++)if(!seed_write(24+i,(Uint8 *)&title+60*i))return 0;
    return 1;
}
int jp_history_source_ready(void)
{return ufgSafeFlag!=0;}
int jp_history_source_refresh(void)
{return ufSafeRefresh();}
int jp_history_source_recover(void)
{
    if(!ufgSafeFlag){jp_history_source_init();return ufgSafeFlag!=0;}
    return ufSetBank(-3);
}
void jp_history_source_reset(void)
{
    jp_history_fmMngFinish();ufgSafeFlag=0;
    ufgWorkBuff=NULL;ufgReadBuff=NULL;ufgReflash3=NULL;ufgReflash4=NULL;ufgProdNum=NULL;
}
__declspec(dllexport) int peer_history_user_state(void *out)
{
    if(!out||(uintptr_t)out>UINTPTR_MAX-60)return JP_HISTORY_ARGUMENT;
    int rc=jp_history_peer_begin(0);if(rc)return rc;
    ufGetUserState((intptr_t)out);return jp_history_peer_end();
}
__declspec(dllexport) int peer_history_play(unsigned mode,unsigned est,unsigned carry,Uint8 *data)
{
    if(mode>UFD_PLAY||est>255||carry>255)return JP_HISTORY_ARGUMENT;
    int rc=jp_history_peer_begin(1);if(rc)return rc;
    (void)ufPlayTime((Uint8)mode,(Uint8)est,(Uint8)carry,data);return jp_history_peer_end();
}
__declspec(dllexport) int peer_history_record(void *out,size_t bytes)
{
    if(!out||bytes!=sizeof ufgPlayHistBuffer)return JP_HISTORY_ARGUMENT;
    int rc=jp_history_peer_begin(0);if(rc)return rc;
    memcpy(out,&ufgPlayHistBuffer,bytes);return jp_history_peer_end();
}
__declspec(dllexport) int peer_history_library_error(void)
{return (int)ufgErrno;}

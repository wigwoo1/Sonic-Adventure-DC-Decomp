/* Authored native owner/drive boundary. Original GDFS software is private,
 * generated from the pinned source; no original bodies are vendored here. */
#include "jp_native_gdfs_internal.h"
#pragma push_macro("NULL")
#undef NULL
#include "gdfs_source_api.h"
#pragma pop_macro("NULL")
#define JP_GD_API __declspec(dllimport)
#include "jp_native_gd_media.h"
#include <stdlib.h>
#include <string.h>
#define SRC(n) jp_gdfs_source_##n
struct chain_record {uint32_t priority,next;uintptr_t callback,argument;};
extern int jp_vblank_chain_describe(uint32_t,struct chain_record *);
extern int jp_vblank_chain_status(void);
extern uint32_t jp_gdfs_source_chain_handle(void);
extern uintptr_t jp_gdfs_source_chain_callback(void);
extern void jp_gdfs_source_bind_interface(int);
struct callback_record {GDFS handle;GDFS_FUNC read,transfer;GDFS_ERRFUNC error;
    void *read_arg,*transfer_arg,*error_arg;};
struct directory_record {GDFS_DIRREC table;long capacity;struct directory_record *next;};
static struct {
    struct jp_gdfs_info info;uint32_t thread,first,raw,chunk,timeout;
    int owns_media,api_depth,callback_depth,in_directory,closing;
    unsigned directory_budget;long mask;
    struct jp_gdfs_allocation work,dir,callbacks;
    struct callback_record *records;GDFS_ERRFUNC global_error;void *global_arg;
    struct directory_record *directories;
} fs;
enum {CMD_NONE,CMD_INIT,CMD_READ,CMD_STREAM,CMD_SEEK};
static struct {
    int kind,done,aborting,error,media_live,transfer_active,dma;
    long id,next_id;uint32_t fad,sectors,submitted;
    size_t copied,view_bytes,view_at,transfer_bytes,transfer_moved;
    unsigned char *destination,*transfer_destination;
    const unsigned char *view;
    GDFS_FUNC pending_callback;void *pending_arg;GDFS pending_handle;
} command={.next_id=1};
static int result(int code,long source)
{fs.info.status=(uint32_t)code;fs.info.source_error=(int32_t)source;return code;}
static int access(int live)
{
    if(fs.thread&&fs.thread!=jp_gdfs_host_thread())return JP_GDFS_THREAD;
    if(live&&fs.info.phase!=JP_GDFS_READY)return JP_GDFS_NOT_INITIALIZED;
    return 0;
}
static int linked(void)
{
    uint32_t h=jp_gdfs_source_chain_handle();struct chain_record r;
    return h&&!jp_vblank_chain_describe(h,&r)&&(r.priority&0x7fffffff)==80&&
        r.callback==jp_gdfs_source_chain_callback()&&!r.argument;
}
static int handle_valid(GDFS h,int used)
{
    if(!gd_wk||!h)return 0;
    uintptr_t start=(uintptr_t)gd_wk->hndlist,at=(uintptr_t)h;
    size_t count=(size_t)fs.info.max_handles+1;
    return at>=start&&(at-start)%sizeof(GDFS_HANDLE)==0&&
        (at-start)/sizeof(GDFS_HANDLE)<count&&(!used||h->used);
}
static int output_valid(const void *p,size_t bytes)
{
    if(!p)return 0;uintptr_t at=(uintptr_t)p;
    if(bytes>UINTPTR_MAX-at)return 0;
    const struct jp_gdfs_allocation *a[]={&fs.work,&fs.dir,&fs.callbacks};
    for(unsigned j=0;j<3;j++)if(a[j]->base){
        uintptr_t base=(uintptr_t)a[j]->base;
        if(at<base+a[j]->reserved&&base<at+bytes)return 0;
    }
    return 1;
}
static int name_valid(const char *name,int path)
{
    if(!name)return 0;unsigned part=0;
    for(unsigned n=0;n<512;n++){
        unsigned char c=(unsigned char)name[n];if(!c)return n>0;
        if(c<32||c>=127)return 0;
        if(c=='/'||c=='\\'){if(!path)return 0;part=0;}
        else if(++part>31)return 0;
    }
    return 0;
}
static long directory_capacity(GDFS_DIRREC table)
{
    if(gd_wk&&table==gd_wk->curdir)return (long)fs.info.max_entries;
    for(struct directory_record *r=fs.directories;r;r=r->next)if(r->table==table)return r->capacity;
    return 0;
}
static int directory_valid(GDFS_DIRREC table)
{
    long cap=directory_capacity(table);if(!cap||table->max_ent!=cap||table->dir_num<0||table->dir_num>cap)return 0;
    for(long j=0;j<table->dir_num;j++){
        unsigned n=0;while(n<32&&table->dirrec_tbl[j].fname[n])n++;if(n==32)return 0;
    }
    return 1;
}
static void free_directories(void)
{
    while(fs.directories){struct directory_record *r=fs.directories;fs.directories=r->next;free(r);}
}
static void flush_callback(void)
{
    if(fs.mask||fs.callback_depth||!command.pending_callback||fs.closing)return;
    GDFS_FUNC fn=command.pending_callback;void *arg=command.pending_arg;
    GDFS handle=command.pending_handle;
    command.pending_callback=NULL;command.pending_arg=NULL;command.pending_handle=NULL;
    if(!handle_valid(handle,1))return;
    fs.callback_depth++;fs.info.callbacks++;fn(arg);fs.callback_depth--;
}
void jp_gdfs_dispatch_callbacks(void){flush_callback();}
long jp_gdfs_mask_get(void){return fs.mask;}
void jp_gdfs_mask_set(long value){fs.mask=value&15;if(!fs.mask)flush_callback();}
void jp_gdfs_note_tick(void){fs.info.vblanks++;}
int jp_gdfs_expired(uint64_t since){return jp_gdfs_host_time()-since>=fs.timeout;}
int jp_gdfs_directory_begin(long fad,long size,long capacity)
{
    struct jp_gd_media_info m;
    if(jp_gd_media_snapshot(&m)||fad<(long)m.first_fad||size<=0||
       capacity<=0||capacity>8192||(uint64_t)fad>=((uint64_t)m.first_fad+m.sectors))return 0;
    uint64_t maximum=(uint64_t)capacity*256+131072;
    if((uint64_t)size>maximum)return 0;
    fs.directory_budget=(unsigned)((maximum+2047)/2048);return 1;
}
int jp_gdfs_directory_step(void)
{if(!fs.directory_budget)return 0;fs.directory_budget--;return 1;}
int jp_gdfs_directory_valid(const unsigned char *p)
{
    size_t at=0;
    while(at<2048&&p[at]){
        const unsigned char *r=p+at;size_t left=2048-at;
        if(left<34||r[0]<34||r[0]>left||r[32]+33U>r[0]||!r[32])return 0;
        unsigned n=r[32];if(r[25]&2){if(n>31)return 0;}
        else if(n<2||n>33||r[33+n-2]!=';'||r[33+n-1]<'0'||r[33+n-1]>'9')return 0;
        at+=r[0];
    }
    return 1;
}
static void read_callback(void *arg)
{
    struct callback_record *r=arg;if(fs.closing||!r->read)return;
    fs.callback_depth++;fs.info.callbacks++;r->read(r->read_arg);fs.callback_depth--;
}
static void error_callback(void *arg,long error)
{
    struct callback_record *r=arg;if(fs.closing||!r->error)return;
    fs.callback_depth++;fs.info.callbacks++;r->error(r->error_arg,error);fs.callback_depth--;
}
static void global_callback(void *arg,long error)
{
    (void)arg;if(fs.closing||!fs.global_error)return;
    fs.callback_depth++;fs.info.callbacks++;fs.global_error(fs.global_arg,error);fs.callback_depth--;
}
static void reset_command(void)
{long next=command.next_id;memset(&command,0,sizeof command);command.next_id=next;}
static void fail_command(long error)
{
    command.error=(int)error;command.done=1;
    command.destination=command.transfer_destination=NULL;command.transfer_active=0;
}
static int release_media(void)
{
    if(!command.media_live)return 0;
    int rc=jp_gd_media_release();if(!rc){command.media_live=0;command.view=NULL;command.view_at=command.view_bytes=0;}return rc;
}
static void drive_step(void)
{
    if(!command.kind||command.done)return;
    if(command.aborting){
        int rc=command.media_live?jp_gd_media_cancel(0):0;
        if(rc==JP_GD_TIMEOUT)return;
        if(rc){struct jp_gd_media_info m;if(jp_gd_media_snapshot(&m)||m.state==JP_GD_READING||m.state==JP_GD_CANCELLING)return;}
        if(release_media())return;command.done=1;return;
    }
    if(command.kind==CMD_INIT){
        struct jp_gd_media_info m;
        if(jp_gd_media_snapshot(&m)||m.state!=JP_GD_IDLE)fail_command(GDD_ERR_RESET);
        else command.done=1;return;
    }
    if(!command.media_live&&command.submitted<command.sectors){
        uint32_t n=command.sectors-command.submitted;if(n>fs.chunk)n=fs.chunk;
        int rc=jp_gd_media_request(command.fad+command.submitted,n);
        if(rc){fail_command(rc==JP_GD_RANGE?GDD_ERR_SIZEOVER:GDD_ERR_READ);return;}
        command.submitted+=n;command.media_live=1;
    }
    if(command.media_live&&!command.view){
        int rc=jp_gd_media_poll();struct jp_gd_media_info m;
        if(jp_gd_media_snapshot(&m)){fail_command(GDD_ERR_READ);return;}
        if(rc||m.state==JP_GD_FAILED){release_media();fail_command(GDD_ERR_READ);return;}
        if(m.state!=JP_GD_COMPLETE)return;
        const void *view=NULL;size_t size=0;
        if(jp_gd_media_view(&view,&size)){fail_command(GDD_ERR_READ);return;}
        command.view=view;command.view_bytes=size;command.view_at=0;
    }
    if(command.kind==CMD_SEEK){
        if(command.view){if(release_media())fail_command(GDD_ERR_READ);else command.done=1;}return;
    }
    if(command.kind==CMD_READ&&command.view){
        memcpy(command.destination+command.copied,command.view,command.view_bytes);
        command.copied+=command.view_bytes;fs.info.read_bytes+=command.view_bytes;
        if(release_media()){fail_command(GDD_ERR_READ);return;}
    }else if(command.kind==CMD_STREAM&&command.transfer_active){
        size_t n=command.transfer_bytes-command.transfer_moved;
        if(command.view){
            size_t available=command.view_bytes-command.view_at;if(n>available)n=available;
            memcpy(command.transfer_destination+command.transfer_moved,command.view+command.view_at,n);
            command.copied+=n;command.transfer_moved+=n;command.view_at+=n;fs.info.read_bytes+=n;
            if(command.view_at==command.view_bytes&&release_media()){fail_command(GDD_ERR_READ);return;}
        }
        if(command.transfer_moved==command.transfer_bytes){
            command.transfer_active=0;GDFS h=gd_wk?gd_wk->now_work:NULL;
            if(h&&h->trendcb){command.pending_callback=h->trendcb;command.pending_arg=h->trcb_1st;command.pending_handle=h;}
        }
    }
    if(command.copied==(size_t)command.sectors*2048&&!command.transfer_active)command.done=1;
}
static long start_command(int kind,long fad,long sectors,void *out)
{
    if(command.kind)return 0;
    if(command.next_id<=0||command.next_id==INT32_MAX)return 0;
    long id=command.next_id++;command.id=id;command.kind=kind;command.fad=(uint32_t)fad;
    command.sectors=(uint32_t)sectors;command.destination=out;fs.info.requests++;
    struct jp_gd_media_info m;
    if(jp_gd_media_snapshot(&m))fail_command(GDD_ERR_INIT);
    else if(kind!=CMD_INIT&&(fad<(long)m.first_fad||sectors<=0||
        (uint64_t)fad+sectors>(uint64_t)m.first_fad+m.sectors||
        (kind==CMD_READ&&!out)))fail_command(GDD_ERR_OFS);
    return id;
}
static Bool drive_init_system(void)
{struct jp_gd_media_info m;return fs.owns_media&&!command.kind&&!jp_gd_media_snapshot(&m)&&m.state==JP_GD_IDLE;}
static long drive_read(long fad,long sectors,void *out,long mode)
{long id=start_command(CMD_READ,fad,sectors,out);if(id){command.dma=mode==1;if(mode!=0&&mode!=1)fail_command(GDD_ERR_ILLTMODE);}return id;}
static long drive_stream(long fad,long sectors){return start_command(CMD_STREAM,fad,sectors,NULL);}
static long drive_transfer(long id,void *out,long bytes)
{
    if(command.kind!=CMD_STREAM||id!=command.id||command.aborting||command.done)return 2;
    if(bytes<0||(size_t)bytes>(size_t)command.sectors*2048-command.copied)return -1;
    if(command.transfer_active||command.pending_callback)return 2;
    if(bytes&&!out)return -1;
    command.transfer_active=1;command.transfer_destination=out;command.transfer_bytes=(size_t)bytes;command.transfer_moved=0;return 0;
}
static long drive_get_stat(long id,long *info)
{
    if(!command.kind||!id||id!=command.id)return GDD_STAT_IDLE;
    drive_step();info[0]=(long)command.copied;info[1]=0;info[2]=1;
    if(command.done){
        if(!command.error&&command.kind==CMD_READ&&command.dma){
            GDFS h=gd_wk?gd_wk->now_work:NULL;
            if(h&&h->trendcb){command.pending_callback=h->trendcb;command.pending_arg=h->trcb_1st;command.pending_handle=h;}
        }
        int error=command.error;GDFS_FUNC callback=command.pending_callback;void *arg=command.pending_arg;
        GDFS handle=command.pending_handle;
        reset_command();command.pending_callback=callback;command.pending_arg=arg;command.pending_handle=handle;
        if(error){info[0]=error;return GDD_STAT_ERR;}return GDD_STAT_COMPLETE;
    }
    return command.kind==CMD_STREAM?GDD_STAT_CONTINUE:GDD_STAT_BUSY;
}
static void drive_abort(long id)
{
    if(command.id!=id||!command.kind)return;
    command.destination=command.transfer_destination=NULL;command.transfer_active=0;
    command.pending_callback=NULL;command.pending_arg=NULL;command.pending_handle=NULL;command.aborting=1;drive_step();
}
static long drive_seek(long fad){return start_command(CMD_SEEK,fad,1,NULL);}
static long drive_init(void){return start_command(CMD_INIT,0,0,NULL);}
static Bool drive_status(long *out)
{
    struct jp_gd_media_info m;if(!fs.owns_media||jp_gd_media_snapshot(&m))return FALSE;
    /* KallistiOS dc/syscalls.h: busy0, paused1, GD-ROM0x80. These describe
     * this actual mounted virtual data medium; no physical tray is claimed. */
    out[0]=command.kind&&!command.done?0:1;out[1]=0x80;return TRUE;
}
static long drive_check_transfer(long id,long *out)
{
    if(id!=command.id||command.kind!=CMD_STREAM){*out=0;return -1;}
    if(command.transfer_active){*out=(long)command.transfer_moved;return 1;}
    *out=(long)((size_t)command.sectors*2048-command.copied);return 0;
}
static Bool drive_exec(void){drive_step();return TRUE;}
static void drive_reset(void){if(command.kind)drive_abort(command.id);}
static long no_play(long a,long b,long c){(void)a;(void)b;(void)c;return 0;}
static long no_void(void){return 0;}
static long no_repeat(long n){(void)n;return 0;}
static long no_toc(long type,void *out){(void)type;(void)out;return 0;}
static long no_subcode(long type,long size,void *out){(void)type;(void)size;(void)out;return 0;}
static long no_position(long *a,long *b,long *c,long *d){(void)a;(void)b;(void)c;(void)d;return 0;}
static long no_version(char *out){(void)out;return 0;}
static const GD_DCF native_dcf={drive_init_system,drive_read,drive_stream,drive_transfer,
    drive_get_stat,drive_abort,drive_seek,drive_init,drive_status,drive_check_transfer,
    no_play,no_play,no_void,no_repeat,no_toc,no_subcode,drive_exec,drive_reset,no_position,no_version};
const GD_DCF *gdFsGetGdDcf(void){return &native_dcf;}
long jp_gdfs_timeout(void *address)
{
    GDFS h=address;if(command.kind){command.error=GDD_ERR_TOUT;drive_abort(command.id);}
    if(h)h->err=GDD_ERR_TOUT;result(JP_GDFS_TIMEOUT,GDD_ERR_TOUT);return GDD_ERR_TOUT;
}
static int drain(void)
{
    command.destination=command.transfer_destination=NULL;command.pending_callback=NULL;
    command.transfer_active=0;
    if(command.media_live){
        int rc=jp_gd_media_cancel(fs.timeout);if(rc)return rc;
        rc=release_media();if(rc)return rc;
    }
    reset_command();if(gd_wk&&gd_wk->now_work){gd_wk->now_work->stat=GDD_STAT_IDLE;gd_wk->now_work=NULL;}return 0;
}
static int raw_read(uint32_t fad,uint32_t sectors,unsigned char *out)
{
    uint32_t done=0;
    while(done<sectors){
        uint32_t n=sectors-done;if(n>fs.chunk)n=fs.chunk;
        if(jp_gd_media_request(fad+done,n)||jp_gd_media_wait(fs.timeout))return 0;
        const void *view=NULL;size_t bytes=0;
        if(jp_gd_media_view(&view,&bytes)||bytes!=(size_t)n*2048)return 0;
        memcpy(out+(size_t)done*2048,view,bytes);if(jp_gd_media_release())return 0;done+=n;
    }
    return 1;
}
static uint32_t le32(const unsigned char *p)
{return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static int preflight(void)
{
    unsigned char pvd[2048];struct jp_gd_media_info m;if(jp_gd_media_snapshot(&m)||!raw_read(45166,1,pvd))return 0;
    if(pvd[0]!=1||memcmp(pvd+1,"CD001",5)||pvd[6]!=1||pvd[128]!=0||pvd[129]!=8)return 0;
    uint32_t path_bytes=le32(pvd+132),path_fad=le32(pvd+140)+150,root=le32(pvd+158)+150,size=le32(pvd+166);
    uint64_t end=(uint64_t)m.first_fad+m.sectors;
    if(!path_bytes||path_bytes>1048576||path_fad<m.first_fad||(uint64_t)path_fad+(path_bytes+2047)/2048>end||
       !size||size>4194304||root<m.first_fad||(uint64_t)root+(size+2047)/2048>end)return 0;
    size_t bytes=((size_t)path_bytes+2047)&~(size_t)2047;unsigned char *table=malloc(bytes);if(!table)return 0;
    int valid=raw_read(path_fad,(uint32_t)(bytes/2048),table);uint32_t at=0,record=1;
    while(valid&&at<path_bytes){
        if(path_bytes-at<8){valid=0;break;}
        unsigned n=table[at],step=8+n+(n&1);uint32_t extent=le32(table+at+2);
        unsigned parent=table[at+6]|((unsigned)table[at+7]<<8);
        if(!n||n>31||step>path_bytes-at||table[at+1]||!parent||parent>record||
           (uint64_t)extent+150<m.first_fad||(uint64_t)extent+150>=end){valid=0;break;}
        at+=step;record++;
    }
    free(table);return valid&&at==path_bytes;
}
int jp_gdfs_configure(const wchar_t *path,uint32_t first,uint32_t raw,uint32_t chunk,uint32_t timeout)
{
    int rc=access(0);if(rc)return rc;if(fs.api_depth||fs.callback_depth)return JP_GDFS_REENTRANT;
    if(fs.info.phase>JP_GDFS_CONFIGURED)return JP_GDFS_STATE;
    if(first!=45150||(raw!=2048&&raw!=2352)||!chunk||chunk>65536||!timeout||timeout>60000)return JP_GDFS_ARGUMENT;
    rc=jp_gdfs_host_path(path);if(rc)return rc;fs.thread=jp_gdfs_host_thread();fs.first=first;fs.raw=raw;fs.chunk=chunk;fs.timeout=timeout;
    fs.info.phase=JP_GDFS_CONFIGURED;return result(0,0);
}
static int initialize(uint32_t handles,uint32_t entries)
{
    int rc=access(0);if(rc)return rc;if(fs.api_depth||fs.callback_depth)return JP_GDFS_REENTRANT;
    if(fs.info.phase!=JP_GDFS_CONFIGURED)return result(JP_GDFS_STATE,GDD_ERR_INIT);
    if(!handles||handles>1024||!entries||entries>8192)return result(JP_GDFS_ARGUMENT,GDD_ERR_INIT);
    fs.info.phase=JP_GDFS_STARTING;fs.info.max_handles=handles;fs.info.max_entries=entries;
    rc=jp_gd_media_open(jp_gdfs_host_path_get(),fs.first,fs.raw,fs.chunk);
    if(rc){fs.info.phase=JP_GDFS_FAILED;return result(JP_GDFS_MEDIA,GDD_ERR_INIT);}
    fs.owns_media=1;
    if(!preflight()){fs.info.phase=JP_GDFS_FAILED;return result(JP_GDFS_MEDIA,GDD_ERR_DISC);}
    size_t work=offsetof(GDFS_WORK,hndlist)+((size_t)handles+1)*sizeof(GDFS_HANDLE),dir=16+(size_t)entries*sizeof(GDFS_DIRREC_ENT);
    if(jp_gdfs_host_allocate(work,&fs.work)||jp_gdfs_host_allocate(dir,&fs.dir)||
       jp_gdfs_host_allocate(((size_t)handles+1)*sizeof(struct callback_record),&fs.callbacks)){
        fs.info.phase=JP_GDFS_FAILED;return result(JP_GDFS_MEMORY,GDD_ERR_INIT);
    }
    fs.records=fs.callbacks.data;fs.info.work_bytes=(uint32_t)work;fs.info.dir_bytes=(uint32_t)dir;
    fs.info.handle_bytes=sizeof(GDFS_HANDLE);fs.info.work=(uint64_t)(uintptr_t)fs.work.data;fs.info.dir=(uint64_t)(uintptr_t)fs.dir.data;
    jp_gdfs_source_bind_interface(1);fs.api_depth++;fs.in_directory=1;
    long err=SRC(gdFsInitG)((long)handles,fs.work.data,(long)entries,fs.dir.data);
    fs.in_directory=0;fs.api_depth--;fs.info.phase=err?JP_GDFS_FAILED:JP_GDFS_READY;
    if(err)return result(JP_GDFS_SOURCE,err);
    if(!linked()){fs.info.phase=JP_GDFS_FAILED;return result(JP_GDFS_CHAIN,GDD_ERR_INIT);}
    return result(0,0);
}
int jp_gdfs_initialize(uint32_t handles,uint32_t entries){return initialize(handles,entries);}
int jp_gdfs_status(void){int rc=access(0);return rc?rc:(fs.info.phase<JP_GDFS_STARTING?JP_GDFS_NOT_INITIALIZED:(int)fs.info.status);}
int jp_gdfs_snapshot(struct jp_gdfs_info *out)
{
    int rc=access(0);if(rc)return rc;if(!out||!output_valid(out,sizeof *out))return JP_GDFS_ARGUMENT;
    fs.info.live_handles=0;fs.info.dir_entries=0;
    if(gd_wk){for(uint32_t j=0;j<fs.info.max_handles;j++)if(gd_wk->hndtbl[j].used)fs.info.live_handles++;
        fs.info.dir_entries=(uint32_t)gd_wk->curdir->dir_num;}
    fs.info.vblank=jp_gdfs_source_chain_handle();fs.info.pending=command.kind!=0;*out=fs.info;return 0;
}
int jp_gdfs_shutdown(void)
{
    int rc=access(0);if(rc)return rc;if(fs.api_depth||fs.callback_depth)return JP_GDFS_REENTRANT;
    if(fs.info.phase<JP_GDFS_STARTING){free_directories();return 0;}fs.closing=1;
    if(fs.owns_media&&drain())return result(JP_GDFS_TIMEOUT,GDD_ERR_TOUT);
    uint32_t h=jp_gdfs_source_chain_handle();if(h&&!linked())return result(JP_GDFS_CHAIN,GDD_ERR_INIT);
    if(gd_wk){SRC(gdFsFinishG)();if(h&&jp_vblank_chain_status())return result(JP_GDFS_CHAIN,GDD_ERR_INIT);}
    jp_gdfs_source_bind_interface(0);
    if(fs.owns_media){rc=jp_gd_media_close(fs.timeout);if(rc)return result(JP_GDFS_MEDIA,GDD_ERR_READ);fs.owns_media=0;}
    if(jp_gdfs_host_free(&fs.callbacks)||jp_gdfs_host_free(&fs.dir)||jp_gdfs_host_free(&fs.work))return result(JP_GDFS_MEMORY,GDD_ERR_INIT);
    fs.records=NULL;free_directories();fs.closing=0;fs.info.phase=JP_GDFS_CONFIGURED;fs.info.work=fs.info.dir=0;fs.info.live_handles=fs.info.dir_entries=fs.info.pending=fs.info.vblank=0;
    return result(0,0);
}
static int begin(void)
{int rc=access(1);if(rc)return rc;if(fs.closing)return JP_GDFS_STATE;if(!linked())return JP_GDFS_CHAIN;fs.api_depth++;return 0;}
static void end(void){fs.api_depth--;}
static long bad_handle(void){result(JP_GDFS_ARGUMENT,GDD_ERR_ILLHNDL);return GDD_ERR_ILLHNDL;}
long gdFsInit(long handles,void *legacy_work,long entries,void *legacy_dir)
{(void)legacy_work;(void)legacy_dir;int rc=initialize((uint32_t)handles,(uint32_t)entries);return rc?(fs.info.source_error?fs.info.source_error:GDD_ERR_INIT):GDD_ERR_OK;}
void gdFsFinish(void){(void)jp_gdfs_shutdown();}
long gdFsReinit(void)
{
    if(begin())return GDD_ERR_INIT;if(fs.callback_depth||fs.api_depth>1){end();return GDD_ERR_BUSY;}
    if(drain()){end();return GDD_ERR_TOUT;}
    fs.in_directory=1;long rc=SRC(gdFsReinit)();fs.in_directory=0;fs.info.phase=rc?JP_GDFS_FAILED:JP_GDFS_READY;
    result(rc?JP_GDFS_SOURCE:0,rc);end();return rc;
}
GDFS gdFsOpen(const char *name,GDFS_DIRREC directory)
{
    if(begin())return NULL;
    if(!name_valid(name,0)){result(JP_GDFS_ARGUMENT,GDD_ERR_NOTFOUND);end();return NULL;}
    GDFS_DIRREC table=directory?directory:gd_wk->curdir;
    if(!directory_valid(table)){result(JP_GDFS_ARGUMENT,GDD_ERR_ILLREQ);end();return NULL;}
    long found=SRC(gdFsSearchName)(name,table,FALSE);
    if(found){GDFS_DIRREC_ENT *entry=&table->dirrec_tbl[found-1];struct jp_gd_media_info m;
        uint64_t fad=(uint64_t)(uint32_t)entry->sector+150;
        if(jp_gd_media_snapshot(&m)||entry->fsize<0||entry->fsize>INT32_MAX-2047||
           (entry->fsize&&(fad<m.first_fad||fad+(uint32_t)(entry->fsize+2047)/2048>(uint64_t)m.first_fad+m.sectors))){
            SRC(gdFsSetGlobalErr)(GDD_ERR_SIZEOVER);result(JP_GDFS_ARGUMENT,GDD_ERR_SIZEOVER);end();return NULL;
        }
    }
    GDFS h=SRC(gdFsOpen)(name,directory);
    if(h){size_t n=(size_t)(h-gd_wk->hndlist);memset(&fs.records[n],0,sizeof fs.records[n]);fs.records[n].handle=h;}
    end();return h;
}
GDFS gdFsOpenFast(const char *name,GDFS_DIRREC directory){return gdFsOpen(name,directory);}
GDFS gdFsOpenRange(long fad,long sectors)
{
    if(begin())return NULL;struct jp_gd_media_info m;
    if(jp_gd_media_snapshot(&m)||fad<(long)m.first_fad||sectors<=0||sectors>INT32_MAX/2048||
       (uint64_t)fad+sectors>(uint64_t)m.first_fad+m.sectors){result(JP_GDFS_ARGUMENT,GDD_ERR_OFS);end();return NULL;}
    GDFS h=SRC(gdFsOpenRange)(fad,sectors);if(h){size_t n=(size_t)(h-gd_wk->hndlist);memset(&fs.records[n],0,sizeof fs.records[n]);fs.records[n].handle=h;}end();return h;
}
long gdFsLoadDir(const char *name,GDFS_DIRREC directory)
{
    if(begin())return GDD_ERR_INIT;
    if(fs.in_directory||fs.callback_depth||!name_valid(name,1)){end();return GDD_ERR_ILLREQ;}
    if(gd_wk->now_work){end();return GDD_ERR_BUSY;}
    GDFS_DIRREC target=directory?directory:gd_wk->curdir;
    long capacity=directory_capacity(target);if(!capacity){end();return GDD_ERR_ILLREQ;}
    size_t bytes=16+(size_t)capacity*sizeof(GDFS_DIRREC_ENT);GDFS_DIRREC scratch=calloc(1,bytes);if(!scratch){end();return GDD_ERR_READ;}
    SRC(gdFsCreateDirhn)(scratch,capacity);fs.in_directory=1;
    long rc=SRC(gdFsLoadDir)(name,scratch);fs.in_directory=0;
    if(!rc)memcpy(target,scratch,bytes);free(scratch);result(rc?JP_GDFS_SOURCE:0,rc);end();return rc;
}
long gdFsLoadDirFast(const char *name,GDFS_DIRREC directory){return gdFsLoadDir(name,directory);}
long gdFsChangeDir(const char *name){return gdFsLoadDir(name,NULL);}
GDFS_DIRREC gdFsCreateDirhn(void *buffer,long capacity)
{
    if(access(0)||!buffer||capacity<=0||capacity>8192||!output_valid(buffer,16+(size_t)capacity*44))return NULL;
    if(!fs.thread)fs.thread=jp_gdfs_host_thread();struct directory_record *r=fs.directories;
    while(r&&r->table!=buffer)r=r->next;
    if(!r){r=malloc(sizeof *r);if(!r)return NULL;r->table=buffer;r->next=fs.directories;fs.directories=r;}
    r->capacity=capacity;return SRC(gdFsCreateDirhn)(buffer,capacity);
}
long gdFsSetDir(GDFS_DIRREC table)
{if(begin())return GDD_ERR_INIT;if(!directory_valid(table)){end();return GDD_ERR_ILLREQ;}long rc=SRC(gdFsSetDir)(table);end();return rc;}
long gdFsGetDirInfo(const char *name,GDFS_DIRINFO *out)
{
    if(begin())return GDD_ERR_INIT;if(!name_valid(name,0)||!output_valid(out,sizeof *out)){end();return GDD_ERR_ILLREQ;}
    GDFS_DIRINFO value={0};long rc=SRC(gdFsGetDirInfo)(name,&value);if(!rc)*out=value;end();return rc;
}
void gdFsExecServer(void){if(begin())return;if(!fs.callback_depth)SRC(gdFsExecServer)();end();}
long gdFsGetGlobalErr(void){if(access(1))return GDD_ERR_INIT;return SRC(gdFsGetGlobalErr)();}
long gdFsSetGlobalErr(long error){if(access(1))return GDD_ERR_INIT;return SRC(gdFsSetGlobalErr)(error);}
GDFS gdFsGetWorkHn(void){return access(1)?NULL:SRC(gdFsGetWorkHn)();}
GDFS gdFsGetSysHn(void){return access(1)?NULL:SRC(gdFsGetSysHn)();}
#define QUERY(name,expression) long name(GDFS h){if(access(1)||!handle_valid(h,1))return bad_handle();return (expression);}
QUERY(gdFsGetStat,SRC(gdFsGetStat)(h))
QUERY(gdFsGetErrStat,SRC(gdFsGetErrStat)(h))
QUERY(gdFsGetNumRd,SRC(gdFsGetNumRd)(h))
QUERY(gdFsGetTransStat,SRC(gdFsGetTransStat)(h))
QUERY(gdFsTell,SRC(gdFsTell)(h))
#undef QUERY
#define OUTPUT_QUERY(name) Bool name(GDFS h,long *out){if(access(1)||!handle_valid(h,1)||!output_valid(out,4))return FALSE;return SRC(name)(h,out);}
OUTPUT_QUERY(gdFsGetFileSize)
OUTPUT_QUERY(gdFsGetFileSctSize)
OUTPUT_QUERY(gdFsGetFileFad)
#undef OUTPUT_QUERY
long gdFsCheckEof(GDFS h,Bool *out)
{if(access(1)||!handle_valid(h,1)||!output_valid(out,sizeof *out))return bad_handle();return SRC(gdFsCheckEof)(h,out);}
long gdFsSeek(GDFS h,long sector,long origin)
{
    if(begin())return GDD_ERR_INIT;if(!handle_valid(h,1)){end();return bad_handle();}
    if(gd_wk->now_work==h){end();return GDD_ERR_BUSY;}
    int64_t base=origin==0?0:origin==1?h->ofs:origin==2?h->fsctsize:INT64_MAX;
    if(base==INT64_MAX||base+sector<INT32_MIN||base+sector>INT32_MAX){end();return GDD_ERR_SEEK;}
    long rc=SRC(gdFsSeek)(h,sector,origin);end();return rc;
}
static int read_valid(GDFS h,long sectors,void *out,int needs_buffer)
{
    if(!handle_valid(h,1)||sectors<0||command.next_id<=0||command.next_id>=INT32_MAX)return 0;
    long n=h->fsctsize-h->ofs;if(n>sectors)n=sectors;
    return !needs_buffer||n<=0||output_valid(out,(size_t)n*2048);
}
static long bounded_sectors(GDFS h,long sectors)
{long available=h->fsctsize-h->ofs;return sectors>available?available:sectors;}
long gdFsRead(GDFS h,long sectors,void *out)
{
    if(begin())return GDD_ERR_INIT;if(!read_valid(h,sectors,out,1)){end();return bad_handle();}
    if(fs.callback_depth){end();return GDD_ERR_BUSY;}
    uint64_t start=jp_gdfs_host_time();size_t previous=command.copied;
    while(gd_wk->now_work){SRC(gdFsExecServer)();if(!gd_wk->now_work)break;
        if(command.copied!=previous){previous=command.copied;start=jp_gdfs_host_time();}
        if(jp_gdfs_expired(start)){end();return GDD_ERR_TOUT;}jp_gdfs_host_yield();}
    long rc=SRC(gdFsRead)(h,bounded_sectors(h,sectors),out);result(rc?JP_GDFS_SOURCE:0,rc);end();return rc;
}
long gdFsReqRd32(GDFS h,long sectors,void *out)
{if(begin())return GDD_ERR_INIT;if(!read_valid(h,sectors,out,1)){end();return bad_handle();}long rc=SRC(gdFsReqRd32)(h,bounded_sectors(h,sectors),out);end();return rc;}
long gdFsReqGdRd(GDFS h,long sectors)
{if(begin())return GDD_ERR_INIT;if(!read_valid(h,sectors,NULL,0)){end();return bad_handle();}long rc=SRC(gdFsReqGdRd)(h,bounded_sectors(h,sectors));end();return rc;}
long gdFsTrans32(GDFS h,long bytes,void *out)
{if(begin())return GDD_ERR_INIT;if(!handle_valid(h,1)||bytes<0||(bytes&&!output_valid(out,(size_t)bytes))){end();return bad_handle();}long rc=SRC(gdFsTrans32)(h,bytes,out);end();return rc;}
long gdFsSetTmode(GDFS h,long mode)
{if(begin())return GDD_ERR_INIT;if(!handle_valid(h,1)){end();return bad_handle();}if(mode!=0&&mode!=1){end();return GDD_ERR_ILLTMODE;}long rc=SRC(gdFsSetTmode)(h,mode);end();return rc;}
long gdFsStopRd(GDFS h)
{if(begin())return GDD_ERR_INIT;if(!handle_valid(h,1)){end();return bad_handle();}if(fs.callback_depth&&gd_wk->now_work==h){end();return GDD_ERR_BUSY;}long rc=SRC(gdFsStopRd)(h);end();return rc;}
void gdFsClose(GDFS h)
{if(begin())return;if(handle_valid(h,1)&&!(fs.callback_depth&&gd_wk->now_work==h)){if(command.pending_handle==h){command.pending_callback=NULL;command.pending_arg=NULL;command.pending_handle=NULL;}SRC(gdFsClose)(h);if(!h->used){size_t n=(size_t)(h-gd_wk->hndlist);memset(&fs.records[n],0,sizeof fs.records[n]);}}end();}
long gdFsMovePickup(GDFS h)
{if(begin())return GDD_ERR_INIT;if(!handle_valid(h,1)){end();return bad_handle();}if(fs.callback_depth){end();return GDD_ERR_BUSY;}long rc=SRC(gdFsMovePickup)(h);end();return rc;}
void gdFsEntryRdEndFunc(GDFS h,GDFS_FUNC fn,void *arg)
{if(begin())return;if(handle_valid(h,1)){struct callback_record *r=&fs.records[h-gd_wk->hndlist];r->read=fn;r->read_arg=arg;SRC(gdFsEntryRdEndFunc)(h,fn?read_callback:NULL,r);}end();}
void gdFsEntryTrEndFunc(GDFS h,GDFS_FUNC fn,void *arg)
{if(begin())return;if(handle_valid(h,1)){struct callback_record *r=&fs.records[h-gd_wk->hndlist];r->transfer=fn;r->transfer_arg=arg;SRC(gdFsEntryTrEndFunc)(h,fn,arg);}end();}
void gdFsEntryErrFunc(GDFS h,GDFS_ERRFUNC fn,void *arg)
{if(begin())return;if(handle_valid(h,1)){struct callback_record *r=&fs.records[h-gd_wk->hndlist];r->error=fn;r->error_arg=arg;SRC(gdFsEntryErrFunc)(h,fn?error_callback:NULL,r);}end();}
void gdFsEntryErrFuncAll(GDFS_ERRFUNC fn,void *arg)
{if(begin())return;fs.global_error=fn;fs.global_arg=arg;SRC(gdFsEntryErrFuncAll)(fn?global_callback:NULL,NULL);end();}
long gdFsGetDrvStat(void){if(begin())return GDD_DRVSTAT_CANTREAD;long rc=SRC(gdFsGetDrvStat)();end();return rc;}
Bool gdFsIsTrayOpen(void){if(begin())return FALSE;Bool rc=SRC(gdFsIsTrayOpen)();end();return rc;}
long gdFsReqDrvStat(void){if(begin())return GDD_ERR_INIT;long rc=SRC(gdFsReqDrvStat)();end();return rc;}
/* Refused requests are not successful emulated firmware/audio operations. */
long gdFsGetToc(long type,void *out){(void)type;(void)out;if(!access(0))result(JP_GDFS_UNSUPPORTED,GDD_ERR_UNDEF);return GDD_ERR_UNDEF;}
long gdFsDaPlay(long a,long b,long c){(void)a;(void)b;(void)c;if(!access(0))result(JP_GDFS_UNSUPPORTED,GDD_ERR_UNDEF);return GDD_ERR_UNDEF;}
long gdFsDaPlaySct(long a,long b,long c){return gdFsDaPlay(a,b,c);}
long gdFsDaStop(void){if(!access(0))result(JP_GDFS_UNSUPPORTED,GDD_ERR_UNDEF);return GDD_ERR_UNDEF;}
long gdFsDaPause(void){return gdFsDaStop();}
long gdFsDaRelease(long n){(void)n;return gdFsDaStop();}
long gdFsDaGetInfo(GDFS_DAINFO *out){(void)out;return gdFsDaStop();}

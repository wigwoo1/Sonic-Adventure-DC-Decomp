/* Authored diagnostic boundary, appended to private original directory C.
 * This is not a second production GDFS API or a game filesystem success stub. */
#include <stdlib.h>
#include <string.h>
static unsigned iso_reads,iso_budget;
static int valid_record(const Uint8 *p,size_t available)
{
    if(available<34||p[0]<34||p[0]>available||p[32]+33U>p[0])return 0;
    unsigned n=p[32];if(!n)return 0;
    if(p[25]&GDD_FF_DIRECTORY){if(n>31)return 0;}
    else if(n<2||n>33||p[33+n-2]!=';'||p[33+n-1]<'0'||p[33+n-1]>'9')return 0;
    return 1;
}
static int valid_directory(const Uint8 *p)
{
    size_t at=0;while(at<2048 && p[at]){
        if(!valid_record(p+at,2048-at))return 0;at+=p[at];
    }
    return 1;
}
Sint32 gdFsReadW(Sint32 fad,Sint32 count,void *out)
{
    if(!out||fad<0||count!=1||!iso_budget)return GDD_ERR_READ;
    iso_budget--;iso_reads++;
    int rc=jp_gd_media_request((uint32_t)fad,1);if(rc)return GDD_ERR_READ;
    rc=jp_gd_media_wait(5000);
    if(rc){(void)jp_gd_media_cancel(100);(void)jp_gd_media_release();return GDD_ERR_READ;}
    const void *data=NULL;size_t bytes=0;rc=jp_gd_media_view(&data,&bytes);
    if(!rc && bytes==2048 && valid_directory(data))memcpy(out,data,bytes);
    else rc=JP_GD_FORMAT;
    int release=jp_gd_media_release();return rc||release?GDD_ERR_READ:GDD_ERR_OK;
}
__declspec(dllexport) int peer_parse_record(const void *data,size_t available,void *out)
{
    if(!data||!out||!valid_record(data,available))return -1;
    return read_dirrec((Uint8 *)data,out);
}
__declspec(dllexport) int peer_list(long fad,long bytes,long capacity,void *out,size_t out_bytes)
{
    if(fad<150||bytes<=0||bytes>1048576||capacity<=0||capacity>8192||!out)return GDD_ERR_SIZE;
    size_t size=16+(size_t)capacity*sizeof(GDFS_DIRREC_ENT);if(out_bytes<size)return GDD_ERR_SIZE;
    GDFS_DIRREC table=calloc(1,size);if(!table)return GDD_ERR_READ;
    Uint8 buffer[2048];gdFsCreateDirhn(table,capacity);iso_reads=0;iso_budget=(unsigned)((bytes+2047)/2048)+1;
    int rc=(int)gdFsGetDirEnt(buffer,fad,table,bytes);
    if(!rc)memcpy(out,table,size);free(table);return rc;
}
__declspec(dllexport) int peer_search(const char *name,const void *data,long capacity,int exact)
{
    if(!name||!data||capacity<=0||capacity>8192||(exact!=0&&exact!=1))return -1;
    GDFS_DIRREC table=(GDFS_DIRREC)data;if(table->dir_num<0||table->dir_num>capacity)return -1;
    unsigned n=0;while(n<32 && name[n])n++;if(n==32)return -1;
    for(long j=0;j<table->dir_num;j++){
        unsigned length=0;while(length<32&&table->dirrec_tbl[j].fname[length])length++;
        if(length==32)return -1;
    }
    return gdFsSearchName(name,table,exact);
}
__declspec(dllexport) unsigned peer_sector_reads(void){return iso_reads;}
__declspec(dllexport) unsigned peer_entry_bytes(void){return sizeof(GDFS_DIRREC_ENT);}

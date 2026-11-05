/* Original VMU file envelopes, not flat SONICADV_JP.SAV records or card I/O. */
#include "jp_native_backup_image.h"
#include <stdlib.h>
#include <string.h>
#define IMAGE_LIMIT (1024u*1024u)
static const size_t visuals[4]={0,8064,4544,2048};
static int span(const void *p,size_t n)
{return p && n<=UINTPTR_MAX-(uintptr_t)p;}
static int overlap(const void *a,size_t an,const void *b,size_t bn)
{return an && bn && (uintptr_t)a<(uintptr_t)b+bn && (uintptr_t)b<(uintptr_t)a+an;}
static size_t length(uint32_t icons,uint32_t visual,uint32_t data)
{
    if(!icons || icons>3 || visual>3)return 0;
    uint64_t total=128u+(uint64_t)icons*512u+visuals[visual]+data;
    return total<=IMAGE_LIMIT?(size_t)total:0;
}
int32_t jp_backup_image_build(void *output,size_t capacity,
                             const BUS_BACKUPFILEHEADER *header)
{
    if(!span(header,sizeof *header))return (int32_t)BUD_ERR_INVALID_PARAM;
    size_t n=length(header->icon_num,header->visual_type,header->save_size);
    if(!n)return (int32_t)BUD_ERR_INVALID_PARAM;
    size_t padded=(n+511)&~(size_t)511;
    if(capacity<padded || !span(output,padded) || overlap(output,padded,header,sizeof *header))
        return (int32_t)BUD_ERR_INVALID_PARAM;
    const void *parts[4]={header->icon_palette,header->icon_data,header->visual_data,header->save_data};
    size_t sizes[4]={32,(size_t)header->icon_num*512,visuals[header->visual_type],header->save_size};
    for(unsigned i=0;i<4;i++)if(sizes[i] && (!span(parts[i],sizes[i]) || overlap(output,padded,parts[i],sizes[i])))
        return (int32_t)BUD_ERR_INVALID_PARAM;
    return (int32_t)buMakeBackupFileImage(output,header);
}
int32_t jp_backup_image_analyze(BUS_BACKUPFILEHEADER *header,const void *image,size_t bytes)
{
    if(!span(header,sizeof *header) || bytes<128 || bytes>IMAGE_LIMIT ||
       !span(image,bytes) || overlap(header,sizeof *header,image,bytes))
        return (int32_t)BUD_ERR_INVALID_PARAM;
    const unsigned char *in=image;
    uint32_t icons=in[64]|(uint32_t)in[65]<<8,visual=in[68]|(uint32_t)in[69]<<8;
    uint32_t data=in[72]|(uint32_t)in[73]<<8|(uint32_t)in[74]<<16|(uint32_t)in[75]<<24;
    size_t n=length(icons,visual,data);
    if(!n)return (int32_t)BUD_ERR_BUPFILE_ILLEGAL;
    if(n>bytes)return (int32_t)BUD_ERR_INVALID_PARAM;
    void *copy=malloc(n);if(!copy)return (int32_t)BUD_ERR_GENERIC;
    memcpy(copy,image,n);BUS_BACKUPFILEHEADER parsed;memcpy(&parsed,header,sizeof parsed);
    Sint32 result=buAnalyzeBackupFileImage(&parsed,copy);
    if(result==BUD_ERR_OK || result==(Sint32)BUD_ERR_BUPFILE_CRC){
        parsed.icon_palette=(void *)(in+96);parsed.icon_data=(void *)(in+128);
        parsed.visual_data=visual?(void *)(in+128+icons*512u):NULL;
        parsed.save_data=data?(void *)(in+128+icons*512u+visuals[visual]):NULL;
        memcpy(header,&parsed,sizeof parsed);
    }
    free(copy);return (int32_t)result;
}

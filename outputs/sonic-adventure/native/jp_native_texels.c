/* Layout constants from pinned sdk/ninja/kmtex_kamui_lib.c and njPalette.c.
 * Morton addressing and channel expansion are authored mathematical routines.
 */
#include "jp_native_texels.h"
#include <string.h>
static size_t morton(unsigned x,unsigned y,unsigned w,unsigned h)
{
    size_t address=0;unsigned outbit=0;
    for(unsigned bit=1;bit<w || bit<h;bit<<=1){
        if(bit<h){if(y&bit)address|=(size_t)1<<outbit;++outbit;}
        if(bit<w){if(x&bit)address|=(size_t)1<<outbit;++outbit;}
    }
    return address;
}
int jp_texel_describe(uint32_t tsp,uint32_t tcw,unsigned level,unsigned stride,
                      struct jp_texel_level *out)
{
    static const size_t off16[11]={6,8,16,48,176,688,2736,10928,43696,174768,699056};
    static const size_t off8[11]={3,4,8,24,88,344,1368,5464,21848,87384,349528};
    static const size_t off4[11]={1,2,4,12,44,172,684,2732,10924,43692,174764};
    static const size_t vqend[11]={1,2,6,22,86,342,1366,5462,21846,87382,349526};
    if(!out)return JP_TEXEL_ARGUMENT;
    unsigned w=8u<<((tsp>>3)&7),h=8u<<(tsp&7),fmt=(tcw>>27)&7;
    unsigned mip=tcw>>31,vq=(tcw>>30)&1,pal=fmt==5 || fmt==6;
    unsigned linear=!pal && (tcw&0x04000000),use_stride=!pal && (tcw&0x02000000);
    if(fmt==3 || fmt==4 || fmt==7 || (vq && pal))return JP_TEXEL_UNSUPPORTED;
    if((mip||vq) && (w!=h||linear||use_stride))return JP_TEXEL_UNSUPPORTED;
    if(use_stride && (!linear || stride<w || stride>992 || stride%32))return JP_TEXEL_ARGUMENT;
    unsigned log=0;while((1u<<log)<w)++log;
    unsigned levels=mip?log+1:1;
    if(level>=levels)return JP_TEXEL_ARGUMENT;
    if(mip){w>>=level;h>>=level;log-=level;}
    if(vq && w==1)return JP_TEXEL_UNSUPPORTED;
    size_t count=(size_t)w*h,offset=0,encoded;
    if(vq){offset=2048+(mip && log?vqend[log-1]:0);encoded=count/4;}
    else{
        if(mip)offset=fmt==5?off4[log]:fmt==6?off8[log]:off16[log];
        if(use_stride)count=(size_t)(h-1)*stride+w;
        encoded=fmt==5?(count+1)/2:fmt==6?count:count*2;
    }
    struct jp_texel_level result={w,h,fmt,levels,offset,offset+encoded,(size_t)w*h*4};
    *out=result;return JP_TEXEL_OK;
}
static unsigned expand5(unsigned c){return (c<<3)|(c>>2);}
static unsigned expand6(unsigned c){return (c<<2)|(c>>4);}
static void unpack(unsigned char *p,unsigned value,unsigned format)
{
    if(format==0){p[0]=(unsigned char)expand5((value>>10)&31);p[1]=(unsigned char)expand5((value>>5)&31);p[2]=(unsigned char)expand5(value&31);p[3]=(value&0x8000)?255:0;}
    else if(format==1){p[0]=(unsigned char)expand5((value>>11)&31);p[1]=(unsigned char)expand6((value>>5)&63);p[2]=(unsigned char)expand5(value&31);p[3]=255;}
    else{p[0]=(unsigned char)(((value>>8)&15)*17);p[1]=(unsigned char)(((value>>4)&15)*17);p[2]=(unsigned char)((value&15)*17);p[3]=(unsigned char)((value>>12)*17);}
}
static int overlaps(const void *a,size_t as,const void *b,size_t bs)
{
    uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;
    if(!as||!bs)return 0;
    if(as>UINTPTR_MAX-x||bs>UINTPTR_MAX-y)return 1;
    return x<y+bs && y<x+as;
}
int jp_texel_decode(uint32_t tsp,uint32_t tcw,unsigned level,unsigned stride,
    const void *source,size_t source_bytes,const uint32_t *palette,size_t entries,
    void *rgba,size_t rgba_bytes,struct jp_texel_level *info)
{
    struct jp_texel_level description;int result=jp_texel_describe(tsp,tcw,level,stride,&description);
    if(result)return result;
    if(!source||!rgba||!info)return JP_TEXEL_ARGUMENT;
    if(source_bytes<description.required_source_bytes)return JP_TEXEL_SOURCE;
    if(rgba_bytes<description.rgba_bytes)return JP_TEXEL_OUTPUT;
    unsigned fmt=description.format,pal=fmt==5 || fmt==6,vq=(tcw>>30)&1;
    unsigned linear=!pal && (tcw&0x04000000),use_stride=!pal && (tcw&0x02000000);
    unsigned bank=fmt==5?((tcw>>21)&63)*16:fmt==6?((tcw>>25)&3)*256:0;
    size_t palette_need=pal?(size_t)bank+(fmt==5?16:256):0;
    if(pal && (!palette||entries<palette_need))return JP_TEXEL_PALETTE;
    if(overlaps(source,description.required_source_bytes,rgba,description.rgba_bytes)||
       overlaps(info,sizeof *info,source,description.required_source_bytes)||
       overlaps(info,sizeof *info,rgba,description.rgba_bytes)||
       (pal && (overlaps(palette,palette_need*4,rgba,description.rgba_bytes)||overlaps(palette,palette_need*4,info,sizeof *info))))return JP_TEXEL_ARGUMENT;
    const unsigned char *src=source;unsigned char *dst=rgba;
    unsigned w=description.width,h=description.height;
    for(unsigned y=0;y<h;y++)for(unsigned x=0;x<w;x++){
        size_t pixel=linear?(size_t)y*(use_stride?stride:w)+x:morton(x,y,w,h);
        unsigned char *p=dst+((size_t)y*w+x)*4;
        if(pal){
            unsigned index=fmt==5?(src[description.encoded_offset+pixel/2]>>((pixel&1)*4))&15:src[description.encoded_offset+pixel];
            uint32_t c=palette[bank+index];p[0]=(unsigned char)(c>>16);p[1]=(unsigned char)(c>>8);p[2]=(unsigned char)c;p[3]=(unsigned char)(c>>24);
        }else{
            size_t address;
            if(vq){
                unsigned code=src[description.encoded_offset+morton(x/2,y/2,w/2,h/2)];
                address=(size_t)code*8+((x&1)*2+(y&1))*2;
            }else address=description.encoded_offset+pixel*2;
            unsigned value=(unsigned)src[address]|((unsigned)src[address+1]<<8);unpack(p,value,fmt);
        }
    }
    *info=description;return JP_TEXEL_OK;
}

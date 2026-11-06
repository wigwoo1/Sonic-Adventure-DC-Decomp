/* Extend, never rewrite, the qualified decoder. All prior paths still call
 * that unchanged implementation. VQ's 1x1 level uses index byte0 and texel3
 * (bottom-right) of that codebook vector. Primary implementation reference:
 * flyinghead/flycast 7adbf3caef251b153e348df3f469a0f35878339d,
 * core/rend/TexCache.cpp VQMipPoint and mipmapped conversion. Format facts
 * only; no Flycast code is included. This is not a console capture claim. */
#include "jp_native_texels.h"
#define jp_texel_describe jp_texel_legacy_describe
#define jp_texel_decode jp_texel_legacy_decode
#include "jp_native_texels.c"
#undef jp_texel_describe
#undef jp_texel_decode
int jp_texel_describe(uint32_t tsp,uint32_t tcw,unsigned level,unsigned stride,
                     struct jp_texel_level *out)
{
    int rc=jp_texel_legacy_describe(tsp,tcw,level,stride,out);
    if(rc!=JP_TEXEL_UNSUPPORTED||!level||(tcw&UINT32_C(0xc0000000))!=UINT32_C(0xc0000000))return rc;
    struct jp_texel_level parent;
    if(jp_texel_legacy_describe(tsp,tcw,level-1,stride,&parent)||parent.width!=2||parent.height!=2)return rc;
    *out=(struct jp_texel_level){1,1,parent.format,parent.mip_count,2048,2049,4};return 0;
}
int jp_texel_decode(uint32_t tsp,uint32_t tcw,unsigned level,unsigned stride,
    const void *source,size_t source_bytes,const uint32_t *palette,size_t entries,
    void *rgba,size_t rgba_bytes,struct jp_texel_level *info)
{
    struct jp_texel_level result;int rc=jp_texel_describe(tsp,tcw,level,stride,&result);
    if(rc)return rc;
    if(result.width!=1||!(tcw&UINT32_C(0x40000000)))return jp_texel_legacy_decode(tsp,tcw,level,stride,source,source_bytes,palette,entries,rgba,rgba_bytes,info);
    if(!source||!rgba||!info)return JP_TEXEL_ARGUMENT;
    if(source_bytes<result.required_source_bytes)return JP_TEXEL_SOURCE;
    if(rgba_bytes<4)return JP_TEXEL_OUTPUT;
    if(overlaps(source,result.required_source_bytes,rgba,4)||overlaps(info,sizeof *info,source,result.required_source_bytes)||overlaps(info,sizeof *info,rgba,4))return JP_TEXEL_ARGUMENT;
    const unsigned char *bytes=source;size_t at=(size_t)bytes[2048]*8+6;
    unsigned value=(unsigned)bytes[at]|((unsigned)bytes[at+1]<<8);unpack(rgba,value,result.format);*info=result;return 0;
}

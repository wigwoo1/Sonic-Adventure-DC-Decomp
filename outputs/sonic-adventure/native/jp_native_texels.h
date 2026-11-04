/* Authored bounded conversion of encoded native surface bytes to RGBA8.
 * Input is the GPU surface layout, NOT a PVR file (file loaders may need to
 * expand small VQ books or insert the 16-bit mip chain's leading four bytes).
 * Palette entries are explicit packed 0xAARRGGBB, not borrowed global state.
 */
#ifndef JP_NATIVE_TEXELS_H
#define JP_NATIVE_TEXELS_H
#include "jp_native_packets.h"
enum jp_texel_error { JP_TEXEL_OK,JP_TEXEL_ARGUMENT,JP_TEXEL_UNSUPPORTED,
    JP_TEXEL_SOURCE,JP_TEXEL_OUTPUT,JP_TEXEL_PALETTE };
struct jp_texel_level {
    uint32_t width,height,format,mip_count;
    size_t encoded_offset,required_source_bytes,rgba_bytes;
};
JP_PACKET_API int jp_texel_describe(uint32_t tsp,uint32_t tcw,unsigned level,
    unsigned stride_pixels,struct jp_texel_level *out);
/* Failure preserves output pixels and *info. Inputs, output and info must be
 * separate storage. Full source/output/palette bounds checked before writes.
 * Supports RGB1555/565/4444 linear, rectangular twiddled, stride, square mips,
 * standard VQ and expanded small VQ, PAL4/8 banks with explicit palettes.
 * YUV, bump, reserved formats, palette+VQ, and the VQ 1x1 mip (pending primary
 * corner-selection qualification) return UNSUPPORTED rather than fake pixels.
 */
JP_PACKET_API int jp_texel_decode(uint32_t tsp,uint32_t tcw,unsigned level,
    unsigned stride_pixels,const void *source,size_t source_bytes,
    const uint32_t *palette,size_t palette_entries,void *rgba,size_t rgba_bytes,
    struct jp_texel_level *info);
#endif

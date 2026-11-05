/* Authored bounded replacement for SH4 store queues, not a font/text engine.
 * All glyph selection, formatting and vertex writes use the original SDK.
 * Fixed RZ/DN1 numeric values; no emulation of FPSCR flags or exceptions. */
#include "jp_native_print_transport.h"
#include "jp_native_random_pick.h"
/* Do not include the separate math ABI's matrix declarations alongside
 * Ninja's native-width SDK declarations. This existing scalar service takes
 * and returns only fixed-width value words. */
extern uint32_t jp_rz_scalar32_multiply(uint32_t,uint32_t);
#include <stdlib.h>
#include <string.h>
static uint32_t *packet;
static size_t packet_words;
static int print_error;
static int error(int value){print_error=value;return 0;}
int jp_print_status(void){return print_error;}
void jp_print_clear_error(void){if(!packet)print_error=JP_PRINT_OK;}
int jp_print_texture_arguments(const void *texture,uint32_t size,uint32_t attr)
{
    if(packet || jp_queue_status()!=JP_QUEUE_OK || jp_textures_status()!=JP_TEXTURE_OK)
        return error(JP_PRINT_STATE);
    if(texture && (size<8 || size>1024 || (size&(size-1)) ||
                   (attr&0xff00u)!=0x100u || (attr&0xffu)>2 || (attr&0xffff0000u)))
        return error(JP_PRINT_ARGUMENT);
    print_error=JP_PRINT_OK;return 1;
}
int jp_print_create_surface(NJS_TEXSURFACE *s,uint32_t w,uint32_t h,uint32_t attr)
{
    /* Default and caller fonts: square, twiddled, 16-bit 1555/565/4444.
     * This is the native allocation boundary, not general kmCreate support.
     * The previous resource remains owned until the exact payload is copied. */
    s->Type=2;s->BitDepth=1;s->PixelFormat=(attr&0xffu)<<27;
    s->nWidth=w;s->nHeight=h;s->TextureSize=w*h*2;s->fSurfaceFlags=4;
    s->pSurface=s->pVirtual=s->pPhysical=NULL;
    return 1;
}
int jp_print_load_surface(NJS_TEXSURFACE *s,const void *data)
{
    if(jp_texture_replace_standalone_surface(s,data,s->TextureSize))return error(JP_PRINT_TEXTURE);
    return 1;
}
int jp_print_string_count(const char *s)
{
    if(!s)return error(JP_PRINT_ARGUMENT),-1;
    /* Original strlen is narrowed to signed 16 bits. Reject longer native
     * strings rather than scan unbounded memory or reproduce that overflow. */
    for(unsigned i=0;i<=32767;i++)if(!s[i])return (int)i;
    return error(JP_PRINT_ARGUMENT),-1;
}
int jp_print_digit_count(int count,int maximum)
{
    if(count<0 || count>maximum)return error(JP_PRINT_ARGUMENT),-1;
    return count;
}
uint32_t *jp_print_open(uint32_t glyphs)
{
    if(packet || jp_queue_status()!=JP_QUEUE_OK)return error(JP_PRINT_STATE),NULL;
    if(glyphs>32767)return error(JP_PRINT_ARGUMENT),NULL;
    packet_words=(size_t)glyphs*32;
    /* One spare block lets the unchanged backwards stores form their
     * one-past pointers even for a zero-length print. */
    packet=calloc(packet_words+8,sizeof *packet);
    if(!packet)return error(JP_PRINT_ALLOCATION),NULL;
    print_error=JP_PRINT_OK;return packet;
}
int jp_print_character_room(const uint32_t *p)
{
    uintptr_t start=(uintptr_t)packet,value=(uintptr_t)p;
    if(!packet || value<start || (value-start)%128 ||
       value-start>packet_words*4 || packet_words*4-(value-start)<128)
        return error(JP_PRINT_STATE);
    return 1;
}
void jp_print_close(const uint32_t *end,const uint32_t header[4])
{
    uintptr_t first=(uintptr_t)packet,last=(uintptr_t)end;
    if(!packet){error(JP_PRINT_STATE);return;}
    if(last<first || last-first>packet_words*4 || (last-first)%128)error(JP_PRINT_STATE);
    else if(jp_queue_append_direct_translucent(header,packet,(uint32_t)((last-first)/32)))error(JP_PRINT_QUEUE);
    free(packet);packet=NULL;packet_words=0;
}
static int16_t wrap(uint32_t v)
{
    v&=65535u;return (int16_t)(v<32768u?(int32_t)v:(int32_t)v-65536);
}
int16_t jp_print_coordinate(int16_t grid,uint16_t size)
{
    uint32_t magnitude=grid<0?(uint32_t)-(int32_t)grid:(uint32_t)grid;
    uint32_t word=jp_rz_scalar32_multiply(jp_random_positive_float_word(magnitude),
                                       jp_random_positive_float_word(size));
    unsigned exponent=(word>>23)&255u;
    uint32_t integer=0;
    if(exponent>=127){
        unsigned power=exponent-127;
        uint32_t sig=(word&0x7fffffu)|0x800000u;
        integer=power>=23?sig<<(power-23):sig>>(23-power);
    }
    return wrap(grid<0?0u-integer:integer);
}
int16_t jp_print_advance(int16_t x,uint16_t size)
{
    /* Both inputs and their sum fit exactly in binary32, then FTRC/EXTS.W. */
    return wrap((uint32_t)(int32_t)x+size);
}

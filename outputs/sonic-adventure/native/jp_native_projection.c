/* Authored native value contracts for the original two SH-4 assembly helpers.
 * The qualified shared RZ/DN1 and FSCA services perform the actual arithmetic;
 * host tanf and host floating arithmetic are not substituted. No FPSCR state
 * emulation or new physical Dreamcast hardware validation is claimed.
 */
#include <ninja.h>
#include "jp_native_sprite_numeric.h"
#include <stdint.h>
#include <string.h>
#undef njTan
Float njTan(Angle angle)
{
    Float sine,cosine;
    jp_sprite_fsca((int32_t)((uint32_t)angle&0xffffu),&sine,&cosine);
    return jp_sprite_divide(sine,cosine);
}
void njSetPerspective(Angle angle)
{
    /* SHAR followed by njTan's EXTU.W observes only bits 1..16. */
    Float tangent=njTan((Angle)(((uint32_t)angle>>1)&0xffffu));
    _nj_screen_.dist=jp_sprite_divide(_nj_screen_.w,jp_sprite_add(tangent,tangent));
}
/* Raw-word diagnostic avoids Python/host NaN conversions on the float ABI. */
__declspec(dllexport) uint32_t jp_projection_tangent_word(uint32_t angle)
{
    Float value=njTan((Angle)(angle&0xffffu));uint32_t word;
    memcpy(&word,&value,sizeof word);return word;
}

/* Host headers only: the Dreamcast SDK must not shadow Windows CRT headers. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include "jp_native_display.h"
_Static_assert(sizeof(unsigned long)==4,"original SDK Uint32 ABI");
int jp_display_query(struct jp_display_info *out,size_t bytes) {
    if (!out || bytes<sizeof(*out)) return JP_DISPLAY_ARGUMENT;
    DEVMODEW mode;memset(&mode,0,sizeof(mode));mode.dmSize=sizeof(mode);
    if (!EnumDisplaySettingsW(NULL,ENUM_CURRENT_SETTINGS,&mode) ||
        !mode.dmPelsWidth || !mode.dmPelsHeight || !mode.dmBitsPerPel)
        return JP_DISPLAY_UNAVAILABLE;
    struct jp_display_info info={sizeof(info),mode.dmPelsWidth,mode.dmPelsHeight,
        mode.dmBitsPerPel,mode.dmDisplayFrequency,mode.dmDisplayFrequency>1,
        0,mode.dmDisplayFlags};
    *out=info;return 0;
}
/* The original high-level SDK decoder remains unchanged. Its low-level
 * hardware-pin read becomes the actual host display query. A failed query
 * maps to the SDK's reserved value; callers still need real startup checks. */
unsigned long syCblCheckCableType(void) {
    struct jp_display_info info;
    return jp_display_query(&info,sizeof(info)) ? 0x100UL :
        (unsigned long)(info.legacy_cable<<8);
}

/* Authored private qualification entry point. External GPL-3.0 core is built
 * from its pinned checkout; no upstream emulator or retail bodies live here. */
#include "dcsound.h"
#include "arm.h"
#include "yam.h"
_Static_assert(sizeof(uint32)==4 && sizeof(sint16)==2 && sizeof(void *)==8,
               "native probe word/sample/pointer widths");
__declspec(dllexport) int jp_aica_probe_init(void)
{
    const uint32 word=0x12345678;
    if(*(const uint8 *)&word!=0x78)return -1;
    int rc=arm_init();if(rc)return rc;
    rc=yam_init();if(rc)return rc;
    return dcsound_init();
}

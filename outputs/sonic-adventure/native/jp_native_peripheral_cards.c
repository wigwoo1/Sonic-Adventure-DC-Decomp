/* Authored bridge to the already qualified actual VMU/LCD attachment owner. */
#include "jp_native_input.h"
#pragma push_macro("NULL")
#undef NULL
#include <shinobi.h>
#include <kaede.h>
#pragma pop_macro("NULL")
#include <string.h>
#include "jp_native_vmu_store.h"
_Static_assert(sizeof(PDS_PERIPHERALINFO)==sizeof(KDS_DEVICEINFO),"original device metadata ABI");
_Static_assert(offsetof(PDS_PERIPHERALINFO,type)==offsetof(KDS_DEVICEINFO,function),"function/type prefix");
static int metadata(void *unused,uint32_t port,void *out,size_t bytes)
{
    (void)unused;
    if(!out||bytes!=sizeof(PDS_PERIPHERALINFO)||port>=24)return -1;
    struct jp_vmu_store_info state;int rc=jp_vmu_store_snapshot(&state);
    if(rc==JP_VMU_NOT_INITIALIZED)return 0;if(rc)return -1;
    const KDS_DEVICEINFO *device=kdGetDeviceInfo((Sint32)port);
    if(!device || !device->function)return 0;
    memcpy(out,device,sizeof *device);return 1;
}
__declspec(dllexport) int jp_input_use_virtual_cards(void)
{return jp_input_configure_expansions(metadata,NULL);}

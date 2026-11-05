#include "jp_native_vblank_chain.h"
/* Preserve the qualified rational deadline/pump code and its single owner.
 * Only lifecycle and callback selection add the original sorted chain.
 */
#define jp_wait_initialize jp_wait_single_initialize
#define jp_wait_shutdown jp_wait_single_shutdown
#define jp_wait_set_vsync jp_wait_single_set_vsync
#define njSetVSyncFunction jp_wait_single_ninja_set_vsync
#include "jp_native_frame_wait.c"
#include "jp_native_frame_service_extension.c"
#undef jp_wait_initialize
#undef jp_wait_shutdown
#undef jp_wait_set_vsync
#undef njSetVSyncFunction

static jp_chain_callback chain_callback;
static void *chain_argument;
static void (*game_callback)(void);
static uint32_t game_handle;
static uint16_t logical_mask;
static int chain_status;
static int chain_fail(int code){chain_status=code;return code;}
static int chain_access(void)
{
    if(!initialized)return chain_fail(JP_CHAIN_NOT_INITIALIZED);
    if(owner!=GetCurrentThreadId())return JP_CHAIN_THREAD;
    if(terminal)return chain_fail(JP_CHAIN_WAIT_ERROR);
    return JP_CHAIN_OK;
}
static void dispatch_chain(void)
{
    uint16_t saved=logical_mask;
    if(chain_callback)chain_callback(chain_argument);
    logical_mask=saved;
}
static void game_thunk(void *unused){(void)unused;if(game_callback)game_callback();}
uint16_t jp_chain_native_mask_get(void){return logical_mask;}
void jp_chain_native_mask_set(int mask){logical_mask=(uint16_t)(mask&15);}
void jp_chain_native_callback(jp_chain_callback callback,void *argument)
{
    chain_callback=callback;chain_argument=argument;
    vsync=callback?dispatch_chain:NULL;
}
static void reset_chain(void)
{
    jp_chain_original_init();chain_callback=NULL;chain_argument=NULL;
    game_callback=NULL;game_handle=0;logical_mask=0;chain_status=JP_CHAIN_OK;
    vsync=NULL;jp_ninja_blank_bind(0);
}
int jp_wait_initialize(uint32_t num,uint32_t den,uint32_t maximum,jp_wait_idle idle,void *context)
{
    int code=jp_wait_single_initialize(num,den,maximum,idle,context);
    if(!code)reset_chain();return code;
}
int jp_wait_shutdown(void)
{
    int code=jp_wait_single_shutdown();if(!code)reset_chain();return code;
}
unsigned long syChainAddHandler(short event,jp_chain_callback callback,unsigned long priority,void *argument)
{
    if(chain_access())return 0;
    if(busy){chain_fail(JP_CHAIN_REENTRANT);return 0;}
    if(event!=0x900){chain_fail(JP_CHAIN_UNSUPPORTED);return 0;}
    if(!callback){chain_fail(JP_CHAIN_ARGUMENT);return 0;}
    uint32_t handle=jp_chain_original_add(event,callback,priority,argument);
    chain_status=handle?JP_CHAIN_OK:JP_CHAIN_FULL;return handle;
}
void syChainDeleteHandler(unsigned long handle)
{
    if(chain_access())return;
    if(!jp_chain_original_live(handle)){chain_fail(JP_CHAIN_ARGUMENT);return;}
    jp_chain_original_delete(handle);
    if(handle==game_handle){game_handle=0;game_callback=NULL;}
    chain_status=JP_CHAIN_OK;
}
void syChainInit(void)
{
    if(chain_access())return;
    if(busy){chain_fail(JP_CHAIN_REENTRANT);return;}
    reset_chain();
}
int jp_wait_set_vsync(void (*callback)(void))
{
    int code=access_owner();if(code)return code;if(busy)return JP_WAIT_REENTRANT;
    uint32_t count=jp_chain_original_count();
    if(callback&&!game_handle&&count==255){chain_fail(JP_CHAIN_FULL);return JP_WAIT_CHAIN_FULL;}
    /* Reuse the original phase anchor/error behavior. All subsequent list
     * mutations are allocation-free and prevalidated on this same thread.
     */
    code=jp_wait_single_set_vsync((callback||count>(game_handle!=0))?dispatch_chain:NULL);
    if(code)return code;
    if(game_handle)jp_chain_original_delete(game_handle);
    game_callback=callback;game_handle=0;
    if(callback)game_handle=jp_chain_original_add(0x900,game_thunk,88,NULL);
    chain_status=JP_CHAIN_OK;return JP_WAIT_OK;
}
void njSetVSyncFunction(void (*callback)(void))
{
    int code=jp_wait_set_vsync(callback);
    if(code&&initialized&&owner==GetCurrentThreadId())fault(code);
}
int jp_vblank_chain_status(void){return initialized&&owner!=GetCurrentThreadId()?JP_CHAIN_THREAD:chain_status;}
void jp_vblank_chain_clear_error(void){if(!chain_access())chain_status=JP_CHAIN_OK;}
uint32_t jp_vblank_chain_count(void){return chain_access()?0:jp_chain_original_count();}
uint32_t jp_vblank_chain_mask(void){return logical_mask;}
int jp_vblank_chain_describe(uint32_t handle,struct jp_chain_record *out)
{
    int code=chain_access();if(code)return code;
    if(!out||!jp_chain_original_live(handle))return chain_fail(JP_CHAIN_ARGUMENT);
    return jp_chain_original_describe(handle,out)?JP_CHAIN_OK:chain_fail(JP_CHAIN_ARGUMENT);
}
int jp_vblank_arm_ninja_blank(void)
{
    int code=chain_access();if(code)return code;
    if(jp_chain_original_live(jp_ninja_blank_handle()))return chain_fail(JP_CHAIN_ARGUMENT);
    uint32_t handle=syChainAddHandler(0x900,njBlank,88,NULL);
    if(!handle)return chain_status;
    jp_ninja_blank_bind(handle);return JP_CHAIN_OK;
}
int jp_vblank_wait_ninja_blank(void)
{
    int value=_nj_blank_count_;return jp_wait_while_equal(&_nj_blank_count_,value);
}

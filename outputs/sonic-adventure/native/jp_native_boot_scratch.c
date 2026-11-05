#include "jp_native_boot_scratch.h"
#include <limits.h>
/* Retain the qualified owner and its one set of SDK globals in this TU.
 * Internal owned-storage operations keep their previous exact bounds.
 * The public entry points below additionally accept a registered caller.
 */
#define jp_render_state_initialize jp_render_state_owned_initialize
#define jp_render_state_shutdown jp_render_state_owned_shutdown
#define jp_render_state_scratch jp_render_state_owned_scratch
#define jp_render_state_capacity jp_render_state_owned_capacity
#define jp_render_state_accept_buffer jp_render_state_owned_accept_buffer
#define jp_render_state_check_guards jp_render_state_owned_check_guards
#define jp_render_state_reset_scratch jp_render_state_owned_reset_scratch
#include "jp_native_render_state.c"
#undef jp_render_state_initialize
#undef jp_render_state_shutdown
#undef jp_render_state_scratch
#undef jp_render_state_capacity
#undef jp_render_state_accept_buffer
#undef jp_render_state_check_guards
#undef jp_render_state_reset_scratch

static unsigned char *registered_buffer;
static size_t registered_bytes;
static uint32_t registered_vertices,active_vertices;
static int borrowed;
static void forget_registration(void)
{
    registered_buffer=NULL;registered_bytes=0;
    registered_vertices=active_vertices=0;borrowed=0;
}
int jp_render_state_register_buffer(void *buffer,size_t bytes,uint32_t vertices)
{
    if(jp_render_state_owned_check_guards()!=JP_RENDER_OK)return status;
    uintptr_t start=(uintptr_t)buffer,own=(uintptr_t)allocation;
    if(!buffer || (start&31) || !vertices || vertices>INT_MAX ||
       bytes/64<vertices || bytes>UINTPTR_MAX-start)return fail(JP_RENDER_ARGUMENT);
    if(start<own+allocated && own<start+bytes)return fail(JP_RENDER_ARGUMENT);
    if(borrowed && (buffer!=registered_buffer || bytes!=registered_bytes ||
                   vertices!=registered_vertices))return fail(JP_RENDER_STATE);
    registered_buffer=buffer;registered_bytes=bytes;registered_vertices=vertices;
    return status;
}
int jp_render_state_accept_buffer(NJS_VERTEX_BUF *buffer,Int count)
{
    if(!ready())return 0;
    if(buffer==(NJS_VERTEX_BUF *)scratch && count==(Int)capacity){borrowed=0;active_vertices=0;return 1;}
    if(buffer!=(NJS_VERTEX_BUF *)registered_buffer || !registered_buffer ||
       count<1 || (uint32_t)count>registered_vertices){fail(JP_RENDER_STATE);return 0;}
    borrowed=1;active_vertices=(uint32_t)count;return 1;
}
int jp_render_state_initialize(uint32_t vertices,size_t budget)
{
    int result=jp_render_state_owned_initialize(vertices,budget);
    if(result==JP_RENDER_OK)forget_registration();
    return result;
}
void jp_render_state_shutdown(void)
{
    forget_registration();jp_render_state_owned_shutdown();
}
void *jp_render_state_scratch(void){return borrowed?registered_buffer:scratch;}
/* The pinned model producer also has fixed DRAW_MOST_POINTS=2048 lighting
 * arrays. Its preflight capacity must retain that bound even though the
 * genuine SDK's _nj_vertex_num_ accepts boot's 4096 64-byte records.
 */
uint32_t jp_render_state_capacity(void){return borrowed?(active_vertices<2048?active_vertices:2048):capacity;}
int jp_render_state_buffer_is_borrowed(void){return borrowed;}
size_t jp_render_state_buffer_bytes(void){return borrowed?registered_bytes:(size_t)capacity*16;}
int jp_render_state_check_guards(void){return jp_render_state_owned_check_guards();}
int jp_render_state_reset_scratch(void)
{
    if(!borrowed)return jp_render_state_owned_reset_scratch();
    if(jp_render_state_check_guards()!=JP_RENDER_OK)return status;
    memset(registered_buffer,0,(size_t)active_vertices*16);return status;
}

#include "jp_native_input.h"
#pragma push_macro("NULL")
#undef NULL
#include <ninja.h>
#include <shinobi.h>
#pragma pop_macro("NULL")
#include <windows.h>
#include <xinput.h>
#include <string.h>
#include <limits.h>
_Static_assert(sizeof(Uint32)==4 && sizeof(Sint16)==2,"original peripheral scalar widths");
typedef DWORD (WINAPI *get_state_fn)(DWORD,XINPUT_STATE *);
static struct {
    PDS_PERIPHERAL pads[4],expansions[24],empty;
    uint32_t previous_buttons[4];
    PDS_PERIPHERALINFO descriptions[4];
    PDS_PERIPHERALINFO expansion_descriptions[24];
    struct jp_input_info info;
    jp_input_source source;void *context;HWND window;DWORD owner;
    HMODULE module;get_state_fn get_state;uint32_t flags,logic;int initialized,busy;
    jp_input_metadata_source metadata;void *metadata_context;
} input;
static struct {
    void *window,*context,*metadata_context;uint32_t flags;DWORD owner;
    jp_input_source source;jp_input_metadata_source metadata;int configured,status;
} bootstrap;
static const char no_device[]="(no device)";
static int ready(void)
{if(!input.initialized)return JP_INPUT_NOT_INITIALIZED;if(input.owner!=GetCurrentThreadId())return JP_INPUT_THREAD;return (int)input.info.status;}
static int fail(int code){input.info.status=(uint32_t)code;return code;}
static int axis(int value)
{if(value>32767)value=32767;if(value< -32768)value=-32768;return value>=0?value/256:-((-value+255)/256);}
static int held(int key){return (GetAsyncKeyState(key)&0x8000)!=0;}
static uint32_t buttons(WORD value)
{
    uint32_t out=0;
    const WORD from[]={XINPUT_GAMEPAD_A,XINPUT_GAMEPAD_B,XINPUT_GAMEPAD_X,XINPUT_GAMEPAD_Y,XINPUT_GAMEPAD_START,XINPUT_GAMEPAD_DPAD_UP,XINPUT_GAMEPAD_DPAD_DOWN,XINPUT_GAMEPAD_DPAD_LEFT,XINPUT_GAMEPAD_DPAD_RIGHT,XINPUT_GAMEPAD_LEFT_SHOULDER,XINPUT_GAMEPAD_RIGHT_SHOULDER};
    const uint32_t to[]={PDD_DGT_TA,PDD_DGT_TB,PDD_DGT_TX,PDD_DGT_TY,PDD_DGT_ST,PDD_DGT_KU,PDD_DGT_KD,PDD_DGT_KL,PDD_DGT_KR,PDD_DGT_TL,PDD_DGT_TR};
    for(unsigned i=0;i<sizeof(from)/sizeof(from[0]);i++)if(value&from[i])out|=to[i];return out;
}
int jp_input_map_xinput(const struct jp_input_xinput *pad,struct jp_input_sample *out)
{
    if(!pad||!out)return JP_INPUT_ARGUMENT;
    struct jp_input_sample s={0};s.connected=1;s.buttons=buttons(pad->buttons);s.left=pad->left;s.right=pad->right;
    if(s.left>XINPUT_GAMEPAD_TRIGGER_THRESHOLD)s.buttons|=PDD_DGT_TL;
    if(s.right>XINPUT_GAMEPAD_TRIGGER_THRESHOLD)s.buttons|=PDD_DGT_TR;
    s.x1=axis(pad->lx);s.y1=axis(-(int)pad->ly);s.x2=axis(pad->rx);s.y2=axis(-(int)pad->ry);*out=s;return JP_INPUT_OK;
}
int jp_input_apply_keys(const uint8_t keys[256],struct jp_input_sample *out)
{
    if(!keys||!out)return JP_INPUT_ARGUMENT;for(unsigned i=0;i<256;i++)if(keys[i]>1)return JP_INPUT_ARGUMENT;
    struct jp_input_sample s=out->connected?*out:(struct jp_input_sample){0};s.connected=1;
    int x=keys['D']-keys['A'],y=keys['S']-keys['W'];
    if(keys['D']||keys['A'])s.x1=x>0?127:x<0?-128:0;if(keys['S']||keys['W'])s.y1=y>0?127:y<0?-128:0;
    const int from[]={VK_SPACE,'Z','X','C',VK_RETURN,VK_UP,VK_DOWN,VK_LEFT,VK_RIGHT,'Q','E'};
    const uint32_t to[]={PDD_DGT_TA,PDD_DGT_TB,PDD_DGT_TX,PDD_DGT_TY,PDD_DGT_ST,PDD_DGT_KU,PDD_DGT_KD,PDD_DGT_KL,PDD_DGT_KR,PDD_DGT_TL,PDD_DGT_TR};
    for(unsigned i=0;i<sizeof(from)/sizeof(from[0]);i++)if(keys[from[i]])s.buttons|=to[i];
    if(keys['Q'])s.left=255;if(keys['E'])s.right=255;
    if(keys[VK_ESCAPE])s.buttons|=PDD_DGT_TA|PDD_DGT_TB|PDD_DGT_TX|PDD_DGT_TY|PDD_DGT_ST;*out=s;return JP_INPUT_OK;
}
static int windows_source(void *context,struct jp_input_sample samples[4])
{
    (void)context;int focus=!input.window || GetForegroundWindow()==input.window;
    input.info.focused=(uint32_t)focus;
    if(input.flags&JP_INPUT_CONTROLLERS){
        for(unsigned slot=0;slot<4;slot++){
            XINPUT_STATE state;memset(&state,0,sizeof state);
            DWORD rc=input.get_state(slot,&state);
            if(rc==ERROR_DEVICE_NOT_CONNECTED)continue;
            if(rc!=ERROR_SUCCESS){input.info.os_error=rc;return 0;}
            struct jp_input_sample *s=&samples[slot];s->connected=1;if(!focus)continue;
            _Static_assert(sizeof(struct jp_input_xinput)==sizeof(XINPUT_GAMEPAD),"XInput packet layout");
            _Static_assert(offsetof(struct jp_input_xinput,lx)==offsetof(XINPUT_GAMEPAD,sThumbLX),"XInput axis offset");
            struct jp_input_xinput packet;memcpy(&packet,&state.Gamepad,sizeof packet);(void)jp_input_map_xinput(&packet,s);
        }
    }
    if(input.flags&JP_INPUT_KEYBOARD){
        struct jp_input_sample *s=&samples[0];s->connected=1;if(!focus)return 1;
        uint8_t keys[256]={0};const int queried[]={VK_SPACE,'Z','X','C',VK_RETURN,VK_UP,VK_DOWN,VK_LEFT,VK_RIGHT,'Q','E','W','A','S','D',VK_ESCAPE};
        for(unsigned i=0;i<sizeof(queried)/sizeof(queried[0]);i++)keys[queried[i]]=(uint8_t)held(queried[i]);
        (void)jp_input_apply_keys(keys,s);
    }
    return 1;
}
static int validate(void *window,uint32_t flags,jp_input_source source,void *context)
{
    if(flags&~3u || (!source && !flags) || (!source && context))return JP_INPUT_ARGUMENT;
    HWND hwnd=(HWND)window;
    if(!source && ((flags&JP_INPUT_KEYBOARD) || hwnd)){
        DWORD process=0;if(!hwnd||!IsWindow(hwnd))return JP_INPUT_ARGUMENT;
        GetWindowThreadProcessId(hwnd,&process);if(process!=GetCurrentProcessId())return JP_INPUT_ARGUMENT;
    }
    return 0;
}
static int initialize(void *window,uint32_t flags,jp_input_source source,void *context,uint32_t logic,int sdk)
{
    if(input.initialized)return input.owner==GetCurrentThreadId()?JP_INPUT_STATE:JP_INPUT_THREAD;
    if(bootstrap.owner && bootstrap.owner!=GetCurrentThreadId())return JP_INPUT_THREAD;
    int code=validate(window,flags,source,context);if(code)return code;
    HWND hwnd=(HWND)window;
    HMODULE module=NULL;get_state_fn get_state=NULL;
    if(!source && (flags&JP_INPUT_CONTROLLERS)){
        const wchar_t *names[]={L"xinput1_4.dll",L"xinput9_1_0.dll"};
        for(unsigned i=0;i<2 && !get_state;i++){
            module=LoadLibraryExW(names[i],NULL,LOAD_LIBRARY_SEARCH_SYSTEM32);
            if(module){FARPROC address=GetProcAddress(module,"XInputGetState");_Static_assert(sizeof(address)==sizeof(get_state),"Win32 function address ABI");memcpy(&get_state,&address,sizeof get_state);if(!get_state){FreeLibrary(module);module=NULL;}}
        }
        if(!get_state)return JP_INPUT_UNAVAILABLE;
    }
    memset(&input,0,sizeof input);input.owner=GetCurrentThreadId();input.window=hwnd;input.flags=flags;input.module=module;input.get_state=get_state;
    input.logic=logic;input.metadata=bootstrap.metadata;input.metadata_context=bootstrap.metadata_context;
    input.source=source?source:windows_source;input.context=context;input.initialized=1;input.info.source_kind=source?1:2;input.info.xinput_available=get_state!=NULL;input.info.record_bytes=sizeof(PDS_PERIPHERAL);
    for(unsigned i=0;i<4;i++){
        input.descriptions[i].type=PDD_DEVTYPE_CONTROLLER;input.descriptions[i].is_root=1;
        const char *name="Native Windows input";memcpy(input.descriptions[i].product_name,name,strlen(name)+1);
        input.pads[i].off=(Uint32)-1;input.pads[i].l=input.pads[i].r=0x8000;input.pads[i].x2=input.pads[i].y2=-32768;
    }
    /* Original software reset semantics for all 24 SDK ports. Console Kaede
     * bus startup is replaced by actual host acquisition above, not called as
     * an emulated hardware success. Legacy native initialization retains its
     * established pre-poll sentinels for compatibility. */
    if(sdk)for(unsigned port=0;port<24;port++){
        PDS_PERIPHERAL *p=port%6?&input.expansions[port]:&input.pads[port/6];
        memset(p,0,sizeof *p);p->name=(char *)no_device;
        if(logic){p->on=p->press=p->release=(Uint32)-1;}else p->off=(Uint32)-1;
    }
    return JP_INPUT_OK;
}
int jp_input_initialize(void *window,uint32_t flags,jp_input_source source,void *context)
{return initialize(window,flags,source,context,0,0);}
static int configure_access(void)
{
    if(input.initialized)return input.owner==GetCurrentThreadId()?JP_INPUT_STATE:JP_INPUT_THREAD;
    if(bootstrap.owner && bootstrap.owner!=GetCurrentThreadId())return JP_INPUT_THREAD;
    return 0;
}
int jp_input_configure(void *window,uint32_t flags,jp_input_source source,void *context)
{
    int rc=configure_access();if(rc)return rc;
    rc=validate(window,flags,source,context);bootstrap.status=rc;if(rc)return rc;
    bootstrap.window=window;bootstrap.flags=flags;bootstrap.source=source;bootstrap.context=context;
    bootstrap.configured=1;bootstrap.owner=GetCurrentThreadId();return 0;
}
int jp_input_configure_expansions(jp_input_metadata_source source,void *context)
{
    int rc=configure_access();if(rc)return rc;
    if(!source&&context)return JP_INPUT_ARGUMENT;
    bootstrap.metadata=source;bootstrap.metadata_context=context;bootstrap.owner=GetCurrentThreadId();return 0;
}
int jp_input_bootstrap_status(void)
{return bootstrap.owner && bootstrap.owner!=GetCurrentThreadId()?JP_INPUT_THREAD:bootstrap.status;}
void pdInitPeripheral(Sint32 logic,void *recvbuf,void *sendbuf)
{
    (void)recvbuf;(void)sendbuf;
    int rc=configure_access();if(rc){if(rc!=JP_INPUT_THREAD)bootstrap.status=rc;return;}
    if(!bootstrap.configured){bootstrap.status=JP_INPUT_STATE;return;}
    if(logic!=0 && logic!=1){bootstrap.status=JP_INPUT_ARGUMENT;return;}
    bootstrap.status=initialize(bootstrap.window,bootstrap.flags,bootstrap.source,bootstrap.context,(uint32_t)logic,1);
}
int jp_input_shutdown(void)
{
    if(!input.initialized){
        if(bootstrap.owner && bootstrap.owner!=GetCurrentThreadId())return JP_INPUT_THREAD;
        memset(&bootstrap,0,sizeof bootstrap);return JP_INPUT_OK;
    }
    if(input.owner!=GetCurrentThreadId())return JP_INPUT_THREAD;if(input.busy)return JP_INPUT_REENTRANT;
    if(input.module)FreeLibrary(input.module);memset(&input,0,sizeof input);
    memset(&bootstrap,0,sizeof bootstrap);return JP_INPUT_OK;
}
int jp_input_status(void){return ready();}
void jp_input_clear_error(void){if(input.initialized && input.owner==GetCurrentThreadId() && !input.busy)input.info.status=JP_INPUT_OK;}
int jp_input_poll(void)
{
    int rc=ready();if(rc)return rc;if(input.busy)return JP_INPUT_REENTRANT;
    if(input.info.polls==UINT64_MAX)return fail(JP_INPUT_STATE);
    struct jp_input_sample samples[4]={{0}};input.busy=1;int ok=input.source(input.context,samples);input.busy=0;
    if(!ok)return fail(JP_INPUT_SOURCE);
    const uint32_t allowed=PDD_DGT_TL|PDD_DGT_TR|PDD_DGT_TX|PDD_DGT_TY|PDD_DGT_TZ|PDD_DGT_KR|PDD_DGT_KL|PDD_DGT_KD|PDD_DGT_KU|PDD_DGT_ST|PDD_DGT_TA|PDD_DGT_TB|PDD_DGT_TC;
    for(unsigned i=0;i<4;i++){
        struct jp_input_sample *s=&samples[i];
        if(s->connected>1 || (s->connected && ((s->buttons&~allowed)||s->x1< -128||s->x1>127||s->y1< -128||s->y1>127||s->x2< -128||s->x2>127||s->y2< -128||s->y2>127||s->left>255||s->right>255)))return fail(JP_INPUT_ARGUMENT);
    }
    PDS_PERIPHERALINFO metadata[24]={{0}};
    if(input.metadata){
        input.busy=1;
        for(unsigned port=0;port<24;port++)if(port%6){
            int result=input.metadata(input.metadata_context,port,&metadata[port],sizeof metadata[port]);
            if(result<0){input.busy=0;return fail(JP_INPUT_SOURCE);}
            if(!result)memset(&metadata[port],0,sizeof metadata[port]);
            else if(result!=1 || !metadata[port].type || (metadata[port].type&~(PDD_DEVTYPE_STORAGE|PDD_DEVTYPE_LCD|PDD_DEVTYPE_TIMER)) || port%6>2){input.busy=0;return fail(JP_INPUT_ARGUMENT);}
            metadata[port].product_name[31]=0;
        }
        input.busy=0;
    }
    uint32_t connected=0;
    for(unsigned i=0;i<4;i++){
        struct jp_input_sample *s=&samples[i];PDS_PERIPHERAL *out=&input.pads[i];uint32_t old=input.previous_buttons[i],on=s->connected?s->buttons:0;input.previous_buttons[i]=on;
        out->old=input.logic?~old:old;out->on=input.logic?~on:on;out->off=~out->on;
        out->press=input.logic?~(on&~old):on&~old;out->release=input.logic?~(old&~on):old&~on;
        out->support=s->connected?(PDD_DEV_CONTROLLER|PDD_DEV_SUPPORT_AX2|PDD_DEV_SUPPORT_AY2):0;out->id=s->connected?PDD_DEV_CONTROLLER:0;
        out->info=s->connected?&input.descriptions[i]:NULL;out->name=s->connected?input.descriptions[i].product_name:NULL;
        out->x1=s->connected?(Sint16)s->x1:0;out->y1=s->connected?(Sint16)s->y1:0;
        out->x2=s->connected?(Sint16)s->x2:-32768;out->y2=s->connected?(Sint16)s->y2:-32768;
        out->l=s->connected?(Uint16)s->left:0x8000;out->r=s->connected?(Uint16)s->right:0x8000;
        if(s->connected)connected|=1u<<i;
    }
    memcpy(input.expansion_descriptions,metadata,sizeof metadata);
    for(unsigned port=0;port<24;port++)if(port%6){
        PDS_PERIPHERAL *out=&input.expansions[port];PDS_PERIPHERALINFO *desc=&input.expansion_descriptions[port];
        out->old=out->on;
        out->info=desc->type?desc:NULL;out->name=desc->type?desc->product_name:(char *)no_device;
    }
    input.info.connected_mask=connected;input.info.polls++;return JP_INPUT_OK;
}
const void *jp_input_record(uint32_t slot)
{int rc=ready();if(rc)return NULL;if(slot>=4){fail(JP_INPUT_ARGUMENT);return NULL;}return &input.pads[slot];}
const PDS_PERIPHERAL *pdGetPeripheral(Uint32 port)
{
    if(ready())return &input.empty;
    if(port==PDD_PORT_A0)return &input.pads[0];if(port==PDD_PORT_B0)return &input.pads[1];
    if(port==PDD_PORT_C0)return &input.pads[2];if(port==PDD_PORT_D0)return &input.pads[3];
    if(port<24)return &input.expansions[port];fail(JP_INPUT_ARGUMENT);return &input.empty;
}
const NJS_PERIPHERAL *njGetPeripheral(Uint32 port){return pdGetPeripheral(port);}
const PDS_PERIPHERALINFO *pdGetPeripheralInfo(Uint32 port){return pdGetPeripheral(port)->info;}
Sint32 pdGetPeripheralError(Uint32 port)
{if(ready())return (Sint32)PDD_ERR_GENERIC;if(port>=24){fail(JP_INPUT_ARGUMENT);return (Sint32)PDD_ERR_GENERIC;}return PDD_ERR_OK;}
void pdExecPeripheralServer(void){(void)jp_input_poll();}
void pdExitPeripheral(void){(void)jp_input_shutdown();}
int jp_input_snapshot(struct jp_input_info *out)
{if(!input.initialized)return JP_INPUT_NOT_INITIALIZED;if(input.owner!=GetCurrentThreadId())return JP_INPUT_THREAD;if(!out)return JP_INPUT_ARGUMENT;*out=input.info;return JP_INPUT_OK;}
size_t jp_input_info_size(void){return sizeof(struct jp_input_info);}
size_t jp_input_record_size(void){return sizeof(PDS_PERIPHERAL);}

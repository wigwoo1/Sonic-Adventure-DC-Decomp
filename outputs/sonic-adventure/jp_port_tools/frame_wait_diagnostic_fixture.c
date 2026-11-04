/* TEST ONLY: trace providers appended after the private original frame/hold
 * bodies. Never link these controlled audio/loader/render inputs in production.
 */
#define JP_PROBE __declspec(dllexport)
static unsigned trace_count,trace[16384];
static Uint16 probe_stage;
static void event(unsigned value){if(trace_count<16384)trace[trace_count++]=value;}
Bool draw_off,demo_playing;
int frame_split;
Uint32 windy_fog_shown,sky_deck_fog_color,jungle_fog_shown,ruin_fog_color;
NJS_FOG_TABLE windy_fog_table,sky_deck_fog_table,jungle_fog_table,ruin_fog;
Int _nj_calc_vertex_,_nj_calc_polygon_,_nj_draw_polygon_;
Uint32 _nj_vsync_count_,_nj_wait_vsync_in_,_nj_wait_vsync_out_;
Uint16 stage_get_stage_act(void){event(4);return probe_stage;}
SDE_ERR sdSysServer(void){event(3);return (SDE_ERR)0;}
void buExecServer(void){event(1);}
void loader_poll_drive(void){event(2);}
void njSetFogColor(Uint32 color){(void)color;event(5);}
void njSetFogTable(NJS_FOG_TABLE table){(void)table;event(6);}
void frame_end(Uint32 wait,int late){(void)wait;event(7);event(1000+(unsigned)late);}
void kmResetPointer(void){event(8);}
void bitmap_flush_queue(void){event(9);}
KMSTATUS kmRender(void){event(10);return KMSTATUS_SUCCESS;}
KMSTATUS kmFlipFrameBuffer(void){event(11);return KMSTATUS_SUCCESS;}
void pdExecPeripheralServer(void){event(12);}
void overlay_pad(void){event(13);}
void sound_send_queue(void){event(14);}
void bitmap_clear_queue(void){event(15);}
JP_PROBE void jp_wait_probe_reset(unsigned stage,unsigned flags)
{
    trace_count=0;probe_stage=(Uint16)stage;draw_off=(flags&1)?TRUE:FALSE;
    frame_split=!!(flags&2);demo_playing=!!(flags&4);holding=!!(flags&8);
    windy_fog_shown=(flags&16)?UINT32_MAX:0xff123456;
    _nj_calc_vertex_=11;_nj_calc_polygon_=22;_nj_draw_polygon_=33;
    _nj_vsync_count_=0;_nj_wait_vsync_in_=_nj_wait_vsync_out_=0;
}
JP_PROBE unsigned jp_wait_probe_events(void){return trace_count;}
JP_PROBE unsigned jp_wait_probe_event(unsigned index){return index<trace_count?trace[index]:UINT32_MAX;}
JP_PROBE void jp_wait_probe_clear_events(void){trace_count=0;}
JP_PROBE int jp_wait_probe_counter(unsigned index)
{
    switch(index){case 0:return frame_countdown;case 1:return frame_interval;case 2:return frame_step_count;case 3:return frame_step;
    case 4:return _nj_calc_vertex_;case 5:return _nj_calc_polygon_;case 6:return _nj_draw_polygon_;case 7:return (int)_nj_vsync_count_;default:return -9999;}
}
JP_PROBE void jp_wait_probe_countdown(int value){frame_countdown=value;}

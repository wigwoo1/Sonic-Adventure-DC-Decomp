"""Shared real transport with independent game/SDK style state and bounds."""
import argparse,ctypes as C,hashlib,json
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime
from native_video_test_support import Video
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;F=C.c_float
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();out=a.output.resolve();assert not out.exists() and out.is_relative_to(r/'work/sonic-adventure');out.mkdir();b=json.loads((a.build/'build_verification.json').read_bytes());assert sha(b['frames_DLL'])==b['frames_DLL_sha256']
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;video=Video(lib);assert bind(lib,'jp_queue_initialize',I,S)(8<<20)==0 and t.init(4,16,8<<20)==0 and video.init(320,128,0xff000000,1)==0 and bind(lib,'jp_ninja_contexts_initialize',I)()==0
    assert bind(lib,'jp_frames_initialize',I,U,U,S)(8,16,8<<20)==0
    bind(lib,'njInitPrint',None,P,U,U)(None,0,0);bind(lib,'text_style_init',None,P,U,U)(None,0,0);I.in_dll(lib,'frame_step').value=0
    text=bind(lib,'text_print',None,I,P);sdk=bind(lib,'njPrintC',None,I,P);fixed=bind(lib,'text_print_float',None,I,F,I);formatted=bind(lib,'text_printf',None,I,P);readstate=bind(lib,'jp_game_text_snapshot',None,P)
    reset=bind(lib,'jp_queue_reset_frame',I);data=bind(lib,'jp_queue_data',P,U);used=bind(lib,'jp_queue_committed_bytes',S,U);ps=bind(lib,'jp_print_status',I);clearprint=bind(lib,'jp_print_clear_error',None);fs=bind(lib,'jp_print_format_status',I);checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def state():values=(U*6)();readstate(values);return bytes(values)
    def packet():return C.string_at(data(1),used(1)) if used(1) else b''
    sample=C.create_string_buffer(b'0AB');baseline=state();text(0,sample);game=packet();check(ps()==0 and len(game)==416,'actual game packet baseline')
    bind(lib,'njPrintSize',None,C.c_uint16)(8);bind(lib,'njPrintColor',None,U)(0xff40c0ff);check(reset()==0,'reset before SDK print');sdk(0,sample);sdk_packet=packet();check(ps()==0 and sdk_packet!=game and state()==baseline,'SDK style changes do not change game style cache')
    check(reset()==0,'reset interleaving');text(0,sample);sdk(0,sample);text(0,sample);check(ps()==0 and packet()==game+sdk_packet+game,'game/SDK/game output concatenates exactly through one owner')
    bind(lib,'text_set_size',None,C.c_uint16)(23);bind(lib,'text_set_color',None,U)(0xffb04080);check(reset()==0,'reset independent style');sdk(0,sample);check(packet()==sdk_packet,'game style changes do not affect SDK text')
    check(reset()==0,'reset formatted bounds');prior=state()
    for value,digits in [(123.4,1),(1.,0),(1.,14),(float('inf'),5),(float('nan'),5),(3.4e38,13)]:
        fixed(0,F(value),digits);check(fs()!=0 and packet()==b'' and state()==prior,'unsafe original fixed-padding domain rejects with no output')
    for fmt,args in [(b'%128s',[C.cast(C.create_string_buffer(b'A'),P)]),(b'%',[]),(b'%s',[P()]),(b'%q',[])]:
        formatted(0,C.create_string_buffer(fmt),*args);check(fs()!=0 and packet()==b'' and state()==prior,'invalid/overflowing native format rejects without draw')
    for frame in [1,-1,2,2147483647]:
        I.in_dll(lib,'frame_step').value=frame;clearprint();text(0,None);formatted(0,None);fixed(0,F(float('nan')),-1);check(packet()==b'' and ps()==0 and state()==prior,'suppressed frames skip all strings and float preflight')
    I.in_dll(lib,'frame_step').value=0;text(0,sample);check(ps()==0 and len(packet())==416,'successful call recovers after prior rejected formats')
    bind(lib,'jp_queue_shutdown',None)();bind(lib,'njExitPrint',None)();t.shutdown();check(bind(lib,'jp_frames_shutdown',I)()==0,'real owners drained');video.shutdown()
    result=dict(status='real_game_SDK_text_interleaving_independent_style_and_bounded_errors_verified',checks=checks,game_SDK_game_packet_bytes_compared=len(game+sdk_packet+game),native_functions_replaced=False,retail_inputs_used=False,build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',flush=True)
if __name__=='__main__':main()

"""Independent progressive countdown oracle and real native GPU/Presents."""
import argparse,ctypes as C,hashlib,json,random,threading,time
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime
from native_gpu_test_support import GPU,Config,Failure
from native_video_test_support import Video
from native_clock_test_support import Clock,Counter
from native_present_test_support import Info
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p;S=C.c_size_t
class Cadence(C.Structure):_fields_=[('tick',Q),('remaining',I),('wait',I)]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for kind in ('frames','present','GPU'):assert sha(b[kind+'_DLL'])==b[kind+'_DLL_sha256']
    d.mkdir();t=Runtime(Path(b['frames_DLL']));fl=t.dll;gpu=GPU(Path(b['GPU_DLL']));gl=gpu.lib;pl=C.CDLL(b['present_DLL']);clock=Clock(Path(b['frames_DLL']));counter=Counter();checks=[]
    def check(ok,label):assert ok,label;checks.append(label)
    check(bind(fl,'jp_queue_initialize',I,S)(8<<20)==0 and bind(fl,'jp_frames_initialize',I,U,U,S)(8,128,32<<20)==0 and t.init(32,1024,32<<20)==0 and Video(fl).init(320,128,0xff000000,1)==0 and bind(fl,'jp_render_state_initialize',I,U,S)(128,1<<20)==0 and bind(fl,'jp_ninja_global_initialize',I)()==0,'actual complete native graphics owners')
    window=P();failure=Failure();check(bind(gl,'jp_window_open',I,P,U,U,I,P,P)(gpu.handle,320,128,1,C.byref(window),C.byref(failure))==0,'real native window')
    wait_init=bind(pl,'jp_wait_initialize',I,U,U,U,P,P);wait_shutdown=bind(pl,'jp_wait_shutdown',I);sync=bind(pl,'jp_wait_sync',I);window_idle=bind(pl,'jp_wait_window_idle',I,P,U)
    Idle=C.CFUNCTYPE(I,P,U)
    @Idle
    def idle(context,ms):
        if ms:counter.tick+=(ms*781250+999)//1000;time.sleep(.0001)
        return window_idle(window,0)
    init=bind(pl,'jp_present_initialize',I,P,P,P,P,U,U);shutdown=bind(pl,'jp_present_shutdown',I);set_count=bind(pl,'kmSetWaitVsyncCount',U,I);get=bind(pl,'jp_present_cadence_snapshot',I,P)
    status=bind(pl,'jp_present_status',I);psnapshot=bind(pl,'jp_present_snapshot',I,P);render=bind(pl,'kmRender',U);flip=bind(pl,'kmFlipFrameBuffer',U);poll=bind(pl,'jp_present_poll',I)
    device=C.addressof((C.c_byte*1).in_dll(fl,'kmiDeviceExtension'));cfg=Config.make(320,128,1);cfg.logical_width=320;cfg.logical_height=128
    def setup():
        counter.tick=0;check(counter.start(clock,781250)==0 and wait_init(60000,1001,8,C.cast(idle,P),None)==0,'controlled logical clock with real native message service')
        check(init(gpu.handle,window,device,C.byref(cfg),5000,0)==0,'real presenter with fresh cadence')
    def sample():v=Cadence();assert get(C.byref(v))==0;return v
    setup();rng=random.Random(0x43414445);tick=remaining=wait=0;transitions=[]
    for n in range(256):
        delta=rng.randrange(51);tick+=delta;counter.tick=(tick*781250*1001+59999)//60000
        for _ in range(delta):
            remaining-=1
            if remaining<0:remaining=wait
        check(sync()==0,'native rational service advances to independent target tick');s=sample()
        check((s.tick,s.remaining,s.wait)==(tick,remaining,wait),'native constant-time cadence matches independently iterated countdown at every tick')
        old=bytes(s)
        for invalid in (0,-1,-2147483648):check(set_count(invalid)!=0 and bytes(sample())==old and status()==0,'invalid count preserves current phase, count and healthy presenter')
        count=rng.choice((1,2,3,4,7,99,2147483647));check(set_count(count)==0,'positive original signed count accepted without overflow');wait=count-1;s=sample()
        check((s.tick,s.remaining,s.wait)==(tick,remaining,wait),'new count preserves unfinished old countdown and changes only its reload value')
        transitions.append((tick,remaining,wait))
    foreign=[];thread=threading.Thread(target=lambda:foreign.append(set_count(1)));thread.start();thread.join();check(foreign[0]!=0 and bytes(sample())==bytes(s) and status()==0,'foreign setter cannot change cadence or poison presenter')
    check(shutdown()==0 and wait_shutdown()==0,'large-count phase model releases its owner')
    setup();bind(fl,'njInitPrint',None,P,U,U)(None,0,0);bind(fl,'text_style_init',None,P,U,U)(None,0,0);I.in_dll(fl,'frame_step').value=0
    bind(fl,'njSetBackColor',None,U,U,U)(0xff000000,0xff000000,0xff000000);bind(fl,'text_set_color',None,U)(0xffffffff)
    text=bind(fl,'text_print',None,I,P);actual=[]
    for count in (1,2,3,4,1):
        check(set_count(count)==0,'actual presentation cadence change')
        check(bind(fl,'jp_queue_reset_frame',I)()==0,'fresh actual original text frame');text(0x10001,C.create_string_buffer(b'PRESENT CADENCE'))
        check(render()==0,'real GPU submission for cadence verification');deadline=time.monotonic()+10
        while True:
            rc=poll()
            if rc==0:break
            assert rc==12 and time.monotonic()<deadline,(rc,status());time.sleep(.001)
        s=sample();expected=s.tick+s.remaining+1
        check(flip()==0,'actual DXGI Present after logical countdown');ps=Info();check(psnapshot(C.byref(ps))==0,'actual frame/job presentation snapshot')
        check(ps.required_tick==ps.presented_tick==expected and ps.required_tick>s.tick,'actual Present uses exactly the first eligible future boundary under the independent pre-flip phase')
        after=sample();check(after.tick==expected and after.remaining==count-1 and after.wait==count-1,'native countdown reloads the current count at the real presented boundary')
        check(bind(fl,'jp_frames_live_count',U)()==bind(gl,'jp_gpu_jobs_live',U,P)(gpu.handle)==0,'actual cadence flip retires GPU/frame ownership')
        actual.append(dict(count=count,before_tick=s.tick,before_remaining=s.remaining,required_tick=ps.required_tick,presented_tick=ps.presented_tick))
    check(shutdown()==0 and wait_shutdown()==0,'actual cadence presenter shuts down')
    bind(gl,'jp_window_close',None,P)(window);gpu.close(gpu.handle);bind(fl,'text_exit',None)();bind(fl,'njExitPrint',None)();clock.shutdown();bind(fl,'jp_queue_shutdown',None)();t.shutdown();check(bind(fl,'jp_frames_shutdown',I)()==0,'all real graphics owners released')
    result=dict(status='native_progressive_wait_count_independent_countdown_and_actual_Presents_verified',checks=len(checks),random_count_transitions=256,controlled_tick_count=tick,
        actual_GPU_submissions=5,actual_Presents=5,presentation_boundaries=actual,signed_max_count_and_invalid_counts_verified=True,
        old_countdown_preserved_on_change=True,real_GPU_and_window_used=True,native_function_bodies_replaced=False,
        original_console_interlaced_IRQ_or_physical_scanout_timing_claimed=False,build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(checks),'checks',flush=True)
if __name__=='__main__':main()

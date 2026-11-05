"""Original Ninja flag instructions plus real GPU EOV/EOR/fatal lifecycle."""
import argparse,ctypes as C,hashlib,json,re,struct,sys,threading,time
from pathlib import Path
from native_packet_test_support import bind
from native_gpu_test_support import GPU,Config,Failure
from native_texture_test_support import Runtime
from native_video_test_support import Video
from native_clock_test_support import Clock,Counter
from native_present_test_support import Info
from model_reference import make_reference
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p;S=C.c_size_t
Callback=C.CFUNCTYPE(None,P);Idle=C.CFUNCTYPE(I,P,U)
class Events(C.Structure):_fields_=[('eov',Q),('fatal',Q),('last',U),('reserved',U)]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();out=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not out.exists() and out.is_relative_to(r/'work/sonic-adventure') and not out.is_relative_to(r/'work/sonic-adventure/build')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(a.build/n)==h,n
    for kind in ('frames','present','GPU'):assert sha(b[kind+'_DLL'])==b[kind+'_DLL_sha256']
    assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    out.mkdir();t=Runtime(Path(b['frames_DLL']));lib=t.dll;v=Video(lib)
    gpu=GPU(Path(b['GPU_DLL']));gl=gpu.lib;pl=C.CDLL(b['present_DLL']);clock=Clock(Path(b['frames_DLL']))
    init=bind(pl,'jp_present_initialize',I,P,P,P,P,U,U);shutdown=bind(pl,'jp_present_shutdown',I)
    status=bind(pl,'jp_present_status',I);poll=bind(pl,'jp_present_poll',I);render=bind(pl,'kmRender',U);flip=bind(pl,'kmFlipFrameBuffer',U)
    snapshot=bind(pl,'jp_present_snapshot',I,P);events=bind(pl,'jp_present_callback_snapshot',I,P)
    eov=bind(pl,'kmSetEndOfVertexCallback',U,P,P);fatal=bind(pl,'kmfSetFatalErrorCallback',U,P);eor=bind(pl,'kmSetEORCallback',U,P,P)
    wait_init=bind(pl,'jp_wait_initialize',I,U,U,U,P,P);wait_shutdown=bind(pl,'jp_wait_shutdown',I);idle=bind(pl,'jp_wait_window_idle',I,P,U)
    original_eov=bind(lib,'njSetEOVflag',None,P);original_error=bind(lib,'njSetERRflag',None,P)
    vf=I.in_dll(lib,'_nj_eov_flag_');ef=I.in_dll(lib,'_nj_fatal_err_callback_')
    rf=I.in_dll(pl,'_nj_render_flag_');erf=I.in_dll(pl,'_nj_eor_flag_')
    checks=[];machine_calls=frames=presents=0;main_thread=threading.get_ident();keep=[]
    def check(ok,label):assert ok,label;checks.append(label)
    def state():x=Info();assert snapshot(C.byref(x))==0;return x
    def counters():x=Events();assert events(C.byref(x))==0;return x
    check(eov(C.cast(original_eov,P),None)!=0 and fatal(C.cast(original_error,P))!=0,'registration before real presenter setup fails')
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,_=make_reference(executor);table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary;seen=set()
    def put(at,value):struct.pack_into('<I',ram,at&0xffffff,value)
    def word(at):return struct.unpack_from('<I',ram,at&0xffffff)[0]
    def trace(pc,*unused):
        for n in ('njSetEOVflag','njSetERRflag'):
            if pc==table[n]:seen.add(n)
    for name in ('njSetEOVflag','njSetERRflag'):
        for code in (0,1,2,3,4,0x10006,0x7fffffff,0x80000000,0xffffffff):
            vf.value=0x12345678;ef.value=-123;put(table['_nj_eov_flag_'],0x12345678);put(table['_nj_fatal_err_callback_'],(-123)&0xffffffff)
            cpu=executor.State();cpu.pc=table[name];cpu.pr=0x8cfed000;cpu.r[4]=code;cpu.r[15]=0x8cf00000;cpu.sr=0x70000000
            fpu=Fpu();fpu.fpscr=0x40001;reference(ram,cpu,fpu=fpu,trace=trace,instruction_budget=100000);machine_calls+=1
            (original_eov if name=='njSetEOVflag' else original_error)(P(code))
            check((vf.value&0xffffffff,ef.value&0xffffffff)==(word(table['_nj_eov_flag_']),word(table['_nj_fatal_err_callback_'])) and cpu.r[15]==0x8cf00000,'both original flag words match complete unhooked SH-4 callback, including signed error bits')
    check(seen=={'njSetEOVflag','njSetERRflag'},'both real original callback entries executed without function hooks')
    qi=bind(lib,'jp_queue_initialize',I,S);fi=bind(lib,'jp_frames_initialize',I,U,U,S);live=bind(lib,'jp_frames_live_count',U)
    check(qi(8<<20)==0 and fi(8,128,32<<20)==0 and t.init(32,1024,32<<20)==0 and v.init(320,128,0xff000000,1)==0,'actual queue/frame/texture/video owners')
    check(bind(lib,'jp_render_state_initialize',I,U,S)(128,1<<20)==0 and bind(lib,'jp_ninja_global_initialize',I)()==0,'complete real Ninja defaults before game text')
    bind(lib,'njInitPrint',None,P,U,U)(None,0,0);bind(lib,'text_style_init',None,P,U,U)(None,0,0);I.in_dll(lib,'frame_step').value=0
    text=bind(lib,'text_print',None,I,P);reset=bind(lib,'jp_queue_reset_frame',I);sort=bind(lib,'kmSetAutoSortMode',I,I)
    wo=bind(gl,'jp_window_open',I,P,U,U,I,P,P);wc=bind(gl,'jp_window_close',None,P);window=P();failure=Failure()
    check(wo(gpu.handle,320,128,1,C.byref(window),C.byref(failure))==0,'real native window for actual original game text')
    jobs=bind(gl,'jp_gpu_jobs_live',U,P);device=C.addressof((C.c_byte*1).in_dll(lib,'kmiDeviceExtension'));log=[]
    def configure(timeout=5000,controlled=False):
        check(wait_shutdown()==0,'fresh logical-wait owner')
        if not controlled:check(clock.init(None,None,0)==0,'actual Windows performance clock')
        check(wait_init(60000,1001,8,C.cast(model_idle if controlled else idle,P),None if controlled else window)==0,'logical ticks service real native window')
        cfg=Config.make(320,128,0);cfg.logical_width=320;cfg.logical_height=128
        check(init(gpu.handle,window,device,C.byref(cfg),timeout,0)==0,'same actual SDK record, GPU, window and clock')
        vf.value=1;ef.value=0;rf.value=0;erf.value=1
    def record():
        check(reset()==0,'fresh real text recording');text(0x10001,C.create_string_buffer(b'NINJA CALLBACKS'))
        check(bind(lib,'jp_print_status',I)()==0,'original game text and Ninja font geometry recorded')
    def ready():
        deadline=time.monotonic()+10
        while True:
            rc=poll()
            if rc==0:return
            assert rc==12 and time.monotonic()<deadline,(rc,status());time.sleep(.001)
    def submit():
        nonlocal frames
        record();check(render()==0,'actual game text captured into a real GPU job');frames+=1
    def present():
        nonlocal presents
        check(flip()==0,'actual native backbuffer Present and logical-boundary retirement');presents+=1
    configure();check(eov(C.cast(original_eov,P),P(0x1234567887654321))==0 and fatal(C.cast(original_error,P))==0 and eor(C.cast(pl.njSetEORflag,P),None)==0,'actual original Ninja handlers registered in shared typed record')
    submit();check(vf.value==0 and erf.value==1 and rf.value==0 and counters().eov==1,'original EOV flag clears after native vertex submission, before EOR')
    s=state();check(s.frame and s.job and s.completions==0 and live()==jobs(gpu.handle)==1,'EOV retains real outstanding frame/GPU lifetime')
    ready();check(erf.value==0 and rf.value==1 and state().eor_calls==1,'original EOR flags change only on observed real GPU completion')
    check(poll()==0 and counters().eov==1 and state().eor_calls==1,'repeated completion polls do not duplicate callbacks');present()
    check(shutdown()==0 and live()==jobs(gpu.handle)==0,'normal callback pipeline drains all owners')
    @Callback
    def observed(argument):
        x=state();log.append((argument,threading.get_ident(),x.submissions,x.completions))
        # Store results, not assertions swallowed by the ctypes callback ABI.
        log.append(('reentry',render(),flip(),shutdown(),poll(),eov(None,None),fatal(None),eor(None,None),status()))
    keep.append(observed);configure();sentinel=0x1234567887654321
    check(eov(C.cast(observed,P),P(sentinel))==0 and eor(C.cast(observed,P),P(sentinel+1))==0,'full native callback argument width')
    submit();ready();present()
    check(log[0]==(sentinel,main_thread,1,0) and log[2]==(sentinel+1,main_thread,1,1),'EOV precedes EOR, exactly once each, on creating game thread')
    check(all(all(x!=0 for x in entry[1:-1]) and entry[-1]==0 for entry in log if entry[0]=='reentry'),'callback mutation/reentry rejected without poisoning successful work')
    check(shutdown()==0,'observer lifecycle cleans up')
    configure();check(fatal(C.cast(original_error,P))==0 and eov(C.cast(original_eov,P),None)==0 and sort(1)==0,'real unsupported autosort request with original fatal handler')
    record();check(render()!=0 and status()==6,'unsupported actual GPU state fails explicitly');s=state();ev=counters()
    check(ef.value==0x10006 and vf.value==1 and ev.fatal==1 and ev.eov==0 and ev.last==0x10006,'real GPU rejection calls original error handler once, with distinct native code and no false EOV')
    check(s.frame and not s.job and live()==1 and jobs(gpu.handle)==0 and not s.presentations,'failed submission preserves captured frame without fabricated GPU work/Present')
    check(render()!=0 and flip()!=0 and poll()==6 and counters().fatal==1,'repeated calls preserve first terminal error without duplicate fatal dispatch')
    check(shutdown()==0 and live()==0 and sort(0)==0,'failed submission releases its actual captured frame')
    log.clear();configure();check(fatal(C.cast(observed,P))==0 and sort(1)==0,'fatal observer installed before real failure');record()
    check(render()!=0 and log[0]==(0x10006,main_thread,0,0),'fatal callback receives numeric error rather than a host object address')
    check(all(x!=0 for x in log[1][1:-1]) and log[1][-1]==6 and counters().fatal==1,'fatal observer sees latched error and cannot recursively mutate or dispatch')
    check(shutdown()==0 and sort(0)==0,'fatal observer owner drained')
    configure();foreign=[]
    def other_thread():foreign.extend([eov(None,None),fatal(None),eor(None,None),shutdown()])
    thread=threading.Thread(target=other_thread);thread.start();thread.join()
    check(all(x!=0 for x in foreign) and status()==0 and counters().fatal==0,'foreign thread cannot change callbacks or trigger a spurious fatal error')
    check(eov(C.cast(observed,P),None)==0 and fatal(C.cast(observed,P))==0 and eov(None,None)==0 and fatal(None)==0 and sort(1)==0,'explicit callback removal')
    record();check(render()!=0 and status()==6 and counters().fatal==0 and counters().last==0x10006,'removed fatal handler leaves an explicit observable error instead of hanging')
    check(shutdown()==0 and sort(0)==0,'null-handler failure drains')
    counter=Counter()
    @Idle
    def model_idle(context,ms):
        if ms:counter.tick+=(ms*781250+999)//1000;time.sleep(.001)
        return idle(window,0)
    keep.append(model_idle);check(counter.start(clock,781250)==0,'controlled logical deadline with real GPU/window')
    configure(timeout=1,controlled=True);check(fatal(C.cast(original_error,P))==0 and eov(C.cast(original_eov,P),None)==0,'original timeout handlers registered')
    submit();check(flip()!=0 and status()==8 and ef.value==4 and counters().last==4 and counters().fatal==1,'real flip deadline dispatches original KMI_FATAL_FLIP_TIMEOUT code')
    check(shutdown()==0 and ef.value==4 and live()==jobs(gpu.handle)==0,'timeout drain retains original error and releases frame/job without callbacks')
    log.clear();configure();check(eov(C.cast(original_eov,P),None)==0 and eor(C.cast(observed,P),None)==0 and fatal(C.cast(observed,P))==0,'pending-drain observers registered')
    submit();check(vf.value==0 and not log,'only actual vertex callback before explicit shutdown')
    check(shutdown()==0 and not log and ef.value==0 and live()==jobs(gpu.handle)==0,'shutdown observes and retires GPU completion without dispatching EOR or fatal game callbacks')
    configure();submit();ready();present()
    check(vf.value==1 and erf.value==1 and rf.value==0 and counters().eov==counters().fatal==0 and not log,'fresh presenter cannot call stale callbacks from prior owner lifetime')
    check(shutdown()==0,'final owner shutdown')
    wc(window);gpu.close(gpu.handle)
    bind(lib,'text_exit',None)();bind(lib,'njExitPrint',None)();wait_shutdown();clock.shutdown()
    bind(lib,'jp_queue_shutdown',None)();t.shutdown();check(bind(lib,'jp_frames_shutdown',I)()==0,'all graphics/text/frame owners released')
    result=dict(status='original_Ninja_EOV_error_instructions_real_GPU_callbacks_and_fatal_lifecycle_verified',checks=len(checks),check_labels=checks,
        complete_original_machine_calls=machine_calls,original_callback_function_hooks_used=False,actual_GPU_text_submissions=frames,actual_Presents=presents,
        encoded_error_width_preserved=True,host_callback_argument_width_preserved=True,callbacks_on_game_thread=True,EOV_before_EOR=True,
        actual_GPU_rejection_and_logical_flip_timeout=True,callback_reentry_thread_and_lifetime_guards=True,
        build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
        full_njInitSystem_or_sbInitSystem_or_gameplay_verified=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(checks),'checks',flush=True)

if __name__=='__main__':main()

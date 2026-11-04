"""Real KM-facing render/flip, typed device/statistics and original frame path."""
import argparse,ctypes as C,hashlib,json,os,struct,threading,time
from pathlib import Path
from native_packet_test_support import bind
from native_gpu_test_support import GPU,Config,Failure
from native_texture_test_support import Runtime,Info,Name,List,Record,field
from native_video_test_support import Video
from native_present_test_support import Present,Stats,Info as State
from native_clock_test_support import Counter
from verify_native_frame_wait import Snapshot as WaitSnapshot
U=C.c_uint32;I=C.c_int;Q=C.c_uint64;P=C.c_void_p;S=C.c_size_t;Callback=C.CFUNCTYPE(None,P)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def fw(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();b=json.loads((a.build/'build_verification.json').read_text());assert not a.report.exists()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(Path(b['DLL']))==b['DLL_sha256'] and sha(Path(b['frames_DLL']))==b['frames_DLL_sha256']
    runtime=Runtime(Path(b['frames_DLL']));lib=runtime.dll;video=Video(lib);gpu=GPU(Path(b['GPU_DLL']));present=Present(Path(b['DLL']));clock=present.clock;gl=gpu.lib
    device=C.addressof((C.c_byte*1).in_dll(lib,'kmiDeviceExtension'));present.device(device)
    checks=images=pixels=flips=0;groups={};callbacks=[];keep=[];main_thread=threading.get_ident()
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);ctx=bind(lib,'jp_queue_set_context',I,U,P)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);live=bind(lib,'jp_frames_live_count',U)
    reserve=bind(lib,'jp_queue_reserve',I,U,U);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None);end=bind(lib,'njEndVertex',None,P);pin=bind(lib,'jp_frames_prepare_texture',I,U)
    starts=[bind(lib,n,P) for n in ['njStartVertexTexOpaque','njStartVertexTexTrans','njStartVertexTexPunch']]
    clear_errors=bind(lib,'jp_frames_clear_error',None);mode=bind(lib,'kmSetPaletteMode',I,I);palette=bind(lib,'kmSetPaletteData',I,P)
    sort=bind(lib,'kmSetAutoSortMode',I,I)
    check(qi(4<<20)==0 and fi(8,8,4<<20)==0 and runtime.init(8,128,1<<20)==0 and video.init(64,64,0xff123456,1)==0,'real shared video/frame/queue owners')
    check(mode(3)==0 and palette((U*1024)(*[0xff00ff00]*1024))==0,'real palette')
    texture=Info();s=texture.texsurface;s.Type=0x10000;s.BitDepth=1;s.PixelFormat=6<<27;s.nWidth=s.nHeight=8;s.TextureSize=64;s.fSurfaceFlags=0x24
    check(runtime.copy(C.byref(texture),C.create_string_buffer(bytes([7]*64)),64)==0,'real encoded PAL surface')
    names=(Name*1)();runtime.setname(C.byref(names[0]),None,1000,0);runtime.entry(field(names[0]),C.byref(texture),1000,2,0xffffffff);runtime.param(runtime.resolve(names[0].texaddr));tcw=Record.from_address(runtime.resolve(names[0].texaddr)).texparambuffer
    listing=List(names,1);check(runtime.select(C.byref(listing))==1,'actual texture selection')
    wo=bind(gl,'jp_window_open',I,P,U,U,I,P,P);wc=bind(gl,'jp_window_close',None,P);read=bind(gl,'jp_window_readback',I,P,P,S,P);copy=bind(gl,'jp_window_copy_job',I,P,Q,P)
    count=bind(gl,'jp_window_presented',Q,P);jobs=bind(gl,'jp_gpu_jobs_live',U,P);window=P();e=Failure()
    check(wo(gpu.handle,64,64,1,C.byref(window),C.byref(e))==0,'visible native window')
    def configure(off=False,timeout=5000):
        check(present.wait_shutdown()==0 and clock.init(None,None,0)==0,'real native clock/wait owner reset')
        check(present.wait_init(60000,1001,8,C.cast(present.window_idle,P),window)==0,'real logical ticks and OS message idle')
        config=Config.make(textures=0 if off else 1)
        check(present.init(gpu.handle,window,device,C.byref(config),timeout,0)==0,'native presenter shares actual SDK record')
        present.probe(0,0);present.pacing(2,2);present.clear();clock.write(0)
        check(present.valid()==0 and present.fits(0)==0,'no zero-cost prediction before first measured completion')
        return config
    def record(lane=0,n=1):
        pcw=0x8000000a|([0,2,4][lane]<<24)
        check(reset()==0 and reserve(lane,1+3*n)==0 and ctx(0,(U*4)(pcw,0xe0800000,0x20800000,tcw))==0 and pin(tcw&0x1fffff)==0,'real complete packet reservation')
        get();ptr=starts[lane]();check(bool(ptr),'actual SDK writer');vertices=[]
        for _ in range(n):
            for i,(x,y) in enumerate([(-64,-64),(192,-64),(-64,192)]):vertices += [0xf0000000 if i==2 else 0xe0000000,fw(x),fw(y),fw(1),fw(.25),fw(.25),0xffff0000,0]
        raw=struct.pack('<'+'I'*len(vertices),*vertices);C.memmove(ptr,raw,len(raw));end(ptr+len(raw));put()
    @Callback
    def done(argument):
        info=present.snapshot();stats=present.statistics();callbacks.append((argument,threading.get_ident(),present.busy(0),info.completions,stats.render_time,stats.ta_time,clock.tstr()))
    keep.append(done)
    def ready():
        deadline=time.monotonic()+10
        while True:
            rc=present.poll()
            if rc==0:return
            check(rc==12 and time.monotonic()<deadline,('real pending/completion',rc));time.sleep(.001)
    cfg=configure();check(present.callback(C.cast(done,P),P(0x1234))==0,'typed real EOR callback slot')
    before=clock.wall();record();check(present.render()==0,'actual kmRender captures and submits GPU work');after=clock.wall();state=present.snapshot();stats=present.statistics()
    check(state.frame and state.job and live()==jobs(gpu.handle)==1 and present.busy(0)==1 and present.busy(1)==0,'real owned pending frame/job and typed busy state')
    check(before<=((~stats.render_start)&0xffffffff)<=after and stats.ta_start==stats.render_start,'real downcounter start in original wall units')
    check(list(stats.list_max)==[128,0,0,0,0],'actual opaque packet length in typed statistics')
    ready();state=present.snapshot();stats=present.statistics()
    check(present.busy(0)==0 and state.ready and state.history and stats.render_time>=stats.ta_time and present.valid()==1,'completed native timing is measured and qualified')
    check(len(callbacks)==1 and callbacks[0][0]==0x1234 and callbacks[0][1]==main_thread and callbacks[0][2:4]==(0,1) and callbacks[0][-1]==0,'EOR observes completed state on game thread with timer 1 paused')
    check(present.poll()==0 and len(callbacks)==1,'EOR delivered exactly once')
    out=C.create_string_buffer(64*64*4);check(copy(window,state.job,C.byref(e))==0 and read(window,out,len(out),C.byref(e))==0 and out.raw==bytes([0,255,0,255])*(64*64),'actual submitted image pixel oracle');images+=1;pixels+=4096
    prior=count(window);check(present.flip()==0,'actual logical-tick kmFlipFrameBuffer');flips+=1;state=present.snapshot()
    check(count(window)==prior+1 and state.presentations==1 and state.presented_tick>=state.required_tick>state.submitted_tick,'real Present at required future logical tick')
    check(live()==jobs(gpu.handle)==0 and not state.frame and not state.job and present.busy(2)==0 and present.busy(3)==1,'flip retires both owners and advances SDK slot')
    for lane in [1,2]:
        record(lane);rc=present.render();check(rc==0,('other actual list lane submission',lane,rc,present.status(),bind(lib,'jp_frames_status',I)(),bind(lib,'jp_queue_status',I)()));ready();state=present.snapshot();check(copy(window,state.job,C.byref(e))==0 and read(window,out,len(out),C.byref(e))==0 and out.raw==bytes([0,255,0,255])*4096,'other list actual GPU image');images+=1;pixels+=4096
        check(present.flip()==0,'other list real flip');flips+=1
    check(list(present.statistics().list_max)==[128,0,128,0,128],'typed opaque/translucent/punch list maxima map correctly')
    saved=Stats();C.memset(C.byref(saved),0x5a,C.sizeof(saved));old=bytes(saved)
    check(present.stats(C.byref(saved),C.sizeof(saved)-1)==1 and bytes(saved)==old,'short stats output rejected without write')
    check(present.shutdown()==0 and live()==jobs(gpu.handle)==0,'clean completed presenter shutdown')
    groups['actual_submission_callbacks_stats_three_lists_and_Present']=checks
    cfg=configure(off=True);record();check(present.render()==0,'Texture Off real native submit');ready();state=present.snapshot()
    check(copy(window,state.job,C.byref(e))==0 and read(window,out,len(out),C.byref(e))==0 and out.raw==bytes([255,0,0,255])*4096,'vertex-color native presentation path');images+=1;pixels+=4096
    check(present.flip()==0 and present.shutdown()==0,'Off real Present and cleanup');flips+=1
    # Actual original frame_wait now calls REAL render/flip functions.
    cfg=configure();record();prior=count(window);present.frame();state=present.snapshot()
    check(present.status()==0 and state.submissions==state.completions==state.presentations==1 and count(window)==prior+1,'complete original frame wait executes actual GPU and Present');flips+=1
    check([present.counter(i) for i in [4,5,6,7]]==[0,0,0,1] and live()==jobs(gpu.handle)==0,'original render counters and actual ownership retire')
    check(state.presented_tick>=state.required_tick and present.wait_status()==0,'original frame uses live logical callbacks')
    record();present.frame();state=present.snapshot();check(state.presentations==2 and state.submissions==2 and count(window)==prior+2,'second original frame completes with real presentation');flips+=1
    check(present.shutdown()==0,'original-frame pipeline clean shutdown')
    cfg=configure();present.probe(0,1);record();prior=count(window);present.frame()
    check(present.status()==0 and present.snapshot().submissions==0 and count(window)==prior and live()==0,'original draw-off resets real recording without fake render success');check(present.shutdown()==0,'drop path shutdown')
    groups['Texture_Off_and_complete_original_frame_path']=checks-sum(groups.values())
    # Unsupported captured video mode must remain an explicit backend failure.
    cfg=configure();check(sort(1)==0,'actual SDK autosort request');record();prior=count(window);present.frame();state=present.snapshot()
    check(present.status()==6 and state.frame and not state.job and count(window)==prior,'unsupported state retained for drain, no fabricated Present')
    check([present.counter(i) for i in [4,5,6,7]]==[11,22,33,0] and present.fits(0)==0,'original error guard stops counter reset/flip and split prediction')
    check(present.shutdown()==0 and live()==0 and sort(0)==0,'failed captured frame releases safely')
    cfg=configure();record();check(present.render()==0,'pending frame before sequence probe');old=present.snapshot()
    check(present.render()!=0 and present.status()==3 and present.snapshot().job==old.job,'second render rejects and preserves previous owned job')
    check(present.shutdown()==0 and live()==jobs(gpu.handle)==0,'failed sequence drains without callbacks')
    cfg=configure();foreign=[]
    def other_thread():foreign.extend([present.render(),present.shutdown(),present.valid()])
    thread=threading.Thread(target=other_thread);thread.start();thread.join();check(foreign[0]!=0 and foreign[1:]==[9,0] and present.status()==0,'foreign thread cannot mutate presenter')
    check(present.shutdown()==0,'foreign probe clean owner shutdown')
    # Controlled wall time with REAL GPU jobs verifies tick-phase ordering,
    # independently of nondeterministic hardware completion duration.
    counter=Counter();Idle=C.CFUNCTYPE(I,P,U);model_eor_ticks=[]
    @Idle
    def model_idle(context,ms):
        if ms:counter.tick+=(ms*781250+999)//1000;time.sleep(.0001)
        return present.window_idle(window,0)
    @Callback
    def model_done(argument):
        sample=WaitSnapshot();assert bind(present.lib,'jp_wait_snapshot',I,P)(C.byref(sample))==0;model_eor_ticks.append(sample.ticks)
    keep.extend([model_idle,model_done]);check(present.wait_shutdown()==0 and counter.start(clock,781250)==0,'controlled phase clock')
    check(present.wait_init(60000,1001,8,C.cast(model_idle,P),None)==0,'controlled tick host with real message pump')
    cfg=Config.make();check(present.init(gpu.handle,window,device,C.byref(cfg),60000,0)==0 and present.callback(C.cast(model_done,P),None)==0,'real presenter in phase-order model')
    present.probe(0,0);present.pacing(2,2);prior=count(window)
    for n in range(2):
        record();present.frame();state=present.snapshot();check(present.status()==0 and state.presentations==n+1,'modeled original frame uses real GPU/Present');flips+=1
        check(state.required_tick==model_eor_ticks[-1]+1,'exact next tick after observed readiness, no unconditional extra tick')
        check(present.counter(0)==2-(state.presented_tick-state.submitted_tick),'original countdown equals actual flip callback ticks')
        sample=WaitSnapshot();check(bind(present.lib,'jp_wait_snapshot',I,P)(C.byref(sample))==0 and sample.ticks==counter.tick*60000//(781250*1001),'independent rational tick oracle through actual render/flip')
    check(count(window)==prior+2 and present.shutdown()==0,'phase model presents and retires both frames')
    cfg=configure(timeout=1);record();check(present.render()==0,'real short-deadline job');prior=count(window)
    rc=present.flip();state=present.snapshot();check(rc!=0 and present.status()==8 and state.timeouts==1 and count(window)==prior,'logical/GPU flip deadline fails explicitly')
    check(present.shutdown()==0 and live()==jobs(gpu.handle)==0,'timeout retains then drains ownership')
    cfg=configure();record();check(present.render()==0,'real job before window close')
    user=C.WinDLL('user32');find=bind(user,'FindWindowW',P,C.c_wchar_p,C.c_wchar_p);pid=bind(user,'GetWindowThreadProcessId',U,P,P);send=bind(user,'SendMessageW',C.c_ssize_t,P,U,C.c_size_t,C.c_ssize_t)
    hwnd=find('SA_Native_Renderer_Research_Window',None);owner=U();check(hwnd and pid(hwnd,C.byref(owner)) and owner.value==os.getpid(),'own native window identified');send(hwnd,0x10,0,0);prior=count(window)
    check(present.flip()!=0 and present.status()==7 and count(window)==prior,'actual closed window cannot produce false flip success')
    check(present.shutdown()==0 and live()==jobs(gpu.handle)==0,'shutdown drains after normal window/tick host closes')
    wc(window);gpu.close(gpu.handle);present.wait_shutdown();qs();runtime.shutdown();check(fs()==0,'all real input and graphics owners clean')
    groups['unsupported_state_sequence_timeout_thread_and_close_failures']=checks-sum(groups.values())
    result=dict(status='real_native_Kamui_render_flip_and_original_frame_path_verified',checks=checks,groups=groups,actual_GPU_images_compared=images,backbuffer_pixels_compared=pixels,visible_Kamui_Presents=flips,
        same_exported_device_owner_used_by_SDK_and_presenter=True,typed_statistics_from_real_work=True,first_timing_sample_guard=True,real_EOR_callback_game_thread_once_verified=True,
        original_frame_wait_calls_real_render_flip=True,diagnostic_audio_bitmap_loader_remain_trace_providers=True,unsupported_state_no_fake_success=True,shutdown_after_window_close_drains=True,
        physical_scanout_or_original_hardware_slot_latency_verified=False,full_game_loop_executed=False,playable=False,overall_completion_percent=10,
        build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(Path(__file__)),support_sha256=sha(Path(__file__).parent/'native_present_test_support.py'))
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()

"""Real device delegation, staged shutdown, retry and process termination."""
import argparse,ctypes as C,hashlib,json,subprocess,sys,threading,time
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime
from native_gpu_test_support import Config,Failure,Report
from verify_native_device_init import Info as DeviceInfo,Options
from verify_native_exit_logo import ExitConfig,ExitInfo
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p;S=C.c_size_t
CASES=('tokens','complete','pending','external_frame','external_gpu_job','foreign','partial_adoption','cold_reject','immediate_exit','failure_exit')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--worker',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for k in ('frames','GPU','present','vertex_lifecycle','device','authored_exit'):assert sha(b[k+'_DLL'])==b[k+'_DLL_sha256']
    if not a.worker:
        assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build');d.mkdir();cases={}
        for name in CASES:
            child=d/name;child.mkdir();cmd=[sys.executable,'-B',str(Path(__file__).resolve()),'--workspace',str(r),'--build',str(a.build.resolve()),'--output',str(child),'--worker',name]
            run=subprocess.run(cmd,capture_output=True,creationflags=subprocess.CREATE_NO_WINDOW);(child/'stdout.txt').write_bytes(run.stdout);(child/'stderr.txt').write_bytes(run.stderr)
            assert run.returncode==(70 if name=='failure_exit' else 0) and not run.stderr,(name,run.returncode,run.stderr.decode()[-6000:])
            v=json.loads((child/'result.json').read_bytes());v['observed_process_exit_code']=run.returncode;cases[name]=v;print(name,'verified',flush=True)
        result=dict(status='real_device_single_exit_authority_quiesce_retry_and_process_termination_verified',checks=sum(x['checks'] for x in cases.values()),cases=cases,
            real_native_devices_used=True,wrong_owner_thread_and_token_rejected=True,ordinary_shutdown_blocked_after_claim=True,
            real_pending_GPU_drain_without_game_callbacks=True,external_frame_and_GPU_job_retirement_required=True,
            actual_partial_drain_adoption=True,actual_native_process_exit_codes_verified=True,original_game_or_SDK_functions_replaced=False,
            build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),full_system_boot_or_gameplay_verified=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True);return
    checks=[]
    def check(ok,label):assert ok,label;checks.append(label)
    def save(**values):(d/'result.json').write_text(json.dumps(dict(checks=len(checks),check_labels=checks,**values),indent=2)+'\n')
    t=Runtime(Path(b['frames_DLL']));fl=t.dll;gl=C.CDLL(b['GPU_DLL']);pl=C.CDLL(b['present_DLL']);C.CDLL(b['vertex_lifecycle_DLL']);dl=C.CDLL(b['device_DLL']);ex=C.CDLL(b['authored_exit_DLL'])
    init=bind(dl,'jp_device_initialize',I,I,I,I);shutdown=bind(dl,'jp_device_shutdown',I);snapshot=bind(dl,'jp_device_snapshot',I,P)
    claim=bind(dl,'jp_device_exit_claim',I,P);quiesce=bind(dl,'jp_device_exit_quiesce',I,Q);finish=bind(dl,'jp_device_exit_finish',I,Q)
    exit_bind=bind(ex,'jp_exit_bind',I,P);clean=bind(ex,'jp_exit_cleanup',I);exit_snapshot=bind(ex,'jp_exit_snapshot',I,P);exit_status=bind(ex,'jp_exit_status',I);boot_exit=bind(ex,'syBtExit',None)
    def device():v=DeviceInfo();assert snapshot(C.byref(v))==0;return v
    def state():v=ExitInfo();assert exit_snapshot(C.byref(v))==0;return v
    cfg=ExitConfig(C.sizeof(ExitConfig),16384,None,None)
    if a.worker=='cold_reject':
        token=Q(0x1234567887654321);check(claim(C.byref(token))==2 and token.value==0x1234567887654321,'cold device cannot issue a cleanup token')
        check(exit_bind(C.byref(cfg))==7 and exit_status()==2 and clean()==2 and device().resources==0,'unavailable device rejection leaves exit unbound and no fabricated resources');save(cold_device_rejected=True);return
    check(bind(fl,'jp_queue_initialize',I,S)(8<<20)==0 and bind(fl,'jp_frames_initialize',I,U,U,S)(8,128,32<<20)==0 and t.init(32,1024,32<<20)==0 and bind(fl,'jp_render_state_initialize',I,U,S)(128,1<<20)==0,'actual borrowed graphics owners')
    options=Options(640,480,0,0,5000,0,64<<20);check(bind(dl,'jp_device_configure',I,P)(C.byref(options))==0 and init(1,0,1)==0,'actual native device initialization')
    gpu=P(bind(dl,'jp_device_gpu',P)());window=P(bind(dl,'jp_device_window',P)());hwnd=bind(gl,'jp_window_native_handle',P,P)(window);user=C.WinDLL('user32');is_window=bind(user,'IsWindow',I,P)
    check(gpu and window and hwnd and is_window(hwnd),'real GPU/window handles')
    if a.worker=='tokens':
        before=bytes(device());token=Q(0x1234567887654321)
        check(claim(None)==1 and bytes(device())==before,'null token output cannot claim resources')
        check(claim(C.byref(token))==0 and token.value,'actual quiescent authority claim');bad=Q(0xaabbccdd)
        check(claim(C.byref(bad))==2 and bad.value==0xaabbccdd,'duplicate claim preserves output')
        check(quiesce(0)==2 and finish(token.value+1)==2 and quiesce(token.value|(1<<32))==2 and bytes(device())==before,'wrong full-width token cannot change native ownership')
        check(shutdown()==2 and init(1,0,1)==2 and bind(dl,'jp_device_configure',I,P)(C.byref(options))==2,'ordinary APIs cannot replace delegated authority')
        check(quiesce(token.value)==0 and device().phase==5 and device().resources==29,'quiesce drains presenter and stops timing but retains final device owners')
        check(bind(pl,'jp_present_status',I)()==bind(pl,'jp_wait_status',I)()==2 and bind(fl,'jp_clock_status',I)()==0 and bind(fl,'jp_video_status',I)()==0 and is_window(hwnd),'window/video/clock remain valid until dependent cleanup')
        check(quiesce(token.value)==0 and finish(token.value)==0 and device().phase==device().resources==0 and not is_window(hwnd),'same authority completes real native window/GPU/timing shutdown')
        check(quiesce(token.value)==finish(token.value)==0 and shutdown()==2 and init(1,0,1)==2,'completed delegation is terminal and idempotent')
        foreign=[];thread=threading.Thread(target=lambda:foreign.extend([quiesce(token.value),finish(token.value),shutdown()]));thread.start();thread.join();check(foreign==[3,3,3],'creator-thread authority persists after completed shutdown')
        save(token_issued=True,completed_resources=16384);return
    # Rejected combinations must leave the device available for the subsequent
    # genuine claim. ABI/ALL remain compatible with existing classic callers.
    bad=ExitConfig(C.sizeof(ExitConfig),16384|8192,gpu,None)
    check(exit_bind(C.byref(bad))==1 and exit_status()==2 and device().resources==63,'duplicate raw GPU/device authority rejected atomically')
    bad=ExitConfig(C.sizeof(ExitConfig),16384|8,None,None)
    check(exit_bind(C.byref(bad))==1 and exit_status()==2 and device().resources==63,'duplicate wait/device authority rejected atomically')
    frame=Q();job=Q();font=False
    def record():
        nonlocal font
        bind(fl,'njInitPrint',None,P,U,U)(None,0,0);bind(fl,'text_style_init',None,P,U,U)(None,0,0);I.in_dll(fl,'frame_step').value=0
        bind(fl,'text_print',None,I,P)(0x10001,C.create_string_buffer(b'DEVICE EXIT OWNERSHIP'));font=True
        check(bind(fl,'jp_print_status',I)()==0,'actual original game/Ninja text geometry')
    if a.worker=='partial_adoption':
        record();check(bind(fl,'jp_frames_submit',I,P)(C.byref(frame))==0,'actual retained frame before partial normal shutdown')
        check(shutdown()==13 and device().phase==5 and device().resources==31,'ordinary partial drain retains real dependent owners before exit quiesces timing')
    check(exit_bind(C.byref(cfg))==0 and exit_status()==0 and state().remaining==16384,'one device cleanup authority accepted')
    check(exit_bind(C.byref(cfg))==5 and shutdown()==2,'rebinding and independent device close rejected after transfer')
    if a.worker in ('pending','external_frame','external_gpu_job','failure_exit'):
        record()
        if a.worker=='pending':
            I.in_dll(pl,'_nj_eor_flag_').value=42;I.in_dll(pl,'_nj_render_flag_').value=0
            check(bind(pl,'kmRender',U)()==0 and bind(gl,'jp_gpu_jobs_live',U,P)(gpu)==1,'real pending GPU work before exit')
        else:
            check(bind(fl,'jp_frames_submit',I,P)(C.byref(frame))==0 and frame.value,'actual retained frame outside the presenter')
            if a.worker=='external_gpu_job':
                config=Config.make(640,480);config.logical_width=640;config.logical_height=480;report=Report();error=Failure()
                check(bind(gl,'jp_gpu_submit_frame',I,P,Q,P,P,P,P)(gpu,frame.value,C.byref(config),C.byref(job),C.byref(report),C.byref(error))==0,'actual independent GPU job retains its own payload')
                check(bind(fl,'jp_frame_release',I,Q)(frame.value)==0,'independent job survives original CPU frame retirement');frame.value=0
    if a.worker=='foreign':
        foreign=[];thread=threading.Thread(target=lambda:foreign.extend([clean(),shutdown(),quiesce(1),finish(1)]));thread.start();thread.join()
        check(foreign==[3,3,3,3] and state().remaining==16384 and device().resources==63,'foreign thread cannot invoke either cleanup authority')
    if a.worker=='immediate_exit':save(actual_syBtExit_entered=True);boot_exit();raise AssertionError('native process exit returned')
    if a.worker in ('external_frame','external_gpu_job','partial_adoption','failure_exit'):
        check(clean()==6 and state().failed_resource==16384 and state().native_error==13 and state().remaining==16384 and state().completed==0,'actual external owner prevents final device shutdown without losing cleanup authority')
        check(device().phase==5 and device().resources==29 and is_window(hwnd),'failed final drain retains native window/GPU/video/clock')
        if a.worker=='failure_exit':save(actual_syBtExit_entered=True,retained_frame=int(frame.value),expected_failure_exit_code=70);boot_exit();raise AssertionError('failed native process exit returned')
        if frame.value:check(bind(fl,'jp_frame_release',I,Q)(frame.value)==0,'actual external frame released for retry')
        if job.value:
            from verify_native_gpu_jobs import Timing
            timing=Timing();error=Failure();deadline=time.monotonic()+10;poll=bind(gl,'jp_gpu_job_poll',I,P,Q,P,P)
            while True:
                rc=poll(gpu,job.value,C.byref(timing),C.byref(error))
                if rc==0:break
                assert rc==100 and time.monotonic()<deadline,(rc,error.code);time.sleep(.001)
            check(bind(gl,'jp_gpu_job_release',I,P,Q,P)(gpu,job.value,C.byref(error))==0,'actual independent GPU completion observed and ownership released for retry')
    check(clean()==0 and state().remaining==0 and state().completed==16384 and device().phase==device().resources==0,'one terminal exit authority completes actual device shutdown')
    check(not is_window(hwnd) and bind(fl,'jp_clock_status',I)()==4 and bind(fl,'jp_video_status',I)()==3 and bind(pl,'jp_wait_status',I)()==2,'actual native window and all device timing/video services closed')
    if a.worker=='pending':check(I.in_dll(pl,'_nj_eor_flag_').value==42 and I.in_dll(pl,'_nj_render_flag_').value==0,'pending shutdown drains real GPU without dispatching game EOR callbacks')
    check(clean()==0 and state().completed==16384 and shutdown()==2,'repeat exit cannot close delegated resources twice')
    if font:bind(fl,'text_exit',None)();bind(fl,'njExitPrint',None)()
    bind(fl,'jp_queue_shutdown',None)();t.shutdown();check(bind(fl,'jp_frames_shutdown',I)()==0,'independently borrowed graphics owners remain caller-owned and drainable')
    save(completed_resources=16384,actual_device_exit=True)
if __name__=='__main__':main()

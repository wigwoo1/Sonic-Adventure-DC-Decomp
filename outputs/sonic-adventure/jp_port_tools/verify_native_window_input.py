"""Actual renderer-owned HWND, stock Windows polling and original frame waits.
No global keyboard events are injected. Controlled samples exercise the same
real provider and game scan; they do not prove physical controller events.
"""
import argparse,ctypes as C,hashlib,json,threading
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime
from native_gpu_test_support import GPU,Config,Failure
from native_video_test_support import Video
from native_present_test_support import Present
from native_clock_test_support import Counter
from verify_native_input_backend import Sample,Source,Peripheral,Info
P=C.c_void_p;I=C.c_int;U=C.c_uint32;S=C.c_size_t
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();assert not a.report.exists()
    b=json.loads((d/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(d/n)==h,n
    for n in ['DLL','GPU_DLL','frames_DLL']:assert sha(b[n])==b[n+'_sha256']
    runtime=Runtime(Path(b['frames_DLL']));lib=runtime.dll;gpu=GPU(Path(b['GPU_DLL']));gl=gpu.lib;present=Present(Path(b['DLL']));pl=present.lib
    checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);fi=bind(lib,'jp_frames_initialize',I,U,U,S)
    check(qi(4<<20)==0 and fi(8,8,4<<20)==0 and runtime.init(8,128,1<<20)==0 and Video(lib).init(64,64,0xff123456,1)==0,'shared renderer owners')
    open_window=bind(gl,'jp_window_open',I,P,U,U,I,P,P);close_window=bind(gl,'jp_window_close',None,P)
    native=bind(gl,'jp_window_native_handle',P,P);pump=bind(gl,'jp_window_pump',I,P)
    window=P();failure=Failure();check(open_window(gpu.handle,64,64,0,C.byref(window),C.byref(failure))==0,'real hidden renderer/DXGI window')
    hwnd=native(window);check(bool(hwnd) and hwnd!=window.value and native(None) is None,'borrow actual HWND, opaque renderer pointer differs')
    user=C.WinDLL('user32');kernel=C.WinDLL('kernel32');user.IsWindow.argtypes=[P];user.IsWindow.restype=I
    user.GetWindowThreadProcessId.argtypes=[P,C.POINTER(U)];user.GetWindowThreadProcessId.restype=U
    kernel.GetCurrentThreadId.restype=U;kernel.GetCurrentProcessId.restype=U
    pid=U();check(user.IsWindow(hwnd)==1 and user.GetWindowThreadProcessId(hwnd,C.byref(pid))==kernel.GetCurrentThreadId() and pid.value==kernel.GetCurrentProcessId(),'native HWND belongs to actual game thread/process')
    foreign=[];thread=threading.Thread(target=lambda:foreign.append(native(window)));thread.start();thread.join();check(foreign==[None],'foreign thread cannot borrow handle')
    init=bind(lib,'jp_input_initialize',I,P,U,P,P);shutdown=bind(lib,'jp_input_shutdown',I);info=bind(lib,'jp_input_snapshot',I,P)
    status=bind(lib,'jp_input_status',I);record=bind(lib,'jp_input_record',P,U);clear=bind(lib,'jp_input_clear_error',None)
    check(init(hwnd,1,None,None)==0,'actual stock Windows collector accepts renderer-owned HWND')
    check(C.cast(bind(lib,'pdExecPeripheralServer',None),P).value==C.cast(bind(pl,'pdExecPeripheralServer',None),P).value,'SDK forward export identifies single actual provider')
    counter=Counter();check(counter.start(present.clock,781250)==0,'single deterministic clock for frame boundaries')
    Idle=C.CFUNCTYPE(I,P,U)
    @Idle
    def idle(context,ms):counter.tick+=max(1,ms*782);return 1
    check(present.wait_init(60000,1001,8,C.cast(idle,P),None)==0,'actual native wait core')
    device=C.addressof((C.c_byte*1).in_dll(lib,'kmiDeviceExtension'));present.device(device);cfg=Config.make(64,64)
    check(present.init(gpu.handle,window,device,C.byref(cfg),5000,0)==0,'actual presenter borrows same window/SDK state')
    step=bind(pl,'frame_wait_step',None,I);scan=bind(lib,'pad_scan',None)
    count=bind(pl,'jp_wait_probe_events',U);event=bind(pl,'jp_wait_probe_event',U,U)
    def events():return [event(i) for i in range(count())]
    present.probe(0,1);present.pacing(0,1)
    def polls():
        s=Info();check(info(C.byref(s))==0,'input snapshot');return s.polls,s.focused
    for case in range(64):
        before,_=polls();present.clear();step(0);after,focus=polls()
        check(after==before+1 and status()==0 and focus==0,'real stock Windows poll from original frame_wait_step')
        check(events()==[12,13,14],'passive trace follows actual polling before overlay/sound')
        scan();q=Peripheral.from_address(record(0));check(q.on==0 and q.press==0,'unfocused real stock keyboard stays neutral')
        before,_=polls();present.clear();present.frame();after,_=polls()
        check(after==before+1 and status()==0 and events()[-2:]==[12,13],'complete original frame_wait polls actual input')
    check(shutdown()==0,'stock collector drained before provider change')
    samples=(Sample*4)();source_ok=True;errors=[]
    @Source
    def source(context,out):
        try:
            if not source_ok:return 0
            C.memmove(out,samples,C.sizeof(samples));return 1
        except BaseException as error:errors.append(str(error));return 0
    check(init(hwnd,0,C.cast(source,P),None)==0,'controlled fixture uses same actual provider and owner')
    records=(Peripheral*8).in_dll(lib,'pad_records')
    for case in range(128):
        samples[0]=Sample(0,0,0,0,0,0,0,0)
        samples[1]=Sample(1,4 if case%2 else 8,case%128,-(case%128),0,0,case%256,255-case%256)
        before,_=polls();present.clear();step(0);scan();after,_=polls()
        check(after==before+1 and not errors and records[0].on==samples[1].buttons,'frame polling feeds complete original scan with actual port compaction')
        check(records[0].x1==samples[1].x1 and records[0].y1==samples[1].y1 and records[0].l==samples[1].left and records[0].r==samples[1].right,'whole-frame axes/triggers reach game state')
        check(events()==[12,13,14],'successful controlled poll reaches remaining frame services')
    source_ok=False;present.clear();step(0)
    check(status()==5 and events()==[12],'failed poll stops frame_wait_step before overlay/sound')
    clear();present.clear();present.frame();check(status()==5 and events()[-1:]==[12] and 13 not in events(),'failed poll stops complete frame_wait before overlay')
    source_ok=True;clear();present.clear();step(0);check(status()==0 and events()==[12,13,14],'explicit input recovery resumes frame service')
    check(shutdown()==0 and present.shutdown()==0 and present.wait_shutdown()==0,'input/presenter/wait drain before closing window')
    present.clock.shutdown()
    user.SendMessageW.argtypes=[P,U,C.c_size_t,C.c_ssize_t];user.SendMessageW.restype=C.c_ssize_t
    user.SendMessageW(hwnd,0x10,0,0)
    check(native(window) is None and pump(window)==0 and not user.IsWindow(hwnd),'closed-but-owned window cannot expose stale HWND')
    close_window(window)
    second=P();check(open_window(gpu.handle,64,64,0,C.byref(second),C.byref(failure))==0 and bool(native(second)),'new window on preserved device')
    gpu.close(gpu.handle);check(native(second) is None and pump(second)==0,'closing GPU cannot expose HWND while deferred window reference remains')
    close_window(second);gpu.handle=P()
    check(bind(lib,'jp_frames_shutdown',I)()==0,'frame ownership drained');runtime.shutdown()
    check(runtime.allocated()==0 and runtime.live()==0,'texture ownership drained');bind(lib,'jp_queue_shutdown',None)()
    report=dict(status='renderer_owned_HWND_real_Windows_polling_and_original_frame_wait_input_verified',checks=checks,stock_Windows_frame_polls=128,controlled_frame_polls=129,failed_frame_polls=2,complete_original_frame_wait_calls=65,original_frame_wait_step_calls=194,full_pad_scans=192,shared_input_export_identity_verified=True,real_renderer_HWND_verified=True,foreign_thread_closed_window_and_closing_GPU_rejected=True,input_failure_stops_frame_services=True,physical_focused_keyboard_or_controller_events_verified=False,global_keyboard_events_injected=False,frame_wait_input_trace_replaced=True,remaining_audio_loader_overlay_traces=True,original_logo_step_executed=False,build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
